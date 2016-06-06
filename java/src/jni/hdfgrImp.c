
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
        CALL_ERROR_CHECK(-1);

    return (jlong)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRend
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval = GRend((int32) gr_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRendaccess
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval =  GRendaccess((int32) gr_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

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
                CALL_ERROR_CHECK(JNI_FALSE);
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

    rval = (jlong)GRselect((int32) gr_id, (int32) index);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRnametoindex
(JNIEnv *env, jclass cls, jlong gr_id, jstring gr_name)
{
    int32 rval;
    const char *str;

    PIN_JAVA_STRING(gr_name, str, -1);

    rval = GRnametoindex((int32)gr_id, str);

    UNPIN_JAVA_STRING(gr_name, str);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetchunkinfo
(JNIEnv *env, jclass cls, jlong grsid, jobject chunk_def, jintArray cflags)
{
    int32 rval;
    HDF_CHUNK_DEF cdef;
    jboolean stat;
    jint *flgs;
    jboolean isCopy;

    if (cflags == NULL) {
        h4nullArgument(env, "GRgetchunkinfo:  cflags is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR cflags) < 1) {
        h4badArgument(env, "GRgetchunkinfo:  cflags input array < order 1");
    } /* end else if */
    else {
        flgs = ENVPTR->GetShortArrayElements(ENVPAR cflags, &isCopy);
        if (flgs == NULL) {
            h4JNIFatalError(env, "GRgetchunkinfo:  cflags not pinned");
        } /* end if */
        else {
            rval = GRgetchunkinfo( (int32)grsid, &cdef, (int32 *)&(flgs[0]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR cflags,(jint *)flgs,JNI_ABORT);
                CALL_ERROR_CHECK(JNI_FALSE);
            }
            else {
                stat = JNI_TRUE;
                if (*flgs != 0) {
                /* convert cdef to HDFchunkinfo */
                    stat = makeChunkInfo(env, chunk_def, *flgs, &cdef);
                }
                ENVPTR->ReleaseIntArrayElements(ENVPAR cflags,(jint *)flgs,0);
            }
        } /* end else */
    } /* end else */

    return stat/*JNI_TRUE*/;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetiminfo
(JNIEnv *env, jclass cls, jlong ri_id, jobjectArray gr_name, jintArray argv, jintArray dim_sizes)
{
    intn rval;
    jint * dims;
    jint * theArgs;
    jclass Sjc;
    char *str;
    jstring rstring;
    jboolean bb;
    jobject o;

    str = (char *)HDmalloc(MAX_GR_NAME+1);
    if (str == NULL) {
        h4outOfMemory(env, "GRgetiminfo");
        return JNI_FALSE;
    }
    /* check for out of memory error ... */

    dims = ENVPTR->GetIntArrayElements(ENVPAR dim_sizes,&bb);
    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

    rval = GRgetiminfo((int32) ri_id, (char *)str, (int32 *)&(theArgs[0]),
        (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]), (int32 *)dims,
        (int32 *)&(theArgs[3]));

    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR dim_sizes,dims,JNI_ABORT);
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        if (str != NULL) HDfree(str);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR dim_sizes,dims,0);
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);
        if (str != NULL) {
            str[MAX_GR_NAME] = '\0';
            rstring = ENVPTR->NewStringUTF(ENVPAR  str);
            o = ENVPTR->GetObjectArrayElement(ENVPAR gr_name,0);
            if (o == NULL) {
                return JNI_FALSE;
            }
            Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
            if (Sjc == NULL) {
                return JNI_FALSE;
            }
            bb = ENVPTR->IsInstanceOf(ENVPAR o,Sjc);
            if (bb == JNI_FALSE) {
                return JNI_FALSE;
            }
            ENVPTR->SetObjectArrayElement(ENVPAR gr_name,0,(jobject)rstring);
            ENVPTR->DeleteLocalRef(ENVPAR o);

            HDfree(str);
        }
        return JNI_TRUE;
    }
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

    arr = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR data,&bb);

    strt = ENVPTR->GetIntArrayElements(ENVPAR start,&bb);
    if (stride == NULL) {
        strd = NULL;
    }
    else {
        strd = ENVPTR->GetIntArrayElements(ENVPAR stride,&bb);
    }
    edg = ENVPTR->GetIntArrayElements(ENVPAR edge,&bb);

    if (strd == NULL) {
        rval = GRreadimage((int32) ri_id, (int32 *)strt, (int32 *)NULL,
            (int32 *)edg, (VOIDP) arr);
    }
    else {
        rval = GRreadimage((int32) ri_id, (int32 *)strt, (int32 *)strd,
            (int32 *)edg, (VOIDP) arr);
    }

    ENVPTR->ReleaseIntArrayElements(ENVPAR start,strt,JNI_ABORT);
    if (stride != NULL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR stride,strd,JNI_ABORT);
    }
    ENVPTR->ReleaseIntArrayElements(ENVPAR edge,edg,JNI_ABORT);

    if (rval == FAIL) {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data,arr,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR data,arr,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRidtoref
(JNIEnv *env, jclass cls, jlong gr_id)
{
    uint16 rval;

    rval = GRidtoref((int32) gr_id);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jshort)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRreftoindex
(JNIEnv *env, jclass cls, jlong gr_id, jshort ref)
{
    int32 rval;

    rval = GRreftoindex((int32) gr_id, (uint16)ref);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqlutil
(JNIEnv *env, jclass cls, jlong gr_id, jint interlace_mode)
{
    intn rval;

    rval = GRreqlutil((int32) gr_id, (intn)interlace_mode);
    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreqimageil
(JNIEnv *env, jclass cls, jlong gr_id, jint interlace_mode)
{
    intn rval;

    rval = GRreqimageil((int32) gr_id, (intn)interlace_mode);
    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutid
(JNIEnv *env, jclass cls, jlong gr_id, jint index)
{
    int32 rval;

    rval = GRgetlutid((int32) gr_id, (int32)index);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jlong)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetnluts
(JNIEnv *env, jclass cls, jlong gr_id)
{
    intn rval;

    rval = GRgetnluts((int32) gr_id);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetlutinfo
(JNIEnv *env, jclass cls, jlong ri_id, jintArray argv)
{
    intn rval;
    jint * theArgs;
    jboolean bb; /* dummy */

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

    rval = GRgetlutinfo((int32) ri_id, (int32 *)&(theArgs[0]),
        (int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]),
        (int32 *)&(theArgs[3]));


    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);
        return JNI_TRUE;
    }
}



JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadlut
(JNIEnv *env, jclass cls, jlong pal_id, jbyteArray pal_data)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    arr = ENVPTR->GetByteArrayElements(ENVPAR pal_data,&bb);

    rval = GRreadlut((int32) pal_id, (VOIDP) arr);

    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data,arr,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data,arr,0);
        return JNI_TRUE;
    }
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
        /* exception */
        h4outOfMemory(env, "GRattrinfo");
        return JNI_FALSE;
    }

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

    rval = GRattrinfo((int32) gr_id, (int32) attr_index,
        (char *)str, (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]));


    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);
        if (str != NULL) {
            str[MAX_GR_NAME] = '\0';
            rstring = ENVPTR->NewStringUTF(ENVPAR  str);
            o = ENVPTR->GetObjectArrayElement(ENVPAR name,0);
            if (o == NULL) {
                return JNI_FALSE;
            }
            jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
            if (bb == JNI_FALSE) {
                return JNI_FALSE;
            }
            ENVPTR->SetObjectArrayElement(ENVPAR name,0,(jobject)rstring);
            ENVPTR->DeleteLocalRef(ENVPAR o);

            HDfree(str);
        }
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetattr
(JNIEnv *env, jclass cls, jlong gr_id, jint attr_index, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);
    rval = GRgetattr((int32) gr_id, (int32) attr_index,  (VOIDP) arr);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRfindattr
