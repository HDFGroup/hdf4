
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
Java_hdf_hdflib_HDFLibrary_DF24getdims
(JNIEnv *env, jclass clss, jstring filename, jintArray argv)
{
    intn rval;

    const char  *hdf_file;
    int copyMode;
    jint *theArgs;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (argv == NULL) {
        h4nullArgument(env, "DF24getdims: output array argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 3) {
        h4badArgument(env, "DF24getdims: output array argv < order 3");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "DF24getdims: argv not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, hdf_file);

            if (hdf_file != NULL) {
                /* get image dimension information */
                rval = DF24getdims(hdf_file, (int32 *)&(theArgs[0]),
                        (int32 *)&(theArgs[1]), (intn *)&(theArgs[2]));

                UNPIN_JAVA_STRING(filename, hdf_file);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                } else {
                    copyMode = 0;
                }
            }

            ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,copyMode);
        }/* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24getimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height)
{
    const char  *hdf_file;
    int copyMode;
    intn   rval;
    jbyte *dat;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (image == NULL) {
        h4nullArgument(env, "DF24getimage: output array image is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR image) < 1) {
        h4badArgument(env, "DF24getimage: output array image invalid size");
    } /* end else if */
    else {
        dat = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DF24getimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, hdf_file);

            if (hdf_file != NULL) {
                rval =  DF24getimage(hdf_file, (VOIDP) dat, (int32) width, (int32) height);

                UNPIN_JAVA_STRING(filename, hdf_file);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                } else {
                    copyMode = 0;
                }
            }

            ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,copyMode);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DF24lastref
(JNIEnv *env, jclass clss)
{
    return ((short)DF24lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24readref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    const char *filePtr;

    PIN_JAVA_STRING(filename, filePtr);

    if (filePtr != NULL) {
        retVal = DF24readref(filePtr, (short)ref);

        UNPIN_JAVA_STRING(filename, filePtr);

        if (retVal == FAIL) {
            CALL_ERROR_CHECK();
        }
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24restart
(JNIEnv *env, jclass clss)
{
    int retVal;
    retVal = DF24restart();

    if (retVal) {
        CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DF24nimages
(JNIEnv *env, jclass clss, jstring hdfFile)
{
    const char  *hdf_file;
    intn retVal = -1;

    PIN_JAVA_STRING(hdfFile, hdf_file);

    if (hdf_file != NULL) {
        retVal = DF24nimages(hdf_file);

        UNPIN_JAVA_STRING(hdfFile, hdf_file);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24reqil
(JNIEnv *env, jclass clss, jint interlace)
{
    intn retVal;

    retVal = DF24reqil((intn)interlace);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24addimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height)
{
    intn rval;
    const char  *f;
    int copyMode;
    jbyte *dat;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (image == NULL) {
        h4nullArgument(env, "DF24addimage: image is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR image) < 1) {
        h4badArgument(env, "DF24addimage: no image data");
    } /* end else if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DF24addimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f);

            if (f != NULL) {
                rval = DF24addimage((char *)f, (VOIDP) dat, (int32) width, (int32) height);

                UNPIN_JAVA_STRING(filename, f);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                } else {
                    copyMode = 0;
                }
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,copyMode);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24putimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height)
{
    intn rval;
    const char  *f;
    int copyMode;
    jbyte *dat;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (image == NULL) {
        h4nullArgument(env, "DF24putimage: image is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR image) < 1) {
        h4badArgument(env, "DF24putimage: no image data");
    } /* end else if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DF24putimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f);

            if (f != NULL) {
                rval = DF24putimage(f, (VOIDP) dat, (int32) width, (int32) height);

                UNPIN_JAVA_STRING(filename, f);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                } else {
                    copyMode = 0;
                }
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,copyMode);
        }
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setcompress
(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn rval;
    comp_info cinf;
    jboolean bval;

    if (cinfo == NULL) {
        h4nullArgument(env, "DF24setcompress: cinfo is NULL");
    } /* end if */
    else {
        bval = getOldCompInfo(env, cinfo,&cinf);

        if (bval == JNI_FALSE) {
            h4JNIFatalError(env, "DF24setcompress: cinfo not pinned");
        } /* end if */
        else {
            /* fill in cinf depending on the value of 'type' */
            rval = DF24setcompress((int32) type, (comp_info *)&cinf);

            if (rval == FAIL) {
                CALL_ERROR_CHECK();
            }
        }
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setdims
(JNIEnv *env, jclass clss, jint width, jint height)
{
    intn rval;

    rval = DF24setdims((int32) width, (int32) height);

    if (rval == FAIL) {
        CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DF24setil
(JNIEnv *env, jclass clss, jint il)
{
    intn rval;
    rval = DF24setil((intn) il);
    if (rval == FAIL) {
        CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
