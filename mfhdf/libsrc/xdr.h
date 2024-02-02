/*	$NetBSD: xdr.h,v 1.19 2000/07/17 05:00:45 matt Exp $	*/

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
 *
 *	from: @(#)xdr.h 1.19 87/04/22 SMI
 *	from: @(#)xdr.h	2.2 88/07/29 4.0 RPCSRC
 * $FreeBSD: src/include/rpc/xdr.h,v 1.23 2003/03/07 13:19:40 nectar Exp $
 */

/*
 * xdr.h, External Data Representation Serialization Routines.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#ifndef H4_XDR_H
#define H4_XDR_H

#include "H4api_adpt.h"

#include <stdio.h>

/*
 * XDR provides a conventional way for converting between C data
 * types and an external bit-string representation.  Library supplied
 * routines provide for the conversion on built-in C data types.  These
 * routines and utility routines defined here are used to help implement
 * a type encode/decode routine for each user-defined type.
 *
 * Each data type provides a single procedure which takes two arguments:
 *
 *    bool_t
 *    xdrproc(xdrs, argresp)
 *        XDR *xdrs;
 *        <type> *argresp;
 *
 * xdrs is an instance of a XDR handle, to which or from which the data
 * type is to be converted.  argresp is a pointer to the structure to be
 * converted.  The XDR handle contains an operation field which indicates
 * which of the operations (ENCODE, DECODE * or FREE) is to be performed.
 *
 * XDR_DECODE may allocate space if the pointer argresp is null.  This
 * data can be freed with the XDR_FREE operation.
 *
 * We write only one procedure per data type to make it easy
 * to keep the encode and decode procedures for a data type consistent.
 * In many cases the same code performs all operations on a user defined type,
 * because all the hard work is done in the component type routines.
 * decode as a series of calls on the nested data types.
 */

/*
 * Xdr operations.  XDR_ENCODE causes the type to be encoded into the
 * stream.  XDR_DECODE causes the type to be extracted from the stream.
 * XDR_FREE can be used to release the space allocated by an XDR_DECODE
 * request.
 */
enum xdr_op { XDR_ENCODE = 0, XDR_DECODE = 1, XDR_FREE = 2 };

/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT (4)

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */
typedef struct __rpc_xdr {
    enum xdr_op x_op; /* operation; fast additional param */
    struct xdr_ops {
        /* Get/put long from underlying stream */
        bool_t (*x_getlong)(struct __rpc_xdr *, long *);
        bool_t (*x_putlong)(struct __rpc_xdr *, const long *);
        /* Get/put bytes. */
        bool_t (*x_getbytes)(struct __rpc_xdr *, char *, u_int);
        bool_t (*x_putbytes)(struct __rpc_xdr *, const char *, u_int);
        /* Get or seek within the stream (offsets from beginning of stream). */
        u_int (*x_getpostn)(struct __rpc_xdr *);
        bool_t (*x_setpostn)(struct __rpc_xdr *, u_int);
        /* Returns a pointer to the next n bytes in the stream. */
        int32_t *(*x_inline)(struct __rpc_xdr *, u_int);
        /* Free the stream. */
        void (*x_destroy)(struct __rpc_xdr *);
        /* Control */
        bool_t (*x_control)(struct __rpc_xdr *, int, void *);
    } * x_ops;
    char *x_public;  /* users' data */
    void *x_private; /* pointer to private data */
    char *x_base;    /* private used for position info */
    u_int x_handy;   /* extra private word */
} XDR;

/*                                                                               
 * A xdrproc_t exists for each data type which is to be encoded or decoded.      
 *                                                                               
 * The second argument to the xdrproc_t is a pointer to an opaque pointer.       
 * The opaque pointer generally points to a structure of the data type           
 * to be decoded.  If this pointer is 0, then the type routines should           
 * allocate dynamic storage of the appropriate size and return it.               
 *                                                                               
 * This typedef has caused problems in the past. Most compilers flag the         
 * signature as incompatible and this can result in undefined behavior           
 * according to the C99 standard.                                                
 *                                                                               
 * The current typedef used here works on all tested platforms, but be           
 * aware that future compilers could have trouble with it. Failures              
 * will most often be raised in xdr_vector calls in xdrtest.c. We've             
 * papered over the incompatible pointer warnings with casts for the             
 * time being.                                                                   
 */
typedef bool_t (*xdrproc_t)(XDR *, void *, ...);

/*
 * Operations defined on a XDR handle
 *
 * XDR        *xdrs;
 * long       *longp;
 * char *      addr;
 * u_int       len;
 * u_int       pos;
 */
#define XDR_GETLONG(xdrs, longp) (*(xdrs)->x_ops->x_getlong)(xdrs, longp)
#define xdr_getlong(xdrs, longp) (*(xdrs)->x_ops->x_getlong)(xdrs, longp)

#define XDR_PUTLONG(xdrs, longp) (*(xdrs)->x_ops->x_putlong)(xdrs, longp)
#define xdr_putlong(xdrs, longp) (*(xdrs)->x_ops->x_putlong)(xdrs, longp)

static __inline int
xdr_getint32(XDR *xdrs, int32_t *ip)
{
    long l;

    if (!xdr_getlong(xdrs, &l))
        return (FALSE);
    *ip = (int32_t)l;
    return (TRUE);
}

static __inline int
xdr_putint32(XDR *xdrs, int32_t *ip)
{
    long l;

    l = (long)*ip;
    return xdr_putlong(xdrs, &l);
}

#define XDR_GETINT32(xdrs, int32p) xdr_getint32(xdrs, int32p)
#define XDR_PUTINT32(xdrs, int32p) xdr_putint32(xdrs, int32p)

#define XDR_GETBYTES(xdrs, addr, len) (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)
#define xdr_getbytes(xdrs, addr, len) (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)

#define XDR_PUTBYTES(xdrs, addr, len) (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)
#define xdr_putbytes(xdrs, addr, len) (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)

#define xdr_getpos(xdrs) (*(xdrs)->x_ops->x_getpostn)(xdrs)
#define xdr_setpos(xdrs, pos) (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)
#define xdr_inline(xdrs, len) (*(xdrs)->x_ops->x_inline)(xdrs, len)

#define xdr_destroy(xdrs)                                                                                    \
    if ((xdrs)->x_ops->x_destroy)                                                                            \
    (*(xdrs)->x_ops->x_destroy)(xdrs)

/*
 * These are the "generic" xdr routines.
 */
#ifdef __cplusplus
extern "C" {
#endif
XDRLIBAPI bool_t xdr_int(XDR *, int *);
XDRLIBAPI bool_t xdr_u_int(XDR *, u_int *);
XDRLIBAPI bool_t xdr_long(XDR *, long *);
XDRLIBAPI bool_t xdr_u_long(XDR *, u_long *);
XDRLIBAPI bool_t xdr_short(XDR *, short *);
XDRLIBAPI bool_t xdr_u_short(XDR *, u_short *);
XDRLIBAPI bool_t xdr_int64_t(XDR *, int64_t *);
XDRLIBAPI bool_t xdr_uint64_t(XDR *, uint64_t *);
XDRLIBAPI bool_t xdr_enum(XDR *, enum_t *);
XDRLIBAPI bool_t xdr_bytes(XDR *, char **, u_int *, u_int);
XDRLIBAPI bool_t xdr_opaque(XDR *, char *, u_int);
XDRLIBAPI bool_t xdr_vector(XDR *, char *, u_int, u_int, xdrproc_t);
XDRLIBAPI bool_t xdr_float(XDR *, float *);
XDRLIBAPI bool_t xdr_double(XDR *, double *);
XDRLIBAPI void   xdr_free(xdrproc_t, void *);

/*
 * These are the public routines for the various implementations of
 * xdr streams.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* XDR using stdio library */
XDRLIBAPI void xdrstdio_create(XDR *, FILE *, enum xdr_op);

#ifdef __cplusplus
}
#endif

#endif /* H4_XDR_H */
