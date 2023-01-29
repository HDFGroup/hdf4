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
 *  HDF 4.2 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */

#include <jni.h>
/* Header for class hdf_hdflib_DFPAL */

#ifndef Included_hdf_hdflib_DFPAL
#define Included_hdf_hdflib_DFPAL

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFPaddpal(JNIEnv *env, jclass clss, jstring filename,
                                                                jbyteArray palette);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFPgetpal(JNIEnv *env, jclass clss, jstring filename,
                                                                jbyteArray palette);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_DFPlastref(JNIEnv *env, jclass clss);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_DFPnpals(JNIEnv *env, jclass clss, jstring filename);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFPputpal(JNIEnv *env, jclass clss, jstring filename,
                                                                jbyteArray palette, jint overwrite,
                                                                jstring filemode);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFPreadref(JNIEnv *env, jclass clss, jstring filename,
                                                                 jshort ref);

JNIEXPORT jshort JNICALL Java_hdf_hdflib_HDFLibrary_DFPrestart(JNIEnv *env, jclass clss);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFPwriteref(JNIEnv *env, jclass clss, jstring filename,
                                                                  jshort ref);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_DFPAL */
