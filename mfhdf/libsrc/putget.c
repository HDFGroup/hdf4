/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	<string.h>
#include	"local_nc.h"
#include	"alloc.h"

/* Local function prototypes */
static bool_t hdf_xdr_NCvdata
    PROTO((NC *handle,NC_var *vp,u_long where,nc_type type,uint32 count,
        VOIDP values));
static bool_t hdf_xdr_NCv1data
    PROTO((NC *handle,NC_var *vp,u_long where,nc_type type,VOIDP values));
static int32 hdf_get_vp_aid
    PROTO((NC *handle, NC_var *vp));

/*
 * If you use ./xdrstdio.c rather than ./xdrposix.c as
 * your bottom layer, the you may need to #define XDRSTDIO
 */

#define MIN(mm,nn) (((mm) < (nn)) ? (mm) : (nn))
#define MAX(mm,nn) (((mm) > (nn)) ? (mm) : (nn))

/* #define VDEBUG */
#ifdef VDEBUG
/*
 * Print the values of an array of longs
 */
arrayp(label, count, array)
const char *label ;
unsigned count ;
const long array[] ;
{
	fprintf(stderr, "%s", label) ;
	fputc('\t',stderr) ;	
	for(; count > 0 ; count--, array++)
		fprintf(stderr," %ld", *array) ;
	fputc('\n',stderr) ;	
}
#endif /* VDEBUG */

#define xdr_NCsetpos(xdrs, pos) xdr_setpos((xdrs),(pos))

/*
 * At the current position, add a record containing the fill values.
 */
static bool_t
NCfillrecord(xdrs, vpp, numvars)
XDR *xdrs ;
NC_var **vpp ;
unsigned numvars ;
{
	int ii ;

	for(ii = 0 ; ii < numvars ; ii++, vpp++)
	{
		if( !IS_RECVAR(*vpp) )
		{
			continue ;	/* skip non-record variables */
		}

		/* compute sub size */
		if( !xdr_NC_fill(xdrs, *vpp) )
			return(FALSE) ;
	}
	return(TRUE) ;
}


/* 
 * Check whether coords are valid for the variable.
 * For 'record' variables add and fill records out
 * to the requested recno == coords[0] as a side effect.
 */
bool_t
NCcoordck(handle, vp, coords)
NC *handle ;
NC_var *vp ; 
const long *coords ;
{
	const long *ip ;
	unsigned long *up ;
	const long *boundary ;
	long unfilled ;

	if( IS_RECVAR(vp) )
	{
		boundary = coords + 1 ;
		if(*coords < 0)
			goto bad ;
	} else
		boundary = coords ;
	
	up = vp->shape + vp->assoc->count - 1 ;
	ip = coords + vp->assoc->count - 1 ;
#ifdef CDEBUG
	fprintf(stderr,"	NCcoordck: coords %ld, count %ld, ip %d\n",
		coords, vp->assoc->count, ip ) ;
#endif /* CDEBUG */
	for( ; ip >= boundary ; ip--, up--)
	{
#ifdef CDEBUG
		fprintf(stderr,"	NCcoordck: ip %p, *ip %ld, up %p, *up %lu\n",
			ip, *ip, up, *up ) ;
#endif /* CDEBUG */
		if( *ip < 0 || *ip >= *up )
			goto bad ;
	}


#ifdef HDF
        if(handle->file_type == HDF_FILE && IS_RECVAR(vp)) {
            Void *strg, *strg1;
            NC_attr ** attr = NULL;
            int count, byte_count;
	    int len;
            
            if((unfilled = *ip - vp->numrecs) <= 0) return TRUE;
            
            /* check to see if we are trying to read beyond the end */
            if(handle->xdrs->x_op != XDR_ENCODE)
                goto bad ;
            /* else */

            /* make sure we can write to this sucker */
            if(vp->aid == FAIL && hdf_get_vp_aid(handle, vp) == FALSE) return(FALSE);
            
            /*
             * Set up the array strg to hold the fill values
             */
	    len = (vp->len / vp->HDFsize) * vp->szof;
            strg = (Void *) HDgetspace(len);
            strg1 = (Void *) HDgetspace(len);
            attr = NC_findattr(&vp->attrs, _FillValue);

            if(attr != NULL)
                hdf_fill_array(strg, len, (*attr)->data->values, vp->type);
            else 
                NC_arrayfill(strg, len, vp->type);

#ifdef DEBUG
            fprintf(stderr, "Going to fill in record %d for variable %s\n", *ip,
                    vp->name->values);
#endif

#ifdef OLD_WAY
            count = vp->dsizes[0] / NC_typelen(vp->type);
#endif

            /*
             * Seek to correct location
             */
            byte_count = vp->len;
	    count = byte_count / vp->HDFsize;

            Hseek(vp->aid, (vp->numrecs + 1) * byte_count, DF_START);

#ifdef DEBUG
            printf("Filling %d bytes starting at %d\n", 
                   byte_count * unfilled, (vp->numrecs + 1) * byte_count);
#endif  

            /*
             * Write out the values
             */

            DFKsetNT(vp->HDFtype);
            DFKnumout(strg, strg1, count, 0, 0);

            for(; unfilled; unfilled--)
                Hwrite(vp->aid, byte_count, (uint8 *) strg1);

#ifdef DEBUG
            fprintf(stderr, "WROTE %d values at location %d (numrecs = %d)\n",
                    count,
                    *ip * count,
                    vp->numrecs);
#endif

            vp->numrecs = MAX(vp->numrecs, *ip);
            if((*ip + 1) > handle->numrecs) {
                handle->numrecs = *ip + 1;
                handle->flags |= NC_NDIRTY;
            }
            HDfreespace((VOIDP)strg);
            HDfreespace((VOIDP)strg1);

            return (TRUE);
        }
#endif /* HDF */


	if( IS_RECVAR(vp) && (unfilled = *ip - handle->numrecs) >= 0 )
	{
		/* check to see if we are trying to read beyond the end */
		if(handle->xdrs->x_op != XDR_ENCODE)
			goto bad ;
		/* else */

		handle->flags |= NC_NDIRTY ;
		if( handle->flags & NC_NOFILL )
		{
			/* Go directly to jail, do not pass go */
			handle->numrecs = *ip + 1 ;
		}
		else
		{
			/* fill out new records */
			if( !xdr_NCsetpos(handle->xdrs,
					handle->begin_rec + handle->recsize*handle->numrecs) )
			{
				nc_serror("NCcoordck seek, var %s", vp->name->values) ;
				return(FALSE) ;
			}
			for( ; unfilled >= 0 ; unfilled--, handle->numrecs++)
			{
				if( !NCfillrecord(handle->xdrs,
						(NC_var **)handle->vars->values, handle->vars->count) )
				{
					nc_serror("NCcoordck fill, var %s, rec %ld",
						vp->name->values, handle->numrecs) ;
					return(FALSE) ;
				}
			}
		}
		if(handle->flags & NC_NSYNC) /* write out header->numrecs NOW */
		{
			if(!xdr_numrecs(handle->xdrs, handle) )
				return(FALSE) ;
			handle->flags &= ~NC_NDIRTY ;
		}
	}

	return(TRUE) ;
bad:
#ifdef VDEBUG
	arrayp("\t\tcoords", vp->assoc->count, coords) ;
	arrayp("\t\tmax", vp->assoc->count, vp->shape) ;
#endif /* VDEBUG */
	NCadvise(NC_EINVALCOORDS, "%s: Invalid Coordinates", vp->name->values) ;
	return(FALSE) ;
}


