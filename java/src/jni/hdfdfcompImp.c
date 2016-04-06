
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

extern jboolean getOldCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFDeprecated_DFgetcomp(JNIEnv *env, jclass clss, jlong file_id, jshort tag,
        jshort ref, jbyteArray image, jint xdim, jint ydim, jint method)
{
    int rval;
    jbyte *im;
    jboolean bb;

    im = ENVPTR->GetByteArrayElements(ENVPAR image, &bb);

    rval = DFgetcomp((int32) file_id, (uint16) tag, (uint16) ref,
        (uint8 *)image, (int32) xdim, (int32) ydim,  (int16) method);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR image, im, JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR image, im, 0);
        return JNI_TRUE;
    }
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFDeprecated_DFputcomp(JNIEnv *env, jclass clss, jlong file_id, jshort tag,
        jshort ref, jbyteArray image, jint xdim, jint ydim, jbyteArray palette, jbyteArray newpal,
        jint scheme, jobject c_info)
{
    intn rval;
    jboolean bval;
    comp_info cinf;
    jbyte *im;
    jbyte *p;
    jbyte *np;
    jboolean bb;

    bval = getOldCompInfo(env,c_info,&cinf);

    /* check for success... */

    im = ENVPTR->GetByteArrayElements(ENVPAR image, &bb);
    p = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);
    np = ENVPTR->GetByteArrayElements(ENVPAR newpal, &bb);


    rval = DFputcomp((int32) file_id, (uint16) tag, (uint16) ref,
        (uint8 *)im, (int32) xdim, (int32) ydim, (uint8 *)p,
        (uint8 *)np, (int16) scheme, (comp_info *)&cinf);

    ENVPTR->ReleaseByteArrayElements(ENVPAR image, im, JNI_ABORT);
    ENVPTR->ReleaseByteArrayElements(ENVPAR palette, p, JNI_ABORT);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR newpal, np, JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR newpal, np, 0);
        return JNI_TRUE;
    }
}


#ifdef __cplusplus
}
#endif
