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
#include "hdfvdataImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VSattach(JNIEnv *env, jclass clss, jlong fid, jint vdata_ref, jstring accessmode)
{
    int32       rval   = -1;
    const char *access = NULL;
    HFILEID     id     = (HFILEID)fid;

    UNUSED(clss);

    if (accessmode == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSattach:  accessmode is null");

    PIN_JAVA_STRING(ENVONLY, accessmode, access, NULL, "VSattach:  accessmode not pinned");

    /* open HDF file specified by hdf_HDF_file */
    if ((rval = VSattach(id, (int32)vdata_ref, access)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (access)
        UNPIN_JAVA_STRING(ENVONLY, accessmode, access);

    return (jlong)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSdetach(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSdetach((int32)vdata_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetid(JNIEnv *env, jclass clss, jlong fid, jint vdata_ref)
{
    int32   rval = FAIL;
    HFILEID id   = (HFILEID)fid;

    UNUSED(clss);

    if ((rval = VSgetid(id, (int32)vdata_ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetclass(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray hdfclass)
{
    int32   rval      = -1;
    char   *className = NULL;
    jstring rstring;

    UNUSED(clss);

    if (NULL == (className = (char *)malloc(VSNAMELENMAX + 1)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSgetclass: failed to allocate data buffer");

    if (hdfclass == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSgetclass: hdfclass is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, hdfclass) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSgetclass: output array hdfclass < order 1");

    /* get the class class of the vgroup */
    if ((rval = VSgetclass((int32)vdata_id, className)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

    className[VSNAMELENMAX] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, className)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, hdfclass, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(className);

    return;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetname(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray hdfname)
{
    int32   rval = -1;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if (NULL == (data = (char *)malloc(sizeof(char) * (size_t)VSNAMELENMAX + 1)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSgetname: failed to allocate data buffer");

    if (hdfname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSgetname: hdfname is NULL");

    /* get the name name of the vgroup */
    if ((rval = VSgetname((int32)vdata_id, data)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

    data[VSNAMELENMAX] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, hdfname, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);

    return;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSelts(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSelts((int32)vdata_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSfdefine(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname,
                                     jint numbertype, jint order)
{
    int32       rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (fieldname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfdefine:  fieldname is null");

    PIN_JAVA_STRING(ENVONLY, fieldname, fstr, NULL, "VSfdefine:  fieldname not pinned");

    if ((rval = VSfdefine((int32)vdata_id, (char *)fstr, (int32)numbertype, (int32)order)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, fieldname, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSfexist(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname)
{
    int32       rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (fieldname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfexist:  fieldname is null");

    PIN_JAVA_STRING(ENVONLY, fieldname, fstr, NULL, "VSfexist:  fieldname not pinned");

    /* Check the fields */
    if ((rval = VSfexist((int32)vdata_id, (char *)fstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, fieldname, fstr);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfind(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname)
{
    int32       rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (fieldname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfind:  fieldname is null");

    PIN_JAVA_STRING(ENVONLY, fieldname, fstr, NULL, "VSfind:  fieldname not pinned");

    /* Check the fields */
    if ((rval = VSfind((int32)vdata_id, (char *)fstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, fieldname, fstr);

    return (rval);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetblocksize(JNIEnv *env, jclass clss, jlong vdata_id, jint blocksize)
{
    int32 rval = FAIL;

    UNUSED(clss);

    /* Check the fields */
    if ((rval = VSsetblocksize((int32)vdata_id, (int32)blocksize)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (rval);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetnumblocks(JNIEnv *env, jclass clss, jlong vdata_id, jint numblocks)
{
    int32 rval = FAIL;

    UNUSED(clss);

    /* Check the fields */
    if ((rval = VSsetnumblocks((int32)vdata_id, (int32)numblocks)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (rval);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetfields(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray fields)
{
    int     rval = FAIL;
    char   *flds = NULL;
    jstring rstring;

    UNUSED(clss);

    if (NULL == (flds = (char *)malloc(sizeof(char) * (size_t)25600)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSgetfields: failed to allocate data buffer");

    if (fields == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSgetfields: fields is NULL");

    /* get the fields name in the vdata */
    if ((rval = VSgetfields((int32)vdata_id, flds)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    flds[25599] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, flds)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, fields, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(flds);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetinterlace(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSgetinterlace((int32)vdata_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSinquire(JNIEnv *env, jclass clss, jlong vdata_id, jintArray iargs,
                                     jobjectArray sargs)
{
    int      rval    = FAIL;
    char    *flds    = NULL;
    char    *name    = NULL;
    jint    *theArgs = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if (iargs == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSinquire: iargs is NULL");

    if (sargs == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSinquire: sargs is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, iargs) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSinquire: output array iargs < order 3");

    if (ENVPTR->GetArrayLength(ENVONLY, sargs) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSinquire: output array sargs < order 2");

    if (NULL == (flds = (char *)malloc(sizeof(char) * (size_t)MAX_FIELD_SIZE + 1)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSinquire: failed to allocate data buffer");

    if (NULL == (name = (char *)malloc(sizeof(char) * (size_t)H4_MAX_NC_NAME + 1)))
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSinquire: failed to allocate data buffer");

    PIN_INT_ARRAY(ENVONLY, iargs, theArgs, &isCopy, "VSinquire:  iargs not pinned");

    if ((rval = VSinquire((int32)vdata_id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]), flds,
                          (int32 *)&(theArgs[2]), name)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    flds[MAX_FIELD_SIZE] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, flds)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, sargs, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

    name[H4_MAX_NC_NAME] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, name)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, sargs, 1, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(name);
    free(flds);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetblockinfo(JNIEnv *env, jclass clss, jlong vdata_id, jintArray iargs)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (iargs == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSgetblockinfo: iargs is NULL");

    PIN_INT_ARRAY(ENVONLY, iargs, theArgs, &isCopy, "VSgetblockinfo:  iargs not pinned");

    if ((rval = VSgetblockinfo((int32)vdata_id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, iargs, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSlone(JNIEnv *env, jclass clss, jlong fid, jintArray ref_array, jint arraysize)
{
    int      rval = FAIL;
    jint    *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (ref_array == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSlone: ref_array is NULL");

    PIN_INT_ARRAY(ENVONLY, ref_array, arr, &isCopy, "VSlone:  ref_array not pinned");

    /* get the lone vdata reference number in the vdata */
    if ((rval = VSlone((int32)fid, (int32 *)arr, (int32)arraysize)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_INT_ARRAY(ENVONLY, ref_array, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSread(JNIEnv *env, jclass clss, jlong vdata_id, jbyteArray databuf, jint nrecords,
                                  jint interlace)
{
    int32    rval = FAIL;
    jbyte   *dat  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (databuf == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSread: databuf is NULL");

    PIN_BYTE_ARRAY_CRITICAL(ENVONLY, databuf, dat, &isCopy, "VSread:  databuf not pinned");

    /* retrieve the general info. */
    if ((rval = VSread((int32)vdata_id, (unsigned char *)dat, nrecords, interlace)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_ARRAY_CRITICAL(ENVONLY, databuf, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSseek(JNIEnv *env, jclass clss, jlong vdata_id, jint nrecord)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSseek((int32)vdata_id, (int32)nrecord)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetfields(JNIEnv *env, jclass clss, jlong vdata_id, jstring fields)
{
    int32       rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (fields == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetfields:  fields is null");

    PIN_JAVA_STRING(ENVONLY, fields, fstr, NULL, "VSsetfields:  fields not pinned");

    /* set the vdata fields to read */
    if ((rval = VSsetfields((int32)vdata_id, (char *)fstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, fields, fstr);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetinterlace(JNIEnv *env, jclass clss, jlong vdata_id, jint interlace)
{
    int32 rval = FAIL;

    UNUSED(clss);

    /* set the interlace for Vdata */
    if ((rval = VSsetinterlace((int32)vdata_id, (int32)interlace)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsizeof(JNIEnv *env, jclass clss, jlong vdata_id, jstring fields)
{
    int32       rval = FAIL;
    const char *fstr = NULL;

    UNUSED(clss);

    if (fields == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsizeof:  fields is null");

    PIN_JAVA_STRING(ENVONLY, fields, fstr, NULL, "VSsizeof:  fields not pinned");

    /* get the size of a Vdata */
    if ((rval = VSsizeof((int32)vdata_id, (char *)fstr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (fstr)
        UNPIN_JAVA_STRING(ENVONLY, fields, fstr);

    return (rval);
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSappendable(JNIEnv *env, jclass clss, jint vkey, jint block_size)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = VSappendable((int32)vkey, (int32)block_size)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSdump(JNIEnv *env, jclass clss, jint vkey)
{
    UNUSED(env);
    UNUSED(clss);

    VSdump((int32)vkey);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfindclass(JNIEnv *env, jclass clss, jlong vdata_id, jstring hdfclassname)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (hdfclassname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfindclass:  hdfclassname is null");

    PIN_JAVA_STRING(ENVONLY, hdfclassname, str, NULL, "VSfindclass:  hdfclassname not pinned");

    /* get the class name of the vgroup */
    if ((rval = VSfindclass((int32)vdata_id, str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, hdfclassname, str);

    return rval;
}

/* no idea at all how to deal with 2-D arrays.... */
JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSfpack(JNIEnv *env, jclass clss, jlong vdata_id, jint action,
                                   jstring fields_in_buf, jarray buf, jint bufsize, jstring fields,
                                   jarray bufptrs)
{

    UNUSED(clss);
    UNUSED(vdata_id);
    UNUSED(action);
    UNUSED(fields_in_buf);
    UNUSED(buf);
    UNUSED(bufsize);
    UNUSED(fields);
    UNUSED(bufptrs);

    /*
        VSfpack((int32) vdata_id, (int) action, char
            *fields_in_buf, void * buf, int buf_size, int
            n_records, char *fields, void * bufptrs[]);
    */
    H4_UNIMPLEMENTED(ENVONLY, "VSfpack");

done:
    return;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetversion(JNIEnv *env, jclass clss, jint key)
{
    int32 rval;

    UNUSED(clss);

    if ((rval = VSgetversion((int32)key)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetclass(JNIEnv *env, jclass clss, jlong vdata_id, jstring vdata_class)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (vdata_class == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetclass:  vdata_class is null");

    PIN_JAVA_STRING(ENVONLY, vdata_class, str, NULL, "VSsetclass:  vdata_class not pinned");

    if ((rval = VSsetclass((int32)vdata_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, vdata_class, str);

    return;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetexternalfile(JNIEnv *env, jclass clss, jint vkey, jstring filename,
                                             jint offset)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetexternalfile:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, str, NULL, "VSsetexternalfile:  filename not pinned");

    if ((rval = VSsetexternalfile((int32)vkey, (char *)str, (int32)offset)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, filename, str);

    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetname(JNIEnv *env, jclass clss, jlong vdata_id, jstring vdata_name)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (vdata_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetname:  vdata_name is null");

    PIN_JAVA_STRING(ENVONLY, vdata_name, str, NULL, "VSsetname:  vdata_name not pinned");

    if ((rval = VSsetname((int32)vdata_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, vdata_name, str);

    return;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSwrite(JNIEnv *env, jclass clss, jlong vdata_id, jbyteArray databuf,
                                   jint n_records, jint interlace)
{
    int32    rval = FAIL;
    jbyte   *dat  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (databuf == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSwrite: databuf is NULL");

    PIN_BYTE_ARRAY(ENVONLY, databuf, dat, &isCopy, "VSwrite:  databuf not pinned");

    if ((rval = VSwrite((int32)vdata_id, (unsigned char *)dat, (int32)n_records, (int32)interlace)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (dat)
        UNPIN_BYTE_ARRAY(ENVONLY, databuf, dat, (rval == FAIL) ? JNI_ABORT : 0);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSattrinfo(JNIEnv *env, jclass clss, jlong id, jint index, jint attr_index,
                                      jobjectArray name, jintArray argv)
{
    int32    rval    = FAIL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(256)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "VSattrinfo: failed to allocate data buffer");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSattrinfo: name is NULL");

    else if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSattrinfo: argv is NULL");

    else if (ENVPTR->GetArrayLength(ENVONLY, name) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSattrinfo: output array name < order 1");

    else if (ENVPTR->GetArrayLength(ENVONLY, argv) < 3)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "VSattrinfo: output array argv < order 3");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "VSattrinfo:  argv not pinned");

    if ((rval = VSattrinfo((int32)id, (int32)index, (int32)attr_index, data, (int32 *)&(theArgs[0]),
                           (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]))) == FAIL)
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
Java_hdf_hdflib_HDFLibrary_VSfindex(JNIEnv *env, jclass clss, jlong id, jstring name, jintArray findex)
{
    int         rval = FAIL;
    const char *str  = NULL;
    jint       *arr  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (findex == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfindex: findex is NULL");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfindex:  name is null");

    PIN_INT_ARRAY(ENVONLY, findex, arr, &isCopy, "VSfindex:  findex not pinned");
    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "VSfindex:  name not pinned");

    if ((rval = VSfindex((int32)id, str, (int32 *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);
    if (arr)
        UNPIN_INT_ARRAY(ENVONLY, findex, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfindattr(JNIEnv *env, jclass clss, jlong id, jint index, jstring name)
{
    int32       rval  = FAIL;
    const char *cname = NULL;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSfindattr:  name is null");

    PIN_JAVA_STRING(ENVONLY, name, cname, NULL, "VSfindattr:  name not pinned");

    if ((rval = VSfindattr((int32)id, (int32)index, cname)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (cname)
        UNPIN_JAVA_STRING(ENVONLY, name, cname);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfnattrs(JNIEnv *env, jclass clss, jlong id, jint attr)
{
    int rval = -1;

    UNUSED(clss);

    if ((rval = VSfnattrs((int32)id, (int32)attr)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetattr(JNIEnv *env, jclass clss, jlong id, jint field_index, jint attr_index,
                                     jbyteArray values)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSgetattr: values is NULL");

    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "VSgetattr:  values not pinned");

    if ((rval = VSgetattr((int32)id, (int32)field_index, (int32)attr_index, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSisattr(JNIEnv *env, jclass clss, jlong id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = VSisattr((int32)id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSnattrs(JNIEnv *env, jclass clss, jlong id)
{
    int rval = -1;

    UNUSED(clss);

    if ((rval = VSnattrs((int32)id)) <= 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JILjava_lang_String_2(JNIEnv *env, jclass clss,
                                                                                 jlong id, jint index,
                                                                                 jstring attr_name,
                                                                                 jlong data_type, jint count,
                                                                                 jstring values)
{
    int         rval = FAIL;
    const char *str  = NULL;
    const char *val  = NULL;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetattr:  attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetattr:  values is null");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "VSsetattr:  attr_name not pinned");
    PIN_JAVA_STRING(ENVONLY, values, val, NULL, "VSsetattr:  values not pinned");

    if ((rval = VSsetattr((int32)id, (int32)index, (char *)str, (int32)data_type, (int32)count,
                          (void *)val)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (val)
        UNPIN_JAVA_STRING(ENVONLY, values, val);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JI_3B(JNIEnv *env, jclass clss, jlong id,
                                                                 jint index, jstring attr_name,
                                                                 jlong data_type, jint count,
                                                                 jbyteArray values)
{
    int         rval = FAIL;
    jbyte      *arr  = NULL;
    const char *str  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetattr: values is NULL");

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VSsetattr:  attr_name is null");

    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "VSsetattr:  values not pinned");
    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "VSsetattr:  attr_name not pinned");

    if ((rval = VSsetattr((int32)id, (int32)index, (char *)str, (int32)data_type, (int32)count,
                          (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