(JNIEnv *env, jclass cls, jlong gr_id, jstring attr_name)
{
    int32 rval;
    const char *str;

    PIN_JAVA_STRING(attr_name, str, -1);

    rval = GRfindattr((int32) gr_id, (char *)str);

    UNPIN_JAVA_STRING(attr_name, str);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_GRcreate
(JNIEnv *env, jclass cls, jlong gr_id, jstring name, jint ncomp, jlong data_type, jint interlace_mode, jintArray dim_sizes)
{
    int32 rval;
    jint *dims;
    const char *str;
    jboolean bb;

    dims = ENVPTR->GetIntArrayElements(ENVPAR dim_sizes,&bb);

    PIN_JAVA_STRING(name, str, JNI_FALSE);

    rval = GRcreate( (int32)gr_id, str, (int32)ncomp,
        (int32)data_type, (int32)interlace_mode, (int32 *)dims);

    UNPIN_JAVA_STRING(name, str);

    ENVPTR->ReleaseIntArrayElements(ENVPAR dim_sizes,dims,JNI_ABORT);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jlong)rval;
}


JNIEXPORT jshort JNICALL
Java_hdf_hdflib_HDFLibrary_GRluttoref
(JNIEnv *env, jclass cls, jlong pal_id)
{
    uint16 rval;

    rval = GRluttoref((int32) pal_id);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jshort)rval;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetattr__JLjava_lang_String_2JILjava_lang_String_2
(JNIEnv *env, jclass cls, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values)
{
    intn rval;
    const char *str;
    const char *val;

    PIN_JAVA_STRING_TWO(attr_name, str, values, val, JNI_FALSE);

    rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (VOIDP)val);

    UNPIN_JAVA_STRING_TWO(attr_name, str, values, val);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

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

    arr = ENVPTR->GetByteArrayElements(ENVPAR values, &bb);

    PIN_JAVA_STRING(attr_name, str, JNI_FALSE);

    rval = GRsetattr((int32)gr_id, str, (int32)data_type, (int32)count, (VOIDP)arr);

    UNPIN_JAVA_STRING(attr_name, str);

    ENVPTR->ReleaseByteArrayElements(ENVPAR values, arr, JNI_ABORT);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetcompress
