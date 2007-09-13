/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at      *
 * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef HREPACKTST_H_
#define HREPACKTST_H_
 
#define HREPACK_FILE1         "hrepacktst1.hdf"
#define HREPACK_FILE1_OUT     "hrepacktst1_out.hdf"
#define HREPACK_FILE2         "hrepacktst2.hdf"
#define HREPACK_FILE2_OUT     "hrepacktst2_out.hdf"
#define HREPACK_FILE3         "hrepacktst3.hdf"
#define HREPACK_FILE3_OUT     "hrepacktst3_out.hdf"

#define DATA_FILE1            "image8.txt"
#define DATA_FILE2            "image24pixel.txt"
#define DATA_FILE3            "image24plane.txt"

#define HDIFF_TSTSTR     "hdiff hrepacktst1.hdf hrepacktst1_out.hdf"
#define HDIFF_TSTSTR2    "hdiff hrepacktst2.hdf hrepacktst2_out.hdf"
#define HDIFF_TSTSTR3    "hdiff hrepacktst3.hdf hrepacktst3_out.hdf"



#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout); }
#define PASSED() {puts(" PASSED");fflush(stdout);}
#define SKIPPED() {puts(" SKIPPED");fflush(stdout);}
#define H4_FAILED() {puts("*FAILED*");fflush(stdout);}


#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------
 * SDS verify routines
 *-------------------------------------------------------------------------
 */
int sds_verifiy_comp(const char *sds_name, int32 in_comp_type, int32 in_comp_info);
int sds_verifiy_comp_all(comp_coder_t in_comp_type, int in_comp_info);
int sds_verifiy_chunk(const char *sds_name, int32 in_chunk_flags, int rank, 
                      int32 *in_chunk_lengths);
int sds_verifiy_chunk_all(int32 in_chunk_flags, int rank, 
                          int32 *in_chunk_lengths,const char *sds_exclude);


/*-------------------------------------------------------------------------
 * VG verify routines
 *-------------------------------------------------------------------------
 */

int vg_verifygrpdep( char* name1, char* name2 );


/*-------------------------------------------------------------------------
 * write data
 *-------------------------------------------------------------------------
 */

int generate_files();

int add_gr_ffile(const char* name_file,
                  int32 gr_id,
                  const char* gr_name,
                  int32 interlace_mode,
                  int32 file_id,
                  int32 vgroup_id);

int add_gr(const char* gr_name,     /* gr name */
            int32 file_id,           /* file ID */
            int32 gr_id,             /* GR ID */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            comp_coder_t comp_type,  /* compression flag */
            comp_info *comp_info     /* compression structure */ );


int add_file_an (int32 file_id);
int add_an(int32 file_id, int32 tag, int32 ref);
int add_r8 (const char* image_file,const char *fname,int32 file_id,int32 vgroup_id);
int add_r24(const char* image_file,const char *fname,int32 file_id,intn il,int32 vgroup_id);
int add_vs (const char* vs_name,int32 file_id,int32 vgroup_id);


int add_sd(const char *fname,       /* file name */
            int32 file_id,           /* file ID */
            int32 sd_id,             /* SD id */
            const char* sds_name,    /* sds name */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            comp_coder_t comp_type,  /* compression flag */
            comp_info *comp_info     /* compression structure */ );

void set_chunk_def( comp_coder_t comp_type, 
                           int32 *dim,
                           int32 ncomps,
                           int32 bits_per_pixel, /* for szip */
                           HDF_CHUNK_DEF *chunk_def );


int add_sd_szip(const char *fname,        /* file name */
                 int32 file_id,           /* file ID */
                 int32 sd_id,             /* SD interface identifier */
                 const char* sds_name,    /* sds name */
                 int32 vgroup_id,         /* group ID */
                 int32 chunk_flags,       /* chunk flags */
                 int32 nt,                /* number type */
                 int32 bits_per_pixel,    /* szip parameter */
                 int32 *dim,              /* dimension of the data set */
                 void *data
                 );


int add_sd3d(const char *fname,       /* file name */
              int32 file_id,           /* file ID */
              int32  sd_id,            /* SD interface identifier */
              const char* sds_name,    /* sds name */
              int32 vgroup_id,         /* group ID */
              int32 chunk_flags,       /* chunk flags */
              comp_coder_t comp_type,  /* compression flag */
              comp_info *comp_info     /* compression structure */ );

int add_empty_sd(int32 sd_id,             /* SD id */
                 const char* sds_name     /* sds name */
                );

int add_pal(const char* fname);

int add_sd_szip_all(const char *fname,       /* file name */
                     int32 file_id,           /* file ID */
                     int32 sd_id,             /* SD interface identifier */
                     int32 vgroup_id          /* group ID */
                     );

int add_glb_attrs(const char *fname,
                   int32 file_id,
                   int32 sd_id,
                   int32 gr_id);


/* read image data */
int  read_data(const  char* file_name);

int  do_big_file(char * file_name);
int  do_lone(char* file_name);




#ifdef __cplusplus
}
#endif


#endif  /* HREPACKTST_H_ */
