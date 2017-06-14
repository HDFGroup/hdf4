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


#ifndef REPACK_VS_H_
#define REPACK_VS_H_

#include "hrepack.h"
#include "hrepack_lsttable.h"


#ifdef __cplusplus
extern "C" {
#endif


int  copy_vdata_attribute(int32 in, int32 out, int32 findex, intn attrindex);


int  copy_vs( int32 infile_id,
              int32 outfile_id,
              int32 tag,
              int32 ref,               /* ref of input VS */
              int32 vgroup_id_out_par, /* output parent group ID */
              char*path_name,          /* absolute path for input group name */
              options_t *options,
              list_table_t *list_tbl,
              int   is_lone);

int copy_vgroup_attrs(int32 vg_in, 
                      int32 vg_out, 
                      char *path,
                      options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_VS_H_ */
