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

#ifndef HDF_XDR_PRIV_H
#define HDF_XDR_PRIV_H

#include "nc_priv.h"

/* NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
 *
 * This is not a normal XDR implementation!
 *
 * XDR has caused a lot of problems due to the waning availability of the
 * library (removed from libc some time ago) and the variable size of long
 * integers and the way various XDR implementations handled that.
 *
 * What we have here is a stripped-down implementation of XDR that only
 * performs POSIX I/O to a file and removes the problematic XDR "long"
 * operations.
 *
 * In the future, this implementation will be replaced with something
 * even simpler.
 *
 * NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
 */

/* XDR Boolean type */
typedef int32_t bool_t;

/*
 * XDR operations
 *
 * - XDR_ENCODE causes the type to be encoded into the stream
 * - XDR_DECODE causes the type to be extracted from the stream
 * - XDR_FREE can be used to release the space allocated by an XDR_DECODE request
 */
enum xdr_op { XDR_ENCODE = 0, XDR_DECODE = 1, XDR_FREE = 2 };

/* Forward declaration */
struct biobuf;

/*
 * The XDR handle
 *
 * Contains the operation which is being applied to the stream and a
 * private field.
 */
typedef struct xinfo {
    enum xdr_op    x_op;      /* Operation */
    struct biobuf *x_private; /* Pointer to private data */
} XDR;

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE! There are additional netCDF XDR ops scattered throughout the code! */

/*
 * Operations defined on an XDR handle
 */
HDFLIBAPI bool_t hdf_xdr_getbytes(XDR *, char *, unsigned);
HDFLIBAPI bool_t hdf_xdr_putbytes(XDR *, const char *, unsigned);

HDFLIBAPI unsigned hdf_xdr_getpos(XDR *);
HDFLIBAPI bool_t   hdf_xdr_setpos(XDR *, unsigned);

/*
 * "Data type" XDR routines that read/write/free depending on the op
 * parameter of the XDR struct
 */
HDFLIBAPI bool_t hdf_xdr_int(XDR *, int *);
HDFLIBAPI bool_t hdf_xdr_u_int(XDR *, unsigned *);

HDFLIBAPI bool_t hdf_xdr_bytes(XDR *, char **, unsigned *, unsigned);
HDFLIBAPI bool_t hdf_xdr_opaque(XDR *, char *, unsigned);

HDFLIBAPI bool_t hdf_xdr_float(XDR *, float *);
HDFLIBAPI bool_t hdf_xdr_double(XDR *, double *);

/* XDR file manipulation */
HDFLIBAPI int  hdf_xdr_create(XDR *xdrs, int fd, int fmode, enum xdr_op op);
HDFLIBAPI void hdf_xdr_destroy(XDR *);

HDFLIBAPI void hdf_xdr_setup_nofile(XDR *xdrs, int ncop);

#ifdef __cplusplus
}
#endif

#endif /* HDF_XDR_PRIV_H */
