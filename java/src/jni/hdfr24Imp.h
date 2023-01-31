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
/* Header for class hdf_hdflib_R24 */

#ifndef Included_hdf_hdflib_R24
#define Included_hdf_hdflib_R24

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24getdims(JNIEnv *env, jclass clss, jstring filename,
                                                                  jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24getimage(JNIEnv *env, jclass clss, jstring filename,
                                                                   jbyteArray image, jint width, jint height);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_DF24lastref(JNIEnv *env, jclass clss);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24readref(JNIEnv *env, jclass clss, jstring filename,
                                                                  jshort ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24restart(JNIEnv *env, jclass clss);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_DF24nimages(JNIEnv *env, jclass clss, jstring hdfFile);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24reqil(JNIEnv *env, jclass clss, jint interlace);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24addimage(JNIEnv *env, jclass clss, jstring filename,
                                                                   jbyteArray image, jint width, jint height);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24putimage(JNIEnv *env, jclass clss, jstring filename,
                                                                   jbyteArray image, jint width, jint height);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24setcompress(JNIEnv *env, jclass clss, jint type,
                                                                      jobject cinfo);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24setdims(JNIEnv *env, jclass clss, jint width,
                                                                  jint height);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DF24setil(JNIEnv *env, jclass clss, jint il);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_R24 */
