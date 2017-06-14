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

#include "hdiff_mattbl.h"


/*-------------------------------------------------------------------------
 * Function: match_table_add
 *
 * Purpose: mark object is in file;
 *  flag[0] = file1
 *  flag[1] = file2
 *  object exists in file = flag = 1
 *  does not exist        = flag = 0
 * the key is <path, tag, ref >
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 25, 2003
 *
 *-------------------------------------------------------------------------
 */

void match_table_add (match_table_t *table, 
                      unsigned *flags, 
                      char* path, 
                      int32 tag1, 
                      int32 ref1,
                      int32 tag2, 
                      int32 ref2 )
{
    uint32 i;
    
    if (table->nobjs == table->size) {
        table->size *= 2;
        table->objs = (match_info_t*)HDrealloc(table->objs, table->size * sizeof(match_info_t));

        for (i = table->nobjs; i < table->size; i++) {
            table->objs[i].tag1 = table->objs[i].ref1 = -1;
            table->objs[i].tag2 = table->objs[i].ref2 = -1;
            table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
        }
    }
    
    i = table->nobjs++;
    table->objs[i].tag1 = tag1;
    table->objs[i].ref1 = ref1;
    table->objs[i].tag2 = tag2;
    table->objs[i].ref2 = ref2;
    HDstrcpy(table->objs[i].obj_name,path);
    table->objs[i].flags[0] = flags[0];
    table->objs[i].flags[1] = flags[1];
}


/*-------------------------------------------------------------------------
 * Function: match_table_init
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

void match_table_init( match_table_t **tbl )
{
    uint32 i;
    match_table_t* table = (match_table_t*)HDmalloc(sizeof(match_table_t));

    table->size = 20;
    table->nobjs = 0;
    table->objs = (match_info_t*)HDmalloc(table->size * sizeof(match_info_t));

    for (i = 0; i < table->size; i++) {
        table->objs[i].tag1 = table->objs[i].ref1 = -1;
        table->objs[i].tag2 = table->objs[i].ref2 = -1;
        table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
    }
    
    *tbl = table;
}



/*-------------------------------------------------------------------------
 * Function: match_table_free
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

void match_table_free( match_table_t *table )
{
    HDfree(table->objs);
    HDfree(table);
}



