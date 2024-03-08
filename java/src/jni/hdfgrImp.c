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
#include "hdfgrImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern jboolean makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf);
extern jboolean getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);
extern jboolean setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf);
extern jboolean getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf);

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRstart(JNIEnv *env, jclass clss, jlong file_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRstart((int32)file_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRend(JNIEnv *env, jclass clss, jlong gr_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRend((int32)gr_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRendaccess(JNIEnv *env, jclass clss, jlong gr_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRendaccess((int32)gr_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRfileinfo(JNIEnv *env, jclass clss, jlong gr_id, jintArray argv)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy; /*  dummy */

    UNUSED(clss);

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRfileinfo:  argv is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRfileinfo:  argv input array < order 2");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "GRfileinfo:  argv not pinned");

    if ((rval = GRfileinfo((int32)gr_id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval = FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRselect(JNIEnv *env, jclass clss, jlong gr_id, jint index)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = GRselect((int32)gr_id, (int32)index)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRnametoindex(JNIEnv *env, jclass clss, jlong gr_id, jstring gr_name)
{
    int32       rval = -1;
    const char *str  = NULL;

    UNUSED(clss);

    if (gr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRnametoindex:  gr_name is null");

    PIN_JAVA_STRING(ENVONLY, gr_name, str, NULL, "GRnametoindex:  gr_name not pinned");

    if ((rval = GRnametoindex((int32)gr_id, str)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, gr_name, str);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetchunkinfo(JNIEnv *env, jclass clss, jlong grsid, jobject chunk_def,
                                          jintArray cflags)
{
    int32         rval = FAIL;
    HDF_CHUNK_DEF cdef;
    jboolean      stat = JNI_FALSE;
    jint         *flgs = NULL;
    jboolean      isCopy;

    UNUSED(clss);

    if (cflags == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetchunkinfo:  cflags is NULL");

    PIN_INT_ARRAY(ENVONLY, cflags, flgs, &isCopy, "GRgetchunkinfo:  cflags not pinned");

    if ((rval = GRgetchunkinfo((int32)grsid, &cdef, (int32 *)&(flgs[0]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    stat = JNI_TRUE;
    if (*flgs != 0) {
        /* convert cdef to HDFchunkinfo */
        stat = makeChunkInfo(env, chunk_def, *flgs, &cdef);
    }

done:
    if (flgs)
        UNPIN_INT_ARRAY(ENVONLY, cflags, flgs, (rval = FAIL) ? JNI_ABORT : 0);

    return stat /*JNI_TRUE*/;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetiminfo(JNIEnv *env, jclass clss, jlong ri_id, jobjectArray gr_name,
                                       jintArray argv, jintArray dim_sizes)
{
    int      rval    = FAIL;
    jint    *dims    = NULL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(H4_MAX_GR_NAME + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "GRgetiminfo: failed to allocate data buffer");

    if (gr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetiminfo:  gr_name is NULL");

    if (dim_sizes == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetiminfo:  dim_sizes is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, dim_sizes) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRgetiminfo:  dim_sizes input array < order 2");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetiminfo:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 4)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRgetiminfo:  argv input array < order 4");

    PIN_INT_ARRAY(ENVONLY, dim_sizes, dims, &isCopy, "GRgetiminfo:  dim_sizes not pinned");
    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "GRgetiminfo:  argv not pinned");

    if ((rval = GRgetiminfo((int32)ri_id, (char *)data, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                            (int32 *)&(theArgs[2]), (int32 *)dims, (int32 *)&(theArgs[3]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[H4_MAX_GR_NAME] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, gr_name, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval = FAIL) ? JNI_ABORT : 0);
    if (dims)
        UNPIN_INT_ARRAY(ENVONLY, dim_sizes, dims, (rval = FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadimage(JNIEnv *env, jclass clss, jlong ri_id, jintArray start,
                                       jintArray stride, jintArray edge, jbyteArray data)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jint    *strt = NULL;
    jint    *strd = NULL;
    jint    *edg  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadimage:  data is NULL");
    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadimage:  start is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, start) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRreadimage:  start input array < order 2");
    if (edge == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadimage:  edge is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, edge) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRreadimage:  edge input array < order 2");

    PIN_BYTE_ARRAY(ENVONLY, data, arr, &isCopy, "GRreadimage:  data not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "GRreadimage:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, edge, edg, &isCopy, "GRreadimage:  edge not pinned");

    if (stride == NULL)
        strd = NULL;
    else
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "GRreadimage:  stride not pinned");

    if ((rval = GRreadimage((int32)ri_id, (int32 *)strt, (int32 *)strd, (int32 *)edg, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (edg)
        UNPIN_INT_ARRAY(ENVONLY, edge, edg, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, data, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRidtoref(JNIEnv *env, jclass clss, jlong gr_id)
{
    uint16 rval = -1;

    UNUSED(clss);

    if ((rval = GRidtoref((int32)gr_id)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jshort)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRreftoindex(JNIEnv *env, jclass clss, jlong gr_id, jshort ref)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = GRreftoindex((int32)gr_id, (uint16)ref)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqlutil(JNIEnv *env, jclass clss, jlong gr_id, jint interlace_mode)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRreqlutil((int32)gr_id, (int)interlace_mode)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqimageil(JNIEnv *env, jclass clss, jlong gr_id, jint interlace_mode)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRreqimageil((int32)gr_id, (int)interlace_mode)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutid(JNIEnv *env, jclass clss, jlong gr_id, jint index)
{
    int32 rval = -1;

    UNUSED(clss);

    if ((rval = GRgetlutid((int32)gr_id, (int32)index)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetnluts(JNIEnv *env, jclass clss, jlong gr_id)
{
    int rval = -1;

    UNUSED(clss);

    if ((rval = GRgetnluts((int32)gr_id)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutinfo(JNIEnv *env, jclass clss, jlong ri_id, jintArray argv)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetlutinfo:  argv is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 4)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRgetlutinfo:  argv input array < order 4");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "GRgetlutinfo:  argv not pinned");

    if ((rval = GRgetlutinfo((int32)ri_id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                             (int32 *)&(theArgs[2]), (int32 *)&(theArgs[3]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, argv, theArgs, (rval = FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadlut(JNIEnv *env, jclass clss, jlong pal_id, jbyteArray pal_data)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (pal_data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadlut:  pal_data is NULL");

    PIN_BYTE_ARRAY(ENVONLY, pal_data, arr, &isCopy, "GRreadlut:  pal_data not pinned");

    if ((rval = GRreadlut((int32)pal_id, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, pal_data, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRattrinfo(JNIEnv *env, jclass clss, jlong gr_id, jint attr_index,
                                      jobjectArray name, jintArray argv)
{
    int32    rval    = FAIL;
    char    *data    = NULL;
    jint    *theArgs = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    /* check for out of memory error ... */
    if ((data = (char *)malloc(H4_MAX_GR_NAME + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "GRattrinfo: failed to allocate data buffer");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRattrinfo:  name is NULL");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRattrinfo:  argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRattrinfo:  argv input array < order 2");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "GRattrinfo:  argv not pinned");

    if ((rval = GRattrinfo((int32)gr_id, (int32)attr_index, (char *)data, (int32 *)&(theArgs[0]),
                           (int32 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[H4_MAX_GR_NAME] = '\0';
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
Java_hdf_hdflib_HDFLibrary_GRgetattr(JNIEnv *env, jclass clss, jlong gr_id, jint attr_index,
                                     jbyteArray values)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetattr:  values is NULL");

    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "GRgetattr:  values not pinned");

    if ((rval = GRgetattr((int32)gr_id, (int32)attr_index, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRfindattr(JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name)
{
    int32       rval = -1;
    const char *str  = NULL;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetattr:  attr_name is null");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "GRsetattr:  attr_name not pinned");

    if ((rval = GRfindattr((int32)gr_id, str)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return (jint)rval;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRcreate(JNIEnv *env, jclass clss, jlong gr_id, jstring name, jint ncomp,
                                    jlong data_type, jint interlace_mode, jintArray dim_sizes)
{
    int32       rval = -1;
    jint       *dims = NULL;
    const char *str  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRcreate:  name is null");

    if (dim_sizes == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRcreate:  dim_sizes is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, dim_sizes) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRcreate:  dim_sizes input array < order 2");

    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "GRcreate:  name not pinned");
    PIN_INT_ARRAY(ENVONLY, dim_sizes, dims, &isCopy, "GRcreate:  dim_sizes not pinned");

    if ((rval = GRcreate((int32)gr_id, str, (int32)ncomp, (int32)data_type, (int32)interlace_mode,
                         (int32 *)dims)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dims)
        UNPIN_INT_ARRAY(ENVONLY, dim_sizes, dims, (rval < 0) ? JNI_ABORT : 0);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);

    return (jlong)rval;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRluttoref(JNIEnv *env, jclass clss, jlong pal_id)
{
    uint16 rval = -1;

    UNUSED(clss);

    if ((rval = GRluttoref((int32)pal_id)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jshort)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JILjava_lang_String_2(
    JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values)
{
    int         rval = FAIL;
    const char *str  = NULL;
    const char *val  = NULL;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetattr:  attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetattr:  values is null");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "GRsetattr:  attr_name not pinned");
    PIN_JAVA_STRING(ENVONLY, values, val, NULL, "GRsetattr:  values not pinned");

    if ((rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (void *)val)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (val)
        UNPIN_JAVA_STRING(ENVONLY, values, val);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JI_3B(JNIEnv *env, jclass clss, jlong gr_id,
                                                                jstring attr_name, jlong data_type,
                                                                jint count, jbyteArray values)
{
    int         rval = FAIL;
    jbyte      *arr  = NULL;
    const char *str  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetattr:  attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetattr:  values is NULL");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "GRsetattr:  attr_name not pinned");
    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "GRsetattr:  values not pinned");

    if ((rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetcompress(JNIEnv *env, jclass clss, jlong ri_id, jint comp_type,
                                         jobject c_info)
{
    int       rval = FAIL;
    comp_info cinf;

    UNUSED(clss);

    if (c_info == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetcompress:  c_info is NULL");

    if (!getNewCompInfo(env, c_info, &cinf))
        H4_JNI_FATAL_ERROR(ENVONLY, "GRsetcompress:  c_info not initialized");

    if ((rval = GRsetcompress((int32)ri_id, (comp_coder_t)comp_type, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetcompress(JNIEnv *env, jclass clss, jlong ri_id, jobject c_info)
{
    int          rval = FAIL;
    comp_coder_t coder;
    comp_info    cinf;

    UNUSED(clss);

    if (c_info == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetcompress:  c_info is NULL");

    if ((rval = GRgetcompress((int32)ri_id, (comp_coder_t *)&coder, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (!setNewCompInfo(env, c_info, coder, &cinf))
        H4_JNI_FATAL_ERROR(ENVONLY, "GRgetcompress:  c_info not created");

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetcompinfo(JNIEnv *env, jclass clss, jlong ri_id, jobject c_info)
{
    int          rval = FAIL;
    comp_coder_t coder;
    comp_info    cinf;

    UNUSED(clss);

    if (c_info == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRgetcompinfo:  c_info is NULL");

    if ((rval = GRgetcompinfo((int32)ri_id, (comp_coder_t *)&coder, (comp_info *)&cinf)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    if (!setNewCompInfo(env, c_info, coder, &cinf))
        H4_JNI_FATAL_ERROR(ENVONLY, "GRgetcompinfo:  c_info not created");

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunk(JNIEnv *env, jclass clss, jlong sdsid, jobject chunk_def, jint flags)
{
    int           rval = FAIL;
    HDF_CHUNK_DEF c_def;

    UNUSED(clss);

    if (chunk_def == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetchunk:  chunk_def is NULL");

    if (!getChunkInfo(env, chunk_def, &c_def))
        H4_JNI_FATAL_ERROR(ENVONLY, "GRsetchunk:  chunk_def not initialized");

    if ((rval = GRsetchunk((int32)sdsid, c_def, (int32)flags)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunkcache(JNIEnv *env, jclass clss, jlong sdsid, jint maxcache, jint flags)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = GRsetchunkcache((int32)sdsid, (int32)maxcache, (int32)flags)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetexternalfile(JNIEnv *env, jclass clss, jlong ri_id, jstring filename,
                                             jint offset)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRsetexternalfile:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, str, NULL, "GRsetexternalfile:  filename not pinned");

    if ((rval = GRsetexternalfile((int32)ri_id, str, (int32)offset)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, filename, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwriteimage(JNIEnv *env, jclass clss, jlong ri_id, jintArray start,
                                        jintArray stride, jintArray edge, jbyteArray data)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jint    *strt = NULL;
    jint    *strd = NULL;
    jint    *edg  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwriteimage:  data is NULL");
    if (start == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwriteimage:  start is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, start) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRwriteimage:  start input array < order 2");
    if (edge == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwriteimage:  edge is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, edge) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRwriteimage:  edge input array < order 2");

    PIN_BYTE_ARRAY(ENVONLY, data, arr, &isCopy, "GRwriteimage:  dat not pinned");
    PIN_INT_ARRAY(ENVONLY, start, strt, &isCopy, "GRwriteimage:  start not pinned");
    PIN_INT_ARRAY(ENVONLY, edge, edg, &isCopy, "GRwriteimage:  edge not pinned");

    if (stride == NULL)
        strd = NULL;
    else
        PIN_INT_ARRAY(ENVONLY, stride, strd, &isCopy, "GRwriteimage:  stride not pinned");

    if ((rval = GRwriteimage((int32)ri_id, (int32 *)strt, (int32 *)strd, (int32 *)edg, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (strd)
        UNPIN_INT_ARRAY(ENVONLY, stride, strd, (rval == FAIL) ? JNI_ABORT : 0);
    if (edg)
        UNPIN_INT_ARRAY(ENVONLY, edge, edg, (rval == FAIL) ? JNI_ABORT : 0);
    if (strt)
        UNPIN_INT_ARRAY(ENVONLY, start, strt, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, data, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritelut(JNIEnv *env, jclass clss, jlong pal_id, jint ncomp, jint data_type,
                                      jint interlace, jint num_entries, jbyteArray pal_data)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (pal_data == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwritelut:  pal_data is NULL");

    PIN_BYTE_ARRAY(ENVONLY, pal_data, arr, &isCopy, "GRwritelut:  pal_data not pinned");

    if ((rval = GRwritelut((int32)pal_id, (int32)ncomp, (int32)data_type, (int32)interlace,
                           (int32)num_entries, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, pal_data, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadchunk(JNIEnv *env, jclass clss, jlong grid, jintArray origin, jbyteArray dat)
{
    int32    rval = FAIL;
    jbyte   *arr  = NULL;
    jint    *org  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (dat == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadchunk:  dat is NULL");
    if (origin == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRreadchunk:  origin is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, origin) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRreadchunk:  origin input array < order 2");

    PIN_BYTE_ARRAY(ENVONLY, dat, arr, &isCopy, "GRreadchunk:  dat not pinned");
    PIN_INT_ARRAY(ENVONLY, origin, org, &isCopy, "GRreadchunk:  origin not pinned");

    if ((rval = GRreadchunk((int32)grid, (int32 *)org, arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (org)
        UNPIN_INT_ARRAY(ENVONLY, origin, org, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, dat, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritechunk(JNIEnv *env, jclass clss, jlong grid, jintArray origin,
                                        jbyteArray dat)
{
    int32    rval = FAIL;
    jbyte   *arr  = NULL;
    jint    *org  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (dat == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwritechunk:  dat is NULL");
    if (origin == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "GRwritechunk:  origin is NULL");
    if (ENVPTR->GetArrayLength(ENVONLY, origin) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "GRwritechunk:  origin input array < order 2");

    PIN_BYTE_ARRAY(ENVONLY, dat, arr, &isCopy, "GRwritechunk:  dat not pinned");
    PIN_INT_ARRAY(ENVONLY, origin, org, &isCopy, "GRwritechunk:  origin not pinned");

    if ((rval = GRwritechunk((int32)grid, (int32 *)org, (char *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (org)
        UNPIN_INT_ARRAY(ENVONLY, origin, org, (rval == FAIL) ? JNI_ABORT : 0);
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, dat, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
