/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "table.h"

/*-------------------------------------------------------------------------
 * Function: table_search
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

int table_search(table_t *table, int tag, int ref )
{
 int i;
 
 for (i = 0; i < table->nobjs; i++)
  if (table->objs[i].tag == tag && table->objs[i].ref == ref)
   return i;
  
  return -1;
}


/*-------------------------------------------------------------------------
 * Function: table_add
 *
 * Purpose: add tag and ref entry to table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

void table_add(table_t *table, int tag, int ref, char* path)
{
 int i;
 
 if (table->nobjs == table->size) {
  table->size *= 2;
  table->objs = (obj_t*)realloc(table->objs, table->size * sizeof(obj_t));
  
  for (i = table->nobjs; i < table->size; i++) {
   table->objs[i].tag = table->objs[i].ref = -1;
  }
 }
 
 i = table->nobjs++;
 table->objs[i].tag = tag;
 table->objs[i].ref = ref;
 strcpy(table->objs[i].obj_name,path);

}


/*-------------------------------------------------------------------------
 * Function: table_init
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

void table_init( table_t **tbl )
{
 int i;
 table_t* table = (table_t*) malloc(sizeof(table_t));
 
 table->size = 20;
 table->nobjs = 0;
 table->objs = (obj_t*) malloc(table->size * sizeof(obj_t));
 
 for (i = 0; i < table->size; i++) {
  table->objs[i].tag = table->objs[i].ref = -1;
 }
 
 *tbl = table;
}



/*-------------------------------------------------------------------------
 * Function: table_free
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

void table_free( table_t *table )
{
 free(table->objs);
 free(table);
}


/*-------------------------------------------------------------------------
 * Function: table_check
 *
 * Purpose: search the table for valid objects
 *
 * Return: error string or NULL for success
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

char* table_check(table_t *table, char*obj_name)
{
 int   i;
 int32 tag;
 
 for (i = 0; i < table->nobjs; i++)
 {
  if (strcmp(table->objs[i].obj_name,obj_name)==0)
  {
   /* found the name; check if it is an SDS or Image */
   tag=table->objs[i].tag;
   if (tag==DFTAG_SD  ||
       tag==DFTAG_SDG ||
       tag==DFTAG_NDG ||
       tag==DFTAG_RI  ||
       tag==DFTAG_CI  ||
       tag==DFTAG_RIG ||
       tag==DFTAG_RI8 ||
       tag==DFTAG_CI8 ||
       tag==DFTAG_II8 ) return NULL; else return "not compressible/chunk object";
  }
 }
  
  return "not found";
}


