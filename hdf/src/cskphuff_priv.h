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
 * File:    cskphuff_priv.h
 * Purpose: Header file for skipping huffman encoding information.
 * Dependencies: should only be included from hcomp_priv.h
 *---------------------------------------------------------------------------*/

#ifndef H4_CSKPHUFF_PRIV_H
#define H4_CSKPHUFF_PRIV_H

#include "hdf_priv.h"

/* The maximum source character code: */
#define SKPHUFF_MAX_CHAR 255

/* One greater than the maximum source character code: */
#define SUCCMAX (SKPHUFF_MAX_CHAR + 1)

/* Twice the maximum source character code: */
#define TWICEMAX (2 * SUCCMAX + 1)

/* The root node in the tree */
#define ROOT 0

/* Skipping huffman [en|de]coding information */
typedef struct {
    int        skip_size; /* number of bytes in each element */
    unsigned **left,      /* define the left and right pointer arrays */
        **right;
    uint8 **up;       /* define the up pointer array */
    int     skip_pos; /* current byte to read or write */
    int32   offset;   /* offset in the de-compressed array */
} comp_coder_skphuff_info_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t cskphuff_funcs; /* functions to perform skipping huffman encoding */

/*
 ** from cskphuff.c
 */

HDFLIBAPI int32 HCPcskphuff_stread(accrec_t *rec);

HDFLIBAPI int32 HCPcskphuff_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPcskphuff_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPcskphuff_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                    int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                    int16 *pspecial);

HDFLIBAPI int32 HCPcskphuff_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPcskphuff_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI int HCPcskphuff_endaccess(accrec_t *access_rec);

#ifdef __cplusplus
}
#endif

#endif /* H4_CSKPHUFF_PRIV_H */
