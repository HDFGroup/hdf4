/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/*	$Id$ */

#include	<string.h>
#include	"local_nc.h"
#include	"alloc.h"


NC_var *
NC_new_var(name,type,ndims,dims)
const char *name ;
nc_type type ;
int ndims ;
const int *dims ;
{
	NC_var *ret ;

	ret = (NC_var *)malloc(sizeof(NC_var)) ;
	if( ret == NULL )
		goto alloc_err ;

	ret->name = NC_new_string((unsigned)strlen(name),name) ;
	if( ret->name == NULL)
		goto alloc_err ;

	ret->assoc = NC_new_iarray((unsigned)ndims, dims) ;
	if( ret->assoc == NULL)
		goto alloc_err ;

	ret->shape = NULL ;
	ret->dsizes = NULL ;
	ret->attrs = NULL ;
	ret->type = type ;
	ret->len = 0 ;
	ret->szof = NC_typelen(type) ;
	ret->begin = 0 ;

#ifdef HDF
        ret->vgid = 0;
        ret->data_ref = 0;
        ret->data_tag = DATA_TAG;  /* Assume normal data unless set   */
        ret->data_offset = 0;      /* Assume data starts at beginning */
        ret->numrecs = 0;
        ret->aid = 0;
        ret->HDFtype = hdf_map_type(type);
        ret->HDFsize = DFKNTsize(ret->HDFtype);
#endif

	return(ret) ;
alloc_err :
	nc_serror("NC_new_var") ;
	return(NULL) ;
}


/*
 * Free var
 */
void
NC_free_var(var)
NC_var *var ;
{
	if(var == NULL)
		return ;
	NC_free_string(var->name) ;
	NC_free_iarray(var->assoc) ;
	if(var->shape != NULL)
		Free(var->shape) ;
	if(var->dsizes != NULL)
		Free(var->dsizes) ;
	NC_free_array(var->attrs) ;
	Free(var) ;
}


/*
 * 'compile' the shape and len of a variable
 *  return -1 on error
 */
#ifndef HDF
static 
#endif
NC_var_shape(var, dims)
NC_var *var ;
NC_array *dims;
{
	unsigned long *shape, *dsizes ;
	int ii ;
	unsigned long *shp, *dsp, *op ;
	int *ip ;
	NC_dim **dp ;
	size_t xszof ;

	xszof = NC_xtypelen(var->type) ;
	var->shape = NULL ;
	var->dsizes = NULL ;

	/*
	 * Allocate the shape array
	 */
	ii = var->assoc->count ;
	if(ii == 0)
	{
		/* scalar var, len == szof */
		var->len = xszof ;
		goto out ;
	}
	shape = (unsigned long *)malloc(ii * sizeof(unsigned long)) ;
	if(shape == NULL)
	{
		nc_serror("NC_var_shape") ;
		return(-1) ;
	}

	/*
	 * use the user supplied dimension indices
	 * to determine the shape
	 */
	for(ip = var->assoc->values, op = shape
		; ii > 0 ; ii--)
	{
		if(*ip < 0 || *ip >= ((dims != NULL) ? dims->count : 1) )
		{
			NCadvise(NC_EBADDIM, "Bad dimension id %d", *ip) ;
			Free(shape) ;
			return(-1) ;
		}
		dp = (NC_dim **)dims->values + *ip ;
		*op = (*dp)->size ;
		if(*op == NC_UNLIMITED && ii != var->assoc->count )
		{
			NCadvise(NC_EUNLIMPOS, "NC_UNLIMITED size applied to index other than 0 %d",
				var->assoc->count - ii) ;
			Free(shape) ;
			return(-1) ;
		}
		op++ ; ip++ ;
	}
	var->shape = shape ;

	/*
	 * Allocate the dsizes array
	 */
	ii = var->assoc->count ;
	dsizes = (unsigned long *)malloc(ii * sizeof(unsigned long)) ;
	if(dsizes == NULL)
	{
		nc_serror("NC_var_dsizes") ;
		return(-1) ;
	}
	var->dsizes = dsizes ;

	/* 
	 * Compute var->len and the dsizes
	 */
	shp = shape + var->assoc->count - 1 ; /* count is > 0 here */
	dsp = dsizes + var->assoc->count - 1 ;
	var->len = (*shp) ? (*shp) : 1 ; /* boundary condition for rec */
	var->len *= xszof ;
	if(dsp != NULL) *dsp = xszof ;

	for( shp--, dsp-- ; shp >= shape ; shp--,dsp--)
	{
		*dsp = var->len ;
		if(shp != shape || *shp ) /* include last mult for non-rec vars */
			var->len *= *shp ;
	}

out :
	switch(var->type) {
	case NC_BYTE :
	case NC_CHAR :
	case NC_SHORT :
		if( var->len%4 != 0 )
		{
			var->len += 4 - var->len%4 ; /* round up */
	/*		*dsp += 4 - *dsp%4 ; */
		}
	}

#if 0
	NCadvise(NC_NOERR, "%s var->len %d, var->szof %d",
		var->name->values, var->len, var->szof) ;
	arrayp("\tshape", var->assoc->count, var->shape) ;
	arrayp("\tdsizes", var->assoc->count, var->dsizes) ;
#endif
	return(var->assoc->count) ;
}


