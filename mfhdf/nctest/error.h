/*********************************************************************
 *   Copyright 1993, University Corporation for Atmospheric Research
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#ifndef NCTEST_ERROR_H
#define NCTEST_ERROR_H

#ifdef HDF
#include "hdf.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Print error message to stderr, don't exit */
#ifndef NO_STRARG
extern void derror(const char *fmt, ...);
#else
extern void derror();
#endif

#ifndef NO_STDARG
extern void error(const char *fmt, ...);
#else /* VARARGS1 */
extern void error(const char *fmt, va_dcl);
#endif

/*
 * Turn off netCDF library handling of errors.  Caller must check all error
 * returns after calling this, until on_errs() is called.
 */
extern void off_errs(void);

/*
 * Let netCDF library handle subsequent errors.  Callers don't need to check
 * error returns after this.  (This is the initial default.)
 */
extern void on_errs(void);

#ifdef __cplusplus
}
#endif

#endif /* NCTEST_ERROR_H */
