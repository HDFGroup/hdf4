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
 * File:    cdeflate_priv.h
 * Purpose: Header file for gzip 'deflate' encoding information.
 * Dependencies: should only be included from hcomp_priv.h
 *---------------------------------------------------------------------------*/

#ifndef H4_CDEFLATE_PRIV_H
#define H4_CDEFLATE_PRIV_H

#include "hdf_priv.h"

/* Get the gzip 'deflate' header */
#define intf zintf
#include "zlib.h"
#undef zintf

/* gzip [en|de]coding information */
typedef struct {
    intn     deflate_level;   /* how hard to try to compress this data */
    int32    offset;          /* offset in the de-compressed array */
    intn     acc_init;        /* is access mode initialized? */
    int16    acc_mode;        /* access mode desired */
    void    *io_buf;          /* buffer for I/O with the file */
    z_stream deflate_context; /* pointer to the deflation context for each byte in the element */
} comp_coder_deflate_info_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t cdeflate_funcs; /* functions to perform gzip encoding */

/*
 ** from cdeflate.c
 */

HDFLIBAPI int32 HCPcdeflate_stread(accrec_t *rec);

HDFLIBAPI int32 HCPcdeflate_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPcdeflate_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPcdeflate_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                    int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                    int16 *pspecial);

HDFLIBAPI int32 HCPcdeflate_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPcdeflate_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI intn HCPcdeflate_endaccess(accrec_t *access_rec);

#ifdef __cplusplus
}
#endif

#endif /* H4_CDEFLATE_PRIV_H */
