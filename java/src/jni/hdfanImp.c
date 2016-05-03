
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
 *  HDF 4.1 library.
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
    return (jlong)ANstart((int32)file_id);
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANend
(JNIEnv *env, jclass clss, jlong anid)
{
    int32 retVal;

    retVal = ANend((int32)anid);

    if (retVal == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANfileinfo
(JNIEnv *env, jclass clss, jlong anid, jintArray info)
{
    int32 retVal;
    jint *theArgs;
    jboolean bb;

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR info,&bb);

    retVal = ANfileinfo((int32)anid, (int32 *)&(theArgs[0]),
        (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]),
        (int32 *)&(theArgs[3]));

    if (retVal == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR info,theArgs,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR info,theArgs,0);
        return JNI_TRUE;
    }
}


JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANselect
(JNIEnv *env, jclass clss, jlong anid, jint index, jint anntype)
{
    return (jlong)ANselect((int32)anid, (int32)index, (ann_type)anntype);
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANendaccess
(JNIEnv *env, jclass clss, jlong ann_id)
{
    int32 retVal;

    retVal = ANendaccess((int32)ann_id);

    if (!retVal) {
        return JNI_TRUE;
    }
    else {
        return JNI_FALSE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANnumann
(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag, jshort ref)
{
    return( (jint) ANnumann((int32)an_id, (ann_type)anntype, (uint16)tag, (uint16)ref) );
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_ANatype2tag
(JNIEnv *env, jclass clss, jint antype)
{
    return (jshort)ANatype2tag((ann_type)antype);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANtag2atype
(JNIEnv *env, jclass clss, jint antag)
{
    return (jint)ANtag2atype((uint16)antag);
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlist
(JNIEnv *env, jclass clss, jlong an_id, jint anntype, jshort tag, jshort ref, jintArray annlist)
{
    intn retVal;
    jint *iarr;
    jboolean bb;

    iarr = ENVPTR->GetIntArrayElements(ENVPAR annlist,&bb);

    retVal = ANannlist((int32)an_id, (ann_type)anntype,
        (uint16)tag,(uint16)ref,(int32 *)iarr);

    if (retVal == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR annlist,iarr,JNI_ABORT);
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR annlist,iarr,0);
    }
    return (jint)retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANannlen
(JNIEnv *env, jclass clss, jlong ann_id)
{
    return ANannlen((int32)ann_id);
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
        return JNI_FALSE;
    }

    /* read annotation from HDF */
    retVal = ANreadann((int32)ann_id, data, (int32)maxlen);
    data[maxlen] = '\0';

    if (retVal == FAIL) {
        if (data != NULL) HDfree((char *)data);
        return JNI_FALSE;
    }
    else {

        o = ENVPTR->GetObjectArrayElement(ENVPAR annbuf,0);
        if (o == NULL) {
            if (data != NULL) HDfree((char *)data);
            return JNI_FALSE;
        }
        Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
        if (Sjc == NULL) {
            if (data != NULL) HDfree((char *)data);
            return JNI_FALSE;
        }
        bb = ENVPTR->IsInstanceOf(ENVPAR o,Sjc);
        if (bb == JNI_FALSE) {
            if (data != NULL) HDfree((char *)data);
            return JNI_FALSE;
        }

        rstring = ENVPTR->NewStringUTF(ENVPAR  data);
        ENVPTR->SetObjectArrayElement(ENVPAR annbuf,0,(jobject)rstring);
        ENVPTR->DeleteLocalRef(ENVPAR o);

        if (data != NULL)
            HDfree((char *)data);

        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreate
(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref, jint type)
{
    return (jlong)ANcreate((int32) an_id, (uint16) tag, (uint16) ref, (ann_type) type);
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANcreatef
(JNIEnv *env, jclass clss, jlong an_id, jint type)
{
    return (jlong)ANcreatef((int32) an_id, (ann_type) type);
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_ANget_1tagref
(JNIEnv *env, jclass clss, jlong an_id, jint index, jint type, jshortArray tagref)
{
    int32 rval;
    short *theArgs;
    jboolean bb;

    theArgs = ENVPTR->GetShortArrayElements(ENVPAR tagref,&bb);

    rval = ANget_tagref((int32) an_id, (int32) index,  (ann_type) type, (uint16 *)&(theArgs[0]), (uint16 *)&(theArgs[1]));

    if (rval == FAIL) {
        ENVPTR->ReleaseShortArrayElements(ENVPAR tagref,theArgs,JNI_ABORT);
    }
    else {
        ENVPTR->ReleaseShortArrayElements(ENVPAR tagref,theArgs,0);
    }
    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANid2tagref
(JNIEnv *env, jclass clss, jlong an_id, jshortArray tagref)
{
    int32 rval;
    short *theArgs;
    jboolean bb;

    theArgs = ENVPTR->GetShortArrayElements(ENVPAR tagref,&bb);

    rval =  ANid2tagref((int32) an_id, (uint16 *)&(theArgs[0]),
        (uint16 *)&(theArgs[1]));

    if (rval == FAIL) {
        ENVPTR->ReleaseShortArrayElements(ENVPAR tagref,theArgs,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseShortArrayElements(ENVPAR tagref,theArgs,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_ANtagref2id
(JNIEnv *env, jclass clss, jlong an_id, jshort tag, jshort ref)
{
    return (jlong) ANtagref2id((int32) an_id, (uint16) tag, (uint16) ref);
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_ANwriteann
(JNIEnv *env, jclass clss, jlong ann_id, jstring label, jint ann_length)
{

    intn rval;
    char * str;

    str =(char *) ENVPTR->GetStringUTFChars(ENVPAR label,0);

    /* should check that str is as long as ann_length.... */

    rval = ANwriteann((int32) ann_id, str, (int32) ann_length);

    ENVPTR->ReleaseStringUTFChars(ENVPAR label,str);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}


#ifdef __cplusplus
}
#endif
