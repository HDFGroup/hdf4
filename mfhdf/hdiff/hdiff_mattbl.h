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

#ifndef HDIFF_MATTBL_H
#define HDIFF_MATTBL_H

#include "hdf.h"
#include "mfhdf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* match objects in 2 HDF files */
typedef struct match_info_t {
    int32 tag1;
    int32 ref1;
    int32 tag2;
    int32 ref2;
    char  obj_name[H4_MAX_NC_NAME]; /* same name for file1 and 2 */
    int   flags[2];                 /* object exists in file=1, no=0 */
} match_info_t;

/* table to store the match info */
typedef struct match_table_t {
    uint32        size;
    uint32        nobjs;
    match_info_t *objs;
} match_table_t;

/* table methods */
void match_table_init(match_table_t **table);
void match_table_free(match_table_t *table);
void match_table_add(match_table_t *table, int *flags, char *path, int32 tag1, int32 ref1, int32 tag2,
                     int32 ref2);

#ifdef __cplusplus
}
#endif

#endif /* HDIFF_MATTBL_H */
