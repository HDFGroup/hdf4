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
 *  This module contains code to translate between the HDF C union 'comp_info'
 *  and a subclass of the Java class CompInfo.
 *
 *  This is nasty and ugly and probably buggy.
 *
 */

#include <jni.h>
/* Header for class hdf_hdflib_UTIL */

#ifndef Included_hdf_hdflib_UTIL
#define Included_hdf_hdflib_UTIL

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 *  Get information from a Java HDFNewCompInfo object in to a C comp_info
 *  struct.
 *
 *  Extract information for the different types of compression.
 */

jboolean getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);

/*
 *   Extract info from C comp_info struct, put in Java HDFCompInfo object.
 *
 *   Put in the fields for each compression method.
 */
jboolean setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf);

/*
 *     Get info from old style C comp_info struct, put in HDFCompInfo object.
 */
jboolean getOldCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);

/*
 *  Get the Chunk info from C HDF_CHUNK_DEF struct, put in
 *  Java HDFChunkInfo object.
 */
jboolean getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf);

/*
 *  Create C HDF_CHUNK_DEF struct from Java HDFChunkInfo object.
 *
 *  Determine the compression method, and create an appropriate subclass
 *  of HDFCompInfo.  Then call the constructor for HDFChunkInfo.
 */
jboolean makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_HCget_1config_1info(JNIEnv *env, jclass clss,
                                                                      jint coder_type);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_UTIL */
