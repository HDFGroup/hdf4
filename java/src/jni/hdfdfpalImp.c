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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "hdfi.h"
#include "h4jni.h"
#include "hdfdfpalImp.h"

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPaddpal(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPaddpal:  filename is null");

    if (palette == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPaddpal:  palette is NULL");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPaddpal:  filename not pinned");
    PIN_BYTE_ARRAY(ENVONLY, palette, dat, &isCopy, "DFPaddpal:  palette not pinned");

    if ((rval = DFPaddpal(fstr, (void *)dat)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, palette, dat, (rval == FAIL) ? JNI_ABORT : 0);
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPgetpal(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPgetpal:  filename is null");

    if (palette == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPgetpal:  palette is NULL");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPgetpal:  filename not pinned");
    PIN_BYTE_ARRAY(ENVONLY, palette, dat, &isCopy, "DFPgetpal:  palette not pinned");

    if ((rval = DFPgetpal(fstr, (void *)dat)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, palette, dat, (rval == FAIL) ? JNI_ABORT : 0);
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFPlastref(JNIEnv *env, jclass clss)
{
    UNUSED(env);
    UNUSED(clss);

    return (DFPlastref());
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFPnpals(JNIEnv *env, jclass clss, jstring filename)
{
    intn        rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPnpals:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPnpals:  filename not pinned");

    if ((rval = DFPnpals(fstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPputpal(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette,
                                     jint overwrite, jstring filemode)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    const char *mstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPputpal:  filename is null");

    if (filemode == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPputpal:  filemode is null");

    if (palette == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPputpal:  palette is NULL");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPputpal:  filename not pinned");
    PIN_JAVA_STRING(ENVONLY, filemode, mstr, NULL, "DFPputpal:  filemode not pinned");
    PIN_BYTE_ARRAY(ENVONLY, palette, dat, &isCopy, "DFPputpal:  palette not pinned");
    if ((rval = DFPputpal(fstr, (void *)dat, (intn)overwrite, mstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, palette, dat, (rval == FAIL) ? JNI_ABORT : 0);
    if (mstr)
        UNPIN_JAVA_STRING(ENVONLY, filemode, mstr);
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPreadref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    intn        rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPreadref:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPreadref:  filename not pinned");

    if ((rval = DFPreadref((char *)fstr, (uint16)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFPrestart(JNIEnv *env, jclass clss)
{
    UNUSED(env);
    UNUSED(clss);

    return (DFPrestart());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPwriteref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    intn        rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPwriteref:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPwriteref:  filename not pinned");

    if ((rval = DFPwriteref((char *)fstr, (uint16)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
