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


#include "hdf.h"
#include "mfhdf.h"
#include "hrepack.h"
#include "hrepack_utils.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"
#include "hrepack_sds.h"
#include "hrepack_gr.h"
#include "hrepack_vs.h"
#include "hrepack_an.h"
#include "hrepack_vg.h"
#include "hrepack_dim.h"


int list_vg (int32 infile_id,int32 outfile_id,int32 sd_id,int32 sd_out,int32 gr_id,int32 gr_out,list_table_t *list_tbl,dim_table_t *td1,dim_table_t *td2,options_t *options);
int list_gr (int32 infile_id,int32 outfile_id,int32 gr_id,int32 gr_out,list_table_t *list_tbl,options_t *options);
int list_sds(int32 infile_id,int32 outfile_id,int32 sd_id, int32 sd_out,list_table_t *list_tbl,dim_table_t *td1,dim_table_t *td2,options_t *options);
int list_vs (int32 infile_id,int32 outfile_id,list_table_t *list_tbl,options_t *options);
int list_glb(int32 infile_id,int32 outfile_id,int32 sd_id,int32 sd_out,int32 gr_id,int32 gr_out,list_table_t *list_tbl,options_t *options);
int list_pal(const char* infname,const char* outfname,list_table_t *list_tbl,options_t *options);
int list_an (int32 infile_id,int32 outfile_id,options_t *options);


/*-------------------------------------------------------------------------
 * Function: list
 *
 * Purpose: locate all HDF objects in the file and compress them using options
 *
 * Return: SUCCEED, FAIL
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 * Modifications: September 9, 2007
 *  Check for duplicate Vgroup insertions
 *
 * Description:
 *
 * A main loop is used to locate all the objects in the file. The algorithm used is: 
 * 1) Obtain the number of lone vgroups in the HDF file, i.e., vgroups that are at
 *     the top of the grouping hierarchy, in a file.
 * 2) Do a loop for each one of these groups. In each iteration a table is updated 
 *    with the tag/reference pair of an object. 
 *    2.1) Obtain the pairs of tag/references for the group 
 *    2.2) Switch between the tag of the current object. Four cases are possible: 
 *         1) Object is a group: recursively repeat the process (obtain the pairs of
 *            tag/references for this group and do another tag switch).
 *            If the Vgroup has not been visited
 *             Add the object to the table and continue recursion
 *            else
 *             Open it, create the insertion into its parent and exit the recursion.
 *         2) Object is a dataset: Add the object to the table.
 *         3) Object is an image: Add the object to the table.  
 *         4) Object is a vdata: Add the object to the table. 
 * 3) Read all the HDF interfaces (SDS, GR and VS), checking for objects that are 
 *    already in the table (meaning they belong to a previous inspected group, 
 *    and should not be added).  These objects belong to a root group. 
 * 4) Read all global attributes and annotations. 
 *
 * Modification: January 15, 2015 - JIRA issue HDFFR-1428
 *	The output file contains a vgroup of class RIG0.0 eventhough the input
 *	file does not contain any GR elements, such as raster images or GR file
 *	attribute.  That is because GRstart is called on the output file
 *	regardless of whether there are any GR items in the input file.  Hrepack
 *	will be reviewed and revised for correctness and efficiency (JIRA issue
 *	HDFFR-1395.)  However, in this release, 4.2.11, an immediate solution is
 *	applied to eliminate the unnecessary RIG0.0 vgroup.  The input file is
 *	checked for GR elements, before any GR activity is invoked on the
 *	output file. -BMR
 *	
 *-------------------------------------------------------------------------
 */


