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


#ifndef REPACK_VG_H_
#define REPACK_VG_H_

#include "hrepack.h"
#include "hrepack_lsttable.h"
#include "hrepack_dim.h"



#ifdef __cplusplus
extern "C" {
#endif


int copy_vgroup_attrs(int32 vg_in, 
                      int32 vg_out, 
                      char *path,
                      options_t *options);

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
                  options_t *options);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_VG_H_ */
