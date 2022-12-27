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


#include "hdiff.h"
#include "hdiff_list.h"
#include "hdiff_mattbl.h"

/*-------------------------------------------------------------------------
 * Function: hdiff
 *
 * Purpose: find differences between two HDF files
 *
 * Return: number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 22, 2003
 *
 *-------------------------------------------------------------------------
 */

uint32 hdiff(const char *fname1, 
             const char *fname2, 
             diff_opt_t *opt)
{
    int32     sd1_id=-1,                 
              sd2_id=-1,
              gr1_id=-1,                 
              gr2_id=-1,
              file1_id=-1,                 
              file2_id=-1;
    uint32    nobjects1;
    uint32    nobjects2;
    uint32    nfound=0;  
    int       err;
    dtable_t  *list1=NULL;
    dtable_t  *list2=NULL;
    /* file 1 */
    diff_dim_table_t *td1_1=NULL;
    diff_dim_table_t *td1_2=NULL;
    /* file 2 */
    diff_dim_table_t *td2_1=NULL;
    diff_dim_table_t *td2_2=NULL;
    
    /* initialize tables */
    dtable_init(&list1);
    dtable_init(&list2);
    diff_dim_table_init(&td1_1);
    diff_dim_table_init(&td1_2);
    diff_dim_table_init(&td2_1);
    diff_dim_table_init(&td2_2);
    
   /*-------------------------------------------------------------------------
    * get a list of objects for both files
    *-------------------------------------------------------------------------
    */
    
    nobjects1=hdiff_list(fname1, 
        list1,
        td1_1,
        td1_2,
        &err);
    
    if (err)
        goto out;
    
    nobjects2=hdiff_list(fname2, 
        list2,
        td2_1,
        td2_2,
        &err);
    
    if (err)
        goto out;
    
    if (opt->verbose) 
    {
        dtable_print(list1, "file 1");
        dtable_print(list2, "file 2");
    }
    
   /*-------------------------------------------------------------------------
    * open file IDs
    *-------------------------------------------------------------------------
    */
    
    if ((file1_id = Hopen(fname1, DFACC_READ, (int16)0))==FAIL)
    {
        printf("Exiting: Hopen failed on <%s>", fname1);
        goto out;
    }
    
    if ((file2_id = Hopen(fname2, DFACC_READ, (int16)0))==FAIL)
    {
        printf("Exiting: Hopen failed on <%s>", fname2);
        goto out;
    }
    
   /*-------------------------------------------------------------------------
    * SD interface
    *-------------------------------------------------------------------------
    */
    
    if ((sd1_id = SDstart(fname1, DFACC_RDONLY))==FAIL) 
    {
        printf("SDstart failed on <%s>", fname1);
        goto out;
    }
   
    if ((sd2_id = SDstart(fname2, DFACC_RDONLY))==FAIL) 
    {
        printf("SDstart failed on <%s>", fname2);
        goto out;
    }
    
   /*-------------------------------------------------------------------------
    * GR interface
    *-------------------------------------------------------------------------
    */
    
    if ((gr1_id = GRstart(file1_id))==FAIL) 
    {
        printf("GRstart failed on <%s>", fname1);
        goto out;
    }
   
    if ((gr2_id = GRstart(file2_id))==FAIL) 
    {
        printf("GRstart failed on <%s>", fname2);
        goto out;
    }
    
   /*-------------------------------------------------------------------------
    * find common objects
    *-------------------------------------------------------------------------
    */
       
    nfound=match(nobjects1,
        list1,
        nobjects2,
        list2,
        sd1_id,
        gr1_id,
        file1_id,
        sd2_id,
        gr2_id,
        file2_id,
        opt);
    
    nfound+=diff_match_dim(sd1_id,
        sd2_id,
        td1_1,
        td1_2,
        td2_1,
        td2_2,
        opt
        );
      
   /*-------------------------------------------------------------------------
    * global attributes
    *-------------------------------------------------------------------------
    */
    if (opt->ga == 1) 
        nfound+=gattr_diff(sd1_id, sd2_id, opt);
      
   /*-------------------------------------------------------------------------
    * close
    *-------------------------------------------------------------------------
    */
    
    if ( SDend(sd1_id)==FAIL) 
    {
        printf("Error: SDend failed on <%s>", fname1);
        goto out;
    }
    if (SDend(sd2_id)==FAIL) 
    {
        printf("Error: SDend failed on <%s>", fname2);
        goto out;
    }
    if (GRend(gr1_id)==FAIL) 
    {
        printf("Error: GRend failed on <%s>", fname1);
        goto out;
    }
    if (GRend(gr2_id)==FAIL) 
    {
        printf("Error: GRend failed on <%s>", fname2);
        goto out;
    }
    if (Hclose(file1_id)==FAIL) 
    {
        printf("Error: Hclose failed on <%s>", fname1);
        goto out;
    }
    if (Hclose(file2_id)==FAIL) 
    {
        printf("Error: Hclose failed on <%s>", fname2);
        goto out;
    }
    
    
    /* free tables */
    dtable_free(list1);
    dtable_free(list2);
    diff_dim_table_free(td1_1);
    diff_dim_table_free(td1_2);
    diff_dim_table_free(td2_1);
    diff_dim_table_free(td2_2);
    
    return nfound;
    
    
out:
    
    opt->err_stat = 1;
    
    /* free tables */
    dtable_free(list1);
    dtable_free(list2);
    diff_dim_table_free(td1_1);
    diff_dim_table_free(td1_2);
    diff_dim_table_free(td2_1);
    diff_dim_table_free(td2_2);
    
    if (sd1_id!=-1)
        SDend(sd1_id);
    if (sd2_id!=-1)
        SDend(sd2_id);
    if (gr1_id!=-1)
        GRend(gr1_id);
    if (gr2_id!=-1)
        GRend(gr2_id);
    if (file1_id!=-1)
        Hclose(file1_id);
    if (file2_id!=-1)
        Hclose(file2_id);
    
    return 0;
}



