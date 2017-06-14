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


#ifndef REPACK_AN_H_
#define REPACK_AN_H_

#include "hrepack.h"


#ifdef __cplusplus
extern "C" {
#endif


int copy_an(int32 infile_id,int32 outfile_id,
            int32 ref_in, int32 tag_in,
            int32 ref_out, int32 tag_out,
            char *path, options_t *options);


int copy_vg_an(int32 infile_id,
               int32 outfile_id,
               int32 vgroup_id,
               int32 vgroup_id_out, 
               char *path,
               options_t *options);

int copy_vs_an(int32 infile_id,
               int32 outfile_id,
               int32 vdata_id,
               int32 vdata_id_out, 
               char *path,
               options_t *options);

int copy_an_data(int32 infile_id,
                 int32 outfile_id,
                 int32 ref_in, 
                 int32 tag_in,
                 int32 ref_out, 
                 int32 tag_out,
                 ann_type type, 
                 char *path, 
                 options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_AN_H_ */
