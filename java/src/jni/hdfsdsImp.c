
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
#include "mfhdf.h"
#include "jni.h"
#include "h4jni.h"

extern jboolean makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf);
extern jboolean getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);
extern jboolean setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf);
extern jboolean getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf);

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDstart
(JNIEnv *env, jclass clss, jstring filename, jint access)
{
    int32 sdid = -1;
    const char  *fname;

    PIN_JAVA_STRING(filename, fname);
    if (fname != NULL) {
        sdid = SDstart(fname, (int32)access);

        UNPIN_JAVA_STRING(filename, fname);
        if (sdid < 0)
            CALL_ERROR_CHECK();
    }

    return (jlong)sdid;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDend
(JNIEnv *env, jclass clss, jlong sdid)
{
    intn rval;
    int32 id = (int32)sdid;

    rval = SDend(id);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDfileinfo
(JNIEnv *env, jclass clss, jlong sdid, jintArray argv)
{
    intn rval;
    jint *theArgs;
    jboolean isCopy; /*  dummy */
    int32 id = (int32)sdid;

    if (argv == NULL) {
        h4nullArgument(env, "GRfileinfo:  argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 2) {
        h4badArgument(env, "GRfileinfo:  argv input array < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &isCopy);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "GRfileinfo:  argv not pinned");
        } /* end if */
        else {
            rval = SDfileinfo(id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, 0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDselect
(JNIEnv *env, jclass clss, jlong sdid, jint index)
{
    int32 rval;
    int32 id = (int32)sdid;

    rval = (jlong)SDselect(id, (int32) index);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDnametoindex
(JNIEnv *env, jclass clss, jlong sdid, jstring name)
{
    intn rval = -1;
    const char *str;
    int32 id = (int32)sdid;

    PIN_JAVA_STRING(name, str);
    if (str != NULL) {
        rval = SDnametoindex(id, str);

        UNPIN_JAVA_STRING(name, str);
        if (rval < 0)
            CALL_ERROR_CHECK();
    }

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetinfo
(JNIEnv *env, jclass clss, jlong sdsid, jobjectArray name, jintArray dimsizes, jintArray argv)
{
    intn   rval;
    int32 *dims;
    int32 *theArgs;
    jclass jc;
    jstring str;
    jboolean isCopy;
    jboolean bb = 0;
    jboolean abb = 0;
    jobject o;
    char  *cname;
    int32 id = (int32)sdsid;

    /* variables of  infomation */

    cname = (char *)HDmalloc(MAX_NC_NAME+1);
    if (cname == NULL) {
        h4outOfMemory(env, "SDgetinfo");
    }
    else {
        if (name == NULL) {
            h4nullArgument(env, "SDgetinfo:  name is NULL");
        } /* end if */
        else if (dimsizes == NULL) {
            h4nullArgument(env, "SDgetinfo:  dimsizes is NULL");
        } /* end if */
        else if (argv == NULL) {
            h4nullArgument(env, "SDgetinfo:  argv is NULL");
        } /* end if */
        else if (ENVPTR->GetArrayLength(ENVPAR argv) < 3) {
            h4badArgument(env, "SDgetinfo:  argv input array < order 3");
        } /* end else if */
        else {
            dims = (int32 *)ENVPTR->GetIntArrayElements(ENVPAR dimsizes, &isCopy);
            if (dims == NULL) {
                h4JNIFatalError(env, "SDgetinfo:  dimsizes not pinned");
            } /* end if */
            else {
                theArgs = (int32 *)ENVPTR->GetIntArrayElements(ENVPAR argv, &isCopy);
                if (theArgs == NULL) {
                    h4JNIFatalError(env, "SDgetinfo:  argv not pinned");
                } /* end if */
                else {
                    rval = SDgetinfo(id, cname, &(theArgs[0]), dims,
                            &(theArgs[1]), &(theArgs[2]));

                    if (rval == FAIL) {
                        abb = JNI_ABORT;
                        bb = JNI_ABORT;
                        CALL_ERROR_CHECK();
                    }
                    else {
                        cname[MAX_NC_NAME] = '\0';
                        str = ENVPTR->NewStringUTF(ENVPAR  cname);
                        if (str != NULL) {
                            o = ENVPTR->GetObjectArrayElement(ENVPAR name, 0);
                            if (o == NULL) {
                                bb = JNI_ABORT;
                                CALL_ERROR_CHECK();
                            }
                            else {
                                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                                if (jc == NULL) {
                                    bb = JNI_ABORT;
                                    CALL_ERROR_CHECK();
                                }
                                else {
                                    if (ENVPTR->IsInstanceOf(ENVPAR o, jc) == JNI_FALSE) {
                                        bb = JNI_ABORT;
                                        CALL_ERROR_CHECK();
                                    }
                                    ENVPTR->SetObjectArrayElement(ENVPAR name, 0, (jobject)str);
                                }
                                ENVPTR->DeleteLocalRef(ENVPAR o);
                            }
                        }
                    }
                    ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, abb);

                } /* end else */
                ENVPTR->ReleaseIntArrayElements(ENVPAR dimsizes, dims, bb);

            } /* end else */
        } /* end else */

        HDfree(cname);
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jbyteArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jbyte *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

/*
    ////////////////////////////////////////////////////////////////////
    //                                                                //
    //         New APIs for read data from library                    //
    //  Using SDreaddata(..., Object buf) requires function calls     //
    //  theArray.emptyBytes() and theArray.arrayify(buf), which       //
    //  triples the actual memory needed by the data set.             //
    //  Using the following APIs solves the problem.                  //
    //                                                                //
    ////////////////////////////////////////////////////////////////////
*/

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1short
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jshortArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jshort *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jshort *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1int
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jintArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jint *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jint *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1long
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jlongArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jlong *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jlong *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1float
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jfloatArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jfloat *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jfloat *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreaddata_1double
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray count, jdoubleArray data)
{
    intn rval;
    int32 *strt;
    int32 *strd;
    int32 *cnt;
    jdouble *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDreaddata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDreaddata:  start is NULL");
    } /* end if */
    else if (count == NULL) {
        h4nullArgument(env, "SDreaddata:  count is NULL");
    } /* end if */
    else {
        d = (jdouble *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDreaddata:  start not pinned");
        } /* end if */
        else {
            cnt = ENVPTR->GetIntArrayElements(ENVPAR count, &bb);
            if (cnt == NULL) {
                h4JNIFatalError(env, "SDreaddata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDreaddata(id, strt, strd, cnt, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR count, cnt, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDendaccess
(JNIEnv *env, jclass clss, jlong sdsid)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDendaccess(id);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimid
(JNIEnv *env, jclass clss, jlong sdsid, jint index)
{
    int32 rval;
    int32 id = (int32)sdsid;

    rval = SDgetdimid(id, (intn)index);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDdiminfo
(JNIEnv *env, jclass clss, jlong dimid, jobjectArray dimname, jintArray argv)
{
    intn rval;
    jclass jc;
    jstring rstring;
    jint *theArgs;
    jboolean bb;
    jobject o;
    char  str[256];  /* what is the correct constant??? */

    if (dimname == NULL) {
        h4nullArgument(env, "SDdiminfo:  dimname is NULL");
    } /* end if */
    else if (argv == NULL) {
        h4nullArgument(env, "SDdiminfo:  argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 3) {
        h4badArgument(env, "SDdiminfo:  argv input array < order 3");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &bb);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "SDdiminfo:  argv not pinned");
        } /* end if */
        else {
            rval = SDdiminfo((int32)dimid, (char *)str, (int32 *)&(theArgs[0]),
                    (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, 0);
                if (str != NULL) {
                    str[255] = '\0';
                    rstring = ENVPTR->NewStringUTF(ENVPAR  str);
                    if (rstring != NULL) {
                        o = ENVPTR->GetObjectArrayElement(ENVPAR dimname, 0);
                        if (o == NULL) {
                            CALL_ERROR_CHECK();
                        }
                        else {
                            jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                            if (jc == NULL) {
                                CALL_ERROR_CHECK();
                            }
                            else {
                                bb = ENVPTR->IsInstanceOf(ENVPAR o, jc);
                                if (bb == JNI_FALSE) {
                                    CALL_ERROR_CHECK();
                                }
                                else
                                    ENVPTR->SetObjectArrayElement(ENVPAR dimname, 0, (jobject)rstring);
                            }
                        }
                        ENVPTR->DeleteLocalRef(ENVPAR o);
                    }
                    else
                        h4JNIFatalError(env, "SDdiminfo: can not create string");
                }
            }
        } /* end else */
    }
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDidtoref
(JNIEnv *env, jclass clss, jlong sdsid)
{
    int32 rval;
    int32 id = (int32)sdsid;

    rval = SDidtoref(id);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDreftoindex
(JNIEnv *env, jclass clss, jlong sdid, jint ref)
{
    int32 rval;
    int32 id = (int32)sdid;

    rval = SDreftoindex(id, (int32)ref);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDattrinfo
(JNIEnv *env, jclass clss, jlong sdsid, jint index, jobjectArray name, jintArray argv)
{
    intn rval;
    jclass jc;
    jstring rstring;
    jint *theArgs;
    jboolean bb;
    jobject o;
    char  str[256];  /* what is the correct constant??? */
    int32 id = (int32)sdsid;

    if (name == NULL) {
        h4nullArgument(env, "SDattrinfo:  name is NULL");
    } /* end if */
    else if (argv == NULL) {
        h4nullArgument(env, "SDattrinfo:  argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 2) {
        h4badArgument(env, "SDattrinfo:  argv input array < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &bb);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "SDattrinfo:  argv not pinned");
        } /* end if */
        else {
            rval = SDattrinfo(id, (int32)index,
                (char *)str, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, 0);
                if (str != NULL) {
                    str[255] = '\0';
                    rstring = ENVPTR->NewStringUTF(ENVPAR  str);
                    o = ENVPTR->GetObjectArrayElement(ENVPAR name, 0);
                    if (o == NULL) {
                        CALL_ERROR_CHECK();
                    }
                    jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                    if (jc == NULL) {
                        CALL_ERROR_CHECK();
                    }
                    bb = ENVPTR->IsInstanceOf(ENVPAR o, jc);
                    if (bb == JNI_FALSE) {
                        CALL_ERROR_CHECK();
                    }
                    ENVPTR->SetObjectArrayElement(ENVPAR name, 0, (jobject)rstring);
                    ENVPTR->DeleteLocalRef(ENVPAR o);
                }
            }
        } /* end else */
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreadattr
(JNIEnv *env, jclass clss, jlong sdsid, jint index, jbyteArray dat)
{
    intn rval;
    jbyte *arr;
    jboolean bb;
    int32 id = (int32)sdsid;

    if (dat == NULL) {
        h4nullArgument(env, "SDreadattr:  dat is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR dat, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "SDreadattr:  dat not pinned");
        } /* end if */
        else {
            rval = SDreadattr(id, (int32)index,  (VOIDP)arr);
            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR dat, arr, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR dat, arr, 0);
            }
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDfindattr
(JNIEnv *env, jclass clss, jlong sdsid, jstring name)
{
    int32 rval = -1;
    const char *str;
    int32 id = (int32)sdsid;

    PIN_JAVA_STRING(name, str);
    if (str != NULL) {
        rval = GRfindattr(id, str);

        UNPIN_JAVA_STRING(name, str);
        if (rval < 0)
            CALL_ERROR_CHECK();
    }

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDiscoordvar
(JNIEnv *env, jclass clss, jlong sdsid)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDiscoordvar(id);
    if (rval <= 0)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcal
(JNIEnv *env, jclass clss, jlong sdsid, jdoubleArray argv, jintArray nt)
{
    intn rval;
    jdouble *theArgs;
    jint *theNT;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (nt == NULL) {
        h4nullArgument(env, "SDgetcal:  nt is NULL");
    } /* end if */
    else if (argv == NULL) {
        h4nullArgument(env, "SDgetcal:  argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 4) {
        h4badArgument(env, "SDgetcal:  argv input array < order 4");
    } /* end else if */
    else {
        theNT = ENVPTR->GetIntArrayElements(ENVPAR nt, &bb);
        if (theNT == NULL) {
            h4JNIFatalError(env, "SDgetcal:  nt not pinned");
        } /* end if */
        else {
            theArgs = ENVPTR->GetDoubleArrayElements(ENVPAR argv, &bb);
            if (theArgs == NULL) {
                h4JNIFatalError(env, "SDgetcal:  argv not pinned");
            } /* end if */
            else {
                rval = SDgetcal(id, (float64 *)&(theArgs[0]),
                        (float64 *)&(theArgs[1]), (float64 *)&(theArgs[2]),
                        (float64 *)&(theArgs[3]), (int32 *)&(theNT[0]));

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
                ENVPTR->ReleaseDoubleArrayElements(ENVPAR argv, theArgs, cbb);
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR nt, theNT, cbb);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdatastrs
(JNIEnv *env, jclass clss, jlong sdsid, jobjectArray strings, jint len)
{
    intn rval;
    jclass Sjc;
    jstring str;
    jobject o;
    jboolean bb;
    jstring label,unit,format,coordsys;
    char *labVal;
    char *unitVal;
    char *fmtVal;
    char *coordsysVal;
    int i;
    int32 id = (int32)sdsid;

    if (strings == NULL) {
        h4nullArgument(env, "SDgetdatastrs:  strings is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR strings) < 4) {
        h4badArgument(env, "SDgetdatastrs:  strings input array < order 4");
    } /* end else if */
    else {
        label = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 0);
        /* allocate space */
        if (label == NULL) {
            labVal = NULL; /* don't read label */
        }
        else {
            labVal = (char *)HDmalloc(len+1);
            if (labVal == NULL) {
                h4outOfMemory(env, "SDgetdatastrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR label);

        unit = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 1);
        if (unit == NULL) {
            unitVal = NULL;
        }
        else {
            unitVal = (char *)HDmalloc(len+1);
            if (unitVal == NULL) {
                h4outOfMemory(env, "SDgetdatastrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR unit);

        format = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 2);
        if (format == NULL) {
            fmtVal = NULL;
        }
        else {
            fmtVal = (char *)HDmalloc(len+1);
            if (fmtVal == NULL) {
                h4outOfMemory(env, "SDgetdatastrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR format);

        coordsys = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 3);
        if (coordsys == NULL) {
            coordsysVal = NULL;
        }
        else {
            coordsysVal = (char *)HDmalloc(len+1);
            if (coordsysVal == NULL) {
                h4outOfMemory(env, "SDgetdatastrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR coordsys);

        rval = SDgetdatastrs(id, labVal, unitVal, fmtVal, coordsysVal, (int32)len);

        if (rval == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            if (labVal != NULL) {
                labVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR labVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 0, (jobject)str);
                }
            }
            if (unitVal != NULL) {
                unitVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR unitVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 1, (jobject)str);
                }
            }
            if (fmtVal != NULL) {
                fmtVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR fmtVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 2, (jobject)str);
                }
            }
            if (coordsysVal != NULL) {
                coordsysVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR coordsysVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 3, (jobject)str);
                }
            }
        }

        if (labVal != NULL) HDfree(labVal);
        if (unitVal != NULL) HDfree(unitVal);
        if (fmtVal != NULL) HDfree(fmtVal);
        if (coordsysVal != NULL) HDfree(coordsysVal);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimstrs
(JNIEnv *env, jclass clss, jlong dimid, jobjectArray strings, jint len)
{
    intn rval;
    jclass Sjc;
    jstring str;
    jobject o;
    jboolean bb;
    jstring label,unit,format,coordsys;
    char *labVal;
    char *unitVal;
    char *fmtVal;
    int i;

    if (strings == NULL) {
        h4nullArgument(env, "SDgetdimstrs:  strings is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR strings) < 3) {
        h4badArgument(env, "SDgetdimstrs:  strings input array < order 3");
    } /* end else if */
    else {
        label = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 0);
        /* allocate space */
        if (label == NULL) {
            labVal = NULL; /* don't read label */
        }
        else {
            labVal = (char *)HDmalloc(len+1);
            if (labVal == NULL) {
                h4outOfMemory(env, "SDgetdimstrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR label);

        unit = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 1);
        if (unit == NULL) {
            unitVal = NULL;
        }
        else {
            unitVal = (char *)HDmalloc(len+1);
            if (unitVal == NULL) {
                h4outOfMemory(env, "SDgetdimstrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR unit);

        format = (jstring)ENVPTR->GetObjectArrayElement(ENVPAR strings, 2);
        if (format == NULL) {
            fmtVal = NULL;
        }
        else {
            fmtVal = (char *)HDmalloc(len+1);
            if (fmtVal == NULL) {
                h4outOfMemory(env, "SDgetdimstrs");
            }
        }
        ENVPTR->DeleteLocalRef(ENVPAR format);

        rval = SDgetdimstrs((int32)dimid, labVal, unitVal, fmtVal, (int32)len);

        if (rval == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            if (labVal != NULL) {
                labVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR labVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 0, (jobject)str);
                }
            }
            if (unitVal != NULL) {
                unitVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR unitVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 1, (jobject)str);
                }
            }
            if (fmtVal != NULL) {
                fmtVal[len] = '\0';
                str = ENVPTR->NewStringUTF(ENVPAR fmtVal);
                if (str != NULL) {
                    ENVPTR->SetObjectArrayElement(ENVPAR strings, 2, (jobject)str);
                }
            }
        }

        if (labVal != NULL) HDfree(labVal);
        if (unitVal != NULL) HDfree(unitVal);
        if (fmtVal != NULL) HDfree(fmtVal);
    } /* end else */
    return JNI_TRUE;
}

/*** note this is returning data of many types in an array of bytes.... */
/* not sure how well this will work for java programs .... */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetdimscale
(JNIEnv *env, jclass clss, jlong dimid, jbyteArray data)
{
    intn rval;
    jbyte *datVal;
    jboolean bb;
    jboolean cbb = 0;

    if (data == NULL) {
        h4nullArgument(env, "SDgetdimscale:  data is NULL");
    } /* end if */
    else {
        datVal = ENVPTR->GetByteArrayElements(ENVPAR data, &bb);
        if (datVal == NULL) {
            h4JNIFatalError(env, "SDgetdimscale:  data not pinned");
        } /* end if */
        else {
            rval = SDgetdimscale((int32)dimid, (char *)datVal);
            if (rval == FAIL) {
                cbb = JNI_ABORT;
                CALL_ERROR_CHECK();
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR data, datVal, cbb);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetfillvalue
(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray data)
{
    intn rval;
    jbyte *datVal;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDgetfillvalue:  data is NULL");
    } /* end if */
    else {
        datVal = ENVPTR->GetByteArrayElements(ENVPAR data, &bb);
        if (datVal == NULL) {
            h4JNIFatalError(env, "SDgetfillvalue:  data not pinned");
        } /* end if */
        else {
            rval = SDgetfillvalue(id, (char *)datVal);
            if (rval == FAIL) {
                cbb = JNI_ABORT;
                CALL_ERROR_CHECK();
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR data, datVal, cbb);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetrange
(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray max, jbyteArray min)
{
    intn rval;
    jbyte *minp, *maxp;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (max == NULL) {
        h4nullArgument(env, "SDgetrange:  max is NULL");
    } /* end if */
    else if (min == NULL) {
        h4nullArgument(env, "SDgetrange:  min is NULL");
    } /* end if */
    else {
        maxp = ENVPTR->GetByteArrayElements(ENVPAR max, &bb);
        if (maxp == NULL) {
            h4JNIFatalError(env, "SDgetrange:  max not pinned");
        } /* end if */
        else {
            minp = ENVPTR->GetByteArrayElements(ENVPAR min, &bb);
            if (minp == NULL) {
                h4JNIFatalError(env, "SDgetrange:  min not pinned");
            } /* end if */
            else {
                rval = SDgetrange(id, (void *)maxp, (void *)minp);
                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
                ENVPTR->ReleaseByteArrayElements(ENVPAR min, minp, cbb);
            } /* end else */

            ENVPTR->ReleaseByteArrayElements(ENVPAR max, maxp, cbb);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_SDcreate
(JNIEnv *env, jclass clss, jlong sd_id, jstring name, jlong number_type, jint rank, jintArray dimsizes)
{
    int32 rval = -1;
    const char *str;
    jint *dims;
    jboolean isCopy;
    int32 id = (int32)sd_id;

    if (dimsizes == NULL) {
        h4nullArgument(env, "SDcreate:  dimsizes is NULL");
    } /* end if */
    else {
        PIN_JAVA_STRING(name, str);
        if (str != NULL) {
            dims = ENVPTR->GetIntArrayElements(ENVPAR dimsizes, &isCopy);
            if (dims == NULL) {
                h4JNIFatalError(env, "SDcreate:  dimsizes not pinned");
            } /* end if */
            else {
                rval = SDcreate(id, str, (int32)number_type, (int32)rank, (int32 *)dims);

                ENVPTR->ReleaseIntArrayElements(ENVPAR dimsizes, dims, JNI_ABORT);

                if (rval < 0)
                    CALL_ERROR_CHECK();
            } /* end else */
            UNPIN_JAVA_STRING(name, str);
        }
    } /* end else */

    return (jlong)rval;
}

// this API call returns false for not record and for failures
// maybe failures should return a negative?
JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDisrecord
(JNIEnv *env, jclass clss, jlong sdsid)
{
    int32 rval;
    int32 id = (int32)sdsid;

    rval = SDisrecord(id);

    if (rval == TRUE) {
        return JNI_TRUE;
    }
    else {
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetattr
(JNIEnv *env, jclass clss, jlong s_id, jstring attr_name, jlong num_type, jint count, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    const char *str;
    jboolean bb;
    int32 id = (int32)s_id;

    if (values == NULL) {
        h4nullArgument(env, "SDsetattr:  values is NULL");
    } /* end if */
    else {
        PIN_JAVA_STRING(attr_name, str);
        if (str != NULL) {
            arr = ENVPTR->GetByteArrayElements(ENVPAR values, &bb);
            if (arr == NULL) {
                h4JNIFatalError(env, "SDsetattr:  values not pinned");
            } /* end if */
            else {
                rval = SDsetattr(id, str, (int32)num_type, (int32)count, (VOIDP)arr);

                ENVPTR->ReleaseByteArrayElements(ENVPAR values, arr, JNI_ABORT);

                if (rval == FAIL)
                    CALL_ERROR_CHECK();
            } /* end else */
            UNPIN_JAVA_STRING(attr_name, str);
        }
    } /* end else */
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetcal
(JNIEnv *env, jclass clss, jlong sds_id, jdouble cal, jdouble cal_err, jdouble offset, jdouble offset_err, jint number_type)
{
    intn rval;
    int32 id = (int32)sds_id;

    rval = SDsetcal(id, (float64)cal, (float64)cal_err,
        (float64)offset, (float64)offset_err, (int32)number_type);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdatastrs
(JNIEnv *env, jclass clss, jlong sds_id, jstring label, jstring unit, jstring format, jstring coordsys)
{
    intn rval;
    const char *labstr;
    const char *unstr;
    const char *formstr;
    const char *csstr;

    if (label == NULL) {
        labstr = NULL;
    }
    else {
        labstr = ENVPTR->GetStringUTFChars(ENVPAR label, 0);
    }
    if (unit == NULL) {
        unstr = NULL;
    }
    else {
        unstr = ENVPTR->GetStringUTFChars(ENVPAR unit, 0);
    }
    if (format == NULL) {
        formstr = NULL;
    }
    else {
        formstr = ENVPTR->GetStringUTFChars(ENVPAR format, 0);
    }
    if (coordsys == NULL) {
        csstr = NULL;
    }
    else {
        csstr = ENVPTR->GetStringUTFChars(ENVPAR coordsys, 0);
    }

    rval = SDsetdatastrs((int32)sds_id, labstr, unstr, formstr, csstr);

    if (labstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR label, labstr);
    if (unstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR unit, unstr);
    if (formstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR format, formstr);
    if (csstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR coordsys, csstr);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimname
(JNIEnv *env, jclass clss, jlong dim_id, jstring dim_name)
{
    intn rval;
    const char *str;

    PIN_JAVA_STRING(dim_name, str);
    if (str != NULL) {
        rval = SDsetdimname((int32)dim_id, str);

        UNPIN_JAVA_STRING(dim_name, str);
        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimscale
(JNIEnv *env, jclass clss, jlong dim_id, jint count, jint number_type, jbyteArray data)
{
    intn rval;
    jbyte *d;
    jboolean bb;

    if (data == NULL) {
        h4nullArgument(env, "SDsetdimscale:  data is NULL");
    } /* end if */
    else {
        d = ENVPTR->GetByteArrayElements(ENVPAR data, &bb);
        if (d == NULL) {
            h4JNIFatalError(env, "SDsetdimscale:  data not pinned");
        } /* end if */
        else {
            rval = SDsetdimscale((int32)dim_id, (int32)count, (int32)number_type, (VOIDP)d);

            ENVPTR->ReleaseByteArrayElements(ENVPAR data, d, JNI_ABORT);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimstrs
(JNIEnv *env, jclass clss, jlong dim_id, jstring label, jstring unit, jstring format)
{
    intn rval;
    const char *labstr;
    const char *unstr;
    const char *formstr;

    if (label == NULL) {
        labstr = NULL;
    }
    else {
        labstr = ENVPTR->GetStringUTFChars(ENVPAR label, 0);
    }
    if (unit == NULL) {
        unstr = NULL;
    }
    else {
        unstr = ENVPTR->GetStringUTFChars(ENVPAR unit, 0);
    }
    if (format == NULL) {
        formstr = NULL;
    }
    else {
        formstr = ENVPTR->GetStringUTFChars(ENVPAR format, 0);
    }

    rval = SDsetdimstrs((int32) dim_id, labstr, unstr, formstr);

    if (labstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR label, labstr);
    if (unstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR unit, unstr);
    if (formstr != NULL) ENVPTR->ReleaseStringUTFChars(ENVPAR format, formstr);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetexternalfile
(JNIEnv *env, jclass clss, jlong sds_id, jstring filename, jint offset)
{
    intn rval;
    const char *f;
    int32 id = (int32)sds_id;

    PIN_JAVA_STRING(filename, f);
    if (f != NULL) {
        rval = SDsetexternalfile(id, f, (int32)offset);

        UNPIN_JAVA_STRING(filename, f);
        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetfillvalue
(JNIEnv *env, jclass clss, jlong sds_id, jbyteArray fill_val)
{
    intn rval;
    jboolean bb;
    jbyte *d;
    int32 id = (int32)sds_id;

    if (fill_val == NULL) {
        h4nullArgument(env, "SDsetfillvalue:  fill_val is NULL");
    } /* end if */
    else {
        d = ENVPTR->GetByteArrayElements(ENVPAR fill_val, &bb);
        if (d == NULL) {
            h4JNIFatalError(env, "SDsetfillvalue:  fill_val not pinned");
        } /* end if */
        else {
            rval = SDsetfillvalue(id, (VOIDP)d);

            ENVPTR->ReleaseByteArrayElements(ENVPAR fill_val, d, JNI_ABORT);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetrange
(JNIEnv *env, jclass clss, jlong sdsid, jbyteArray max, jbyteArray min)
{
    int32 rval;
    jboolean bb;
    jbyte *minp, *maxp;
    int32 id = (int32)sdsid;

    if (max == NULL) {
        h4nullArgument(env, "SDsetrange:  max is NULL");
    } /* end if */
    else if (min == NULL) {
        h4nullArgument(env, "SDsetrange:  min is NULL");
    } /* end if */
    else {
        maxp = ENVPTR->GetByteArrayElements(ENVPAR max, &bb);
        if (maxp == NULL) {
            h4JNIFatalError(env, "SDgetrange:  max not pinned");
        } /* end if */
        else {
            minp = ENVPTR->GetByteArrayElements(ENVPAR min, &bb);
            if (minp == NULL) {
                h4JNIFatalError(env, "SDgetrange:  min not pinned");
            } /* end if */
            else {
                rval = SDsetrange(id, maxp, minp);
                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
                ENVPTR->ReleaseByteArrayElements(ENVPAR min, minp, JNI_ABORT);
            } /* end else */

            ENVPTR->ReleaseByteArrayElements(ENVPAR max, maxp, JNI_ABORT);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDwritedata
(JNIEnv *env, jclass clss, jlong sdsid, jintArray start, jintArray stride, jintArray edge, jbyteArray data)
{
    int32 rval;
    int32 *strt;
    int32 *strd;
    int32 *e;
    jbyte *d;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (data == NULL) {
        h4nullArgument(env, "SDwritedata:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "SDwritedata:  start is NULL");
    } /* end if */
    else if (edge == NULL) {
        h4nullArgument(env, "SDwritedata:  count is NULL");
    } /* end if */
    else {
        d = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "SDwritedata:  start not pinned");
        } /* end if */
        else {
            e = ENVPTR->GetIntArrayElements(ENVPAR edge, &bb);
            if (e == NULL) {
                h4JNIFatalError(env, "SDwritedata:  count not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = SDwritedata(id, strt, strd, e, d);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR edge, e, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, d, cbb);
    } /* end else */
    return JNI_TRUE;
}

/* new stuff for chunking */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetnbitdataset
(JNIEnv *env, jclass clss, jlong sdsid, jint start_bit, jint bit_len, jint sign_ext, jint fill_one)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetnbitdataset(id, (intn) start_bit, (intn) bit_len,
            (intn) sign_ext, (intn) fill_one);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetcompress
(JNIEnv *env, jclass clss, jlong sdsid, jint type, jobject cinfo)
{
    intn rval;
    comp_info cinf;
    int32 id = (int32)sdsid;

    if (cinfo == NULL) {
        h4nullArgument(env, "SDsetcompress:  cinfo is NULL");
    } /* end if */
    else if (getNewCompInfo(env, cinfo, &cinf) == JNI_FALSE) {
        h4raiseException(env, "SDsetcompress: error creating comp_info struct");
    }
    else {
        rval = SDsetcompress(id, (comp_coder_t)type, (comp_info *)&cinf);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcompinfo
(JNIEnv *env, jclass clss, jlong sdsid, jobject cinfo)
{
    intn rval;
    comp_coder_t coder;
    comp_info cinf;
    int32 id = (int32)sdsid;

    if (cinfo == NULL) {
        h4nullArgument(env, "SDgetcompinfo:  cinfo is NULL");
    } /* end if */
    else {
        rval = SDgetcompinfo(id, (comp_coder_t *)&coder, (comp_info *)&cinf);

        if (rval == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            if (setNewCompInfo(env, cinfo, coder, &cinf) == JNI_FALSE)
                h4raiseException(env, "SDgetcompinfo: error creating comp_info struct");
        }
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetcompress
(JNIEnv *env, jclass clss, jlong sdsid, jobject cinfo)
{
    intn rval;
    comp_coder_t coder;
    comp_info cinf;
    int32 id = (int32)sdsid;

    if (cinfo == NULL) {
        h4nullArgument(env, "SDgetcompress:  cinfo is NULL");
    } /* end if */
    else {
        rval = SDgetcompress(id, (comp_coder_t *)&coder, (comp_info *)&cinf);

        if (rval == FAIL) {
            CALL_ERROR_CHECK();
        }
        else {
            if (setNewCompInfo(env, cinfo, coder, &cinf) == JNI_FALSE)
                h4raiseException(env, "SDgetcompress: error creating comp_info struct");
        }
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetaccesstype
(JNIEnv *env, jclass clss, jlong sdsid, jint accesstype)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetaccesstype(id, (uintn)accesstype);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetblocksize
(JNIEnv *env, jclass clss, jlong sdsid, jint block_size)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetblocksize(id, (int32)block_size);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetfillmode
(JNIEnv *env, jclass clss, jlong sdsid, jint fillmode)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetfillmode(id, (intn)fillmode);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetdimval_1comp
(JNIEnv *env, jclass clss, jlong sdsid, jint comp_mode)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetdimval_comp(id, (intn)comp_mode);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDisdimval_1bwcomp
(JNIEnv *env, jclass clss, jlong dimid)
{
    intn rval;

    rval = SDisdimval_bwcomp((int32)dimid);
    if (rval == SD_DIMVAL_BW_COMP) {
        return JNI_TRUE;
    }
    else if (rval == SD_DIMVAL_BW_INCOMP) {
        return JNI_FALSE;
    }
    else
        CALL_ERROR_CHECK();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetchunk
(JNIEnv *env, jclass clss, jlong sdsid, jobject chunk_def, jint flags)
{
    intn rval;
    HDF_CHUNK_DEF c_def;
    int32 id = (int32)sdsid;

    if (chunk_def == NULL) {
        h4nullArgument(env, "SDsetchunk:  chunk_def is NULL");
    } /* end if */
    else if (getChunkInfo(env, chunk_def, &c_def) == JNI_FALSE) {
        h4raiseException(env, "SDsetchunk: error creating chunk_def struct");
    }
    else {
        rval = SDsetchunk(id, c_def, (int32)flags);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDgetchunkinfo
(JNIEnv *env, jclass clss, jlong sdsid, jobject chunk_def, jintArray cflags)
{
    int32 rval;
    HDF_CHUNK_DEF cdef;
    jint *flgs;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdsid;

    if (chunk_def == NULL) {
        h4nullArgument(env, "SDgetchunkinfo:  chunk_def is NULL");
    } /* end if */
    else if (cflags == NULL) {
        h4nullArgument(env, "SDgetchunkinfo:  cflags is NULL");
    } /* end else if */
    else {
        flgs = ENVPTR->GetIntArrayElements(ENVPAR cflags, &bb);
        if (flgs == NULL) {
            h4JNIFatalError(env, "SDgetchunkinfo:  cflags not pinned");
        } /* end if */
        else {
            rval = SDgetchunkinfo(id, &cdef, (int32 *)&(flgs[0]));

            if (rval == FAIL) {
                cbb = JNI_ABORT;
                CALL_ERROR_CHECK();
            }
            else {
            /* convert cdef to HDFchunkinfo */
                if (makeChunkInfo(env, chunk_def, (int32)*flgs, &cdef) == JNI_FALSE)
                    h4raiseException(env, "SDgetchunkinfo: error creating chunk_def struct");
            }
            ENVPTR->ReleaseIntArrayElements(ENVPAR cflags, (jint *)flgs, cbb);
        }
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDreadchunk
(JNIEnv *env, jclass clss, jlong sdid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte * s;
    jint *arr;
    jboolean bb;
    jboolean cbb = 0;
    int32 id = (int32)sdid;

    if (dat == NULL) {
        h4nullArgument(env, "SDreadchunk:  data is NULL");
    } /* end if */
    else if (origin == NULL) {
        h4nullArgument(env, "SDreadchunk:  origin is NULL");
    } /* end else if */
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR origin, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "SDreadchunk:  origin not pinned");
        } /* end if */
        else {
            s = ENVPTR->GetByteArrayElements(ENVPAR dat, &bb);
            if (s == NULL) {
                h4JNIFatalError(env, "SDreadchunk:  data not pinned");
            } /* end if */
            else {
                rval = SDreadchunk(id, (int32 *)arr, s);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
                ENVPTR->ReleaseByteArrayElements(ENVPAR dat, s, cbb);
            }
            ENVPTR->ReleaseIntArrayElements(ENVPAR origin, arr, JNI_ABORT);
        }
    }
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_SDsetchunkcache
(JNIEnv *env, jclass clss, jlong sdsid, jint maxcache, jint flags)
{
    intn rval;
    int32 id = (int32)sdsid;

    rval = SDsetchunkcache(id, (int32)maxcache, (int32)flags);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDwritechunk
(JNIEnv *env, jclass clss, jlong sdid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte *s;
    jint  *arr;
    jboolean bb;
    int32 id = (int32)sdid;

    if (dat == NULL) {
        h4nullArgument(env, "SDwritechunk:  data is NULL");
    } /* end if */
    else if (origin == NULL) {
        h4nullArgument(env, "SDwritechunk:  origin is NULL");
    } /* end else if */
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR origin, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "SDwritechunk:  origin not pinned");
        } /* end if */
        else {
            s = ENVPTR->GetByteArrayElements(ENVPAR dat, &bb);
            if (s == NULL) {
                h4JNIFatalError(env, "SDwritechunk:  data not pinned");
            } /* end if */
            else {
                rval = SDwritechunk(id, (int32 *)arr, s);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
                ENVPTR->ReleaseByteArrayElements(ENVPAR dat, s, JNI_ABORT);
            }
            ENVPTR->ReleaseIntArrayElements(ENVPAR origin, arr, JNI_ABORT);
        }
    }
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_SDcheckempty
(JNIEnv *env, jclass clss, jlong sdsid, jintArray emptySDS)
{
    int32 rval;
    jboolean isCopy;
    intn *isempty;
    int32 id = (int32)sdsid;

    if (emptySDS == NULL) {
        h4nullArgument(env, "SDcheckempty:  emptySDS is NULL");
    } /* end if */
    else {
        isempty = (intn *)ENVPTR->GetIntArrayElements(ENVPAR emptySDS, &isCopy);
        if (isempty == NULL) {
            h4JNIFatalError(env, "SDcheckempty:  emptySDS not pinned");
        } /* end if */
        else {
            rval = SDcheckempty(id, isempty);

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR emptySDS, isempty, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR emptySDS, isempty, 0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
