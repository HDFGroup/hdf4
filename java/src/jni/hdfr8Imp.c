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
#include "hdfr8Imp.h"

extern jboolean getOldCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getdims(JNIEnv *env, jclass clss, jstring filename, jintArray argv,
                                       jbooleanArray isp)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    int         ispal;
    jint       *theArgs = NULL;
    jboolean   *theB    = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getdims:  filename is null");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getdims: output array argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DFR8getdims: argv output array < order 2");

    if (isp == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getdims: output array isp is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, isp) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DFR8getdims: output array isp < order 1");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFR8getdims:  filename not pinned");
    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "DFR8getdims:  argv not pinned");
    PIN_BOOL_ARRAY(ENVONLY, isp, theB, &isCopy, "DFR8getdims:  isp not pinned");

    theB[0] = JNI_FALSE;
    /* get image dimension information */
    if ((rval = DFR8getdims(fstr, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]), (intn *)&ispal)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (ispal)
        theB[0] = JNI_TRUE;
    else
        theB[0] = JNI_FALSE;

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (theB)
        UNPIN_BOOL_ARRAY(ENVONLY, isp, theB, (rval == FAIL) ? JNI_ABORT : 0);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height, jbyteArray palette)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jbyte      *pal  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getimage: output array image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getimage:  filename is null");

    PIN_BYTE_ARRAY_CRITICAL(ENVONLY, image, dat, &isCopy, "DFR8getimage:  palette not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFR8getimage:  filename not pinned");

    if ((rval = DFR8getimage((char *)fstr, (uint8 *)dat, (int32)width, (int32)height, (uint8 *)NULL)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    PIN_BYTE_ARRAY(ENVONLY, palette, pal, &isCopy, "DFR8getimage:  palette not pinned");

    if ((rval = DFR8getimage((char *)fstr, (uint8 *)dat, (int32)width, (int32)height, (uint8 *)pal)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (pal)
        UNPIN_BYTE_ARRAY(ENVONLY, palette, pal, (rval == FAIL) ? JNI_ABORT : 0);
    if (dat)
        UNPIN_ARRAY_CRITICAL(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8lastref(JNIEnv *env, jclass clss)
{
    UNUSED(env);
    UNUSED(clss);

    return ((short)DFR8lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8readref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int         rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8readref:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFR8readref:  filename not pinned");

    if ((rval = DFR8readref(fstr, (short)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8restart(JNIEnv *env, jclass clss)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = DFR8restart()) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8nimages(JNIEnv *env, jclass clss, jstring hdfFile)
{
    int         rval     = FAIL;
    const char *hdf_file = NULL;

    UNUSED(clss);

    if (hdfFile == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8nimages:  hdfFile is null");

    PIN_JAVA_STRING(ENVONLY, hdfFile, hdf_file, NULL, "DFR8nimages:  hdfFile not pinned");

    if ((rval = DFR8nimages(hdf_file)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (hdf_file)
        UNPIN_JAVA_STRING(ENVONLY, hdfFile, hdf_file);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8addimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height, jshort compress)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8addimage: image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8addimage:  filename is null");

    PIN_BYTE_ARRAY(ENVONLY, image, dat, &isCopy, "DFR8addimage:  image not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFR8addimage:  filename not pinned");

    if ((rval = DFR8addimage((char *)fstr, (void *)dat, (int32)width, (int32)height, (uint16)compress)) ==
        FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8putimage(JNIEnv *env, jclass clss, jstring filename, jbyteArray image,
                                        jint width, jint height, jint compress)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    jbyte      *dat  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (image == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8putimage: image is NULL");

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8putimage:  filename is null");

    PIN_BYTE_ARRAY(ENVONLY, image, dat, &isCopy, "DFR8putimage:  image not pinned");
    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFR8putimage:  filename not pinned");

    if ((rval = DFR8putimage((char *)fstr, (void *)dat, (int32)width, (int32)height, (uint16)compress)) ==
        FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, image, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8setcompress(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn      rval = FAIL;
    comp_info cinf;

    UNUSED(clss);

    if (cinfo == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8setcompress: cinfo is NULL");

    if (getOldCompInfo(env, cinfo, &cinf) == JNI_FALSE)
        H4_JNI_FATAL_ERROR(ENVONLY, "DFR8setcompress: cinfo not pinned");

    /* fill in cinf depending on the value of 'type' */
    if ((rval = DFR8setcompress((int32)type, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getpalref(JNIEnv *env, jclass clss, jshortArray palref)
{
    int      rval    = FAIL;
    short   *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (palref == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFR8getpalref: output array palref is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, palref) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "DFR8getpalref: output array palref < order 1");

    PIN_SHORT_ARRAY(ENVONLY, palref, theArgs, &isCopy, "DFR8getpalref:  palref not pinned");

    if ((rval = DFR8getpalref((uint16 *)&(theArgs[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_SHORT_ARRAY(ENVONLY, palref, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8setpalette(JNIEnv *env, jclass clss, jbyteArray palette)
{
    int      rval = FAIL;
    jbyte   *dat  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (palette == NULL)
        dat = NULL;
    else
        PIN_BYTE_ARRAY(ENVONLY, palette, dat, &isCopy, "DFR8setpalette:  palette not pinned");

    if ((rval = DFR8setpalette((uint8 *)dat)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, palette, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8writeref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int         rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "DFPwriteref:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "DFPwriteref:  filename not pinned");

    if ((rval = DFR8writeref(fstr, (short)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
