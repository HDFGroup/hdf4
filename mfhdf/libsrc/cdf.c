/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	"local_nc.h"
#include	"alloc.h"


void
NC_free_cdf(handle)
NC *handle ;
{
	if(handle == NULL)
		return ;
	NC_free_array(handle->dims) ;
	NC_free_array(handle->attrs) ;
	NC_free_array(handle->vars) ;

	xdr_destroy(handle->xdrs) ;
	Free(handle->xdrs) ;
	Free(handle) ;
}


NC *
NC_new_cdf(name, mode)
const char *name ;
int mode ;
{
	NC *cdf ;

	cdf = (NC *)malloc(sizeof(NC)) ;
	if( cdf == NULL )
	{
		nc_serror("NC_new_cdf") ;
		return(NULL) ;
	}

	cdf->flags = mode ;

	cdf->xdrs = (XDR *)malloc(sizeof(XDR)) ;
	if( cdf->xdrs == NULL)
	{
		nc_serror("NC_new_cdf: xdrs") ;
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	if( NCxdrfile_create( cdf->xdrs, name, mode ) < 0) 
	{
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	cdf->dims = NULL ;
	cdf->attrs = NULL ;
	cdf->vars = NULL ;
	cdf->begin_rec = 0 ;
	cdf->recsize = 0 ;
	cdf->numrecs = 0 ;
	cdf->redefid = -1 ;

#ifdef HDF 

        if(mode & NC_CREAT)
            cdf->is_hdf = TRUE;
        else
            cdf->is_hdf = (int) Hishdf((char *) name);
        
        if(cdf->is_hdf) {
            int32 hdf_mode;
            
            switch(mode) {
            case NC_CLOBBER   :
                hdf_mode = DFACC_CLOBBER; break;
            case NC_NOCLOBBER :
            case NC_WRITE     :
		hdf_mode = DFACC_RDWR;    break;
            case NC_NOWRITE   :
		hdf_mode = DFACC_RDONLY;  break;
            default:
                hdf_mode = DFACC_RDWR;
            }

            cdf->hdf_file = (int32) Hopen((char *) name, hdf_mode, 200);
            Vstart(cdf->hdf_file);
            
            if(cdf->hdf_file == FAIL) {
                return (NULL);
            }
            
            cdf->hdf_mode = hdf_mode;
            cdf->vgid = 0;
            
            HDstrncpy(cdf->path, name, FILENAME_MAX);
            
#ifdef DEBUG
            printf("value returned from Hopen() : %d\n", cdf->hdf_file);
#endif
        }
#endif


	if(cdf->xdrs->x_op == XDR_DECODE) /* Not NC_CREAT */
	{
		if(!xdr_cdf(cdf->xdrs, &cdf) )
		{
			NC_free_cdf(cdf) ;
			return(NULL) ;
		}
		if( NC_computeshapes(cdf) == -1)
			return(NULL) ;
	}

	return(cdf) ;
}


/* 
 * Duplicate a description structure.
 * Can only be called for 'old' extant on disk, eg, old in DATA mode.
 */
NC *
NC_dup_cdf(name, mode, old)
const char *name ;
int mode ;
NC *old ;
{
	NC *cdf ;

	cdf = (NC *)malloc(sizeof(NC)) ;
	if( cdf == NULL )
	{
		nc_serror("NC_dup_cdf") ;
		return(NULL) ;
	}

	cdf->flags = old->flags | NC_INDEF ;

	cdf->xdrs = (XDR *)malloc(sizeof(XDR)) ;
	if( cdf->xdrs == NULL)
	{
		nc_serror("NC_dup_cdf: xdrs") ;
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	cdf->dims = NULL ;
	cdf->attrs = NULL ;
	cdf->vars = NULL ;
	cdf->begin_rec = 0 ;
	cdf->recsize = 0 ;
	cdf->numrecs = 0 ;

/*
#ifdef HDF
        if(old->is_hdf) {
          cdf->hdf_file = old->hdf_file;
          cdf->is_hdf = old->is_hdf;
          cdf->vgid = old->vgid;
        }
#endif
*/

#ifdef HDF
        cdf->is_hdf = old->is_hdf;
#endif

	if(NCxdrfile_create( cdf->xdrs, name, mode) < 0)
	{
		Free(cdf) ;
		return(NULL) ;
	} /* else */	

	old->xdrs->x_op = XDR_DECODE ;
	if(!xdr_cdf(old->xdrs, &cdf) )
	{
		NC_free_cdf(cdf) ;
		return(NULL) ;
	}
	if( NC_computeshapes(cdf) == -1)
		return(NULL) ;

	return(cdf) ;
}


ncinquire(cdfid, ndimsp, nvarsp, nattrsp, xtendimp)
int cdfid ;
int *ndimsp ;
int *nvarsp ;
int *nattrsp ;
int *xtendimp ;
{
	NC *handle ;

	cdf_routine_name = "ncinquire" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	
	if(nvarsp != NULL)
		*nvarsp = (handle->vars != NULL) ? handle->vars->count : 0 ;
	if(nattrsp != NULL)
		*nattrsp = (handle->attrs != NULL) ? handle->attrs->count : 0 ;
	if(handle->dims != NULL)
	{
		NC_dim **dp ;
		int ii ;

		if(ndimsp != NULL)
			*ndimsp = handle->dims->count ;
		if(xtendimp != NULL) {
			*xtendimp = -1 ;

			dp = (NC_dim**)handle->dims->values ;
			for(ii = 0 ; ii < handle->dims->count ; ii++, dp++)
			{
				if((*dp)->size == NC_UNLIMITED)
				{
					*xtendimp = ii ;
				}
			}
		}
	} else {
		if(ndimsp != NULL)
			*ndimsp = 0 ;
		if(xtendimp != NULL)
			*xtendimp = -1 ;
	}

	return(cdfid) ;
}

#ifdef HDF
bool_t
xdr_cdf(xdrs, handlep)
	XDR *xdrs;
	NC **handlep;
{

  if((*handlep)->is_hdf)
    return(hdf_xdr_cdf(xdrs, handlep));
  else
    return(NC_xdr_cdf(xdrs, handlep));

}
#endif


#ifdef HDF
bool_t
NC_xdr_cdf(xdrs, handlep)
	XDR *xdrs;
	NC **handlep;
#else
bool_t
xdr_cdf(xdrs, handlep)
	XDR *xdrs;
	NC **handlep;
#endif
{

	u_long	magic = NCMAGIC ;

	if( xdrs->x_op == XDR_FREE)
	{
		NC_free_cdf(*handlep) ;
		return(TRUE) ;
	}
	
	if( xdr_getpos(xdrs) != 0)
	{
		if( !xdr_setpos(xdrs, 0L) )
		{
			nc_serror("Can't set position to begin") ;
			return(FALSE) ;
		}
	}

	/* magic number */
	if( !xdr_u_long(xdrs, &magic) )
	{
		if( xdrs->x_op == XDR_DECODE)
		{
			NCadvise(NC_ENOTNC,
				"Not a netcdf file (Can't read magic number)") ;
		}
		else
		{
			/* write error */
			nc_serror("xdr_cdf: xdr_u_long") ;
		}
		return(FALSE) ;
	}

	if( xdrs->x_op == XDR_DECODE && magic != NCMAGIC )
	{
		if(magic == NCLINKMAGIC)
		{
			NCadvise(NC_NOERR, "link file not handled yet") ;
			return(FALSE) ;
		} /* else */
		NCadvise(NC_ENOTNC, "Not a netcdf file") ;
		return(FALSE) ;
	}

	if( !xdr_numrecs(xdrs, *handlep))
	{
		NCadvise(NC_EXDR, "xdr_numrecs") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->dims)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:dims") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->attrs)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:attrs") ;
		return(FALSE) ;
	}
	if( !xdr_NC_array(xdrs, &((*handlep)->vars)))
	{
		NCadvise(NC_EXDR, "xdr_cdf:vars") ;
		return(FALSE) ;
	}	
	return(TRUE) ;
}

#ifdef HDF

#include "hdfcdf.c"

#endif /* HDF */

/*
 * How much space will the xdr'd NC description take.
 *
 */
NC_xlen_cdf(cdf)
NC *cdf ;
{
	int len = 8 ;

	if(cdf == NULL)
		return(0) ;

	len += NC_xlen_array(cdf->dims) ;
	len += NC_xlen_array(cdf->attrs) ;
	len += NC_xlen_array(cdf->vars) ;

	return(len) ;
}


#define RECPOS	4L 	/* seek index of numrecs value */
bool_t
xdr_numrecs(xdrs, handle)
	XDR *xdrs;
	NC *handle;
{

#ifdef HDF
    if(handle->is_hdf) return TRUE;
#endif

	if( (handle->flags & NC_NOFILL)
		&& xdrs->x_op == XDR_ENCODE
		&& handle->begin_rec > 0)
	{
		/*
		 * we need to write something just beyond the last 
		 * record so we can successfully read back the 
		 * entire last record.
		 */
		if( !xdr_setpos(xdrs, handle->begin_rec
				+  handle->numrecs * handle->recsize) )
		{
			nc_serror("Can't set position to EOF") ;
			return(FALSE) ;
		}
#ifdef RDEBUG
		fprintf(stderr,"\txdr_numrecs %ld = %d + %ld * %d\n",
			xdr_getpos(xdrs), 
			handle->begin_rec, handle->numrecs, handle->recsize) ;
#endif /*  RDEBUG */
		if( !XDR_PUTLONG(xdrs, (long*)&(handle->numrecs)) )
			return(FALSE) ;
	}

	if( !xdr_setpos(xdrs, RECPOS) )
	{
		nc_serror("Can't set position to RECPOS") ;
		return(FALSE) ;
	}
	return( xdr_u_long(xdrs, &(handle->numrecs)) ) ;
}


bool_t
xdr_NC_fill(xdrs, vp)
XDR *xdrs ;
NC_var *vp ;
{
	char fillp[2*sizeof(double)] ;
	bool_t stat ;
	bool_t (*xdr_NC_fnct)() ;
	u_long alen = vp->len ;
	NC_attr **attr = NULL ;


	/*
	 * set up fill value
	 */
	/* start with the default */
	NC_arrayfill((Void *)fillp, (size_t)2*sizeof(double),
		vp->type) ;

	/* 
	 * if there is a valid user defined value, use it instead
	 */
	attr = NC_findattr(&vp->attrs, _FillValue) ;
	if( attr != NULL )
	{
		if( (*attr)->data->type != vp->type || (*attr)->data->count != 1 )
			NCadvise(NC_EBADTYPE, "var %s: _FillValue type mismatch",
				vp->name->values) ;
		else
		{
			int len = NC_typelen(vp->type) ;
			char *cp = fillp ;
			while(cp < &fillp[sizeof(fillp) -1])
			{
				NC_copy_arrayvals(cp, (*attr)->data) ;
				cp += len ;
			}
		}
	}

		switch(vp->type){
		case NC_BYTE :
		case NC_CHAR :
			alen /= 4 ;
			xdr_NC_fnct = xdrs->x_ops->x_putlong ;
			break ;
		case NC_SHORT :
#ifndef BIG_SHORTS
			alen /= 4 ;
			xdr_NC_fnct = xdrs->x_ops->x_putlong ;
			break ;
#else
			
			for(stat = TRUE ; stat && (alen > 0) ; alen -= 4)
			{
				stat = xdr_shorts(xdrs,(short *)fillp, 2) ;
			}
			goto out ;
#endif
		case NC_LONG :
			alen /= 4 ;
			xdr_NC_fnct = xdrs->x_ops->x_putlong ;
			break ;	
		case NC_FLOAT :
			alen /= 4 ;
			xdr_NC_fnct = xdr_float ;
			break ;
		case NC_DOUBLE : 
			alen /= 8 ;
			xdr_NC_fnct = xdr_double ;
			break ;
		default :
			NCadvise(NC_EBADTYPE, "bad type %d", vp->type) ;
			return(FALSE) ;
		}

		/* write out fill values */
		for(stat = TRUE ; stat && (alen > 0) ; alen--)
		{
			stat = (*xdr_NC_fnct)(xdrs,fillp) ;	
		}
#ifdef BIG_SHORTS
out:
#endif
		if(!stat)
		{
			NCadvise(NC_EXDR, "xdr_NC_fill") ;
			return(FALSE) ;
		}
		
		return(TRUE) ;
}
