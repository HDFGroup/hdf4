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


#include "hdf.h"
#include "mfhdf.h"
#include "hrepack.h"
#include "hrepack_utils.h"
#include "hrepack_parse.h"
#include "hrepack_opttable.h"
#include "hrepack_sdutil.h"
#include "hrepack_sds.h"
#include "hrepack_gr.h"
#include "hrepack_vs.h"
#include "hrepack_an.h"



void list_vg (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_gr (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_sds(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_vs (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_glb(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_pal(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_an (char* infname,char* outfname,int32 infile_id,int32 outfile_id,options_t *options);


/*-------------------------------------------------------------------------
 * Function: list
 *
 * Purpose: locate all HDF objects in the file and compress them using options
 *
 * Return: 0, ok, -1 no
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 10, 2003
 *
 * Description:
 *
 * A main loop is used to locate all the objects in the file. This loop preserves the 
 * hierarchy of the file. The algorithm used is 
 * 1) Obtain the number of lone VGroups in the HDF file. 
 * 2) Do a loop for each one of these groups. In each iteration a table is updated 
 *    with the tag/reference pair of an object. 
 *    2.1) Obtain the pairs of tag/references for the group 
 *    2.2) Switch between the tag of the current object. Four cases are possible: 
 *         1) Object is a group: recursively repeat the process (obtain the pairs of
 *            tag/references for this group and do another tag switch). 
 *            Add the object to the table. 
 *         2) Object is a dataset: Add the object to the table.
 *         3) Object is an image: Add the object to the table.  
 *         4) Object is a vdata: Add the object to the table. 
 * 3) Read all the HDF interfaces (SDS, GR and VS), checking for objects that are 
 *    already in the table (meaning they belong to a previous inspected group, 
 *    and should not be added).  These objects belong to a root group. 
 * 4) Read all global attributes and annotations. 
 *
 *-------------------------------------------------------------------------
 */


int list(char* infname, char* outfname, options_t *options)
{
 table_t  *table=NULL;
 int32    infile_id;
 int32    outfile_id;
 int      i;
 char*    err;

 /* init table */
 table_init(&table);

 /* open the input file for read and create the output file */
 infile_id  = Hopen (infname,DFACC_READ,0);
 outfile_id = Hopen (outfname,DFACC_CREATE,0);

 if (infile_id==FAIL)
 {
  table_free(table);
  printf("Cannot open file <%s>\n",infname);
  return FAIL;
 }
 if (outfile_id==FAIL)
 {
  table_free(table);
  printf("Cannot create file <%s>\n",outfname);
  return FAIL;
 }

 if (options->verbose && options->trip==0)
  printf("Building list of objects in %s...\n",infname);

 /* iterate tru HDF interfaces */
 list_vg (infname,outfname,infile_id,outfile_id,table,options);
 list_gr (infname,outfname,infile_id,outfile_id,table,options);
 list_sds(infname,outfname,infile_id,outfile_id,table,options);
 list_vs (infname,outfname,infile_id,outfile_id,table,options);
	list_glb(infname,outfname,infile_id,outfile_id,table,options);
 list_pal(infname,outfname,infile_id,outfile_id,table,options);
	list_an (infname,outfname,infile_id,outfile_id,options);

 /* close the HDF files */
 if (Hclose (infile_id)==FAIL)
  printf( "Failed to close file <%s>\n", infname);
 if (Hclose (outfile_id)==FAIL)
  printf( "Failed to close file <%s>\n", outfname);

 /* 
 check for objects in the file table:
 1) the input object names are present in the file
 2) they are valid objects (SDS or GR)
 check only if selected objects are given (all==0)
 */
 if ( options->trip==0 ) 
 {
  if (options->verbose)
   printf("Searching for objects to modify...\n");
   
  for ( i = 0; i < options->op_tbl->nelems; i++) 
  {
   char* obj_name=options->op_tbl->objs[i].path;
   if (options->verbose)
    printf(PFORMAT1,"","",obj_name);
   
   /* the input object names are present in the file and are valid */
   err=table_check(table,obj_name);
   if (err!=NULL)
   {
    printf("\nError: <%s> %s in file <%s>. Exiting...\n",obj_name,err,infname);
    table_free(table);
    exit(1);
   }
   if (options->verbose)
    printf("...Found\n");
  }
 }

 /* free table */
 table_free(table);
 return 0;
}



/*-------------------------------------------------------------------------
 * Function: list_vg
 *
 * Purpose: locate all lone Vgroups in the file
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */


void list_vg(char* infname,char* outfname,int32 infile_id,int32 outfile_id,
             table_t *table,options_t *options)
{
 int32 vgroup_id,      /* vgroup identifier */
       nlones = 0,     /* number of lone vgroups */
       ntagrefs,       /* number of tag/ref pairs in a vgroup */
       *ref_array=NULL,/* buffer to hold the ref numbers of lone vgroups   */
       *tags,          /* buffer to hold the tag numbers of vgroups   */
       *refs,          /* buffer to hold the ref numbers of vgroups   */
       vgroup_id_out,  /* vgroup identifier */
       ref_vg,
       tag_vg;
 char  vgroup_name[VGNAMELENMAX], vgroup_class[VGNAMELENMAX];
 int   i;

 /* initialize the V interface for both files */
 Vstart (infile_id);
 Vstart (outfile_id);

/*
 * get and print the names and class names of all the lone vgroups.
 * first, call Vlone with nlones set to 0 to get the number of
 * lone vgroups in the file, but not to get their reference numbers.
 */
 nlones = Vlone (infile_id, NULL, nlones );

 if (nlones > 0)
 {
 /*
  * use the nlones returned to allocate sufficient space for the
  * buffer ref_array to hold the reference numbers of all lone vgroups,
  */
  ref_array = (int32 *) malloc(sizeof(int32) * nlones);
  
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
  /*
   * attach to the current vgroup then get its
   * name and class. note: the current vgroup must be detached before
   * moving to the next.
   */
   vgroup_id = Vattach (infile_id, ref_array[i], "r");
   if (Vgetname (vgroup_id, vgroup_name)==FAIL){
    printf( "Could not get name for group\n");
   }
   if (Vgetclass (vgroup_id, vgroup_class)==FAIL){
    printf( "Could not get class for group\n");
   }
   
   /* ignore reserved HDF groups/vdatas */
   if( is_reserved(vgroup_class)){
    if (Vdetach (vgroup_id)==FAIL){
     printf( "Could not detach group\n");
    }
    continue;
   }
   if(vgroup_name != NULL) 
    if(strcmp(vgroup_name,GR_NAME)==0) {
     if (Vdetach (vgroup_id)==FAIL){
      printf( "Could not detach group\n");
     }
     continue;
    }
       
    if ((ref_vg = VQueryref(vgroup_id))==FAIL){
     printf( "Failed to get ref for <%s>\n", vgroup_name);
    }
    if ((tag_vg = VQuerytag(vgroup_id))==FAIL){
     printf( "Failed to get tag for <%s>\n", vgroup_name);
    }

     /* add object to table */
    table_add(table,tag_vg,ref_vg,vgroup_name);

    if (options->verbose)
    printf(PFORMAT,"","",vgroup_name);    
      
   /* 
    * create the group in the output file.  the vgroup reference number is set
    * to -1 for creating and the access mode is "w" for writing 
    */
    vgroup_id_out = Vattach (outfile_id, -1, "w");
    if (Vsetname (vgroup_id_out, vgroup_name)==FAIL){
     printf("Error: Could not create group <%s>\n", vgroup_name);
    }
    if (Vsetclass (vgroup_id_out, vgroup_class)==FAIL){
     printf("Error: Could not create group <%s>\n", vgroup_name);
    }

    copy_vgroup_attrs(vgroup_id,vgroup_id_out,vgroup_name,options);
    copy_vg_an(infile_id,outfile_id,vgroup_id,vgroup_id_out,vgroup_name,options);
       
    /* insert objects for this group */
    ntagrefs = Vntagrefs(vgroup_id);
    if ( ntagrefs > 0 )
    {
     tags = (int32 *) malloc(sizeof(int32) * ntagrefs);
     refs = (int32 *) malloc(sizeof(int32) * ntagrefs);
     Vgettagrefs(vgroup_id, tags, refs, ntagrefs);
     
     vgroup_insert(infname,outfname,infile_id,outfile_id,vgroup_id_out,vgroup_name,
                   tags,refs,ntagrefs,table,options);
     
     free (tags);
     free (refs);
    }
    
    if(Vdetach (vgroup_id)==FAIL){
     printf("Error: Could not detach group <%s>\n", vgroup_name);
    }
    if (Vdetach (vgroup_id_out)==FAIL){
     printf("Error: Could not detach group <%s>\n", vgroup_name);
    }

  } /* for */
  
  
  /* free the space allocated */
  if (ref_array) 
   free (ref_array);
 } /* if */
 

 /* terminate access to the V interface */
 if (Vend (infile_id)==FAIL){
  printf("Error: Could not end group interface in <%s>\n", vgroup_name);
 }
 if (Vend (outfile_id)==FAIL){
  printf("Error: Could not end group interface in <%s>\n", vgroup_name);
 }
 

}

/*-------------------------------------------------------------------------
 * Function: vgroup_insert
 *
 * Purpose: recursive function to locate objects in lone Vgroups
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void vgroup_insert(char* infname,
                   char* outfname,
                   int32 infile_id,
                   int32 outfile_id,
                   int32 vgroup_id_out_par, /* output parent group ID */
                   char*path_name,          /* absolute path for input group name */          
                   int32* in_tags,          /* tag list for parent group */
                   int32* in_refs,          /* ref list for parent group */
                   int npairs,              /* number tag/ref pairs for parent group */
                   table_t *table,
                   options_t *options)
{
 int32 vgroup_id,             /* vgroup identifier */
       ntagrefs,              /* number of tag/ref pairs in a vgroup */
       sd_id,                 /* SD interface identifier */
       sd_out,                /* SD interface identifier */
       tag,                   /* temporary tag */
       ref,                   /* temporary ref */
       *tags,                 /* buffer to hold the tag numbers of vgroups   */
       *refs,                 /* buffer to hold the ref numbers of vgroups   */
       gr_id,                 /* GR interface identifier */
       gr_out,                /* GR interface identifier */
       vgroup_id_out;         /* vgroup identifier */
       
 char  vgroup_name[VGNAMELENMAX], vgroup_class[VGNAMELENMAX];
 char  *path=NULL;
 int   i;
 
 for ( i = 0; i < npairs; i++ ) 
 {
  tag = in_tags[i];
  ref = in_refs[i];
  
  switch(tag) 
  {
/*-------------------------------------------------------------------------
 * VG
 *-------------------------------------------------------------------------
 */
  case DFTAG_VG: 
   
   vgroup_id = Vattach (infile_id, ref, "r");
   if (Vgetname (vgroup_id, vgroup_name)==FAIL){
    printf( "Could not get name for VG\n");
   }
   if (Vgetclass (vgroup_id, vgroup_class)==FAIL){
    printf( "Could not get class for VG\n");
   }
   
   /* ignore reserved HDF groups/vdatas */
   if( is_reserved(vgroup_class)){
    if (Vdetach (vgroup_id)==FAIL){
     printf( "Could not detach VG\n");
    }
    break;
   }
   if(vgroup_name != NULL) 
    if(strcmp(vgroup_name,GR_NAME)==0) {
     if (Vdetach (vgroup_id)==FAIL){
      printf( "Could not detach VG\n");
     }
     break;
    }

   /* initialize path */
   path=get_path(path_name,vgroup_name);

   /* add object to table */
   table_add(table,tag,ref,path);

   if (options->verbose)
    printf(PFORMAT,"","",path);    
     
#if defined(HZIP_DEBUG)
   printf ("\t%s %d\n", path, ref); 
#endif
   
   if ( options->trip==0 ) 
   {
    /*we must go to other groups always */
   }
   
  /* 
   * create the group in the output file.  the vgroup reference number is set
   * to -1 for creating and the access mode is "w" for writing 
   */
   vgroup_id_out = Vattach (outfile_id, -1, "w");
   if (Vsetname (vgroup_id_out, vgroup_name)==FAIL){
    printf("Error: Could not create group <%s>\n", vgroup_name);
   }
   if (Vsetclass (vgroup_id_out, vgroup_class)==FAIL){
    printf("Error: Could not create group <%s>\n", vgroup_name);
   }

   copy_vgroup_attrs(vgroup_id, vgroup_id_out,path,options);
   copy_vg_an(infile_id,outfile_id,vgroup_id,vgroup_id_out,path,options);
   
   /* insert the created vgroup into its parent */
   if (Vinsert (vgroup_id_out_par, vgroup_id_out)==FAIL){
    printf("Could not insert group <%s>\n", vgroup_name);
   }
    
   /* insert objects for this group */
   ntagrefs  = Vntagrefs(vgroup_id);
   if ( ntagrefs > 0 )
   {
    tags = (int32 *) malloc(sizeof(int32) * ntagrefs);
    refs = (int32 *) malloc(sizeof(int32) * ntagrefs);
    Vgettagrefs(vgroup_id, tags, refs, ntagrefs);
    /* recurse */
    vgroup_insert(infname,outfname,infile_id,outfile_id,vgroup_id_out,
                  path,tags,refs,ntagrefs,table,options);
    free (tags);
    free (refs);
   }
   if(Vdetach (vgroup_id)==FAIL)
   {
    printf("Error: Could not detach group <%s>\n", vgroup_name);
   }
   if (Vdetach (vgroup_id_out)==FAIL)
   {
    printf("Error: Could not detach group <%s>\n", vgroup_name);
   }
   if (path)
    free(path);

   break;
   

/*-------------------------------------------------------------------------
 * SDS
 *-------------------------------------------------------------------------
 */   
   
  case DFTAG_SD:  /* Scientific Data */
  case DFTAG_SDG: /* Scientific Data Group */
  case DFTAG_NDG: /* Numeric Data Group */

   sd_id  = SDstart(infname, DFACC_RDONLY);
   sd_out = SDstart(outfname, DFACC_WRITE);

   /* copy dataset */
   copy_sds(sd_id,sd_out,tag,ref,vgroup_id_out_par,path_name,options,table,
            infile_id,outfile_id);
 
   SDend (sd_id);
   SDend (sd_out);

   
   break;
   
/*-------------------------------------------------------------------------
 * Image
 *-------------------------------------------------------------------------
 */   
   
  case DFTAG_RI:  /* Raster Image */
  case DFTAG_CI:  /* Compressed Image */
  case DFTAG_RIG: /* Raster Image Group */

  case DFTAG_RI8:  /* Raster-8 image */
  case DFTAG_CI8:  /* RLE compressed 8-bit image */
  case DFTAG_II8:  /* IMCOMP compressed 8-bit image */

   gr_id  = GRstart(infile_id);
   gr_out = GRstart(outfile_id);

   /* copy GR  */
   copy_gr(infile_id,outfile_id,gr_id,gr_out,tag,ref,vgroup_id_out_par,
           path_name,options,table);

   GRend (gr_id);
   GRend (gr_out);
   
   break;

/*-------------------------------------------------------------------------
 * Vdata
 *-------------------------------------------------------------------------
 */   
   
  case DFTAG_VH:  /* Vdata Header */
   copy_vs(infile_id,outfile_id,tag,ref,vgroup_id_out_par,path_name,options,table,0);
   break;
   
   
  }
  
 }
 
}


/*-------------------------------------------------------------------------
 * Function: list_gr
 *
 * Purpose: get top level GR images
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void list_gr(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options)
{
 int32 gr_id,             /* GR interface identifier */
       gr_out,            /* GR interface identifier */
       ri_id,             /* raster image identifier */
       n_rimages,         /* number of raster images in the file */
       n_file_attrs,      /* number of file attributes */
       ri_index,          /* index of a image */
       gr_ref,            /* reference number of the GR image */
       dim_sizes[2],      /* dimensions of an image */
       n_comps,           /* number of components an image contains */
       interlace_mode,    /* interlace mode of an image */ 
       data_type,         /* number type of an image */
       n_attrs;           /* number of attributes belong to an image */
 char  name[MAX_GR_NAME]; /* name of an image */
 
 /* initialize the GR interface */
 if ((gr_id  = GRstart (infile_id))==FAIL){
  printf( "Could not start GR for <%s>\n",infname);
  return;
 }
 if ((gr_out = GRstart (outfile_id))==FAIL){
  printf( "Could not start GR for <%s>\n",outfname);
  GRend (gr_id);
  return;
 }
 
 /* determine the contents of the file */
 if (GRfileinfo (gr_id, &n_rimages, &n_file_attrs)==FAIL){
  printf( "Could not get info for GR\n");
  GRend (gr_id);
  GRend (gr_out);
  return;
 }
  
 for (ri_index = 0; ri_index < n_rimages; ri_index++)
 {
  ri_id = GRselect (gr_id, ri_index);
  if (GRgetiminfo (ri_id, name, &n_comps, &data_type, &interlace_mode, 
   dim_sizes, &n_attrs)==FAIL){
   printf("Could not get GR info\n");
  }

  gr_ref = GRidtoref(ri_id);

  /* check if already inserted in Vgroup; search all image tags */
  if ( table_search(table,DFTAG_RI,gr_ref)>=0 ||
       table_search(table,DFTAG_CI,gr_ref)>=0 ||
       table_search(table,DFTAG_RIG,gr_ref)>=0 ||
       table_search(table,DFTAG_RI8,gr_ref)>=0 ||
       table_search(table,DFTAG_CI8,gr_ref)>=0 ||
       table_search(table,DFTAG_II8,gr_ref)>=0 )
  {
   if (GRendaccess (ri_id)==FAIL){
    printf("Could not close GR\n");
   }
   continue;
  }

  /* copy GR  */
  copy_gr(infile_id,outfile_id,gr_id,gr_out,DFTAG_RI,gr_ref,0,NULL,options,table);

  /* terminate access to the current raster image */
  if (GRendaccess (ri_id)==FAIL){
   printf( "Could not end GR\n");
  }
 }
 
 /* terminate access to the GR interface */
 if (GRend (gr_id)==FAIL||
     GRend (gr_out)==FAIL){
   printf( "Could not end GR\n");
  }
 

}


/*-------------------------------------------------------------------------
 * Function: list_sds
 *
 * Purpose: get top level SDS
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void list_sds(char* infname,
              char* outfname,
              int32 infile_id,
              int32 outfile_id,
              table_t *table,
              options_t *options)
{
 int32 sd_id,                  /* SD interface identifier */
       sd_out,                 /* SD interface identifier */
       sds_id,                 /* dataset identifier */
       n_datasets,             /* number of datasets in the file */
       n_file_attrs,           /* number of file attributes */
       index,                  /* index of a dataset */
       sds_ref,                /* reference number */
       dim_sizes[MAX_VAR_DIMS],/* dimensions of an image */
       data_type,              /* number type  */
       rank,                   /* rank */
       n_attrs;                /* number of attributes */
 char  name[MAX_GR_NAME];      /* name of dataset */
 
 /* initialize the SD interface */
 if ((sd_id  = SDstart (infname, DFACC_READ))==FAIL){
  printf( "Could not start SD for <%s>\n",infname);
  return;
 }

 if ((sd_out = SDstart (outfname, DFACC_WRITE))==FAIL){
  printf( "Could not start GR for <%s>\n",outfname);
  return;
 }
 
 /* determine the number of data sets in the file and the number of file attributes */
 if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL){
  printf("Could not get SDS info\n");
 }

 for (index = 0; index < n_datasets; index++)
 {
  sds_id  = SDselect (sd_id, index);
  SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
  sds_ref = SDidtoref(sds_id);

  /* check if already inserted in Vgroup; search all SDS tags */
  if ( table_search(table,DFTAG_SD,sds_ref)>=0 ||
       table_search(table,DFTAG_SDG,sds_ref)>=0 ||
       table_search(table,DFTAG_NDG,sds_ref)>=0 )
  {
   SDendaccess (sds_id);
   continue;
  }

  /* copy SDS  */
  copy_sds(sd_id,sd_out,TAG_GRP_DSET,sds_ref,0,NULL,options,table,
           infile_id,outfile_id);
     
  /* terminate access to the current dataset */
  SDendaccess (sds_id);
 }
 
 /* terminate access to the SD interface */
 SDend (sd_id);
 SDend (sd_out);
 

}

/*-------------------------------------------------------------------------
 * Function: list_vs
 *
 * Purpose: get top level VS
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */


void list_vs(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options)
{
 int32 nlones = 0,   /* number of lone vdatas */
       *ref_array,   /* buffer to hold the ref numbers of lone vdatas   */
       ref;          /* temporary ref number  */
 int   i;

 /* initialize the V interface */
 Vstart (infile_id);
 Vstart (outfile_id);

/*
 * get and print the names and class names of all the lone vdatas.
 * first, call Vlone with nlones set to 0 to get the number of
 * lone vdatas in the file, but not to get their reference numbers.
 */
 nlones = VSlone (infile_id, NULL, nlones );

 if (nlones > 0)
 {
 /*
  * use the nlones returned to allocate sufficient space for the
  * buffer ref_array to hold the reference numbers of all lone vgroups,
  */
  ref_array = (int32 *) malloc(sizeof(int32) * nlones);
  
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

   /* check if already inserted in Vgroup; search all VS tags */
   if ( table_search(table,DFTAG_VH,ref)>=0 ) {
    continue;
   }

   /* copy VS */
   copy_vs(infile_id,outfile_id,DFTAG_VH,ref,0,NULL,options,table,1);
 
  } /* for */

  
  /* free the space allocated */
  if (ref_array) free (ref_array);
 } /* if */

 /* terminate access to the VS interface */
 if (Vend (infile_id)==FAIL||
  Vend (outfile_id)==FAIL){
  printf( "Could not end VG\n");
 }



}



