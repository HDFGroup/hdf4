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
#include "hzip.h"
#include "copy.h"
#include "parse.h"
#include "utils.h"



void list_vg (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_gr (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_sds(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_vs (char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_glb(char* infname,char* outfname,int32 infile_id,int32 outfile_id,table_t *table,options_t *options);
void list_an (char* infname,char* outfname,int32 infile_id,int32 outfile_id,options_t *options);

void vgroup_insert(char* infname, char* outfname, 
                   int32 infile_id, int32 outfile_id,
                   int32 vgroup_id_out, char*path_name, 
                   int32* tags, int32* refs, int npairs, 
                   table_t *table, options_t *options);



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
 intn     status_n;  
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
	list_an (infname,outfname,infile_id,outfile_id,options);

 /* close the HDF files */
 if ((status_n = Hclose (infile_id)) == FAIL )
  printf( "Failed to close file <%s>\n", infname);
 if ((status_n = Hclose (outfile_id)) == FAIL )
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
 intn  status_n;       /* returned status_n for functions returning an intn  */
 int32 status_32,      /* returned status_n for functions returning an int32 */
       vgroup_ref=-1,  /* reference number of the group */
       vgroup_id,      /* vgroup identifier */
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
 status_n = Vstart (infile_id);
 status_n = Vstart (outfile_id);

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
   status_32 = Vgetname (vgroup_id, vgroup_name);
   status_32 = Vgetclass (vgroup_id, vgroup_class);
   
   /* ignore reserved HDF groups/vdatas */
   if( is_reserved(vgroup_class)){
    status_32 = Vdetach (vgroup_id);
    continue;
   }
   if(vgroup_name != NULL) 
    if(strcmp(vgroup_name,GR_NAME)==0) {
     status_32 = Vdetach (vgroup_id);
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
 intn  status_n;              /* returned status_n for functions returning an intn  */
 int32 status_32,             /* returned status_n for functions returning an int32 */
       vgroup_id,             /* vgroup identifier */
       ntagrefs,              /* number of tag/ref pairs in a vgroup */
       sd_id,                 /* SD interface identifier */
       sd_out,                /* SD interface identifier */
       tag,                   /* temporary tag */
       ref,                   /* temporary ref */
       *tags,                 /* buffer to hold the tag numbers of vgroups   */
       *refs,                 /* buffer to hold the ref numbers of vgroups   */
       gr_id,                 /* GR interface identifier */
       gr_out,                /* GR interface identifier */
       vgroup_id_out,         /* vgroup identifier */
       vg_index;              /* position of a vgroup in the vgroup  */
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
   status_32 = Vgetname (vgroup_id, vgroup_name);
   status_32 = Vgetclass (vgroup_id, vgroup_class);
   
   /* ignore reserved HDF groups/vdatas */
   if( is_reserved(vgroup_class)){
    status_32 = Vdetach (vgroup_id);
    break;
   }
   if(vgroup_name != NULL) 
    if(strcmp(vgroup_name,GR_NAME)==0) {
     status_32 = Vdetach (vgroup_id);
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
   if (Vsetname (vgroup_id_out, vgroup_name)==FAIL)
   {
    printf("Error: Could not create group <%s>\n", vgroup_name);
   }
   if (Vsetclass (vgroup_id_out, vgroup_class)==FAIL)
   {
    printf("Error: Could not create group <%s>\n", vgroup_name);
   }

   copy_vgroup_attrs(vgroup_id, vgroup_id_out,path,options);
   copy_vg_an(infile_id,outfile_id,vgroup_id,vgroup_id_out,path,options);
   
   /* insert the created vgroup into its parent */
   vg_index = Vinsert (vgroup_id_out_par, vgroup_id_out);
    
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
 
   status_n = SDend (sd_id);
   status_n = SDend (sd_out);

   
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

   status_n = GRend (gr_id);
   status_n = GRend (gr_out);
   
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
 intn  status;            /* status for functions returning an intn */
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
 gr_id  = GRstart (infile_id);
 gr_out = GRstart (outfile_id);
 
 /* determine the contents of the file */
 status = GRfileinfo (gr_id, &n_rimages, &n_file_attrs);
  
 for (ri_index = 0; ri_index < n_rimages; ri_index++)
 {
  ri_id = GRselect (gr_id, ri_index);
  status = GRgetiminfo (ri_id, name, &n_comps, &data_type, &interlace_mode, 
   dim_sizes, &n_attrs);

  gr_ref = GRidtoref(ri_id);

  /* check if already inserted in Vgroup; search all image tags */
  if ( table_search(table,DFTAG_RI,gr_ref)>=0 ||
       table_search(table,DFTAG_CI,gr_ref)>=0 ||
       table_search(table,DFTAG_RIG,gr_ref)>=0 ||
       table_search(table,DFTAG_RI8,gr_ref)>=0 ||
       table_search(table,DFTAG_CI8,gr_ref)>=0 ||
       table_search(table,DFTAG_II8,gr_ref)>=0 )
  {
   status = GRendaccess (ri_id);
   continue;
  }

  /* copy GR  */
  copy_gr(infile_id,outfile_id,gr_id,gr_out,DFTAG_RI,gr_ref,0,NULL,options,table);

  /* terminate access to the current raster image */
  status = GRendaccess (ri_id);
 }
 
 /* terminate access to the GR interface */
 status = GRend (gr_id);
 status = GRend (gr_out);
 

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
 intn  status;                 /* status for functions returning an intn */
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
 sd_id  = SDstart (infname, DFACC_READ);
 sd_out = SDstart (outfname, DFACC_WRITE);
 
 /* determine the number of data sets in the file and the number of file attributes */
 status = SDfileinfo (sd_id, &n_datasets, &n_file_attrs);

 for (index = 0; index < n_datasets; index++)
 {
  sds_id  = SDselect (sd_id, index);
  status  = SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
  sds_ref = SDidtoref(sds_id);

  /* check if already inserted in Vgroup; search all SDS tags */
  if ( table_search(table,DFTAG_SD,sds_ref)>=0 ||
       table_search(table,DFTAG_SDG,sds_ref)>=0 ||
       table_search(table,DFTAG_NDG,sds_ref)>=0 )
  {
   status = SDendaccess (sds_id);
   continue;
  }

  /* copy SDS  */
  copy_sds(sd_id,sd_out,TAG_GRP_DSET,sds_ref,0,NULL,options,table,
           infile_id,outfile_id);
     
  /* terminate access to the current dataset */
  status = SDendaccess (sds_id);
 }
 
 /* terminate access to the SD interface */
 status = SDend (sd_id);
 status = SDend (sd_out);
 

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
 intn  status_n;     /* returned status_n for functions returning an intn  */
 int32 vdata_ref=-1, /* reference number of the vdata */
       nlones = 0,   /* number of lone vdatas */
       *ref_array,   /* buffer to hold the ref numbers of lone vdatas   */
       ref;          /* temporary ref number  */
 int   i;

 /* initialize the V interface */
 status_n = Vstart (infile_id);
 status_n = Vstart (outfile_id);

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
 status_n = Vend (infile_id);
 status_n = Vend (outfile_id);



}


/*-------------------------------------------------------------------------
 * Function: copy_vgroup_attrs
 *
 * Purpose: copy VG attributes
 *
 * Return: 1 
 *
 *-------------------------------------------------------------------------
 */

int copy_vgroup_attrs(int32 vg_in, int32 vg_out, char *path,options_t *options) 
{
 int    n_attrs;
 int32  data_type, size,  n_values;
 char   attr_name[MAX_NC_NAME];
 int    i;
 char   *buf=NULL;

 if ( options->trip==0 ) 
 {
  return 1;
 }

 /* Get the number of attributes attached to this vgroup.  */
 if((n_attrs = Vnattrs (vg_in))==FAIL) {
  printf( "Failed to get attributes for <%s>\n", path);
  return-1;
 }
 
 for (i = 0; i < n_attrs; i++) 
 {
  if((Vattrinfo (vg_in, i, attr_name, &data_type, &n_values, &size))==FAIL) {
   printf( "Failed to get attribute %d of <%s>\n", i, path);
   continue;
  }
  if ((buf = (char *)malloc(size * n_values))==NULL ) {
   printf( "Failed to get memory for attribute %d of <%s>\n", i, path);
   continue;
  }
  if((Vgetattr (vg_in, i, buf))==FAIL){
   printf( "Failed to get attribute %d of <%s>\n", i, path);
   if (buf) free(buf);
   continue;
  }
  if((Vsetattr(vg_out, attr_name, data_type, n_values, buf))==FAIL){
   printf( "Failed to set attribute %d of <%s>\n", i, path);
  }
  if (buf) free(buf);
 }
 return 1;
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
 intn  status;                 /* status for functions returning an intn */
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
 sd_id  = SDstart (infname, DFACC_READ);
 sd_out = SDstart (outfname, DFACC_WRITE);

 /* determine the number of data sets in the file and the number of file attributes */
 status = SDfileinfo (sd_id, &n_datasets, &n_file_attrs);
 
 copy_sds_attrs(sd_id,sd_out,n_file_attrs,options);

 /* terminate access to the SD interface */
 status = SDend (sd_id);
 status = SDend (sd_out);


/*-------------------------------------------------------------------------
 * copy GR global attributes
 *-------------------------------------------------------------------------
 */ 

 gr_id  = GRstart(infile_id);
 gr_out = GRstart(outfile_id);

 /* determine the number of data sets in the file and the number of file attributes */
 status = GRfileinfo (gr_id, &n_datasets, &n_file_attrs);
 
 copy_gr_attrs(gr_id,gr_out,n_file_attrs,options);

 /* terminate access to the GR interface */
 status = GRend (gr_id);
 status = GRend (gr_out);
 

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
 intn  status_n;      /* returned status for functions returning an intn  */
 int32 status_32,     /* returned status for functions returning an int32 */
       an_id,         /* AN interface identifier */
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
 status_n = ANfileinfo (an_id, &n_file_labels, &n_file_descs, 
  &n_data_labels, &n_data_descs);
 

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
  status_32 = ANreadann (ann_id, ann_buf, ann_length+1);

  /* Create the file label */
  file_label_id = ANcreatef (an_out, AN_FILE_LABEL);

  /* Write the annotations  */
  if ((status_32 = ANwriteann (file_label_id, ann_buf, ann_length))==FAIL) {
   printf("Failed to write file label %d\n", i);
		}
  
  /* Terminate access to the current data label */
  status_n = ANendaccess (ann_id);
  status_n = ANendaccess (file_label_id);
  
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
 
  status_32 = ANreadann (ann_id, ann_buf, ann_length+1);

   /* Create the label */
  file_desc_id = ANcreatef (an_out, AN_FILE_DESC);

  /* Write the annotations  */
  if ((status_32 = ANwriteann (file_desc_id, ann_buf, ann_length))==FAIL){
   printf("Failed to write file description %d\n", i);
		}
  
  /* Terminate access to the current data label */
  status_n = ANendaccess (ann_id);
  status_n = ANendaccess (file_desc_id);
 
  /* Free the space allocated for the annotation buffer */
  free (ann_buf);
 }
 
 /* Terminate access to the AN interface */
 status_32 = ANend (an_id);
 status_32 = ANend (an_out);
 
}


/*-------------------------------------------------------------------------
 * Function: copy_vg_an
 *
 * Purpose: copy Vgroup ANs
 *
 * Return: ok, 1, -1 not ok 
 *
 *-------------------------------------------------------------------------
 */

int copy_vg_an(int32 infile_id,int32 outfile_id,int32 vgroup_id,int32 vgroup_id_out, 
               char *path,options_t *options) 
{
 int32 ref_in,
       tag_in,
       ref_out,
       tag_out;

 if ( options->trip==0 ) 
 {
  return 1;
 }

 if ((ref_in = VQueryref(vgroup_id))==FAIL){
  printf( "Failed to get ref for <%s>\n", path);
  return-1;
 }
 if ((tag_in = VQuerytag(vgroup_id))==FAIL){
  printf( "Failed to get tag for <%s>\n", path);
  return-1;
 }
 if ((ref_out = VQueryref(vgroup_id_out))==FAIL){
  printf( "Failed to get ref for <%s>\n", path);
  return-1;
 }
 if ((tag_out = VQuerytag(vgroup_id_out))==FAIL){
  printf( "Failed to get tag for <%s>\n", path);
  return-1;
 }

 copy_an(infile_id,outfile_id,
         ref_in,tag_in,ref_out,tag_out, 
         path,options);

 return 1;
}


/*-------------------------------------------------------------------------
 * Function: copy_vs_an
 *
 * Purpose: copy Vdata ANs
 *
 * Return: ok, 1, -1 not ok 
 *
 *-------------------------------------------------------------------------
 */

int copy_vs_an(int32 infile_id,int32 outfile_id,int32 vdata_id,int32 vdata_id_out, 
               char *path,options_t *options) 
{
 int32 ref_in,
       tag_in,
       ref_out,
       tag_out;

 if ( options->trip==0 ) 
 {
  return 1;
 }

 if ((ref_in = VSQueryref(vdata_id))==FAIL){
  printf( "Failed to get ref for <%s>\n", path);
  return-1;
 }
 if ((tag_in = VSQuerytag(vdata_id))==FAIL){
  printf( "Failed to get tag for <%s>\n", path);
  return-1;
 }
 if ((ref_out = VSQueryref(vdata_id_out))==FAIL){
  printf( "Failed to get ref for <%s>\n", path);
  return-1;
 }
 if ((tag_out = VSQuerytag(vdata_id_out))==FAIL){
  printf( "Failed to get tag for <%s>\n", path);
  return-1;
 }

 copy_an(infile_id,outfile_id,
         ref_in,tag_in,ref_out,tag_out, 
         path,options);

 return 1;
}


/*-------------------------------------------------------------------------
 * Function: copy_an_data
 *
 * Purpose: copy DATA ANs
 *
 * Return: ok, 1, -1 not ok 
 *
 *-------------------------------------------------------------------------
 */

int copy_an_data(int32 infile_id,
                 int32 outfile_id,
                 int32 ref_in, 
                 int32 tag_in,
                 int32 ref_out, 
                 int32 tag_out,
                 ann_type type, 
                 char *path, 
                 options_t *options) 
{
 int32 status_32,
       an_id,         /* AN interface identifier */
       an_out,        /* AN interface identifier */
       ann_id,        /* an annotation identifier */
       ann_out,       /* an annotation identifier */
       i,             /* position of an annotation */
       ann_length,    /* length of the text in an annotation */
       n_anno;
       
 char *buf;           /* buffer to hold the read annotation */
 int  is_label= (type==AN_DATA_LABEL)?1:0;

 if ( options->trip==0 ) 
 {
  return 1;
 }
 
 /* Initialize the AN interface  */
 an_id  = ANstart (infile_id);
 an_out = ANstart (outfile_id);

 /* Get the number of ANs in this object  */
 if((n_anno = ANnumann(an_id,type,(uint16)tag_in,(uint16)ref_in))==FAIL) {
  printf( "Failed to get annotations for <%s>\n", path);
  return-1;
 }
 
 for (i = 0; i < n_anno; i++) 
 {
 /*-------------------------------------------------------------------------
  * read
  *-------------------------------------------------------------------------
  */ 
  if((ann_id = ANselect(an_id,i,type))==FAIL) {
   printf( "Failed to select AN %d of <%s>\n", i, path);
   continue;
  }
  if((ann_length = ANannlen(ann_id))==FAIL) {
   printf( "Failed to get AN %d lenght of <%s>\n", i, path);
   continue;
  }
  
 /*
  * Read the data label.  Note that the size of the buffer,
  * i.e., the third parameter, is 1 character more than the length of
  * the data label; that is for the null character.  It is not the case
  * when a description is retrieved because the description does not 
  * necessarily end with a null character.
  * 
  */
  if (is_label)
   ann_length++;

  if ((buf = (char *)malloc((ann_length)*sizeof(int8)))==NULL ) {
   printf( "Failed to get memory for AN %d of <%s>\n", i, path);
   continue;
  }
  if(ANreadann(ann_id,buf,ann_length)==FAIL){
   printf( "Failed to read AN %d of <%s>\n", i, path);
   if (buf) free(buf);
   continue;
  }
  if(ANendaccess(ann_id)==FAIL){
   printf( "Failed to end AN %d of <%s>\n", i, path);
   if (buf) free(buf);
   continue;
  }
 /*-------------------------------------------------------------------------
  * write
  *-------------------------------------------------------------------------
  */  
 /* Create the data label for the vgroup identified by its tag and ref number */
  if((ann_out = ANcreate(an_out,(uint16)tag_out,(uint16)ref_out,type))==FAIL) {
   printf( "Failed to create AN %d of <%s>\n", i, path);
   continue;
  }
  /* Write the annotation  */
  if (ANwriteann (ann_out,buf,ann_length)==FAIL){
   printf( "Failed to write AN %d of <%s>\n", i, path);
		}
  if(ANendaccess(ann_out)==FAIL){
   printf( "Failed to end AN %d of <%s>\n", i, path);
   if (buf) free(buf);
   continue;
  }
  if (buf) free(buf);
 }

 /* Terminate access to the AN interface */
 status_32 = ANend (an_id);
 status_32 = ANend (an_out);
 return 1;
}


/*-------------------------------------------------------------------------
 * Function: copy_an
 *
 * Purpose: copy DATA ANs (AN_DATA_LABEL and AN_DATA_DESC)
 *
 * Return: ok, 1, -1 not ok 
 *
 *-------------------------------------------------------------------------
 */

int copy_an(int32 infile_id,
            int32 outfile_id,
            int32 ref_in, 
            int32 tag_in,
            int32 ref_out, 
            int32 tag_out,
            char *path, 
            options_t *options) 
{

 copy_an_data(infile_id,outfile_id,
              ref_in,tag_in,ref_out,tag_out, 
              AN_DATA_LABEL,path,options);
 copy_an_data(infile_id,outfile_id,
              ref_in,tag_in,ref_out,tag_out,
              AN_DATA_DESC,path,options);

 return 1;
}



