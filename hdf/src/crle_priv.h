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
 * File:    crle_priv.h
 * Purpose: Header file for run-length encoding information.
 * Dependencies: should only be included from hcomp_priv.h
 * Invokes: none
 * Contents: Structures & definitions for run-length encoding.  This header
 *              should only be included in hcomp.c and crle.c.
 *---------------------------------------------------------------------------*/

#ifndef H4_CRLE_PRIV_H
#define H4_CRLE_PRIV_H

#include "hdf_priv.h"

/* size of the RLE buffer */
#define RLE_BUF_SIZE 128

/* NIL code for run bytes */
#define RLE_NIL (-1)

/* minimum length of run */
#define RLE_MIN_RUN 3

/* maximum length of run */
#define RLE_MAX_RUN (RLE_BUF_SIZE + RLE_MIN_RUN - 1)

/* minimum length of mix */
#define RLE_MIN_MIX 1

/*
 * Notes on RLE_MIN_RUN and RLE_MIN_MIX:
 * (excerpt from QAK's email to RA - see bug HDFFR-1261)
 *
 * These are [small] optimizations for improving the compression ratio. The
 * algorithm won't encode a run of identical bytes unless it's at least
 * RLE_MIN_RUN bytes long.  So, we can assume that all runs are at least
 * that many bytes, and subtract RLE_MIN_RUN from the actual run length,
 * allowing encoding of runs that are a little bit longer than otherwise
 * allowed (i.e. runs up to 127+RLE_MIN_RUN bytes, instead of only 127 bytes).
 * Similarly for RLE_MIN_MIX - there must be at least RLE_MIN_MIX bytes in a
 * "mixed" sequence of bytes, so we can encode a little bit longer sequence
 * of mixed bytes (127+RLE_MIN_MIX bytes, instead of only 127 bytes).
 */

/* RLE [en|de]coding information */
typedef struct {
    int32    offset;               /* offset in the file */
    uint8    buffer[RLE_BUF_SIZE]; /* buffer for storing RLE bytes */
    int      buf_length;           /* number of bytes in buffer */
    int      buf_pos;              /* offset into the buffer */
    unsigned last_byte;            /* the last byte stored in the buffer */
    unsigned second_byte;          /* the second to last byte stored in the buffer */
    enum {
        RLE_INIT, /* initial state, need to read a byte to
                     determine the next state */
        RLE_RUN,  /* buffer up to the current position is a run */
        RLE_MIX   /* buffer up to the current position is a mix */
    } rle_state;  /* state of the buffer storage */
} comp_coder_rle_info_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t crle_funcs; /* functions to perform run-length encoding */

/*
 ** from crle.c
 */

HDFLIBAPI int32 HCPcrle_stread(accrec_t *rec);

HDFLIBAPI int32 HCPcrle_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPcrle_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPcrle_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                int16 *pspecial);

HDFLIBAPI int32 HCPcrle_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPcrle_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI int HCPcrle_endaccess(accrec_t *access_rec);

#ifdef __cplusplus
}
#endif

#endif /* H4_CRLE_PRIV_H */
