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


#include <string.h>
#include <stdio.h>

#include "hrepack_opttable.h"



/*-------------------------------------------------------------------------
 * Function: options_table_init
 *
 * Purpose: init options table
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void options_table_init( options_table_t **tbl )
{
    int i;
    options_table_t* op_tbl = (options_table_t*)HDmalloc(sizeof(options_table_t));
    
    op_tbl->size   = 3;
    op_tbl->nelems = 0;
    op_tbl->objs   = (pack_info_t*)HDmalloc(op_tbl->size * sizeof(pack_info_t));
    
    for (i = 0; i < op_tbl->size; i++) {
        HDstrcpy(op_tbl->objs[i].objpath,"\0");
        op_tbl->objs[i].comp.info  = -1;
        op_tbl->objs[i].comp.type  = COMP_CODE_NONE;
        op_tbl->objs[i].chunk.rank = -1;
    }
    
    *tbl = op_tbl;
}

/*-------------------------------------------------------------------------
 * Function: options_table_free
 *
 * Purpose: free table memory
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void options_table_free( options_table_t *op_tbl )
{
    if (op_tbl->objs!=NULL)
        HDfree(op_tbl->objs);
    if (op_tbl!=NULL)
        HDfree(op_tbl);
}

/*-------------------------------------------------------------------------
 * Function: options_add_chunk
 *
 * Purpose: add a chunking -c option to the option list
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int options_add_chunk(obj_list_t *obj_list,
                      int n_objs,
                      int32 *chunk_lengths,
                      int chunk_rank,
                      options_table_t *op_tbl )
{
    int i, j, k, I, added=0, found=0;
    
    if (op_tbl->nelems+n_objs >= op_tbl->size) {
        op_tbl->size += n_objs;
        op_tbl->objs = (pack_info_t*)HDrealloc(op_tbl->objs, op_tbl->size * sizeof(pack_info_t));
        for (i = op_tbl->nelems; i < op_tbl->size; i++) {
            HDstrcpy(op_tbl->objs[i].objpath,"\0");
            op_tbl->objs[i].comp.info  = -1;
            op_tbl->objs[i].comp.type  = COMP_CODE_NONE;
            op_tbl->objs[i].chunk.rank = -1;
        }
    }
    
    /* search if this object is already in the table; "path" is the key */
    if (op_tbl->nelems>0)
    {
        /* go tru the supplied list of names */
        for (j = 0; j < n_objs; j++) 
        {
            /* linear table search */
            for (i = 0; i < op_tbl->nelems; i++) 
            {
                /*already on the table */
                if (HDstrcmp(obj_list[j].obj,op_tbl->objs[i].objpath)==0)
                {
                    /* already chunk info inserted for this one; exit */
                    if (op_tbl->objs[i].chunk.rank>0)
                    {
                        printf("Input Error: chunk information already inserted for <%s>\n",obj_list[j].obj);
                        return FAIL;
                    }
                    /* insert the chunk info */
                    else
                    {
                        op_tbl->objs[i].chunk.rank = chunk_rank;
                        for (k = 0; k < chunk_rank; k++) 
                            op_tbl->objs[i].chunk.chunk_lengths[k] = chunk_lengths[k];
                        found=1;
                        break;
                    }
                } /* if */
            } /* i */
            
            if (found==0)
            {
                /* keep the grow in a temp var */
                I = op_tbl->nelems + added;  
                added++;
                HDstrcpy(op_tbl->objs[I].objpath,obj_list[j].obj);
                op_tbl->objs[I].chunk.rank = chunk_rank;
                for (k = 0; k < chunk_rank; k++) 
                    op_tbl->objs[I].chunk.chunk_lengths[k] = chunk_lengths[k];
            }
        } /* j */ 
    }
    
    /* first time insertion */
    else
    {
        /* go tru the supplied list of names */
        for (j = 0; j < n_objs; j++) 
        {
            I = op_tbl->nelems + added;  
            added++;
            HDstrcpy(op_tbl->objs[I].objpath,obj_list[j].obj);
            op_tbl->objs[I].chunk.rank = chunk_rank;
            for (k = 0; k < chunk_rank; k++) 
                op_tbl->objs[I].chunk.chunk_lengths[k] = chunk_lengths[k];
        }
    }
    
    op_tbl->nelems+= added;
    
    return SUCCEED;
}



/*-------------------------------------------------------------------------
 * Function: options_add_comp
 *
 * Purpose: add a compression -t option to the option list
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int options_add_comp(obj_list_t *obj_list,
                     int n_objs,
                     comp_info_t comp,
                     options_table_t *op_tbl )
{
    int i, j, I, added=0, found=0;
    
    if (op_tbl->nelems+n_objs >= op_tbl->size) {
        op_tbl->size += n_objs;
        op_tbl->objs = (pack_info_t*)HDrealloc(op_tbl->objs, op_tbl->size * sizeof(pack_info_t));
        for (i = op_tbl->nelems; i < op_tbl->size; i++) {
            HDstrcpy(op_tbl->objs[i].objpath,"\0");
            op_tbl->objs[i].comp.info  = -1;
            op_tbl->objs[i].comp.type  = COMP_CODE_NONE;
            op_tbl->objs[i].chunk.rank = -1;
        }
    }
    
    /* search if this object is already in the table; "path" is the key */
    if (op_tbl->nelems>0)
    {
        /* go tru the supplied list of names */
        for (j = 0; j < n_objs; j++) 
        {
            /* linear table search */
            for (i = 0; i < op_tbl->nelems; i++) 
            {
                /*already on the table */
                if (HDstrcmp(obj_list[j].obj,op_tbl->objs[i].objpath)==0)
                {
                    /* already COMP info inserted for this one; exit */
                    if (op_tbl->objs[i].comp.type>0)
                    {
                        printf("Input Error: compression information already inserted for <%s>\n",obj_list[j].obj);
                        return FAIL;
                    }
                    /* insert the comp info */
                    else
                    {
                        op_tbl->objs[i].comp = comp;
                        found=1;
                        break;
                    }
                } /* if */
            } /* i */
            
            if (found==0)
            {
                /* keep the grow in a temp var */
                I = op_tbl->nelems + added;  
                added++;
                HDstrcpy(op_tbl->objs[I].objpath,obj_list[j].obj);
                op_tbl->objs[I].comp = comp;
            }
        } /* j */ 
    }
    
    /* first time insertion */
    else
    {
        /* go tru the supplied list of names */
        for (j = 0; j < n_objs; j++) 
        {
            I = op_tbl->nelems + added;  
            added++;
            HDstrcpy(op_tbl->objs[I].objpath,obj_list[j].obj);
            op_tbl->objs[I].comp = comp;
        }
    }
    
    op_tbl->nelems+= added;
    
    return 0;
}

/*-------------------------------------------------------------------------
 * Function: options_get_object
 *
 * Purpose: get object from table; "path" is the key
 *
 * Return: information for one object, contains PATH, CHUNK info and COMP info
 *
 *-------------------------------------------------------------------------
 */

pack_info_t* options_get_object(char *path,
                                options_table_t *op_tbl)
{
    int i;
    
    for ( i = 0; i < op_tbl->nelems; i++) 
    {
        /* found it */
        if (HDstrcmp(op_tbl->objs[i].objpath,path)==0)
        {
            return (&op_tbl->objs[i]);
        }
    }
    
    return NULL;
}




