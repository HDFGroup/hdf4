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
#include "hdfr24Imp.h"

extern jboolean getOldCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24getdims(JNIEnv *env, jclass clss, jstring filename, jintArray argv)
{
    intn        rval    = FAIL;
    const char *fstr    = NULL;
    jint       *theArgs = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24getdims: output array argv is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24getdims:  filename is null");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DF24getdims: output array argv < order 3");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "DF24getdims:  argv not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DF24getdims:  filename not pinned");

    /* get image dimension information */
    if ((rval = DF24getdims(fstr, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]), (intn *)&(theArgs[2]))) ==
        FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24getimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24getimage: output array image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24getimage:  filename is null");

    if (ENVPTR->GetArrayLength(ENVONLY, image) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DF24getimage: output array image invalid size");

    PIN_BYTE_ARRAY_CRITICAL(ENVONLY, image, dat, &isCopy, "DF24getimage:  image not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DF24getimage:  filename not pinned");

    if ((rval = DF24getimage(fstr, (void *)dat, (int32)width, (int32)height)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (dat)
        UNPIN_ARRAY_CRITICAL(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DF24lastref(JNIEnv *env, jclass clss)
{
    UNUSED(env);
    UNUSED(clss);

    return ((short)DF24lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24readref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int         rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24readref:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DF24readref:  filename not pinned");

    if ((rval = DF24readref(fstr, (short)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24restart(JNIEnv *env, jclass clss)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = DF24restart()) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DF24nimages(JNIEnv *env, jclass clss, jstring hdfFile)
{
    intn        rval     = FAIL;
    const char *hdf_file = NULL;

    UNUSED(clss);

    if (hdfFile == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24nimages:  hdfFile is null");

    PIN_JAVA_STRING(ENVONLY, hdfFile, hdf_file, NULL, "DF24nimages:  hdfFile not pinned");

    if ((rval = DF24nimages(hdf_file)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (hdf_file)
        UNPIN_JAVA_STRING(ENVONLY, hdfFile, hdf_file);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24reqil(JNIEnv *env, jclass clss, jint interlace)
{
    intn rval = FAIL;

    UNUSED(clss);

    if ((rval = DF24reqil((intn)interlace)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24addimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24addimage: image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24addimage:  filename is null");

    if (ENVPTR->GetArrayLength(ENVONLY, image) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DF24addimage: no image data");

    PIN_BYTE_ARRAY(ENVONLY, image, dat, &isCopy, "DF24addimage:  image not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DF24addimage:  filename is not pinned");

    if ((rval = DF24addimage((char *)fstr, (void *)dat, (int32)width, (int32)height)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24putimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24putimage: image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24putimage: filename is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, image) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DF24putimage: no image data");

    PIN_BYTE_ARRAY(ENVONLY, image, dat, &isCopy, "DF24putimage:  image not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DF24putimage:  filename is not pinned");

    if ((rval = DF24putimage(fstr, (void *)dat, (int32)width, (int32)height)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setcompress(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn      rval = FAIL;
    comp_info cinf;
    jboolean  bval = JNI_FALSE;

    UNUSED(clss);

    if (cinfo == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DF24setcompress: cinfo is NULL");

    bval = getOldCompInfo(env, cinfo, &cinf);

    if (bval == JNI_FALSE) {
        H4_JNI_FATAL_ERROR(ENVONLY, "DF24setcompress: cinfo not pinned");
    } /* end if */
    else {
        /* fill in cinf depending on the value of 'type' */
        if ((rval = DF24setcompress((int32)type, (comp_info *)&cinf)) == FAIL)
            H4_LIBRARY_ERROR(ENVONLY);
    }

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setdims(JNIEnv *env, jclass clss, jint width, jint height)
{
    intn rval = FAIL;

    UNUSED(clss);

    if ((rval = DF24setdims((int32)width, (int32)height)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setil(JNIEnv *env, jclass clss, jint il)
{
    intn rval = FAIL;

    UNUSED(clss);

    if ((rval = DF24setil((intn)il)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
