
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

extern jboolean getOldCompInfo( JNIEnv *env, jobject ciobj, comp_info *cinf);

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getdims(JNIEnv *env, jclass clss, jstring filename, jintArray argv, jbooleanArray isp)
{
    intn rval;
    char  *hdf_file;
    int   ispal;
    jint *theArgs;
    jboolean *theB;
    jboolean bb;

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);
    theB = ENVPTR->GetBooleanArrayElements(ENVPAR isp,&bb);
    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    /* get image dimension information */
    rval = DFR8getdims(hdf_file, (int32 *)&(theArgs[0]),
        (int32 *)&(theArgs[1]), (intn *)&ispal);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,hdf_file);
    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        theB[0] = JNI_FALSE;
        ENVPTR->ReleaseBooleanArrayElements(ENVPAR isp,theB,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);
        if (ispal) {
            theB[0] = JNI_TRUE;
        }
        else {
            theB[0] = JNI_FALSE;
        }
        ENVPTR->ReleaseBooleanArrayElements(ENVPAR isp,theB,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getimage(JNIEnv *env, jclass clss, jstring filename,
        jbyteArray image, jint width, jint height, jbyteArray pallete)
{
    char  *hdf_file;
    intn   rval;
    jbyte *dat;
    jbyte *p;
    jboolean bb;

    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR image,&bb);
    if (pallete == NULL) {
        rval =  DFR8getimage((char *)hdf_file, (uint8 *) dat, (int32) width, (int32) height,
            (uint8 *)NULL);
    }
    else {
        p = ENVPTR->GetByteArrayElements(ENVPAR pallete,&bb);
        rval =  DFR8getimage((char *)hdf_file, (uint8 *) dat, (int32) width, (int32) height,
            (uint8 *)p);
    }

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,hdf_file);
    if (rval == FAIL) {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,JNI_ABORT);
        if (pallete != NULL) {
            ENVPTR->ReleaseByteArrayElements(ENVPAR pallete,p,JNI_ABORT);
        }
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,0);
        if (pallete != NULL) {
            ENVPTR->ReleaseByteArrayElements(ENVPAR pallete,p,0);
        }
        return JNI_TRUE;
    }

}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8lastref(JNIEnv *env, jclass clss)
{
    return ((short)DFR8lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8readref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    char *filePtr;
    filePtr =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    retVal = DFR8readref(filePtr, (short)ref);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,filePtr);
    if (retVal == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8restart(JNIEnv *env, jclass clss)
{
    int retVal;
    retVal = DFR8restart();

    if (retVal) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8nimages(JNIEnv *env, jclass clss, jstring hdfFile)
{
    char  *hdf_file;

    hdf_file =(char *) ENVPTR->GetStringUTFChars(ENVPAR hdfFile,0);
    return(DFR8nimages(hdf_file));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8addimage(JNIEnv *env, jclass clss, jstring filename,
        jbyteArray image, jint width, jint height, jshort compress)
{
    intn rval;
    char  *f;
    jbyte *dat;
    jboolean bb;

    f =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

    rval = DFR8addimage((char *)f, (VOIDP) dat, (int32) width, (int32) height,
        (uint16)compress);

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
Java_hdf_hdflib_HDFLibrary_DFR8putimage(JNIEnv *env, jclass clss, jstring filename,
        jbyteArray image, jint width, jint height, jint compress)
{
    intn rval;
    char  *f;
    jbyte *dat;
    jboolean bb;

    f =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);
    dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

    rval = DFR8putimage((char *)f, (VOIDP) dat, (int32) width, (int32) height,
        (uint16)compress);

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
Java_hdf_hdflib_HDFLibrary_DFR8setcompress(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn rval;
    comp_info cinf;
    jboolean bval;

    bval = getOldCompInfo(env, cinfo,&cinf);

    /* check for success... */

    /* fill in cinf depending on the value of 'type' */
    rval = DFR8setcompress((int32) type, (comp_info *)&cinf);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getpalref(JNIEnv *env, jclass clss, jshortArray palref)
{
    int rval;
    short *theArgs;
        jboolean bb;

        theArgs = ENVPTR->GetShortArrayElements(ENVPAR palref,&bb);

    rval = DFR8getpalref((uint16 *)&(theArgs[0]));

    if (rval == FAIL) {
        ENVPTR->ReleaseShortArrayElements(ENVPAR palref,theArgs,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseShortArrayElements(ENVPAR palref,theArgs,0);
        return JNI_TRUE;
    }
}



JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8setpalette(JNIEnv *env, jclass clss, jbyteArray palette)
{
    int  rval;
    jbyte *p;
    jboolean bb;

    p = ENVPTR->GetByteArrayElements(ENVPAR palette,&bb);

    rval = DFR8setpalette((uint8 *)p);

    ENVPTR->ReleaseByteArrayElements(ENVPAR palette,p,JNI_ABORT);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8writeref(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    char *filePtr;
    filePtr =(char *) ENVPTR->GetStringUTFChars(ENVPAR filename,0);

    retVal = DFR8writeref(filePtr, (short)ref);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,filePtr);
    if (retVal == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

#ifdef __cplusplus
}
#endif
