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


#ifndef REPACK_LSTTABLE_H_
#define REPACK_LSTTABLE_H_


#include "hdf.h"
#include "mfhdf.h"

#define PFORMAT  "  %-7s %-4s %-10s %-7s\n" /*chunk info, compression info, ratio, name*/
#define PFORMAT1 "  %-7s %-7s %-7s"   /*chunk info, compression info, name*/

#ifdef __cplusplus
extern "C" {
#endif


/*
 structure to store the tag/ref and path of an object 
 the pair tag/ref uniquely identifies an HDF object 
 the path field is used for information purposes
*/
typedef struct obj_info_t {
 int   tag;
 int   ref;
 char  *path;  /*  build a path for each object using the vgroup separation symbol "/" 
                *  along the vgroup hierarchy traversal, e.g., the vgroup hierarchy
                *
                *    vg0 -----> vg1 -----> vg2 -----> sds1
                *                   -----> vg3 -----> sds2
                *
                *  would built 6 objects with the paths
                *    
                *    vg0
                *    vg0/vg1
                *    vg0/vg1/vg2
                *    vg0/vg1/vg2/sds1
                *    vg0/vg1/vg3
                *    vg0/vg1/vg3/sds2
                */
} obj_info_t;

/*struct that stores all objects */
typedef struct list_table_t {
 int        size;
 int        nobjs;
 obj_info_t *objs;
} list_table_t;


/* table methods */
void  list_table_init(list_table_t **list_tbl);
void  list_table_free(list_table_t *list_tbl);
int   list_table_search(list_table_t *list_tbl, int tag, int ref );
void  list_table_add(list_table_t *list_tbl, int tag, int ref, char* obj_name);
const char* list_table_check(list_table_t *list_tbl, char*obj_name);
void  list_table_print(list_table_t *list_tbl);

#ifdef __cplusplus
}
#endif


#endif  /* REPACK_LSTTABLE_H_ */
