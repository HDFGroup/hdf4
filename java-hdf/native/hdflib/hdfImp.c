
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
#include "hfile.h"

#include <stdio.h>

#include "jni.h"


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hopen 
(
JNIEnv *env,
jobject obj, 
jstring hdfFile,
jint access)

{

	char* file;
	int  retVal;
	int errval;
	jclass jc;

	file =(char *) (*env)->GetStringUTFChars(env,hdfFile,0);

	if (file == NULL) {
		/* call failed? */
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFJavaException");
		if (jc == NULL) {
			return -1; /* exception is raised */
		}
		(*env)->ThrowNew(env,jc,"Hopen: GetStringUTFChars failed");
	}

	/* open HDF file specified by ncsa_hdf_HDF_file */
	retVal = Hopen((char *)file, access, 0);
	(*env)->ReleaseStringUTFChars(env,hdfFile,file);

	if (retVal == FAIL) {
		/* check for error */
		/* for now:  use top of exception stack:  fix this
                   to do whole stack */
		errval = HEvalue(1);
		if (errval != DFE_NONE) {
			buildException( env, obj, errval );
			jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFLibraryException");
			if (jc == NULL) {
				return -1; /* exception is raised */
			}
			(*env)->ThrowNew(env,jc,HEstring(errval));
		}
		return -1;
	}
	else {
		return retVal;
	}

}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hclose 
( JNIEnv *env,
jobject obj, 
jint fid)
{
	int status = 0;

	if (fid < 0) {
		/* maybe not an exception -- the file is already closed? */
		return JNI_FALSE;
	} else {
		/* close the HDF file */
		status = Hclose(fid);
		if (status == FAIL) {
			/* ideally, return an exception explaining the
					   reason, especially for DFE_OPENAID error
					*/
			return JNI_FALSE;
		} else {
			return JNI_TRUE;
		}

	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hishdf 
( JNIEnv *env,
jobject obj, 
jstring hdfFile)
{

	char * hfile;
	int  retVal;

	hfile = (char *)(*env)->GetStringUTFChars(env,hdfFile,0);

	/* open HDF file specified by ncsa_hdf_HDF_file */
	retVal = Hishdf((char *)hfile);
	(*env)->ReleaseStringUTFChars(env,hdfFile,hfile);
	if (retVal == FALSE) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}

}


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hnumber 
( JNIEnv *env,
jobject obj, 
jint fid)
{
	return (Hnumber(fid, DFTAG_WILDCARD));
}

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_DFKNTsize 
( JNIEnv *env,
jobject obj, 
jint numbertype)
{
	return (DFKNTsize(numbertype));
}



JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hcache 
( JNIEnv *env,
jobject obj, 
jint file_id,
jint cache_switch)
{

	intn rval;
	rval =  Hcache((int32) file_id, (intn) cache_switch);

	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hgetfileversion 
( JNIEnv *env,
jobject obj, 
jint file_id,
jintArray vers,  /* OUT: int major_v, minor_v, release */
jobjectArray string)  /* OUT: String[] */
{
	intn rval;
	jclass Sjc;
	char s[LIBVSTR_LEN] ;
	jstring name;
	jint *theArgs;
	jboolean bb;
	jobject o;

	theArgs = (*env)->GetIntArrayElements(env,vers,&bb);

	rval = Hgetfileversion((int32) file_id, (uint32 *)&(theArgs[0]), 
		(uint32 *)&(theArgs[1]), (uint32 *)&(theArgs[2]), s);

	if (rval == FAIL) {
		(*env)->ReleaseIntArrayElements(env,vers,theArgs,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseIntArrayElements(env,vers,theArgs,JNI_COMMIT);
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			return JNI_FALSE;
		}
		o = (*env)->GetObjectArrayElement(env,string,0);
		if (o == NULL) {
			return JNI_FALSE;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			/* exception */
			return JNI_FALSE;
		}
		name = (*env)->NewStringUTF(env,s);
		if (name != NULL) {
                        (*env)->SetObjectArrayElement(env,string,0,(jobject)name);
		}
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hgetlibversion 
( JNIEnv *env,
jobject obj, 
jintArray vers,  /* OUT: int major_v, minor_v, release */
jobjectArray string)  /* OUT: String[] */
{
	intn rval;
	jclass Sjc;
	char s[LIBVSTR_LEN] ;
	jint *theArgs;
	jstring name;
	jobject o;
	jboolean bb;

	theArgs = (*env)->GetIntArrayElements(env,vers,&bb);

	rval = Hgetlibversion((uint32 *)&(theArgs[0]), 
		(uint32 *)&(theArgs[1]), (uint32 *)&(theArgs[2]), s);

	if (rval == FAIL) {
		(*env)->ReleaseIntArrayElements(env,vers,theArgs,JNI_ABORT);
		return JNI_FALSE;
	} else {
		(*env)->ReleaseIntArrayElements(env,vers,theArgs,JNI_COMMIT);
		Sjc = (*env)->FindClass(env, "java/lang/String");
		if (Sjc == NULL) {
			return JNI_FALSE;
		}
		o = (*env)->GetObjectArrayElement(env,string,0);
		if (o == NULL) {
			return JNI_FALSE;
		}
		bb = (*env)->IsInstanceOf(env,o,Sjc);
		if (bb == JNI_FALSE) {
			/* exception */
			return JNI_FALSE;
		}
		name = (*env)->NewStringUTF(env,s);
		if (name != NULL) {
                        (*env)->SetObjectArrayElement(env,string,0,(jobject)name);
		}
		return JNI_TRUE;
	}
}


JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hsetaccesstype 
( JNIEnv *env,
jobject obj, 
jint h_id,
jint  access_type)
{
	int32 rval;

	rval = Hsetaccesstype((int32) h_id, (uintn)  access_type);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_Hsync 
( JNIEnv *env,
jobject obj, 
jint file_id)
{
	intn rval;

	rval = Hsync((int32) file_id);
	if (rval == FAIL) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HDFclose 
( JNIEnv *env,
jobject obj, 
jint file_id)
{
	intn rval;

	rval = Hclose((int32) file_id);

	if (rval == FAIL)  {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}


JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HDFopen 
( JNIEnv *env,
jobject obj, 
jstring filename, 
jint access, 
jshort n_dds)
{
	int32 rval;
	char * str;

        str =(char *) (*env)->GetStringUTFChars(env,filename,0);

	rval = HDFopen((char *)str, (intn) access, (int16) n_dds);

	(*env)->ReleaseStringUTFChars(env,filename,str);

	return rval;
}


#ifdef not_yet_implemented
JNIEXPORT jboolean JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HDFflusdd
( JNIEnv *env,
jobject obj, 
jint file_id)
{
intn rval;
	
	rval = Hflushdd((int32)file_id);

	if (rval == FAIL)  {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}
#endif

JNIEXPORT jstring JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HDgetNTdesc 
( JNIEnv *env,
jobject obj, 
jint nt)
{
char *rval;
jstring rstring;

      rval = HDgetNTdesc((int32) nt);

      if (rval != NULL) {
              rstring = (*env)->NewStringUTF(env, rval);
      } else {
              rstring = NULL;
      }

      return rstring;
}

