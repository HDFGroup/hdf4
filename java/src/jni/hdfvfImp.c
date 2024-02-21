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

#include "hdf.h"
#include "h4jni.h"
#include "hdfvfImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldesize(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VFfieldesize((int32)vdata_id, (int32)field_index)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldisize(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VFfieldisize((int32)vdata_id, (int32)field_index)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jstring JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldname(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    jstring rstring = NULL;
    char   *str;

    UNUSED(clss);

    if ((str = VFfieldname((int32)vdata_id, (int32)field_index)) == NULL)
        H4_LIBRARY_ERROR(ENVONLY);

    /* convert it to java string */
    rstring = ENVPTR->NewStringUTF(ENVONLY, str);

done:
    return rstring;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldorder(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VFfieldorder((int32)vdata_id, (int32)field_index)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldtype(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VFfieldtype((int32)vdata_id, (int32)field_index)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFnfields(JNIEnv *env, jclass clss, jlong key)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VFnfields((int32)key)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
