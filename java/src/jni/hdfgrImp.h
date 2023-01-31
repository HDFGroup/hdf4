/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */

#include <jni.h>
/* Header for class hdf_hdflib_GR */

#ifndef Included_hdf_hdflib_GR
#define Included_hdf_hdflib_GR

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_GRstart(JNIEnv *env, jclass clss, jlong file_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRend(JNIEnv *env, jclass clss, jlong gr_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRendaccess(JNIEnv *env, jclass clss, jlong gr_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRfileinfo(JNIEnv *env, jclass clss, jlong gr_id,
                                                                 jintArray argv);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_GRselect(JNIEnv *env, jclass clss, jlong gr_id,
                                                            jint index);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_GRnametoindex(JNIEnv *env, jclass clss, jlong gr_id,
                                                                jstring gr_name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetchunkinfo(JNIEnv *env, jclass clss, jlong grsid,
                                                                     jobject chunk_def, jintArray cflags);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetiminfo(JNIEnv *env, jclass clss, jlong ri_id,
                                                                  jobjectArray gr_name, jintArray argv,
                                                                  jintArray dim_sizes);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRreadimage(JNIEnv *env, jclass clss, jlong ri_id,
                                                                  jintArray start, jintArray stride,
                                                                  jintArray edge, jbyteArray data);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_GRidtoref(JNIEnv *env, jclass clss, jlong gr_id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_GRreftoindex(JNIEnv *env, jclass clss, jlong gr_id,
                                                               jshort ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRreqlutil(JNIEnv *env, jclass clss, jlong gr_id,
                                                                 jint interlace_mode);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRreqimageil(JNIEnv *env, jclass clss, jlong gr_id,
                                                                   jint interlace_mode);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_GRgetlutid(JNIEnv *env, jclass clss, jlong gr_id,
                                                              jint index);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_GRgetnluts(JNIEnv *env, jclass clss, jlong gr_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetlutinfo(JNIEnv *env, jclass clss, jlong ri_id,
                                                                   jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRreadlut(JNIEnv *env, jclass clss, jlong pal_id,
                                                                jbyteArray pal_data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRattrinfo(JNIEnv *env, jclass clss, jlong gr_id,
                                                                 jint attr_index, jobjectArray name,
                                                                 jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetattr(JNIEnv *env, jclass clss, jlong gr_id,
                                                                jint attr_index, jbyteArray values);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_GRfindattr(JNIEnv *env, jclass clss, jlong gr_id,
                                                             jstring attr_name);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_GRcreate(JNIEnv *env, jclass clss, jlong gr_id,
                                                            jstring name, jint ncomp, jlong data_type,
                                                            jint interlace_mode, jintArray dim_sizes);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_GRluttoref(JNIEnv *env, jclass clss, jlong pal_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JILjava_lang_String_2(
    JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JI_3B(
    JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name, jlong data_type, jint count, jbyteArray values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRsetcompress(JNIEnv *env, jclass clss, jlong ri_id,
                                                                    jint comp_type, jobject c_info);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetcompress(JNIEnv *env, jclass clss, jlong ri_id,
                                                                    jobject c_info);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRgetcompinfo(JNIEnv *env, jclass clss, jlong ri_id,
                                                                    jobject c_info);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRsetchunk(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jobject chunk_def, jint flags);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_GRsetchunkcache(JNIEnv *env, jclass clss, jlong sdsid,
                                                                  jint maxcache, jint flags);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRsetexternalfile(JNIEnv *env, jclass clss, jlong ri_id,
                                                                        jstring filename, jint offset);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRwriteimage(JNIEnv *env, jclass clss, jlong ri_id,
                                                                   jintArray start, jintArray stride,
                                                                   jintArray edge, jbyteArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRwritelut(JNIEnv *env, jclass clss, jlong pal_id,
                                                                 jint ncomp, jint data_type, jint interlace,
                                                                 jint num_entries, jbyteArray pal_data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRreadchunk(JNIEnv *env, jclass clss, jlong grid,
                                                                  jintArray origin, jbyteArray dat);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_GRwritechunk(JNIEnv *env, jclass clss, jlong grid,
                                                                   jintArray origin, jbyteArray dat);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_GR */
