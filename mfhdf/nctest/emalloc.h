/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Id$
 *********************************************************************/

#undef PROTO
#ifndef NO_HAVE_PROTOTYPES 
#   define	PROTO(x)	x
#else
#   define	PROTO(x)	()
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void	*emalloc	PROTO((
				       int size
				       ));

extern void	*erealloc	PROTO((
				       void *ptr,
				       int size
				       ));

#ifdef __cplusplus
}
#endif

