
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
 *  HDF 4.2 library.
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

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANstart
(JNIEnv *env, jclass clss, jlong file_id)
{
    int32       retVal = -1;

    retVal = ANstart((int32)file_id);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANend
(JNIEnv *env, jclass clss, jlong anid)
{
    int32 retVal;

    retVal = ANend((int32)anid);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANfileinfo
(JNIEnv *env, jclass clss, jlong anid, jintArray info)
{
    intn retVal;
    jint *theArgs;
    jboolean isCopy;

    if (info == NULL) {
        h4nullArgument(env, "ANfileinfo:  info is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR info) < 4) {
        h4badArgument(env, "ANfileinfo:  info input array < order 4");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR info, &isCopy);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "ANfileinfo:  info not pinned");
        } /* end if */
        else {
            retVal = ANfileinfo((int32)anid, (int32 *)&(theArgs[0]),
                (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]),
                (int32 *)&(theArgs[3]));

            if (retVal == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR info, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR info, theArgs, 0);
            } /* end else */
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}


JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANselect
(JNIEnv *env, jclass clss, jlong anid, jint index, jint anntype)
{
    int32 retVal;

    retVal = ANselect((int32)anid, (int32)index, (ann_type)anntype);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANendaccess
(JNIEnv *env, jclass clss, jlong ann_id)
{
    intn retVal;

    retVal = ANendaccess((int32)ann_id);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANnumann
(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag, jshort ref)
{
    int32 retVal;

    retVal = ANnumann((int32)an_id, (ann_type)anntype, (uint16)tag, (uint16)ref);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_ANatype2tag
(JNIEnv *env, jclass clss, jint antype)
{
    uint16 retVal;

    retVal = ANatype2tag((ann_type)antype);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jshort)retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANtag2atype
(JNIEnv *env, jclass clss, jint antag)
{
    int32 retVal;

    retVal = ANtag2atype((uint16)antag);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlist
(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag, jshort ref, jintArray annlist)
{
    intn retVal = -1;
    jint *iarr;
    jboolean isCopy;

    if (annlist == NULL) {
        h4nullArgument(env, "ANannlist:  annlist is NULL");
    } /* end if */
    else {
        iarr = ENVPTR->GetIntArrayElements(ENVPAR annlist, &isCopy);
        if (iarr == NULL) {
            h4JNIFatalError(env, "ANannlist:  annlist not pinned");
        } /* end if */
        else {
            retVal = ANannlist((int32)an_id, (ann_type)anntype, (uint16)tag,(uint16)ref,(int32 *)iarr);

            if (retVal == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR annlist, iarr, JNI_ABORT);
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR annlist, iarr, 0);
            } /* end else */
        } /* end else */
    } /* end else */
    return (jint)retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlen
(JNIEnv *env, jclass clss, jlong ann_id)
{
    int32 retVal;

    retVal = ANannlen((int32)ann_id);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANreadann
(JNIEnv *env, jclass clss, jlong ann_id, jobjectArray annbuf, jint maxlen)
{
    int32 retVal;
    char  *data;
    jclass Sjc;
    jstring rstring;
    jobject o;
    jboolean bb;

    data = (char *)HDmalloc((maxlen+1)*sizeof(char));
    if (data == NULL) {
        /* Exception */
        h4outOfMemory(env, "ANreadan");
    }
    else {
        /* read annotation from HDF */
        retVal = ANreadann((int32)ann_id, data, (int32)maxlen);

        if (retVal == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            o = ENVPTR->GetObjectArrayElement(ENVPAR annbuf, 0);
            if (o == NULL) {
                CALL_ERROR_CHECK();
            }
            else {
                Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (Sjc == NULL) {
                    CALL_ERROR_CHECK();
                }
                else if (ENVPTR->IsInstanceOf(ENVPAR o, Sjc) == JNI_FALSE) {
                        CALL_ERROR_CHECK();
                }
                else {
                    data[maxlen] = '\0';
                    rstring = ENVPTR->NewStringUTF(ENVPAR data);
                    if (rstring != NULL)
                        ENVPTR->SetObjectArrayElement(ENVPAR annbuf, 0, (jobject)rstring);
                }
                ENVPTR->DeleteLocalRef(ENVPAR o);
            }
        }
        HDfree((char *)data);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreate
(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref, jint type)
{
    int32 retVal;

    retVal = ANcreate((int32) an_id, (uint16) tag, (uint16) ref, (ann_type) type);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreatef
(JNIEnv *env, jclass clss, jlong an_id, jint type)
{
    int32 retVal;

    retVal = ANcreatef((int32) an_id, (ann_type) type);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANget_1tagref
(JNIEnv *env, jclass clss, jlong an_id, jint index, jint type, jshortArray tagref)
{
    int32 rval = -1;
    short *theArgs;
    jboolean isCopy;

    if (tagref == NULL) {
        h4nullArgument(env, "ANget_tagref:  tagref is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR tagref) < 2) {
        h4badArgument(env, "ANget_tagref:  tagref input array < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetShortArrayElements(ENVPAR tagref, &isCopy);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "ANget_tagref:  tagref not pinned");
        } /* end if */
        else {
            rval = ANget_tagref((int32)an_id, (int32)index, (ann_type)type, (uint16 *)&(theArgs[0]), (uint16 *)&(theArgs[1]));

            if (rval == FAIL) {
                ENVPTR->ReleaseShortArrayElements(ENVPAR tagref, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                ENVPTR->ReleaseShortArrayElements(ENVPAR tagref, theArgs, 0);
            } /* end else */
        } /* end else */
    } /* end else */

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANid2tagref
(JNIEnv *env, jclass clss, jlong an_id, jshortArray tagref)
{
    int32 rval;
    short *theArgs;
    jboolean isCopy;

    if (tagref == NULL) {
        h4nullArgument(env, "ANid2tagref:  tagref is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR tagref) < 2) {
        h4badArgument(env, "ANid2tagref:  tagref input array < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetShortArrayElements(ENVPAR tagref, &isCopy);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "ANid2tagref:  tagref not pinned");
        } /* end if */
        else {
            rval = ANid2tagref((int32)an_id, (uint16 *)&(theArgs[0]), (uint16 *)&(theArgs[1]));

            if (rval == FAIL) {
                ENVPTR->ReleaseShortArrayElements(ENVPAR tagref, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseShortArrayElements(ENVPAR tagref, theArgs, 0);
            } /* end else */
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANtagref2id
(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref)
{
    int32 retVal;

    retVal = ANtagref2id((int32)an_id, (uint16)tag, (uint16)ref);
    if (retVal < 0)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANwriteann
(JNIEnv *env, jclass clss, jlong ann_id, jstring label, jint ann_length)
{

    intn         rval;
    const char *str;

    PIN_JAVA_STRING(label, str);

    if (str != NULL) {
        /* should check that str is as long as ann_length.... */

        rval = ANwriteann((int32)ann_id, str, (int32)ann_length);

        UNPIN_JAVA_STRING(label, str);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}


#ifdef __cplusplus
}
#endif
