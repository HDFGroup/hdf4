
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

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQuerycount
(JNIEnv *env, jclass clss, jlong vdata_id, jintArray n_records)
{
    intn rval;
    jint * theArg;
    jboolean bb;

    if (n_records == NULL) {
        h4nullArgument(env, "VSQuerycount: n_records is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR n_records) < 1) {
        h4badArgument(env, "VSQuerycount: output array n_records < order 1");
    } /* end else if */
    else {
        theArg = ENVPTR->GetIntArrayElements(ENVPAR n_records,&bb);

        if (theArg == NULL) {
            h4JNIFatalError(env, "VSQuerycount: n_records not pinned");
        } /* end if */
        else {
            rval = VSQuerycount((int32) vdata_id, (int32 *)&(theArg[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR n_records,theArg,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR n_records,theArg,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryfields
(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray fields)
{
    intn rval;
    char flds[4096];
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (fields == NULL) {
        h4nullArgument(env, "VSQueryfields: fields is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR fields) < 1) {
        h4badArgument(env, "VSQueryfields: output array fields < order 1");
    } /* end else if */
    else {
        rval = VSQueryfields((int32) vdata_id, (char *)flds);
        flds[4095] = '\0';

        if (rval == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            /* convert it to java string */
            rstring = ENVPTR->NewStringUTF(ENVPAR flds);

            /*  create a Java String object in the calling environment... */
            jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
            if (jc == NULL) {
                return JNI_FALSE; /* exception is raised */
            }
            o = ENVPTR->GetObjectArrayElement(ENVPAR fields,0);
            if (o == NULL) {
                return JNI_FALSE;
            }
            bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
            if (bb == JNI_FALSE) {
                return JNI_FALSE;
            }
            ENVPTR->SetObjectArrayElement(ENVPAR fields,0,(jobject)rstring);
            ENVPTR->DeleteLocalRef(ENVPAR o);

            return JNI_TRUE;
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryinterlace
(JNIEnv *env, jclass clss, jlong vdata_id, jintArray interlace)
{
    intn rval;
    jint * theArg;
    jboolean bb;

    if (interlace == NULL) {
        h4nullArgument(env, "VSQueryinterlace: interlace is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR interlace) < 1) {
        h4badArgument(env, "VSQueryinterlace: output array interlace < order 1");
    } /* end else if */
    else {
        theArg = ENVPTR->GetIntArrayElements(ENVPAR interlace,&bb);

        if (theArg == NULL) {
            h4JNIFatalError(env, "VSQueryinterlace: interlace not pinned");
        } /* end if */
        else {
            rval = VSQueryinterlace((int32) vdata_id, (int32 *)&(theArg[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR interlace,theArg,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR interlace,theArg,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}



JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryname
(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray vdata_name)
{
    intn rval;
    char *nm;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (vdata_name == NULL) {
        h4nullArgument(env, "VSQueryname: vdata_name is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR vdata_name) < 1) {
        h4badArgument(env, "VSQueryname: output array vdata_name < order 1");
    } /* end else if */
    else {
        nm = (char *)HDmalloc(VSNAMELENMAX+1);

        if (nm == NULL) {
            h4outOfMemory(env,  "VSQueryname");
        } /* end if */
        else {
            rval = VSQueryname((int32) vdata_id, (char *)nm);
            nm[VSNAMELENMAX] = '\0';

            if (rval == FAIL) {
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                /* convert it to java string */
                rstring = ENVPTR->NewStringUTF(ENVPAR nm);

                /*  create a Java String object in the calling environment... */
                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (jc == NULL) {
                    HDfree(nm);
                    return JNI_FALSE; /* exception is raised */
                }
                o = ENVPTR->GetObjectArrayElement(ENVPAR vdata_name,0);
                if (o == NULL) {
                    HDfree(nm);
                    return JNI_FALSE;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                if (bb == JNI_FALSE) {
                    free(nm);
                    return JNI_FALSE;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR vdata_name,0,(jobject)rstring);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            } /* end else */

            HDfree(nm);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryref
(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval;

    rval = VSQueryref((int32)vdata_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSQuerytag
(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32 rval;

    rval = VSQuerytag((int32)vdata_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSQueryvsize
(JNIEnv *env, jclass clss, jlong vdata_id, jintArray vdata_size)
{
    intn rval;
    jint * theArg;
    jboolean bb;

    if (vdata_size == NULL) {
        h4nullArgument(env, "VSQuerysize: vdata_size is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR vdata_size) < 1) {
        h4badArgument(env, "VSQueryvsize: output array vdata_size < order 1");
    } /* end else if */
    else {
        theArg = ENVPTR->GetIntArrayElements(ENVPAR vdata_size,&bb);

        if (theArg == NULL) {
            h4JNIFatalError(env, "VSQuerysize: vdata_size not pinned");
        } /* end if */
        else {
            rval = VSQueryvsize((int32) vdata_id, (int32 *)&(theArg[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR vdata_size,theArg,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR vdata_size,theArg,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
