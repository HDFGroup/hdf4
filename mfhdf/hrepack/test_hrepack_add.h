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


#ifndef REPACK_ADD_H_
#define REPACK_ADD_H_

#define TESTING(WHAT) {printf("%-70s", "Testing " WHAT); fflush(stdout); }
#define PASSED() {puts(" PASSED");fflush(stdout);}
#define SKIPPED() {puts(" SKIPPED");fflush(stdout);}
#define H4_FAILED() {puts("*FAILED*");fflush(stdout);}

#ifdef __cplusplus
extern "C" {
#endif


/* write data */

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


#endif  /* REPACK_ADD_H_ */
