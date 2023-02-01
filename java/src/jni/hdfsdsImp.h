/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */

#include <jni.h>
/* Header for class hdf_hdflib_SDS */

#ifndef Included_hdf_hdflib_SDS
#define Included_hdf_hdflib_SDS

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_SDstart(JNIEnv *env, jclass clss, jstring filename,
                                                           jint access);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDend(JNIEnv *env, jclass clss, jlong sdid);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDfileinfo(JNIEnv *env, jclass clss, jlong sdid,
                                                             jintArray argv);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_SDselect(JNIEnv *env, jclass clss, jlong sdid, jint index);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDnametoindex(JNIEnv *env, jclass clss, jlong sdid,
                                                                jstring name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetinfo(JNIEnv *env, jclass clss, jlong sdsid,
                                                                jobjectArray name, jintArray dimsizes,
                                                                jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jintArray start, jintArray stride,
                                                                 jintArray count, jbyteArray data);

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

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata_1short(JNIEnv *env, jclass clss, jlong sdsid,
                                                                        jintArray start, jintArray stride,
                                                                        jintArray count, jshortArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata_1int(JNIEnv *env, jclass clss, jlong sdsid,
                                                                      jintArray start, jintArray stride,
                                                                      jintArray count, jintArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata_1long(JNIEnv *env, jclass clss, jlong sdsid,
                                                                       jintArray start, jintArray stride,
                                                                       jintArray count, jlongArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata_1float(JNIEnv *env, jclass clss, jlong sdsid,
                                                                        jintArray start, jintArray stride,
                                                                        jintArray count, jfloatArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreaddata_1double(JNIEnv *env, jclass clss,
                                                                         jlong sdsid, jintArray start,
                                                                         jintArray stride, jintArray count,
                                                                         jdoubleArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDendaccess(JNIEnv *env, jclass clss, jlong sdsid);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_SDgetdimid(JNIEnv *env, jclass clss, jlong sdsid,
                                                              jint index);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDdiminfo(JNIEnv *env, jclass clss, jlong dimid,
                                                                jobjectArray dimname, jintArray argv);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDidtoref(JNIEnv *env, jclass clss, jlong sdsid);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDreftoindex(JNIEnv *env, jclass clss, jlong sdid,
                                                               jint ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDattrinfo(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jint index, jobjectArray name,
                                                                 jintArray argv);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreadattr(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jint index, jbyteArray dat);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDfindattr(JNIEnv *env, jclass clss, jlong sdsid,
                                                             jstring name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDiscoordvar(JNIEnv *env, jclass clss, jlong sdsid);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetcal(JNIEnv *env, jclass clss, jlong sdsid,
                                                               jdoubleArray argv, jintArray nt);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetdatastrs(JNIEnv *env, jclass clss, jlong sdsid,
                                                                    jobjectArray strings, jint len);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetdimstrs(JNIEnv *env, jclass clss, jlong dimid,
                                                                   jobjectArray strings, jint len);

/*** note this is returning data of many types in an array of bytes.... */
/* not sure how well this will work for java programs .... */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetdimscale(JNIEnv *env, jclass clss, jlong dimid,
                                                                    jbyteArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetfillvalue(JNIEnv *env, jclass clss, jlong sdsid,
                                                                     jbyteArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetrange(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jbyteArray max, jbyteArray min);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_SDcreate(JNIEnv *env, jclass clss, jlong sd_id,
                                                            jstring name, jlong number_type, jint rank,
                                                            jintArray dimsizes);

// this API call returns false for not record and for failures
// maybe failures should return a negative?
JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDisrecord(JNIEnv *env, jclass clss, jlong sdsid);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetattr(JNIEnv *env, jclass clss, jlong s_id,
                                                                jstring attr_name, jlong num_type, jint count,
                                                                jbyteArray values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetcal(JNIEnv *env, jclass clss, jlong sds_id,
                                                               jdouble cal, jdouble cal_err, jdouble offset,
                                                               jdouble offset_err, jint number_type);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetdatastrs(JNIEnv *env, jclass clss, jlong sds_id,
                                                                    jstring label, jstring unit,
                                                                    jstring format, jstring coordsys);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetdimname(JNIEnv *env, jclass clss, jlong dim_id,
                                                                   jstring dim_name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetdimscale(JNIEnv *env, jclass clss, jlong dim_id,
                                                                    jint count, jint number_type,
                                                                    jbyteArray data);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetdimstrs(JNIEnv *env, jclass clss, jlong dim_id,
                                                                   jstring label, jstring unit,
                                                                   jstring format);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetexternalfile(JNIEnv *env, jclass clss,
                                                                        jlong sds_id, jstring filename,
                                                                        jint offset);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetfillvalue(JNIEnv *env, jclass clss, jlong sds_id,
                                                                     jbyteArray fill_val);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetrange(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jbyteArray max, jbyteArray min);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDwritedata(JNIEnv *env, jclass clss, jlong sdsid,
                                                                  jintArray start, jintArray stride,
                                                                  jintArray edge, jbyteArray data);

/* new stuff for chunking */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetnbitdataset(JNIEnv *env, jclass clss, jlong sdsid,
                                                                       jint start_bit, jint bit_len,
                                                                       jint sign_ext, jint fill_one);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetcompress(JNIEnv *env, jclass clss, jlong sdsid,
                                                                    jint type, jobject cinfo);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetcompinfo(JNIEnv *env, jclass clss, jlong sdsid,
                                                                    jobject cinfo);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetcompress(JNIEnv *env, jclass clss, jlong sdsid,
                                                                    jobject cinfo);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetaccesstype(JNIEnv *env, jclass clss, jlong sdsid,
                                                                      jint accesstype);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetblocksize(JNIEnv *env, jclass clss, jlong sdsid,
                                                                     jint block_size);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetfillmode(JNIEnv *env, jclass clss, jlong sdsid,
                                                                    jint fillmode);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetdimval_1comp(JNIEnv *env, jclass clss, jlong sdsid,
                                                                        jint comp_mode);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDisdimval_1bwcomp(JNIEnv *env, jclass clss,
                                                                         jlong dimid);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDsetchunk(JNIEnv *env, jclass clss, jlong sdsid,
                                                                 jobject chunk_def, jint flags);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDgetchunkinfo(JNIEnv *env, jclass clss, jlong sdsid,
                                                                     jobject chunk_def, jintArray cflags);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDreadchunk(JNIEnv *env, jclass clss, jlong sdid,
                                                                  jintArray origin, jbyteArray dat);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_SDsetchunkcache(JNIEnv *env, jclass clss, jlong sdsid,
                                                                  jint maxcache, jint flags);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDwritechunk(JNIEnv *env, jclass clss, jlong sdid,
                                                                   jintArray origin, jbyteArray dat);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_SDcheckempty(JNIEnv *env, jclass clss, jlong sdsid,
                                                                   jintArray emptySDS);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_SDS */
