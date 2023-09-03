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
 * File:    bitvect_private.h
 * Purpose: private header file for bit-vector API
 *---------------------------------------------------------------------------*/

#ifndef H4_BITVECT_PRIVATE_H
#define H4_BITVECT_PRIVATE_H

/* Base type of the array used to store the bits */
typedef uint8 bv_base;

/* # of bits in the base type of the array used to store the bits */
#define BV_BASE_BITS 8

/* bit-vector structure used
 *
 * All values are set to be 32-bit signed integers since that's what the
 * API accepts (negative values are reserved for errors). Bit vectors
 * larger than 2Gbits would require an internal API change.
 */
typedef struct bv_struct_tag {
    int32    bits_used;  /* The actual number of bits current in use */
    int32    array_size; /* The number of bv_base elements in the bit-vector */
    int32    last_zero;  /* The last location we know had a zero bit */
    bv_base *buffer;     /* Pointer to the buffer used to store the bits */
} bv_struct;

#endif /* H4_BITVECT_PRIVATE_H */
