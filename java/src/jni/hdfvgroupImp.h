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
/* Header for class hdf_hdflib_VGROUP */

#ifndef Included_hdf_hdflib_VGROUP
#define Included_hdf_hdflib_VGROUP

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vstart(JNIEnv *env, jclass clss, jlong fid);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_Vattach(JNIEnv *env, jclass clss, jlong fid,
                                                           jint vgroup_ref, jstring accessmode);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_Vdetach(JNIEnv *env, jclass clss, jlong vgroup_id);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_Vend(JNIEnv *env, jclass clss, jlong fid);

JNIEXPORT jlong JNICALL Java_hdf_hdflib_HDFLibrary_Vgetid(JNIEnv *env, jclass clss, jlong fid,
                                                          jint vgroup_ref);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_Vgetclass(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                            jobjectArray hdfclassname);

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFLibrary_Vgetname(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                           jobjectArray hdfname);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Visvg(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                            jint vgroup_ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Visvs(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                            jint vdata_ref);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vgettagrefs(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                              jintArray tags, jintArray refs, jint size);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vgettagref(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                                 jint index, jintArray tagref);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vntagrefs(JNIEnv *env, jclass clss, jlong vgroup_id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vinqtagref(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                             jint tag, jint ref);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vlone(JNIEnv *env, jclass clss, jlong fid,
                                                        jintArray ref_array, jint arraysize);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vaddtagref(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                             jint tag, jint ref);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vdeletetagref(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                                jint tag, jint ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vclose_I(JNIEnv *env, jclass clss, jlong file_id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vfind(JNIEnv *env, jclass clss, jlong file_id,
                                                        jstring vgname);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vfindclass(JNIEnv *env, jclass clss, jlong file_id,
                                                             jstring vgclassname);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vflocate(JNIEnv *env, jclass clss, jint key, jstring field);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vgetnext(JNIEnv *env, jclass clss, jint vkey,
                                                           jint elem_ref);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vinquire(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                               jintArray n_entries, jobjectArray vgroup_name);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vinsert(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                          jint v_id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vnrefs(JNIEnv *env, jclass clss, jint vkey, jint tag);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vopen(JNIEnv *env, jclass clss, jstring filename,
                                                        jint access, jshort ndds);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vsetclass(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                                jstring hdfclassname);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vsetname(JNIEnv *env, jclass clss, jlong vgroup_id,
                                                               jstring name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vattrinfo(JNIEnv *env, jclass clss, jlong id,
                                                                jint index, jobjectArray name,
                                                                jintArray argv);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vfindattr(JNIEnv *env, jclass clss, jlong id, jstring name);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vgetattr(JNIEnv *env, jclass clss, jlong gr_id,
                                                               jint attr_index, jbyteArray values);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vgetversion(JNIEnv *env, jclass clss, jlong id);

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_Vnattrs(JNIEnv *env, jclass clss, jlong id);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JILjava_lang_String_2(
    JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JI_3B(
    JNIEnv *env, jclass clss, jlong id, jstring attr_name, jlong data_type, jint count, jbyteArray values);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_hdf_hdflib_VGROUP */
