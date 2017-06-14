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



#include <assert.h>
#include "hdf.h"
#include "mfhdf.h"
#include "hdiff_dim.h"


/* match name between 2 diff_dim_table_t type lists */
typedef struct diff_match_dim_name_t {
    int32 ref;                    /* reference */
    char  dim_name[H4_MAX_NC_NAME];  /* name */
    int   flags[2];               /* name exists 1, no 0 */  
} diff_match_dim_name_t;

/* table for diff_match_dim_name_t */
typedef struct diff_match_dim_table_t {
    int        size;
    int        nobjs;
    diff_match_dim_name_t *objs;
} diff_match_dim_table_t;

/*-------------------------------------------------------------------------
 * local prototypes
 *-------------------------------------------------------------------------
 */

/* methods for diff_match_dim_table_t */
static void diff_match_dim_table_free( diff_match_dim_table_t *table );
static void diff_match_dim_table_init( diff_match_dim_table_t **tbl );
static void diff_match_dim_table_add ( diff_match_dim_table_t *table, unsigned *flags, char* dim_name, int32 ref);


/*-------------------------------------------------------------------------
 * Function: diff_match_dim
 *
 * Purpose: diff "lone" dimensions. 
 *  Find common dimension names; the algorithm used for this search is the 
 *  cosequential match algorithm and is described in 
 *  Folk, Michael; Zoellick, Bill. (1992). File Structures. Addison-Wesley.
 *
 * Return: void
 *
 * Programmer: Pedro Vicente Nunes, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */
uint32 diff_match_dim(int32 sd1_id,
                      int32 sd2_id,
                      diff_dim_table_t *td1_1,
                      diff_dim_table_t *td1_2,
                      diff_dim_table_t *td2_1,
                      diff_dim_table_t *td2_2,
                      diff_opt_t * opt)
{
    int                    cmp;
    int                    more_names_exist;
    int                    curr1;
    int                    curr2;
    diff_match_dim_table_t *mattbl_file1=NULL;
    diff_match_dim_table_t *mattbl_file2=NULL;
    unsigned               inlist[2]; 
    int                    i;
    uint32                 nfound = 0;
    
#if defined (HDIFF_DEBUG)
    for (i = 0; i < td1_1->nobjs; i++) 
    {
        printf("%s\n", td1_1->objs[i].dim_name);
    }
    for (i = 0; i < td1_2->nobjs; i++) 
    {
        printf("%s\n", td1_2->objs[i].dim_name);
    }
#endif
    
    
   /*-------------------------------------------------------------------------
    * build the list for file 1
    *-------------------------------------------------------------------------
    */
    
    
    more_names_exist = (td1_1->nobjs>0 && td1_2->nobjs>0) ? 1 : 0;
    curr1=0;
    curr2=0;
    
    diff_match_dim_table_init( &mattbl_file1 );
    
    while ( more_names_exist )
    {
        cmp = strcmp( td1_1->objs[curr1].dim_name, td1_2->objs[curr2].dim_name );
        if ( cmp == 0 )
        {
            inlist[0]=1; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file1,inlist,
                td1_1->objs[curr1].dim_name,
                td1_1->objs[curr1].ref);
            
            curr1++;
            curr2++;
        }
        else if ( cmp < 0 )
        {
            inlist[0]=1; inlist[1]=0;
            diff_match_dim_table_add(mattbl_file1,inlist,
                td1_1->objs[curr1].dim_name,
                td1_1->objs[curr1].ref);
            curr1++;
        }
        else 
        {
            inlist[0]=0; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file1,inlist,
                td1_2->objs[curr2].dim_name,
                td1_2->objs[curr2].ref);
            curr2++;
        }
        
        more_names_exist = (curr1<td1_1->nobjs && curr2<td1_1->nobjs) ? 1 : 0;
        
        
    } /* end while */
    
    /* td1_1 did not end */
    if (curr1<td1_1->nobjs)
    {
        while ( curr1<td1_1->nobjs )
        {
            inlist[0]=1; inlist[1]=0;
            diff_match_dim_table_add(mattbl_file1,inlist,
                td1_1->objs[curr1].dim_name,
                td1_1->objs[curr1].ref);
            curr1++;
        }
    }
    
    /* td1_2 did not end */
    if (curr2<td1_2->nobjs)
    {
        while ( curr2<td1_2->nobjs )
        {
            inlist[0]=0; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file1,inlist,
                td1_2->objs[curr2].dim_name,
                td1_2->objs[curr2].ref);
            curr2++;
        }
    }
    
   /*-------------------------------------------------------------------------
    * print the list
    *-------------------------------------------------------------------------
    */
