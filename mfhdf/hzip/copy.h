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


#ifndef HDF_ZIP_COPY_H__
#define HDF_ZIP_COPY_H__

#include "hzip.h"
#include "table.h"




#ifdef __cplusplus
extern "C" {
#endif

int  is_reserved(char*vgroup_class);
int  copy_sds(int32 sd_in,int32 sd_out,int32 tag,int32 ref,int32 vgroup_id_out_par,
              char*group_name,options_t *options,table_t *table);
int  copy_gr (int32 gr_in,int32 gr_out,int32 tag,int32 ref,int32 vgroup_id_out_par,
              char*group_name,options_t *options,table_t *table);
int  copy_vs( int32 infile_id,
              int32 outfile_id,
              int32 tag,
              int32 ref,               /* ref of input VS */
              int32 vgroup_id_out_par, /* output parent group ID */
              char*path_name,          /* absolute path for input group name */
              options_t *options,
              table_t *table);

void copy_vg(char* infname,
             char* outfname,
             int32 infile_id,
             int32 outfile_id,
             int32 tag,               /* tag of input VS */
             int32 ref,               /* ref of input VS */
             int32 vgroup_id_out_par, /* output parent group ID */
             char*path_name,          /* absolute path for input group name */
             options_t *options,
             table_t *table);


void vgroup_insert(char* infname,char* outfname,int32 infile_id,int32 outfile_id,
                   int32 vgroup_id_out_par, /* output parent group ID */
                   char*path_name,          /* absolute path for input group name */          
                   int32* in_tags,          /* tag list for parent group */
                   int32* in_refs,          /* ref list for parent group */
                   int npairs,              /* number tag/ref pairs for parent group */
                   table_t *table,
                   options_t *options);


char *get_path(char*path_name, char*obj_name);

#ifdef __cplusplus
}
#endif


#endif  /* HDF_ZIP_COPY_H__ */
