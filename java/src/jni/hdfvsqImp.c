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

#include <stdlib.h>

#include <jni.h>

#include "hdf.h"
#include "h4jni.h"
#include "hdfvsqImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQuerycount(JNIEnv *env, jclass clss, jlong vdata_id, jintArray n_records)
{
    int      rval   = FAIL;
    jint    *theArg = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (n_records == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSQuerycount: n_records is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, n_records) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSQuerycount: output array n_records < order 1");

    PIN_INT_ARRAY(ENVONLY, n_records, theArg, &isCopy, "VSQuerycount:  n_records not pinned");

    if ((rval = VSQuerycount((int32)vdata_id, (int32 *)&(theArg[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArg)
        UNPIN_INT_ARRAY(ENVONLY, n_records, theArg, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryfields(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray fields)
{
    int     rval = FAIL;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if ((data = (char *)malloc(4096)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSQueryfields: failed to allocate data buffer");

    if (fields == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSQueryfields: fields is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, fields) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSQueryfields: output array fields < order 1");

    if ((rval = VSQueryfields((int32)vdata_id, (char *)data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[4095] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, fields, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryinterlace(JNIEnv *env, jclass clss, jlong vdata_id, jintArray interlace)
{
    int      rval   = FAIL;
    jint    *theArg = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (interlace == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSQueryinterlace: interlace is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, interlace) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSQueryinterlace: output array interlace < order 1");

    PIN_INT_ARRAY(ENVONLY, interlace, theArg, &isCopy, "VSQueryinterlace:  interlace not pinned");

    if ((rval = VSQueryinterlace((int32)vdata_id, (int32 *)&(theArg[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArg)
        UNPIN_INT_ARRAY(ENVONLY, interlace, theArg, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryname(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray vdata_name)
{
    int     rval = FAIL;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if ((data = (char *)malloc(VSNAMELENMAX + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSQueryname: failed to allocate data buffer");

    if (vdata_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSQueryname: vdata_name is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, vdata_name) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSQueryname: output array vdata_name < order 1");

    if ((rval = VSQueryname((int32)vdata_id, (char *)data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[VSNAMELENMAX] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, vdata_name, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryref(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSQueryref((int32)vdata_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSQuerytag(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSQuerytag((int32)vdata_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryvsize(JNIEnv *env, jclass clss, jlong vdata_id, jintArray vdata_size)
{
    int      rval   = FAIL;
    jint    *theArg = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (vdata_size == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSQueryvsize: vdata_size is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, vdata_size) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSQueryvsize: output array vdata_size < order 1");

    PIN_INT_ARRAY(ENVONLY, vdata_size, theArg, &isCopy, "VSQuerysize: vdata_size not pinned");

    if ((rval = VSQueryvsize((int32)vdata_id, (int32 *)&(theArg[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArg)
        UNPIN_INT_ARRAY(ENVONLY, vdata_size, theArg, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