#if defined (HDIFF_DEBUG)
    {
        char     c1, c2;
        printf("---------------------------------------\n");
        printf("list1     list2\n");
        printf("---------------------------------------\n");
        for (i = 0; i < mattbl_file1->nobjs; i++)
        {
            c1 = (char)((mattbl_file1->objs[i].flags[0]) ? 'x' : ' ');
            c2 = (char)((mattbl_file1->objs[i].flags[1]) ? 'x' : ' ');
            printf("%5c %6c    %-15s\n", c1, c2, mattbl_file1->objs[i].dim_name);
        }
        printf("\n");
    }
#endif
    
    
   /*-------------------------------------------------------------------------
    * build the list for file 2
    *-------------------------------------------------------------------------
    */
      
    more_names_exist = (td2_1->nobjs>0 && td2_2->nobjs>0) ? 1 : 0;
    curr1=0;
    curr2=0;
    
    diff_match_dim_table_init( &mattbl_file2 );
    
    while ( more_names_exist )
    {
        cmp = strcmp( td2_1->objs[curr1].dim_name, td2_2->objs[curr2].dim_name );
        if ( cmp == 0 )
        {
            inlist[0]=1; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file2,inlist,
                td2_1->objs[curr1].dim_name,
                td2_1->objs[curr1].ref);
            
            curr1++;
            curr2++;
        }
        else if ( cmp < 0 )
        {
            inlist[0]=1; inlist[1]=0;
            diff_match_dim_table_add(mattbl_file2,inlist,
                td2_1->objs[curr1].dim_name,
                td2_1->objs[curr1].ref);
            curr1++;
        }
        else 
        {
            inlist[0]=0; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file2,inlist,
                td2_2->objs[curr2].dim_name,
                td2_2->objs[curr2].ref);
            curr2++;
        }
        
        more_names_exist = (curr1<td2_1->nobjs && curr2<td2_1->nobjs) ? 1 : 0;
        
        
    } /* end while */
    
    /* td2_1 did not end */
    if (curr1<td2_1->nobjs)
    {
        while ( curr1<td2_1->nobjs )
        {
            inlist[0]=1; inlist[1]=0;
            diff_match_dim_table_add(mattbl_file2,inlist,
                td2_1->objs[curr1].dim_name,
                td2_1->objs[curr1].ref);
            curr1++;
        }
    }
    
    /* td2_2 did not end */
    if (curr2<td2_2->nobjs)
    {
        while ( curr2<td2_2->nobjs )
        {
            inlist[0]=0; inlist[1]=1;
            diff_match_dim_table_add(mattbl_file2,inlist,
                td2_2->objs[curr2].dim_name,
                td2_2->objs[curr2].ref);
            curr2++;
        }
    }
    
   /*-------------------------------------------------------------------------
    * print the list
    *-------------------------------------------------------------------------
    */
#if defined (HDIFF_DEBUG)
    {
        char     c1, c2;
        printf("---------------------------------------\n");
        printf("list1     list2\n");
        printf("---------------------------------------\n");
        for (i = 0; i < mattbl_file2->nobjs; i++)
        {
            c1 = (char)((mattbl_file2->objs[i].flags[0]) ? 'x' : ' ');
            c2 = (char)((mattbl_file2->objs[i].flags[1]) ? 'x' : ' ');
            printf("%5c %6c    %-15s\n", c1, c2, mattbl_file2->objs[i].dim_name);
        }
        printf("\n");
    }
#endif
       
   /*-------------------------------------------------------------------------
    * get objects from list1 not in list2
    *-------------------------------------------------------------------------
    */
    
    for (i = 0; i < mattbl_file1->nobjs; i++)
    {
        if ( mattbl_file1->objs[i].flags[0] && ( ! mattbl_file1->objs[i].flags[1] ) )
        {
            int j;
            
            /* search in file 2 for this dimension */
            for (j = 0; j < mattbl_file2->nobjs; j++)
            {
                
                /* same name */
                if ( strcmp( mattbl_file1->objs[i].dim_name, mattbl_file2->objs[j].dim_name ) == 0 )
                {
                    
                    /* and also a lone dim in file 2 */
                    if ( mattbl_file2->objs[j].flags[0] && ( ! mattbl_file2->objs[j].flags[1] ) )
                    {
                        
                        int32 ref1 = mattbl_file1->objs[i].ref;
                        int32 ref2 = mattbl_file2->objs[j].ref;
                        
                        nfound+=diff_sds(sd1_id,
                            sd2_id,
                            ref1,
                            ref2,
                            opt);
                        
                        
                    }
                    
                    
                    
                }
                
                
            }
            
        
        }
    
    }
    
    
    /* free tables */
    diff_match_dim_table_free(mattbl_file1);
    diff_match_dim_table_free(mattbl_file2);
    
    return nfound;
}





