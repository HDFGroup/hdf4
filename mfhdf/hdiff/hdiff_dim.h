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


#ifndef HDIFF_DIM_H_
#define HDIFF_DIM_H_


#include "hdiff.h"


/* dimension SDS  */
typedef struct dim_name_t {
    int32 ref;                    /* reference */
    char  dim_name[MAX_NC_NAME];  /* name */
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

void dim_table_add(dim_table_t *table, int ref, char* name);
void dim_table_init( dim_table_t **tbl );
void dim_table_free( dim_table_t *table );


uint32 match_dim(int32 sd1_id,
               int32 sd2_id,
               dim_table_t *td1_1,
               dim_table_t *td1_2,
               dim_table_t *td2_1,
               dim_table_t *td2_2,
               diff_opt_t * opt);



#ifdef __cplusplus
}
#endif


#endif  /* HDIFF_DIM_H_ */
