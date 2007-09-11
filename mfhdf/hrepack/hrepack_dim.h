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


#ifndef REPACK_DIM_H_
#define REPACK_DIM_H_

#include "hrepack.h"


/* dimension SDS  */
typedef struct dim_name_t {
    int32 ref;                    /* reference */
    char  dim_name[H4_MAX_NC_NAME];  /* name */
} dim_name_t;

/* table for dim_name_t */
typedef struct dim_table_t {
    int        size;
    int        nobjs;
    dim_name_t *objs;
} dim_table_t;




#ifdef __cplusplus
extern "C" {
#endif

void dim_table_add(dim_table_t *dim_tbl, int ref, char* name);
void dim_table_init( dim_table_t **tbl );
void dim_table_free( dim_table_t *dim_tbl );


void match_dim(int32 sd_in,
               int32 sd_out,
               dim_table_t *dt1,
               dim_table_t *dt2,
               options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* REPACK_DIM_H_ */