ncvardef(cdfid, name, type, ndims, dims)
int cdfid ;
const char *name ;
nc_type type ;
int ndims ;
const int dims[] ;
{
	NC *handle ;
	NC_var *var[1] ;
	NC_var **dp ;
	int ii ;
	int len ;

	cdf_routine_name = "ncvardef" ;

	if( !NC_indefine(cdfid,TRUE) )
		return(-1) ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

	if(!NCcktype(type))
		return(-1) ;

	if(ndims < 0) /* 0 => scalar */
	{
		NCadvise(NC_EINVAL, "Number of dimensions %d < 0", ndims) ;
		return(-1) ;
	} 

	if(ndims > 0 )
	{
		if(handle->dims == NULL || ndims > handle->dims->count )
		{
			NCadvise(NC_EINVAL, "Invalid number of dimensions %d > %d",
				ndims, (handle->dims != NULL) ? handle->dims->count : 0) ;
			return(-1) ;
		}
	}

	if(handle->vars == NULL) /* first time */
	{
		var[0] = NC_new_var(name,type,ndims,dims) ;
		if(var[0] == NULL)
			return(-1) ;
		handle->vars = NC_new_array(NC_VARIABLE,(unsigned)1,
			(Void *)var) ;
		if(handle->vars == NULL)
			return(-1) ;
	} else if(handle->vars->count >= MAX_NC_VARS)
	{
		NCadvise(NC_EMAXVARS, "maximum number of variables %d exceeded",
			handle->vars->count ) ;
		return(-1) ;
	} else {
		/* check for name in use */
		len = strlen(name) ;
		dp = (NC_var**)handle->vars->values ;
		for(ii = 0 ; ii < handle->vars->count ; ii++, dp++)
		{
			if( len == (*dp)->name->count &&
				strncmp(name, (*dp)->name->values, (*dp)->name->count) == 0)
			{
				NCadvise(NC_ENAMEINUSE, "variable \"%s\" in use with index %d",
					(*dp)->name->values, ii) ;
				return(-1) ;
			}
		}
		var[0] = NC_new_var(name,type,ndims,dims) ;
		if(var[0] == NULL)
			return(-1) ;
		if( NC_incr_array(handle->vars, (Void *)var) == NULL)
			return(-1) ;
	}
	if( NC_var_shape(*var, handle->dims) != -1)
	{
		return(handle->vars->count -1) ;
	}
		/* unwind */
	handle->vars->count-- ;
	NC_free_var(var[0]) ;
	return(-1) ;
}


/*
 * Recompute the shapes of all variables
 * Sets handle->begin_rec to start of first record variable
 * returns -1 on error
 */
NC_computeshapes( handle )
NC *handle ;
{
	NC_var **vbase, **vpp ;
	NC_var *first = NULL ;

	handle->begin_rec = 0 ;
	handle->recsize = 0 ;

	if(handle->vars == NULL)
		return(0) ;
	vbase = (NC_var **)handle->vars->values ;
	for( vpp =  vbase ;
		vpp < &vbase[handle->vars->count] ;
		vpp ++)
	{
		if( NC_var_shape(*vpp, handle->dims) == -1)
			return(-1) ;
	  	if(IS_RECVAR(*vpp))	
		{
	  		if(first == NULL)	
				first = *vpp ;
			handle->recsize += (*vpp)->len ;
		}
	}
	if(first != NULL)
	{
		handle->begin_rec = first->begin ;
		/*
	 	 * for special case of exactly one record variable, pack values
	 	 */
		if(handle->recsize == first->len)
			handle->recsize = *first->dsizes ;
	}
	return(handle->vars->count) ;
}


