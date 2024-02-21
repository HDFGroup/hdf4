/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/

#include <stdarg.h>
#include <stdio.h>

#include "hdf.h"

#ifdef H4_HAVE_NETCDF
#include "netcdf.h"
#else
#include "hdf4_netcdf.h"
#endif

#include "error.h"

int error_count = 0;

/*
 * Use for logging error conditions.
 */
void
error(const char *fmt, ...)
{
    va_list args;

    fprintf(stderr, "*** ");

    va_start(args, fmt);
    (void)vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    error_count++;
}

/*
 * Turn off netCDF library handling of errors.  Caller must check all error
 * returns after calling this, until on_errs() is called.
 */
void
off_errs(void)
{
    ncopts &= ~NC_FATAL;   /* make errors nonfatal */
    ncopts &= ~NC_VERBOSE; /* turn off error messages */
}

/*
 * Let netCDF library handle subsequent errors.  Callers don't need to check
 * error returns after this.  (This is the initial default.)
 */
void
on_errs(void)
{
    ncopts |= NC_FATAL;   /* make errors fatal */
    ncopts |= NC_VERBOSE; /* library prints error messages */
}