/* 
 * Translate the (variable, coords) pair into a seek index
 */
static u_long
NC_varoffset(handle, vp, coords)
NC *handle ;
NC_var *vp ; 
const long *coords ;
{
	u_long offset ;
	const long *ip  ;
	unsigned long *up ;
	const long *boundary ;

	if(vp->assoc->count == 0) /* 'scaler' variable */
		return(vp->begin) ;

	if( IS_RECVAR(vp) )
		boundary = coords + 1 ;
	else
		boundary = coords ;
	
	up = vp->dsizes + vp->assoc->count - 1 ;
	ip = coords + vp->assoc->count - 1 ;
	for(offset = 0 ; ip >= boundary ; ip--, up--)
		offset += *up * *ip ;

	if( IS_RECVAR(vp) )
	{
            switch(handle->file_type) {
            case HDF_FILE:
                return( vp->begin + vp->dsizes[0] * *coords + offset) ;
                break;
            case netCDF_FILE:
                return( vp->begin + handle->recsize * *coords + offset) ;
                break;
            }
	}
	/* else */
	return( vp->begin + offset );
}


/*
 * xdr 1 - 3 bytes, leaving adjoining bytes within the word ok.
 * (minimum unit of io is 4 bytes)
 */
static bool_t
xdr_NCvbyte(xdrs, rem, count, values)
XDR *xdrs ;
unsigned rem ;
unsigned count ;
char *values ;
{
	char buf[4] ;
	u_long origin ;
        enum xdr_op  x_op = xdrs->x_op ; /* save state */

	if(x_op == XDR_ENCODE)
	{
	/*
	 * Since we only read/write multiples of four bytes,
	 * We will read in the word to change one byte in it.
	 */
		origin = xdr_getpos( xdrs ) ;
#ifdef XDRSTDIO
	/*
	 * N.B. : "a file positioning function must be called between
	 * a write and a read or vice versa"
	 *		- limitations of stdio, open for update
	 */
		if( !xdr_setpos(xdrs, origin) ) 
			return(FALSE) ;
#endif /* XDRSTDIO */
                /* next op is a get */
                xdrs->x_op = XDR_DECODE ;
        }

        if(!xdr_opaque(xdrs, buf, 4))
        {
                /* get failed, assume we are trying to read off the end */
#ifdef XDRSTDIO
		/*
		 * N.B. 2 : Violates layering,
		 * assumes stdio under xdr.
		 * This clause could be safely replaced with
		 * just the 'memset' line.
		 */
		if(feof((FILE*)xdrs->x_private)) /* NC_NOFILL */
		{
			/* failed because we tried to read
			 * beyond EOF
			 */
			clearerr((FILE*)xdrs->x_private) ;
			(void)HDmemset(buf, 0, sizeof(buf)) ;
		}
		else
		{
			NCadvise(NC_EXDR, "xdr_NCvbyte") ;
			xdrs->x_op = x_op ;
                        return(FALSE) ;
		}
#else
		(void)HDmemset(buf, 0, sizeof(buf)) ;
#endif /* XDRSTDIO */
	}

       if(x_op == XDR_ENCODE) /* back to encode */
                xdrs->x_op = x_op ;

	while(count-- != 0)
	{
		if(x_op == XDR_ENCODE)
			buf[rem] = *values ;
		else
			*values = buf[rem] ;
	
		rem++ ;
		values++ ;
	}

	if(x_op == XDR_ENCODE)
	{
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
		if( !xdr_opaque(xdrs, buf, 4))
			return(FALSE) ;
	}

	return(TRUE) ;
}


/*
 * xdr a short leaving adjoining short within the word ok.
 * (minimum unit of io is 4 bytes)
 */
bool_t
xdr_NCvshort(xdrs, which, values)
XDR *xdrs ;
unsigned which ;
short *values ;
{
        unsigned char buf[4] ; /* unsigned is important here */
        u_long origin ;
        enum xdr_op  x_op = xdrs->x_op ; /* save state */

        if(x_op == XDR_ENCODE)
        {
                origin = xdr_getpos( xdrs ) ;
#ifdef XDRSTDIO
                /* See N.B. above */
                if( !xdr_setpos(xdrs, origin) )
                        return(FALSE) ;
#endif /* XDRSTDIO */
                /* next op is a get */
                xdrs->x_op = XDR_DECODE ;
        }

        if(!xdr_opaque(xdrs, (caddr_t)buf, 4))
        {
                /* get failed, assume we are trying to read off the end */
#ifdef XDRSTDIO
                /* See N.B. 2 above */
                if(feof((FILE*)xdrs->x_private)) /* NC_NOFILL */
                {
                        /* failed because we tried to read
                         * beyond EOF
                         */
                        clearerr((FILE*)xdrs->x_private) ;
                        (void)memset(buf, 0, sizeof(buf)) ;
                }
                else
                {
                        NCadvise(NC_EXDR, "xdr_NCvbyte") ;
                        xdrs->x_op = x_op ;
                        return(FALSE) ;
                }
#else
                (void)HDmemset(buf, 0, sizeof(buf)) ;
#endif /* XDRSTDIO */
        }

        if(x_op == XDR_ENCODE) /* back to encode */
                xdrs->x_op = x_op ;
 
        if(which != 0) which = 2 ;

        if(xdrs->x_op == XDR_ENCODE)
        {
                buf[which +1] = *values % 256 ;
                buf[which] = (*values >> 8) ;

                if( !xdr_setpos(xdrs, origin) )
                        return(FALSE) ;
                if( !xdr_opaque(xdrs, (caddr_t)buf, 4))
                        return(FALSE) ;
        }
        else
        {
                *values = ((buf[which] & 0x7f) << 8) + buf[which + 1] ;
                if(buf[which] & 0x80)
                {
                        /* extern is neg */
                       *values -= 0x8000 ;
                 }
        }
        return(TRUE) ;
}


