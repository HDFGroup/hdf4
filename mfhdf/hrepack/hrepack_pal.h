/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef REPACK_PAL_H_
#define REPACK_PAL_H_

#include "hrepack.h"
#include "hrepack_lsttable.h"



#ifdef __cplusplus
extern "C" {
#endif


int copy_pal(char* infname,char* outfname,int32 infile_id,int32 outfile_id,
              list_table_t *list_tbl,options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_PAL_H_ */
