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


#ifndef HDF_ZIP_H__
#define HDF_ZIP_H__

#include "table.h"



#define TAG_GRP_IMAGE DFTAG_RIG
#define TAG_GRP_DSET  DFTAG_NDG


#if 0
#define HZIP_DEBUG
#endif


#if 1
#define ONE_TABLE
#endif



typedef enum {
	NONE, SELECTED, ALL
} behaviour_t;

/* a list of names */
typedef struct {
 char obj[MAX_NC_NAME];
} obj_list_t;

/* the type of compression and additional parameter */
typedef struct {
 int type;
 int info;
} comp_t;



#if defined (ONE_TABLE)

/* the type of compression and additional parameter */
typedef struct {
 int type;
 int info;
} comp_info_t;

/* chunk lengths along each dimension and rank */
typedef struct {
 int32   chunk_lengths[MAX_VAR_DIMS]; 
 int     rank;
} chunk_info_t;

/* information for one object, contains PATH, CHUNK info and COMP info */
typedef struct {
	char         path[MAX_NC_NAME];            /* name of object */
	comp_info_t  comp;                         /* compression information */
	chunk_info_t chunk;                        /* chunk information */
} obj_info_t;

/* store a list of all objects */
typedef struct {
	int        size;
	int        nelems;
	obj_info_t *objs;
} options_table_t;


#else

/* store info for one -t option */
typedef struct {
 obj_list_t *obj_list;  /* a list of object names */
 int        n_objs;     /* the number of object names */
 comp_t     comp;       /* their compression information */
} obj_comp_t;

/* store a list of all -t options */
typedef struct {
	int size;
	int nelems;
	obj_comp_t *objs;
} comp_table_t;

/* store info for one -c option */
typedef struct {
 obj_list_t    *obj_list;                   /* a list of object names */
 int           n_objs;                      /* the number of object names */
 int32         chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
	int           rank;
} obj_chunk_t;

/* store a list of all -c options */
typedef struct {
	int size;
	int nelems;
	obj_chunk_t *objs;
} chunk_table_t;





#endif



/* all the above, ready to go to the hzip call */
typedef struct {
 

#if defined (ONE_TABLE)
	options_table_t *op_tbl;     /*table with all -c and -t options */
	int             all_chunk;   /*chunk all objects, input of "*" */
	int             all_comp;    /*comp all objects, input of "*" */
	comp_info_t     comp_g;      /*global compress INFO for the ALL case */
	chunk_info_t    chunk_g;     /*global chunk INFO for the ALL case */

#else

	int             compress;    /*do compress; 0 for NO(default), 1 for SELECTED, 2 for ALL*/
	int             chunk;       /*do chunking; 0 for NO(default), 1 for SELECTED, 2 for ALL*/
	comp_t          comp;        /*global compress type for the ALL case */
	HDF_CHUNK_DEF   chunk_def;   /*global chunk def for the ALL case */
	int32           chunk_flags; /*global chunk flags for the ALL case */
	int32           chunk_rank;  /*global chunk rank for the ALL case */
	comp_table_t    *cp_tbl;     /*table with all -t options */
	chunk_table_t   *ck_tbl;     /*table with all -c options */

#endif

	int verbose;                 /*verbose mode */
	int trip;                    /*which cycle are we in */
} options_t;



#ifdef __cplusplus
extern "C" {
#endif

void hzip         (char* infname, char* outfname, options_t *options);
void hzip_addcomp (char* str, options_t *options);
void hzip_addchunk(char* str, options_t *options);
void hzip_init    (options_t *options);
void hzip_end     (options_t *options);


int  list(char* infname,char* outfname,options_t *options);



#ifdef __cplusplus
}
#endif


#endif  /* HDF_ZIP_H__ */