/*
 * xdr a single datum of type 'type' at 'where'
 */
static bool_t 
xdr_NCv1data(xdrs, where, type, values)
XDR *xdrs ;
u_long where ;
nc_type type ;
Void *values ;
{
	u_long rem ;

	switch(type){
	case NC_BYTE :
	case NC_CHAR :
	case NC_SHORT :
		rem = where%4 ;
		where -= rem ; /* round down to nearest word */
		break ;
	}
	if( !xdr_NCsetpos(xdrs, where) )
		return(FALSE) ;

	switch(type){
	case NC_BYTE :
	case NC_CHAR :
		return( xdr_NCvbyte(xdrs, (unsigned)rem, (unsigned)1,
			(char *)values) ) ;
	case NC_SHORT :
		return( xdr_NCvshort(xdrs, (unsigned)rem/2, (short *)values) ) ;
	case NC_LONG :
		return( xdr_long(xdrs, (long *)values) ) ;
	case NC_FLOAT :
		return( xdr_float(xdrs, (float *)values) ) ;
	case NC_DOUBLE : 
		return( xdr_double(xdrs, (double *)values) ) ;
	}
	return(FALSE) ;
}

#ifdef HDF

/*****************************************************************************
* 
*			NCSA HDF / netCDF Project
*			       May, 1993
*
* NCSA HDF / netCDF source code and documentation are in the public domain.  
* Specifically, we give to the public domain all rights for future
* licensing of the source code, all resale rights, and all publishing rights.
* 
* We ask, but do not require, that the following message be included in all
* derived works:
* 
* Portions developed at the National Center for Supercomputing Applications at
* the University of Illinois at Urbana-Champaign.  Funding for this project 
* has come primarily from the National Science Foundation.
* 
* THE UNIVERSITY OF ILLINOIS GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR THE
* SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
* WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
* 
******************************************************************************
*
* Please report all bugs / comments to chouck@ncsa.uiuc.edu
*
*****************************************************************************/


PRIVATE int32 tBuf_size = 0;
PRIVATE int8  *tBuf = NULL;

/* ---------------------------- hdf_fill_array ---------------------------- */
/*
  Fill the array pointed to by storage with the value pointed to
  by value.  It is assumed that the filling is being done on
  behalf of variable vp which is used to compute the number of 
  fill values to generate.
*/
int 
    hdf_fill_array(storage, len, value, type)
Void  * storage;
Void  * value;
int32   len;
int32   type;
{
    register uint8 *lo, *hi;
    
    lo = (uint8 *) storage;
    hi = (uint8 *) storage + len;

    switch(type) {
    case NC_BYTE    :
    case NC_CHAR    :
        HDmemset(lo, *((char *)value), len);
        break;
    case NC_SHORT    :
        {
            int16 t = *((int16 *) value);
            while(lo < hi) {
                *((int16 *) lo) = t;
                lo += sizeof(int16);
            }
        }
        break;
    case NC_LONG     :
        {
            int32 t =  *((int32 *) value);
            while(lo < hi) {
                *((int32 *) lo) = t;
                lo += sizeof(int32);
            }
        }
        break;
    case NC_FLOAT    :
        {
            float t = *((float *) value);
            while(lo < hi) {
                *((float *) lo) = t;
                lo += sizeof(float);
            }
        }
        break;
    case NC_DOUBLE   :
        {
            double t = *((double *) value);
            while(lo < hi) {
                *((double *) lo) = t;
                lo += sizeof(double);
            }
        }
        break;
    }
    
} /* hdf_fill_array */

#define MAX_SIZE 1000000

/* ------------------------- hdf_get_data ------------------- */
/*
 * Given a variable vgid return the id of a valid data storage
 * Create and fill in the VS as a side effect if it doesn't
 *  exist yet
 */
int 
  hdf_get_data(handle, vp)
