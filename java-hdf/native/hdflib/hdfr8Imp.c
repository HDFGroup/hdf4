
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

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8getdims 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jintArray argv,  /* OUT: w, h */
jbooleanArray isp) /* OUT: isp */
{
	intn rval;
	char  *hdf_file;
	int   ispal;
	jint *theArgs;
	jboolean *theB;
        jboolean bb;

        theArgs = (*env)->GetIntArrayElements(env,argv,&bb);
        theB = (*env)->GetBooleanArrayElements(env,isp,&bb);
	hdf_file =(char *) (*env)->GetStringUTFChars(env,filename,0);

	/* get image dimension information */
	rval = DFR8getdims(hdf_file, (int32 *)&(theArgs[0]), 
		(int32 *)&(theArgs[1]), (intn *)&ispal);

	(*env)->ReleaseStringUTFChars(env,filename,hdf_file);
	if (rval == FAIL) {
		(*env)->ReleaseIntArrayElements(env,argv,theArgs,JNI_ABORT);
		theB[0] = JNI_FALSE;
		(*env)->ReleaseBooleanArrayElements(env,isp,theB,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseIntArrayElements(env,argv,theArgs,JNI_COMMIT);
		if (ispal) {
			theB[0] = JNI_TRUE;
		} else {
			theB[0] = JNI_FALSE;
		}
		(*env)->ReleaseBooleanArrayElements(env,isp,theB,JNI_ABORT);
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8getimage 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jbyteArray image, /* OUT: image data width X height X 3 */
jint width, 
jint height,
jbyteArray pallete) /* OUT: byte[] */
{
	char  *hdf_file;
	intn   rval;
	jbyte *dat;
	jbyte *p;
	jboolean bb;

	hdf_file =(char *) (*env)->GetStringUTFChars(env,filename,0);
	dat = (*env)->GetByteArrayElements(env,image,&bb);
	if (pallete == NULL) {
		rval =  DFR8getimage((char *)hdf_file, (VOIDP) dat, (int32) width, (int32) height, 
		    (uint8 *)NULL);
	} else {
		p = (*env)->GetByteArrayElements(env,pallete,&bb);
		rval =  DFR8getimage((char *)hdf_file, (VOIDP) dat, (int32) width, (int32) height, 
		    (uint8 *)p);
	}

	(*env)->ReleaseStringUTFChars(env,filename,hdf_file);
	if (rval == FAIL) {
		(*env)->ReleaseByteArrayElements(env,image,dat,JNI_ABORT);
		if (pallete != NULL) {
			(*env)->ReleaseByteArrayElements(env,pallete,p,JNI_ABORT);
		}
		return JNI_FALSE;
	} else {
		(*env)->ReleaseByteArrayElements(env,image,dat,JNI_COMMIT);
		if (pallete != NULL) {
			(*env)->ReleaseByteArrayElements(env,pallete,p,JNI_COMMIT);
		}
		return JNI_TRUE;
	}

}

JNIEXPORT jshort JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8lastref 
( JNIEnv *env,
jobject obj) 
{
	return ((short)DFR8lastref());
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8readref 
( JNIEnv *env,
jobject obj, 
jstring filename,
jshort ref)
{
	int  retVal;
	char *filePtr;
	filePtr =(char *) (*env)->GetStringUTFChars(env,filename,0);

	retVal = DFR8readref(filePtr, (short)ref);

	(*env)->ReleaseStringUTFChars(env,filename,filePtr);
	if (retVal == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8restart 
( JNIEnv *env,
jobject obj) 
{
	int retVal;
	retVal = DFR8restart();

	if (retVal) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8nimages 
( JNIEnv *env,
jobject obj, 
jstring hdfFile)
{
	char  *hdf_file;

	hdf_file =(char *) (*env)->GetStringUTFChars(env,hdfFile,0);
	return(DFR8nimages(hdf_file));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8addimage 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jbyteArray image, /* IN: image data width X height X 3 */
jint width, 
jint height,
jshort compress)
{
	intn rval;
	char  *f;
	jbyte *dat;
	jboolean bb;

	f =(char *) (*env)->GetStringUTFChars(env,filename,0);
	dat = (*env)->GetByteArrayElements(env,image,&bb);

	rval = DFR8addimage((char *)f, (VOIDP) dat, (int32) width, (int32) height, 
	    (uint16)compress);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	(*env)->ReleaseByteArrayElements(env,image,dat,JNI_ABORT);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8putimage 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jbyteArray image, /* IN: image data width X height X 3 */
jint width, 
jint height,
jint compress)
{
	intn rval;
	char  *f;
	jbyte *dat;
	jboolean bb;

	f =(char *) (*env)->GetStringUTFChars(env,filename,0);
	dat = (*env)->GetByteArrayElements(env,image,&bb);

	rval = DFR8putimage((char *)f, (VOIDP) dat, (int32) width, (int32) height,
	    (uint16)compress);

	(*env)->ReleaseStringUTFChars(env,filename,f);
	(*env)->ReleaseByteArrayElements(env,image,dat,JNI_ABORT);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8setcompress 
( JNIEnv *env,
jobject obj, 
jint type, 
jobject cinfo)
{
	jboolean bval;
	intn rval;
	comp_info cinf;

	bval = getOldCompInfo(env,obj,cinfo,&cinf);	

	/* check for success... */

	/* fill in cinf depending on the value of 'type' */
	rval = DFR8setcompress((int32) type, (comp_info *)&cinf);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8getpalref 
( JNIEnv *env,
jobject obj, 
jshortArray palref) /* OUT: Short */
{
	int rval;
	short *theArgs;
        jboolean bb;

        theArgs = (*env)->GetShortArrayElements(env,palref,&bb);

	rval = DFR8getpalref((uint16 *)&(theArgs[0]));

	if (rval == FAIL) {
		(*env)->ReleaseShortArrayElements(env,palref,theArgs,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseShortArrayElements(env,palref,theArgs,JNI_COMMIT);
		return JNI_TRUE;
	}
}



JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8setpalette 
( JNIEnv *env,
jobject obj, 
jbyteArray palette) /* IN:  byte[] */
{
	int  rval;
	jbyte *p;
	jboolean bb;

	p = (*env)->GetByteArrayElements(env,palette,&bb);

	rval = DFR8setpalette((uint8 *)p);

	(*env)->ReleaseByteArrayElements(env,palette,p,JNI_ABORT);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFR8writeref 
( JNIEnv *env,
jobject obj, 
jstring filename,
jshort ref)
{
	int  retVal;
	char *filePtr;
	filePtr =(char *) (*env)->GetStringUTFChars(env,filename,0);

	retVal = DFR8writeref(filePtr, (short)ref);

	(*env)->ReleaseStringUTFChars(env,filename,filePtr);
	if (retVal == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}