(JNIEnv *env, jclass cls, jlong ri_id, jint comp_type, jobject c_info)
{
    intn rval;
    comp_info cinf;
    jboolean bval;

    bval = getNewCompInfo(env, c_info,&cinf);

    /* check for success... */

    rval = GRsetcompress((int32) ri_id, (comp_coder_t) comp_type, (comp_info *)&cinf);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRgetcompress
(JNIEnv *env, jclass cls, jlong ri_id, jobject c_info)
{
    intn rval;
    comp_coder_t coder;
    comp_info cinf;
    jboolean bval;


    /* check for success... */

    rval = GRgetcompress((int32) ri_id, (comp_coder_t *) &coder, (comp_info *)&cinf);

    bval = setNewCompInfo(env, c_info,coder,&cinf);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunk
(JNIEnv *env, jclass cls, jlong sdsid, jobject chunk_def, jint flags)
{
    intn rval;
    HDF_CHUNK_DEF c_def;
    jboolean bval;

/*
    bzero((char *)&c_def, sizeof(c_def));
*/

    bval = getChunkInfo(env, chunk_def,&c_def);

    /* check results */

    rval = SDsetchunk ((int32) sdsid, c_def, (int32) flags);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetchunkcache
(JNIEnv *env, jclass cls, jlong sdsid, jint maxcache, jint flags)
{
    intn rval;

    rval = SDsetchunkcache((int32)sdsid, (int32)maxcache, (int32)flags);
    if (rval < 0)
        CALL_ERROR_CHECK(-1);

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRsetexternalfile
(JNIEnv *env, jclass cls, jlong ri_id, jstring filename, jint offset)
{
    intn rval;
    const char *str;

    PIN_JAVA_STRING(filename, str, JNI_FALSE);

    rval = GRsetexternalfile((int32)ri_id, str, (int32)offset);

    UNPIN_JAVA_STRING(filename, str);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

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

    arr = ENVPTR->GetByteArrayElements(ENVPAR data,&bb);
    strt = ENVPTR->GetIntArrayElements(ENVPAR start,&bb);
    if (stride == NULL) {
        strd = NULL;
    }
    else {
        strd = ENVPTR->GetIntArrayElements(ENVPAR stride,&bb);
    }
    edg = ENVPTR->GetIntArrayElements(ENVPAR edge,&bb);

    if (strd == NULL) {
        rval = GRwriteimage((int32) ri_id, (int32 *)strt, (int32  *)NULL,
            (int32 *)edg, (VOIDP) arr);
    }
    else {
        rval = GRwriteimage((int32) ri_id, (int32 *)strt, (int32  *)strd,
            (int32 *)edg, (VOIDP) arr);
    }

    ENVPTR->ReleaseIntArrayElements(ENVPAR start,strt,JNI_ABORT);
    if (stride != NULL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR stride,strd,JNI_ABORT);
    }
    ENVPTR->ReleaseIntArrayElements(ENVPAR edge,edg,JNI_ABORT);
    ENVPTR->ReleaseByteArrayElements(ENVPAR data,arr,JNI_ABORT);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritelut
(JNIEnv *env, jclass cls, jlong pal_id, jint ncomp, jint data_type, jint interlace, jint num_entries, jbyteArray pal_data)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    arr = ENVPTR->GetByteArrayElements(ENVPAR pal_data,&bb);

    rval = GRwritelut((int32) pal_id, (int32) ncomp, (int32) data_type,
        (int32) interlace, (int32) num_entries, (VOIDP) arr);

    ENVPTR->ReleaseByteArrayElements(ENVPAR pal_data,arr,JNI_ABORT);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRreadchunk
(JNIEnv *env, jclass cls, jlong grid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte * s;
    jint *arr;
    jboolean bb;

    arr = ENVPTR->GetIntArrayElements(ENVPAR origin,&bb);
    s = ENVPTR->GetByteArrayElements(ENVPAR dat,&bb);

    rval = GRreadchunk((int32)grid,(int32 *)arr,s);

    ENVPTR->ReleaseIntArrayElements(ENVPAR origin,arr,JNI_ABORT);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR dat,s,JNI_ABORT);
        CALL_ERROR_CHECK(JNI_FALSE);
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR dat,s,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_GRwritechunk
(JNIEnv *env, jclass cls, jlong grid, jintArray origin, jbyteArray dat)
{
    int32 rval;
    jbyte * s;
    jint * arr;
    jboolean bb;

    s = ENVPTR->GetByteArrayElements(ENVPAR dat,&bb);
    arr = ENVPTR->GetIntArrayElements(ENVPAR origin,&bb);

    rval = GRwritechunk((int32)grid,(int32 *)arr,(char *)s);

    ENVPTR->ReleaseByteArrayElements(ENVPAR dat,s,JNI_ABORT);
    ENVPTR->ReleaseIntArrayElements(ENVPAR origin,arr,JNI_ABORT);

    if (rval == FAIL)
        CALL_ERROR_CHECK(JNI_FALSE);

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