NC *handle;
NC_var *vp;
{
    NC_attr **attr;
    int32 vg;
    int32 vsid, nvalues, status, tag, t, n;
    register Void *values;
    int32 byte_count, len;
    int32 to_do, done, chunk_size;
    
#if DEBUG 
    fprintf(stderr, "hdf_get_data I've been called\n");
#endif
    
    if(!handle) return NULL;
    if(!vp) return NULL;

    /* 
     * if it is stored as NDGs we can't do any better than what was
     *    originally stored in vp->data_ref
     */
    if(vp->data_tag == DFTAG_SDS) return vp->data_ref;
    
    if(vp->vgid) {
        /* attach to the variable's Vgroup */
        vg = Vattach(handle->hdf_file, vp->vgid, "w");
        if(vg == FAIL) {
            HEprint(stderr, 0);
            return NULL;
        }
        
        /* loop through looking for a data storage object */
        n = Vntagrefs(vg);
        for(t = 0; t < n; t++) {
            Vgettagref(vg, t, &tag, &vsid);
            if(tag == DATA_TAG) return vsid;
        }
    }
    
    if(handle->hdf_mode == DFACC_RDONLY)
        return NULL;
  
    /* 
     * create a new data storage object
     */
#ifdef DEBUG
    fprintf(stderr, "--- Creating new data storage (len = %d) --- \n", vp->len);
    fprintf(stderr, "shape[0]= %d shape[1]= %d\n", vp->shape[0], vp->shape[1]);
    fprintf(stderr, "dsize[0]= %d dsize[1]= %d\n", vp->dsizes[0], vp->dsizes[1]);
#endif  
    
    /* look up fill value (if it exists) */
    attr = NC_findattr(&(vp->attrs), _FillValue);


    /* compute the various size parameters */
    if(vp->len > MAX_SIZE)
        chunk_size = MAX_SIZE;
    else
        chunk_size = vp->len;

    nvalues = vp->len / vp->HDFsize;        /* total number of values */
    to_do   = chunk_size / vp->HDFsize;     /* number of values in a chunk */
    
    len = to_do * vp->szof;                 /* size of buffer for fill values */
    values = (Void *) HDgetspace(len);      /* buffer to hold unconv fill vals */
    byte_count = to_do * vp->HDFsize;       /* external buffer size */

    if(!attr) {
        NC_arrayfill(values, len, vp->type);
    } else {
        hdf_fill_array(values, len, (*attr)->data->values, vp->type);
    }
    
    /* --------------------------------------
     *
     * allocate new data storage 
     *
     * --------------------------------------
     */

    vsid = Hnewref(handle->hdf_file);
    vp->aid = Hstartwrite(handle->hdf_file, DATA_TAG, vsid, byte_count);

    if(vp->aid == FAIL) return NULL;

    /* make sure our tmp buffer is big enough to hold everything */
    if(tBuf_size < byte_count) {
        if(tBuf) HDfreespace((VOIDP)tBuf);
        tBuf_size = byte_count;
        tBuf = (int8 *) HDgetspace(byte_count);
    }

    /*
     * Do numerical conversions
     */
    DFKsetNT(vp->HDFtype);
    DFKnumout((uint8 *) values, tBuf, (uint32) to_do, 0, 0);

    /*
     * Write out the values
     */
    done = 0;
    while(done != nvalues) {
        status = Hwrite(vp->aid, byte_count, (uint8 *) tBuf);
        if(status != byte_count) return NULL;
        done += to_do;
        if(nvalues - done < to_do) {
            to_do = nvalues - done;
            byte_count = to_do * vp->HDFsize;
        }
    }

    if(Hendaccess(vp->aid) == FAIL) return NULL;

    /* if it is a record var might as well make it linked blocks now */
    if(IS_RECVAR(vp)) {
        vp->aid = HLcreate(handle->hdf_file, DATA_TAG, vsid, 
                           vp->len * BLOCK_SIZE, BLOCK_COUNT);
        if(vp->aid == FAIL) return NULL;
        if(Hendaccess(vp->aid) == FAIL) return NULL;
    }

    if(vp->vgid) {
        /* add new Vdata to existing Vgroup */
        Vaddtagref(vg, (int32) DATA_TAG, (int32) vsid);
        
        /* detach from the variable's VGroup --- will no longer need it */
        Vdetach(vg);
    }
        
#if DEBUG 
    fprintf(stderr, "Done with the DATA Vdata returning id %d\n", vsid);
#endif

    HDfreespace((VOIDP)values);
    vp->aid = FAIL;

    /* added a new object -- make sure we flush the header */
    handle->flags |= NC_HDIRTY;
            
    return vsid;

} /* hdf_get_data */


/* ---------------------------- hdf_get_vp_aid ---------------------------- */
/*

  Return an AID for the current variable.  Return FALSE on error TRUE on success

*/
static int32
hdf_get_vp_aid(handle, vp)
NC        * handle;
NC_var    * vp;
{

    /* attach to proper data storage*/
    if(!vp->data_ref)
        vp->data_ref = hdf_get_data(handle, vp);

    /*
     * Fail if there is no data
     */
    if(vp->data_ref == NULL) return(FALSE);

    if(handle->hdf_mode == DFACC_RDONLY)
        vp->aid = Hstartread(handle->hdf_file, vp->data_tag, vp->data_ref);
    else
        vp->aid = Hstartwrite(handle->hdf_file, vp->data_tag, vp->data_ref, 0);
    
    if(vp->aid == FAIL) {
        HEprint(stderr, 0);
        return(FALSE);
    }

    return(TRUE);

} /* hdf_get_vp_aid */

 
/* --------------------------- hdf_xdr_NCvdata ---------------------------- */
/*
 *  Read / write 'count' items of contiguous data of type 'type' at 'where'
 *
 * If we are not already attached to the VData representing the
 *  data attach to it now.  Since attaching / detaching is so
 *  slow, stay attached for future reads / writes.  As a result,
 *  we must always attach with write access.
 *
 * The calling routine is responsible for calling DFKsetNT() as required.
 */
