/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *
 *	This file supports netCDF variable I/O for generalized hyperslabs.
 *	A generalized hyperslab is one in which the locations of the
 *	memory-resident data values may be arbitrary, though they are 
 *	constrained to have a regular structure.  In addition, the values 
 *	of the netCDF variable may be accessed using non-unity strides.
 *
 *	$Id$
 */

#include	"local_nc.h"

#ifndef HDF
    static 
#endif
    int

#ifdef OLD_NCgenio
/*
 * Perform I/O on a generalized hyperslab.  The efficiency of this
 * implementation is dependent upon caching in the lower layers.
 */

NCgenio(handle, varid, start, count, stride, imap, values)
    NC		*handle;
    int		varid;
    const long	*start;		/* NULL => first corner */
    const long	*count;		/* NULL => everything following start[] */
    const long	*stride;	/* NULL => unity strides */
    const long	*imap;		/* NULL => same structure as netCDF variable */
    Void	*values ;
{
    int		maxidim;	/* maximum dimensional index */
    NC_var	*vp	= NC_hlookupvar( handle, varid );

    if (vp == NULL)
	return(-1) ;

    maxidim = vp->assoc->count - 1;

    if (maxidim < 0) {
	/*
	 * The variable is a scalar; consequently, there's only one thing 
	 * to get and only one place to put it.  (Why was I called?)
	 */
	return NCvario(handle, varid, start, count, values);

    } else {
	/*
	 * The variable is an array.
	 */
	int	idim;
	char	*valp	= (char*)values;
	long	mycount[MAX_NC_DIMS];
	long	mystart[MAX_NC_DIMS];
	long	mystride[MAX_NC_DIMS];
	long	myimap[MAX_NC_DIMS];
	long	iocount[MAX_NC_DIMS];	/* count vector for NCvario() */
	long	stop[MAX_NC_DIMS];	/* stop indexes */
	long	length[MAX_NC_DIMS];	/* internal array size */

	/*
	 * Verify stride argument.
	 */
	for (idim = 0; idim <= maxidim; ++idim) {
	    if (stride != NULL && stride[idim] < 1) {
		NCadvise(NC_EINVAL, "Non-positive stride");
		return(-1) ;
	    }
	}

	/*
	 * Initialize I/O parameters.
	 */
	for (idim = maxidim; idim >= 0; --idim) {
	    mystart[idim]	= start != NULL
				    ? start[idim]
				    : 0;
	    mycount[idim]	= count != NULL
				    ? count[idim]
				    : idim == 0 && IS_RECVAR(vp)
					? handle->numrecs - mystart[idim]
					: vp->shape[idim] - mystart[idim];
	    mystride[idim]	= stride != NULL 
				    ? stride[idim]
				    : 1;
	    myimap[idim]	= imap != NULL 
				    ? imap[idim]
				    : vp->szof;

	    iocount[idim]	= 1;
	    length[idim]	= myimap[idim] * mycount[idim];
	    stop[idim]		= mystart[idim] + mycount[idim]*mystride[idim];

printf("mycount %d    myimap %d    length %d\n", mycount[idim], myimap[idim], length[idim]);

	}

	/*
	 * As an optimization, adjust I/O parameters when the fastest 
	 * dimension has unity stride both externally and internally.
	 * In this case, the user could have called a simpler routine
	 * (i.e. ncvarget() or ncvarput()).
	 */
	if (mystride[maxidim] == 1 && myimap[maxidim] == vp->szof) {
	    iocount[maxidim]		= mycount[maxidim];
	    stop[maxidim]		= mystart[maxidim];
	}

	/*
	 * Perform I/O.  Exit when done.
	 */
	for (;;) {

	    int		iostat	= NCvario(handle, varid, mystart, iocount, 
					  (Void*)valp);

printf("Reading from %d %d with lengths %d %d into %d\n", mystart[0], mystart[1], iocount[0], iocount[1], valp);


	    if (iostat != 0)
		return iostat;

	    /*
	     * The following code permutes through the variable's external
	     * start-index space and it's internal address space.  At the 
	     * UPC, this algorithm is commonly called `odometer code'.
	     */
	    idim = maxidim;
	carry:
	    valp		+= myimap[idim];
	    mystart[idim]	+= mystride[idim];
	    if (mystart[idim] >= stop[idim]) {
		mystart[idim]	 = start[idim];
/* 		valp		-= length[idim]; */
 		valp		-= myimap[idim];
		if (--idim < 0)
		    return 0;
		goto carry;
	    }
	}				/* I/O loop */
    }					/* variable is array */
}

#else


/*
 * Perform I/O on a generalized hyperslab.  The efficiency of this
 * implementation is dependent upon caching in the lower layers.
 */
