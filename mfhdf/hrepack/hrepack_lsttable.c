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
#include <assert.h>


#include "hrepack_lsttable.h"

/*-------------------------------------------------------------------------
 * Function: list_table_search
 *
 * Purpose: linear search the table for tag and ref
 *
 * Return: index on success, -1 on failure
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 3, 2003
 *
 *-------------------------------------------------------------------------
 */

int list_table_search(list_table_t *list_tbl, int tag, int ref )
{
    int i;
    
    for (i = 0; i < list_tbl->nobjs; i++) 
    {
        if (list_tbl->objs[i].tag == tag && list_tbl->objs[i].ref == ref)
            return i;
    }
    
    return -1;
}


/*-------------------------------------------------------------------------
 * Function: list_table_add
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

void list_table_add(list_table_t *list_tbl, int tag, int ref, char* path)
{
    int path_len;
    int i;
    
    if (list_tbl->nobjs == list_tbl->size) 
    {
        list_tbl->size *= 2;
        list_tbl->objs = (obj_info_t*)HDrealloc(list_tbl->objs, list_tbl->size * sizeof(obj_info_t));
        
        for (i = list_tbl->nobjs; i < list_tbl->size; i++) 
        {
            list_tbl->objs[i].tag = -1;
            list_tbl->objs[i].ref = -1;
            list_tbl->objs[i].path = NULL;
        }
    }
    
    i = list_tbl->nobjs++;
    list_tbl->objs[i].tag = tag;
    list_tbl->objs[i].ref = ref;

    /* copy the path over */
    path_len = HDstrlen(path);
    list_tbl->objs[i].path = (char *)HDmalloc(path_len+1);
    HIstrncpy(list_tbl->objs[i].path, path, path_len+1);
}



/*-------------------------------------------------------------------------
 * Function: list_table_init
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

void list_table_init( list_table_t **tbl )
{
    int i;
    list_table_t* list_tbl = (list_table_t*)HDmalloc(sizeof(list_table_t));
    
    list_tbl->size = 20;
    list_tbl->nobjs = 0;
    list_tbl->objs = (obj_info_t*)HDmalloc(list_tbl->size * sizeof(obj_info_t));
    
    for (i = 0; i < list_tbl->size; i++) 
    {
        list_tbl->objs[i].tag = -1;
        list_tbl->objs[i].ref = -1;
        
    }
    
    *tbl = list_tbl;
}

/*-------------------------------------------------------------------------
 * Function: list_table_free
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

void list_table_free( list_table_t *list_tbl )
{
    int i;

    for (i = 0; i < list_tbl->nobjs; i++) 
    {
        assert(list_tbl->objs[i].path);
        HDfree(list_tbl->objs[i].path);
    }
    HDfree(list_tbl->objs);
    HDfree(list_tbl);
}


/*-------------------------------------------------------------------------
 * Function: list_table_check
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

const char* list_table_check(list_table_t *list_tbl, char*obj_name)
{
    int   i;
    int32 tag;
    
    for (i = 0; i < list_tbl->nobjs; i++)
    {
        if (HDstrcmp(list_tbl->objs[i].path,obj_name)==0)
        {
            /* found the name; check if it is an SDS or Image */
            tag=list_tbl->objs[i].tag;
            if (tag==DFTAG_SD  ||
                tag==DFTAG_SDG ||
                tag==DFTAG_NDG ||
                tag==DFTAG_RI  ||
                tag==DFTAG_CI  ||
                tag==DFTAG_RIG ||
                tag==DFTAG_RI8 ||
                tag==DFTAG_CI8 ||
                tag==DFTAG_II8 ) return NULL; 
            else return "not compressible/chunk object";
        }
    }
    
    return "not found";
}


/*-------------------------------------------------------------------------
 * Function: list_table_print
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

void list_table_print(list_table_t *list_tbl)
{
    int i;
    
    printf("---------------------------------------\n");
    printf("%5s %6s    %-15s\n", "Tag", "Ref", "Name");
    printf("---------------------------------------\n");
    
    for (i = 0; i < list_tbl->nobjs; i++)
    {
        printf("%5d %6d    %-15s\n", 
            list_tbl->objs[i].tag, 
            list_tbl->objs[i].ref, 
            list_tbl->objs[i].path);
    }
    
}


