/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 1993, University Corporation for Atmospheric Research           *
 * See netcdf/COPYRIGHT file for copying and redistribution conditions.      *
 *                                                                           *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *   Utility Functions to implement consistent error logging
 * mechanisms for netcdf
 */

#include <errno.h>
#include <stdarg.h>

#include "local_nc.h"

/*
 * Log SYSTEM errors
 * Use where you would want to call perror(3).
 * Calling sequence is
 *	nc_serror(format, arg1, arg2,...)
 * with zero or more args of types compatible with the associated format
 * specifiers.  For example:
 *         nc_serror("shutting down");
 *	   nc_serror("can't open %s", file_name);
 *         nc_serror("process %d in state %s",pid,state);
 */
void
nc_serror(const char *fmt, ...)
{

    if (ncopts & NC_VERBOSE) {
        va_list           args;
        static const char unknown[] = "Unknown Error";
        int               errnum    = errno; /* save real errno in case we wipe it out */
        const char       *cp;

        va_start(args, fmt);

        fprintf(stderr, "%s: ", cdf_routine_name);
        vfprintf(stderr, fmt, args);
        va_end(args);

        switch (errnum) {
            case 0:
                ncerr = NC_NOERR;
                fputc('\n', stderr);
                break;
            default:
                ncerr = NC_SYSERR;
                fprintf(stderr, ": %s\n", (cp = strerror(errnum)) == NULL ? unknown : cp);
                break;
        }

        errno = 0; /* ??? */
    }              /* NC_VERBOSE */

    if (ncopts & NC_FATAL) {
        exit(ncopts);
    }
}

/*
 * Like nc_serror above, but doesn't check for system error.
 * Use for logging error conditions which are not system errors.
 * Calling sequence is
 *	NCadvise(ncerr, format, arg1, arg2,...)
 * with zero or more args of types compatible with the associated format
 * specifiers.  For example:
 *         NCadvise(NC_NOERR, "just advice");
 *         NCadvise(NC_EBADID, "%d is not a valid cdf id", cdfid);
 */
void
NCadvise(int err, const char *fmt, ...)
{
    va_list args;

    ncerr = err;

    if (ncopts & NC_VERBOSE) {
        fprintf(stderr, "%s: ", cdf_routine_name);
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }

    if ((ncopts & NC_FATAL) && ncerr != NC_NOERR) {
        exit(ncopts);
    }
}
