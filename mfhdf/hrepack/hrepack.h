/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef REPACK_H_
#define REPACK_H_

#include "hrepack_lsttable.h"

#ifdef H4_HAVE_LIBSZ
#include "szlib.h"
#endif
#define NN_MODE 0
#define EC_MODE 1
#define TAG_GRP_IMAGE DFTAG_RIG
#define TAG_GRP_DSET  DFTAG_NDG

/* a list of names */
typedef struct {
 char obj[H4_MAX_NC_NAME];
} obj_list_t;

/* the type of compression and additional parameter */
typedef struct {
 comp_coder_t type; /* compression enum type */
 int info;          /* numerical parameter for several types of compression */
 int szip_mode;     /* NN_MODE or EC_MODE */
} comp_info_t;

/* chunk lengths along each dimension and rank */
typedef struct {
 int32   chunk_lengths[H4_MAX_VAR_DIMS]; 
 int     rank;
} chunk_info_t;

/* information for one object, contains PATH, CHUNK info and COMP info */
typedef struct {
 char         objpath[H4_MAX_NC_NAME];      /* name of object */
 comp_info_t  comp;                         /* compression information */
 chunk_info_t chunk;                        /* chunk information */
} pack_info_t;

/* store a list of all objects */
typedef struct {
 int        size;
 int        nelems;
 pack_info_t *objs;
} options_table_t;


/* all the above, ready to go to the hrepack call */
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

HDFTOOLSAPI int  hrepack         (const char* infname, const char* outfname, options_t *options);
HDFTOOLSAPI int  hrepack_addcomp (const char* str, options_t *options);
HDFTOOLSAPI int  hrepack_addchunk(const char* str, options_t *options);
HDFTOOLSAPI void hrepack_init    (options_t *options, int verbose);
HDFTOOLSAPI void hrepack_end     (options_t *options);
HDFTOOLSAPI int  hrepack_main    (const char* infile, const char* outfile, options_t *options);

HDFTOOLSAPI int  list(const char* infname,const char* outfname,options_t *options);
HDFTOOLSAPI int  read_info(const char *filename,options_t *options);

#ifdef __cplusplus
}
#endif


#endif  /* REPACK_H_ */
