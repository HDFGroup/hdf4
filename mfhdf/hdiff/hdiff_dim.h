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

#ifndef HDIFF_DIM_H
#define HDIFF_DIM_H

#include "hdiff.h"

/* dimension SDS  */
typedef struct diff_dim_name_t {
    int32 ref;                      /* reference */
    char  dim_name[H4_MAX_NC_NAME]; /* name */
} diff_dim_name_t;

/* table for diff_dim_name_t */
typedef struct diff_dim_table_t {
    int              size;
    int              nobjs;
    diff_dim_name_t *objs;
} diff_dim_table_t;

#ifdef __cplusplus
extern "C" {
#endif

void diff_dim_table_add(diff_dim_table_t *table, int ref, char *name);
void diff_dim_table_init(diff_dim_table_t **tbl);
void diff_dim_table_free(diff_dim_table_t *table);

uint32 diff_match_dim(int32 sd1_id, int32 sd2_id, diff_dim_table_t *td1_1, diff_dim_table_t *td1_2,
                      diff_dim_table_t *td2_1, diff_dim_table_t *td2_2, diff_opt_t *opt);

#ifdef __cplusplus
}
#endif

#endif /* HDIFF_DIM_H */
