/* @(#)xdr_float.c    1.1 87/11/04 3.9 RPCSRC */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_float.c 1.12 87/08/11 Copyr 1984 Sun Micro";
#endif

#include "H4api_adpt.h"

/*
 * xdr_float.c, Generic XDR routines impelmentation.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * These are the "floating point" xdr routines used to (de)serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include <stdio.h>

#include "types.h"
#include "xdr.h"

/* from netCDF */
#define XDRUNIT 4

/* from netCDF */
/* signature: void swapinline32(unsigned int* ip) */
#define swapinline32(ip)           \
{                                  \
    char dst[4];                   \
    char* src = (char*)(ip);       \
    dst[0] = src[3];               \
    dst[1] = src[2];               \
    dst[2] = src[1];               \
    dst[3] = src[0];               \
    *(ip) = *((unsigned int*)dst); \
}

/* from netCDF */
/* Double needs special handling */
void
xdrntohdouble(char* c8, double* dp)
{
    unsigned int ii[2];
    memcpy(ii,c8,(size_t)2*XDRUNIT);
    unsigned int tmp;

    swapinline32(&ii[0]);
    swapinline32(&ii[1]);

    /* interchange ii[0] and ii[1] */
    tmp = ii[0];
    ii[0] = ii[1];
    ii[1] = tmp;
    if(dp) *dp = *(double*)ii;
}


bool_t
xdr_float(xdrs, fp)
    register XDR *xdrs;
    register float *fp;
{
    switch (xdrs->x_op) {

    case XDR_ENCODE:
        return (XDR_PUTLONG(xdrs, (long *)fp));

    case XDR_DECODE:
        return (XDR_GETLONG(xdrs, (long *)fp));

    case XDR_FREE:
        return (TRUE);
    }
    return (FALSE);
}

bool_t
xdr_double(xdrs, dp)
    register XDR *xdrs;
    double *dp;
{
    double dbl_val = 100.0;
    int status = TRUE;

    if (!dp)
        return FALSE;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
#ifndef H4_WORDS_BIGENDIAN
        xdrntohdouble((char*)dp, &dbl_val);
        *dp = dbl_val;
#endif
        return xdr_opaque(xdrs, (caddr_t)dp, (off_t)2*XDRUNIT);
        break;

    case XDR_DECODE:
        /* Pull two units */
        status = xdr_opaque(xdrs, (caddr_t)dp, (off_t)2*XDRUNIT);
#ifndef H4_WORDS_BIGENDIAN
        xdrntohdouble((char*)dp, &dbl_val);
        *dp = dbl_val;
#endif
        return status;
        break;

    case XDR_FREE:
        return TRUE;
    } /* switch xdrs->x_op */

    return FALSE;
}