int list_main(const char* infname,
         const char* outfname, 
         options_t *options)
{
    list_table_t *list_tbl=NULL;     /* list of objects */
    dim_table_t  *td1=NULL;       /* dimensions */
    dim_table_t  *td2=NULL;       /* dimensions */
    int32        sd_id=FAIL,      /* SD interface identifier */
                 sd_out=FAIL,     /* SD interface identifier */
                 gr_id=FAIL,      /* GR interface identifier */
                 gr_out=FAIL,     /* GR interface identifier */
                 infile_id=FAIL,
                 outfile_id=FAIL,
		 n_rimages,       /* number of raster images in the file */
		 n_file_attrs;    /* number of file attributes */
    intn         has_GRelems = 0;  /* set to 1 when there are GR images or */
                                  /* attributes in the file (HDFFR-1428) */
    int          i;
    const char*  err;

   /*-------------------------------------------------------------------------
    * initialize tables
    *-------------------------------------------------------------------------
    */
    
    list_table_init(&list_tbl);
    dim_table_init(&td1);
    dim_table_init(&td2);
    
   /*-------------------------------------------------------------------------
    * open the input file for read and initialize interfaces
    *-------------------------------------------------------------------------
    */
   
    if ((infile_id  = Hopen (infname,DFACC_READ,(int16)0))==FAIL)
    {
        printf("Cannot open file <%s>\n",infname);
        goto out;
    }
    if ((sd_id  = SDstart (infname, DFACC_READ))==FAIL)
    {
        printf( "Could not start SD for <%s>\n",infname);
        goto out;
    }
    if ((gr_id  = GRstart (infile_id))==FAIL)
    {
        printf( "Could not start GR for <%s>\n",infname);
        goto out;
    }

   /*-------------------------------------------------------------------------
    * set flag to indicate whether the input file contains any GR elements
    *-------------------------------------------------------------------------
    */
    if (GRfileinfo (gr_id, &n_rimages, &n_file_attrs)==FAIL){
        printf( "Could not get info for GR\n");
        goto out;
    }
    if (n_rimages > 0 || n_file_attrs > 0)
        has_GRelems = 1;
   
   /*-------------------------------------------------------------------------
    * create the output file and initialize interfaces
    *-------------------------------------------------------------------------
    */

    if ( options->trip==1 ) 
    {
        if ((outfile_id = Hopen (outfname,DFACC_CREATE,(int16)0))==FAIL)
        {
            printf("Cannot create file <%s>\n",outfname);
            goto out;
        }
        if ((sd_out = SDstart (outfname, DFACC_WRITE))==FAIL)
        {
            printf( "Could not start GR for <%s>\n",outfname);
            goto out;
        }
        
	if (has_GRelems)
            if ((gr_out = GRstart (outfile_id))==FAIL)
            {
                printf( "Could not start GR for <%s>\n",outfname);
                goto out;
            }
    } /* options->trip==1 */

    
    if (options->verbose && options->trip==0)
        printf("Building list of objects in %s...\n",infname);


   /*-------------------------------------------------------------------------
    * iterate thru HDF interfaces 
    *-------------------------------------------------------------------------
    */

    if (options->verbose) 
    {
        printf("-----------------------------------------------\n");
        printf(" Chunk   Filter(compression) Name\n");
        printf("-----------------------------------------------\n");
    }
    
    if (list_vg (infile_id,outfile_id,sd_id,sd_out,gr_id,gr_out,list_tbl,td1,td2,options)<0) 
        goto out;

    /* Only process GR interface if there is any GR elements in the file */
    /* Design note: consider using the same approach for the SD API during the
       improvement of hrepack, HDFFR-1395 */
    if (has_GRelems)
        if (list_gr (infile_id,outfile_id,gr_id,gr_out,list_tbl,options)<0) 
            goto out;
    if (list_sds(infile_id,outfile_id,sd_id,sd_out,list_tbl,td1,td2,options)<0) 
        goto out;
    if (list_vs (infile_id,outfile_id,list_tbl,options)<0) 
        goto out;
    if (list_glb(infile_id,outfile_id,sd_id,sd_out,gr_id,gr_out,list_tbl,options)<0) 
        goto out;
    if (list_pal(infname,outfname,list_tbl,options)<0) 
        goto out;
    if (list_an (infile_id,outfile_id,options)<0) 
        goto out;
    
    
    if ( options->trip==1 ) 
    {
        match_dim(sd_id,sd_out,td1,td2,options);
    }
    

   /*-------------------------------------------------------------------------
    * check for objects in the file table:
    *   1) the input object names are present in the file
    *   2) they are valid objects (SDS or GR)
    * check only if selected objects are given (all==0)
    *-------------------------------------------------------------------------
    */
    
    if ( options->trip==0 ) 
    {
        if (options->verbose)
            printf("Searching for objects to modify...\n");
        
        for ( i = 0; i < options->op_tbl->nelems; i++) 
        {
            char* obj_name=options->op_tbl->objs[i].objpath;
            if (options->verbose)
                printf(PFORMAT1,"","",obj_name);
            
            /* the input object names are present in the file and are valid */
            err=list_table_check(list_tbl,obj_name);
            if (err!=NULL)
            {
                printf("\nError: <%s> %s in file <%s>. Exiting...\n",obj_name,err,infname);
                goto out;
            }
            if (options->verbose)
                printf("...Found\n");
        }
    }
    
    
   /*-------------------------------------------------------------------------
    * close interfaces
    *-------------------------------------------------------------------------
    */
    if (GRend (gr_id)==FAIL)
	printf( "Failed to close GR interface <%s>\n", infname);
    if (SDend (sd_id)==FAIL)
        printf( "Failed to close file <%s>\n", infname);
    if (Hclose (infile_id)==FAIL)
        printf( "Failed to close file <%s>\n", infname);

    if ( options->trip==1 ) 
    {
	if (has_GRelems)
	    if (GRend (gr_out)==FAIL)
		printf( "Failed to close GR interface <%s>\n", outfname);
        if (SDend (sd_out)==FAIL)
            printf( "Failed to close file <%s>\n", outfname);
        if (Hclose (outfile_id)==FAIL)
            printf( "Failed to close file <%s>\n", outfname);
    }


   /*-------------------------------------------------------------------------
    * free tables
    *-------------------------------------------------------------------------
    */
  
    list_table_free(list_tbl);
    dim_table_free(td1);
    dim_table_free(td2);
    
    return SUCCEED;
    
out:
    
    if (list_tbl!=NULL)
        list_table_free(list_tbl);
    if (td1!=NULL)
        dim_table_free(td1);
    if (td2!=NULL)
        dim_table_free(td2);
    if (gr_id!=FAIL)
    {
        if (GRend (gr_id)==FAIL)
            printf( "Failed to close GR interface <%s>\n", infname);
    }
    if (gr_out!=FAIL)
    {
        if (GRend (gr_out)==FAIL)
            printf( "Failed to close GR interface <%s>\n", outfname);
    }
    if (sd_id!=FAIL)
    {
        if (SDend (sd_id)==FAIL)
            printf( "Failed to close SD interface for <%s>\n", infname);
        
    }
    if (sd_out!=FAIL)
    {
        if (SDend (sd_out)==FAIL)
            printf( "Failed to close SD interface for <%s>\n", outfname);
    }
    if (infile_id!=FAIL)
    {
        if (Hclose (infile_id)==FAIL)
            printf( "Failed to close file <%s>\n", infname);
    }
    if (outfile_id!=FAIL)
    {
        if (Hclose (outfile_id)==FAIL)
            printf( "Failed to close file <%s>\n", outfname);
        
    }
    
    return FAIL;
    
}



