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
#include "mfhdf.h"
#include "h4jni.h"
#include "hdfsdsImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern jboolean makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf);
extern jboolean getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);
extern jboolean setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf);
extern jboolean getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf);

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDstart(JNIEnv *env, jclass clss, jstring filename, jint access)
{
    int32       sdid = -1;
    const char *fstr = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDstart:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "SDstart:  filename not pinned");

    if ((sdid = SDstart(fstr, (int32)access)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return (jlong)sdid;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDend(JNIEnv *env, jclass clss, jlong sdid)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdid;

    UNUSED(clss);

    if ((rval = SDend(id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDfileinfo(JNIEnv *env, jclass clss, jlong sdid, jintArray argv)
{
    intn     rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy;
    int32    id = (int32)sdid;

    UNUSED(clss);

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDfileinfo:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDfileinfo:  argv input array < order 2");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "SDfileinfo:  argv not pinned");

    if ((rval = SDfileinfo(id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDselect(JNIEnv *env, jclass clss, jlong sdid, jint index)
{
    int32 rval = -1;
    int32 id   = (int32)sdid;

    UNUSED(clss);

    if ((rval = (jlong)SDselect(id, (int32)index)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDnametoindex(JNIEnv *env, jclass clss, jlong sdid, jstring name)
{
    intn        rval = -1;
    const char *str  = NULL;
    int32       id   = (int32)sdid;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDnametoindex:  name is null");

    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "SDnametoindex:  name not pinned");

    if ((rval = SDnametoindex(id, str)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetinfo(JNIEnv *env, jclass clss, jlong sdsid, jobjectArray name,
                                     jintArray dimsizes, jintArray argv)
{
    intn     rval    = FAIL;
    char    *cname   = NULL;
    int32   *dims    = NULL;
    int32   *theArgs = NULL;
    jstring  rstring;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if ((cname = (char *)malloc(H4_MAX_NC_NAME + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetinfo: failed to allocate data buffer");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetinfo:  name is NULL");

    if (dimsizes == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetinfo:  dimsizes is NULL");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetinfo:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDgetinfo:  argv input array < order 3");

    PIN_INT_ARRAY(ENVONLY, dimsizes, dims, &isCopy, "SDgetinfo:  dimsizes not pinned");
    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "SDgetinfo:  argv not pinned");

    if ((rval = SDgetinfo(id, cname, &(theArgs[0]), dims, &(theArgs[1]), &(theArgs[2]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    cname[H4_MAX_NC_NAME] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, cname)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, name, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

done:
    free(cname);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);
    if (dims)
        UNPIN_INT_ARRAY(ENVONLY, dimsizes, dims, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                      jintArray stride, jintArray count, jbyteArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jbyte   *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_BYTE_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_BYTE_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

/*
    ////////////////////////////////////////////////////////////////////
    //                                                                //
    //         New APIs for read data from library                    //
    //  Using SDreaddata(..., Object buf) requires function calls     //
    //  theArray.emptyBytes() and theArray.arrayify(buf), which       //
    //  triples the actual memory needed by the data set.             //
    //  Using the following APIs solves the problem.                  //
    //                                                                //
    ////////////////////////////////////////////////////////////////////
*/

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1short(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                             jintArray stride, jintArray count, jshortArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jshort  *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    else if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    else if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_SHORT_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_SHORT_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1int(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                           jintArray stride, jintArray count, jintArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jint    *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_INT_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_INT_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1long(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                            jintArray stride, jintArray count, jlongArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jlong   *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_LONG_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_LONG_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1float(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                             jintArray stride, jintArray count, jfloatArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jfloat  *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_FLOAT_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_FLOAT_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1double(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                              jintArray stride, jintArray count, jdoubleArray data)
{
    intn     rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *cnt  = NULL;
    jdouble *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  start is NULL");

    if (count == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  count is NULL");

    PIN_DOUBLE_ARRAY(ENVONLY, data, d, &isCopy, "SDreaddata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDreaddata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, count, cnt, &isCopy, "SDreaddata:  count not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDreaddata:  stride not pinned");

    if ((rval = SDreaddata(id, strt, strd, cnt, d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (cnt)
        UNPIN_INT_ARRAY(ENVONLY, count, cnt, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_DOUBLE_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDendaccess(JNIEnv *env, jclass clss, jlong sdsid)
{
    intn  rval = -1;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDendaccess(id)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimid(JNIEnv *env, jclass clss, jlong sdsid, jint index)
{
    int32 rval = -1;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDgetdimid(id, (intn)index)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDdiminfo(JNIEnv *env, jclass clss, jlong dimid, jobjectArray dimname,
                                     jintArray argv)
{
    intn     rval    = FAIL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(256)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDdiminfo: failed to allocate data buffer");

    if (dimname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDdiminfo:  dimname is NULL");

    else if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDdiminfo:  argv is NULL");

    else if (ENVPTR->GetArrayLength(ENVONLY, argv) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDdiminfo:  argv input array < order 3");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "SDdiminfo:  argv not pinned");

    if ((rval = SDdiminfo((int32)dimid, (char *)data, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                          (int32 *)&(theArgs[2]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[255] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, dimname, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDidtoref(JNIEnv *env, jclass clss, jlong sdsid)
{
    int32 rval = -1;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDidtoref(id)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDreftoindex(JNIEnv *env, jclass clss, jlong sdid, jint ref)
{
    int32 rval = -1;
    int32 id   = (int32)sdid;

    UNUSED(clss);

    if ((rval = SDreftoindex(id, (int32)ref)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDattrinfo(JNIEnv *env, jclass clss, jlong sdsid, jint index, jobjectArray name,
                                      jintArray argv)
{
    intn     rval    = FAIL;
    char    *data    = NULL;
    jint    *theArgs = NULL;
    jstring  rstring;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if ((data = (char *)malloc(256)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDattrinfo: failed to allocate data buffer");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDattrinfo:  name is NULL");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDattrinfo:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDattrinfo:  argv input array < order 2");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "SDattrinfo:  argv not pinned");

    if ((rval = SDattrinfo(id, (int32)index, (char *)data, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]))) ==
        FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[255] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, name, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreadattr(JNIEnv *env, jclass clss, jlong sdsid, jint index, jbyteArray dat)
{
    intn     rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (dat == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreadattr:  dat is NULL");

    PIN_BYTE_ARRAY(ENVONLY, dat, arr, &isCopy, "SDreadattr:  dat not pinned");

    if ((rval = SDreadattr(id, (int32)index, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, dat, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDfindattr(JNIEnv *env, jclass clss, jlong sdsid, jstring name)
{
    int32       rval = -1;
    const char *str  = NULL;
    int32       id   = (int32)sdsid;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDfindattr:  name is null");

    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "SDfindattr:  name not pinned");

    if ((rval = SDfindattr(id, str)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDiscoordvar(JNIEnv *env, jclass clss, jlong sdsid)
{
    intn  rval = -1;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDiscoordvar(id)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcal(JNIEnv *env, jclass clss, jlong sdsid, jdoubleArray argv, jintArray nt)
{
    intn     rval    = FAIL;
    jdouble *theArgs = NULL;
    jint    *theNT   = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (nt == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetcal:  nt is NULL");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetcal:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 4)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDgetcal:  argv input array < order 4");

    PIN_INT_ARRAY(ENVONLY, nt, theNT, &isCopy, "SDgetcal:  nt not pinned");
    PIN_DOUBLE_ARRAY(ENVONLY, argv, theArgs, &isCopy, "SDgetcal:  argv not pinned");

    if ((rval = SDgetcal(id, (float64 *)&(theArgs[0]), (float64 *)&(theArgs[1]), (float64 *)&(theArgs[2]),
                         (float64 *)&(theArgs[3]), (int32 *)&(theNT[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_DOUBLE_ARRAY(ENVONLY, argv, theArgs, (rval == FAIL) ? JNI_ABORT : 0);
    if (theNT)
        UNPIN_INT_ARRAY(ENVONLY, nt, theNT, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdatastrs(JNIEnv *env, jclass clss, jlong sdsid, jobjectArray strings,
                                         jint len)
{
    intn    rval        = FAIL;
    char   *labVal      = NULL;
    char   *unitVal     = NULL;
    char   *fmtVal      = NULL;
    char   *coordsysVal = NULL;
    jstring str;
    jstring label, unit, format, coordsys;
    int32   id = (int32)sdsid;

    UNUSED(clss);

    if (strings == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetdatastrs:  strings is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, strings) < 4)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDgetdatastrs:  strings input array < order 4");

    if ((label = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 0)) != NULL) {
        /* allocate space */
        if ((labVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdatastrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, label);

    if ((unit = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 1)) != NULL) {
        if ((unitVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdatastrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, unit);

    if ((format = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 2)) != NULL) {
        if ((fmtVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdatastrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, format);

    if ((coordsys = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 3)) != NULL) {
        if ((coordsysVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdatastrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, coordsys);

    if ((rval = SDgetdatastrs(id, labVal, unitVal, fmtVal, coordsysVal, (int32)len)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (labVal != NULL) {
        labVal[len] = '\0';
        str         = ENVPTR->NewStringUTF(ENVONLY, labVal);
        if (str != NULL) {
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 0, (jobject)str);
        }
    }
    if (unitVal != NULL) {
        unitVal[len] = '\0';
        str          = ENVPTR->NewStringUTF(ENVONLY, unitVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 1, (jobject)str);
    }
    if (fmtVal != NULL) {
        fmtVal[len] = '\0';
        str         = ENVPTR->NewStringUTF(ENVONLY, fmtVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 2, (jobject)str);
    }
    if (coordsysVal != NULL) {
        coordsysVal[len] = '\0';
        str              = ENVPTR->NewStringUTF(ENVONLY, coordsysVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 3, (jobject)str);
    }

done:
    free(labVal);
    free(unitVal);
    free(fmtVal);
    free(coordsysVal);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimstrs(JNIEnv *env, jclass clss, jlong dimid, jobjectArray strings, jint len)
{
    intn    rval    = FAIL;
    char   *labVal  = NULL;
    char   *unitVal = NULL;
    char   *fmtVal  = NULL;
    jstring str;
    jstring label, unit, format;

    UNUSED(clss);

    if (strings == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetdimstrs:  strings is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, strings) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDgetdimstrs:  strings input array < order 3");

    if ((label = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 0)) != NULL) {
        /* allocate space */
        if ((labVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdimstrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, label);

    if ((unit = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 1)) != NULL) {
        if ((unitVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdimstrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, unit);

    if ((format = (jstring)ENVPTR->GetObjectArrayElement(ENVONLY, strings, 2)) != NULL) {
        if ((fmtVal = (char *)malloc(len + 1)) == NULL)
            H4_OUT_OF_MEMORY_ERROR(ENVONLY, "SDgetdimstrs");
    }
    ENVPTR->DeleteLocalRef(ENVONLY, format);

    if ((rval = SDgetdimstrs((int32)dimid, labVal, unitVal, fmtVal, (int32)len)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (labVal != NULL) {
        labVal[len] = '\0';
        str         = ENVPTR->NewStringUTF(ENVONLY, labVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 0, (jobject)str);
    }
    if (unitVal != NULL) {
        unitVal[len] = '\0';
        str          = ENVPTR->NewStringUTF(ENVONLY, unitVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 1, (jobject)str);
    }
    if (fmtVal != NULL) {
        fmtVal[len] = '\0';
        str         = ENVPTR->NewStringUTF(ENVONLY, fmtVal);
        if (str != NULL)
            ENVPTR->SetObjectArrayElement(ENVONLY, strings, 2, (jobject)str);
    }

done:
    free(labVal);
    free(unitVal);
    free(fmtVal);

    return JNI_TRUE;
}

/*** note this is returning data of many types in an array of bytes.... */
/* not sure how well this will work for java programs .... */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimscale(JNIEnv *env, jclass clss, jlong dimid, jbyteArray data)
{
    intn     rval   = FAIL;
    jbyte   *datVal = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetdimscale:  data is NULL");

    PIN_BYTE_ARRAY(ENVONLY, data, datVal, &isCopy, "SDgetdimscale:  data not pinned");

    if ((rval = SDgetdimscale((int32)dimid, (char *)datVal)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (datVal)
        UNPIN_BYTE_ARRAY(ENVONLY, data, datVal, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetfillvalue(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray data)
{
    intn     rval   = FAIL;
    jbyte   *datVal = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetfillvalue:  data is NULL");

    PIN_BYTE_ARRAY(ENVONLY, data, datVal, &isCopy, "SDgetfillvalue:  data not pinned");

    if ((rval = SDgetfillvalue(id, (char *)datVal)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (datVal)
        UNPIN_BYTE_ARRAY(ENVONLY, data, datVal, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetrange(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray max, jbyteArray min)
{
    intn     rval = FAIL;
    jbyte   *minp = NULL;
    jbyte   *maxp = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (max == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetrange:  max is NULL");

    if (min == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetrange:  min is NULL");

    PIN_BYTE_ARRAY(ENVONLY, max, maxp, &isCopy, "SDgetrange:  max not pinned");
    PIN_BYTE_ARRAY(ENVONLY, min, minp, &isCopy, "SDgetrange:  min not pinned");

    if ((rval = SDgetrange(id, (void *)maxp, (void *)minp)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (minp)
        UNPIN_BYTE_ARRAY(ENVONLY, min, minp, (rval == FAIL) ? JNI_ABORT : 0);
    if (maxp)
        UNPIN_BYTE_ARRAY(ENVONLY, max, maxp, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDcreate(JNIEnv *env, jclass clss, jlong sd_id, jstring name, jlong number_type,
                                    jint rank, jintArray dimsizes)
{
    int32       rval = -1;
    const char *str  = NULL;
    jint       *dims = NULL;
    jboolean    isCopy;
    int32       id = (int32)sd_id;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDcreate:  name is null");

    if (dimsizes == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDcreate:  dimsizes is NULL");

    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "SDcreate:  name not pinned");
    PIN_INT_ARRAY(ENVONLY, dimsizes, dims, &isCopy, "SDcreate:  dimsizes not pinned");

    if ((rval = SDcreate(id, str, (int32)number_type, (int32)rank, (int32 *)dims)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dims)
        UNPIN_INT_ARRAY(ENVONLY, dimsizes, dims, (rval == FAIL) ? JNI_ABORT : 0);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);

    return (jlong)rval;
}

// this API call returns false for not record and for failures
// maybe failures should return a negative?
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDisrecord(JNIEnv *env, jclass clss, jlong sdsid)
{
    int32 rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(env);
    UNUSED(clss);

    if ((rval = SDisrecord(id)) == TRUE)
        return JNI_TRUE;

    return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetattr(JNIEnv *env, jclass clss, jlong s_id, jstring attr_name, jlong num_type,
                                     jint count, jbyteArray values)
{
    intn        rval = FAIL;
    jbyte      *arr  = NULL;
    const char *str  = NULL;
    jboolean    isCopy;
    int32       id = (int32)s_id;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetattr:  attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetattr:  values is NULL");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "SDsetattr:  attr_name not pinned");
    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "SDsetattr:  values not pinned");

    if ((rval = SDsetattr(id, str, (int32)num_type, (int32)count, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetcal(JNIEnv *env, jclass clss, jlong sds_id, jdouble cal, jdouble cal_err,
                                    jdouble offset, jdouble offset_err, jint number_type)
{
    intn  rval = FAIL;
    int32 id   = (int32)sds_id;

    UNUSED(clss);

    if ((rval = SDsetcal(id, (float64)cal, (float64)cal_err, (float64)offset, (float64)offset_err,
                         (int32)number_type)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdatastrs(JNIEnv *env, jclass clss, jlong sds_id, jstring label, jstring unit,
                                         jstring format, jstring coordsys)
{
    intn        rval    = FAIL;
    const char *labstr  = NULL;
    const char *unstr   = NULL;
    const char *formstr = NULL;
    const char *csstr   = NULL;

    UNUSED(clss);

    if (label == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdatastrs:  label is null");

    if (unit == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdatastrs:  unit is null");

    if (format == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdatastrs:  format is null");

    if (coordsys == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdatastrs:  coordsys is null");

    PIN_JAVA_STRING(ENVONLY, label, labstr, NULL, "SDsetdatastrs:  label not pinned");
    PIN_JAVA_STRING(ENVONLY, unit, unstr, NULL, "SDsetdatastrs:  unit not pinned");
    PIN_JAVA_STRING(ENVONLY, format, formstr, NULL, "SDsetdatastrs:  format not pinned");
    PIN_JAVA_STRING(ENVONLY, coordsys, csstr, NULL, "SDsetdatastrs:  coordsys not pinned");

    if ((rval = SDsetdatastrs((int32)sds_id, labstr, unstr, formstr, csstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (csstr)
        UNPIN_JAVA_STRING(ENVONLY, coordsys, csstr);
    if (formstr)
        UNPIN_JAVA_STRING(ENVONLY, format, formstr);
    if (unstr)
        UNPIN_JAVA_STRING(ENVONLY, unit, unstr);
    if (labstr)
        UNPIN_JAVA_STRING(ENVONLY, label, labstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimname(JNIEnv *env, jclass clss, jlong dim_id, jstring dim_name)
{
    intn        rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (dim_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdimname:  dim_name is null");

    PIN_JAVA_STRING(ENVONLY, dim_name, str, NULL, "SDsetdimname:  dim_name not pinned");

    if ((rval = SDsetdimname((int32)dim_id, str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, dim_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimscale(JNIEnv *env, jclass clss, jlong dim_id, jint count, jint number_type,
                                         jbyteArray data)
{
    intn     rval = FAIL;
    jbyte   *d    = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdimscale:  data is NULL");

    PIN_BYTE_ARRAY(ENVONLY, data, d, &isCopy, "SDsetdimscale:  data not pinned");

    if ((rval = SDsetdimscale((int32)dim_id, (int32)count, (int32)number_type, (void *)d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (d)
        UNPIN_BYTE_ARRAY(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimstrs(JNIEnv *env, jclass clss, jlong dim_id, jstring label, jstring unit,
                                        jstring format)
{
    intn        rval    = FAIL;
    const char *labstr  = NULL;
    const char *unstr   = NULL;
    const char *formstr = NULL;

    UNUSED(clss);

    if (label == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdimstrs:  label is null");

    if (unit == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdimstrs:  unit is null");

    if (format == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetdimstrs:  format is null");

    PIN_JAVA_STRING(ENVONLY, label, labstr, NULL, "SDsetdimstrs:  label not pinned");
    PIN_JAVA_STRING(ENVONLY, unit, unstr, NULL, "SDsetdimstrs:  unit not pinned");
    PIN_JAVA_STRING(ENVONLY, format, formstr, NULL, "SDsetdimstrs:  format not pinned");

    if ((rval = SDsetdimstrs((int32)dim_id, labstr, unstr, formstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (formstr)
        UNPIN_JAVA_STRING(ENVONLY, format, formstr);
    if (unstr)
        UNPIN_JAVA_STRING(ENVONLY, unit, unstr);
    if (labstr)
        UNPIN_JAVA_STRING(ENVONLY, label, labstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetexternalfile(JNIEnv *env, jclass clss, jlong sds_id, jstring filename,
                                             jint offset)
{
    intn        rval = FAIL;
    const char *fstr = NULL;
    int32       id   = (int32)sds_id;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetexternalfile:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, fstr, NULL, "SDsetexternalfile:  filename not pinned");

    if ((rval = SDsetexternalfile(id, fstr, (int32)offset)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, filename, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetfillvalue(JNIEnv *env, jclass clss, jlong sds_id, jbyteArray fill_val)
{
    intn     rval = FAIL;
    jboolean isCopy;
    jbyte   *d  = NULL;
    int32    id = (int32)sds_id;

    UNUSED(clss);

    if (fill_val == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetfillvalue:  fill_val is NULL");

    PIN_BYTE_ARRAY(ENVONLY, fill_val, d, &isCopy, "SDsetfillvalue:  fill_val not pinned");

    if ((rval = SDsetfillvalue(id, (void *)d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (d)
        UNPIN_BYTE_ARRAY(ENVONLY, fill_val, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetrange(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray max, jbyteArray min)
{
    int32    rval = FAIL;
    jboolean isCopy;
    jbyte   *minp = NULL;
    jbyte   *maxp = NULL;
    int32    id   = (int32)sdsid;

    UNUSED(clss);

    if (max == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetrange:  max is NULL");

    if (min == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetrange:  min is NULL");

    PIN_BYTE_ARRAY(ENVONLY, max, maxp, &isCopy, "SDsetrange:  max not pinned");
    PIN_BYTE_ARRAY(ENVONLY, min, minp, &isCopy, "SDsetrange:  min not pinned");

    if ((rval = SDsetrange(id, maxp, minp)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (minp)
        UNPIN_BYTE_ARRAY(ENVONLY, min, minp, (rval == FAIL) ? JNI_ABORT : 0);
    if (maxp)
        UNPIN_BYTE_ARRAY(ENVONLY, max, maxp, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDwritedata(JNIEnv *env, jclass clss, jlong sdsid, jintArray start,
                                       jintArray stride, jintArray edge, jbyteArray data)
{
    int32    rval = FAIL;
    int32   *strt = NULL;
    int32   *strd = NULL;
    int32   *e    = NULL;
    jbyte   *d    = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (id < 0)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "SDreaddata:  sdsid is invalid");

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDwritedata:  data is NULL");

    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDwritedata:  start is NULL");

    if (edge == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDwritedata:  count is NULL");

    PIN_BYTE_ARRAY_CRITICAL(ENVONLY, data, d, &isCopy, "SDwritedata:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "SDwritedata:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, edge, e, &isCopy, "SDwritedata:  edge not pinned");

    if (stride != NULL)
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "SDwritedata:  stride not pinned");

    if ((rval = SDwritedata(id, strt, strd, e, (void *)d)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (e)
        UNPIN_INT_ARRAY(ENVONLY, edge, e, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (d)
        UNPIN_ARRAY_CRITICAL(ENVONLY, data, d, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

/* new stuff for chunking */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetnbitdataset(JNIEnv *env, jclass clss, jlong sdsid, jint start_bit,
                                            jint bit_len, jint sign_ext, jint fill_one)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetnbitdataset(id, (intn)start_bit, (intn)bit_len, (intn)sign_ext, (intn)fill_one)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetcompress(JNIEnv *env, jclass clss, jlong sdsid, jint type, jobject cinfo)
{
    intn      rval = FAIL;
    comp_info cinf;
    int32     id = (int32)sdsid;

    UNUSED(clss);

    if (cinfo == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetcompress:  cinfo is NULL");

    if (getNewCompInfo(env, cinfo, &cinf) == JNI_FALSE)
        H4_RAISE_EXCEPTION(ENVONLY, "SDsetcompress: error creating comp_info struct");

    if ((rval = SDsetcompress(id, (comp_coder_t)type, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcompinfo(JNIEnv *env, jclass clss, jlong sdsid, jobject cinfo)
{
    intn         rval = FAIL;
    comp_coder_t coder;
    comp_info    cinf;
    int32        id = (int32)sdsid;

    UNUSED(clss);

    if (cinfo == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetcompinfo:  cinfo is NULL");

    if ((rval = SDgetcompinfo(id, (comp_coder_t *)&coder, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (setNewCompInfo(env, cinfo, coder, &cinf) == JNI_FALSE)
        H4_RAISE_EXCEPTION(ENVONLY, "SDgetcompinfo: error creating comp_info struct");

done:
    return JNI_TRUE;
}

#ifndef H4_NO_DEPRECATED_SYMBOLS
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcompress(JNIEnv *env, jclass clss, jlong sdsid, jobject cinfo)
{
    intn         rval = FAIL;
    comp_coder_t coder;
    comp_info    cinf;
    int32        id = (int32)sdsid;

    UNUSED(clss);

    if (cinfo == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetcompress:  cinfo is NULL");

    if ((rval = SDgetcompress(id, (comp_coder_t *)&coder, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (setNewCompInfo(env, cinfo, coder, &cinf) == JNI_FALSE)
        H4_RAISE_EXCEPTION(ENVONLY, "SDgetcompress: error creating comp_info struct");

done:
    return JNI_TRUE;
}
#endif /* H4_NO_DEPRECATED_SYMBOLS */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetaccesstype(JNIEnv *env, jclass clss, jlong sdsid, jint accesstype)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetaccesstype(id, (uintn)accesstype)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetblocksize(JNIEnv *env, jclass clss, jlong sdsid, jint block_size)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetblocksize(id, (int32)block_size)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetfillmode(JNIEnv *env, jclass clss, jlong sdsid, jint fillmode)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetfillmode(id, (intn)fillmode)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimval_1comp(JNIEnv *env, jclass clss, jlong sdsid, jint comp_mode)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetdimval_comp(id, (intn)comp_mode)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDisdimval_1bwcomp(JNIEnv *env, jclass clss, jlong dimid)
{
    intn rval = FAIL;

    UNUSED(clss);

    if ((rval = SDisdimval_bwcomp((int32)dimid)) == SD_DIMVAL_BW_COMP)
        return JNI_TRUE;
    else if (rval == SD_DIMVAL_BW_INCOMP)
        return JNI_FALSE;
    else
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetchunk(JNIEnv *env, jclass clss, jlong sdsid, jobject chunk_def, jint flags)
{
    intn          rval = FAIL;
    HDF_CHUNK_DEF c_def;
    int32         id = (int32)sdsid;

    UNUSED(clss);

    if (chunk_def == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDsetchunk:  chunk_def is NULL");

    if (getChunkInfo(env, chunk_def, &c_def) == JNI_FALSE)
        H4_RAISE_EXCEPTION(ENVONLY, "SDsetchunk: error creating chunk_def struct");

    if ((rval = SDsetchunk(id, c_def, (int32)flags)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetchunkinfo(JNIEnv *env, jclass clss, jlong sdsid, jobject chunk_def,
                                          jintArray cflags)
{
    int32         rval = FAIL;
    HDF_CHUNK_DEF cdef;
    jint         *flgs = NULL;
    jboolean      isCopy;
    int32         id = (int32)sdsid;

    UNUSED(clss);

    if (chunk_def == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetchunkinfo:  chunk_def is NULL");

    if (cflags == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDgetchunkinfo:  cflags is NULL");

    PIN_INT_ARRAY(ENVONLY, cflags, flgs, &isCopy, "SDgetchunkinfo:  cflags not pinned");

    if ((rval = SDgetchunkinfo(id, &cdef, (int32 *)&(flgs[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    /* convert cdef to HDFchunkinfo */
    if (makeChunkInfo(env, chunk_def, (int32)*flgs, &cdef) == JNI_FALSE)
        H4_RAISE_EXCEPTION(ENVONLY, "SDgetchunkinfo: error creating chunk_def struct");

done:
    if (flgs)
        UNPIN_INT_ARRAY(ENVONLY, cflags, flgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreadchunk(JNIEnv *env, jclass clss, jlong sdid, jintArray origin, jbyteArray dat)
{
    int32    rval = FAIL;
    jbyte   *s    = NULL;
    jint    *arr  = NULL;
    jboolean isCopy;
    int32    id = (int32)sdid;

    UNUSED(clss);

    if (dat == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreadchunk:  data is NULL");

    if (origin == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDreadchunk:  origin is NULL");

    PIN_INT_ARRAY(ENVONLY, origin, arr, &isCopy, "SDreadchunk:  origin not pinned");

    PIN_BYTE_ARRAY(ENVONLY, dat, s, &isCopy, "SDreadchunk:  dat not pinned");

    if ((rval = SDreadchunk(id, (int32 *)arr, s)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (s)
        UNPIN_BYTE_ARRAY(ENVONLY, dat, s, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_INT_ARRAY(ENVONLY, origin, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetchunkcache(JNIEnv *env, jclass clss, jlong sdsid, jint maxcache, jint flags)
{
    intn  rval = FAIL;
    int32 id   = (int32)sdsid;

    UNUSED(clss);

    if ((rval = SDsetchunkcache(id, (int32)maxcache, (int32)flags)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDwritechunk(JNIEnv *env, jclass clss, jlong sdid, jintArray origin,
                                        jbyteArray dat)
{
    int32    rval = FAIL;
    jbyte   *s    = NULL;
    jint    *arr  = NULL;
    jboolean isCopy;
    int32    id = (int32)sdid;

    UNUSED(clss);

    if (dat == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDwritechunk:  dat is NULL");

    if (origin == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDwritechunk:  origin is NULL");

    PIN_INT_ARRAY(ENVONLY, origin, arr, &isCopy, "SDwritechunk:  origin not pinned");
    PIN_BYTE_ARRAY(ENVONLY, dat, s, &isCopy, "SDwritechunk:  dat not pinned");

    if ((rval = SDwritechunk(id, (int32 *)arr, s)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (s)
        UNPIN_BYTE_ARRAY(ENVONLY, dat, s, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_INT_ARRAY(ENVONLY, origin, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDcheckempty(JNIEnv *env, jclass clss, jlong sdsid, jintArray emptySDS)
{
    int32    rval    = FAIL;
    intn    *isempty = NULL;
    jboolean isCopy;
    int32    id = (int32)sdsid;

    UNUSED(clss);

    if (emptySDS == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "SDcheckempty:  emptySDS is NULL");

    PIN_INT_ARRAY(ENVONLY, emptySDS, isempty, &isCopy, "SDcheckempty:  emptySDS not pinned");

    if ((rval = SDcheckempty(id, isempty)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (isempty)
        UNPIN_INT_ARRAY(ENVONLY, emptySDS, isempty, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
