
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

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VHmakegroup 
( JNIEnv *env,
jobject obj, 
jint file_id,
jintArray tag_array, /* IN: int[] */
jintArray ref_array, /* IN: int[] */
jint n_objects, 
jstring vgroup_name, 
jstring vgroup_class)
{
int32 rval;
jint *tags;
jint *refs;
char *name;
char *class;
	jboolean bb;

 	tags = (*env)->GetIntArrayElements(env,tag_array,&bb);

 	refs = (*env)->GetIntArrayElements(env,ref_array,&bb);

	name = (char *)(*env)->GetStringUTFChars(env,vgroup_name,0);

	class = (char *)(*env)->GetStringUTFChars(env,vgroup_class,0);

	rval = VHmakegroup((int32) file_id, (int32 *) tags, (int32 *)refs, 
		(int32) n_objects, (char *)name, (char *)class);

	(*env)->ReleaseIntArrayElements(env,tag_array,tags,JNI_ABORT);
	(*env)->ReleaseIntArrayElements(env,ref_array,refs,JNI_ABORT);
	(*env)->ReleaseStringUTFChars(env,vgroup_name,name);
	(*env)->ReleaseStringUTFChars(env,vgroup_class,class);

	return rval;
}
	
JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VHstoredata 
( JNIEnv *env,
jobject obj, 
jint file_id,
jstring fieldname, 
jintArray buf, /* IN: byte[] */
jint n_records, 
jint data_type, 
jstring vdata_name, 
jstring vdata_class)
{
int32 rval;
jbyte *buffer;
char *fldname;
char *name;
char *class;
	jboolean bb;

 	buffer = (*env)->GetByteArrayElements(env,buf,&bb);

	fldname = (char *)(*env)->GetStringUTFChars(env,fieldname,0);

	name = (char *)(*env)->GetStringUTFChars(env,vdata_name,0);

	class = (char *)(*env)->GetStringUTFChars(env,vdata_class,0);


	rval = VHstoredata((int32) file_id, (char *)fldname, 
		(uint8 *) buffer, (int32) n_records, (int32) data_type, 
		(char *)name, (char *)class);

	(*env)->ReleaseByteArrayElements(env,buf,buffer,JNI_ABORT);
	(*env)->ReleaseStringUTFChars(env,vdata_name,name);
	(*env)->ReleaseStringUTFChars(env,vdata_class,class);
	(*env)->ReleaseStringUTFChars(env,fieldname,fldname);

	return rval;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_VHstoredatam 
( JNIEnv *env,
jobject obj, 
jint file_id,
jstring fieldname, 
jbyteArray buf, /* IN: byte[] */
jint n_records, 
jint data_type, 
jstring vdata_name, 
jstring vdata_class, 
jint order)
{
int32 rval;
jbyte *buffer;
char *fldname;
char *name;
char *class;
jboolean bb;

 	buffer = (*env)->GetByteArrayElements(env,buf,&bb);

	fldname = (char *)(*env)->GetStringUTFChars(env,fieldname,0);

	name = (char *)(*env)->GetStringUTFChars(env,vdata_name,0);

	class = (char *)(*env)->GetStringUTFChars(env,vdata_class,0);

	rval = VHstoredatam((int32) file_id, (char *)fldname, 
		(uint8 *) buffer, (int32) n_records, (int32) data_type, 
		(char *)name, (char *)class, (int32) order);

	(*env)->ReleaseByteArrayElements(env,buf,buffer,JNI_ABORT);
	(*env)->ReleaseStringUTFChars(env,vdata_name,name);
	(*env)->ReleaseStringUTFChars(env,vdata_class,class);
	(*env)->ReleaseStringUTFChars(env,fieldname,fldname);

	return rval;
}
