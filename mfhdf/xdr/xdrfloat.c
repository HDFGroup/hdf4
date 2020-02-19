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
    XDR *xdrs;
    float *fp;
{
    switch (xdrs->x_op) {

    case XDR_ENCODE:
        return (XDR_PUTINT32(xdrs, (int32_t *)fp));

    case XDR_DECODE:
        return (XDR_GETINT32(xdrs, (int32_t *)fp));

    case XDR_FREE:
        return (TRUE);
    }
    return (FALSE);
}

bool_t
xdr_double(xdrs, dp)
    XDR *xdrs;
    double *dp;
{
    int32_t *i32p;
    bool_t rv;

    if (!dp)
        return FALSE;

    switch (xdrs->x_op) {
    case XDR_ENCODE:
        i32p = (int32_t *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
        rv = XDR_PUTINT32(xdrs, i32p);
        if (!rv)
            return (rv);
        rv = XDR_PUTINT32(xdrs, i32p+1);
#else
        rv = XDR_PUTINT32(xdrs, i32p+1);
        if (!rv)
            return (rv);
        rv = XDR_PUTINT32(xdrs, i32p);
#endif
        return rv;
        break;

    case XDR_DECODE:
        i32p = (int32_t *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
        rv = XDR_GETINT32(xdrs, i32p);
        if (!rv)
            return (rv);
        rv = XDR_GETINT32(xdrs, i32p+1);
#else
        rv = XDR_GETINT32(xdrs, i32p+1);
        if (!rv)
            return (rv);
        rv = XDR_GETINT32(xdrs, i32p);
#endif
        return (rv);
        break;

    case XDR_FREE:
        return TRUE;
    } /* switch xdrs->x_op */

    return FALSE;
}
