
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


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANstart 
( JNIEnv *env,
jobject obj, 
jint file_id)
{
	int32 anid;

	return ANstart((int32)file_id);
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANend 
( JNIEnv *env,
jobject obj, 
jint anid)
{
	int32 retVal;

	retVal = ANend((int32)anid);

	if (retVal == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANfileinfo 
( JNIEnv *env,
jobject obj, 
jint anid,
jintArray info)  /* OUT: n_file_label, n_file_desc, n_data_label, n_data_desc */
{
	int32 retVal;
	jint *theArgs;
	jboolean bb;

	theArgs = (*env)->GetIntArrayElements(env,info,&bb);

	retVal = ANfileinfo((int32)anid, (int32 *)&(theArgs[0]), 
		(int32 *)&(theArgs[1]), (int32 *)&(theArgs[2]), 
		(int32 *)&(theArgs[3]));

	if (retVal == FAIL) {
		(*env)->ReleaseIntArrayElements(env,info,theArgs,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseIntArrayElements(env,info,theArgs,JNI_COMMIT);
		return JNI_TRUE;
	}
}


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANselect 
( JNIEnv *env,
jobject obj, 
jint anid,
jint index,
jint anntype)
{
	return(ANselect((int32)anid, (int32)index, (int32)anntype));
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANendaccess 
( JNIEnv *env,
jobject obj, 
jint ann_id)
{
	int32 retVal;

	retVal = ANendaccess((int32)ann_id);

	if (!retVal) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANnumann 
( JNIEnv *env,
jobject obj, 
jint an_id,
jint anntype, 
jshort tag, 
jshort ref)
{
	return( ANnumann((int32)an_id, (int32)anntype, (uint16)tag, (uint16)ref) );
}

JNIEXPORT jshort JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANatype2tag 
( JNIEnv *env,
jobject obj, 
jint antype)
{
	return (jshort)ANatype2tag((int32)antype);
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANtag2atype 
( JNIEnv *env,
jobject obj, 
jint antag)
{
	return (jint)ANtag2atype((uint16)antag);
}


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANannlist 
( JNIEnv *env,
jobject obj, 
jint an_id,
jint anntype, 
jshort tag, 
jshort ref,
jintArray annlist  /* OUT: int[] */
)
{
	int32 retVal;
	int32 annNum;
	jint *iarr;
	jboolean bb;

	iarr = (*env)->GetIntArrayElements(env,annlist,&bb);

	retVal = ANannlist((int32)an_id, (int32)anntype, (int32)tag,(int32)ref,
		     (int32 *)iarr);

	if (retVal == FAIL) {
		(*env)->ReleaseIntArrayElements(env,annlist,iarr,JNI_ABORT);
	} else {
		(*env)->ReleaseIntArrayElements(env,annlist,iarr,JNI_COMMIT);
	}
	return retVal;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANannlen 
( JNIEnv *env,
jobject obj, 
jint ann_id)
{
	return ANannlen((int32)ann_id);
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANreadann 
( JNIEnv *env,
jobject obj, 
jint ann_id,
jobjectArray annbuf, /* OUT: string */
jint maxlen)
{
	int32 retVal;
	char  *data;
	jmethodID Sjmi;
	jclass Sjc;
	jstring rstring;
	jobject o;
	jboolean bb;

	data = (char *)HDmalloc(maxlen*sizeof(char));

	if (data == NULL) {
		/* Exception */
		return JNI_FALSE;
	}

	/* read annotation from HDF */
	retVal = ANreadann((int32)ann_id, data, (int32)maxlen);

	if (retVal == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
		return JNI_FALSE;
	} else {

		o = (*env)->GetObjectArrayElement(env,annbuf,0);
		if (o == NULL) {
			return JNI_FALSE;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			return JNI_FALSE;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			return JNI_FALSE;
		}

		rstring = (*env)->NewStringUTF(env, data);
		(*env)->SetObjectArrayElement(env,annbuf,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);

		return JNI_TRUE;
	}
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANcreate 
( JNIEnv *env,
jobject obj, 
jint an_id,
jshort tag, 
jshort ref,
jint type)
{
	return (ANcreate((int32) an_id, (uint16) tag, (uint16) ref, (ann_type) type));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANcreatef 
( JNIEnv *env,
jobject obj, 
jint an_id,
jint type)
{
	return (ANcreatef((int32) an_id, (ann_type) type));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANdestroy 
( JNIEnv *env,
jobject obj)
{
	intn rval = ANdestroy( );
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANget_tagref 
( JNIEnv *env,
jobject obj, 
jint an_id,
jint index,  
jint type, 
jshortArray tagref) /* OUT: short tag, ref */
{
	int32 rval;
	short *theArgs;
	jboolean bb;

	theArgs = (*env)->GetShortArrayElements(env,tagref,&bb);

	rval = ANget_tagref((int32) an_id, (int32) index,  (ann_type) type, (uint16 *)&(theArgs[0]), (uint16 *)&(theArgs[1]));

	if (rval == FAIL) {
		(*env)->ReleaseShortArrayElements(env,tagref,theArgs,JNI_ABORT);
	} else {
		(*env)->ReleaseShortArrayElements(env,tagref,theArgs,JNI_COMMIT);
	}
	return rval;
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANid2tagref 
( JNIEnv *env,
jobject obj, 
jint an_id,
jshortArray tagref) /* OUT: short tag, ref */
{
	int32 rval;
	short *theArgs;
	jboolean bb;

	theArgs = (*env)->GetShortArrayElements(env,tagref,&bb);

	rval =  ANid2tagref((int32) an_id, (uint16 *)&(theArgs[0]),  
		(uint16 *)&(theArgs[1]));

	if (rval == FAIL) {
		(*env)->ReleaseShortArrayElements(env,tagref,theArgs,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseShortArrayElements(env,tagref,theArgs,JNI_COMMIT);
		return JNI_TRUE;
	}
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANtagref2id 
( JNIEnv *env,
jobject obj, 
jint an_id,
jshort tag,
jshort ref
)
{
	return( ANtagref2id((int32) an_id, (uint16) tag, (uint16) ref));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_ANwriteann 
( JNIEnv *env,
jobject obj, 
jint ann_id,
jstring label, 
jint ann_length)
{

	intn rval;
	char * str;

	str =(char *) (*env)->GetStringUTFChars(env,label,0);

	/* should check that str is as long as ann_length.... */

	rval = ANwriteann((int32) ann_id, str, (int32) ann_length);

	(*env)->ReleaseStringUTFChars(env,label,str);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}
