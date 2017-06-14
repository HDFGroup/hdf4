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

#include <stdio.h>
#include <stdlib.h>

#include "hdiff_table.h"

/*-------------------------------------------------------------------------
 * Function: dtable_search
 *
 * Purpose: search the table for tag and ref
 *
 * Return: index on success, -1 on failure
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int dtable_search(dtable_t *table, int32 tag, int32 ref )
{
 uint32 i;
 
 for (i = 0; i < table->nobjs; i++)
  if (table->objs[i].tag == tag && table->objs[i].ref == ref)
   return i;
  
  return -1;
}


/*-------------------------------------------------------------------------
 * Function: dtable_add
 *
 * Purpose: add pair tag/ref and object path to table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void dtable_add(dtable_t *table, int32 tag, int32 ref, char* path)
{
 uint32 i;
 
 if (table->nobjs == table->size) {
  table->size *= 2;
  table->objs = (dobj_info_t*)realloc(table->objs, table->size * sizeof(dobj_info_t));
  
  for (i = table->nobjs; i < table->size; i++) {
   table->objs[i].tag = table->objs[i].ref = -1;
   table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
  }
 }
 
 i = table->nobjs++;
 table->objs[i].tag = tag;
 table->objs[i].ref = ref;
 strcpy(table->objs[i].obj_name,path);
 table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
}



/*-------------------------------------------------------------------------
 * Function: dtable_init
 *
 * Purpose: initialize table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void dtable_init( dtable_t **tbl )
{
 uint32 i;
 dtable_t* table = (dtable_t*) malloc(sizeof(dtable_t));
 
 table->size = 20;
 table->nobjs = 0;
 table->objs = (dobj_info_t*) malloc(table->size * sizeof(dobj_info_t));
 
 for (i = 0; i < table->size; i++) {
  table->objs[i].tag = table->objs[i].ref = -1;
  table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
 }
 
 *tbl = table;
}

/*-------------------------------------------------------------------------
 * Function: dtable_free
 *
 * Purpose: free table memory
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void dtable_free( dtable_t *table )
{
 free(table->objs);
 free(table);
}

/*-------------------------------------------------------------------------
 * Function: dtable_print
 *
 * Purpose: print object list
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 25, 2003
 *
 *-------------------------------------------------------------------------
 */

void dtable_print(dtable_t *table, char* header)
{
 uint32 i;

 if ( ! table->nobjs ) /* nothing to do */
     return;

 printf("---------------------------------------\n");
 printf("%s %5s %6s    %-15s\n", header, "Tag", "Ref", "Name");
 printf("---------------------------------------\n");

 for (i = 0; i < table->nobjs; i++)
 {
  printf("       %5ld %6ld    %-15s\n", 
   table->objs[i].tag, 
   table->objs[i].ref, 
   table->objs[i].obj_name);
 }

}
