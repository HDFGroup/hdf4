
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

/* Name changed from MAX_GR_NAME to H4_MAX_GR_NAME in hdf4.2r2 */
#define MAX_GR_NAME H4_MAX_GR_NAME

extern jboolean makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf);
extern jboolean getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf);
extern jboolean setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf);
extern jboolean getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf);



JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRstart
(JNIEnv *env, jclass cls, jlong file_id)
{
    intn rval;

    rval = GRstart((int32)file_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRend
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval = GRend((int32) gr_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRendaccess
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval =  GRendaccess((int32) gr_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRfileinfo
(JNIEnv *env, jclass cls, jlong gr_id, jintArray argv)
{
    intn rval;
    jint *theArgs;
    jboolean isCopy; /*  dummy */

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
            rval = GRfileinfo((int32)gr_id, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]));

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
Java_hdf_hdflib_HDFLibrary_GRselect
(JNIEnv *env, jclass cls, jlong gr_id, jint index)
{
    int32 rval;

    rval = GRselect((int32)gr_id, (int32) index);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRnametoindex
(JNIEnv *env, jclass cls, jlong gr_id, jstring gr_name)
{
    int32 rval = -1;
    const char *str;

    PIN_JAVA_STRING(gr_name, str);
    if (str != NULL) {
        rval = GRnametoindex((int32)gr_id, str);

        UNPIN_JAVA_STRING(gr_name, str);

        if (rval < 0)
            CALL_ERROR_CHECK();
    }

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetchunkinfo
(JNIEnv *env, jclass cls, jlong grsid, jobject chunk_def, jintArray cflags)
{
    int32 rval;
    HDF_CHUNK_DEF cdef;
    jboolean stat = JNI_FALSE;
    jint *flgs;
    jboolean isCopy;

    if (cflags == NULL) {
        h4nullArgument(env, "GRgetchunkinfo:  cflags is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR cflags) < 1) {
        h4badArgument(env, "GRgetchunkinfo:  cflags input array < order 1");
    } /* end else if */
    else {
        flgs = ENVPTR->GetIntArrayElements(ENVPAR cflags, &isCopy);
        if (flgs == NULL) {
            h4JNIFatalError(env, "GRgetchunkinfo:  cflags not pinned");
        } /* end if */
        else {
            rval = GRgetchunkinfo( (int32)grsid, &cdef, (int32 *)&(flgs[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR cflags, (jint *)flgs, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                stat = JNI_TRUE;
                if (*flgs != 0) {
                /* convert cdef to HDFchunkinfo */
                    stat = makeChunkInfo(env, chunk_def, *flgs, &cdef);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR cflags, (jint *)flgs, 0);
            }
        } /* end else */
    } /* end else */

    return stat/*JNI_TRUE*/;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetiminfo
(JNIEnv *env, jclass cls, jlong ri_id, jobjectArray gr_name, jintArray argv, jintArray dim_sizes)
{
    intn     rval;
    jint    *dims;
    jint    *theArgs;
    jclass   Sjc;
    char    *str;
    jstring  rstring;
    jboolean isCopy;
    jboolean bb = 0;
    jboolean abb = 0;
    jobject  o;

    str = (char *)HDmalloc(MAX_GR_NAME+1);
    if (str == NULL) {
        h4outOfMemory(env, "GRgetiminfo");
    }
    else {
        if (gr_name == NULL) {
            h4nullArgument(env, "GRgetiminfo:  gr_name is NULL");
        } /* end if */
        else if (dim_sizes == NULL) {
            h4nullArgument(env, "GRgetiminfo:  dim_sizes is NULL");
        } /* end if */
        else if (ENVPTR->GetArrayLength(ENVPAR dim_sizes) < 2) {
            h4badArgument(env, "GRgetiminfo:  dim_sizes input array < order 2");
        } /* end else if */
        else if (argv == NULL) {
            h4nullArgument(env, "GRgetiminfo:  argv is NULL");
        } /* end if */
        else if (ENVPTR->GetArrayLength(ENVPAR argv) < 4) {
            h4badArgument(env, "GRgetiminfo:  argv input array < order 4");
        } /* end else if */
        else {
            dims = ENVPTR->GetIntArrayElements(ENVPAR dim_sizes, &isCopy);
            if (dims == NULL) {
                h4JNIFatalError(env, "GRgetiminfo:  dim_sizes not pinned");
            } /* end if */
            else {
                theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &isCopy);
                if (theArgs == NULL) {
                    h4JNIFatalError(env, "GRgetiminfo:  argv not pinned");
                } /* end if */
                else {
                    rval = GRgetiminfo((int32) ri_id, (char *)str, (int32 *)&(theArgs[0]),
                            (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]), (int32 *)dims,
                            (int32 *)&(theArgs[3]));

                    if (rval == FAIL) {
                        abb = JNI_ABORT;
                        bb = JNI_ABORT;
                        CALL_ERROR_CHECK();
                    }
                    else {
                        o = ENVPTR->GetObjectArrayElement(ENVPAR gr_name, 0);
                        if (o == NULL) {
                            bb = JNI_ABORT;
                            CALL_ERROR_CHECK();
                        }
                        else {
                            Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                            if (Sjc == NULL) {
                                bb = JNI_ABORT;
                                CALL_ERROR_CHECK();
                            }
                            else if (ENVPTR->IsInstanceOf(ENVPAR o, Sjc) == JNI_FALSE) {
                                bb = JNI_ABORT;
                                CALL_ERROR_CHECK();
                            }
                            else {
                                str[MAX_GR_NAME] = '\0';
                                rstring = ENVPTR->NewStringUTF(ENVPAR  str);
                                if (rstring != NULL)
                                    ENVPTR->SetObjectArrayElement(ENVPAR gr_name, 0, (jobject)rstring);
                            }
                            ENVPTR->DeleteLocalRef(ENVPAR o);
                        }
                    }
                    ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, abb);

                } /* end else */
                ENVPTR->ReleaseIntArrayElements(ENVPAR dim_sizes, dims, bb);

            } /* end else */
        } /* end else */

        HDfree(str);
    }
    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadimage
(JNIEnv *env, jclass cls, jlong ri_id, jintArray start, jintArray stride, jintArray edge, jbyteArray data)
{
    intn rval;
    jbyte *arr;
    jint *strt;
    jint *strd;
    jint *edg;
    jboolean bb;
    jboolean cbb = 0;

    if (data == NULL) {
        h4nullArgument(env, "GRreadimage:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "GRreadimage:  start is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR start) < 2) {
        h4badArgument(env, "GRreadimage:  start input array < order 2");
    } /* end else if */
    else if (edge == NULL) {
        h4nullArgument(env, "GRreadimage:  edge is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR edge) < 2) {
        h4badArgument(env, "GRreadimage:  edge input array < order 2");
    } /* end else if */
    else {
        arr = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data, &bb);

        strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
        if (strt == NULL) {
            h4JNIFatalError(env, "GRreadimage:  start not pinned");
        } /* end if */
        else {
            edg = ENVPTR->GetIntArrayElements(ENVPAR edge, &bb);
            if (edg == NULL) {
                h4JNIFatalError(env, "GRreadimage:  edge not pinned");
            } /* end if */
            else {
                if (stride == NULL) {
                    strd = NULL;
                }
                else {
                    strd = ENVPTR->GetIntArrayElements(ENVPAR stride, &bb);
                }
                rval = GRreadimage((int32)ri_id, (int32 *)strt, (int32 *)strd,
                        (int32 *)edg, (VOIDP)arr);

                if (stride != NULL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR edge, edg, JNI_ABORT);

                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
        }
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data, arr, cbb);
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRidtoref
(JNIEnv *env, jclass cls, jlong gr_id)
{
    uint16 rval;

    rval = GRidtoref((int32) gr_id);
    if (rval <= 0)
        CALL_ERROR_CHECK();

    return (jshort)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRreftoindex
(JNIEnv *env, jclass cls, jlong gr_id, jshort ref)
{
    int32 rval;

    rval = GRreftoindex((int32) gr_id, (uint16)ref);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqlutil
(JNIEnv *env, jclass cls, jlong gr_id, jint interlace_mode)
{
    intn rval;

    rval = GRreqlutil((int32) gr_id, (intn)interlace_mode);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqimageil
(JNIEnv *env, jclass cls, jlong gr_id, jint interlace_mode)
{
    intn rval;

    rval = GRreqimageil((int32) gr_id, (intn)interlace_mode);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutid
(JNIEnv *env, jclass cls, jlong gr_id, jint index)
{
    int32 rval;

    rval = GRgetlutid((int32) gr_id, (int32)index);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetnluts
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval = GRgetnluts((int32) gr_id);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutinfo
(JNIEnv *env, jclass cls, jlong ri_id, jintArray argv)
{
    intn rval;
    jint * theArgs;
    jboolean isCopy; /*  dummy */

    if (argv == NULL) {
        h4nullArgument(env, "GRgetlutinfo:  argv is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 4) {
        h4badArgument(env, "GRgetlutinfo:  argv input array < order 4");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &isCopy);
        if (theArgs == NULL) {
            h4JNIFatalError(env, "GRgetlutinfo:  argv not pinned");
        } /* end if */
        else {
            rval = GRgetlutinfo((int32)ri_id, (int32 *)&(theArgs[0]),
                (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]),
                (int32 *)&(theArgs[3]));


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



JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadlut
(JNIEnv *env, jclass cls, jlong pal_id, jbyteArray pal_data)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    if (pal_data == NULL) {
        h4nullArgument(env, "GRreadlut:  pal_data is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR pal_data, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRreadlut:  pal_data not pinned");
        } /* end if */
        else {
            rval = GRreadlut((int32) pal_id, (VOIDP)arr);

            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data, arr, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data, arr, 0);
            }
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRattrinfo
(JNIEnv *env, jclass cls, jlong gr_id, jint attr_index, jobjectArray name, jintArray argv)
{
    int32 rval;
    char *str;
    jclass jc;
    jstring rstring;
    jint *theArgs;
    jboolean bb;
    jobject o;


    /* check for out of memory error ... */
    str = (char *)HDmalloc(MAX_GR_NAME+1);
    if (str == NULL) {
        h4outOfMemory(env, "GRattrinfo");
    }
    else {
        if (name == NULL) {
            h4nullArgument(env, "GRattrinfo:  name is NULL");
        } /* end if */
        else if (argv == NULL) {
            h4nullArgument(env, "GRattrinfo:  argv is NULL");
        } /* end if */
        else if (ENVPTR->GetArrayLength(ENVPAR argv) < 2) {
            h4badArgument(env, "GRattrinfo:  argv input array < order 2");
        } /* end else if */
        else {
            theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv, &bb);
            if (theArgs == NULL) {
                h4JNIFatalError(env, "GRattrinfo:  argv not pinned");
            } /* end if */
            else {
                rval = GRattrinfo((int32)gr_id, (int32)attr_index,
                    (char *)str, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]));

                if (rval == FAIL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, JNI_ABORT);
                    CALL_ERROR_CHECK();
                }
                else {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR argv, theArgs, 0);
                    if (str != NULL) {
                        str[MAX_GR_NAME] = '\0';
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
        } /* end else */
        HDfree(str);
    }
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetattr
(JNIEnv *env, jclass cls, jlong gr_id, jint attr_index, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "GRgetattr:  values is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR values, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRgetattr:  values not pinned");
        } /* end if */
        else {
            rval = GRgetattr((int32)gr_id, (int32)attr_index,  (VOIDP)arr);
            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values, arr, JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values, arr, 0);
            }
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRfindattr
(JNIEnv *env, jclass cls, jlong gr_id, jstring attr_name)
{
    int32 rval = -1;
    const char *str;

    PIN_JAVA_STRING(attr_name, str);
    if (str != NULL) {
        rval = GRfindattr((int32)gr_id, str);

        UNPIN_JAVA_STRING(attr_name, str);

        if (rval < 0)
            CALL_ERROR_CHECK();
    }

    return (jint)rval;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRcreate
(JNIEnv *env, jclass cls, jlong gr_id, jstring name, jint ncomp, jlong data_type, jint interlace_mode, jintArray dim_sizes)
{
    int32 rval = -1;
    jint *dims;
    const char *str;
    jboolean bb;

    if (dim_sizes == NULL) {
        h4nullArgument(env, "GRcreate:  dim_sizes is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR dim_sizes) < 2) {
        h4badArgument(env, "GRcreate:  dim_sizes input array < order 2");
    } /* end else if */
    else {
        PIN_JAVA_STRING(name, str);
        if (str != NULL) {
            dims = ENVPTR->GetIntArrayElements(ENVPAR dim_sizes, &bb);
            if (dims == NULL) {
                h4JNIFatalError(env, "GRcreate:  dim_sizes not pinned");
            } /* end if */
            else {
                rval = GRcreate( (int32)gr_id, str, (int32)ncomp,
                    (int32)data_type, (int32)interlace_mode, (int32 *)dims);

                ENVPTR->ReleaseIntArrayElements(ENVPAR dim_sizes, dims, JNI_ABORT);
                if (rval < 0)
                    CALL_ERROR_CHECK();
            } /* end else */
            UNPIN_JAVA_STRING(name, str);
        }
    } /* end else */
    return (jlong)rval;
}


JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRluttoref
(JNIEnv *env, jclass cls, jlong pal_id)
{
    uint16 rval;

    rval = GRluttoref((int32) pal_id);
    if (rval <= 0)
        CALL_ERROR_CHECK();

    return (jshort)rval;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JILjava_lang_String_2
(JNIEnv *env, jclass cls, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values)
{
    intn rval;
    const char *str;
    const char *val;

    PIN_JAVA_STRING_TWO(attr_name, str, values, val);
    if (str != NULL && val != NULL) {
        rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (VOIDP)val);

        UNPIN_JAVA_STRING_TWO(attr_name, str, values, val);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JI_3B
(JNIEnv *env, jclass cls, jlong gr_id, jstring attr_name, jlong data_type, jint count, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    const char *str;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "GRsetattr:  values is NULL");
    } /* end if */
    else {
        PIN_JAVA_STRING(attr_name, str);
        if (str != NULL) {
            arr = ENVPTR->GetByteArrayElements(ENVPAR values, &bb);
            if (arr == NULL) {
                h4JNIFatalError(env, "GRsetattr:  values not pinned");
            } /* end if */
            else {
                rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (VOIDP)arr);

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
Java_hdf_hdflib_HDFLibrary_GRsetcompress
(JNIEnv *env, jclass cls, jlong ri_id, jint comp_type, jobject c_info)
{
    intn rval;
    comp_info cinf;

    if (c_info == NULL) {
        h4nullArgument(env, "GRsetcompress:  c_info is NULL");
    } /* end if */
    else {
        if (getNewCompInfo(env, c_info, &cinf)) {
            rval = GRsetcompress((int32)ri_id, (comp_coder_t)comp_type, (comp_info *)&cinf);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end if */
        else {
            h4JNIFatalError(env, "GRsetcompress:  c_info not initialized");
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetcompress
(JNIEnv *env, jclass cls, jlong ri_id, jobject c_info)
{
    intn rval;
    comp_coder_t coder;
    comp_info cinf;

    if (c_info == NULL) {
        h4nullArgument(env, "GRgetcompress:  c_info is NULL");
    } /* end if */
    else {
        rval = GRgetcompress((int32)ri_id, (comp_coder_t *)&coder, (comp_info *)&cinf);

        if (setNewCompInfo(env, c_info, coder, &cinf)) {
            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end if */
        else {
            h4JNIFatalError(env, "GRgetcompress:  c_info not created");
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetcompinfo
(JNIEnv *env, jclass cls, jlong ri_id, jobject c_info)
{
    intn rval;
    comp_coder_t coder;
    comp_info cinf;

    if (c_info == NULL) {
        h4nullArgument(env, "GRgetcompinfo:  c_info is NULL");
    } /* end if */
    else {
        rval = GRgetcompinfo((int32)ri_id, (comp_coder_t *)&coder, (comp_info *)&cinf);

        if (setNewCompInfo(env, c_info, coder, &cinf)) {
            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end if */
        else {
            h4JNIFatalError(env, "GRgetcompinfo:  c_info not created");
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunk
(JNIEnv *env, jclass cls, jlong sdsid, jobject chunk_def, jint flags)
{
    intn rval;
    HDF_CHUNK_DEF c_def;

    if (chunk_def == NULL) {
        h4nullArgument(env, "GRsetchunk:  chunk_def is NULL");
    } /* end if */
    else {
        if (getChunkInfo(env, chunk_def, &c_def)) {
            rval = SDsetchunk ((int32)sdsid, c_def, (int32)flags);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end if */
        else {
            h4JNIFatalError(env, "GRsetchunk:  chunk_def not initialized");
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunkcache
(JNIEnv *env, jclass cls, jlong sdsid, jint maxcache, jint flags)
{
    intn rval;

    rval = SDsetchunkcache((int32)sdsid, (int32)maxcache, (int32)flags);
    if (rval < 0)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetexternalfile
(JNIEnv *env, jclass cls, jlong ri_id, jstring filename, jint offset)
{
    intn rval;
    const char *str;

    PIN_JAVA_STRING(filename, str);
    if (str != NULL) {
        rval = GRsetexternalfile((int32)ri_id, str, (int32)offset);

        UNPIN_JAVA_STRING(filename, str);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwriteimage(JNIEnv *env, jclass cls, jlong ri_id,
        jintArray start, jintArray stride, jintArray edge, jbyteArray data)
{
    intn rval;
    jbyte *arr;
    jint *strt;
    jint *strd;
    jint *edg;
    jboolean bb;
    jboolean cbb = 0;

    if (data == NULL) {
        h4nullArgument(env, "GRwriteimage:  data is NULL");
    } /* end if */
    else if (start == NULL) {
        h4nullArgument(env, "GRreadimage:  start is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR start) < 2) {
        h4badArgument(env, "GRreadimage:  start input array < order 2");
    } /* end else if */
    else if (edge == NULL) {
        h4nullArgument(env, "GRreadimage:  edge is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR edge) < 2) {
        h4badArgument(env, "GRreadimage:  edge input array < order 2");
    } /* end else if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR data, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRwriteimage:  data not pinned");
        } /* end if */
        else {
            strt = ENVPTR->GetIntArrayElements(ENVPAR start, &bb);
            if (strt == NULL) {
                h4JNIFatalError(env, "GRreadimage:  start not pinned");
            } /* end if */
            else {
                edg = ENVPTR->GetIntArrayElements(ENVPAR edge, &bb);
                if (edg == NULL) {
                    h4JNIFatalError(env, "ANget_tagref:  edge not pinned");
                } /* end if */
                else {
                    if (stride == NULL) {
                        strd = NULL;
                    }
                    else {
                        strd = ENVPTR->GetIntArrayElements(ENVPAR stride,&bb);
                    }
                    rval = GRwriteimage((int32)ri_id, (int32 *)strt, (int32 *)strd,
                            (int32 *)edg, (VOIDP)arr);

                    if (stride != NULL) {
                        ENVPTR->ReleaseIntArrayElements(ENVPAR stride, strd, JNI_ABORT);
                    }

                    if (rval == FAIL)
                        CALL_ERROR_CHECK();

                    ENVPTR->ReleaseIntArrayElements(ENVPAR edge, edg, JNI_ABORT);
                } /* end else */
                ENVPTR->ReleaseIntArrayElements(ENVPAR start, strt, JNI_ABORT);
            } /* end else */
            ENVPTR->ReleaseByteArrayElements(ENVPAR data, arr, JNI_ABORT);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritelut
(JNIEnv *env, jclass cls, jlong pal_id, jint ncomp, jint data_type, jint interlace, jint num_entries, jbyteArray pal_data)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    if (pal_data == NULL) {
        h4nullArgument(env, "GRwritelut:  pal_data is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR pal_data, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRwritelut:  pal_data not pinned");
        } /* end if */
        else {
            rval = GRwritelut((int32)pal_id, (int32)ncomp, (int32)data_type,
                (int32)interlace, (int32)num_entries, (VOIDP)arr);

            ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data, arr, JNI_ABORT);

            if (rval == FAIL)
                CALL_ERROR_CHECK();
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadchunk
(JNIEnv *env, jclass cls, jlong grid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte *arr;
    jint *org;
    jboolean bb;
    jboolean cbb = 0;

    if (dat == NULL) {
        h4nullArgument(env, "GRreadchunk:  dat is NULL");
    } /* end if */
    else if (origin == NULL) {
        h4nullArgument(env, "GRreadchunk:  origin is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR origin) < 2) {
        h4badArgument(env, "GRreadchunk:  origin input array < order 2");
    } /* end else if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR dat, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRreadchunk:  dat not pinned");
        } /* end if */
        else {
            org = ENVPTR->GetIntArrayElements(ENVPAR origin, &bb);
            if (org == NULL) {
                h4JNIFatalError(env, "GRreadchunk:  origin not pinned");
            } /* end if */
            else {
                rval = GRreadchunk((int32)grid, (int32 *)org, arr);

                ENVPTR->ReleaseIntArrayElements(ENVPAR origin, org, JNI_ABORT);
                if (rval == FAIL) {
                    cbb = JNI_ABORT;
                    CALL_ERROR_CHECK();
                }
            } /* end else */
            ENVPTR->ReleaseByteArrayElements(ENVPAR dat, arr, cbb);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritechunk
(JNIEnv *env, jclass cls, jlong grid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte * arr;
    jint * org;
    jboolean bb;

    if (dat == NULL) {
        h4nullArgument(env, "GRwritechunk:  dat is NULL");
    } /* end if */
    else if (origin == NULL) {
        h4nullArgument(env, "GRwritechunk:  origin is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR origin) < 2) {
        h4badArgument(env, "GRwritechunk:  origin input array < order 2");
    } /* end else if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR dat, &bb);
        if (arr == NULL) {
            h4JNIFatalError(env, "GRwritechunk:  dat not pinned");
        } /* end if */
        else {
            org = ENVPTR->GetIntArrayElements(ENVPAR origin, &bb);
            if (org == NULL) {
                h4JNIFatalError(env, "GRwritechunk:  origin not pinned");
            } /* end if */
            else {
                rval = GRwritechunk((int32)grid, (int32 *)org, (char *)arr);

                ENVPTR->ReleaseIntArrayElements(ENVPAR origin, org, JNI_ABORT);

                if (rval == FAIL)
                    CALL_ERROR_CHECK();
            } /* end else */
            ENVPTR->ReleaseByteArrayElements(ENVPAR dat, arr, JNI_ABORT);
        } /* end else */
    } /* end else */
    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
