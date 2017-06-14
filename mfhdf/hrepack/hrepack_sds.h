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


#ifndef REPACK_SDS_H_
#define REPACK_SDS_H_

#include "hrepack.h"
#include "hrepack_lsttable.h"
#include "hrepack_dim.h"



#ifdef __cplusplus
extern "C" {
#endif


int  copy_sds(int32 sd_in,
              int32 sd_out,
              int32 tag,
              int32 ref,
              int32 vgroup_id_out_par,
              char*group_name,
              options_t *options,
              list_table_t *list_tbl,
              dim_table_t *td1,
              dim_table_t *td2,
              int32 infile_id,
              int32 outfile_id);



int copy_sds_attrs(int32 sds_id,
                   int32 sds_out,
                   int32 nattrs,          
                   options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_SDS_H_ */
