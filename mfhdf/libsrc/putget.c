/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	<string.h>
#include	"local_nc.h"
#include	"alloc.h"

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
		fprintf(stderr," %d", *array) ;
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
	fprintf(stderr,"	NCcoordck: coords %d, count %d, ip %d\n",
		coords, vp->assoc->count, ip ) ;
#endif /* CDEBUG */
	for( ; ip >= boundary ; ip--, up--)
	{
#ifdef CDEBUG
		fprintf(stderr,"	NCcoordck: ip %d, *ip %d, up %d, *up %d\n",
			ip, *ip, up, *up ) ;
#endif /* CDEBUG */
		if( *ip < 0 || *ip >= *up )
			goto bad ;
	}


#ifdef HDF
        if(handle->is_hdf && IS_RECVAR(vp)) {
            Void *strg, *strg1;
            NC_attr ** attr = NULL;
            int count, byte_count;
            
            if((unfilled = *ip - vp->numrecs) <= 0) return TRUE;

            /*
             * Set up the array strg to hold the fill values
             */
            strg = (Void *) malloc(vp->len);
            strg1 = (Void *) malloc(vp->len);
            attr = NC_findattr(&vp->attrs, _FillValue) ;
            if(attr != NULL)
                hdf_fill_array(vp, strg, (*attr)->data->values);
            else 
                NC_arrayfill(strg, vp->len, vp->type);

#ifdef DEBUG
            fprintf(stderr, "Going to fill in record %d for variable %s\n", *ip,
                    vp->name->values);
#endif

            count = vp->dsizes[0] / NC_typelen(vp->type);

            /*
             * Seek to correct location
             */
            byte_count = count * vp->HDFsize;
/*            Hseek(vp->aid, *ip * byte_count, DF_START); */
            Hseek(vp->aid, (vp->numrecs + 1) * byte_count, DF_START);

#ifdef DEBUG
            printf("Filling %d bytes starting at %d\n", 
                   byte_count * unfilled, (vp->numrecs + 1) * byte_count);
#endif      
            vp->numrecs = MAX(vp->numrecs, *ip);
            if((*ip + 1) > handle->numrecs) {
                handle->numrecs = *ip + 1;
                handle->flags |= NC_NDIRTY;
            }
            free(strg);
            free(strg1);

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
#ifdef HDF
          if(handle->is_hdf)
            return( vp->begin + vp->dsizes[0] * *coords + offset) ;
          else
#endif
            return( vp->begin + handle->recsize * *coords + offset) ;
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
	char buf[sizeof(long)] ;
	u_long origin ;

	if(xdrs->x_op == XDR_ENCODE)
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
	}
	if( !XDR_GETLONG(xdrs, (long *)buf ))
	{
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
			(void)memset(buf, 0, sizeof(long)) ;
		}
		else
		{
			NCadvise(NC_EXDR, "xdr_NCvbyte") ;
			return(FALSE) ;
		}
#else
		(void)memset(buf, 0, sizeof(long)) ;
#endif /* XDRSTDIO */
	}

#ifdef SWAP
	rem = 3 - rem ;
#endif /* SWAP */
#ifdef BIG_LONGS
/* this correction should be applied anytime sizeof(long) > 4 */
	rem += (sizeof(long) - 4) ;
#endif
	while(count-- != 0)
	{
		if(xdrs->x_op == XDR_ENCODE)
			buf[rem] = *values ;
		else
			*values = buf[rem] ;
	
#ifndef SWAP
		rem++ ;
#else
		rem-- ;
#endif
		values++ ;
	}
	if(xdrs->x_op == XDR_ENCODE)
	{
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
		if( !XDR_PUTLONG(xdrs, (long *)buf ))
			return(FALSE) ;
	}
	return(TRUE) ;
}


/*
 * xdr a short leaving adjoining short within the word ok.
 * (minimum unit of io is 4 bytes)
 */
