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
 *  This module contains the implementation of all the native methods
 *  used for number conversion.  This is represented by the Java
 *  class HDFNativeData.
 *
 *  These routines convert one dimensional arrays of bytes into
 *  one-D arrays of other types (int, float, etc) and vice versa.
 *
 *  These routines are called from the Java parts of the Java-C
 *  interface.
 *
 *  ***Important notes:
 *
 *     1.  These routines are designed to be portable--they use the
 *         C compiler to do the required native data manipulation.
 *     2.  These routines copy the data at least once -- a serious
 *         but unavoidable performance hit.
 */

#include <jni.h>
/* Header for class hdf_hdflib_Native */

#ifndef Included_hdf_hdflib_Native
#define Included_hdf_hdflib_Native

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* returns int [] */
JNIEXPORT jintArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToInt___3B(JNIEnv *env, jclass clss,
                                                                         jbyteArray bdata);

/* returns float [] */
JNIEXPORT jfloatArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToFloat___3B(JNIEnv *env, jclass clss,
                                                                             jbyteArray bdata);

/* returns short [] */
JNIEXPORT jshortArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToShort___3B(JNIEnv *env, jclass clss,
                                                                             jbyteArray bdata);

/* returns long [] */
JNIEXPORT jlongArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToLong___3B(JNIEnv *env, jclass clss,
                                                                           jbyteArray bdata);

/* returns double [] */
JNIEXPORT jdoubleArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToDouble___3B(JNIEnv *env, jclass clss,
                                                                               jbyteArray bdata);

/* returns int [] */
JNIEXPORT jintArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToInt__II_3B(JNIEnv *env, jclass clss,
                                                                           jint start, jint len,
                                                                           jbyteArray bdata);

/* returns short [] */
JNIEXPORT jshortArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToShort__II_3B(JNIEnv *env, jclass clss,
                                                                               jint start, jint len,
                                                                               jbyteArray bdata);

/* returns float [] */
JNIEXPORT jfloatArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToFloat__II_3B(JNIEnv *env, jclass clss,
                                                                               jint start, jint len,
                                                                               jbyteArray bdata);

/* returns long [] */
JNIEXPORT jlongArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToLong__II_3B(JNIEnv *env, jclass clss,
                                                                             jint start, jint len,
                                                                             jbyteArray bdata);

/* returns double [] */
JNIEXPORT jdoubleArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToDouble__II_3B(JNIEnv *env, jclass clss,
                                                                                 jint start, jint len,
                                                                                 jbyteArray bdata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_intToByte__II_3I(JNIEnv *env, jclass clss,
                                                                            jint start, jint len,
                                                                            jintArray idata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_shortToByte__II_3S(JNIEnv *env, jclass clss,
                                                                              jint start, jint len,
                                                                              jshortArray sdata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_floatToByte__II_3F(JNIEnv *env, jclass clss,
                                                                              jint start, jint len,
                                                                              jfloatArray fdata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_doubleToByte__II_3D(JNIEnv *env, jclass clss,
                                                                               jint start, jint len,
                                                                               jdoubleArray ddata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_longToByte__II_3J(JNIEnv *env, jclass clss,
                                                                             jint start, jint len,
                                                                             jlongArray ldata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_intToByte__I(JNIEnv *env, jclass clss, jint idata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_floatToByte__F(JNIEnv *env, jclass clss,
                                                                          jfloat fdata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_shortToByte__S(JNIEnv *env, jclass clss,
                                                                          jshort sdata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_doubleToByte__D(JNIEnv *env, jclass clss,
                                                                           jdouble ddata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_longToByte__J(JNIEnv *env, jclass clss,
                                                                         jlong ldata);

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_hdf_hdflib_HDFNativeData_byteToByte__B(JNIEnv *env, jclass clss,
                                                                         jbyte bdata);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_Native */
