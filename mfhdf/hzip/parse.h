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


#ifndef HDF_PARSE_H__
#define HDF_PARSE_H__

#include "hdf.h"
#include "mfhdf.h"
#include "hzip.h"


#ifdef __cplusplus
extern "C" {
#endif

/* compression */

obj_list_t* parse_comp(char *str, int *n_objs, comp_t *comp);
char*       get_scomp(int code);

/* chunking */

obj_list_t* parse_chunk(char *str, int *n_objs, int32 *chunk_lengths, int *chunk_rank);


#if defined (ONE_TABLE)

void options_table_init( options_table_t **tbl );
void options_table_free( options_table_t *table );
int options_add_chunk(obj_list_t *obj_list,int n_objs,int32 *chunk_lengths,
																			   int chunk_rank,options_table_t *table );
int options_add_comp(obj_list_t *obj_list,int n_objs,comp_info_t comp,
																			  options_table_t *table );
obj_info_t* options_get_object(char *path,options_table_t *table);


#else

int         comp_list_add(obj_list_t *obj_list, int n_objs, comp_t comp, comp_table_t *table );
void        comp_list_init( comp_table_t **tbl );
void        comp_list_free( comp_table_t *table );
int         chunk_list_add(obj_list_t *obj_list,int n_objs,int32 *chunk_lengths,
																			int chunk_rank,chunk_table_t *table );
void        chunk_list_init( chunk_table_t **tbl );
void        chunk_list_free( chunk_table_t *table );

#endif

#ifdef __cplusplus
}
#endif


#endif  /* HDF_PARSE_H__ */
