
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

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANaddfds 
( JNIEnv *env,
jobject obj, 
jint file_id,
jstring description, 
jint desc_len)
{
	intn rval;
	char * str;

	str =(char *) (*env)->GetStringUTFChars(env,description,0);

	/* should check that str is as long as desc_length.... */

	rval = DFANaddfds((int32) file_id, (char *)str, (int32) desc_len);

	(*env)->ReleaseStringUTFChars(env,description,str);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANaddfid 
( JNIEnv *env,
jobject obj, 
jint file_id,
jstring label)
{
	intn rval;
	char *str;

	str =(char *) (*env)->GetStringUTFChars(env,label,0);

	/* should check that str is as long as desc_length.... */

	rval = DFANaddfid((int32) file_id, (char *)str);

	(*env)->ReleaseStringUTFChars(env,label,str);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANclear 
( JNIEnv *env,
jobject obj)
{
	intn rval;
	rval = DFANclear( );
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetdesc 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref,
jobjectArray desc_buf,  /* OUT:  String */
jint buf_len)
{
	intn rval;
	char  *data;
	char  *str;
	jmethodID Sjmi;
	jclass Sjc;
	jstring rstring;
	jobject o;
	jboolean bb;


	data = (char *)HDmalloc(buf_len*sizeof(char));

	if (data == NULL) {
		/* Exception */
		return JNI_FALSE;
	}

	str =(char *) (*env)->GetStringUTFChars(env,filename,0);

	/* read annotation */
	rval = DFANgetdesc((char *)str, (uint16) tag, (uint16) ref, 
	    (char *)data, (int32) buf_len);

	if (rval == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
		(*env)->ReleaseStringUTFChars(env,filename,str);
		return JNI_FALSE;
	} else {

		(*env)->ReleaseStringUTFChars(env,filename,str);

		rstring = (*env)->NewStringUTF(env, data);
		o = (*env)->GetObjectArrayElement(env,desc_buf,0);
		if (o == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return JNI_FALSE;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return JNI_FALSE;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			if (data != NULL)
				HDfree((char *)data);
			return JNI_FALSE;
		}
		(*env)->SetObjectArrayElement(env,desc_buf,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);
		return JNI_TRUE;
	}
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetdesclen 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref)
{
	int32 rval;
	char * str;

	str =(char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = DFANgetdesclen((char *)str, (uint16) tag, (uint16) ref);

	(*env)->ReleaseStringUTFChars(env,filename,str);

	return rval;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetfds 
( JNIEnv *env,
jobject obj, 
jint file_id, 
jobjectArray desc_buf,  /* OUT: String */
jint buf_len, 
jint isfirst)
{
	int32 rval;
	char  *data;
	jclass Sjc;
	jstring rstring;
	jobject o;
	jboolean bb;


	data = (char *)HDmalloc(buf_len*sizeof(char));

	if (data == NULL) {
		/* Exception */
		return FAIL;
	}

	/* should check that data is 'buf_len' long */

	rval = DFANgetfds((int32) file_id, (char *)data, (int32) buf_len, (intn) isfirst);

	if (rval == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
	} else {

		rstring = (*env)->NewStringUTF(env, data);
		o = (*env)->GetObjectArrayElement(env,desc_buf,0);
		if (o == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		(*env)->SetObjectArrayElement(env,desc_buf,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);

	}
	return rval;

}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetfdslen 
( JNIEnv *env,
jobject obj, 
jint file_id, 
jint isfirst)
{
	return DFANgetfdslen((int32) file_id, (intn) isfirst);
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetfid 
( JNIEnv *env,
jobject obj, 
jint file_id, 
jobjectArray desc_buf,  
jint buf_len, 
jint isfirst)
{
	int32 rval;
	char  *data;
	jclass Sjc;
	jstring rstring;
	jobject o;
	jboolean bb;


	data = (char *)HDmalloc(buf_len*sizeof(char));

	if (data == NULL) {
		/* Exception */
		return FAIL;
	}

	/* should check that data is 'buf_len' long */

	rval = DFANgetfid((int32) file_id, (char *)data,  (int32) buf_len, (intn) isfirst);
	if (rval == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
	} else {

		rstring = (*env)->NewStringUTF(env, data);
		o = (*env)->GetObjectArrayElement(env,desc_buf,0);
		if (o == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		(*env)->SetObjectArrayElement(env,desc_buf,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);
	}
	return rval;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetfidlen 
( JNIEnv *env,
jobject obj, 
jint file_id, 
jint isfirst)
{
	return(DFANgetfidlen((int32) file_id, (intn) isfirst));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetlabel 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref,
jobjectArray label_buf, 
jint buf_len)
{
	intn rval;
	char  *data;
	jclass Sjc;
	jstring rstring;
	char *str;
	jobject o;
	jboolean bb;


	data = (char *)HDmalloc(buf_len*sizeof(char));

	if (data == NULL) {
		/* Exception */
		return FAIL;
	}

	/* should check lenght of buffer */

	str =(char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = DFANgetlabel((char *)str, (uint16) tag, (uint16) ref,
	    (char *)data, (int32) buf_len);

	if (rval == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
		(*env)->ReleaseStringUTFChars(env,filename,str);
	} else {

		(*env)->ReleaseStringUTFChars(env,filename,str);
		rstring = (*env)->NewStringUTF(env, data);
		o = (*env)->GetObjectArrayElement(env,label_buf,0);
		if (o == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		(*env)->SetObjectArrayElement(env,label_buf,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);
	}
	return rval;
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANgetlablen 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref)
{
	int32 rval;
	char *str;

	str =(char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = DFANgetlablen((char *)str, (uint16) tag, (uint16) ref);

	(*env)->ReleaseStringUTFChars(env,filename,str);

	return rval;

}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANlablist 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshortArray ref_list,  /* OUT:  short[] */
jobjectArray label_list, /* OUT: String */
jint list_len, 
jint label_len, 
jint start_pos)
{
	int rval;
	char  *data;
	jmethodID Sjmi;
	jclass Sjc;
	jstring rstring;
	char *args[2];
	char *str;
	jshort *sarr;
	jboolean bb;
	jobject o;

	data = (char *)HDmalloc(label_len * (list_len - 1));

	if (data == NULL) {
		/* Exception */
		return FAIL;
	}


	sarr = (*env)->GetShortArrayElements(env,ref_list,&bb);

	/* should check length of buffer */
	str =(char *) (*env)->GetStringUTFChars(env,filename,0);


	rval = DFANlablist((char *)str, (uint16) tag, (uint16 *) sarr, 
	    (char *)data, (int) list_len, (intn) label_len, (intn) start_pos);


	if (rval == FAIL) {
		if (data != NULL)
			HDfree((char *)data);
		(*env)->ReleaseStringUTFChars(env,filename,str);
		(*env)->ReleaseShortArrayElements(env,ref_list,(jshort *)sarr, JNI_ABORT);
	} else {

		(*env)->ReleaseStringUTFChars(env,filename,str);
		(*env)->ReleaseShortArrayElements(env,ref_list,(jshort *)sarr, JNI_COMMIT);

		rstring = (*env)->NewStringUTF(env, data);
		o = (*env)->GetObjectArrayElement(env,label_list,0);
		if (o == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			if (data != NULL)
				HDfree((char *)data);
			return FAIL;
		}
		(*env)->SetObjectArrayElement(env,label_list,0,(jobject)rstring);

		if (data != NULL)
			HDfree((char *)data);
	}
	return rval;
}

JNIEXPORT jshort JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANlastref 
( JNIEnv *env,
jobject obj) 
{
	return (DFANlastref( ));
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANputdesc 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref,
jstring description, 
jint desc_len)
{
	int rval;
	char *fn;
	char *str;

	fn =(char *) (*env)->GetStringUTFChars(env,filename,0);
	str =(char *) (*env)->GetStringUTFChars(env,description,0);

	/* should check length of description */

	rval = DFANputdesc((char *)fn, (uint16) tag, (uint16) ref,
	    (char *)str, (int32) desc_len);

	(*env)->ReleaseStringUTFChars(env,filename,fn);
	(*env)->ReleaseStringUTFChars(env,description,str);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_DFANputlabel 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jshort tag, 
jshort ref,
jstring label)
{
	intn rval;
	char *fn;
	char *str;

	fn =(char *) (*env)->GetStringUTFChars(env,filename,0);
	str =(char *) (*env)->GetStringUTFChars(env,label,0);

	/* should check length of description */

	rval = DFANputlabel((char *)fn, (uint16) tag, (uint16) ref, (char *)str);

	(*env)->ReleaseStringUTFChars(env,filename,fn);
	(*env)->ReleaseStringUTFChars(env,label,str);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}