/*-------------------------------------------------------------------------
 * Function: list_glb
 *
 * Purpose: list/copy global SDS attributes, global GR atrributes
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void list_glb(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options)
{
 int32 sd_id,                  /* SD interface identifier */
       sd_out,                 /* SD interface identifier */
       gr_id,                  /* GR interface identifier */
       gr_out,                 /* GR interface identifier */
       n_datasets,             /* number of datasets in the file */
       n_file_attrs;           /* number of file attributes */
	
	if ( options->trip==0 ) 
	{
		return;
	}
     
/*-------------------------------------------------------------------------
 * copy SDS global attributes
 *-------------------------------------------------------------------------
 */ 

 /* initialize the SD interface */
 if ((sd_id  = SDstart (infname, DFACC_READ))==FAIL){
  printf( "Could not start SD for <%s>\n",infname);
  return;
 }

 if ((sd_out = SDstart (outfname, DFACC_WRITE))==FAIL){
  printf( "Could not start GR for <%s>\n",outfname);
  return;
 }

 /* determine the number of data sets in the file and the number of file attributes */
 if (SDfileinfo (sd_id, &n_datasets, &n_file_attrs)==FAIL){
  printf("Could not get SDS info\n");
 }
 
 copy_sds_attrs(sd_id,sd_out,n_file_attrs,options);

 /* terminate access to the SD interface */
 SDend (sd_id);
 SDend (sd_out);


