
/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/
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
#endif


#include "hdf.h"
#include "jni.h"
#include "h4jni.h"

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldesize
(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32   retVal;

    retVal = VFfieldesize((int32)vdata_id, (int32)field_index);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldisize
(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32   retVal;

    retVal = VFfieldisize((int32)vdata_id, (int32)field_index);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jstring JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldname
(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    jstring rstring;
    char * str;

    str = VFfieldname((int32) vdata_id,  (int32) field_index);

    /* check for error */

    /* convert it to java string */
    rstring = ENVPTR->NewStringUTF(ENVPAR str);

    return rstring;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldorder
(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32   retVal;

    retVal = VFfieldorder((int32)vdata_id, (int32)field_index);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VFfieldtype
(JNIEnv *env, jclass clss, jlong vdata_id, int field_index)
{
    int32   retVal;

    retVal = VFfieldtype((int32)vdata_id, (int32)field_index);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VFnfields
(JNIEnv *env, jclass clss, jlong key)
{
    int32   retVal;

    retVal = VFnfields((int32)key);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

#ifdef __cplusplus
}
#endif
