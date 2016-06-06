
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
#include "h4jni.h"

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPaddpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn         rval;
    const char  *f;
    jbyte       *dat;
    jboolean     bb;

    if (palette == NULL) {
        h4nullArgument(env, "DFPaddpal:  palette is NULL");
    } /* end if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);
        if (dat == NULL) {
            h4JNIFatalError(env, "DFPaddpal:  palette not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f, JNI_FALSE);

            rval = DFPaddpal(f, (VOIDP)dat);

            UNPIN_JAVA_STRING(filename, f);

            ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, JNI_ABORT);
        } /* end else */
    } /* end else */

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPgetpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette)
{
    intn         rval;
    const char  *f;
    jbyte       *dat;
    jboolean     bb;

    if (palette == NULL) {
        h4nullArgument(env, "DFPgetpal:  palette is NULL");
    } /* end if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);
        if (dat == NULL) {
            h4JNIFatalError(env, "DFPgetpal:  palette not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f, JNI_FALSE);

            rval = DFPgetpal(f, (VOIDP)dat);

            UNPIN_JAVA_STRING(filename, f);

            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, JNI_ABORT);
                CALL_ERROR_CHECK(JNI_FALSE);
            }
            ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, 0);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
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
    intn         rval;
    const char  *f;

    PIN_JAVA_STRING(filename, f, -1);

    rval = DFPnpals(f);

    UNPIN_JAVA_STRING(filename, f);

    if (rval == FAIL)
        CALL_ERROR_CHECK(-1);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPputpal
(JNIEnv *env, jclass clss, jstring filename, jbyteArray palette, jint overwrite, jstring filemode)
{
    intn         rval;
    const char  *f;
    const char  *m;
    jbyte       *dat;
    jboolean     bb;

    if (palette == NULL) {
        h4nullArgument(env, "DFPputpal:  palette is NULL");
    } /* end if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);
        if (dat == NULL) {
            h4JNIFatalError(env, "DFPputpal:  palette not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING_TWO(filename, f, filemode, m, JNI_FALSE);

            dat = ENVPTR->GetByteArrayElements(ENVPAR palette, &bb);

            rval = DFPputpal (f, (VOIDP)dat, (intn)overwrite, m);

            UNPIN_JAVA_STRING_TWO(filename, f, filemode, m);

            ENVPTR->ReleaseByteArrayElements(ENVPAR palette, dat, JNI_ABORT);
        } /* end else */
    } /* end else */

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFPreadref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    intn         rval;
    const char  *f;

    PIN_JAVA_STRING(filename, f, JNI_FALSE);

    rval = DFPreadref((char *)f, (uint16) ref);

    UNPIN_JAVA_STRING(filename, f);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
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
    intn         rval;
    const char  *f;

    PIN_JAVA_STRING(filename, f, JNI_FALSE);

    rval = DFPwriteref((char *)f, (uint16) ref);

    UNPIN_JAVA_STRING(filename, f);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}


#ifdef __cplusplus
}
#endif
