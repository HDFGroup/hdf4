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

/* The XDR handle
 *
 * Contains operation which is being applied to the stream, an operations
 * vector for the particular implementation (e.g. see xdr_mem.c), and a
 * private field for the use of the particular implementation.
 */
typedef struct xinfo {
    enum xdr_op x_op; /* Operation */

    /* XDR functions */
    struct xdr_ops {
        /* Get/put long from underlying stream */
        bool_t (*x_getlong)(struct xinfo *, long *);
        bool_t (*x_putlong)(struct xinfo *, const long *);

        /* Get/put bytes */
        bool_t (*x_getbytes)(struct xinfo *, char *, unsigned);
        bool_t (*x_putbytes)(struct xinfo *, const char *, unsigned);

        /* Get or seek within the stream (offsets from beginning of stream) */
        unsigned (*x_getpostn)(struct xinfo *);
        bool_t (*x_setpostn)(struct xinfo *, unsigned);

        /* Free the stream */
        void (*x_destroy)(struct xinfo *);
    } * x_ops;

    void *x_private; /* Pointer to private data */
} XDR;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Operations defined on an XDR handle
 */
HDFLIBAPI bool_t xdr_getlong(XDR *, long *);
HDFLIBAPI bool_t xdr_putlong(XDR *, long *);

HDFLIBAPI bool_t xdr_getbytes(XDR *, char *, unsigned);
HDFLIBAPI bool_t xdr_putbytes(XDR *, const char *, unsigned);

HDFLIBAPI unsigned xdr_getpos(XDR *);
HDFLIBAPI bool_t   xdr_setpos(XDR *, unsigned);

HDFLIBAPI void xdr_destroy(XDR *);


/*
 * "Generic type" XDR routines
 */
HDFLIBAPI bool_t xdr_int(XDR *, int *);
HDFLIBAPI bool_t xdr_u_int(XDR *, unsigned *);

HDFLIBAPI bool_t xdr_long(XDR *, long *);
HDFLIBAPI bool_t xdr_u_long(XDR *, unsigned long *);

HDFLIBAPI bool_t xdr_bytes(XDR *, char **, unsigned *, unsigned);
HDFLIBAPI bool_t xdr_opaque(XDR *, char *, unsigned);

HDFLIBAPI bool_t xdr_float(XDR *, float *);
HDFLIBAPI bool_t xdr_double(XDR *, double *);

/* POSIX implementation of XDR */
HDFLIBAPI int xdrposix_create(XDR *xdrs, int fd, int fmode, enum xdr_op op);
HDFLIBAPI int xdrposix_sync(XDR *xdrs);

#ifdef __cplusplus
}
#endif

#endif /* H4_XDR_H */