/*-------------------------------------------------------------------------
 * Function: list_vg
 *
 * Purpose: locate all objects in the file under a group hierarchy
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */


int list_vg(int32 infile_id,
            int32 outfile_id,
            int32 sd_id,
            int32 sd_out,
            int32 gr_id,
            int32 gr_out,
            list_table_t *list_tbl,
            dim_table_t *td1,
            dim_table_t *td2,
            options_t *options)
{
   

    int32  vg_id;          /* vgroup identifier */
    int32  nlones = 0;     /* number of lone vgroups */
    int32  ntagrefs;       /* number of tag/ref pairs in a vgroup */
    int32  *ref_array=NULL;/* buffer to hold the ref numbers of lone vgroups   */
    int32  *tags=NULL;     /* buffer to hold the tag numbers of vgroups   */
    int32  *refs=NULL;     /* buffer to hold the ref numbers of vgroups   */
    int32  vgroup_id_out=0;/* vgroup identifier */
    int32  tag_vg;
    int32  ref_vg;
    char   *vg_name;
    char   *vg_class;
    uint16 name_len;
    int32  i;
    
   /*-------------------------------------------------------------------------
    * initialize the V interface
    *-------------------------------------------------------------------------
    */
    
    if (Vstart(infile_id) == FAIL)  
    {
        return FAIL;
    } 
    
    if (options->trip==1)
    {
        if (Vstart(outfile_id) == FAIL)  
        {
            return FAIL;
        }
    }
    
   /*-------------------------------------------------------------------------
    * get and print the names and class names of all the lone vgroups.
    * first, call Vlone with nlones set to 0 to get the number of
    * lone vgroups in the file, but not to get their reference numbers.
    *-------------------------------------------------------------------------
    */
    nlones = Vlone (infile_id, NULL, nlones );
    
    if (nlones > 0)
    {
       /*
        * use the nlones returned to allocate sufficient space for the
        * buffer ref_array to hold the reference numbers of all lone vgroups,
        */
        ref_array = (int32 *)HDmalloc(sizeof(int32) * nlones);
        
       /*
        * and call Vlone again to retrieve the reference numbers into 
        * the buffer ref_array.
        */
        nlones = Vlone (infile_id, ref_array, nlones);
        
       /*
        * iterate tru each lone vgroup.
        */
        for (i = 0; i < nlones; i++)
        {

            int32 ref = ref_array[i];
	    uint16 name_len;

           /*
            * attach to the current vgroup then get its
            * name and class. note: the current vgroup must be detached before
            * moving to the next.
            */
            if ((vg_id = Vattach (infile_id, ref, "r"))==FAIL)
            {
                printf("Error: Could not attach group with ref <%ld>\n", ref);
                goto out;
            }

	    /* Get vgroup's name */
            if (Vgetnamelen(vg_id, &name_len)==FAIL)
            {
                printf("Error: Could not get name lenght for group with ref <%ld>\n", ref);
                goto out;
            }
            vg_name = (char *) HDmalloc(sizeof(char) * (name_len+1));

            if (Vgetname (vg_id, vg_name)==FAIL)
            {
                printf( "Could not get name for group\n");
                goto out;
                
            }

	    /* Get vgroup's class name */
            if (Vgetclassnamelen(vg_id, &name_len)==FAIL)
            {
                printf("Error: Could not get name lenght for group with ref <%ld>\n", ref);
                goto out;
            }


            vg_class = (char *) HDmalloc(sizeof(char) * (name_len+1));

            if (Vgetclass (vg_id, vg_class)==FAIL)
            {
                printf( "Could not get class for group\n");
                goto out;
            }
            
            /* ignore reserved HDF groups/vdatas */
            if( is_reserved(vg_class))
            {
                if (Vdetach (vg_id)==FAIL)
                {
                    printf( "Could not detach group\n");
                    goto out;
                }
                continue;
            }
            if(HDstrcmp(vg_name,GR_NAME)==0)
            {
                if (Vdetach (vg_id)==FAIL)
                {
                    printf( "Could not detach group\n");
                    goto out;
                }
                continue;
            }
            
            /* get ref, tag */
            if ((ref_vg = VQueryref(vg_id))==FAIL)
            {
                printf( "Failed to get ref for <%s>\n", vg_name);
                goto out;
            }
            if ((tag_vg = VQuerytag(vg_id))==FAIL)
            {
                printf( "Failed to get tag for <%s>\n", vg_name);
                goto out;
            }
            
           /*-------------------------------------------------------------------------
            * add object to table
            *-------------------------------------------------------------------------
            */
            list_table_add(list_tbl,tag_vg,ref_vg,vg_name);
            
            if (options->verbose)
                printf(PFORMAT,"","","",vg_name);  
            
            if (options->trip==1)
            {
                
               /* 
                * create the group in the output file.  the vgroup reference number is set
                * to -1 for creating and the access mode is "w" for writing 
                */
                vgroup_id_out = Vattach (outfile_id, -1, "w");
                if (Vsetname (vgroup_id_out, vg_name)==FAIL)
                {
                    printf("Error: Could not create group <%s>\n", vg_name);
                    goto out;
                }
                if (Vsetclass (vgroup_id_out, vg_class)==FAIL)
                {
                    printf("Error: Could not create group <%s>\n", vg_name);
                    goto out;
                }
                
                if (copy_vgroup_attrs(vg_id,vgroup_id_out,vg_name,options)<0)
                    goto out;
                if (copy_vg_an(infile_id,outfile_id,vg_id,vgroup_id_out,vg_name,options)<0)
                    goto out;
            }
            
            
            
            /* insert objects for this group */
            ntagrefs = Vntagrefs(vg_id);
            if ( ntagrefs > 0 )
            {
                tags = (int32 *)HDmalloc(sizeof(int32) * ntagrefs);
                refs = (int32 *)HDmalloc(sizeof(int32) * ntagrefs);
                if (Vgettagrefs(vg_id, tags, refs, ntagrefs)<0)
                    goto out;
                
                if (vgroup_insert(infile_id,
                    outfile_id,
                    sd_id,
                    sd_out,
                    gr_id,
                    gr_out,
                    vgroup_id_out,
                    vg_name,
                    tags,
                    refs,
                    ntagrefs,
                    list_tbl,
                    td1,
                    td2,
                    options)<0) {
                    goto out;
                }
                
                HDfree(tags);
                tags=NULL;
                HDfree(refs);
                refs=NULL;
            }
            
            if(Vdetach (vg_id)==FAIL)
            {
                printf("Error: Could not detach group <%s>\n", vg_name);
                goto out;
            }
            if (options->trip==1)
            {
                if (Vdetach (vgroup_id_out)==FAIL)
                {
                    printf("Error: Could not detach group <%s>\n", vg_name);
                    goto out;
                }
            }

            HDfree(vg_name);
            
  } /* for nlones */
  
  
  /* free the space allocated */
  if (ref_array!=NULL) 
    HDfree(ref_array);

 } /* if  nlones */
 

/*-------------------------------------------------------------------------
 * terminate access to the V interface
 *-------------------------------------------------------------------------
 */
 
 if (Vend (infile_id)==FAIL)
 {
     printf("Error: Could not end group interface in <%s>\n", vg_name);
     return FAIL;
 }
 if (options->trip==1)
 {
     if (Vend (outfile_id)==FAIL){
         printf("Error: Could not end group interface in <%s>\n", vg_name);
         return FAIL;
     }
 }
 
 return SUCCEED;
 
 
out:
 
 Vend (infile_id);
 if (options->trip==1)
     Vend (outfile_id);
 
 /* free the space allocated */
 if (ref_array!=NULL) 
    HDfree(ref_array);
 if (tags!=NULL) 
    HDfree(tags);
 if (refs!=NULL) 
    HDfree(refs);
 
 return FAIL;
 
}

