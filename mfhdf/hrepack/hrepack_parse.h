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


#ifndef REPACK_PARSE_H_
#define REPACK_PARSE_H_

#include "hdf.h"
#include "mfhdf.h"
#include "hrepack.h"


#ifdef __cplusplus
extern "C" {
#endif

int parse_number(char *str);


/* compression */

obj_list_t* parse_comp(const char *str, int *n_objs, comp_info_t *comp);
const char* get_scomp(comp_coder_t code);

/* chunking */

obj_list_t* parse_chunk(const char *str, int *n_objs, int32 *chunk_lengths, int *chunk_rank);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_PARSE_H_ */
