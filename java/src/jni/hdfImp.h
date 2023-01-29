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
/* Header for class hdf_hdflib_HDF */

#ifndef Included_hdf_hdflib_HDF
#define Included_hdf_hdflib_HDF

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hopen
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_Hopen(JNIEnv *env, jclass clss, jstring hdfFile,
                                                         jint access, jint ndds);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hclose(JNIEnv *env, jclass clss, jlong fid);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDdont_atexit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_HDdont_1atexit(JNIEnv *env, jclass clss);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hishdf
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hishdf(JNIEnv *env, jclass clss, jstring hdfFile);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hnumber
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Hnumber(JNIEnv *env, jclass clss, jlong fid, jint tagtype);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    DFKNTsize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_DFKNTsize(JNIEnv *env, jclass clss, jlong numbertype);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hcache
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hcache(JNIEnv *env, jclass clss, jlong file_id,
                                                             jint cache_switch);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetfileversion
 * Signature: (J[I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hgetfileversion(JNIEnv *env, jclass clss, jlong file_id,
                                                                      jintArray vers, jobjectArray fvstring);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetlibversion
 * Signature: ([I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hgetlibversion(JNIEnv *env, jclass clss, jintArray vers,
                                                                     jobjectArray lvstring);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsetaccesstype
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hsetaccesstype(JNIEnv *env, jclass clss, jlong h_id,
                                                                     jint access_type);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsync
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Hsync(JNIEnv *env, jclass clss, jlong file_id);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_HDFclose(JNIEnv *env, jclass clss, jlong file_id);

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFopen
 * Signature: (Ljava/lang/String;IS)I
 */
JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_HDFopen(JNIEnv *env, jclass clss, jstring filename,
                                                          jint access, jshort n_dds);

#ifdef not_yet_implemented
/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFflusdd
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_HDFflusdd(JNIEnv *env, jclass clss, jlong file_id);
#endif

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDgetNTdesc
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_hdf_hdflib_HDFLibrary_HDgetNTdesc(JNIEnv *env, jclass clss, jint nt);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_HDF */
