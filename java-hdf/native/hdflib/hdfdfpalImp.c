
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


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPaddpal 
( JNIEnv *env,
jobject obj, 
jstring filename,
jbyteArray palette)  /* IN:  byte[] */
{
	intn rval;
	char * f;
	jbyte *dat;
	jboolean bb;

	f = (char *) (*env)->GetStringUTFChars(env,filename,0);
	dat = (*env)->GetByteArrayElements(env,palette,&bb);

	rval = DFPaddpal((char *)f, (VOIDP) dat);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	(*env)->ReleaseByteArrayElements(env,palette,dat,JNI_ABORT);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPgetpal 
( JNIEnv *env,
jobject obj, 
jstring filename,
jbyteArray palette)  /* OUT:  byte[] */
{
	intn rval;
	char * f;
	jbyte *dat;
	jboolean bb;

	f = (char *) (*env)->GetStringUTFChars(env,filename,0);
	dat = (*env)->GetByteArrayElements(env,palette,&bb);

	rval = DFPgetpal((char *)f, (VOIDP) dat);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	if (rval == FAIL) {
		(*env)->ReleaseByteArrayElements(env,palette,dat,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseByteArrayElements(env,palette,dat,JNI_COMMIT);
		return JNI_TRUE;
	}
}

JNIEXPORT jshort JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPlastref 
( JNIEnv *env,
jobject obj) 
{
	return (DFPlastref( ));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPnpals 
( JNIEnv *env,
jobject obj, 
jstring filename)
{
	intn rval;
	char * f;
	f = (char *) (*env)->GetStringUTFChars(env,filename,0);
	rval = DFPnpals((char *)f);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPputpal 
( JNIEnv *env,
jobject obj, 
jstring filename,
jbyteArray palette, /* IN:  byte[] */
jint overwrite,
jstring filemode)
{
	intn rval;
	char * f;
	char * m;
	jbyte *dat;
	jboolean bb;

	f = (char *) (*env)->GetStringUTFChars(env,filename,0);
	m = (char *) (*env)->GetStringUTFChars(env,filemode,0);
	dat = (*env)->GetByteArrayElements(env,palette,&bb);

	rval = DFPputpal ((char *)f, (VOIDP) dat, (intn) overwrite, (char *)m);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	(*env)->ReleaseStringUTFChars(env,filemode,m);
	(*env)->ReleaseByteArrayElements(env,palette,dat,JNI_ABORT);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPreadref 
( JNIEnv *env,
jobject obj, 
jstring filename,
jshort ref)
{
	intn rval;
	char * f;
	f = (char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = DFPreadref((char *)f, (uint16) ref);
	(*env)->ReleaseStringUTFChars(env,filename,f);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jshort JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPrestart 
( JNIEnv *env,
jobject obj) 
{
	return (DFPrestart( ));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFPwriteref 
( JNIEnv *env,
jobject obj, 
jstring filename,
jshort ref)
{
	intn rval;
	char * f;
	f = (char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = DFPwriteref((char *)f, (uint16) ref);

	(*env)->ReleaseStringUTFChars(env,filename,f);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}