/*-------------------------------------------------------------------------
 * copy GR global attributes
 *-------------------------------------------------------------------------
 */ 

 gr_id  = GRstart(infile_id);
 gr_out = GRstart(outfile_id);

 /* determine the number of data sets in the file and the number of file attributes */
 if (GRfileinfo (gr_id, &n_datasets, &n_file_attrs)==FAIL){
  printf("Could not get GR info\n");
 }
 
 copy_gr_attrs(gr_id,gr_out,n_file_attrs,options);

 /* terminate access to the GR interface */
 GRend (gr_id);
 GRend (gr_out);
 

}


/*-------------------------------------------------------------------------
 * Function: list_an
 *
 * Purpose: list/copy AN FILE objects
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void list_an(char* infname,char* outfname,int32 infile_id,int32 outfile_id,options_t *options)
{
 int32 an_id,         /* AN interface identifier */
       ann_id,        /* an annotation identifier */
       i,             /* position of an annotation in all of the same type*/
       ann_length,    /* length of the text in an annotation */
       an_out,        /* AN interface identifier */
       file_label_id, /* file label identifier */
       file_desc_id,  /* file description identifier */
       n_file_labels, n_file_descs, n_data_labels, n_data_descs;
 char *ann_buf;       /* buffer to hold the read annotation */

 if ( options->trip==0 ) 
 {
  return;
 }
 
 /* Initialize the AN interface  */
 an_id  = ANstart (infile_id);
 an_out = ANstart (outfile_id);
 