static bool_t
hdf_xdr_NCvdata(handle, vp, where, type, count, values)
NC      * handle;
NC_var  * vp;
u_long    where;
nc_type   type;
VOIDP     values;
uint32    count;
{
    int32 status;
    int32 byte_count;

#if DEBUG
    fprintf(stderr, "hdf_xdr_NCvdata I've been called : %s\n", vp->name->values);
#endif

    /* map to the correct location */
    where -= vp->begin;

#if DEBUG 
    fprintf(stderr, "Where = %d  count = %d\n", where, count);
#endif
    
    if(vp->aid == FAIL && hdf_get_vp_aid(handle, vp) == FALSE) {
        /*
         * Fail if there is no data *AND* we were trying to read...
         * Otherwise, we should fill with the fillvalue
         */
        if(vp->data_ref == 0) 
            if(handle->hdf_mode == DFACC_RDONLY) {
                if(vp->data_tag == DATA_TAG) {
                    NC_attr ** attr;
                    int len;
                    
                    attr = NC_findattr(&vp->attrs, _FillValue);
                    len = (vp->len / vp->HDFsize) * vp->szof;       
                    if(attr != NULL)
                        hdf_fill_array(values, len, (*attr)->data->values, vp->type);
                    else 
                        NC_arrayfill(values, len, vp->type);
                    
                }
                return TRUE;
            } else {
                return FALSE;
            }
    }

#if 0    
    if((vp->data_offset < 0) && (handle->hdf_mode == DFACC_RDONLY)) {
        /* blank the memory */
        HDmemset(values, (char) 0, count * vp->szof);
        return TRUE;
    }
#endif    

    
    /* 
     * It may be the case that the current does NOT begin at the start of the
     *   data-object which is storing it.  In that case compute the correct 
     *   location.
     */
    if(vp->data_offset > 0) where += vp->data_offset;
    
    /* position ourselves correctly */
    status = Hseek(vp->aid, where, DF_START);
    if(status == FAIL) {
        HEprint(stderr, 0);
        return(FALSE);
    }
    
    /* make sure our tmp buffer is big enough to hold everything */
    byte_count = count * vp->HDFsize;
    if(tBuf_size < byte_count) {
        if(tBuf) HDfreespace((VOIDP)tBuf);
        tBuf_size = byte_count;
        tBuf = (int8 *) HDgetspace(tBuf_size);
        if(tBuf == NULL) return FALSE;
    }
    

/*    This should be set by the caller */
/*    DFKsetNT(vp->HDFtype); */
    
    /* Read or write the data into / from values */
    if(handle->xdrs->x_op == XDR_DECODE) {
        status = Hread(vp->aid, byte_count, (uint8 *) tBuf);
        if(status != byte_count) return FALSE;
        
        /* convert tBuf into values */
        DFKnumin((uint8 *) tBuf, (uint8 *) values, (uint32) count, 0, 0);
        
    } else {
      
        /*  convert values into tBuf */
        DFKnumout((uint8 *) values, tBuf, (uint32) count, 0, 0);
        
        status = Hwrite(vp->aid, byte_count, (uint8 *) tBuf);
        if(status != byte_count) return FALSE;
    }
    
#if 0

    /*
     * Just do the I/O let the level above us worry about converting
     *  the data
     */
    if(handle->xdrs->x_op == XDR_DECODE) {
        status = Hread(vp->aid, byte_count, (uint8 *) values);
        if(status != byte_count) return FALSE;
    } else {
        status = Hwrite(vp->aid, byte_count, (uint8 *) values);
        if(status != byte_count) return FALSE;
    }
 
#endif

#ifdef DEBUG
    fprintf(stderr, " * * * Done with call to xdr_NCvdata * * *\n");
#endif
    
    return(TRUE);
    
} /* xdr_NCvdata */


/* ------------------------- xdr_NCv1data ------------------- */
/*
 * read / write a single datum of type 'type' at 'where'
 * This is designed to replace the xdr based routine of the
 *  similar name
 * Return TRUE if everything worked, else FALSE
 */
static bool_t
  hdf_xdr_NCv1data(handle, vp, where, type, values)
NC      * handle;
NC_var  * vp;
u_long    where;
nc_type   type;
VOIDP     values;
{

#if 0

    intn status;
    uint8 buf[100];
    
    /* Read or write the data into / from values */
    DFKsetNT(vp->HDFtype);
    
    if(handle->xdrs->x_op == XDR_DECODE) {

        if(!(hdf_xdr_NCvdata(handle, vp, where, type, 1, buf)))
            return FALSE;

        /* convert buf into values */
        DFKnumin((uint8 *) buf, (uint8 *) values, 1, 0, 0);
        
    } else {
        
        /*  convert values into tBuf */
        DFKnumout((uint8 *) values, buf, 1, 0, 0);

        if(!(hdf_xdr_NCvdata(handle, vp, where, type, 1, buf)))
            return FALSE;

    }
    
    return TRUE;

#endif
    
    DFKsetNT(vp->HDFtype);
    return (hdf_xdr_NCvdata(handle, vp, where, type, 1, values)); 

} /* hdf_xdr_NCv1data */

#endif

static
NCvar1io(handle, varid, coords, value)
NC *handle ;
int varid ;
const long *coords ;
Void *value ;
{
	NC_var *vp ;
	u_long offset ;
	NC_var *NC_hlookupvar() ;

	if(handle->flags & NC_INDEF )
		return(-1) ;
	if(handle->vars == NULL)
		return(-1) ;

	vp = NC_hlookupvar( handle, varid ) ;
	if(vp == NULL)
		return(-1) ;

	if(vp->assoc->count == 0) /* 'scaler' variable */
	{
            switch(handle->file_type) {
            case HDF_FILE:
                return(
                       hdf_xdr_NCv1data(handle, vp, vp->begin, vp->type, value) ?
                       0 : -1 ) ;
                break;
            case netCDF_FILE:
                return(
                       xdr_NCv1data(handle->xdrs, vp->begin, vp->type, value) ?
                       0 : -1 ) ;
                break;
                
            }
        }

	if( !NCcoordck(handle, vp, coords) )
		return(-1) ;

	offset = NC_varoffset(handle, vp, coords) ;
		
#ifdef VDEBUG
	NCadvise(NC_NOERR, "%s offset %d",
		vp->name->values, offset ) ;
	arrayp("shape", vp->assoc->count, vp->shape) ;
	arrayp("coords", vp->assoc->count, coords) ;
#endif /* VDEBUG */
        
        switch(handle->file_type) {
        case HDF_FILE:
            if( !hdf_xdr_NCv1data(handle, vp, offset, vp->type, value))
                return(-1) ;
            break;
        case netCDF_FILE:
            if( !xdr_NCv1data(handle->xdrs, offset, vp->type, value))
                return(-1) ;
            break;
        }
        
	return(0) ;
}


ncvarput1(cdfid, varid, coords, value)
int cdfid ;
int varid ;
const long *coords ;
const ncvoid *value ;
{
	NC *handle ;

	cdf_routine_name = "ncvarput1" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

        if(!(handle->flags & NC_RDWR))
        {
                NCadvise(NC_EPERM, "%s: NC_NOWRITE", handle->path) ;
                return(-1) ;
        }
	handle->xdrs->x_op = XDR_ENCODE ;

	return( NCvar1io(handle, varid, coords, (Void *)value) ) ;
}


ncvarget1(cdfid, varid, coords, value)
int cdfid ;
int varid ;
const long *coords ;
ncvoid *value ;
{
	NC *handle ;

	cdf_routine_name = "ncvarget1" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_DECODE ;

	return( NCvar1io(handle, varid, coords, (Void *)value) ) ;
}


/*
 * xdr 'count' items of contiguous data of type 'type' at 'where'
 */
