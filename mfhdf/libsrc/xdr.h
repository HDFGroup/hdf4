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
 * xdr.h, External Data Representation Serialization Routines.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#ifndef H4_XDR_H
#define H4_XDR_H

#include "H4api_adpt.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef H4_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "mfhdf.h"

typedef int32_t bool_t;

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
 * XDR operations.  XDR_ENCODE causes the type to be encoded into the
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
typedef struct xinfo {
    enum xdr_op x_op; /* operation; fast additional param */
    struct xdr_ops {
        /* Get/put long from underlying stream */
        bool_t (*x_getlong)(struct xinfo *, long *);
        bool_t (*x_putlong)(struct xinfo *, const long *);
        /* Get/put bytes. */
        bool_t (*x_getbytes)(struct xinfo *, char *, unsigned);
        bool_t (*x_putbytes)(struct xinfo *, const char *, unsigned);
        /* Get or seek within the stream (offsets from beginning of stream). */
        unsigned (*x_getpostn)(struct xinfo *);
        bool_t (*x_setpostn)(struct xinfo *, unsigned);
        /* Free the stream. */
        void (*x_destroy)(struct xinfo *);
    } * x_ops;
    void *x_private; /* pointer to private data */
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
 * char       *addr;
 * unsigned    len;
 * unsigned    pos;
 */
#define xdr_getlong(xdrs, longp) (*(xdrs)->x_ops->x_getlong)(xdrs, longp)
#define xdr_putlong(xdrs, longp) (*(xdrs)->x_ops->x_putlong)(xdrs, longp)

static inline int
xdr_getint32(XDR *xdrs, int32_t *ip)
{
    long l;

    if (!xdr_getlong(xdrs, &l))
        return FALSE;
    *ip = (int32_t)l;
    return TRUE;
}

static inline int
xdr_putint32(XDR *xdrs, int32_t *ip)
{
    long l;

    l = (long)*ip;
    return xdr_putlong(xdrs, &l);
}

#define xdr_getbytes(xdrs, addr, len) (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)
#define xdr_putbytes(xdrs, addr, len) (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)

#define xdr_getpos(xdrs)      (*(xdrs)->x_ops->x_getpostn)(xdrs)
#define xdr_setpos(xdrs, pos) (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)

#define xdr_destroy(xdrs)                                                                                    \
    if ((xdrs)->x_ops->x_destroy)                                                                            \
    (*(xdrs)->x_ops->x_destroy)(xdrs)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * These are the "generic" xdr routines.
 */
HDFLIBAPI bool_t xdr_int(XDR *, int *);
HDFLIBAPI bool_t xdr_u_int(XDR *, unsigned *);

HDFLIBAPI bool_t xdr_long(XDR *, long *);
HDFLIBAPI bool_t xdr_u_long(XDR *, unsigned long *);

HDFLIBAPI bool_t xdr_bytes(XDR *, char **, unsigned *, unsigned);
HDFLIBAPI bool_t xdr_opaque(XDR *, char *, unsigned);

HDFLIBAPI bool_t xdr_float(XDR *, float *);
HDFLIBAPI bool_t xdr_double(XDR *, double *);

/* XDR using stdio library (only used in xdrtest.c) */
HDFLIBAPI void xdrstdio_create(XDR *, FILE *, enum xdr_op);

#ifdef __cplusplus
}
#endif

#endif /* H4_XDR_H */
