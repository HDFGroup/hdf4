
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

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPaddpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn rval;
    char * f;
    jbyte *dat;
    jboolean bb;

    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR palette,&bb);

    rval = DFPaddpal((char *)f, (VOIDP) dat);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    ENVPTR->ReleaseByteArrayElements(ENVPAR palette,dat,JNI_ABORT);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPgetpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn rval;
    char * f;
    jbyte *dat;
    jboolean bb;

    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename, 0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);

    rval = DFPgetpal((char *)f, (VOIDP) dat);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, 0);
        return JNI_TRUE;
    }
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFPlastref
(JNIEnv *env, jclass clss)
{
    return (DFPlastref());
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFPnpals
(JNIEnv *env, jclass clss, jstring filename)
{
    intn rval;
    char * f;
    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    rval = DFPnpals((char *)f);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPputpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette, jint overwrite, jstring filemode)
{
    intn rval;
    char * f;
    char * m;
    jbyte *dat;
    jboolean bb;

    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    m = (char *) ENVPTR->GetStringUTFChars(ENVPAR filemode,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR palette,&bb);

    rval = DFPputpal ((char *)f, (VOIDP) dat, (intn) overwrite, (char *)m);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    ENVPTR->ReleaseStringUTFChars(ENVPAR filemode,m);
    ENVPTR->ReleaseByteArrayElements(ENVPAR palette,dat,JNI_ABORT);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPreadref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    intn rval;
    char * f;
    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    rval = DFPreadref((char *)f, (uint16) ref);
    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFPrestart
(JNIEnv *env, jclass clss)
{
    return (DFPrestart());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPwriteref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    intn rval;
    char * f;
    f = (char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    rval = DFPwriteref((char *)f, (uint16) ref);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);

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
