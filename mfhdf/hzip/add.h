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


#ifndef HDF_ZIP_ADD__
#define HDF_ZIP_ADD__


#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout);}
#define PASSED() {puts(" PASSED");fflush(stdout);}
#define H5_FAILED() {puts("*FAILED*");fflush(stdout);}

#ifdef __cplusplus
extern "C" {
#endif


/* write data */
void add_an (int32 file_id);
void add_glb_attrs(char *fname,int32 file_id);
void add_gr (char* name_file,char* gr_name,int32 file_id,int32 vgroup_id);
void add_r8 (char *fname,char* name_file,int32 vgroup_id);
void add_r24(char *fname,char* name_file,int32 vgroup_id);
void add_vs (char* vs_name,int32 file_id,int32 vgroup_id);
void add_sd (char *fname,
             char* sds_name,          /* sds name */
             int32 vgroup_id,         /* group ID */
             int32 chunk_flags,       /* chunk flags */
             int32 comp_type,         /* compression flag */
             comp_info *c_info        /* compression structure */ );


/* read image data */
int  read_data(char* file_name);


#ifdef __cplusplus
}
#endif


#endif  /* HDF_ZIP_ADD__ */
