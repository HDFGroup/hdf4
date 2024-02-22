/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
 **  hbitio_priv.h
 **
 **  Data structures and macros for bitfile access to HDF data objects.
 **  These are mainly used for compression I/O and N-bit data objects.
 */

#ifndef H4_HBITIO_PRIV_H
#define H4_HBITIO_PRIV_H

#include "hdf.h"

typedef struct bitrec_t {
    int32 acc_id;       /* Access ID for H layer I/O routines */
    int32 bit_id;       /* Bitfile ID for internal use */
                        /* Note that since HDF has signed 32bit offset limit we need to change this to signed
                           since the get passed to Hxxx calls which take signed 32bit arguments */
    int32 block_offset; /* offset of the current buffered block in the dataset */
    int32 max_offset;   /* offset of the last byte written to the dataset */
    int32 byte_offset;  /* offset of the current byte in the dataset */

    intn   count;    /* bit count to next boundary */
    intn   buf_read; /* number of bytes read into buffer (necessary for random I/O) */
    uint8  access;   /* What the access on this file is ('r', 'w', etc..) */
    uint8  mode;     /* how are we interacting with the data now ('r', 'w', etc) */
    uint8  bits;     /* extra bit buffer, 0..BITNUM-1 bits */
    uint8 *bytep;    /* current position in buffer */
    uint8 *bytez;    /* end of buffer to compare */
    uint8 *bytea;    /* byte buffer */
} bitrec_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI const uint8 maskc[9];

HDFLIBAPI const uint32 maskl[33];

#ifdef __cplusplus
}
#endif

#endif /* H4_HBITIO_PRIV_H */