/*-------------------------------------------------------------------------
 * Function: vgroup_insert
 *
 * Purpose: recursive function to locate objects in vgroups
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int vgroup_insert(int32 infile_id,
                  int32 outfile_id,
                  int32 sd_id,             /* SD interface identifier */
                  int32 sd_out,            /* SD interface identifier */
                  int32 gr_id,             /* GR interface identifier */
                  int32 gr_out,            /* GR interface identifier */
                  int32 vgroup_id_out_par, /* output parent group ID */
                  char*path_name,          /* absolute path for input group name */          
                  int32* in_tags,          /* tag list for parent group */
                  int32* in_refs,          /* ref list for parent group */
                  int npairs,              /* number tag/ref pairs for parent group */
                  list_table_t *list_tbl,
                  dim_table_t *td1,
                  dim_table_t *td2,
                  options_t *options)
{
    int32 vg_id;             /* vgroup identifier for opened group in input */
    int32 ntagrefs;              /* number of tag/ref pairs in a vgroup */
    int32 *tags=NULL;            /* buffer to hold the tag numbers of vgroups   */
    int32 *refs=NULL;            /* buffer to hold the ref numbers of vgroups   */
    int32 vgroup_id_out =-1;         /* vgroup identifier for the created group in output */
    char  *vg_name;
    char  *vg_class;
    char  *path=NULL;
    uint16 name_len;
    int   visited;
    int32 tag;                   
    int32 ref; 
    int   i;

    
    for ( i = 0; i < npairs; i++ ) 
    {       
        tag = in_tags[i];
        ref = in_refs[i];
        
        switch(tag) 
        {
        /*-------------------------------------------------------------------------
         * DFTAG_VG
         *-------------------------------------------------------------------------
         */
        case DFTAG_VG: 

            visited = list_table_search(list_tbl,DFTAG_VG,ref);
            
           /*-------------------------------------------------------------------------
            * open input
            *-------------------------------------------------------------------------
            */
                    
            vg_id = Vattach (infile_id, ref, "r");

	    /* Get vgroup's name */
            if (Vgetnamelen(vg_id, &name_len)==FAIL)
            {
                printf("Error: Could not get name lenght for group with ref <%ld>\n", ref);
                goto out;
            }
            vg_name = (char *) HDmalloc(sizeof(char) * (name_len+1));
            if (Vgetname (vg_id, vg_name)==FAIL)
            {
                printf( "Could not get name for group\n");
                goto out;
                
            }

	    /* Get vgroup's class name */
            if (Vgetclassnamelen(vg_id, &name_len)==FAIL)
            {
                printf("Error: Could not get name lenght for group with ref <%ld>\n", ref);
                goto out;
            }

            vg_class = (char *) HDmalloc(sizeof(char) * (name_len+1));
            if (Vgetclass (vg_id, vg_class)==FAIL)
            {
                printf( "Could not get class for group\n");
                goto out;
            }
            
            /* ignore reserved HDF groups/vdatas */
            if( is_reserved(vg_class))
            {
                if (Vdetach (vg_id)==FAIL)
                {
                    printf( "Could not detach group\n");
                    goto out;
                }
                continue;
            }
            if(HDstrcmp(vg_name,GR_NAME)==0)
            {
                if (Vdetach (vg_id)==FAIL)
                {
                    printf( "Could not detach group\n");
                    goto out;
                }
                continue;
            }
            
            
           /*-------------------------------------------------------------------------
            * create the group in output or create the link
            *-------------------------------------------------------------------------
            */
         
            if (options->trip==1)
            {
            
                if ( visited < 0 )
                    
                {
                    
                   /* 
                    * create the group in the output file.  the vgroup reference number 
                    * is set to -1 for creating and the access mode is "w" for writing 
                    */
                    vgroup_id_out = Vattach (outfile_id, -1, "w");
                    if (Vsetname (vgroup_id_out, vg_name)==FAIL)
                    {
                        printf("Error: Could not create group <%s>\n", vg_name);
                        goto out;
                    }
                    if (Vsetclass (vgroup_id_out, vg_class)==FAIL)
                    {
                        printf("Error: Could not create group <%s>\n", vg_name);
                        goto out;
                    }
                    
                    if (copy_vgroup_attrs(vg_id,vgroup_id_out,path,options)<0)
                        goto out;
                    if (copy_vg_an(infile_id,outfile_id,vg_id,vgroup_id_out,path,options)<0)
                        goto out;
                    
                }
                
                else
                    
                {
                    /* open previously visited group */
                    vgroup_id_out = Vattach (outfile_id, ref, "r");


                }
                    
                
                /* insert the created (or opened) vgroup into its parent */
                if (Vinsert (vgroup_id_out_par, vgroup_id_out)==FAIL)
                {
                    printf("Could not insert group <%s>\n", vg_name);
                    goto out;
                }
                    
            } /* create the group in output or create the link */
            
            
            
             
           /*-------------------------------------------------------------------------
            * if group not visited, add to table and check for more tag/ref pairs
            *-------------------------------------------------------------------------
            */
            
                     
            /* check if already visited */
            if ( visited < 0  ) 
            {
                
                /* initialize path */
                path=get_path(path_name,vg_name);
                
                /* add object to table */
                list_table_add(list_tbl,tag,ref,path);
                
                if (options->verbose)
                    printf(PFORMAT,"","","",path);    
                
                if ( options->trip==0 ) 
                {
                    /*we must go to other groups always */
                }
                
                
                /* insert objects for this group */
                ntagrefs  = Vntagrefs(vg_id);
                if ( ntagrefs > 0 )
                {
                    tags = (int32 *)HDmalloc(sizeof(int32) * ntagrefs);
                    refs = (int32 *)HDmalloc(sizeof(int32) * ntagrefs);
                    if (Vgettagrefs(vg_id, tags, refs, ntagrefs)<0)
                        goto out;
                    /* recurse */
                    if (vgroup_insert(
                        infile_id,
                        outfile_id,
                        sd_id,
                        sd_out,
                        gr_id,
                        gr_out,
                        vgroup_id_out,
                        path,
                        tags,
                        refs,
                        ntagrefs,
                        list_tbl,
                        td1,
                        td2,
                        options)<0) {
                        goto out;
                    }
                    HDfree(tags);
                    tags=NULL;
                    HDfree(refs);
                    refs=NULL;
                } /* ntagrefs > 0 */
                
                
                if (path)
                    HDfree(path);
                
            } /* check if already visited */
            
            
            if(Vdetach (vg_id)==FAIL)
            {
                printf("Error: Could not detach group <%s>\n", vg_name);
                goto out;
            }
            if (options->trip==1)
            {
                if (Vdetach (vgroup_id_out)==FAIL)
                {
                    printf("Error: Could not detach group <%s>\n", vg_name);
                    goto out;
                }
            }
            
            break;
            
            
  /*-------------------------------------------------------------------------
   * SDS
   *-------------------------------------------------------------------------
   */   
            
  case DFTAG_SD:  /* Scientific Data */
  case DFTAG_SDG: /* Scientific Data Group */
  case DFTAG_NDG: /* Numeric Data Group */
      /* copy dataset */
      if (copy_sds(sd_id,
          sd_out,
          tag,ref,
          vgroup_id_out_par,
          path_name,
          options,
          list_tbl,
          td1,
          td2,
          infile_id,
          outfile_id)<0)
          return FAIL;
      
      break;
      
  /*-------------------------------------------------------------------------
   * Image
   *-------------------------------------------------------------------------
   */   
      
  case DFTAG_RI:   /* Raster Image */
  case DFTAG_CI:   /* Compressed Image */
  case DFTAG_RIG:  /* Raster Image Group */
  case DFTAG_RI8:  /* Raster-8 image */
  case DFTAG_CI8:  /* RLE compressed 8-bit image */
  case DFTAG_II8:  /* IMCOMP compressed 8-bit image */
      /* copy GR  */
      if (copy_gr(infile_id,
          outfile_id,
          gr_id,
          gr_out,
          tag,
          ref,
          vgroup_id_out_par,
          path_name,
          options,
          list_tbl)<0)
          return FAIL;
      break;
      
 /*-------------------------------------------------------------------------
  * Vdata
  *-------------------------------------------------------------------------
  */   
      
  case DFTAG_VH:  /* Vdata Header */
      if (copy_vs(infile_id,
          outfile_id,
          tag,
          ref,
          vgroup_id_out_par,
          path_name,
          options,
          list_tbl,
          0)<0)
          return FAIL;
      break;
  } /* switch */
  
 } /* i */
 
 return SUCCEED;
 
out:
 
 if (tags!=NULL)
    HDfree(tags);
 if (refs!=NULL)
    HDfree(refs);
 
 return FAIL;
}


