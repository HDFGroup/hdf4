
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
#include "hfile.h"
#include "jni.h"
#include "h4jni.h"

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hopen
 * Signature: (Ljava/lang/String;II)J
 */
JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Hopen
(JNIEnv *env, jclass clss, jstring hdfFile, jint access, jint ndds)
{
    const char *file;
    int32  retVal;
    jclass jc;

    PIN_JAVA_STRING(hdfFile, file);
    if (file != NULL) {
        /* open HDF file specified by hdf_HDF_file */
        retVal = Hopen(file, (intn)access, (int16)ndds);

        UNPIN_JAVA_STRING(hdfFile, file);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }
    return (jlong)retVal;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hclose
(JNIEnv *env, jclass clss, jlong fid)
{
    intn status = 0;

    if (fid < 0) {
        /* maybe not an exception -- the file is already closed? */
        return JNI_FALSE;
    }
    /* close the HDF file */
    status = Hclose((int32)fid);
    if (status == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDdont_atexit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HDdont_1atexit
(JNIEnv *env, jclass clss)
{
    intn ret_value = SUCCEED;
    ret_value = HDdont_atexit();
    return (jint)ret_value;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hishdf
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hishdf
(JNIEnv *env, jclass clss, jstring hdfFile)
{

    const char *hfile;
    intn  retVal;

    PIN_JAVA_STRING(hdfFile, hfile);
    if (hfile != NULL) {
        /* open HDF file specified by hdf_HDF_file */
        retVal = Hishdf(hfile);

        UNPIN_JAVA_STRING(hdfFile, hfile);

        if (retVal == FALSE)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hnumber
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Hnumber
(JNIEnv *env, jclass clss, jlong fid, jint tagtype)
{
    int32  retVal;

    retVal = Hnumber((int32)fid, (uint16)tagtype);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    DFKNTsize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_DFKNTsize
(JNIEnv *env, jclass clss, jlong numbertype)
{
    int  retVal;

    retVal = DFKNTsize((int32)numbertype);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hcache
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hcache
(JNIEnv *env, jclass clss, jlong file_id, jint cache_switch)
{

    intn rval;
    rval =  Hcache((int32)file_id, (intn)cache_switch);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetfileversion
 * Signature: (J[I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hgetfileversion
(JNIEnv *env, jclass clss, jlong file_id, jintArray vers, jobjectArray string)
{
    intn rval;
    jclass Sjc;
    char s[LIBVSTR_LEN+1];
    jstring name;
    jint *theArgs;
    jboolean bb;
    jobject o;

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR vers, &bb);

    rval = Hgetfileversion((int32) file_id, (uint32 *)&(theArgs[0]),
        (uint32 *)&(theArgs[1]), (uint32 *)&(theArgs[2]), s);
    s[LIBVSTR_LEN] = '\0';

    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR vers, theArgs, JNI_ABORT);
        CALL_ERROR_CHECK();
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR vers, theArgs, 0);
        o = ENVPTR->GetObjectArrayElement(ENVPAR string, 0);
        if (o == NULL) {
            CALL_ERROR_CHECK();
        }
        else {
            Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
            if (Sjc == NULL) {
                CALL_ERROR_CHECK();
            }
            else  if (ENVPTR->IsInstanceOf(ENVPAR o, Sjc) == JNI_FALSE) {
                CALL_ERROR_CHECK();
            }
            else {
                name = ENVPTR->NewStringUTF(ENVPAR s);
                if (name != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR string, 0, (jobject)name);
                }
            }
            ENVPTR->DeleteLocalRef(ENVPAR o);
        }
    }
    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hgetlibversion
 * Signature: ([I[Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hgetlibversion
(JNIEnv *env, jclass clss, jintArray vers, jobjectArray string)
{
    intn rval;
    jclass Sjc;
    char s[LIBVSTR_LEN+1] ;
    jint *theArgs;
    jstring name;
    jobject o;
    jboolean bb;

    s[LIBVSTR_LEN] = '\0';
    if (string == NULL) {
        h4nullArgument(env, "Hgetlibversion:  string is NULL");
    } /* end if */
    else if (vers == NULL) {
        h4nullArgument(env, "Hgetlibversion:  vers is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR vers) < 3) {
        h4badArgument(env, "Hgetlibversion:  vers input array < order 3");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR vers, &bb);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "Hgetlibversion:  vers not pinned");
        } /* end if */
        else {
            rval = Hgetlibversion((uint32 *)&(theArgs[0]),
                (uint32 *)&(theArgs[1]), (uint32 *)&(theArgs[2]), s);

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR vers, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR vers, theArgs, 0);
                Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (Sjc == NULL) {
                    CALL_ERROR_CHECK();
                }
                else {
                    o = ENVPTR->GetObjectArrayElement(ENVPAR string, 0);
                    if (o == NULL) {
                        CALL_ERROR_CHECK();
                    }
                    else {
                        bb = ENVPTR->IsInstanceOf(ENVPAR o, Sjc);
                        if (bb == JNI_FALSE) {
                            CALL_ERROR_CHECK();
                        }
                        else {
                            ENVPTR->DeleteLocalRef(ENVPAR o);
                            name = ENVPTR->NewStringUTF(ENVPAR s);
                            if (name != NULL) {
                                ENVPTR->SetObjectArrayElement(ENVPAR string, 0, (jobject)name);
                            }
                        }
                    }
                }
            }
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}


/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsetaccesstype
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hsetaccesstype
(JNIEnv *env, jclass clss, jlong h_id, jint  access_type)
{
    intn rval;

    rval = Hsetaccesstype((int32)h_id, (uintn)access_type);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    Hsync
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Hsync
(JNIEnv *env, jclass clss, jlong file_id)
{
    intn rval;

    rval = Hsync((int32)file_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFclose
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HDFclose
(JNIEnv *env, jclass clss, jlong file_id)
{
    intn rval;

    rval = Hclose((int32)file_id);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}


/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFopen
 * Signature: (Ljava/lang/String;IS)I
 */
JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HDFopen
(JNIEnv *env, jclass clss, jstring filename, jint access, jshort n_dds)
{
    int32 rval;
    const char *str;

    PIN_JAVA_STRING(filename, str);
    if (str != NULL) {
        rval = HDFopen((char *)str, (intn)access, (int16)n_dds);

        UNPIN_JAVA_STRING(filename, str);
        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }
    return rval;
}


#ifdef not_yet_implemented
/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDFflusdd
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HDFflusdd
(JNIEnv *env, jclass clss, jlong file_id)
{
    intn rval;

    rval = Hflushdd((int32)file_id);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}
#endif

/*
 * Class:     hdf_hdflib_HDFLibrary
 * Method:    HDgetNTdesc
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_hdf_hdflib_HDFLibrary_HDgetNTdesc
(JNIEnv *env, jclass clss, jint nt)
{
    char *rval;
    jstring rstring;

    rval = HDgetNTdesc((int32)nt);

    if (rval != NULL) {
        rstring = ENVPTR->NewStringUTF(ENVPAR  rval);
        HDfree(rval);
    }
    else
        rstring = NULL;

    return rstring;
}

#ifdef __cplusplus
}
#endif
