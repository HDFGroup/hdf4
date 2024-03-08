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
#include "hdfvgroupImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vstart(JNIEnv *env, jclass clss, jlong fid)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Vstart((int32)fid)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Vattach(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref, jstring accessmode)
{
    int         rval   = FAIL;
    const char *access = NULL;
    HFILEID     id     = (HFILEID)fid;

    UNUSED(clss);

    if (accessmode == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vattach:  accessmode is null");

    PIN_JAVA_STRING(ENVONLY, accessmode, access, NULL, "Vattach:  accessmode not pinned");

    /* open HDF file specified by hdf_HDF_file */
    if ((rval = Vattach(id, (int32)vgroup_ref, access)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (access)
        UNPIN_JAVA_STRING(ENVONLY, accessmode, access);

    return (jlong)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vdetach(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vdetach((int32)vgroup_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vend(JNIEnv *env, jclass clss, jlong fid)
{
    int     rval = FAIL;
    HFILEID id   = (HFILEID)fid;

    UNUSED(clss);

    if ((rval = Vend(id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetid(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vgetid((int32)fid, (int32)vgroup_ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jlong)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetclass(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfclassname)
{
    int32   rval = FAIL;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if ((data = (char *)malloc(H4_MAX_NC_CLASS + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Vgetclass: failed to allocate data buffer");

    if (hdfclassname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgetclass: hdfclassname is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, hdfclassname) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vgetclass: output array hdfclassname < order 1");

    /* get the class name of the vgroup */
    if ((rval = Vgetclass((int32)vgroup_id, data)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

    data[H4_MAX_NC_CLASS] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, hdfclassname, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);

    return;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetname(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfname)
{
    int32   rval = FAIL;
    char   *data = NULL;
    jstring rstring;

    UNUSED(clss);

    if ((data = (char *)malloc(H4_MAX_GR_NAME + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Vgetname: failed to allocate data buffer");

    if (hdfname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgetname: hdfname is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, hdfname) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vgetname: array hdfname < order 1");

    if ((rval = Vgetname((int32)vgroup_id, data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[H4_MAX_GR_NAME] = '\0';
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

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvg(JNIEnv *env, jclass clss, jlong vgroup_id, jint vgroup_ref)
{
    int rval = FALSE;

    UNUSED(clss);

    if ((rval = Visvg((int32)vgroup_id, vgroup_ref)) == FALSE)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvs(JNIEnv *env, jclass clss, jlong vgroup_id, jint vdata_ref)
{
    int rval = FALSE;

    UNUSED(clss);

    if ((rval = Visvs((int32)vgroup_id, vdata_ref)) == FALSE)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagrefs(JNIEnv *env, jclass clss, jlong vgroup_id, jintArray tags,
                                       jintArray refs, jint size)
{
    int32    rval   = FAIL;
    jint    *tagVal = NULL;
    jint    *refVal = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (tags == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgettagrefs: tags is NULL");

    if (refs == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgettagrefs: refs is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, tags) < size)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vgettagrefs: output array tags < order 'size'");

    if (ENVPTR->GetArrayLength(ENVONLY, refs) < size)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vgettagrefs: output array refs < order 'size'");

    PIN_INT_ARRAY(ENVONLY, tags, tagVal, &isCopy, "Vgettagrefs: tags not pinned");
    PIN_INT_ARRAY(ENVONLY, refs, refVal, &isCopy, "Vgettagrefs: refs not pinned");

    /* get the tag/ref pairs number in the vgroup */
    if ((rval = Vgettagrefs((int32)vgroup_id, (int32 *)tagVal, (int32 *)refVal, size)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (refVal)
        UNPIN_INT_ARRAY(ENVONLY, refs, refVal, (rval == FAIL) ? JNI_ABORT : 0);
    if (tagVal)
        UNPIN_INT_ARRAY(ENVONLY, tags, tagVal, (rval == FAIL) ? JNI_ABORT : 0);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint index, jintArray tagref)
{
    int      rval    = FAIL;
    jint    *theArgs = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (tagref == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgettagref: tagref is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, tagref) < 2)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vgettagref: output array tagref < order 2");

    PIN_INT_ARRAY(ENVONLY, tagref, theArgs, &isCopy, "Vgettagref: tagref not pinned");

    /* get the tag/ref pairs number in the vgroup */
    if ((rval = Vgettagref((int32)vgroup_id, index, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]))) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (theArgs)
        UNPIN_INT_ARRAY(ENVONLY, tagref, theArgs, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vntagrefs(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vntagrefs((int32)vgroup_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinqtagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vinqtagref((int32)vgroup_id, (int32)tag, (int32)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vlone(JNIEnv *env, jclass clss, jlong fid, jintArray ref_array, jint arraysize)
{
    int      rval = FAIL;
    jint    *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (ref_array == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vlone: ref_array is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, ref_array) < arraysize)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vlone: output array ref_array < order 'arraysize'");

    PIN_INT_ARRAY(ENVONLY, ref_array, arr, &isCopy, "Vlone:  ref_array not pinned");

    /* get the lone group reference number in the vgroup */
    if ((rval = Vlone((int32)fid, (int32 *)arr, arraysize)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_INT_ARRAY(ENVONLY, ref_array, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vaddtagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Vaddtagref((int32)vgroup_id, (int32)tag, (int32)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vdeletetagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Vdeletetagref((int32)vgroup_id, (int32)tag, (int32)ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vclose_I(JNIEnv *env, jclass clss, jlong file_id)
{
    int rval = FAIL;

    UNUSED(clss);

    if ((rval = Vclose((int32)file_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfind(JNIEnv *env, jclass clss, jlong file_id, jstring vgname)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (vgname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vfind:  vgname is null");

    PIN_JAVA_STRING(ENVONLY, vgname, str, NULL, "Vfind:  vgname not pinned");

    if ((rval = Vfind((int32)file_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, vgname, str);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindclass(JNIEnv *env, jclass clss, jlong file_id, jstring vgclassname)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (vgclassname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vfindclass:  vgclassname is null");

    PIN_JAVA_STRING(ENVONLY, vgclassname, str, NULL, "Vfindclass:  vgclassname not pinned");

    if ((rval = Vfindclass((int32)file_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, vgclassname, str);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vflocate(JNIEnv *env, jclass clss, jint key, jstring field)
{
    int32       rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (field == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vflocate:  field is null");

    PIN_JAVA_STRING(ENVONLY, field, str, NULL, "Vflocate:  field not pinned");

    if ((rval = Vflocate((int32)key, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, field, str);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetnext(JNIEnv *env, jclass clss, jint vkey, jint elem_ref)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vgetnext((int32)vkey, (int32)elem_ref)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vinquire(JNIEnv *env, jclass clss, jlong vgroup_id, jintArray n_entries,
                                    jobjectArray vgroup_name)
{
    int      rval   = FAIL;
    jint    *theArg = NULL;
    char    *data   = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(H4_MAX_NC_NAME + 1)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Vinquire: failed to allocate data buffer");

    if (n_entries == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vinquire: n_entries is NULL");

    if (vgroup_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vinquire: vgroup_name is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, n_entries) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vinquire: output array n_entries < order 1");

    if (ENVPTR->GetArrayLength(ENVONLY, vgroup_name) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vinquire: output array vgroup_name < order 1");

    PIN_INT_ARRAY(ENVONLY, n_entries, theArg, &isCopy, "Vinquire:  n_entries not pinned");

    if ((rval = Vinquire((int32)vgroup_id, (int32 *)&(theArg[0]), data)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

    data[H4_MAX_NC_NAME] = '\0';
    /* convert it to java string */
    if (NULL == (rstring = ENVPTR->NewStringUTF(ENVONLY, data)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->SetObjectArrayElement(ENVONLY, vgroup_name, 0, rstring);
    CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    ENVPTR->DeleteLocalRef(ENVONLY, rstring);

done:
    free(data);
    if (theArg)
        UNPIN_INT_ARRAY(ENVONLY, n_entries, theArg, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinsert(JNIEnv *env, jclass clss, jlong vgroup_id, jint v_id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vinsert((int32)vgroup_id, (int32)v_id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnrefs(JNIEnv *env, jclass clss, jint vkey, jint tag)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vnrefs((int32)vkey, (int32)tag)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vopen(JNIEnv *env, jclass clss, jstring filename, jint access, jshort ndds)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (filename == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vopen:  filename is null");

    PIN_JAVA_STRING(ENVONLY, filename, str, NULL, "Vopen:  filename not pinned");

    if ((rval = Vopen((char *)str, (int)access, (int16)ndds)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, filename, str);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetclass(JNIEnv *env, jclass clss, jlong vgroup_id, jstring hdfclassname)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (hdfclassname == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetclass:  hdfclassname is null");

    PIN_JAVA_STRING(ENVONLY, hdfclassname, str, NULL, "Vsetclass:  hdfclassname not pinned");

    if ((rval = Vsetclass((int32)vgroup_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, hdfclassname, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetname(JNIEnv *env, jclass clss, jlong vgroup_id, jstring name)
{
    int         rval = FAIL;
    const char *str  = NULL;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetname: name is null");

    PIN_JAVA_STRING(ENVONLY, name, str, NULL, "Vsetname:  name not pinned");

    if ((rval = Vsetname((int32)vgroup_id, (char *)str)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vattrinfo(JNIEnv *env, jclass clss, jlong id, jint index, jobjectArray name,
                                     jintArray argv)
{
    int32    rval    = FAIL;
    jint    *theArgs = NULL;
    char    *data    = NULL;
    jstring  rstring;
    jboolean isCopy;

    UNUSED(clss);

    if ((data = (char *)malloc(256)) == NULL)
        H4_OUT_OF_MEMORY_ERROR(ENVONLY, "Vattrinfo2: failed to allocate data buffer");

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vattrinfo2: name is NULL");

    if (argv == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vattrinfo2: argv is NULL");

    if (ENVPTR->GetArrayLength(ENVONLY, name) < 1)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vattrinfo2: output array name < order 1");

    if (ENVPTR->GetArrayLength(ENVONLY, argv) < 5)
        H4_BAD_ARGUMENT_ERROR(ENVONLY, "Vattrinfo2: output array argv < order 5");

    PIN_INT_ARRAY(ENVONLY, argv, theArgs, &isCopy, "Vattrinfo2:  argv not pinned");

    if ((rval = Vattrinfo2((int32)id, (int32)index, data, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                           (int32 *)&(theArgs[2]), (int32 *)&(theArgs[3]), (uint16 *)&(theArgs[4]))) == FAIL)
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

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindattr(JNIEnv *env, jclass clss, jlong id, jstring name)
{
    int32       rval  = FAIL;
    const char *cname = NULL;

    UNUSED(clss);

    if (name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vfindattr: name is null");

    PIN_JAVA_STRING(ENVONLY, name, cname, NULL, "Vfindattr:  name not pinned");

    if ((rval = Vfindattr((int32)id, cname)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (cname)
        UNPIN_JAVA_STRING(ENVONLY, name, cname);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetattr(JNIEnv *env, jclass clss, jlong gr_id, jint attr_index, jbyteArray values)
{
    int      rval = FAIL;
    jbyte   *arr  = NULL;
    jboolean isCopy;

    UNUSED(clss);

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vgetattr: values is NULL");

    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "Vgetattr:  values not pinned");

    if ((rval = Vgetattr2((int32)gr_id, (int32)attr_index, (void *)arr)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (arr)
        UNPIN_BYTE_ARRAY(ENVONLY, values, arr, (rval == FAIL) ? JNI_ABORT : 0);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetversion(JNIEnv *env, jclass clss, jlong id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vgetversion((int32)id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnattrs(JNIEnv *env, jclass clss, jlong id)
{
    int32 rval = FAIL;

    UNUSED(clss);

    if ((rval = Vnattrs2((int32)id)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JILjava_lang_String_2(JNIEnv *env, jclass clss,
                                                                               jlong gr_id, jstring attr_name,
                                                                               jlong data_type, jint count,
                                                                               jstring values)
{
    int         rval = FAIL;
    const char *str  = NULL;
    const char *val  = NULL;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetattr: attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetattr: values is null");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "Vsetattr:  attr_name not pinned");
    PIN_JAVA_STRING(ENVONLY, values, val, NULL, "Vsetattr:  values not pinned");

    if ((rval = Vsetattr((int32)gr_id, (char *)str, (int32)data_type, (int32)count, (void *)val)) == FAIL)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (val)
        UNPIN_JAVA_STRING(ENVONLY, values, val);
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, attr_name, str);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JI_3B(JNIEnv *env, jclass clss, jlong id,
                                                               jstring attr_name, jlong data_type, jint count,
                                                               jbyteArray values)
{
    int         rval = FAIL;
    jbyte      *arr  = NULL;
    const char *str  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (attr_name == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetattr: attr_name is null");

    if (values == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "Vsetattr: values is null");

    PIN_JAVA_STRING(ENVONLY, attr_name, str, NULL, "Vsetattr:  attr_name not pinned");
    PIN_BYTE_ARRAY(ENVONLY, values, arr, &isCopy, "Vsetattr:  values not pinned");

    if ((rval = Vsetattr((int32)id, (char *)str, (int32)data_type, (int32)count, (void *)arr)) == FAIL)
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
