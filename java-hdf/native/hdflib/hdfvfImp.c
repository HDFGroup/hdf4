
/****************************************************************************
 * NCSA HDF                                                                 *
 * National Comptational Science Alliance                                   *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/
 
/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4.1 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 *  For details of the HDF libraries, see the HDF Documentation at:
 *     http://hdf.ncsa.uiuc.edu
 *
 */

#include "hdf.h"
#include <stdio.h>

#include "jni.h"

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFfieldesize 
( JNIEnv *env,
jobject obj, 
jint vdata_id,
int field_index)
{
	return (VFfieldesize((int32) vdata_id,  (int32) field_index));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFfieldisize 
( JNIEnv *env,
jobject obj, 
jint vdata_id,
int field_index)
{

	return (VFfieldisize((int32) vdata_id,  (int32) field_index));
}

JNIEXPORT jstring JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFfieldname 
( JNIEnv *env,
jobject obj, 
jint vdata_id,
int field_index)
{
	jstring rstring;
	char * str;

	str = VFfieldname((int32) vdata_id,  (int32) field_index);

	/* check for error */

	/* convert it to java string */
	rstring = (*env)->NewStringUTF(env,str);

	return rstring;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFfieldorder 
( JNIEnv *env,
jobject obj, 
jint vdata_id,
int field_index)
{

        return (VFfieldorder((int32) vdata_id,  (int32) field_index));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFfieldtype 
( JNIEnv *env,
jobject obj, 
jint vdata_id,
int field_index)
{

        return (VFfieldtype((int32) vdata_id,  (int32) field_index));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VFnfields 
( JNIEnv *env,
jobject obj, 
jint key)
{
	return (VFnfields((int32) key));
}