ncvarid( cdfid, name)
int cdfid ;
const char *name ;
{
	NC *handle ;
	NC_var **dp ;
	int ii ;
	int len ;

	cdf_routine_name = "ncvarid" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	if(handle->vars == NULL)
		return(-1) ;
	len = strlen(name) ;
	dp = (NC_var**)handle->vars->values ;
	for(ii = 0 ; ii < handle->vars->count ; ii++, dp++)
	{
		if( len == (*dp)->name->count &&
			strncmp(name, (*dp)->name->values, (*dp)->name->count) == 0)
		{
			return(ii) ;
		}
	}
	NCadvise(NC_ENOTVAR, "variable \"%s\" not found", name) ;
	return(-1) ;
}


/*
 * Given valid handle and varid, return var
 *  else NULL on error
 */
NC_var *
NC_hlookupvar( handle, varid )
NC *handle ;
int varid ;
{
	NC_array **ap ;

	if(varid == NC_GLOBAL) /* Global is error in this context */
	{
		return(NULL) ;
	}else if(handle->vars != NULL && varid >= 0 && varid < handle->vars->count)
	{
		ap = (NC_array **)handle->vars->values ;
		ap += varid ;
	} else {
		NCadvise(NC_ENOTVAR, "%d is not a valid variable id", varid) ;
		return( NULL ) ;
	}
	return((NC_var *)*ap) ;
}


/*
 * Given cdfid and varid, return var
 *  else NULL on error
 */
static NC_var *
NC_lookupvar( cdfid, varid )
int cdfid ;
int varid ;
{
	NC *handle ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(NULL) ;

	return(NC_hlookupvar(handle,varid)) ;
}


ncvarinq( cdfid, varid, name, typep, ndimsp, dims, nattrsp)  
int cdfid ;
int varid ;
char *name ;
nc_type *typep ;
int *ndimsp ;
int dims[] ;
int *nattrsp ;
{
	NC_var *vp ;
	int ii ;

	cdf_routine_name = "ncvarinq" ;

	vp = NC_lookupvar( cdfid, varid ) ;
	if(vp == NULL)
		return(-1) ;

	if(name != NULL)
	{
		(void)strncpy( name, vp->name->values, vp->name->count) ;
		name[vp->name->count] = 0 ;
	}

	if(typep != 0)
		*typep = vp->type ;
	if(ndimsp != 0)
	{
		*ndimsp = vp->assoc->count ;
	}
	if(dims != 0)
	{
		for(ii = 0 ; ii < vp->assoc->count ; ii++)
		{
			dims[ii] = vp->assoc->values[ii] ;
		}
	}
	if(nattrsp != 0)
	{
		if( vp->attrs != NULL)
		{
			*nattrsp = vp->attrs->count ;
		} else {
			*nattrsp = 0 ;
		}
	}

	return(varid) ;
}


ncvarrename(cdfid, varid, newname)
int cdfid ;
int varid ; 
const char *newname ;
{
	
	NC *handle ;
	NC_var **vpp ;
	int ii ;
	int len ;
	NC_string *old, *new ;

	cdf_routine_name = "ncvarrename" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;
	if(!(handle->flags & NC_RDWR))
		return(-1) ;

	/* check for name in use */
	len = strlen(newname) ;
	vpp = (NC_var**)handle->vars->values ;
	for(ii = 0 ; ii < handle->vars->count ; ii++, vpp++)
	{
		if( len == (*vpp)->name->count &&
			strncmp(newname, (*vpp)->name->values, (*vpp)->name->count) == 0)
		{
			NCadvise(NC_ENAMEINUSE, "variable name \"%s\" in use with index %d",
				(*vpp)->name->values, ii) ;
			return(-1) ;
		}
	}

	if(varid == NC_GLOBAL) /* Global is error in this context */
	{
		NCadvise(NC_EGLOBAL, "action prohibited on NC_GLOBAL varid") ;
		return(-1) ;
	}else if(handle->vars != NULL && varid >= 0 && varid < handle->vars->count)
	{
		vpp = (NC_var **)handle->vars->values ;
		vpp += varid ;
	} else {
		NCadvise(NC_ENOTVAR, "%d is not a valid variable id", varid) ;
		return( -1 ) ;
	}

	old = (*vpp)->name ;
	if( NC_indefine(cdfid,TRUE) )
	{
		new = NC_new_string((unsigned)strlen(newname),newname) ;
		if( new == NULL)
			return(-1) ;
		(*vpp)->name = new ;
		NC_free_string(old) ;
		return(varid) ;
	} /* else */
	new = NC_re_string(old, (unsigned)strlen(newname),newname) ;
	if( new == NULL)
		return(-1) ;
	if(handle->flags & NC_HSYNC)
	{
		handle->xdrs->x_op = XDR_ENCODE ;
		if(!xdr_cdf(handle->xdrs, &handle) )
			return(-1) ;
		handle->flags &= ~(NC_NDIRTY | NC_HDIRTY) ;
	} else
		handle->flags |= NC_HDIRTY ;
	return(varid) ;
}


