/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *      See netcdf/COPYRIGHT file for copying and redistribution conditions.
 */
/* $Id$ */
#ifndef _NC_ERROR_
#define _NC_ERROR_

#ifndef NO_STDARG
void nc_serror(const char *fmt, ...) ;
void NCadvise(int err, const char *fmt,...) ;
#else
void nc_serror() ;
void NCadvise() ;
#endif /* NO_STDARG */

#endif /* _NC_ERROR_ */
