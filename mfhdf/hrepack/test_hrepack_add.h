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


#ifndef REPACK_ADD_H_
#define REPACK_ADD_H_


/* turn on for verbose output of hrepack and hdiff */
#if 0
#define HZIPTST_DEBUG  
#endif

#if defined (HZIPTST_DEBUG)
#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout); printf("\n");}
#else
#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout); }
#endif
#define PASSED() {puts(" PASSED");fflush(stdout);}
#define SKIPPED() {puts(" SKIPPED");fflush(stdout);}
#define H4_FAILED() {puts("*FAILED*");fflush(stdout);}

#ifdef __cplusplus
extern "C" {
#endif


/* write data */
void add_file_an (int32 file_id);
void add_an(int32 file_id, int32 tag, int32 ref);
void add_glb_attrs(const char *fname,int32 file_id);
void add_gr_ffile (const char* name_file,const char* gr_name,int32 im,int32 file_id,int32 vgroup_id);
void add_r8 (const char* image_file,const char *fname,int32 file_id,int32 vgroup_id);
void add_r24(const char* image_file,const char *fname,int32 file_id,intn il,int32 vgroup_id);
void add_vs (const char* vs_name,int32 file_id,int32 vgroup_id);
void add_sd (const char *fname,
             int32 file_id,           /* file ID */
             const char* sds_name,    /* sds name */
             int32 vgroup_id,         /* group ID */
             int32 chunk_flags,       /* chunk flags */
             comp_coder_t comp_type,  /* compression flag */
             comp_info *c_info        /* compression structure */ );

void add_sd3d(const char *fname,       /* file name */
              int32 file_id,           /* file ID */
              const char* sds_name,    /* sds name */
              int32 vgroup_id,         /* group ID */
              int32 chunk_flags,       /* chunk flags */
              comp_coder_t comp_type,  /* compression flag */
              comp_info *comp_info     /* compression structure */ );

void add_gr(const char* gr_name,     /* gr name */
            int32 file_id,           /* file ID */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            comp_coder_t comp_type,  /* compression flag */
            comp_info *comp_info     /* compression structure */ );

void add_pal(const char* fname);

void add_sd_szip_all(const  char *fname,      /* file name */
                     int32 file_id,           /* file ID */
                     int32 vgroup_id          /* group ID */
                     );


/* read image data */
int  read_data(const  char* file_name);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_ADD_H_ */