/*-------------------------------------------------------------------------
 * Function: list_gr
 *
 * Purpose: get top level GR image list
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int list_gr(int32 infile_id,
            int32 outfile_id,
            int32 gr_id,             /* GR interface identifier */
            int32 gr_out,            /* GR interface identifier */
            list_table_t *list_tbl,
            options_t *options)
{
    int32 ri_id,             /* raster image identifier */
          n_rimages,         /* number of raster images in the file */
          n_file_attrs,      /* number of file attributes */
          ri_index,          /* index of a image */
          gr_ref,            /* reference number of the GR image */
          dim_sizes[2],      /* dimensions of an image */
          n_comps,           /* number of components an image contains */
          interlace_mode,    /* interlace mode of an image */ 
          data_type,         /* number type of an image */
          n_attrs;           /* number of attributes belong to an image */
    char  name[H4_MAX_GR_NAME]; /* name of an image */
    
    /* determine the contents of the file */
    if (GRfileinfo (gr_id, &n_rimages, &n_file_attrs)==FAIL){
        printf( "Could not get info for GR\n");
        return FAIL;
    }
    
    for (ri_index = 0; ri_index < n_rimages; ri_index++)
    {
        ri_id = GRselect (gr_id, ri_index);
        if (GRgetiminfo (ri_id, name, &n_comps, &data_type, &interlace_mode, 
            dim_sizes, &n_attrs)==FAIL){
            printf("Could not get GR info\n");
            goto out;
        }
        
        gr_ref = GRidtoref(ri_id);
        
        /* check if already inserted in Vgroup; search all image tags */
        if ( list_table_search(list_tbl,DFTAG_RI,gr_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_CI,gr_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_RIG,gr_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_RI8,gr_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_CI8,gr_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_II8,gr_ref)>=0 )
        {
            if (GRendaccess (ri_id)==FAIL){
                printf("Could not close GR\n");
                return FAIL;
            }
            continue;
        }
        
        /* copy GR  */
        if (copy_gr(infile_id,outfile_id,gr_id,gr_out,DFTAG_RI,gr_ref,0,NULL,options,list_tbl)<0)
            goto out;
        
        /* terminate access to the current raster image */
        if (GRendaccess (ri_id)==FAIL){
            printf( "Could not end GR\n");
            return FAIL;
        }
    }
    
     return SUCCEED;
    
out:
    GRendaccess (ri_id);
    return FAIL;
}


