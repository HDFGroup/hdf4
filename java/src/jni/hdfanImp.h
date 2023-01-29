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

#include <jni.h>
/* Header for class hdf_hdflib_AN */

#ifndef Included_hdf_hdflib_AN
#define Included_hdf_hdflib_AN

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_ANstart(JNIEnv *env, jclass clss, jlong file_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANend(JNIEnv *env, jclass clss, jlong anid);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANfileinfo(JNIEnv *env, jclass clss, jlong anid,
                                                                 jintArray info);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_ANselect(JNIEnv *env, jclass clss, jlong anid, jint index,
                                                            jint anntype);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANendaccess(JNIEnv *env, jclass clss, jlong ann_id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANnumann(JNIEnv *env, jclass clss, jlong an_id,
                                                           jint anntype, jshort tag, jshort ref);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_ANatype2tag(JNIEnv *env, jclass clss, jint antype);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANtag2atype(JNIEnv *env, jclass clss, jint antag);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANannlist(JNIEnv *env, jclass clss, jlong an_id,
                                                            jint anntype, jshort tag, jshort ref,
                                                            jintArray annlist);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANannlen(JNIEnv *env, jclass clss, jlong ann_id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANreadann(JNIEnv *env, jclass clss, jlong ann_id,
                                                                jobjectArray annbuf, jint maxlen);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANcreate(JNIEnv *env, jclass clss, jlong an_id, jshort tag,
                                                           jshort ref, jint type);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_ANcreatef(JNIEnv *env, jclass clss, jlong an_id,
                                                             jint type);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_ANget_1tagref(JNIEnv *env, jclass clss, jlong an_id,
                                                                jint index, jint type, jshortArray tagref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANid2tagref(JNIEnv *env, jclass clss, jlong an_id,
                                                                  jshortArray tagref);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_ANtagref2id(JNIEnv *env, jclass clss, jlong an_id,
                                                               jshort tag, jshort ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_ANwriteann(JNIEnv *env, jclass clss, jlong ann_id,
                                                                 jstring label, jint ann_length);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_AN */
