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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <jni.h>
#include "hdfi.h"
#include "h4jni.h"
#include "hdfvqImp.h"

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VQueryref(JNIEnv *env, jclass clss, jlong vkey)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VQueryref((int32)vkey)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VQuerytag(JNIEnv *env, jclass clss, jlong vkey)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VQuerytag((int32)vkey)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
