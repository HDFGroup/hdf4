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
/* Header for class hdf_hdflib_VDATA */

#ifndef Included_hdf_hdflib_VDATA
#define Included_hdf_hdflib_VDATA

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_VSattach(JNIEnv *env, jclass clss, jlong fid,
                                                            jint vdata_ref, jstring accessmode);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSdetach(JNIEnv *env, jclass clss, jlong vdata_id);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_VSgetid(JNIEnv *env, jclass clss, jlong fid,
                                                           jint vdata_ref);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSgetclass(JNIEnv *env, jclass clss, jlong vdata_id,
                                                             jobjectArray hdfclass);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSgetname(JNIEnv *env, jclass clss, jlong vdata_id,
                                                            jobjectArray hdfname);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSelts(JNIEnv *env, jclass clss, jlong vdata_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSfdefine(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                jstring fieldname, jint numbertype,
                                                                jint order);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSfexist(JNIEnv *env, jclass clss, jlong vdata_id,
                                                               jstring fieldname);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSfind(JNIEnv *env, jclass clss, jlong vdata_id,
                                                         jstring fieldname);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSsetblocksize(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                 jint blocksize);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSsetnumblocks(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                 jint numblocks);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSgetfields(JNIEnv *env, jclass clss, jlong vdata_id,
                                                              jobjectArray fields);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSgetinterlace(JNIEnv *env, jclass clss, jlong vdata_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSinquire(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                jintArray iargs, jobjectArray sargs);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSgetblockinfo(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                     jintArray iargs);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSlone(JNIEnv *env, jclass clss, jlong fid,
                                                         jintArray ref_array, jint arraysize);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSread(JNIEnv *env, jclass clss, jlong vdata_id,
                                                         jbyteArray databuf, jint nrecords, jint interlace);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSseek(JNIEnv *env, jclass clss, jlong vdata_id,
                                                         jint nrecord);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSsetfields(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                  jstring fields);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSsetinterlace(JNIEnv *env, jclass clss, jlong vdata_id,
                                                                     jint interlace);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSsizeof(JNIEnv *env, jclass clss, jlong vdata_id,
                                                           jstring fields);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSappendable(JNIEnv *env, jclass clss, jint vkey,
                                                                   jint block_size);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSdump(JNIEnv *env, jclass clss, jint vkey);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSfindclass(JNIEnv *env, jclass clss, jlong vdata_id,
                                                              jstring hdfclassname);

/* no idea at all how to deal with 2-D arrays.... */
JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSfpack(JNIEnv *env, jclass clss, jlong vdata_id,
                                                          jint action, jstring fields_in_buf, jarray buf,
                                                          jint bufsize, jstring fields, jarray bufptrs);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSgetversion(JNIEnv *env, jclass clss, jint key);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSsetclass(JNIEnv *env, jclass clss, jlong vdata_id,
                                                             jstring vdata_class);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSsetexternalfile(JNIEnv *env, jclass clss, jint vkey,
                                                                        jstring filename, jint offset);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_VSsetname(JNIEnv *env, jclass clss, jlong vdata_id,
                                                            jstring vdata_name);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSwrite(JNIEnv *env, jclass clss, jlong vdata_id,
                                                          jbyteArray databuf, jint n_records, jint interlace);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSattrinfo(JNIEnv *env, jclass clss, jlong id,
                                                                 jint index, jint attr_index,
                                                                 jobjectArray name, jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSfindex(JNIEnv *env, jclass clss, jlong id,
                                                               jstring name, jintArray findex);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSfindattr(JNIEnv *env, jclass clss, jlong id, jint index,
                                                             jstring name);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSfnattrs(JNIEnv *env, jclass clss, jlong id, jint attr);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSgetattr(JNIEnv *env, jclass clss, jlong id,
                                                                jint field_index, jint attr_index,
                                                                jbyteArray values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSisattr(JNIEnv *env, jclass clss, jlong id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VSnattrs(JNIEnv *env, jclass clss, jlong id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JILjava_lang_String_2(
    JNIEnv *env, jclass clss, jlong id, jint index, jstring attr_name, jlong data_type, jint count,
    jstring values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JI_3B(
    JNIEnv *env, jclass clss, jlong id, jint index, jstring attr_name, jlong data_type, jint count,
    jbyteArray values);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_VDATA */