static bool_t 
xdr_NCvdata(xdrs, where, type, count, values)
XDR *xdrs ;
u_long where ;
nc_type type ;
unsigned count ;
Void *values ;
{
	u_long rem = 0 ;
	bool_t (*xdr_NC_fnct)() ;
	bool_t stat ;
	size_t szof ;

	switch(type){
	case NC_BYTE :
	case NC_CHAR :
	case NC_SHORT :
		rem = where%4 ;
		where -= rem ; /* round down to nearest word */
		break ;
	}
	if( !xdr_NCsetpos(xdrs, where) )
		return(FALSE) ;

	switch(type){
	case NC_BYTE :
	case NC_CHAR :
		if(rem != 0)
		{
			unsigned vcount = MIN(count, 4 - rem) ;
			if(!xdr_NCvbyte(xdrs, (unsigned)rem, vcount, values) )
				return(FALSE) ;
			values += vcount ;
			count -= vcount ;
		}

		rem = count%4 ; /* tail remainder */
		count -= rem ;
		if(!xdr_opaque(xdrs, values, count))
			return(FALSE) ;

		if(rem != 0)
		{
			values += count ;
			if( !xdr_NCvbyte(xdrs, (unsigned)0, (unsigned)rem ,
				values) )
				return(FALSE) ;
		 	return(TRUE) ;	
		} /* else */
		return(TRUE) ;
	case NC_SHORT :
		if(rem != 0)
		{
			if(!xdr_NCvshort(xdrs, (unsigned)1, (short *)values) )
				return(FALSE) ;
			values += sizeof(short) ;
			count -= 1 ;
		}
		rem = count%2 ; /* tail remainder */
		count -= rem ;
		if(!xdr_shorts(xdrs, (short *)values, count))
			return(FALSE) ;
		if(rem != 0)
		{
			values += (count * sizeof(short)) ;
			return( xdr_NCvshort(xdrs, (unsigned)0,
				(short *)values) ) ;
		} /* else */
		return(TRUE) ;
	case NC_LONG :
		xdr_NC_fnct = xdr_long ;
		szof = sizeof(long) ;
		break ;
	case NC_FLOAT :
		xdr_NC_fnct = xdr_float ;
		szof = sizeof(float) ;
		break ;
	case NC_DOUBLE : 
		xdr_NC_fnct = xdr_double ;
		szof = sizeof(double) ;
		break ;
	default :
		return(FALSE) ;
	}
	for(stat = TRUE ; stat && (count > 0) ; count--)
	{
		stat = (*xdr_NC_fnct)(xdrs,values) ;	
		values += szof ;
	}
	return(stat) ;
}


/*
 *  For a "hypercube" put/get, compute the largest contiguous block
 */
const long *
NCvcmaxcontig(handle, vp, origin, edges)
NC *handle ;
NC_var *vp ;
const long *origin ;
const long *edges ;
{
	const long *edp, *orp ;
	unsigned long *boundary, *shp ;

	if( IS_RECVAR(vp) )
	{
		/*     one dimensional   &&  the only 'record' variable  */
		if(vp->assoc->count == 1 && handle->recsize <= vp->len)
		{
				return(edges) ;
		} /* else */
		boundary = vp->shape +1 ;
	}
	else
		boundary = vp->shape ;

	/* find max contiguous */
	shp = vp->shape + vp->assoc->count - 1 ;
	edp = edges + vp->assoc->count - 1 ;
	orp = origin + vp->assoc->count - 1 ;
	for( ; shp >= boundary ; shp--,edp--,orp--)
	{
		if(*edp > *shp - *orp || *edp < 0 )
		{
			NCadvise(NC_EINVAL, "Invalid edge length %d", *edp) ;
			return(NULL) ;
		}
		if(*edp < *shp )
			break ;
	}
	if(shp < boundary) /* made it all the way */
		edp++ ;
	/* shp, edp reference last index s.t. shape[ii] == edge[ii] */
	return(edp) ;
}


static
NCsimplerecio(handle, vp, start, edges, values)
NC *handle ;
NC_var *vp ;
const long *start ;
const long *edges ;
Void *values ;
{
	long offset ;
	long newrecs ;

	/* 'start' should be verified as valid upon prior to entry to this
	 * routine
	 */
	if(*edges <= 0)
	{
		NCadvise(NC_EINVALCOORDS, "%s: Invalid edge length %ld",
			vp->name->values, *edges) ;
		return -1 ;
	}

	/* check to see if we are trying to read beyond the end */
	newrecs = (*start + *edges) - handle->numrecs ;
	if(handle->xdrs->x_op != XDR_ENCODE && newrecs > 0)
	{
		NCadvise(NC_EINVALCOORDS, "%s: Invalid Coordinates",
			vp->name->values) ;
		return -1 ;
	}

	offset = NC_varoffset(handle, vp, start) ;
#ifdef VDEBUG
	fprintf(stderr, "\t\t %s offset %ld, *edges %lu\n",
				vp->name->values, offset, *edges ) ;
	arrayp("\t\t coords", vp->assoc->count, start) ;
#endif

	if(newrecs > 0)
		handle->flags |= NC_NDIRTY ;

        switch(handle->file_type) {
        case HDF_FILE:
            DFKsetNT(vp->HDFtype);
            if(!hdf_xdr_NCvdata(handle, vp,
                                offset, vp->type, 
                                (uint32)*edges, values))
                return(-1) ;
            break;
        case netCDF_FILE:
            if(!xdr_NCvdata(handle->xdrs,
                            offset, vp->type, 
                            (unsigned)*edges, values))
                return(-1) ;
            break;
        }
        
	if(newrecs > 0)
	{
		handle->numrecs += newrecs ;
		if(handle->flags & NC_NSYNC) /* write out header->numrecs NOW */
		{
			if(!xdr_numrecs(handle->xdrs, handle) )
				return(-1) ;
			handle->flags &= ~NC_NDIRTY ;
		}
	}
	return(0) ;
}


/*
 * The following routine is not `static' because it is used by the `putgetg'
 * module for generalized hyperslab access.
 */
