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
 * File:    cszip.h
 * Purpose: Header file for szip encoding information.
 * Dependencies: should only be included from hcompi.h
 * Invokes: none
 * Contents: Structures & definitions for szip encoding.  This header
 *              should only be included in hcomp.c and cszip.c.
 * Structure definitions:
 * Constant definitions:
 *---------------------------------------------------------------------------*/

#ifndef H4_CSZIP_H
#define H4_CSZIP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Special parameters for szip compression */
/* [These are aliases for the similar definitions in ricehdf.h header file] */
#define H4_SZ_ALLOW_K13_OPTION_MASK 1
#define H4_SZ_CHIP_OPTION_MASK      2
#define H4_SZ_EC_OPTION_MASK        4
#define H4_SZ_LSB_OPTION_MASK       8
#define H4_SZ_MSB_OPTION_MASK       16
#define H4_SZ_NN_OPTION_MASK        32
#define H4_SZ_RAW_OPTION_MASK       128

/*
 ** from cszip.c
 */

HDFLIBAPI int32 HCPcszip_stread(accrec_t *rec);

HDFLIBAPI int32 HCPcszip_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPcszip_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPcszip_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                 int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                 int16 *pspecial);

HDFLIBAPI int32 HCPcszip_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPcszip_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI intn HCPcszip_endaccess(accrec_t *access_rec);

/*
 * prototype in proto.h
 * HDFLIBAPI intn HCPsetup_szip_parms(comp_info *c_info, int32 nt, int32 ncomp, int32 ndims, int32 *dims,
 *                                   int32 *cdims);
 */

#ifdef __cplusplus
}
#endif

/* SZIP [en|de]coding information */
typedef struct {
    int32  offset; /* offset in the file */
    uint8 *buffer; /* buffer for storing SZIP bytes */
    int32  buffer_pos;
    int32  buffer_size;
    int32  bits_per_pixel;
    int32  options_mask;
    int32  pixels;
    int32  pixels_per_block;
    int32  pixels_per_scanline;
    enum { SZIP_INIT, SZIP_RUN, SZIP_TERM } szip_state; /* state of the buffer storage */
    enum { SZIP_CLEAN, SZIP_DIRTY } szip_dirty;
} comp_coder_szip_info_t;

#define SZ_H4_REV_2 0x10000 /* special bit to signal revised format */

#ifndef CSZIP_MASTER
HDFLIBAPI funclist_t cszip_funcs; /* functions to perform szip encoding */
#else
funclist_t cszip_funcs = {/* functions to perform szip encoding */
                          HCPcszip_stread,
                          HCPcszip_stwrite,
                          HCPcszip_seek,
                          HCPcszip_inquire,
                          HCPcszip_read,
                          HCPcszip_write,
                          HCPcszip_endaccess,
                          NULL,
                          NULL};
#endif

#endif /* H4_CSZIP_H */
