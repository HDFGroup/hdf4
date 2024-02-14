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

#ifndef HREPACK_DIM_H
#define HREPACK_DIM_H

#include "hrepack.h"

/* dimension SDS  */
typedef struct dim_name_t {
    int32 ref;                      /* reference */
    char  dim_name[H4_MAX_NC_NAME]; /* name */
} dim_name_t;

/* table for dim_name_t */
typedef struct dim_table_t {
    int         size;
    int         nobjs;
    dim_name_t *objs;
} dim_table_t;

#ifdef __cplusplus
extern "C" {
#endif

void dim_table_add(dim_table_t *dim_tbl, int ref, char *name);
void dim_table_init(dim_table_t **tbl);
void dim_table_free(dim_table_t *dim_tbl);

void match_dim(int32 sd_in, int32 sd_out, dim_table_t *dt1, dim_table_t *dt2, options_t *options);

#ifdef __cplusplus
}
#endif

#endif /* HREPACK_DIM_H */
