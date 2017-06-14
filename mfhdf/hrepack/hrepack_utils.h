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


#ifndef REPACK_UTILS_H_
#define REPACK_UTILS_H_

#include "hrepack.h"
#include "hrepack_parse.h"


#ifdef __cplusplus
extern "C" {
#endif


int is_reserved(char*vgroup_class);
char *get_path(char*path_name, char*obj_name);

int  options_get_info(options_t      *options,     /* global options */
                      int32          *chunk_flags, /* chunk flags OUT */
                      HDF_CHUNK_DEF  *chunk_def,   /* chunk definition OUT */
                      int            *info,        /* compression info OUT */
                      int            *szip_mode,   /* compression information OUT */
                      comp_coder_t   *comp_type,   /* compression type OUT  */
                      int            rank,         /* rank of object IN */
                      char           *path,        /* path of object IN */
                      int            ncomps,       /* number of GR image planes (for SZIP), IN */
                      int32          *dimsizes,    /* dimensions (for SZIP), IN */
                      int32          dtype         /* numeric type (for SZIP), IN */
                      );

int set_szip( int   pixels_per_block, /*in */
             int   compression_mode, /* in */
             comp_info *c_info/*out*/);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_UTILS_H_ */
