
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

JNIEXPORT jint JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HEvalue 
( JNIEnv *env,
jobject obj, 
jint level)
{
	return HEvalue((int32) level);
}

JNIEXPORT void JNICALL Java_ncsa_hdf_hdflib_HDFDeprecated_HEprint 
( JNIEnv *env,
jobject obj, 
jobject stream,  /* FILE * to output to? */
jint level)
{
printf("HEprint called:\n");fflush(stdout);
	/* HEprint is not implemented 
		because I don't know HOW to implement it !*/
	/* Exception....*/
	NotImplemented( env, obj, "HEprint");
}

JNIEXPORT jstring JNICALL Java_ncsa_hdf_hdflib_HDFLibrary_HEstring 
( JNIEnv *env,
jobject obj, 
jshort error_code)
{
	char * str;
	jstring rstring;

	str = (char *)HEstring((int16)error_code);

	rstring = (*env)->NewStringUTF(env,str);

	return rstring;
}
