
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
 *  This module contains the implementation of all the native methods
 *  used for number conversion.  This is represented by the Java
 *  class HDFNativeData.
 *
 *  These routines convert one dimensional arrays of bytes into
 *  one-D arrays of other types (int, float, etc) and vice versa.
 *
 *  These routines are called from the Java parts of the Java-C
 *  interface.
 *
 *  ***Important notes:
 *
 *     1.  These routines are designed to be portable--they use the
 *         C compiler to do the required native data manipulation.
 *     2.  These routines copy the data at least once -- a serious
 *         but unavoidable performance hit.
 */
#include "hdf.h"

#include <stdio.h>

#include "jni.h"

/* returns int [] */
JNIEXPORT jintArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToInt___3B 
( JNIEnv *env,
jobject obj, 
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	jbyte *barr;
	jintArray rarray;
	int blen;
	jint *iarray;
	jboolean bb;

	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	blen = (*env)->GetArrayLength(env,bdata);

	rarray = (*env)->NewIntArray(env,(blen/sizeof(jint)));
	iarray = (*env)->GetIntArrayElements(env,rarray,&bb);


	(*env)->ReleaseIntArrayElements(env,rarray,(jint *)barr, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns float [] */
JNIEXPORT jfloatArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToFloat___3B 
( JNIEnv *env,
jobject obj, 
jfloatArray bdata)  /* IN: array of bytes */
{
	intn rval;
	jbyte *barr;
	jfloatArray rarray;
	int blen;
	jfloat *farray;
	jboolean bb;

	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	blen = (*env)->GetArrayLength(env,bdata);

	rarray = (*env)->NewFloatArray(env,(blen/sizeof(jfloat)));
	farray = (*env)->GetFloatArrayElements(env,rarray,&bb);


	(*env)->ReleaseFloatArrayElements(env,rarray,(jfloat *)barr, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns short [] */
JNIEXPORT jshortArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToShort___3B 
( JNIEnv *env,
jobject obj, 
jshortArray bdata)  /* IN: array of bytes */
{
	intn rval;
	jbyte *barr;
	jshortArray rarray;
	int blen;
	jshort *sarray;
	jboolean bb;

	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	blen = (*env)->GetArrayLength(env,bdata);

	rarray = (*env)->NewShortArray(env,(blen/sizeof(jshort)));
	sarray = (*env)->GetShortArrayElements(env,rarray,&bb);


	(*env)->ReleaseShortArrayElements(env,rarray,(jshort *)barr, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}


/* returns long [] */
JNIEXPORT jlongArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToLong___3B 
( JNIEnv *env,
jobject obj, 
jlongArray bdata)  /* IN: array of bytes */
{
	intn rval;
	jbyte *barr;
	jlongArray rarray;
	int blen;
	jlong *larray;
	jboolean bb;

	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	blen = (*env)->GetArrayLength(env,bdata);

	rarray = (*env)->NewLongArray(env,(blen/sizeof(jlong)));
	larray = (*env)->GetLongArrayElements(env,rarray,&bb);


	(*env)->ReleaseLongArrayElements(env,rarray,(jlong *)barr, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}


/* returns double [] */
JNIEXPORT jdoubleArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToDouble___3B 
( JNIEnv *env,
jobject obj, 
jdoubleArray bdata)  /* IN: array of bytes */
{
	intn rval;
	jbyte *barr;
	jdoubleArray rarray;
	int blen;
	jdouble *darray;
	jboolean bb;

	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	blen = (*env)->GetArrayLength(env,bdata);

	rarray = (*env)->NewDoubleArray(env,(blen/sizeof(jdouble)));
	darray = (*env)->GetDoubleArrayElements(env,rarray,&bb);


	(*env)->ReleaseDoubleArrayElements(env,rarray,(jdouble *)barr, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;
}


/* returns int [] */
JNIEXPORT jintArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToInt__II_3B 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	char *bp;
	jbyte *barr;
	jintArray rarray;
	int blen;
	jint *iarray;
	jint *iap;
	int ii;
	jboolean bb;

	if (bdata == NULL) {
		printf("Exception: bdata is NULL?\n");
		return NULL;
	}
	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	if (barr == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	blen = (*env)->GetArrayLength(env,bdata);
	if ((start < 0) || ((start + (len*sizeof(jint))) > blen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	bp = (char *)barr + start;

	rarray = (*env)->NewIntArray(env,len);
	if (rarray == NULL) {
		printf("Exception: NewIntArray failed?\n");
		return NULL;
	}

	iarray = (*env)->GetIntArrayElements(env,rarray,&bb);
	if (iarray == NULL) {
		printf("Exception: GetIntArrayElements failed?\n");
		return NULL;
	}

	iap = iarray;
	for (ii = 0; ii < len; ii++) {
		*iap = *(jint *)bp;
		iap++;
		bp += sizeof(jint);
	}

	(*env)->ReleaseIntArrayElements(env,rarray,(jint *)iarray, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns short [] */
JNIEXPORT jshortArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToShort__II_3B 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	char *bp;
	jbyte *barr;
	jshortArray rarray;
	int blen;
	jshort *iarray;
	jshort *iap;
	int ii;
	jboolean bb;

	if (bdata == NULL) {
		printf("Exception: bdata is NULL?\n");
		return NULL;
	}
	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	if (barr == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	blen = (*env)->GetArrayLength(env,bdata);
	if ((start < 0) || ((start + (len*(sizeof(jshort)))) > blen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	bp = (char *)barr + start;

	rarray = (*env)->NewShortArray(env,len);
	if (rarray == NULL) {
		printf("Exception: NewShortArray failed?\n");
		return NULL;
	}

	iarray = (*env)->GetShortArrayElements(env,rarray,&bb);
	if (iarray == NULL) {
		printf("Exception: GetShortArrayElements failed?\n");
		return NULL;
	}

	iap = iarray;
	for (ii = 0; ii < len; ii++) {
		*iap = *(jshort *)bp;
		iap++;
		bp += sizeof(jshort);
	}

	(*env)->ReleaseShortArrayElements(env,rarray,(jshort *)iarray, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns float [] */
JNIEXPORT jfloatArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToFloat__II_3B 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	char *bp;
	jbyte *barr;
	jfloatArray rarray;
	int blen;
	jfloat *iarray;
	jfloat *iap;
	int ii;
	jboolean bb;

	if (bdata == NULL) {
		printf("Exception: bdata is NULL?\n");
		return NULL;
	}
	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	if (barr == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	blen = (*env)->GetArrayLength(env,bdata);
	if ((start < 0) || ((start + (len*(sizeof(jfloat)))) > blen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	bp = (char *)barr + start;

	rarray = (*env)->NewFloatArray(env,len);
	if (rarray == NULL) {
		printf("Exception: NewFloatArray failed?\n");
		return NULL;
	}

	iarray = (*env)->GetFloatArrayElements(env,rarray,&bb);
	if (iarray == NULL) {
		printf("Exception: GetFloatArrayElements failed?\n");
		return NULL;
	}

	iap = iarray;
	for (ii = 0; ii < len; ii++) {
		*iap = *(jfloat *)bp;
		iap++;
		bp += sizeof(jfloat);
	}

	(*env)->ReleaseFloatArrayElements(env,rarray,(jfloat *)iarray, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns long [] */
JNIEXPORT jlongArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToLong__II_3B 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	char *bp;
	jbyte *barr;
	jlongArray rarray;
	int blen;
	jlong *iarray;
	jlong *iap;
	int ii;
	jboolean bb;

	if (bdata == NULL) {
		printf("Exception: bdata is NULL?\n");
		return NULL;
	}
	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	if (barr == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	blen = (*env)->GetArrayLength(env,bdata);
	if ((start < 0) || ((start + (len*(sizeof(jlong)))) > blen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	bp = (char *)barr + start;

	rarray = (*env)->NewLongArray(env,len);
	if (rarray == NULL) {
		printf("Exception: NewLongArray failed?\n");
		return NULL;
	}

	iarray = (*env)->GetLongArrayElements(env,rarray,&bb);
	if (iarray == NULL) {
		printf("Exception: GetLongArrayElements failed?\n");
		return NULL;
	}

	iap = iarray;
	for (ii = 0; ii < len; ii++) {

		*iap = *(jlong *)bp;
		iap++;
		bp += sizeof(jlong);
	}

	(*env)->ReleaseLongArrayElements(env,rarray,(jlong *)iarray, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;

}

/* returns double [] */
JNIEXPORT jdoubleArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToDouble__II_3B 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jbyteArray bdata)  /* IN: array of bytes */
{
	intn rval;
	char *bp;
	jbyte *barr;
	jdoubleArray rarray;
	int blen;
	jdouble *iarray;
	jdouble *iap;
	int ii;
	jboolean bb;

	if (bdata == NULL) {
		printf("Exception: bdata is NULL?\n");
		return NULL;
	}
	barr = (*env)->GetByteArrayElements(env,bdata,&bb);
	if (barr == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	blen = (*env)->GetArrayLength(env,bdata);
	if ((start < 0) || ((start + (len*(sizeof(jdouble)))) > blen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	bp = (char *)barr + start;

	rarray = (*env)->NewDoubleArray(env,len);
	if (rarray == NULL) {
		printf("Exception: NewDoubleArray failed?\n");
		return NULL;
	}

	iarray = (*env)->GetDoubleArrayElements(env,rarray,&bb);
	if (iarray == NULL) {
		printf("Exception: GetDoubleArrayElements failed?\n");
		return NULL;
	}

	iap = iarray;
	for (ii = 0; ii < len; ii++) {
		*iap = *(jdouble *)bp;
		iap++;
		bp += sizeof(jdouble);
	}

	(*env)->ReleaseDoubleArrayElements(env,rarray,(jdouble *)iarray, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env,bdata,barr,JNI_COMMIT);

	return rarray;
}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_intToByte__II_3I 
(JNIEnv *env,
jobject obj, 
jint start,
jint len,
jintArray idata)  /* IN: array of int */
{
	intn rval;
	jint *ip;
	jint *iarr;
	int ilen;
	jbyteArray rarray;
	int blen;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ii;
	int ij;
	union things {
		int ival;
		char bytes[4];
	} u;

	if (idata == NULL) {
		printf("Exception: idata is NULL?\n");
		return NULL;
	}
	iarr = (*env)->GetIntArrayElements(env,idata,&bb);
	if (iarr == NULL) {
		printf("Exception: GetIntArrayElements failed?\n");
		return NULL;
	}

	ilen = (*env)->GetArrayLength(env,idata);
	if ((start < 0) || (((start + len)) > ilen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	ip = iarr + start;

	blen = ilen * sizeof(jint);
	rarray = (*env)->NewByteArray(env,blen);
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	for (ii = 0; ii < len; ii++) {
		u.ival = *ip++;
		for (ij = 0; ij < sizeof(jint); ij++) {
			*bap = u.bytes[ij];
			bap++;
		}
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	(*env)->ReleaseIntArrayElements(env,idata,iarr,JNI_ABORT);

	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_shortToByte__II_3S 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jshortArray idata)  /* IN: array of short */
{
	intn rval;
	jshort *ip;
	jshort *iarr;
	int ilen;
	jbyteArray rarray;
	int blen;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ii;
	int ij;
	union things {
		short ival;
		char bytes[4];
	} u;

	if (idata == NULL) {
		printf("Exception: idata is NULL?\n");
		return NULL;
	}
	iarr = (*env)->GetShortArrayElements(env,idata,&bb);
	if (iarr == NULL) {
		printf("Exception: GetShortArrayElements failed?\n");
		return NULL;
	}

	ilen = (*env)->GetArrayLength(env,idata);
	if ((start < 0) || (((start + len)) > ilen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	ip = iarr + start;

	blen = ilen * sizeof(jshort);
	rarray = (*env)->NewByteArray(env,blen);
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	for (ii = 0; ii < len; ii++) {
		u.ival = *ip++;
		for (ij = 0; ij < sizeof(jshort); ij++) {
			*bap = u.bytes[ij];
			bap++;
		}
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	(*env)->ReleaseShortArrayElements(env,idata,iarr,JNI_ABORT);

	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_floatToByte__II_3F 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jfloatArray idata)  /* IN: array of float */
{
	intn rval;
	jfloat *ip;
	jfloat *iarr;
	int ilen;
	jbyteArray rarray;
	int blen;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ii;
	int ij;
	union things {
		float ival;
		char bytes[4];
	} u;

	if (idata == NULL) {
		printf("Exception: idata is NULL?\n");
		return NULL;
	}
	iarr = (*env)->GetFloatArrayElements(env,idata,&bb);
	if (iarr == NULL) {
		printf("Exception: GetFloatArrayElements failed?\n");
		return NULL;
	}

	ilen = (*env)->GetArrayLength(env,idata);
	if ((start < 0) || (((start + len)) > ilen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	ip = iarr + start;

	blen = ilen * sizeof(jfloat);
	rarray = (*env)->NewByteArray(env,blen);
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	for (ii = 0; ii < len; ii++) {
		u.ival = *ip++;
		for (ij = 0; ij < sizeof(jfloat); ij++) {
			*bap = u.bytes[ij];
			bap++;
		}
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	(*env)->ReleaseFloatArrayElements(env,idata,iarr,JNI_ABORT);

	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_doubleToByte__II_3D 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jdoubleArray idata)  /* IN: array of double */
{
	intn rval;
	jdouble *ip;
	jdouble *iarr;
	int ilen;
	jbyteArray rarray;
	int blen;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ii;
	int ij;
	union things {
		double ival;
		char bytes[8];
	} u;

	if (idata == NULL) {
		printf("Exception: idata is NULL?\n");
		return NULL;
	}
	iarr = (*env)->GetDoubleArrayElements(env,idata,&bb);
	if (iarr == NULL) {
		printf("Exception: GetDoubleArrayElements failed?\n");
		return NULL;
	}

	ilen = (*env)->GetArrayLength(env,idata);
	if ((start < 0) || (((start + len)) > ilen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	ip = iarr + start;

	blen = ilen * sizeof(jdouble);
	rarray = (*env)->NewByteArray(env,blen);
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	for (ii = 0; ii < len; ii++) {
		u.ival = *ip++;
		for (ij = 0; ij < sizeof(jdouble); ij++) {
			*bap = u.bytes[ij];
			bap++;
		}
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	(*env)->ReleaseDoubleArrayElements(env,idata,iarr,JNI_ABORT);

	return rarray;

}


/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_longToByte__II_3J 
( JNIEnv *env,
jobject obj, 
jint start,
jint len,
jlongArray idata)  /* IN: array of long */
{
	intn rval;
	jlong *ip;
	jlong *iarr;
	int ilen;
	jbyteArray rarray;
	int blen;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ii;
	int ij;
	union things {
		jlong ival;
		char bytes[8];
	} u;

	if (idata == NULL) {
		printf("Exception: idata is NULL?\n");
		return NULL;
	}
	iarr = (*env)->GetLongArrayElements(env,idata,&bb);
	if (iarr == NULL) {
		printf("Exception: GetLongArrayElements failed?\n");
		return NULL;
	}

	ilen = (*env)->GetArrayLength(env,idata);
	if ((start < 0) || (((start + len)) > ilen)) {
		printf("Exception: start or len is out of bounds?\n");
		return NULL;
	}

	ip = iarr + start;

	blen = ilen * sizeof(jlong);
	rarray = (*env)->NewByteArray(env,blen);
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	for (ii = 0; ii < len; ii++) {
		u.ival = *ip++;
		for (ij = 0; ij < sizeof(jlong); ij++) {
			*bap = u.bytes[ij];
			bap++;
		}
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	(*env)->ReleaseLongArrayElements(env,idata,iarr,JNI_ABORT);

	return rarray;

}
 /******/


/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_intToByte__I 
( JNIEnv *env,
jobject obj, 
jint idata)  /* IN: int */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	int ij;
	jboolean bb;
	union things {
		int ival;
		char bytes[sizeof(int)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jint));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jint); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_floatToByte__F 
( JNIEnv *env,
jobject obj, 
jfloat idata)  /* IN: int */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ij;
	union things {
		float ival;
		char bytes[sizeof(float)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jfloat));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jfloat); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);
	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_shortToByte__S 
( JNIEnv *env,
jobject obj, 
jshort idata)  /* IN: short */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ij;
	union things {
		short ival;
		char bytes[sizeof(short)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jshort));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jshort); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);

	return rarray;

}


/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_doubleToByte__D 
( JNIEnv *env,
jobject obj, 
jdouble idata)  /* IN: double */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ij;
	union things {
		double ival;
		char bytes[sizeof(double)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jdouble));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jdouble); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);

	return rarray;
}


/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_longToByte__J 
( JNIEnv *env,
jobject obj, 
jlong idata)  /* IN: array of long */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ij;
	union things {
		jlong ival;
		char bytes[sizeof(jlong)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jlong));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jlong); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);

	return rarray;

}

/* returns byte [] */
JNIEXPORT jbyteArray JNICALL Java_ncsa_hdf_hdflib_HDFNativeData_byteToByte__B 
( JNIEnv *env,
jobject obj, 
jbyte idata)  /* IN: array of long */
{
	jbyteArray rarray;
	jbyte *barray;
	jbyte *bap;
	jboolean bb;
	int ij;
	union things {
		jbyte ival;
		char bytes[sizeof(jbyte)];
	} u;

	rarray = (*env)->NewByteArray(env,sizeof(jbyte));
	if (rarray == NULL) {
		printf("Exception: NewByteArray failed?\n");
		return NULL;
	}

	barray = (*env)->GetByteArrayElements(env,rarray,&bb);
	if (barray == NULL) {
		printf("Exception: GetByteArrayElements failed?\n");
		return NULL;
	}

	bap = barray;
	u.ival = idata;
	for (ij = 0; ij < sizeof(jbyte); ij++) {
		*bap = u.bytes[ij];
		bap++;
	}

	(*env)->ReleaseByteArrayElements(env,rarray,(jbyte *)barray, JNI_COMMIT);

	return rarray;
}