/*-------------------------------------------------------------------------
 * Function: diff_match_dim_table_add
 *
 * Purpose: add an entry from a list of dimension names into the match table 
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

static 
void diff_match_dim_table_add (diff_match_dim_table_t *table, 
                               unsigned *flags, 
                               char* dim_name, 
                               int32 ref)
{
    int i;
    
    /* check if name already on match table */
    for (i = 0; i < table->nobjs; i++) 
    {
        /* insert information at position i */
        if ( strcmp(dim_name, table->objs[i].dim_name)==0 )
        {
            if (table->objs[i].flags[0] == 0)
                table->objs[i].flags[0] = flags[0];
            if (table->objs[i].flags[1] == 0)
                table->objs[i].flags[1] = flags[1];
            return;
        }
    }
    
    
    if (table->nobjs == table->size) 
    {
        table->size *= 2;
        table->objs = (diff_match_dim_name_t*)HDrealloc(table->objs, table->size * sizeof(diff_match_dim_name_t));

        for (i = table->nobjs; i < table->size; i++) {
            table->objs[i].ref = -1;
            table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
        }
    }
    
    i = table->nobjs++;
    table->objs[i].ref = ref;
    HDstrcpy(table->objs[i].dim_name,dim_name);
    table->objs[i].flags[0] = flags[0];
    table->objs[i].flags[1] = flags[1];
}


/*-------------------------------------------------------------------------
 * Function: diff_match_dim_table_init
 *
 * Purpose: initialize match table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

static 
void diff_match_dim_table_init( diff_match_dim_table_t **tbl )
{
    int i;
    diff_match_dim_table_t *table = (diff_match_dim_table_t*)HDmalloc(sizeof(diff_match_dim_table_t));

    table->size = 20;
    table->nobjs = 0;
    table->objs = (diff_match_dim_name_t*)HDmalloc(table->size * sizeof(diff_match_dim_name_t));

    for (i = 0; i < table->size; i++) {
        table->objs[i].ref = -1;
        table->objs[i].flags[0] = table->objs[i].flags[1] = -1;
    }
    
    *tbl = table;
}



/*-------------------------------------------------------------------------
 * Function: diff_match_dim_table_free
 *
 * Purpose: free match table 
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

static 
void diff_match_dim_table_free( diff_match_dim_table_t *table )
{
    HDfree(table->objs);
    HDfree(table);
}



/*-------------------------------------------------------------------------
 * Function: diff_dim_table_add
 *
 * Purpose: add an entry of pair REF/NAME into a dimension table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

void diff_dim_table_add(diff_dim_table_t *table, 
                        int ref, 
                        char* name)
{
    int i;
    
    assert(table);
    
    if (table->nobjs == table->size) 
    {
        table->size *= 2;
        table->objs = (diff_dim_name_t*)HDrealloc(table->objs, table->size * sizeof(diff_dim_name_t));

        for (i = table->nobjs; i < table->size; i++) {
            table->objs[i].ref = -1;
        }
    }
    
    i = table->nobjs++;
    table->objs[i].ref = ref;
    HDstrcpy(table->objs[i].dim_name,name);
}



/*-------------------------------------------------------------------------
 * Function: diff_dim_table_init
 *
 * Purpose: initialize dimension table
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

void diff_dim_table_init( diff_dim_table_t **tbl )
{
    int i;
    diff_dim_table_t* table = (diff_dim_table_t*)HDmalloc(sizeof(diff_dim_table_t));

    table->size = 20;
    table->nobjs = 0;
    table->objs = (diff_dim_name_t*)HDmalloc(table->size * sizeof(diff_dim_name_t));

    for (i = 0; i < table->size; i++) {
        table->objs[i].ref = -1;
    }
    
    *tbl = table;
}

/*-------------------------------------------------------------------------
 * Function: diff_dim_table_free
 *
 * Purpose: free dimension table 
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@hdfgroup.org
 *
 * Date: July 16, 2007
 *
 *-------------------------------------------------------------------------
 */

void diff_dim_table_free( diff_dim_table_t *table )
{
    HDfree(table->objs);
    HDfree(table);
}

