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


#ifndef REPACK_OPTTABLE_H_
#define REPACK_OPTTABLE_H_

#include "hdf.h"
#include "mfhdf.h"

#include "hrepack.h"


#ifdef __cplusplus
extern "C" {
#endif


void options_table_init( options_table_t **tbl );
void options_table_free( options_table_t *table );
int options_add_chunk(obj_list_t *obj_list,int n_objs,int32 *chunk_lengths,
                      int chunk_rank,options_table_t *table );
int options_add_comp(obj_list_t *obj_list,int n_objs,comp_info_t comp,
                     options_table_t *table );
pack_info_t* options_get_object(char *path,options_table_t *table);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_OPTTABLE_H_ */
