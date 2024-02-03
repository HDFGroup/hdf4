/*
 * Copyright (c) 2009, Sun Microsystems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Sun Microsystems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * xdr.c, Generic XDR routines implementation.
 *
 * Copyright (C) 1986, Sun Microsystems, Inc.
 *
 * These are the "generic" xdr routines used to serialize and de-serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include "h4config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xdr.h"

/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT (4)

/*
 * for unit alignment
 */
static const char xdr_zero[BYTES_PER_XDR_UNIT] = {0, 0, 0, 0};

/*
 * Utility functions for 32-bit ints
 */
static int
xdr_getint32(XDR *xdrs, int32_t *ip)
{
    long l;

    if (!xdr_getlong(xdrs, &l))
        return FALSE;
    *ip = (int32_t)l;
    return TRUE;
}

static int
xdr_putint32(XDR *xdrs, int32_t *ip)
{
    long l;

    l = (long)*ip;
    return xdr_putlong(xdrs, &l);
}

/*
 * XDR integers
 */
bool_t
xdr_int(XDR *xdrs, int *ip)
{
    long l;

    switch (xdrs->x_op) {

        case XDR_ENCODE:
            l = (long)*ip;
            return xdr_putlong(xdrs, &l);

        case XDR_DECODE:
            if (!xdr_getlong(xdrs, &l))
                return FALSE;
            *ip = (int)l;
            return TRUE;

        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR unsigned integers
 */
bool_t
xdr_u_int(XDR *xdrs, unsigned *up)
{
    unsigned long l;

    switch (xdrs->x_op) {

        case XDR_ENCODE:
            l = (unsigned long)*up;
            return xdr_putlong(xdrs, (long *)&l);

        case XDR_DECODE:
            if (!xdr_getlong(xdrs, (long *)&l))
                return FALSE;
            *up = (unsigned)l;
            return TRUE;

        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR long integers
 */
bool_t
xdr_long(XDR *xdrs, long *lp)
{
    switch (xdrs->x_op) {
        case XDR_ENCODE:
            return xdr_putlong(xdrs, lp);
        case XDR_DECODE:
            return xdr_getlong(xdrs, lp);
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR unsigned long integers
 */
bool_t
xdr_u_long(XDR *xdrs, unsigned long *ulp)
{
    switch (xdrs->x_op) {
        case XDR_ENCODE:
            return xdr_putlong(xdrs, (long *)ulp);
        case XDR_DECODE:
            return xdr_getlong(xdrs, (long *)ulp);
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR opaque data
 * Allows the specification of a fixed size sequence of opaque bytes.
 * cp points to the opaque object and cnt gives the byte length.
 */
bool_t
xdr_opaque(XDR *xdrs, char *cp, unsigned cnt)
{
    unsigned   rndup;
    static int crud[BYTES_PER_XDR_UNIT];

    /*
     * if no data we are done
     */
    if (cnt == 0)
        return TRUE;

    /*
     * round byte count to full xdr units
     */
    rndup = cnt % BYTES_PER_XDR_UNIT;
    if (rndup > 0)
        rndup = BYTES_PER_XDR_UNIT - rndup;

    if (xdrs->x_op == XDR_DECODE) {
        if (!xdr_getbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return xdr_getbytes(xdrs, (char *)crud, rndup);
    }

    if (xdrs->x_op == XDR_ENCODE) {
        if (!xdr_putbytes(xdrs, cp, cnt))
            return FALSE;
        if (rndup == 0)
            return TRUE;
        return xdr_putbytes(xdrs, xdr_zero, rndup);
    }

    if (xdrs->x_op == XDR_FREE)
        return TRUE;

    return FALSE;
}

/*
 * XDR counted bytes
 * *cpp is a pointer to the bytes, *sizep is the count.
 * If *cpp is NULL maxsize bytes are allocated
 */
bool_t
xdr_bytes(XDR *xdrs, char **cpp, unsigned *sizep, unsigned maxsize)
{
    char    *sp = *cpp; /* sp is the actual string pointer */
    unsigned nodesize;
    bool_t   ret;
    bool_t   allocated = FALSE;

    /*
     * first deal with the length since xdr bytes are counted
     */
    if (!xdr_u_int(xdrs, sizep))
        return FALSE;
    nodesize = *sizep;
    if ((nodesize > maxsize) && (xdrs->x_op != XDR_FREE))
        return FALSE;

    /*
     * now deal with the actual bytes
     */
    switch (xdrs->x_op) {

        case XDR_DECODE:
            if (nodesize == 0)
                return TRUE;
            if (sp == NULL) {
                *cpp = sp = calloc(1, nodesize);
                allocated = TRUE;
            }
            if (sp == NULL) {
                fprintf(stderr, "xdr_bytes: out of memory\n");
                return FALSE;
            }
            /* FALLTHROUGH */

        case XDR_ENCODE:
            ret = xdr_opaque(xdrs, sp, nodesize);
            if ((xdrs->x_op == XDR_DECODE) && (ret == FALSE)) {
                if (allocated == TRUE) {
                    free(sp);
                    *cpp = NULL;
                }
            }
            return ret;

        case XDR_FREE:
            if (sp != NULL) {
                free(sp);
                *cpp = NULL;
            }
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

bool_t
xdr_float(XDR *xdrs, float *fp)
{
    switch (xdrs->x_op) {

        case XDR_ENCODE:
            return xdr_putint32(xdrs, (int32_t *)fp);

        case XDR_DECODE:
            return xdr_getint32(xdrs, (int32_t *)fp);

        case XDR_FREE:
            return TRUE;
    }
    return FALSE;
}

bool_t
xdr_double(XDR *xdrs, double *dp)
{
    int32_t *i32p;
    bool_t   rv;

    if (!dp)
        return FALSE;

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            i32p = (int32_t *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = xdr_putint32(xdrs, i32p);
            if (!rv)
                return rv;
            rv = xdr_putint32(xdrs, i32p + 1);
#else
            rv = xdr_putint32(xdrs, i32p + 1);
            if (!rv)
                return rv;
            rv = xdr_putint32(xdrs, i32p);
#endif
            return rv;
            break;

        case XDR_DECODE:
            i32p = (int32_t *)(void *)dp;
#ifdef H4_WORDS_BIGENDIAN
            rv = xdr_getint32(xdrs, i32p);
            if (!rv)
                return rv;
            rv = xdr_getint32(xdrs, i32p + 1);
#else
            rv = xdr_getint32(xdrs, i32p + 1);
            if (!rv)
                return rv;
            rv = xdr_getint32(xdrs, i32p);
#endif
            return rv;
            break;

        case XDR_FREE:
            return TRUE;
    }

    return FALSE;
}

/* XXX: Keep this code around in case it's needed when handling longs */

#if 0
/*
 * XDR 64-bit integers
 */
bool_t
xdr_int64_t(XDR *xdrs, int64_t *llp)
{
    u_long ul[2];

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            ul[0] = (u_long)((uint64_t)*llp >> 32) & 0xffffffff;
            ul[1] = (u_long)((uint64_t)*llp) & 0xffffffff;
            if (xdr_putlong(xdrs, (long *)&ul[0]) == FALSE)
                return FALSE;
            return (xdr_putlong(xdrs, (long *)&ul[1]));
        case XDR_DECODE:
            if (xdr_getlong(xdrs, (long *)&ul[0]) == FALSE)
                return FALSE;
            if (xdr_getlong(xdrs, (long *)&ul[1]) == FALSE)
                return FALSE;
            *llp = (int64_t)(((uint64_t)ul[0] << 32) | ((uint64_t)(ul[1]) & 0xffffffff));
            return TRUE;
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}

/*
 * XDR unsigned 64-bit integers
 */
bool_t
xdr_uint64_t(XDR *xdrs, uint64_t *ullp)
{
    u_long ul[2];

    switch (xdrs->x_op) {
        case XDR_ENCODE:
            ul[0] = (u_long)(*ullp >> 32) & 0xffffffff;
            ul[1] = (u_long)(*ullp) & 0xffffffff;
            if (xdr_putlong(xdrs, (long *)&ul[0]) == FALSE)
                return (FALSE);
            return (xdr_putlong(xdrs, (long *)&ul[1]));
        case XDR_DECODE:
            if (xdr_getlong(xdrs, (long *)&ul[0]) == FALSE)
                return FALSE;
            if (xdr_getlong(xdrs, (long *)&ul[1]) == FALSE)
                return FALSE;
            *ullp = (uint64_t)(((uint64_t)ul[0] << 32) | ((uint64_t)(ul[1]) & 0xffffffff));
            return TRUE;
        case XDR_FREE:
            return TRUE;
    }
    /* NOTREACHED */
    return FALSE;
}
#endif
