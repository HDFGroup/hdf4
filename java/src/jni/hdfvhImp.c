
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
#include "h4jni.h"

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHmakegroup
(JNIEnv *env, jclass oclass, jlong file_id, jintArray tag_array, jintArray ref_array,
        jint n_objects, jstring vgroup_name, jstring vgroup_class)
{
    int32 rval;
    jint *tags;
    jint *refs;
    const char *vname;
    const char *vcls;
    jboolean bb;

    PIN_JAVA_STRING_TWO(vgroup_name, vname, vgroup_class, vcls);
    if (vname != NULL && vcls != NULL) {
        if (tag_array == NULL) {
            h4nullArgument(env, "VHmakegroup:  tag_array is NULL");
        } /* end if */
        else if (ref_array == NULL) {
            h4nullArgument(env, "VHmakegroup:  ref_array is NULL");
        } /* end if */
        else {
            tags = ENVPTR->GetIntArrayElements(ENVPAR tag_array, &bb);
            if (tags == NULL) {
                h4JNIFatalError(env, "VHmakegroup:  tag_array not pinned");
            } /* end if */
            else {
                refs = ENVPTR->GetIntArrayElements(ENVPAR ref_array, &bb);
                if (refs == NULL) {
                    h4JNIFatalError(env, "VHmakegroup:  ref_array not pinned");
                } /* end if */
                else {
                    rval = VHmakegroup((int32)file_id, (int32 *)tags, (int32 *)refs,
                        (int32)n_objects, vname, vcls);
                    if (rval < 0)
                        CALL_ERROR_CHECK();

                    ENVPTR->ReleaseIntArrayElements(ENVPAR ref_array, refs, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR tag_array, tags, JNI_ABORT);
            }
        }
        UNPIN_JAVA_STRING_TWO(vgroup_name, vname, vgroup_class, vcls);
    }
    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHstoredata
(JNIEnv *env, jclass oclass, jlong file_id, jstring fieldname, jbyteArray buf, jint n_records,
        jint data_type, jstring vdata_name, jstring vdata_class)
{
    int32 rval;
    jbyte *buffer;
    const char *fldname;
    const char *vname;
    const char *vcls;
    jboolean bb;

    PIN_JAVA_STRING_THREE(fieldname, fldname, vdata_name, vname, vdata_class, vcls);
    if (fldname != NULL && vname != NULL && vcls != NULL) {
        if (buf == NULL) {
            h4nullArgument(env, "VHstoredata:  buf is NULL");
        } /* end if */
        else {
            buffer = ENVPTR->GetByteArrayElements(ENVPAR buf, &bb);
            if (buffer == NULL) {
                h4JNIFatalError(env, "VHstoredatam:  buf not pinned");
            } /* end if */
            else {
                rval = VHstoredata((int32) file_id, fldname,
                    (uint8 *)buffer, (int32)n_records, (int32)data_type,
                    vname, vcls);
                if (rval < 0)
                    CALL_ERROR_CHECK();
            }
            ENVPTR->ReleaseByteArrayElements(ENVPAR buf, buffer, JNI_ABORT);
        }
        UNPIN_JAVA_STRING_THREE(fieldname, fldname, vdata_name, vname, vdata_class, vcls);
    }
    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VHstoredatam
(JNIEnv *env, jclass oclass, jlong file_id, jstring fieldname, jbyteArray buf, jint n_records,
        jint data_type, jstring vdata_name, jstring vdata_class, jint order)
{
    int len;
    int32 rval;
    jbyte *buffer;
    const char *fldname;
    const char *vname;
    const char *vcls;
    jboolean bb;
    HFILEID fid = (int32)file_id;

    PIN_JAVA_STRING_THREE(fieldname, fldname, vdata_name, vname, vdata_class, vcls);
    if (fldname != NULL && vname != NULL && vcls != NULL) {
        if (buf == NULL) {
            h4nullArgument(env, "VHstoredatam:  buf is NULL");
        } /* end if */
        else {
            buffer = ENVPTR->GetByteArrayElements(ENVPAR buf, &bb);
            if (buffer == NULL) {
                h4JNIFatalError(env, "VHstoredatam:  buf not pinned");
            } /* end if */
            else {
                rval = VHstoredatam(fid, fldname,
                    (const uint8 *)buffer, (int32)n_records, (int32)data_type,
                    vname, vcls, (int32)order);
                if (rval < 0)
                    CALL_ERROR_CHECK();

                ENVPTR->ReleaseByteArrayElements(ENVPAR buf, buffer, JNI_ABORT);
            }
        }
        UNPIN_JAVA_STRING_THREE(fieldname, fldname, vdata_name, vname, vdata_class, vcls);
    }
    return (jint)rval;
}

#ifdef __cplusplus
}
#endif
