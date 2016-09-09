
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
Java_hdf_hdflib_HDFLibrary_DFR8getdims
(JNIEnv *env, jclass clss, jstring filename, jintArray argv, jbooleanArray isp)
{
    intn rval;
    const char  *hdf_file;
    int   ispal;
    int copyMode;
    jint *theArgs;
    jboolean *theB;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (argv == NULL) {
        h4nullArgument(env, "DFR8getdims: output array argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 2) {
        h4badArgument(env, "DFR8getdims: argv output array < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "DFR8getdims: argv not pinned");
        } /* end if */
        else {
            if (isp == NULL) {
                h4nullArgument(env, "DFR8getdims: output array isp is NULL");
            } /* end if */
            else if (ENVPTR->GetArrayLength(ENVPAR isp) < 1) {
                h4badArgument(env, "DFR8getdims: output array isp < order 1");
            } /* end else if */
            else {
                theB = ENVPTR->GetBooleanArrayElements(ENVPAR isp,&bb);

                if (theB == NULL) {
                    h4JNIFatalError(env, "DFR8getdims: isp not pinned");
                } /* end if */
                else {
                    PIN_JAVA_STRING(filename, hdf_file);

                    if (hdf_file != NULL) {
                        /* get image dimension information */
                        rval = DFR8getdims(hdf_file, (int32 *)&(theArgs[0]),
                                (int32 *)&(theArgs[1]), (intn *)&ispal);

                        UNPIN_JAVA_STRING(filename, hdf_file);

                        if (rval == FAIL) {
                            theB[0] = JNI_FALSE;

                            CALL_ERROR_CHECK();
                        }
                        else {
                            copyMode = 0;

                            if (ispal) {
                                theB[0] = JNI_TRUE;
                            }
                            else {
                                theB[0] = JNI_FALSE;
                            }
                        } /* end else */
                    }

                    ENVPTR->ReleaseBooleanArrayElements(ENVPAR isp,theB,copyMode);
                } /* end else */
            } /* end else */

            ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,copyMode);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height, jbyteArray pallete)
{
    const char  *hdf_file;
    int copyMode;
    intn   rval;
    jbyte *dat;
    jbyte *p;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (image == NULL) {
        h4nullArgument(env, "DFR8getimage: output array image is NULL");
    } /* end if */
    else {
        dat = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DFR8getimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, hdf_file);

            if (hdf_file != NULL) {
                if (pallete == NULL) {
                    rval =  DFR8getimage((char *)hdf_file, (uint8 *) dat, (int32) width, (int32) height,
                            (uint8 *)NULL);
                }
                else {
                    p = ENVPTR->GetByteArrayElements(ENVPAR pallete,&bb);
                    rval =  DFR8getimage((char *)hdf_file, (uint8 *) dat, (int32) width, (int32) height,
                            (uint8 *)p);
                }

                UNPIN_JAVA_STRING(filename, hdf_file);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
                else {
                    copyMode = 0;
                } /* end else */

                ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR image,dat,copyMode);
                if (pallete != NULL) {
                    ENVPTR->ReleaseByteArrayElements(ENVPAR pallete,p,copyMode);
                }
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8lastref
(JNIEnv *env, jclass clss)
{
    return ((short)DFR8lastref());
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8readref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    const char *filePtr;

    PIN_JAVA_STRING(filename, filePtr);

    if (filePtr != NULL) {
        retVal = DFR8readref(filePtr, (short)ref);

        UNPIN_JAVA_STRING(filename, filePtr);

        if (retVal == FAIL) {
            CALL_ERROR_CHECK();
        }
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8restart
(JNIEnv *env, jclass clss)
{
    int retVal;
    retVal = DFR8restart();

    if (retVal) {
        CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8nimages
(JNIEnv *env, jclass clss, jstring hdfFile)
{
    const char  *hdf_file;
    int retVal = -1;

    PIN_JAVA_STRING(hdfFile, hdf_file);

    if (hdf_file != NULL) {
        retVal = DFR8nimages(hdf_file);

        UNPIN_JAVA_STRING(hdfFile, hdf_file);

        if (retVal == FAIL) {
            CALL_ERROR_CHECK();
        }
    }

    return(retVal);
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8addimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height, jshort compress)
{
    intn rval;
    const char  *f;
    jbyte *dat;
    jboolean bb;

    if (image == NULL) {
        h4nullArgument(env, "DFR8addimage: image is NULL");
    } /* end if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DFR8addimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f);

            if (f != NULL) {
                rval = DFR8addimage((char *)f, (VOIDP) dat, (int32) width, (int32) height,
                        (uint16)compress);

                UNPIN_JAVA_STRING(filename, f);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,JNI_ABORT);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8putimage
(JNIEnv *env, jclass clss, jstring filename, jbyteArray image, jint width, jint height, jint compress)
{
    intn rval;
    const char  *f;
    jbyte *dat;
    jboolean bb;

    if (image == NULL) {
        h4nullArgument(env, "DFR8putimage: image is NULL");
    } /* end if */
    else {
        dat = ENVPTR->GetByteArrayElements(ENVPAR image,&bb);

        if (dat == NULL) {
            h4JNIFatalError(env, "DFR8putimage: image not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(filename, f);

            if (f != NULL) {
                rval = DFR8putimage((char *)f, (VOIDP) dat, (int32) width, (int32) height,
                        (uint16) compress);

                UNPIN_JAVA_STRING(filename, f);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR image,dat,JNI_ABORT);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8setcompress
(JNIEnv *env, jclass clss, jint type, jobject cinfo)
{
    intn rval;
    comp_info cinf;

    if (cinfo == NULL) {
        h4nullArgument(env, "DFR8setcompress: cinfo is NULL");
    } /* end if */
    else {
        if (getOldCompInfo(env, cinfo,&cinf) == JNI_FALSE) {
            h4JNIFatalError(env, "DFR8setcompress: cinfo not pinned");
        } /* end if */
        else {
            /* fill in cinf depending on the value of 'type' */
            rval = DFR8setcompress((int32) type, (comp_info *)&cinf);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8getpalref
(JNIEnv *env, jclass clss, jshortArray palref)
{
    int rval;
    short *theArgs;
    jboolean bb;

    if (palref == NULL) {
        h4nullArgument(env, "DFR8getpalref: output array palref is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR palref) < 1) {
        h4badArgument(env, "DFR8getpalref: output array palref < order 1");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetShortArrayElements(ENVPAR palref,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "DFR8getpalref: palref not pinned");
        } /* end if */
        else {
            rval = DFR8getpalref((uint16 *)&(theArgs[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseShortArrayElements(ENVPAR palref,theArgs,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseShortArrayElements(ENVPAR palref,theArgs,0);
            } /* end else */
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}



JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8setpalette
(JNIEnv *env, jclass clss, jbyteArray palette)
{
    int  rval;
    jbyte *p;
    jboolean bb;

    if (palette == NULL) {
        p = NULL;
    }
    else {
        p = ENVPTR->GetByteArrayElements(ENVPAR palette,&bb);
    }

    rval = DFR8setpalette((uint8 *)p);

    if (p) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR palette,p,JNI_ABORT);
    }

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFR8writeref
(JNIEnv *env, jclass clss, jstring filename, jshort ref)
{
    int  retVal;
    const char *filePtr;

    PIN_JAVA_STRING(filename, filePtr);

    if (filePtr != NULL) {
        retVal = DFR8writeref(filePtr, (short)ref);

        UNPIN_JAVA_STRING(filename, filePtr);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
