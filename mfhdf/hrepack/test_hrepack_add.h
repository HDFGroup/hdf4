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
#define H4_FAILED() {puts("*FAILED*");fflush(stdout);}

#ifdef __cplusplus
extern "C" {
#endif


/* write data */
void add_file_an (int32 file_id);
void add_an(int32 file_id, int32 tag, int32 ref);
void add_glb_attrs(char *fname,int32 file_id);
void add_gr_ffile (char* name_file,char* gr_name,int32 im,int32 file_id,int32 vgroup_id);
void add_r8 (char* image_file,char *fname,int32 file_id,int32 vgroup_id);
void add_r24(char* image_file,char *fname,int32 file_id,intn il,int32 vgroup_id);
void add_vs (char* vs_name,int32 file_id,int32 vgroup_id);
void add_sd (char *fname,
             int32 file_id,           /* file ID */
             char* sds_name,          /* sds name */
             int32 vgroup_id,         /* group ID */
             int32 chunk_flags,       /* chunk flags */
             int32 comp_type,         /* compression flag */
             comp_info *c_info        /* compression structure */ );

void add_sd3d(char *fname,
              int32 file_id,           /* file ID */
              char* sds_name,          /* sds name */
              int32 vgroup_id,         /* group ID */
              int32 chunk_flags,       /* chunk flags */
              int32 comp_type,         /* compression flag */
              comp_info *c_info        /* compression structure */ );

void add_gr(char* gr_name,           /* gr name */
            int32 file_id,           /* file ID */
            int32 vgroup_id,         /* group ID */
            int32 chunk_flags,       /* chunk flags */
            int32 comp_type,         /* compression flag */
            comp_info *comp_info     /* compression structure */ );

void add_pal(char* fname);


/* read image data */
int  read_data(char* file_name);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_ADD_H_ */