/*
 * Get the annotation information, e.g., the numbers of file labels, file
 * descriptions, data labels, and data descriptions.
 */
 if (ANfileinfo (an_id, &n_file_labels, &n_file_descs, 
  &n_data_labels, &n_data_descs)==FAIL){
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
  ann_buf = malloc ((ann_length+1) * sizeof (char));
  
 /*
  * Read and display the file label.  Note that the size of the buffer,
  * i.e., the third parameter, is 1 character more than the length of
  * the data label; that is for the null character.  It is not the case
  * when a description is retrieved because the description does not 
  * necessarily end with a null character.
  * 
  */
  if (ANreadann (ann_id, ann_buf, ann_length+1)==FAIL){
   printf( "Could not read AN\n");
  }

  /* Create the file label */
  file_label_id = ANcreatef (an_out, AN_FILE_LABEL);

  /* Write the annotations  */
  if (ANwriteann (file_label_id, ann_buf, ann_length)==FAIL) {
   printf("Failed to write file label %d\n", i);
		}
  
  /* Terminate access to the current data label */
  if (ANendaccess (ann_id)==FAIL||
      ANendaccess (file_label_id)==FAIL){
   printf( "Could not end AN\n");
  }

  
  /* Free the space allocated for the annotation buffer */
  free (ann_buf);
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
  ann_buf = malloc ((ann_length+1) * sizeof (char));
 
  if (ANreadann (ann_id, ann_buf, ann_length+1)==FAIL){
   printf( "Could not read AN\n");
  }

   /* Create the label */
  file_desc_id = ANcreatef (an_out, AN_FILE_DESC);

  /* Write the annotations  */
  if (ANwriteann (file_desc_id, ann_buf, ann_length)==FAIL){
   printf("Failed to write file description %d\n", i);
		}
  
  /* Terminate access to the current data label */
  if (ANendaccess (ann_id)==FAIL||
      ANendaccess (file_desc_id)==FAIL){
   printf( "Could not read AN\n");
  }
 
  /* Free the space allocated for the annotation buffer */
  free (ann_buf);
 }
 
 /* Terminate access to the AN interface */
out:
 if (ANend (an_id)==FAIL||
  ANend (an_out)==FAIL){
  printf( "Could not end AN\n");
 }

 
}




/*-------------------------------------------------------------------------
 * Function: list_pal
 *
 * Purpose: list/copy lone palettes
 *
 * Return: void
 *
 *-------------------------------------------------------------------------
 */

void list_pal(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options)
{
 uint8  palette_data[256*3];
 intn   nPals, j;
 uint16 ref;
	
	if ( options->trip==0 ) 
	{
		return;
	}

 DFPrestart();
 
 if((nPals = DFPnpals (infname))==FAIL ) {
  printf( "Failed to get palettes in <%s>\n", infname);
  return;
 }
 
 for ( j = 0; j < nPals; j++) 
 {
  if (DFPgetpal(infname, (VOIDP)palette_data)==FAIL ) {
   printf( "Failed to read palette <%d> in <%s>\n", j, infname);
   continue;
  }
  
  ref=DFPlastref();
  
  /* check if already inserted in image */
  if ( table_search(table,DFTAG_IP8,ref)>=0 ){
   continue;
  }
  
  if (DFPaddpal(outfname,palette_data)==FAIL){
   printf( "Failed to write palette in <%s>\n", outfname);
  }
  
 }
 
}