/*-------------------------------------------------------------------------
 * Function: list_sds
 *
 * Purpose: get top level SDS
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int list_sds(int32 infile_id,
             int32 outfile_id,
             int32 sd_id,
             int32 sd_out,
             list_table_t *list_tbl,
             dim_table_t *td1,
             dim_table_t *td2,
             options_t *options)
{
    int32 sds_id,                 /* dataset identifier */
          n_datasets,             /* number of datasets in the file */
          n_file_attrs,           /* number of file attributes */
          index,                  /* index of a dataset */
          sds_ref,                /* reference number */
          dim_sizes[H4_MAX_VAR_DIMS],/* dimensions of an image */
          data_type,              /* number type  */
          rank,                   /* rank */
          n_attrs;                /* number of attributes */
    char  name[H4_MAX_GR_NAME];      /* name of dataset */
    
    /* determine the number of data sets in the file and the number of file attributes */
    if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL){
        printf("Could not get SDS info\n");
        return FAIL;
    }
    
    for (index = 0; index < n_datasets; index++)
    {
        sds_id  = SDselect (sd_id, index);
        SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
        sds_ref = SDidtoref(sds_id);
        
        /* check if already inserted in Vgroup; search all SDS tags */
        if ( list_table_search(list_tbl,DFTAG_SD,sds_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_SDG,sds_ref)>=0 ||
            list_table_search(list_tbl,DFTAG_NDG,sds_ref)>=0 )
        {
            SDendaccess (sds_id);
            continue;
        }
        
        /* copy SDS  */
        if (copy_sds(sd_id,sd_out,TAG_GRP_DSET,sds_ref,0,NULL,options,list_tbl,td1,td2,
            infile_id,outfile_id)<0) goto out;
        
        /* terminate access to the current dataset */
        SDendaccess (sds_id);
    }
    
    return SUCCEED;
    