static bool_t
xdr_NCvshort(xdrs, which, values)
XDR *xdrs ;
unsigned which ;
short *values ;
{
#ifndef BIG_SHORTS
	short buf[2] ;
	u_long origin ;

	if(xdrs->x_op == XDR_ENCODE)
	{
		origin = xdr_getpos( xdrs ) ;

#ifdef XDRSTDIO
		/* See N.B. above */
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
#endif /* XDRSTDIO */
	}
	if(!XDR_GETLONG(xdrs, (long *)buf) )
	{
#ifdef XDRSTDIO
		/* See N.B. 2 above */
		if(feof((FILE*)xdrs->x_private)) /* NC_NOFILL */
		{
			clearerr((FILE*)xdrs->x_private) ;
			(void)memset((char *)buf, 0, sizeof(long)) ;
		}
		else
		{
			NCadvise(NC_EXDR, "xdr_NCvshort") ;
			return(FALSE) ;
		}
#else
	(void)memset((char *)buf, 0, sizeof(long)) ;
#endif /* XDRSTDIO */
	}

#ifdef SWAP
	which = 1 - which ;
#endif /* SWAP */
	if(xdrs->x_op == XDR_ENCODE)
	{
		buf[which] = *values ;
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
		if( !XDR_PUTLONG(xdrs, (long *)buf) )
			return(FALSE) ;
	} else
		*values = buf[which] ;
	return(TRUE) ;
#else
	/* BIG_SHORTS */
	struct {
		unsigned pad : 32 ;
		unsigned hs: 1 ;
		unsigned hi: 15 ;
		unsigned ls: 1 ;
		unsigned lo: 15 ;
	} buf;
	u_long origin ;

	if(xdrs->x_op == XDR_ENCODE)
	{
		origin = xdr_getpos( xdrs ) ;

#ifdef XDRSTDIO
		/* See N.B. above */
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
#endif /* XDRSTDIO */
		if(!XDR_GETLONG(xdrs, &buf) )
		{
#ifdef XDRSTDIO
			/* See N.B. 2 above */
			if(feof((FILE*)xdrs->x_private)) /* NC_NOFILL */
			{
				clearerr((FILE*)xdrs->x_private) ;
				(void)memset((char *)&buf, 0, sizeof(buf)) ;
			}
			else
			{
				NCadvise(NC_EXDR, "xdr_NCvshort") ;
				return(FALSE) ;
			}
#else
			(void)memset((char *)&buf, 0, sizeof(buf)) ;
#endif /* XDRSTDIO */
		}
	} else if( !XDR_GETLONG(xdrs, &buf) )
		return(FALSE) ;

	if(xdrs->x_op == XDR_ENCODE)
	{
		if(which == 1)
		{
			if( *values < 0 )
			{
				buf.ls = 1 ;
				buf.lo = *values + 32768 ;
			}  else {
				buf.ls = 0 ;
				buf.lo = *values ;
			}
		} else {
			if( *values < 0 )
			{
				buf.hs = 1 ;
				buf.hi = *values + 32768 ;
			}  else {
				buf.hs = 0 ;
				buf.hi = *values ;
			}
		}
		if( !xdr_setpos(xdrs, origin) )
			return(FALSE) ;
		if( !XDR_PUTLONG(xdrs, &buf) )
			return(FALSE) ;
	} else {
		if(which == 1)
		{
			*values = buf.ls ? -32768 + buf.lo : buf.lo ;
		} else {
			*values = buf.hs ? -32768 + buf.hi : buf.hi ;
		}
	}
	return(TRUE) ;
#endif /* !BIG_SHORTS */
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

#include "hdfpg.c"

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
#ifdef HDF
          if(handle->is_hdf)
            return(
                   hdf_xdr_NCv1data(handle, vp, vp->begin, vp->type, value) ?
                   0 : -1 ) ;
          else
#endif
            return(
                   xdr_NCv1data(handle->xdrs, vp->begin, vp->type, value) ?
                   0 : -1 ) ;
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
        
#ifdef HDF
        if(handle->is_hdf) {
            if( !hdf_xdr_NCv1data(handle, vp, offset, vp->type, value))
                return(-1) ;
        } else 
#endif
            {
                if( !xdr_NCv1data(handle->xdrs, offset, vp->type, value))
                    return(-1) ;
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
	fprintf(stderr, "\t\t %s offset %d, *edges %lu\n",
				vp->name->values, offset, *edges ) ;
	arrayp("\t\t coords", vp->assoc->count, start) ;
#endif

	if(newrecs > 0)
		handle->flags |= NC_NDIRTY ;

#ifdef HDF
        if(handle->is_hdf) {
          if(!hdf_xdr_NCvdata(handle, vp,
                          offset, vp->type, 
                          (unsigned)*edges, values))
            return(-1) ;
        } else 
#endif
          {
            if(!xdr_NCvdata(handle->xdrs,
                            offset, vp->type, 
                            (unsigned)*edges, values))
              return(-1) ;
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
	
#ifdef HDF
          if(handle->is_hdf) {
		return(
		hdf_xdr_NCv1data(handle, vp, vp->begin, vp->type, values) ?
		0 : -1 ) ;
          } else
#endif
            {
		return(
		xdr_NCv1data(handle->xdrs, vp->begin, vp->type, values) ?
		0 : -1 ) ;
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
	fprintf(stderr, "edp0 %d\n", edp0 - edges) ;
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

	coords = (long *)malloc(vp->assoc->count * sizeof(long)) ;
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

	upper = (long *)malloc(vp->assoc->count * sizeof(long)) ;
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
		fprintf(stderr, "\t*cc %d, *mm %d\n",
			*cc, *mm) ;
#endif /* VDEBUG */
		while( *cc < *mm )
		{
#ifdef VDEBUG
			fprintf(stderr, "\t\tedp0 %d, edges %d, mm %d, &upper[] %d\n",
				edp0, edges, mm, &upper[edp0-edges-1]) ;
#endif /* VDEBUG */
			if(edp0 == edges || mm == &upper[edp0-edges-1])
			{
				/* doit */
				if( !NCcoordck(handle, vp, coords) )
					return(-1) ;
				offset = NC_varoffset(handle, vp, coords) ;
#ifdef VDEBUG
				fprintf(stderr, "\t\t %s offset %d, iocount %lu\n",
					vp->name->values, offset, iocount ) ;
				arrayp("\t\t coords", vp->assoc->count, coords) ;
#endif

#ifdef HDF
                                if(handle->is_hdf) {
                                  if(!hdf_xdr_NCvdata(handle, vp,
                                                  offset, vp->type, 
                                                  (unsigned)iocount, values))
                                    return(-1) ;
                                } else
#endif
                                    {
                                        if(!xdr_NCvdata(handle->xdrs,
                                                        offset, vp->type, 
                                                        (unsigned)iocount, values))
                                            return(-1) ;
                                    }
                                
				values += iocount * szof ;
				(*cc) += (edp0 == edges ? iocount : 1) ;
#ifdef VDEBUG
				fprintf(stderr, "\t\t *cc %d, *mm %d  continue\n",
					*cc, *mm) ;
#endif /* VDEBUG */
				continue ;
			}
			cc++ ;
			mm++ ;
#ifdef VDEBUG
			fprintf(stderr, "\t\t*cc %d, *mm %d\n",
				*cc, *mm) ;
#endif /* VDEBUG */
		}
#ifdef VDEBUG
		fprintf(stderr, "\tcc %d, coords %d\n",
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
		fprintf(stderr, "\t*coords %d, *upper %d\n",
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
	long coords[MAX_NC_DIMS];
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



#ifdef HDF
                if(handle->is_hdf) {
                    if(!hdf_xdr_NCvdata(handle, rvp[ii],
                                        offset, rvp[ii]->type, 
                                        (unsigned)iocount, datap[ii]))
                        return(-1) ;
                } else
#endif
                    {
                        if(!xdr_NCvdata(handle->xdrs,
                                        offset, rvp[ii]->type, 
                                        (unsigned)iocount, datap[ii]))
                            return(-1) ;
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
