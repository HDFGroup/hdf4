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

#include <stdlib.h>
#include <string.h>

#include <jni.h>

#include "hdf.h"
#include "h4jni.h"
#include "hdfanImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANstart(JNIEnv *env, jclass clss, jlong file_id)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANstart((int32)file_id)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANend(JNIEnv *env, jclass clss, jlong anid)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = ANend((int32)anid)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANfileinfo(JNIEnv *env, jclass clss, jlong anid, jintArray info)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (info == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANfileinfo:  info is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, info) < 4)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "ANfileinfo:  info input array < order 4");

    PIN_INT_ARRAY(ENVONLY, info, theArgs, &isCopy, "ANfileinfo: info input array not pinned");

    if ((rval = ANfileinfo((int32)anid, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                           (int32 *)&(theArgs[2]), (int32 *)&(theArgs[3]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, info, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANselect(JNIEnv *env, jclass clss, jlong anid, jint index, jint anntype)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANselect((int32)anid, (int32)index, (ann_type)anntype)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANendaccess(JNIEnv *env, jclass clss, jlong ann_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = ANendaccess((int32)ann_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANnumann(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag,
                                    jshort ref)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANnumann((int32)an_id, (ann_type)anntype, (uint16)tag, (uint16)ref)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_ANatype2tag(JNIEnv *env, jclass clss, jint antype)
{
    uint16 rval = -1;

    UNUSED(clss);

    if ((rval = ANatype2tag((ann_type)antype)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jshort)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANtag2atype(JNIEnv *env, jclass clss, jint antag)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANtag2atype((uint16)antag)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlist(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag,
                                     jshort ref, jintArray annlist)
{
    int      rval = FAIL;
    jint    *iarr = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (annlist == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANannlist:  annlist is NULL");

    PIN_INT_ARRAY(ENVONLY, annlist, iarr, &isCopy, "ANannlist:  annlist not pinned");

    if ((rval = ANannlist((int32)an_id, (ann_type)anntype, (uint16)tag, (uint16)ref, (int32 *)iarr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (iarr)
        UNPIN_INT_ARRAY(ENVONLY, annlist, iarr, (rval == FAIL) ? JNI_ABORT : 0);

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlen(JNIEnv *env, jclass clss, jlong ann_id)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANannlen((int32)ann_id)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANreadann(JNIEnv *env, jclass clss, jlong ann_id, jobjectArray annbuf, jint maxlen)
{
    int32   rval = FAIL;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if (NULL == (data = (char *)malloc(sizeof(char) * (size_t)maxlen + 1)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "ANreadan: failed to allocate data buffer");

    if (annbuf == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANreadan:  annbuf is NULL");

    /* read annotation from HDF */
    if ((rval = ANreadann((int32)ann_id, data, (int32)maxlen)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[maxlen] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, annbuf, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreate(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref, jint type)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANcreate((int32)an_id, (uint16)tag, (uint16)ref, (ann_type)type)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreatef(JNIEnv *env, jclass clss, jlong an_id, jint type)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANcreatef((int32)an_id, (ann_type)type)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANget_1tagref(JNIEnv *env, jclass clss, jlong an_id, jint index, jint type,
                                         jshortArray tagref)
{
    int32    rval    = FAIL;
    short   *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (tagref == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANget_tagref:  tagref is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, tagref) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "ANget_tagref:  tagref input array < order 2");

    PIN_SHORT_ARRAY(ENVONLY, tagref, theArgs, &isCopy, "ANget_tagref:  tagref not pinned");

    if ((rval = ANget_tagref((int32)an_id, (int32)index, (ann_type)type, (uint16 *)&(theArgs[0]),
                             (uint16 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_SHORT_ARRAY(ENVONLY, tagref, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANid2tagref(JNIEnv *env, jclass clss, jlong an_id, jshortArray tagref)
{
    int32    rval    = FAIL;
    short   *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (tagref == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANid2tagref:  tagref is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, tagref) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "ANid2tagref:  tagref input array < order 2");

    PIN_SHORT_ARRAY(ENVONLY, tagref, theArgs, &isCopy, "ANid2tagref:  tagref not pinned");

    if ((rval = ANid2tagref((int32)an_id, (uint16 *)&(theArgs[0]), (uint16 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_SHORT_ARRAY(ENVONLY, tagref, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANtagref2id(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = ANtagref2id((int32)an_id, (uint16)tag, (uint16)ref)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANwriteann(JNIEnv *env, jclass clss, jlong ann_id, jstring label, jint ann_length)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (label == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "ANwriteann: label is NULL");

    PIN_JAVA_STRING(ENVONLY, label, str, NULL, "ANwriteann:  label not pinned");

    if ((rval = ANwriteann((int32)ann_id, str, (int32)ann_length)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, label, str);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
