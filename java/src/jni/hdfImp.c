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
#include "hfile_priv.h" /* only needed for library version symbols */
#include "h4jni.h"
#include "hdfImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hopen
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Hopen(JNIEnv *env, jclass clss, jstring hdfFile, jint access, jint ndds)
{
    int32       rval = FAIL;
    const char *file = NULL;

    UNUSED(clss);

    if (hdfFile == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Hopen:  hdfFile is null");

    PIN_JAVA_STRING(ENVONLY, hdfFile, file, NULL, "Hopen:  hdfFile is not pinned");

    /* open HDF file specified by hdf_HDF_file */
    if ((rval = Hopen(file, (int)access, (int16)ndds)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (file)
        UNPIN_JAVA_STRING(ENVONLY, hdfFile, file);

    return (jlong)rval;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hclose(JNIEnv *env, jclass clss, jlong fid)
{
    int status = 0;

    UNUSED(clss);

    if (fid < 0) {
        /* maybe not an exception -- the file is already closed? */
        return JNI_FALSE;
    }
    /* close the HDF file */
    if ((status = Hclose((int32)fid)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDdont_atexit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HDdont_1atexit(JNIEnv *env, jclass clss)
{
    int ret_value = SUCCEED;

    UNUSED(env);
    UNUSED(clss);

    ret_value = HDdont_atexit();

    return (jint)ret_value;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hishdf
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hishdf(JNIEnv *env, jclass clss, jstring hdfFile)
{
    const char *hfile = NULL;
    int         rval  = FALSE;

    UNUSED(clss);

    if (hdfFile == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Hishdf:  hdfFile is null");

    PIN_JAVA_STRING(ENVONLY, hdfFile, hfile, NULL, "Hishdf:  hdfFile is not pinned");

    /* open HDF file specified by hdf_HDF_file */
    if ((rval = Hishdf(hfile)) == FALSE)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (hfile)
        UNPIN_JAVA_STRING(ENVONLY, hdfFile, hfile);

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hnumber
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Hnumber(JNIEnv *env, jclass clss, jlong fid, jint tagtype)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Hnumber((int32)fid, (uint16)tagtype)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    DFKNTsize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFKNTsize(JNIEnv *env, jclass clss, jlong numbertype)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = DFKNTsize((int32)numbertype)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hcache
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hcache(JNIEnv *env, jclass clss, jlong file_id, jint cache_switch)
{

    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Hcache((int32)file_id, (int)cache_switch)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetfileversion
 * Signature: (J[I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hgetfileversion(JNIEnv *env, jclass clss, jlong file_id, jintArray vers,
                                           jobjectArray fvstring)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(LIBVSTR_LEN + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Hgetfileversion: failed to allocate data buffer");

    if (fvstring == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Hgetfileversion:  fvstring is NULL");

    PIN_INT_ARRAY(ENVONLY, vers, theArgs, &isCopy, "Hgetfileversion:  vers not pinned");

    if ((rval = Hgetfileversion((int32)file_id, (uint32 *)&(theArgs[0]), (uint32 *)&(theArgs[1]),
                                (uint32 *)&(theArgs[2]), data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[LIBVSTR_LEN] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, fvstring, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, vers, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetlibversion
 * Signature: ([I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hgetlibversion(JNIEnv *env, jclass clss, jintArray vers, jobjectArray lvstring)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(LIBVSTR_LEN + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Hgetlibversion: failed to allocate data buffer");

    if (lvstring == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Hgetlibversion:  lvstring is NULL");

    if (vers == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Hgetlibversion:  vers is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, vers) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Hgetlibversion:  vers input array < order 3");

    PIN_INT_ARRAY(ENVONLY, vers, theArgs, &isCopy, "Hgetlibversion:  vers not pinned");

    if ((rval = Hgetlibversion((uint32 *)&(theArgs[0]), (uint32 *)&(theArgs[1]), (uint32 *)&(theArgs[2]),
                               data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);
    fprintf(stderr, "theArgs= %d:%d:%d\n", theArgs[0], theArgs[1], theArgs[2]);
    fprintf(stderr, "data= %s\n", data);
    data[LIBVSTR_LEN] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, lvstring, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, vers, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsetaccesstype
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hsetaccesstype(JNIEnv *env, jclass clss, jlong h_id, jint access_type)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Hsetaccesstype((int32)h_id, (unsigned)access_type)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsync
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hsync(JNIEnv *env, jclass clss, jlong file_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Hsync((int32)file_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HDFclose(JNIEnv *env, jclass clss, jlong file_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Hclose((int32)file_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFopen
 * Signature: (Ljava/lang/String;IS)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HDFopen(JNIEnv *env, jclass clss, jstring filename, jint access, jshort n_dds)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "HDFopen:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, str, NULL, "HDFopen:  filename is not pinned");
    if ((rval = HDFopen((char *)str, (int)access, (int16)n_dds)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, filename, str);

    return rval;
}

#ifdef not_yet_implemented
/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFflusdd
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HDFflusdd(JNIEnv *env, jclass clss, jlong file_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Hflushdd((int32)file_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}
#endif

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDgetNTdesc
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_hdf_hdflib_HDFLibrary_HDgetNTdesc(JNIEnv *env, jclass clss, jint nt)
{
    char   *rval    = NULL;
    jstring rstring = NULL;

    UNUSED(clss);

    if ((rval = HDgetNTdesc((int32)nt)) == NULL)
        H4_LIBRARY_ERROR(ENVONLY);

    rstring = ENVPTR->NewStringUTF(ENVONLY, rval);

done:
    free(rval);

    return rstring;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