NCvario(handle, varid, start, edges, values)
NC *handle ;
int varid ;
const long *start ;
const long *edges ;
Void *values ;
{

	NC_var *vp ;
	const long *edp0, *edp ;
	unsigned long iocount ;
	NC_var *NC_hlookupvar() ;

	if(handle->flags & NC_INDEF)
		return(-1) ;

	/* find the variable */
	if(handle->vars == NULL)
		return(-1) ;
	vp = NC_hlookupvar( handle, varid ) ;
	if(vp == NULL)
		return(-1) ;

#ifdef VDEBUG
	fprintf(stderr, "Entering NCvario, variable %s\n", vp->name->values ) ;
	arrayp("start", vp->assoc->count, start) ;
	arrayp("edges", vp->assoc->count, edges) ;
#endif /* VDEBUG */

	if(vp->assoc->count == 0) /* 'scaler' variable */
	{
            switch(handle->file_type) {
            case HDF_FILE:
                DFKsetNT(vp->HDFtype);
                return(
                       hdf_xdr_NCv1data(handle, vp, vp->begin, vp->type, values) ?
                       0 : -1 ) ;
                break;
            case netCDF_FILE:
		return(
                       xdr_NCv1data(handle->xdrs, vp->begin, vp->type, values) ?
                       0 : -1 ) ;
                break;
            }
	}

	if( !NCcoordck(handle, vp, start) )
		return(-1) ;

	if( IS_RECVAR(vp) 
		&& vp->assoc->count == 1
		&& handle->recsize <= vp->len)
	{
		/* one dimensional   &&  the only 'record' variable  */
		return(NCsimplerecio(handle, vp, start, edges, values)) ;
	}

	/* find max contiguous, check sanity of edges */
	edp0 = NCvcmaxcontig(handle, vp, start, edges) ;
	if(edp0 == NULL)
		return(-1) ;
#ifdef VDEBUG
	fprintf(stderr, "edp0\t%ld\n", (unsigned long)edp0 - (unsigned long)edges) ;
#endif /* VDEBUG */

	/* now accumulate max count for a single io operation */
	edp = edges + vp->assoc->count - 1 ; /* count is > 0 at this point */
	iocount = 1 ;
	for( ; edp >= edp0 ; edp--)
		iocount *= *edp ;
	/* now edp = edp0 - 1 */

	{ /* inline */
	long  *coords, *upper, *cc ;
	const long *mm ;
	u_long offset ;
	size_t szof = nctypelen(vp->type) ;

	coords = (long *)HDgetspace(vp->assoc->count * sizeof(long)) ;
	if(coords == NULL)
	{
		nc_serror("") ;
		return(-1) ;
	}
	/* copy in starting indices */
	cc = coords ;
	mm = start ;
	while(cc < &coords[vp->assoc->count] )
		*cc++ = *mm++ ;
#ifdef VDEBUG
	arrayp("coords", vp->assoc->count, coords) ;
#endif

	upper = (long *)HDgetspace(vp->assoc->count * sizeof(long)) ;
	if(upper == NULL)
	{
		nc_serror("") ;
		return(-1) ;
	}
	/* set up in maximum indices */
	cc = upper ;
	mm = coords ;
	edp = edges ;
	while(cc < &upper[vp->assoc->count] )
		*cc++ = *mm++ + *edp++ ;
#ifdef VDEBUG
	arrayp("upper", vp->assoc->count, upper) ;
#endif

	/* ripple counter */
	cc = coords ;
	mm = upper ;
	while(*coords < *upper)
	{
#ifdef VDEBUG
		fprintf(stderr, "\t*cc %ld, *mm %ld\n",
			*cc, *mm) ;
#endif /* VDEBUG */
		while( *cc < *mm )
		{
#ifdef VDEBUG
			fprintf(stderr, "\t\tedp0 %p, edges %p, mm %p, &upper[] %p\n",
				edp0, edges, mm, &upper[edp0-edges-1]) ;
#endif /* VDEBUG */
			if(edp0 == edges || mm == &upper[edp0-edges-1])
			{
				/* doit */
				if( !NCcoordck(handle, vp, coords) )
					return(-1) ;
				offset = NC_varoffset(handle, vp, coords) ;
#ifdef VDEBUG
				fprintf(stderr, "\t\t %s offset %lu, iocount %lu\n",
					vp->name->values, offset, iocount ) ;
				arrayp("\t\t coords", vp->assoc->count, coords) ;
#endif

                                switch(handle->file_type) {
                                case HDF_FILE:
                                    if(!hdf_xdr_NCvdata(handle, vp,
                                                        offset, vp->type, 
                                                        (uint32)iocount, values))
                                        return(-1) ;
                                    break;
                                case netCDF_FILE:
                                    if(!xdr_NCvdata(handle->xdrs,
                                                    offset, vp->type, 
                                                    (unsigned)iocount, values))
                                        return(-1) ;
                                    break;
                                }
                                
				values += iocount * szof ;
				(*cc) += (edp0 == edges ? iocount : 1) ;
#ifdef VDEBUG
				fprintf(stderr, "\t\t *cc %ld, *mm %ld  continue\n",
					*cc, *mm) ;
#endif /* VDEBUG */
				continue ;
			}
			cc++ ;
			mm++ ;
#ifdef VDEBUG
			fprintf(stderr, "\t\t*cc %ld, *mm %ld\n",
				*cc, *mm) ;
#endif /* VDEBUG */
		}
#ifdef VDEBUG
		fprintf(stderr, "\tcc %p, coords %p\n",
			cc, coords) ;
#endif /* VDEBUG */
		if(cc == coords)
		{
#ifdef VDEBUG
			fprintf(stderr, "\t break\n") ;
#endif /* VDEBUG */
			break ;
		}
		*cc = start[ cc - coords ] ;
		cc-- ;
		mm-- ;
		(*cc)++ ;
#ifdef VDEBUG
		fprintf(stderr, "\t*coords %ld, *upper %ld\n",
			*coords, *upper) ;
#endif
	}

	Free(coords) ;
	Free(upper) ;
	} /* end inline */

#ifdef VDEBUG
	fprintf(stderr, "Exiting NCvario\n") ;
#endif /* VDEBUG */
	return(0) ;
}


ncvarput(cdfid, varid, start, edges, values)
int cdfid ;
int varid ;
const long *start ;
const long *edges ;
const ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvarput" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

        if(!(handle->flags & NC_RDWR))
        {
                NCadvise(NC_EPERM, "%s: NC_NOWRITE", handle->path) ;
                return(-1) ;
        }
	handle->xdrs->x_op = XDR_ENCODE ;

	return( NCvario(handle, varid, start, edges, (Void *)values) ) ;
}


