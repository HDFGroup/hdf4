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
#include "hdfvhImp.h"

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHmakegroup(JNIEnv *env, jclass clss, jlong file_id, jintArray tag_array,
                                       jintArray ref_array, jint n_objects, jstring vgroup_name,
                                       jstring vgroup_class)
{
    int32       rval  = -1;
    jint       *tags  = NULL;
    jint       *refs  = NULL;
    const char *vname = NULL;
    const char *vcls  = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (vgroup_name == NULL)
        h4nullArgument(env, "VHmakegroup:  vgroup_name is NULL");

    if (vgroup_class == NULL)
        h4nullArgument(env, "VHmakegroup:  vgroup_class is NULL");

    if (tag_array == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VHmakegroup:  tag_array is NULL");

    if (ref_array == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "VHmakegroup:  ref_array is NULL");

    PIN_JAVA_STRING(ENVONLY, vgroup_name, vname, NULL, "VHmakegroup:  vgroup_name not pinned");
    PIN_JAVA_STRING(ENVONLY, vgroup_class, vcls, NULL, "VHmakegroup:  vgroup_class not pinned");
    PIN_INT_ARRAY(ENVONLY, tag_array, tags, &isCopy, "VHmakegroup:  tag_array not pinned");
    PIN_INT_ARRAY(ENVONLY, ref_array, refs, &isCopy, "VHmakegroup:  ref_array not pinned");

    if ((rval = VHmakegroup((int32)file_id, (int32 *)tags, (int32 *)refs, (int32)n_objects, vname, vcls)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (refs)
        UNPIN_INT_ARRAY(ENVONLY, ref_array, refs, (rval == FAIL) ? JNI_ABORT : 0);
    if (tags)
        UNPIN_INT_ARRAY(ENVONLY, tag_array, tags, (rval == FAIL) ? JNI_ABORT : 0);
    if (vcls)
        UNPIN_JAVA_STRING(ENVONLY, vgroup_class, vcls);
    if (vname)
        UNPIN_JAVA_STRING(ENVONLY, vgroup_name, vname);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHstoredata(JNIEnv *env, jclass clss, jlong file_id, jstring fieldname,
                                       jbyteArray buf, jint n_records, jint data_type, jstring vdata_name,
                                       jstring vdata_class)
{
    int32       rval    = -1;
    jbyte      *buffer  = NULL;
    const char *fldname = NULL;
    const char *vname   = NULL;
    const char *vcls    = NULL;
    jboolean    isCopy;

    UNUSED(clss);

    if (fieldname == NULL)
        h4nullArgument(env, "VHstoredatam:  fieldname is NULL");

    if (vdata_name == NULL)
        h4nullArgument(env, "VHstoredatam:  vdata_name is NULL");

    if (vdata_class == NULL)
        h4nullArgument(env, "VHstoredatam:  vdata_class is NULL");

    if (buf == NULL)
        h4nullArgument(env, "VHstoredatam:  buf is NULL");

    PIN_JAVA_STRING(ENVONLY, fieldname, fldname, NULL, "VHstoredata:  fieldname not pinned");
    PIN_JAVA_STRING(ENVONLY, vdata_name, vname, NULL, "VHstoredata:  vdata_name not pinned");
    PIN_JAVA_STRING(ENVONLY, vdata_class, vcls, NULL, "VHstoredata:  vdata_class not pinned");
    PIN_BYTE_ARRAY(ENVONLY, buf, buffer, &isCopy, "VHstoredata:  buf not pinned");

    if ((rval = VHstoredata((int32)file_id, fldname, (uint8 *)buffer, (int32)n_records, (int32)data_type,
                            vname, vcls)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (buffer)
        UNPIN_BYTE_ARRAY(ENVONLY, buf, buffer, (rval == FAIL) ? JNI_ABORT : 0);
    if (vcls)
        UNPIN_JAVA_STRING(ENVONLY, vdata_class, vcls);
    if (vname)
        UNPIN_JAVA_STRING(ENVONLY, vdata_name, vname);
    if (fldname)
        UNPIN_JAVA_STRING(ENVONLY, fieldname, fldname);

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHstoredatam(JNIEnv *env, jclass clss, jlong file_id, jstring fieldname,
                                        jbyteArray buf, jint n_records, jint data_type, jstring vdata_name,
                                        jstring vdata_class, jint order)
{
    int32       rval    = -1;
    jbyte      *buffer  = NULL;
    const char *fldname = NULL;
    const char *vname   = NULL;
    const char *vcls    = NULL;
    jboolean    isCopy;
    HFILEID     fid = (int32)file_id;

    UNUSED(clss);

    if (fieldname == NULL)
        h4nullArgument(env, "VHstoredatam:  fieldname is NULL");

    if (vdata_name == NULL)
        h4nullArgument(env, "VHstoredatam:  vdata_name is NULL");

    if (vdata_class == NULL)
        h4nullArgument(env, "VHstoredatam:  vdata_class is NULL");

    if (buf == NULL)
        h4nullArgument(env, "VHstoredatam:  buf is NULL");

    PIN_JAVA_STRING(ENVONLY, fieldname, fldname, NULL, "VHstoredatam:  fieldname not pinned");
    PIN_JAVA_STRING(ENVONLY, vdata_name, vname, NULL, "VHstoredatam:  vdata_name not pinned");
    PIN_JAVA_STRING(ENVONLY, vdata_class, vcls, NULL, "VHstoredatam:  vdata_class not pinned");
    PIN_BYTE_ARRAY(ENVONLY, buf, buffer, &isCopy, "VHstoredatam:  buf not pinned");

    if ((rval = VHstoredatam(fid, fldname, (const uint8 *)buffer, (int32)n_records, (int32)data_type, vname,
                             vcls, (int32)order)) < 0)
        H4_LIBRARY_ERROR(ENVONLY);

done:
    if (buffer)
        UNPIN_BYTE_ARRAY(ENVONLY, buf, buffer, (rval == FAIL) ? JNI_ABORT : 0);
    if (vcls)
        UNPIN_JAVA_STRING(ENVONLY, vdata_class, vcls);
    if (vname)
        UNPIN_JAVA_STRING(ENVONLY, vdata_name, vname);
    if (fldname)
        UNPIN_JAVA_STRING(ENVONLY, fieldname, fldname);

    return (jint)rval;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
