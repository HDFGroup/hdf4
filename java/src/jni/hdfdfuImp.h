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
/* Header for class hdf_hdflib_DFU */

#ifndef Included_hdf_hdflib_DFU
#define Included_hdf_hdflib_DFU

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFUfptoimage(JNIEnv *env, jclass clss, jint hdim,
                                                                   jint vdim, jfloat max, jfloat min,
                                                                   jfloatArray hscale, jfloatArray vscale,
                                                                   jfloatArray data, jbyteArray palette,
                                                                   jstring outfile, jint ct_method, jint hres,
                                                                   jint vres, jint compress);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_DFU */