ncvarget(cdfid, varid, start, edges, values)
int cdfid ;
int varid ;
const long *start ;
const long *edges ;
ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvarget" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_DECODE ;

	return( NCvario(handle, varid, start, edges, (Void *)values) ) ;
}

/* Begin recio */

/*
 * Returns number of record variables in an open netCDF file,
 * Optionally fills an array of record variable handles.
 * Optionally fills an array of record variable ids.
 * Returns -1 on error.
 */
static int
NCnumrecvars(handle, vpp, recvarids)
     NC *handle;
     NC_var **vpp;
	 int *recvarids;
{
	NC_var **dp ;
	int ii ;
	int nrecvars = 0;

	if(handle->vars == NULL)
		return -1 ;

	dp = (NC_var**)handle->vars->values ;
	for(ii = 0 ; ii < handle->vars->count ; ii++, dp++)
	{
		if(IS_RECVAR((*dp)))
		{
			if(vpp != NULL)
				vpp[nrecvars] = *dp ;
			if(recvarids != NULL)
				recvarids[nrecvars] = ii ;
			nrecvars++ ;
		}
	}
	return nrecvars ;
}


static long
NCelemsPerRec(vp)
NC_var *vp ;
{
	long nelems = 1 ;
	int jj ;
	for(jj = 1 ; jj < vp->assoc->count ; jj++)	
		nelems *= vp->shape[jj] ;
	return nelems ;
}


/*
 * Retrieves the number of record variables, the record variable ids, and the
 * record size of each record variable.  If any pointer to info to be returned
 * is null, the associated information is not returned.  Returns -1 on error.
 */
int
ncrecinq(cdfid, nrecvars, recvarids, recsizes)
int cdfid ;
int *nrecvars ;
int *recvarids ;
long *recsizes ;
{
	NC *handle ;
	int nrvars ;
	NC_var *rvp[MAX_NC_VARS] ; 

	cdf_routine_name = "ncrecinq" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	
	nrvars = NCnumrecvars(handle, rvp, recvarids) ;
	if(nrvars == -1)
		return -1 ;

	if(nrecvars != NULL)
		*nrecvars = nrvars;
	
	if(recsizes != NULL)
	{
		int ii ;
		for(ii = 0 ; ii < nrvars ; ii++)
		{
			recsizes[ii] = nctypelen(rvp[ii]->type) * NCelemsPerRec(rvp[ii]) ;
		}
	}
	return nrvars ;
}


static int
NCrecio(handle, recnum, datap)
NC *handle ;
long recnum ;
Void **datap ;
{
	int nrvars ;
	NC_var *rvp[MAX_NC_VARS] ; 
	int ii ;
	long coords[MAX_VAR_DIMS];
	u_long offset ;
	unsigned iocount ;

	nrvars = NCnumrecvars(handle, rvp, NULL) ;
	if(nrvars == -1)
		return -1 ; /* TODO: what error message ?*/

	memset(coords, 0, sizeof(coords)) ;
	coords[0] = recnum ;
	for(ii = 0 ; ii < nrvars ; ii++)
	{
		if(datap[ii] == NULL)
			continue ;
		/* else */
		offset = NC_varoffset(handle, rvp[ii], coords) ;
		iocount = NCelemsPerRec(rvp[ii]) ;

                switch(handle->file_type) {
                case HDF_FILE:
                    DFKsetNT(rvp[ii]->HDFtype);
                    if(!hdf_xdr_NCvdata(handle, rvp[ii],
                                        offset, rvp[ii]->type, 
                                        (uint32)iocount, datap[ii]))
                        return(-1) ;
                    break;
                case netCDF_FILE:
                    if(!xdr_NCvdata(handle->xdrs,
                                    offset, rvp[ii]->type, 
                                    iocount, datap[ii]))
                        return(-1) ;
                    break;
                }

	}
	return 0 ;
}


/*
 * Write one record's worth of data, except don't write to variables for which
 * the address of the data to be written is null.  Return -1 on error.
 */
int
ncrecput(cdfid, recnum, datap)
int cdfid ;
long recnum ;
ncvoid * const *datap ;
{
	NC *handle ;
	long unfilled ;

	cdf_routine_name = "ncrecput" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	if(handle->flags & NC_INDEF )
		return(-1) ;

	if((unfilled = recnum - handle->numrecs) >= 0)
	{
		handle->flags |= NC_NDIRTY ;
		if( handle->flags & NC_NOFILL )
		{
			/* Go directly to jail, do not pass go */
			handle->numrecs = recnum + 1 ;
		}
		else
		{
			/* fill out new records */
			if( !xdr_NCsetpos(handle->xdrs,
					handle->begin_rec + handle->recsize*handle->numrecs) )
			{
				nc_serror("seek, rec %ld", handle->numrecs) ;
				return(FALSE) ;
			}
			for( ; unfilled >= 0 ; unfilled--, handle->numrecs++)
			{
				if( !NCfillrecord(handle->xdrs,
						(NC_var **)handle->vars->values, handle->vars->count) )
				{
					nc_serror("NCfillrec, rec %ld", handle->numrecs) ;
					return(FALSE) ;
				}
			}
		}
		if(handle->flags & NC_NSYNC) /* write out header->numrecs NOW */
		{
			if(!xdr_numrecs(handle->xdrs, handle) )
				return(FALSE) ;
			handle->flags &= ~NC_NDIRTY ;
		}
	}

	handle->xdrs->x_op = XDR_ENCODE ;

	return( NCrecio(handle, recnum, (Void **)datap) ) ;
}


/*
 * Read one record's worth of data, except don't read from variables for which
 * the address of the data to be read is null.  Return -1 on error;
 */
int
ncrecget(cdfid, recnum, datap)
int cdfid ;
long recnum ;
ncvoid **datap ;
{
	NC *handle ;

	cdf_routine_name = "ncrecget" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	if(handle->flags & NC_INDEF )
		return(-1) ;

	handle->xdrs->x_op = XDR_DECODE ;

	return( NCrecio(handle, recnum, (Void **)datap) ) ;
}
