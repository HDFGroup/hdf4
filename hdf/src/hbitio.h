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
 **  hbitio.h
 **
 **  Data structures and macros for bitfile access to HDF data objects.
 **  These are mainly used for compression I/O and N-bit data objects.
 */

#ifndef H4_HBITIO_H
#define H4_HBITIO_H

#include "hdf.h"

/* Macro to define the number of bits cached in the 'bits' variable */
#define BITNUM (sizeof(uint8) * 8)

/* Function-like Macros */
#define Hputbit(bitid, bit) ((Hbitwrite(bitid, 1, (uint32)bit) == FAIL) ? FAIL : SUCCEED)

#endif /* H4_HBITIO_H */