out:
    SDendaccess (sds_id);
    return FAIL;
}

/*-------------------------------------------------------------------------
 * Function: list_vs
 *
 * Purpose: get top level VS
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */


int list_vs(int32 infile_id,
            int32 outfile_id,
            list_table_t *list_tbl,
            options_t *options)
{
    int32 nlones = 0,        /* number of lone vdatas */
          *ref_array=NULL,   /* buffer to hold the ref numbers of lone vdatas   */
          ref;               /* temporary ref number  */
    int   i;

   /*-------------------------------------------------------------------------
    * initialize the V interface
    *-------------------------------------------------------------------------
    */
    
    if (Vstart(infile_id) == FAIL)  
    {
        return FAIL;
    } 
    
    if (options->trip==1)
    {
        if (Vstart(outfile_id) == FAIL)  
        {
            return FAIL;
        }
    }

   /*-------------------------------------------------------------------------
    * get the names and class names of all the lone vdatas
    * first, call Vlone with nlones set to 0 to get the number of lone vdatas 
    * in the file
    *
    *-------------------------------------------------------------------------
    */
    nlones = VSlone (infile_id, NULL, nlones );
    
    if (nlones > 0)
    {
       /*
        * use the nlones returned to allocate sufficient space for the
        * buffer ref_array to hold the reference numbers of all lone vgroups,
        */
        ref_array = (int32 *)HDmalloc(sizeof(int32) * nlones);
        
       /*
        * and call VSlone again to retrieve the reference numbers into 
        * the buffer ref_array.
        */
        nlones = VSlone (infile_id, ref_array, nlones);
        
       /*
        * iterate tru each lone vdata.
        */
        for (i = 0; i < nlones; i++)
        {
           /*
            * attach to the current vdata then get its
            * name and class. note: the current vdata must be detached before
            * moving to the next.
            */
            ref = ref_array[i];
            
            /* check if already inserted in Vgroup, search with VS tag */
            if ( list_table_search(list_tbl,DFTAG_VH,ref)>=0 ) 
            {
                continue;
            }
            
            /* copy VS */
            if (copy_vs(infile_id,outfile_id,DFTAG_VH,ref,0,NULL,options,list_tbl,1)<0)
            {
                goto out;
            }
            
        } /* for */
        
        
        /* free the space allocated */
        if (ref_array) 
        {
            HDfree(ref_array);
            ref_array = NULL;
        }
    } /* if */
    
   /*-------------------------------------------------------------------------
    * terminate access to the V interface
    *-------------------------------------------------------------------------
    */
    if (Vend (infile_id)==FAIL)
    {
        printf("Error: Could not end Vdata interface\n");
        return FAIL;
    }
    if (options->trip==1)
    {
        if (Vend (outfile_id)==FAIL){
            printf("Error: Could not end Vdata interface\n");
            return FAIL;
        }
    }
    
    
    return SUCCEED;


out:
 
 Vend (infile_id);
 if (options->trip==1)
     Vend (outfile_id);
 
 /* free the space allocated */
 if (ref_array!=NULL) 
    HDfree(ref_array);
  
 return FAIL;
}



/*-------------------------------------------------------------------------
 * Function: list_glb
 *
 * Purpose: list/copy global SDS attributes, global GR atrributes
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int list_glb(int32 infile_id,
             int32 outfile_id,
             int32 sd_id,
             int32 sd_out,
             int32 gr_id,
             int32 gr_out,
             list_table_t *list_tbl,
             options_t *options)
{
    int32 n_datasets,             /* number of datasets in the file */
          n_file_attrs;           /* number of file attributes */
    
    if ( options->trip==0 ) 
    {
        return SUCCEED;
    }
    
    /*-------------------------------------------------------------------------
     * copy SDS global attributes
     *-------------------------------------------------------------------------
     */ 

    /* determine the number of data sets in the file and the number of file attributes */
    if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL)
    {
        printf("Could not get SDS info\n");
        return FAIL;
    }
    
    if (copy_sds_attrs(sd_id,sd_out,n_file_attrs,options)<0)
        return FAIL;
    
    /*-------------------------------------------------------------------------
     * copy GR global attributes
     *-------------------------------------------------------------------------
     */ 
   
    /* determine the number of data sets in the file and the number of file attributes */
    if (GRfileinfo (gr_id, &n_datasets, &n_file_attrs)==FAIL)
    {
        printf("Could not get GR info\n");
        return FAIL;
    }
    if (copy_gr_attrs(gr_id,gr_out,n_file_attrs,options)<0)
        return FAIL;
    
    return SUCCEED;
}