/*
 * Given valid handle, name string, and length of the name, get a var.
 *  else NULL on error
 */
static NC_var *
NC_hvarid(handle, name, namelen)
NC *handle ;
const char *name ;
int namelen ;
{
	NC_var **dp ;
	int ii ;
	if(handle->vars == NULL)
		return NULL ;
	dp = (NC_var**)handle->vars->values ;
	for(ii = 0 ; ii < handle->vars->count ; ii++, dp++)
	{
		if( namelen == (*dp)->name->count &&
				strncmp(name, (*dp)->name->values, (*dp)->name->count) == 0)
		{
			return(*dp) ; /* normal exit */
		}
	}
	return NULL ; /* not found */
}


/*
 * Copy the values of a variable from an input netCDF to an output netCDF.
 * Input and output var assummed to have the same shape.
 * return -1 on error.
 *
 * This function added to support the netcdf operators. The interface
 * is not documented. We plan to supercede it with something more
 * general in a future release.
 */
int
ncvarcpy(incdf, varid, outcdf)
int             incdf;
int             varid;
int             outcdf;
{
	NC *inhandle, *outhandle ;
	NC_var *invp, *outvp ;
	int ndims ;
	int ii ;

	cdf_routine_name = "ncvarcpy" ;

	inhandle = NC_check_id(incdf) ; 
	if(inhandle == NULL)
		return(-1) ;

	if(inhandle->flags & NC_INDEF)
	{
		NCadvise(NC_EINDEFINE, "%s in define mode.", inhandle->path) ;
		return(-1) ;
	}

	outhandle = NC_check_id(outcdf) ; 
	if(outhandle == NULL)
		return(-1) ;

	if(!(outhandle->flags & NC_RDWR))
	{
		/* output file isn't writable */
		NCadvise(NC_EPERM, "%s is not writable", outhandle->path) ;
		return -1 ;
	}

	if(outhandle->flags & NC_INDEF)
	{
		NCadvise(NC_EINDEFINE, "%s in define mode.", outhandle->path) ;
		return(-1) ;
	}

	/* find the variable in the input cdf */
	if(inhandle->vars == NULL
			|| (invp = NC_hlookupvar(inhandle, varid )) == NULL)
	{
		NCadvise(NC_ENOTVAR, "%s: varid %d not found",
			inhandle->path, varid) ;
		return(-1) ;
	}

	/* find the variable in the output cdf */
	outvp = NC_hvarid(outhandle, invp->name->values, invp->name->count) ;
	if(outvp == NULL)
	{
		NCadvise(NC_ENOTVAR, "%s: variable %s not found",
			outhandle->path, invp->name->values) ;
		return(-1) ;
	}

	/* can we even attempt to copy without conversion? */
	if(outvp->type != invp->type)
	{
		NCadvise(NC_EINVAL,
			"Input and output variables must be same type") ;
		return -1 ;
	}

	ndims = invp->assoc->count ;

	if (ndims == 0)
	{
	/* special case scalar vars */
		if(outvp->assoc->count != 0)
		{
			NCadvise(NC_EINVAL,
				"Input and output variables must be same shape") ;
			return -1 ;
		}

	}
	else
	{
		long end[MAX_NC_DIMS] ;
		memcpy(end, invp->shape, ndims * sizeof(unsigned long)) ;
		if(IS_RECVAR(invp))
		{
			/* assert(*end == 0) ; */
			*end = inhandle->numrecs ;
		}
	
		for(ii = 0 ; ii < ndims ; ii++)
			end[ii] -- ;
		/* at this point, end is the largest valid coord of invp */
		if( !NCcoordck(outhandle, outvp, end) )
		{
			NCadvise(NC_EINVAL,
				"Input and output variables not conformable") ;
			return -1 ;
		}
		/* Fill is side effect of NCcoordck */
	}

	/* four cases, really not neccessary here, left for future generalization */
	if(IS_RECVAR(invp))
	{
		if(IS_RECVAR(outvp))
		{
			long outoff, inoff ;
			/* both input and output are rec vars */
			/* check that input fits in output. (per record) */
			if(invp->len > outvp->len)
			{
				NCadvise(NC_EINVALCOORDS,
					"Output var smaller than input") ;
				return -1 ;
			}
			/* copy the data */
			outoff = outhandle->begin_rec + outvp->begin ;
			inoff = inhandle->begin_rec + invp->begin ;
			outhandle->flags |= NC_NDIRTY ;
			for(ii=0 ; ii < inhandle->numrecs ; ii++)
			{
				if(!xdr_setpos(outhandle->xdrs, outoff))
				{
					NCadvise(NC_EXDR, "%s: xdr_setpos", 
						outhandle->path) ;
					return -1 ;
				}
				if(!xdr_setpos(inhandle->xdrs, inoff))
				{
					NCadvise(NC_EXDR, "%s: xdr_setpos", 
						inhandle->path) ;
					return -1 ;
				}
				/* copy data */
				if(!NC_dcpy(outhandle->xdrs, inhandle->xdrs, invp->len))
					return(-1) ;
				outoff += outhandle->recsize ;
				inoff += inhandle->recsize ;
			}
		}
		else
		{
			NCadvise(NC_EINVAL,
				"Input and output variables must be same shape") ;
			return -1 ;
		}
	}
	else
	{
		if(IS_RECVAR(outvp))
		{
			/* input not rec var, output is rec var */
			NCadvise(NC_EINVAL,
				"Input and output variables must be same shape") ;
			return -1 ;
		}
		else
		{
			/* both input and output are not rec vars */
			/* check that input fits in output. */
			if(invp->len > outvp->len)
			{
				NCadvise(NC_EINVALCOORDS,
					"Output var smaller than input") ;
				return -1 ;
			}
			if(!xdr_setpos(outhandle->xdrs, outvp->begin))
			{
				NCadvise(NC_EXDR, "%s: xdr_setpos", 
					outhandle->path) ;
				return -1 ;
			}
			if(!xdr_setpos(inhandle->xdrs, invp->begin))
			{
				NCadvise(NC_EXDR, "%s: xdr_setpos", 
					inhandle->path) ;
				return -1 ;
			}
			/* copy data */
			outhandle->flags |= NC_NDIRTY ;
			if(!NC_dcpy(outhandle->xdrs, inhandle->xdrs, invp->len))
				return(-1) ;
		}
	}
	return 0 ;
}


