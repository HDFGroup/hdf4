/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *   $Header$
 *********************************************************************/

extern char	*progname;	/* for error messages */
extern char	*cdlname;	/* for error messages */

#undef PROTO
#ifndef NO_HAVE_PROTOTYPES 
#   define	PROTO(x)	x
#else
#   define	PROTO(x)	()
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIPSEL /* punt */
#ifndef NO_STRARG
extern void derror(char *fmt, ...);
#else
extern void derror();
#endif
#endif

/*
extern void	derror		PROTO((
				       char *fmt,
				       ...
				       ));
*/

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
