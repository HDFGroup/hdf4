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


#ifndef REPACK_VERIFY_H
#define REPACK_VERIFY_H



#ifdef __cplusplus
extern "C" {
#endif

 
#define HREPACK_FILE1         "hrepacktest1.hdf"
#define HREPACK_FILE1_OUT     "hrepacktest1_out.hdf"
#define HREPACK_FILE2         "hrepacktest2.hdf"
#define HREPACK_FILE2_OUT     "hrepacktest2_out.hdf"


int sds_verifiy_comp(const char *sds_name, int32 in_comp_type, int32 in_comp_info);
int sds_verifiy_comp_all(comp_coder_t in_comp_type, int in_comp_info);
int sds_verifiy_chunk(const char *sds_name, int32 in_chunk_flags, int rank, 
                      int32 *in_chunk_lengths);
int sds_verifiy_chunk_all(int32 in_chunk_flags, int rank, 
                          int32 *in_chunk_lengths,const char *sds_exclude);


#ifdef __cplusplus
}
#endif


#endif  /* REPACK_VERIFY_H */