/*-------------------------------------------------------------------------
 * Function: match
 *
 * Purpose: Find common objects; the algorithm used for this search is the 
 *  cosequential match algorithm and is described in 
 *  Folk, Michael; Zoellick, Bill. (1992). File Structures. Addison-Wesley.
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 22, 2003
 *
 *-------------------------------------------------------------------------
 */
uint32 match( uint32 nobjects1, 
              dtable_t *list1,
              uint32 nobjects2, 
              dtable_t *list2,
              int32 sd1_id, 
              int32 gr1_id, 
              int32 file1_id,                
              int32 sd2_id, 
              int32 gr2_id, 
              int32 file2_id,
              diff_opt_t *opt )
{
    int           cmp;
    int           more_names_exist = (nobjects1>0 && nobjects2>0) ? 1 : 0;
    uint32        curr1=0;
    uint32        curr2=0;
    uint32        nfound=0;
    match_table_t *mattbl=NULL;
    unsigned      infile[2]; 
    char          c1, c2;
    uint32        i;
    
   /*-------------------------------------------------------------------------
    * build the list
    *-------------------------------------------------------------------------
    */
    match_table_init( &mattbl );
    
    while ( more_names_exist )
    {
        cmp = strcmp( list1->objs[curr1].obj_name, list2->objs[curr2].obj_name );
        if ( cmp == 0 )
        {
            infile[0]=1; infile[1]=1;
            match_table_add(mattbl,infile,
                list1->objs[curr1].obj_name,
                list1->objs[curr1].tag,
                list1->objs[curr1].ref,
                list2->objs[curr2].tag,
                list2->objs[curr2].ref);
            
            curr1++;
            curr2++;
        }
        else if ( cmp < 0 )
        {
            infile[0]=1; infile[1]=0;
            match_table_add(mattbl,infile,
                list1->objs[curr1].obj_name,
                list1->objs[curr1].tag,
                list1->objs[curr1].ref,
                -1,
                -1);
            curr1++;
        }
        else 
        {
            infile[0]=0; infile[1]=1;
            match_table_add(mattbl,infile,
                list2->objs[curr2].obj_name,
                -1,
                -1,
                list2->objs[curr2].tag,
                list2->objs[curr2].ref);
            curr2++;
        }
        
        more_names_exist = (curr1<nobjects1 && curr2<nobjects2) ? 1 : 0;
        
        
    } /* end while */
    
    /* list1 did not end */
    if (curr1<nobjects1)
    {
        while ( curr1<nobjects1 )
        {
            infile[0]=1; infile[1]=0;
            match_table_add(mattbl,infile,
                list1->objs[curr1].obj_name,
                list1->objs[curr1].tag,
                list1->objs[curr1].ref,
                -1,
                -1);
            curr1++;
        }
    }
    
    /* list2 did not end */
    if (curr2<nobjects2)
    {
        while ( curr2<nobjects2 )
        {
            infile[0]=0; infile[1]=1;
            match_table_add(mattbl,infile,
                list2->objs[curr2].obj_name,
                -1,
                -1,
                list2->objs[curr2].tag,
                list2->objs[curr2].ref);
            curr2++;
        }
    }
    
   /*-------------------------------------------------------------------------
    * print the list
    *-------------------------------------------------------------------------
    */
    
    if (opt->verbose) 
    {
        printf("---------------------------------------\n");
        printf("file1     file2\n");
        printf("---------------------------------------\n");
        for (i = 0; i < mattbl->nobjs; i++)
        {
            c1 = (char)((mattbl->objs[i].flags[0]) ? 'x' : ' ');
            c2 = (char)((mattbl->objs[i].flags[1]) ? 'x' : ' ');
            printf("%5c %6c    %-15s\n", c1, c2, mattbl->objs[i].obj_name);
        }
        printf("\n");
    }
    
    
   /*-------------------------------------------------------------------------
    * do the diff for objects
    *-------------------------------------------------------------------------
    */
    
    for (i = 0; i < mattbl->nobjs; i++)
    {
        if ( mattbl->objs[i].flags[0] && mattbl->objs[i].flags[1] )
        {
            nfound += diff(file1_id,
                file2_id,
                sd1_id,
                sd2_id,
                gr1_id,
                gr2_id,
                mattbl->objs[i].obj_name, 
                mattbl->objs[i].obj_name, 
                mattbl->objs[i].tag1,
                mattbl->objs[i].ref1,
                mattbl->objs[i].tag2,
                mattbl->objs[i].ref2,
                opt );
        }
    }
    
    
    /* free table */
    match_table_free(mattbl);
    return nfound;
}