/*-------------------------------------------------------------------------
 * Function: list_an
 *
 * Purpose: list/copy AN FILE objects
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int list_an(int32 infile_id,
            int32 outfile_id,
            options_t *options)
{
    int32 an_id,         /* AN interface identifier */
          ann_id,        /* an annotation identifier */
          ann_length,    /* length of the text in an annotation */
          an_out,        /* AN interface identifier */
          file_label_id, /* file label identifier */
          file_desc_id,  /* file description identifier */
          n_file_labels, n_file_descs, n_data_labels, n_data_descs;
    char  *ann_buf=NULL; /* buffer to hold the read annotation */
    int   i;             /* position of an annotation in all of the same type*/
    
    if ( options->trip==0 ) 
    {
        return SUCCEED;
    }
    
    /* Initialize the AN interface  */
    an_id  = ANstart (infile_id);
    an_out = ANstart (outfile_id);
    
    /*
     * Get the annotation information, e.g., the numbers of file labels, file
     * descriptions, data labels, and data descriptions.
     */
    if (ANfileinfo (an_id,&n_file_labels,&n_file_descs,&n_data_labels,&n_data_descs)==FAIL)
    {
        printf( "Could not get AN info\n");
        goto out;
    }
    
    
   /*-------------------------------------------------------------------------
    * AN_FILE_LABEL
    *-------------------------------------------------------------------------
    */ 
    
    
    for (i = 0; i < n_file_labels; i++)
    {
        /* Get the identifier of the current data label */
        ann_id = ANselect (an_id, i, AN_FILE_LABEL);
        
        /* Get the length of the data label */
        ann_length = ANannlen (ann_id);
        
        /* Allocate space for the buffer to hold the data label text */
        ann_buf = HDmalloc((ann_length+1) * sizeof (char));
        
       /*
        * Read and display the file label.  Note that the size of the buffer,
        * i.e., the third parameter, is 1 character more than the length of
        * the data label; that is for the null character.  It is not the case
        * when a description is retrieved because the description does not 
        * necessarily end with a null character.
        * 
        */
        if (ANreadann (ann_id, ann_buf, ann_length+1)==FAIL)
        {
            printf( "Could not read AN\n");
            goto out;
        }
        
        /* Create the file label */
        file_label_id = ANcreatef (an_out, AN_FILE_LABEL);
        
        /* Write the annotations  */
        if (ANwriteann (file_label_id, ann_buf, ann_length)==FAIL) 
        {
            printf("Failed to write file label %d\n", i);
            goto out;
        }
        
        /* Terminate access to the current data label */
        if (ANendaccess (ann_id)==FAIL || ANendaccess (file_label_id)==FAIL)
        {
            printf( "Could not end AN\n");
            goto out;
        }
        
        
        /* Free the space allocated for the annotation buffer */
        if (ann_buf)
            HDfree(ann_buf);
    }
    
    /*-------------------------------------------------------------------------
     * AN_FILE_DESC
     *-------------------------------------------------------------------------
     */ 
    
    for (i = 0; i < n_file_descs; i++)
    {
        /* Get the identifier of the current data label */
        ann_id = ANselect (an_id, i, AN_FILE_DESC);
        
        /* Get the length of the data label */
        ann_length = ANannlen (ann_id);
        
        /* Allocate space for the buffer to hold the data label text */
        ann_buf = HDmalloc((ann_length+1) * sizeof (char));
        
        if (ANreadann (ann_id, ann_buf, ann_length+1)==FAIL)
        {
            printf( "Could not read AN\n");
            goto out;
        }
        
        /* Create the label */
        file_desc_id = ANcreatef (an_out, AN_FILE_DESC);
        
        /* Write the annotations  */
        if (ANwriteann (file_desc_id, ann_buf, ann_length)==FAIL)
        {
            printf("Failed to write file description %d\n", i);
            goto out;
        }
        
        /* Terminate access to the current data label */
        if (ANendaccess (ann_id)==FAIL || ANendaccess (file_desc_id)==FAIL)
        {
            printf( "Could not read AN\n");
            goto out;
        }
        
        /* Free the space allocated for the annotation buffer */
        if (ann_buf)
        {
            HDfree(ann_buf);
            ann_buf = NULL;
        }
    }
    

   /* Terminate access to the AN interface */
    if (ANend (an_id)==FAIL || ANend (an_out)==FAIL)
    {
        printf( "Could not end AN\n");
        goto out;
    }

    return SUCCEED;
    
out:
    if (ANend (an_id)==FAIL || ANend (an_out)==FAIL)
    {
        printf( "Could not end AN\n");
    }
    if (ann_buf!=NULL)
        HDfree(ann_buf);
    
    return FAIL;
    
}




/*-------------------------------------------------------------------------
 * Function: list_pal
 *
 * Purpose: list/copy lone palettes
 *
 * Return: SUCCEED, FAIL
 *
 *-------------------------------------------------------------------------
 */

int list_pal(const char* infname,
             const char* outfname,
             list_table_t *list_tbl,
             options_t *options)
{
    uint8  palette_data[256*3];
    intn   nPals, j;
    uint16 ref;
    
    if ( options->trip==0 ) 
    {
        return SUCCEED;
    }
    
    DFPrestart();
    
    if((nPals = DFPnpals (infname))==FAIL ) 
    {
        printf( "Failed to get palettes in <%s>\n", infname);
        return FAIL;
    }
    
    for ( j = 0; j < nPals; j++) 
    {
        if (DFPgetpal(infname, (VOIDP)palette_data)==FAIL ) 
        {
            printf( "Failed to read palette <%d> in <%s>\n", j, infname);
            return FAIL;
        }
        
        ref=DFPlastref();
        
        /* check if already inserted in image */
        if ( list_table_search(list_tbl,DFTAG_IP8,ref)>=0 )
        {
            continue;
        }
        
        if (DFPaddpal(outfname,palette_data)==FAIL)
        {
            printf( "Failed to write palette in <%s>\n", outfname);
            return FAIL;
        }
        
    }
    
    return SUCCEED;
}