NCgenio(handle, varid, start, count, stride, imap, values)
    NC		*handle;
    int		varid;
    const long	*start;		/* NULL => first corner */
    const long	*count;		/* NULL => everything following start[] */
    const long	*stride;	/* NULL => unity strides */
    const long	*imap;		/* NULL => same structure as netCDF variable */
    Void	*values ;
{
    int		maxidim;	/* maximum dimensional index */
    NC_var	*vp	= NC_hlookupvar( handle, varid );

    if (vp == NULL)
	return(-1) ;

    maxidim = vp->assoc->count - 1;

    if (maxidim < 0) {
	/*
	 * The variable is a scalar; consequently, there's only one thing 
	 * to get and only one place to put it.  (Why was I called?)
	 */
	return NCvario(handle, varid, start, count, values);

    } else {
	/*
	 * The variable is an array.
	 */
	int	idim;
	char	*valp	= (char*)values;
	long	mycount[MAX_NC_DIMS];
	long	mystart[MAX_NC_DIMS];
	long	mystride[MAX_NC_DIMS];
	long	myimap[MAX_NC_DIMS];
	long	iocount[MAX_NC_DIMS];	/* count vector for NCvario() */
	long	stop[MAX_NC_DIMS];	/* stop indexes */

	/*
	 * Verify stride argument.
	 */
	for (idim = 0; idim <= maxidim; ++idim) {
	    if (stride != NULL && stride[idim] < 1) {
		NCadvise(NC_EINVAL, "Non-positive stride");
		return(-1) ;
	    }
	}

	/*
	 * Initialize I/O parameters.
	 */
	for (idim = maxidim; idim >= 0; --idim) {
	    mystart[idim]	= start != NULL
				    ? start[idim]
				    : 0;
	    mycount[idim]	= count != NULL
				    ? count[idim]
				    : idim == 0 && IS_RECVAR(vp)
					? handle->numrecs - mystart[idim]
					: vp->shape[idim] - mystart[idim];
	    mystride[idim]	= stride != NULL 
				    ? stride[idim]
				    : 1;
	    myimap[idim]	= imap != NULL 
				    ? imap[idim]
				    : idim == maxidim
					? vp->szof
					: myimap[idim+1] * mycount[idim+1];

	    iocount[idim]	= 1;
	    stop[idim]		= mystart[idim] + mycount[idim]*mystride[idim];
	}

	/*
	 * As an optimization, adjust I/O parameters when the fastest 
	 * dimension has unity stride both externally and internally.
	 * In this case, the user could have called a simpler routine
	 * (i.e. ncvarget() or ncvarput()).
	 */
	if (mystride[maxidim] == 1 && myimap[maxidim] == vp->szof) {
	    iocount[maxidim]	= mycount[maxidim];
	    stop[maxidim]	= mystart[maxidim];
	}

	/*
	 * Perform I/O.  Exit when done.
	 */
	for (;;) {
	    int		iostat	= NCvario(handle, varid, mystart, iocount, 
					  (Void*)valp);

	    if (iostat != 0)
		return iostat;

	    /*
	     * The following code permutes through the variable's external
	     * start-index space and it's internal address space.  At the 
	     * UPC, this algorithm is commonly called `odometer code'.
	     */
	    idim = maxidim;
	carry:
	    valp		+= myimap[idim];
	    mystart[idim]	+= mystride[idim];
	    if (mystart[idim] >= stop[idim]) {
		mystart[idim]	 = start[idim];
		if (--idim < 0)
		    return 0;
		valp		-= myimap[idim];
		goto carry;
	    }
	}				/* I/O loop */
    }					/* variable is array */
}

#endif

/*
 * Generalized hyperslab output.
 */
    int
ncvarputg(cdfid, varid, start, count, stride, imap, values)
int cdfid ;
int varid ;
const long *start ;
const long *count ;
const long *stride ;
const long *imap ;
const ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvarputg" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_ENCODE ;

	return NCgenio(handle, varid, start, count, 
		       stride, imap, (Void*)values);
}


/*
 * Generalized hyperslab input.
 */
    int
ncvargetg(cdfid, varid, start, count, stride, imap, values)
int cdfid ;
int varid ;
const long *start ;
const long *count ;
const long *stride ;
const long *imap ;
ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvargetg" ;

	handle = NC_check_id(cdfid) ;
	if (handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_DECODE ;

	return NCgenio(handle, varid, start, count, 
		       stride, imap, (Void*)values);
}


/*
 * Stride-oriented hyperslab output.
 */
    int
ncvarputs(cdfid, varid, start, count, stride, values)
int cdfid ;
int varid ;
const long *start ;
const long *count ;
const long *stride ;
const ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvarputs" ;

	handle = NC_check_id(cdfid) ;
	if(handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_ENCODE ;

	return NCgenio(handle, varid, start, count, 
		       stride, (long*)0, (Void*)values);
}


/*
 * Stride-oriented hyperslab input.
 */
    int
ncvargets(cdfid, varid, start, count, stride, values)
int cdfid ;
int varid ;
const long *start ;
const long *count ;
const long *stride ;
ncvoid *values ;
{
	NC *handle ;

	cdf_routine_name = "ncvargets" ;

	handle = NC_check_id(cdfid) ;
	if (handle == NULL)
		return(-1) ;

	handle->xdrs->x_op = XDR_DECODE ;

	return NCgenio(handle, varid, start, count, 
		       stride, (long*)0, (Void*)values);
}
