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



#include <assert.h>
#include "hdf.h"
#include "mfhdf.h"

#include "hrepack_vg.h"
#include "hrepack_utils.h"
#include "hrepack_an.h"


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
 * Function: copy_vg
 *
 * Purpose: copy a VG from input file to output file
 *
 * Return: void
 *
 * Programmer: Pedro Vicente, pvn@ncsa.uiuc.edu
 *
 * Date: July 11, 2003
 *
 *-------------------------------------------------------------------------
 */

void copy_vg(char* infname,
             char* outfname,
             int32 infile_id,
             int32 outfile_id,
             int32 tag,               /* tag of input VS */
             int32 ref,               /* ref of input VS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table)
{
 int32 status_32,             /* returned status_n for functions returning an int32 */
       vgroup_id,             /* vg identifier */
       vgroup_id_out,         /* vg identifier */
       ntagrefs,              /* number of tag/ref pairs in a vgroup */
       *tags,                 /* buffer to hold the tag numbers of vgroups   */
       *refs,                 /* buffer to hold the ref numbers of vgroups   */
       vg_index;              /* position of a vgroup in the vgroup  */
 char  vgroup_name [VGNAMELENMAX], vgroup_class[VGNAMELENMAX];
 char  *path=NULL;
 
/*
 * attach to the current vgroup then get its
 * name and class. note: the current vgroup must be detached before
 * moving to the next.
 */

 vgroup_id = Vattach (infile_id, ref, "r");
 status_32 = Vgetname (vgroup_id, vgroup_name);
 status_32 = Vgetclass (vgroup_id, vgroup_class);
 
 /* ignore reserved HDF groups/vdatas */
 if( is_reserved(vgroup_class)){
  status_32 = Vdetach (vgroup_id);
  return;
 }
 if(vgroup_name != NULL) 
  if(strcmp(vgroup_name,GR_NAME)==0) {
   status_32 = Vdetach (vgroup_id);
   return;
 }
  
 /* initialize path */
 path=get_path(path_name,vgroup_name);
 
 /* add object to table */
 table_add(table,tag,ref,path);
  
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
 status_32     = Vsetname (vgroup_id_out, vgroup_name);
 status_32     = Vsetclass (vgroup_id_out, vgroup_class);
 
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
 status_32 = Vdetach (vgroup_id);
 status_32 = Vdetach (vgroup_id_out);
 
 if (path)
  free(path);
  
 return;
}


