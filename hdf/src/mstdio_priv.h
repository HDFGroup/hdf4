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
 * File:    mstdio_priv.h
 * Purpose: Header file for stdio-like modeling information.
 * Dependencies: should be included after hdf.h
 * Contents: Structures & definitions for stdio modeling.  This header
 *              should only be included in hcomp.c and mstdio.c.
 *---------------------------------------------------------------------------*/

#ifndef H4_MSTDIO_PRIV_H
#define H4_MSTDIO_PRIV_H

#include "hdf_priv.h"

/* model information about stdio model */
typedef struct {
    int32 pos; /* position ? */
} comp_model_stdio_info_t;

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI funclist_t mstdio_funcs;

/*
 ** from mstdio.c
 */

HDFLIBAPI int32 HCPmstdio_stread(accrec_t *rec);

HDFLIBAPI int32 HCPmstdio_stwrite(accrec_t *rec);

HDFLIBAPI int32 HCPmstdio_seek(accrec_t *access_rec, int32 offset, int origin);

HDFLIBAPI int32 HCPmstdio_inquire(accrec_t *access_rec, int32 *pfile_id, uint16 *ptag, uint16 *pref,
                                  int32 *plength, int32 *poffset, int32 *pposn, int16 *paccess,
                                  int16 *pspecial);

HDFLIBAPI int32 HCPmstdio_read(accrec_t *access_rec, int32 length, void *data);

HDFLIBAPI int32 HCPmstdio_write(accrec_t *access_rec, int32 length, const void *data);

HDFLIBAPI intn HCPmstdio_endaccess(accrec_t *access_rec);

#ifdef __cplusplus
}
#endif

#endif /* H4_MSTDIO_PRIV_H */
