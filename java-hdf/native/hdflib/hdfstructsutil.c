
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
 *  This module contains code to translate between the HDF C union 'comp_info'
 *  and a subclass of the Java class CompInfo.
 *
 *  This is nasty and ugly and probably buggy.
 *
 */
#include "hdf.h"
#include "hfile.h"
#include "hcomp.h"

#include <stdio.h>

#include "jni.h"

jboolean getNewCompInfo( JNIEnv *env, jobject obj, jobject ciobj, comp_info *cinf)
{
jfieldID jf;
jclass jc;
jint ctype;

	jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFNewCompInfo");
	if (jc == NULL) {
		return JNI_FALSE;
	}
	jf = (*env)->GetFieldID(env, jc, "ctype", "I");
	if (jf == NULL) {
		return JNI_FALSE;
	}
	ctype = (*env)->GetIntField(env, ciobj, jf);

	switch(ctype) {
	case COMP_CODE_NONE:
	case COMP_CODE_RLE:
	default:
		break;

	case COMP_CODE_SKPHUFF:
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFSKPHUFFCompInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "skp_size", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->skphuff.skp_size = (*env)->GetIntField(env, ciobj, jf);
		break;

	case COMP_CODE_DEFLATE:
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFDeflateCompInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "level", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->deflate.level = (*env)->GetIntField(env, ciobj, jf);
		break;
	case COMP_CODE_NBIT:
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFNBITCompInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "nt", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->nbit.nt = (*env)->GetIntField(env, ciobj, jf);

		jf = (*env)->GetFieldID(env, jc, "sign_ext", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->nbit.sign_ext = (*env)->GetIntField(env, ciobj, jf);

		jf = (*env)->GetFieldID(env, jc, "fill_one", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->nbit.fill_one = (*env)->GetIntField(env, ciobj, jf);

		jf = (*env)->GetFieldID(env, jc, "start_bit", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->nbit.start_bit = (*env)->GetIntField(env, ciobj, jf);

		jf = (*env)->GetFieldID(env, jc, "bit_len", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->nbit.bit_len = (*env)->GetIntField(env, ciobj, jf);
		break;
	}

	return JNI_TRUE;
}


jboolean getOldCompInfo( JNIEnv *env, jobject obj, jobject ciobj, comp_info *cinf)
{
jfieldID jf;
jclass jc;
jint ctype;

	jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFOldCompInfo");
	if (jc == NULL) {
		return JNI_FALSE;
	}
	jf = (*env)->GetFieldID(env, jc, "ctype", "I");
	if (jf == NULL) {
		return JNI_FALSE;
	}
	ctype = (*env)->GetIntField(env, ciobj, jf);

	switch(ctype) {
	case COMP_NONE:
	case COMP_RLE:
	case COMP_IMCOMP:
	default:
		break;

	case COMP_JPEG:
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFJPEGCompInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "quality", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->jpeg.quality = (*env)->GetIntField(env, ciobj, jf);

		jf = (*env)->GetFieldID(env, jc, "force_baseline", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		cinf->jpeg.force_baseline = (*env)->GetIntField(env, ciobj, jf);
		break;
	}

	return JNI_TRUE;
}



/*
  this is not finished ....
  need to get static array of ints, then copy to C array.

  Not sure how the JNI works ....
*/
jboolean getCompModel(JNIEnv *env, jobject obj, jobject compobj, model_info *cmod)
{
jfieldID jf;
jclass jc;
jmethodID jm;
jint cm;
int *d;
jobject darr;
jint *dims;
int i;
jboolean bb;

        jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompModel");
        if (jc == NULL) {
                return JNI_FALSE;
        }
        jf = (*env)->GetFieldID(env, jc, "comp_model", "I");
        if (jf == NULL) {
                return JNI_FALSE;
        }
        cm = (*env)->GetIntField(env, compobj, jf);

        switch(cm) {
	default:
                break;

        case COMP_MODEL_STDIO :
                jf = (*env)->GetFieldID(env, jc, "nt", "I");
                if (jf == NULL) {
                        return JNI_FALSE;
                }
                cmod->dim.nt = (*env)->GetIntField(env, compobj, jf);

                jf = (*env)->GetFieldID(env, jc, "ndim", "I");
                if (jf == NULL) {
                        return JNI_FALSE;
                }
                cmod->dim.ndim = (*env)->GetIntField(env, compobj, jf);

		/* need to get static array 'dims', copy into C array,
			put pointer in struct */
		cmod->dim.dims = HDmalloc(cmod->dim.ndim * DFKNTsize(cmod->dim.nt));

                jf = (*env)->GetFieldID(env, jc, "dims", "[I");
                if (jf == NULL) {
                        return JNI_FALSE;
                }
		darr = (*env)->GetObjectField(env,compobj,jf);
		if (darr == NULL) {
			return JNI_FALSE;
		}

		dims = (jint *)(*env)->GetIntArrayElements(env,(jintArray)darr,&bb);

		for (i = 0; i < cmod->dim.ndim; i++) {
			cmod->dim.dims[i] = (int32)dims[i];
		}

		(*env)->ReleaseIntArrayElements(env,(jintArray)darr,(jint *)dims,JNI_ABORT);

                break;
        }

	return JNI_TRUE;
}

void freeCompModel(JNIEnv *env, jobject obj, model_info *cmod)
{
	if (cmod->dim.dims != NULL) {
		HDfree(cmod->dim.dims);
	}
}


jboolean getChunkInfo( JNIEnv *env, jobject obj, jobject chunkobj, HDF_CHUNK_DEF *cinf)
{
jfieldID jf;
jclass jc;
jint ctype;
jobject larr;
jint * lens;
int i;
jboolean bval;
jboolean bb;

	jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFChunkInfo");
	if (jc == NULL) {
		return JNI_FALSE;
	}
	jf = (*env)->GetFieldID(env, jc, "ctype", "I");
	if (jf == NULL) {
		return JNI_FALSE;
	}
	ctype = (*env)->GetIntField(env, chunkobj, jf);

	switch (ctype) {
	    case HDF_CHUNK:
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFOnlyChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "chunk_lengths", "[I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		larr = (*env)->GetObjectField(env,chunkobj,jf);
		if (larr == NULL) {
			return JNI_FALSE;
		}

		lens = (jint *)(*env)->GetIntArrayElements(env,(jintArray)larr,&bb);

		for (i = 0; i < MAX_VAR_DIMS; i++) {
			cinf->chunk_lengths[i] = (int32)lens[i];
		}

		(*env)->ReleaseIntArrayElements(env,(jintArray)larr,(jint *)lens,JNI_ABORT);
		break;

	case (HDF_CHUNK | HDF_COMP):
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "chunk_lengths", "[I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		larr = (*env)->GetObjectField(env,chunkobj,jf);
		if (larr == NULL) {
			return JNI_FALSE;
		}

		lens = (jint *)(*env)->GetIntArrayElements(env,(jintArray)larr,&bb);

		for (i = 0; i < MAX_VAR_DIMS; i++) {
			cinf->comp.chunk_lengths[i] = (int32)lens[i];
		}

		(*env)->ReleaseIntArrayElements(env,(jintArray)larr,(jint *)lens,JNI_ABORT);

		jf = (*env)->GetFieldID(env, jc, "comp_type", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->comp.comp_type = (*env)->GetIntField(env, chunkobj, jf);

		jf = (*env)->GetFieldID(env, jc, "model_type", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->comp.model_type = (*env)->GetIntField(env, chunkobj, jf);

		jf = (*env)->GetFieldID(env, jc, "cinfo", "Lncsa/hdf/hdflib/HDFCompInfo;");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		larr = (*env)->GetObjectField(env,chunkobj,jf);
		if (larr == NULL) {
			return JNI_FALSE;
		}
		bval = getOldCompInfo(env,obj,(jobject)larr,&(cinf->comp.cinfo));

		jf = (*env)->GetFieldID(env, jc, "minfo", "Lncsa/hdf/hdflib/HDFModelInfo;");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		larr = (*env)->GetObjectField(env,chunkobj,jf);
		if (larr == NULL) {
			return JNI_FALSE;
		}
		bval = getCompModel(env,obj,(jobject)larr,&(cinf->comp.minfo));
		break;
	case (HDF_CHUNK | HDF_NBIT):
		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFNBITChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jf = (*env)->GetFieldID(env, jc, "chunk_lengths", "[I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
		larr = (*env)->GetObjectField(env,chunkobj,jf);
		if (larr == NULL) {
			return JNI_FALSE;
		}

		lens = (jint *)(*env)->GetIntArrayElements(env,(jintArray)larr,&bb);

		for (i = 0; i < MAX_VAR_DIMS; i++) {
			cinf->nbit.chunk_lengths[i] = (int32)lens[i];
		}

		(*env)->ReleaseIntArrayElements(env,(jintArray)larr,(jint *)lens,JNI_ABORT);

		jf = (*env)->GetFieldID(env, jc, "start_bit", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->nbit.start_bit = (*env)->GetIntField(env, chunkobj, jf);

		jf = (*env)->GetFieldID(env, jc, "bit_len", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->nbit.bit_len = (*env)->GetIntField(env, chunkobj, jf);

		jf = (*env)->GetFieldID(env, jc, "sign_ext", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->nbit.sign_ext = (*env)->GetIntField(env, chunkobj, jf);

		jf = (*env)->GetFieldID(env, jc, "fill_one", "I");
		if (jf == NULL) {
			return JNI_FALSE;
		}
                cinf->nbit.fill_one = (*env)->GetIntField(env, chunkobj, jf);
		break;

		default:
			return JNI_FALSE;
	}

	return JNI_TRUE;
}

jboolean makeChunkInfo( JNIEnv *env, jobject obj, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf)
{
jclass jc;
jmethodID jmi;
jintArray rarray;
jobject compinfo;
jobject compmodel;

	switch (flgs) {
	    case HDF_CHUNK:
		rarray = (*env)->NewIntArray(env,MAX_VAR_DIMS);
		(*env)->SetIntArrayRegion(env,rarray,0,MAX_VAR_DIMS,(jint *)cinf->chunk_lengths);

		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFOnlyChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jmi = (*env)->GetMethodID(env, jc, "<init>", "([I)V");
		if (jmi == NULL) {
			return JNI_FALSE;
		}
		(*env)->CallVoidMethod(env,chunkobj,jmi,rarray);
		break;

	    case (HDF_CHUNK | HDF_COMP):
		/* create an HDFCompInfo (appropriate type) */
		switch (cinf->comp.comp_type) {
			case COMP_NONE:
			default:
				/* new HDFCompInfo() */
				jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompInfo");
				if (jc == NULL) {
					return JNI_FALSE;
				}
				jmi = (*env)->GetMethodID(env, jc, "<init>", "()V");
				if (jmi == NULL) {
					return JNI_FALSE;
				}
				compmodel = (*env)->NewObject(env,jc,jmi);
				break;
			case COMP_JPEG:
				/* new HDFJPEGCompInfo() */
				jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompInfo");
				if (jc == NULL) {
					return JNI_FALSE;
				}
				jmi = (*env)->GetMethodID(env, jc, "<init>", "(II)V");
				if (jmi == NULL) {
					return JNI_FALSE;
				}
				compmodel = (*env)->NewObject(env,jc,jmi,cinf->comp.cinfo.jpeg.quality,
					cinf->comp.cinfo.jpeg.force_baseline);
				break;
		}
		/* create an HDFCompModel (what type?) */
		switch (cinf->comp.model_type) {
			default:
				rarray = (*env)->NewIntArray(env,cinf->comp.minfo.dim.ndim);
				(*env)->SetIntArrayRegion(env,rarray,0,MAX_VAR_DIMS,
					(jint *)cinf->comp.minfo.dim.dims);

				/* new HDFCompModel() */
				jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompInfo");
				if (jc == NULL) {
					return JNI_FALSE;
				}
				jmi = (*env)->GetMethodID(env, jc, "<init>", "(III[I)V");
				if (jmi == NULL) {
					return JNI_FALSE;
				}
				compinfo = (*env)->NewObject(env,jc,jmi,
					cinf->comp.model_type,
					cinf->comp.minfo.dim.nt,
					cinf->comp.minfo.dim.ndim,
					rarray);
				break;
		}

		/* create an int[] array */
		rarray = (*env)->NewIntArray(env,MAX_VAR_DIMS);
		(*env)->SetIntArrayRegion(env,rarray,0,MAX_VAR_DIMS,(jint *)cinf->comp.chunk_lengths);

		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFCompChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jmi = (*env)->GetMethodID(env, jc, "<init>", 
			"([IIILncsa_hdf_HDFCompInfo;Lncsa_hdf_HDFCompModel;)V");
		if (jmi == NULL) {
			return JNI_FALSE;
		}
		(*env)->CallVoidMethod(env,chunkobj,jmi, rarray, cinf->comp.comp_type, 
			cinf->comp.model_type, compinfo, compmodel);

		/* create an HDFCompChunkInfo, using arguments */
		break;
		case (HDF_COMP | HDF_NBIT):
		/* create an int[] array */
		rarray = (*env)->NewIntArray(env,MAX_VAR_DIMS);
		(*env)->SetIntArrayRegion(env,rarray,0,MAX_VAR_DIMS,(jint *)cinf->nbit.chunk_lengths);

		jc = (*env)->FindClass(env, "ncsa/hdf/hdflib/HDFNBITChunkInfo");
		if (jc == NULL) {
			return JNI_FALSE;
		}
		jmi = (*env)->GetMethodID(env, jc, "<init>", 
			"([IIILncsa_hdf_HDFCompInfo;Lncsa_hdf_HDFCompModel;)V");
		if (jmi == NULL) {
			return JNI_FALSE;
		}
		(*env)->CallVoidMethod(env,chunkobj,jmi, rarray, 
			cinf->nbit.start_bit, 
			cinf->nbit.bit_len, 
			cinf->nbit.sign_ext, 
			cinf->nbit.fill_one); 
		break;

		default:
			return JNI_FALSE;
	}

	return JNI_TRUE;
}
