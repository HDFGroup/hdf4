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
 * File:         hchunks.h
 * Purpose:      Header file for Chunked elements
 * Dependencies: tbbt.c mcache.c
 * Contents:     Structures & definitions for chunked elements
 * Structure definitions: DIM_DEF, HCHUNK_DEF
 *---------------------------------------------------------------------------*/

#ifndef H4_HCHUNKS_H
#define H4_HCHUNKS_H

#include "hdfi.h"
#include "hfile.h" /* special info stuff */

/* Public structures */

/* Structure for each Data array dimension Definition */
typedef struct dim_def_struct {
    int32 dim_length;   /* length of this dimension */
    int32 chunk_length; /* chunk length along this dimension */
    int32 distrib_type; /* Data distribution along this dimension */
} DIM_DEF, *DIM_DEF_PTR;

/* Structure for each Chunk Definition*/
typedef struct hchunk_def_struct {
    int32    chunk_size; /* size of this chunk*/
    int32    nt_size;    /* number type size i.e. size of data type */
    int32    num_dims;   /* number of actual dimensions */
    DIM_DEF *pdims;      /* ptr to array of dimension records for this chunk*/
    int32    chunk_flag; /* multiply specialness? SPECIAL_COMP */

    /* For Compression info */
    comp_coder_t comp_type;  /* Compression type */
    comp_model_t model_type; /* Compression model type */
    comp_info   *cinfo;      /* Compression info struct */
    model_info  *minfo;      /* Compression model info struct */
} HCHUNK_DEF, *HCHUNK_DEF_PTR;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t chunked_funcs; /* functions to perform chunking */

/*
** from hchunks.c
*/

/* User Public */
HDFLIBAPI int32 HMCcreate
        (int32 file_id,       /* IN: file to put linked chunk element in */
         uint16 tag,          /* IN: tag of element */
         uint16 ref,          /* IN: ref of element */
         uint8 nlevels,       /* IN: number of levels of chunks */
         int32 fill_val_len,  /* IN: fill value length in bytes */
         void  *fill_val,     /* IN: fill value */
         HCHUNK_DEF *chk_array /* IN: structure describing chunk distribution
                                 can be an array? but we only handle 1 level */);

HDFLIBAPI intn HMCgetcompress(accrec_t     *access_rec, /* IN: access record */
                              comp_coder_t *comp_type,  /* OUT: compression type */
                              comp_info    *c_info /* OUT: retrieved compression info */);

HDFLIBAPI intn HMCgetcomptype(int32         access_id, /* IN: access record */
                              comp_coder_t *comp_type /* OUT: compression type */);

HDFLIBAPI intn HMCgetdatainfo(int32  file_id, /* IN: file in which element is located */
                              uint16 data_tag, uint16 data_ref,
                              int32 *chk_coord,    /* IN: chunk coord array or NULL for non-chunk SDS */
                              uintn  start_block,  /* IN: data block to start at, 0 base */
                              uintn  info_count,   /* IN: size of offset/length lists */
                              int32 *offsetarray,  /* OUT: array to hold offsets */
                              int32 *lengtharray); /* OUT: array to hold lengths */

HDFLIBAPI intn HMCgetdatasize(int32  file_id,   /* IN: file in which element is located */
                              uint8 *p,         /* IN: buffer of special info header */
                              int32 *comp_size, /* OUT: size of compressed data */
                              int32 *orig_size /* OUT: size of non-compressed data */);

HDFLIBAPI int32 HMCsetMaxcache(int32 access_id, /* IN: access aid to mess with */
                               int32 maxcache,  /* IN: max number of pages to cache */
                               int32 flags /* IN: flags = 0, HMC_PAGEALL */);

HDFLIBAPI int32 HMCwriteChunk(int32       access_id, /* IN: access aid to mess with */
                              int32      *origin,    /* IN: origin of chunk to write */
                              const void *datap /* IN: buffer for data */);

HDFLIBAPI int32 HMCreadChunk(int32  access_id, /* IN: access aid to mess with */
                             int32 *origin,    /* IN: origin of chunk to read */
                             void  *datap /* IN: buffer for data */);

HDFLIBAPI int32 HMCPcloseAID(accrec_t *access_rec /* IN:  access record of file to close */);

HDFLIBAPI int32 HMCPgetnumrecs /* has to be here because used in hfile.c */
    (accrec_t *access_rec,     /* IN:  access record to return info about */
     int32    *num_recs /* OUT: length of the chunked elt */);

#ifdef __cplusplus
}
#endif

#endif /* H4_HCHUNKS_H */
