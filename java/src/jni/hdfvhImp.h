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
/* Header for class hdf_hdflib_VH */

#ifndef Included_hdf_hdflib_VH
#define Included_hdf_hdflib_VH

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VHmakegroup(JNIEnv *env, jclass clss, jlong file_id,
                                                              jintArray tag_array, jintArray ref_array,
                                                              jint n_objects, jstring vgroup_name,
                                                              jstring vgroup_class);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VHstoredata(JNIEnv *env, jclass clss, jlong file_id,
                                                              jstring fieldname, jbyteArray buf,
                                                              jint n_records, jint data_type,
                                                              jstring vdata_name, jstring vdata_class);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_VHstoredatam(JNIEnv *env, jclass clss, jlong file_id,
                                                               jstring fieldname, jbyteArray buf,
                                                               jint n_records, jint data_type,
                                                               jstring vdata_name, jstring vdata_class,
                                                               jint order);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_VH */