bool_t
xdr_NC_var(xdrs, vpp)
	XDR *xdrs;
	NC_var **vpp;
{
	u_long begin ;

	if( xdrs->x_op == XDR_FREE)
	{
		NC_free_var((*vpp)) ;
		return(TRUE) ;
	}

	if( xdrs->x_op == XDR_DECODE )
	{
		*vpp = (NC_var *)malloc(sizeof(NC_var)) ;
		if( *vpp == NULL )
		{
			nc_serror("xdr_NC_var") ;
			return(FALSE) ;
		}
	}

	if( !xdr_NC_string(xdrs, &((*vpp)->name)))
		return(FALSE) ;
	if( !xdr_NC_iarray(xdrs, &((*vpp)->assoc)))
		return(FALSE) ;
	if( !xdr_NC_array(xdrs, &((*vpp)->attrs)))
		return(FALSE) ;

#ifdef USE_ENUM
	if (! xdr_enum(xdrs, (enum_t *)&((*vpp)->type)) ) {
		return (FALSE);
	}
#else
	if (! xdr_int(xdrs, &((*vpp)->type)) ) {
		return (FALSE);
	}
#endif
	if (! xdr_u_long(xdrs, &((*vpp)->len)) ) {
		return (FALSE);
	}

	if( xdrs->x_op == XDR_DECODE )
		(*vpp)->szof = NC_typelen((*vpp)->type) ;

	if( xdrs->x_op == XDR_ENCODE )
 		begin = (*vpp)->begin ;
	if (! xdr_u_long(xdrs, &begin))
		return (FALSE);
	if( xdrs->x_op == XDR_DECODE )
		(*vpp)->begin = begin ;

	return( TRUE ) ;
}


/*
 * How much space will the xdr'd var take.
 *
 */
NC_xlen_var(vpp)
NC_var **vpp ;
{
	int len ;

	if(*vpp == NULL)
		return(4) ;

	len = NC_xlen_string((*vpp)->name) ;
	len += NC_xlen_iarray((*vpp)->assoc) ;
	len += NC_xlen_array((*vpp)->attrs) ;
	len += 12 ;

	return(len) ;
}
