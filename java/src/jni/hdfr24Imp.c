
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

extern jboolean
getOldCompInfo
(JNIEnv *env, jobject ciobj, comp_info *cinf);


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24getdims
(JNIEnv *env, jclass clss, jstring filename, jintArray argv)
{
    intn rval;

    char  *hdf_file;
    jint *theArgs;
    jboolean bb;

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);
    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    /* get image dimension information */
    rval = DF24getdims(hdf_file, (int32 *)&(theArgs[0]),
        (int32 *)&(theArgs[1]), (intn *)&(theArgs[2]));

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,hdf_file);
    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24getimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height)
{
    char  *hdf_file;
    intn   rval;
    jbyte *dat;
    jboolean bb;

    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR image,&bb);

    rval =  DF24getimage((char *)hdf_file, (VOIDP) dat, (int32) width, (int32) height);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,hdf_file);
    if (rval == FAIL) {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,0);
        return JNI_TRUE;
    }

}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DF24lastref
(JNIEnv *env, jclass clss)
{
    return ((short)DF24lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24readref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    char *filePtr;
    filePtr =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    retVal = DF24readref(filePtr, (short)ref);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,filePtr);
    if (retVal == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24restart
(JNIEnv *env, jclass clss)
{
    int retVal;
    retVal = DF24restart();

    if (retVal) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24nimages
(JNIEnv *env, jclass clss, jstring hdfFile)
{
    char  *hdf_file;

    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR hdfFile,0);
    return(DF24nimages(hdf_file));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24reqil
(JNIEnv *env, jclass clss, jint interlace)
{
    return(DF24reqil((intn)interlace));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24addimage
(JNIEnv *env, jclass clss, jstring filename,
        jbyteArray image, jint width, jint height)
{
    intn rval;
    char  *f;
    jbyte *dat;
    jboolean bb;

    f =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

    rval = DF24addimage((char *)f, (VOIDP) dat, (int32) width, (int32) height);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,JNI_ABORT);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }

}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_1DF24putimage
(JNIEnv *env, jclass clss, jstring filename,
        jbyteArray image, jint width, jint height)
{
    intn rval;
    char  *f;
    jbyte *dat;
    jboolean bb;

    f =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

    rval = DF24putimage((char *)f, (VOIDP) dat, (int32) width, (int32) height);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,f);
    ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,JNI_ABORT);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }

}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setcompress
(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn rval;
    comp_info cinf;
    jboolean bval;

    bval = getOldCompInfo(env, cinfo,&cinf);

    /* check fo rsuccess... */

    /* fill in cinf depending on the value of 'type' */
    rval = DF24setcompress((int32) type, (comp_info *)&cinf);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setdims
(JNIEnv *env, jclass clss, jint width, jint height)
{
    intn rval;

    rval = DF24setdims((int32) width, (int32) height);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setil
(JNIEnv *env, jclass clss, jint il)
{
    intn rval;
    rval = DF24setil((intn) il);
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
