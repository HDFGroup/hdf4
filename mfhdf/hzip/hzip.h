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

/* for SZIP */
#if !defined (NN_OPTION_MASK)
#define NN_OPTION_MASK				 32
#endif
#if !defined (RAW_OPTION_MASK)
#define RAW_OPTION_MASK				128
#endif

#define TAG_GRP_IMAGE DFTAG_RIG
#define TAG_GRP_DSET  DFTAG_NDG


#if 0
#define HZIP_DEBUG
#endif


/* a list of names */
typedef struct {
 char obj[MAX_NC_NAME];
} obj_list_t;

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


/* all the above, ready to go to the hzip call */
typedef struct {
 options_table_t *op_tbl;     /*table with all -c and -t options */
 int             all_chunk;   /*chunk all objects, input of "*" */
 int             all_comp;    /*comp all objects, input of "*" */
 comp_info_t     comp_g;      /*global compress INFO for the ALL case */
 chunk_info_t    chunk_g;     /*global chunk INFO for the ALL case */
 int verbose;                 /*verbose mode */
 int trip;                    /*which cycle are we in */
	int threshold;               /*minimum size to compress, in bytes */
} options_t;



#ifdef __cplusplus
extern "C" {
#endif

void hzip         (char* infname, char* outfname, options_t *options);
void hzip_addcomp (char* str, options_t *options);
void hzip_addchunk(char* str, options_t *options);
void hzip_init    (options_t *options, int verbose);
void hzip_end     (options_t *options);

int  list(char* infname,char* outfname,options_t *options);
void read_info(char *filename,options_t *options); 

#ifdef __cplusplus
}
#endif


#endif  /* HDF_ZIP_H__ */