/*-------------------------------------------------------------------------
 * Function: diff
 *
 * Purpose: switch between types and choose the diff function
 *
 * Return: Number of differences found
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: August 25, 2003
 *
 *-------------------------------------------------------------------------
 */

uint32 diff( int32 file1_id,
             int32 file2_id,
             int32 sd1_id,
             int32 sd2_id,
             int32 gr1_id,
             int32 gr2_id,
             char *obj1_name,
             char *obj2_name,
             int32 tag1,
             int32 ref1,
             int32 tag2,
             int32 ref2,
             diff_opt_t *opt )
{
    uint32 nfound=0;
    
    switch ( tag1 )
    {
    case DFTAG_SD:  /* Scientific Data */
    case DFTAG_SDG: /* Scientific Data Group */
    case DFTAG_NDG: /* Numeric Data Group */
        
        nfound=diff_sds(sd1_id,sd2_id,ref1,ref2,opt);
        break;
        
    case DFTAG_VG: 
        break;
        
    case DFTAG_RI:  /* Raster Image */
    case DFTAG_CI:  /* Compressed Image */
    case DFTAG_RIG: /* Raster Image Group */
    case DFTAG_RI8: /* Raster-8 image */
    case DFTAG_CI8: /* RLE compressed 8-bit image */
    case DFTAG_II8: /* IMCOMP compressed 8-bit image */
        
        if (opt->gr == 1) 
            nfound=diff_gr(gr1_id,gr2_id,ref1,ref2,opt);
        break;
        
    case DFTAG_VH: 
        
        if (opt->vd == 1) 
            nfound=diff_vs(file1_id,file2_id,ref1,ref2,opt);
        break;
        
    default:
        printf("Tag <%ld> and Tag <%ld>: Comparison not supported for <%s> and <%s> \n", 
            tag1, tag2, obj1_name, obj2_name);
        break;
    } 
    
    return nfound;
}

