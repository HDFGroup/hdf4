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
 * File:    cnone_priv.h
 * Purpose: Header file for "none" encoding information.
 * Dependencies: should only be included from hcomp_priv.h
 * Invokes: none
 * Contents: Structures & definitions for "none" encoding.  This header
 *              should only be included in hcomp.c and cnone.c.
 *---------------------------------------------------------------------------*/

#ifndef H4_CNONE_PRIV_H
#define H4_CNONE_PRIV_H

#include "hdf_priv.h"

/* "none" [en|de]coding information */
typedef struct {
    int space_holder; /* merely a space holder so compilers don't barf */
} comp_coder_none_info_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t cnone_funcs; /* functions to perform run-length encoding */

/*
 ** from cnone.c
 */

HDFLIBAPI int32 HCPcnone_stread(accrec_t *rec);

HDFLIBAPI int32 HCPcnone_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPcnone_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPcnone_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                 int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                 int16 *pspecial);

HDFLIBAPI int32 HCPcnone_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPcnone_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI int HCPcnone_endaccess(accrec_t *access_rec);

#ifdef __cplusplus
}
#endif

#endif /* H4_CNONE_PRIV_H */
