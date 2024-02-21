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

/*-----------------------------------------------------------------------------
 * File:    bitvect_priv.h
 * Purpose: header file for bit-vector API
 *---------------------------------------------------------------------------*/

#ifndef H4_BITVECT_PRIV_H
#define H4_BITVECT_PRIV_H

#include "hdf_priv.h"

/* Boolean values used */
typedef enum { BV_FALSE = 0, BV_TRUE = 1 } bv_bool;

/* Default size of a bit-vector */
#define BV_DEFAULT_BITS 128

/* Define the size of the chunks bits are allocated in */
#define BV_CHUNK_SIZE 64

/* Create the external interface data structures needed */
typedef struct bv_struct_tag *bv_ptr;

/* Useful routines for generally private use */

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI bv_ptr bv_new(int32 num_bits);

HDFLIBAPI intn bv_delete(bv_ptr b);

HDFLIBAPI intn bv_set(bv_ptr b, int32 bit_num, bv_bool value);

HDFLIBAPI intn bv_get(bv_ptr b, int32 bit_num);

HDFLIBAPI int32 bv_size(bv_ptr b);

HDFLIBAPI int32 bv_find_next_zero(bv_ptr b);

#ifdef __cplusplus
}
#endif

#endif /* H4_BITVECT_PRIV_H */
