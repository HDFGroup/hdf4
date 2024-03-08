/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This module contains code to translate between the HDF C union 'comp_info'
 *  and a subclass of the Java class CompInfo.
 *
 *  This is nasty and ugly and probably buggy.
 *
 */

#include <jni.h>

#include "hdf.h"
#include "h4jni.h"
#include "hdfstructsutil.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 *  Get information from a Java HDFNewCompInfo object in to a C comp_info
 *  struct.
 *
 *  Extract information for the different types of compression.
 */

jboolean
getNewCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf)
{
    jfieldID jf;
    jclass   jc;
    jint     ctype;

    jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFNewCompInfo");
    if (jc == NULL) {
        return JNI_FALSE;
    }
    jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
    if (jf == NULL) {
        return JNI_FALSE;
    }
    ctype = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

    switch (ctype) {
        case COMP_CODE_NONE:
        case COMP_CODE_RLE:
        default:
            break;

        case COMP_CODE_SKPHUFF:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFSKPHUFFCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "skp_size", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->skphuff.skp_size = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            break;

        case COMP_CODE_DEFLATE:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFDeflateCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "level", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->deflate.level = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            break;
        case COMP_CODE_SZIP:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFSZIPCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "bits_per_pixel", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }

            cinf->szip.bits_per_pixel = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            jf                        = ENVPTR->GetFieldID(ENVONLY, jc, "options_mask", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->szip.options_mask = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            /*      changes from hdf-42r0 to hdf-42r1
                    jf = ENVPTR->GetFieldID(ENVONLY,  jc, "compression_mode", "I");
                    if (jf == NULL) {
                        return JNI_FALSE;
                    }
                    cinf->szip.compression_mode = ENVPTR->GetIntField(ENVONLY,  ciobj, jf);
            */

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->szip.pixels = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels_per_block", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->szip.pixels_per_block = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels_per_scanline", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->szip.pixels_per_scanline = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            break;
        case COMP_CODE_NBIT:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFNBITCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "nt", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->nbit.nt = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "sign_ext", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->nbit.sign_ext = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "fill_one", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->nbit.fill_one = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "start_bit", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->nbit.start_bit = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "bit_len", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->nbit.bit_len = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            break;
    }

    return JNI_TRUE;
}

/*
 *   Extract info from C comp_info struct, put in Java HDFCompInfo object.
 *
 *   Put in the fields for each compression method.
 */
jboolean
setNewCompInfo(JNIEnv *env, jobject ciobj, comp_coder_t coder, comp_info *cinf)
{
    jfieldID jf;
    jclass   jc;

    jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFCompInfo");

    if (jc == NULL) {
        return JNI_FALSE;
    }
    else {
        jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        ENVPTR->SetIntField(ENVONLY, ciobj, jf, coder);
    }

    switch (coder) {
        case COMP_CODE_NONE:
        case COMP_CODE_RLE:
        default:
            break;
        case COMP_CODE_SKPHUFF:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFSKPHUFFCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, COMP_CODE_SKPHUFF);
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "skp_size", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->skphuff.skp_size);
            break;

        case COMP_CODE_DEFLATE:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFDeflateCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, COMP_CODE_DEFLATE);
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "level", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->deflate.level);
            break;
        case COMP_CODE_SZIP:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFSZIPCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, COMP_CODE_SZIP);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "bits_per_pixel", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->szip.bits_per_pixel);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "options_mask", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->szip.options_mask);

            /*   changes from hdf-42r0 to hdf-42r1
                    jf = ENVPTR->GetFieldID(ENVONLY,  jc, "compression_mode", "I");
                    if (jf == NULL) {
                        return JNI_FALSE;
                    }
                    ENVPTR->SetIntField(ENVONLY,  ciobj, jf, cinf->szip.compression_mode);
            */

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->szip.pixels);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels_per_block", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->szip.pixels_per_block);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "pixels_per_scanline", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->szip.pixels_per_scanline);
            break;
        case COMP_CODE_NBIT:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFNBITCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, COMP_CODE_NBIT);
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "nt", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->nbit.nt);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "sign_ext", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->nbit.sign_ext);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "fill_one", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->nbit.fill_one);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "start_bit", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->nbit.start_bit);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "bit_len", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            ENVPTR->SetIntField(ENVONLY, ciobj, jf, cinf->nbit.bit_len);
            break;
    }

    return JNI_TRUE;
}

/*
 *     Get info from old style C comp_info struct, put in HDFCompInfo object.
 */
jboolean
getOldCompInfo(JNIEnv *env, jobject ciobj, comp_info *cinf)
{
    jfieldID jf;
    jclass   jc;
    jint     ctype;

    jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFOldCompInfo");
    if (jc == NULL) {
        return JNI_FALSE;
    }
    jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
    if (jf == NULL) {
        return JNI_FALSE;
    }
    ctype = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

    switch (ctype) {
        case COMP_NONE:
        case COMP_RLE:
        case COMP_IMCOMP:
        default:
            break;

        case COMP_JPEG:
            jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFJPEGCompInfo");
            if (jc == NULL) {
                return JNI_FALSE;
            }
            jf = ENVPTR->GetFieldID(ENVONLY, jc, "quality", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->jpeg.quality = ENVPTR->GetIntField(ENVONLY, ciobj, jf);

            jf = ENVPTR->GetFieldID(ENVONLY, jc, "force_baseline", "I");
            if (jf == NULL) {
                return JNI_FALSE;
            }
            cinf->jpeg.force_baseline = ENVPTR->GetIntField(ENVONLY, ciobj, jf);
            break;
    }

    return JNI_TRUE;
}

/*
 *  Get the Chunk info from C HDF_CHUNK_DEF struct, put in
 *  Java HDFChunkInfo object.
 */
jboolean
getChunkInfo(JNIEnv *env, jobject chunkobj, HDF_CHUNK_DEF *cinf)
{
    jfieldID jf;
    jclass   jc;
    jint     ctype;
    jobject  larr;
    jint    *lens;
    int      i;
    jboolean bb;

    jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFChunkInfo");
    if (jc == NULL) {
        return JNI_FALSE;
    }
    jf = ENVPTR->GetFieldID(ENVONLY, jc, "ctype", "I");
    if (jf == NULL) {
        return JNI_FALSE;
    }
    ctype = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);

    jf = ENVPTR->GetFieldID(ENVONLY, jc, "chunk_lengths", "[I");
    if (jf == NULL) {
        return JNI_FALSE;
    }
    larr = ENVPTR->GetObjectField(ENVONLY, chunkobj, jf);
    if (larr == NULL) {
        return JNI_FALSE;
    }

    lens = (jint *)ENVPTR->GetIntArrayElements(ENVONLY, (jintArray)larr, &bb);

    for (i = 0; i < H4_MAX_VAR_DIMS; i++) {
        cinf->comp.chunk_lengths[i] = (int32)lens[i];
    }

    ENVPTR->ReleaseIntArrayElements(ENVONLY, (jintArray)larr, (jint *)lens, JNI_ABORT);

    if (ctype == (HDF_CHUNK | HDF_COMP)) {
        jf = ENVPTR->GetFieldID(ENVONLY, jc, "comp_type", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        cinf->comp.comp_type = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);

        jf = ENVPTR->GetFieldID(ENVONLY, jc, "cinfo", "Lhdf/hdflib/HDFCompInfo;");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        larr = ENVPTR->GetObjectField(ENVONLY, chunkobj, jf);
        if (larr == NULL) {
            return JNI_FALSE;
        }

        /* set compression information */
        return getNewCompInfo(env, (jobject)larr, &(cinf->comp.cinfo));
    }
    else if (ctype == (HDF_CHUNK | HDF_NBIT)) {
        jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFNBITChunkInfo");
        if (jc == NULL) {
            return JNI_FALSE;
        }
        jf = ENVPTR->GetFieldID(ENVONLY, jc, "chunk_lengths", "[I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        larr = ENVPTR->GetObjectField(ENVONLY, chunkobj, jf);
        if (larr == NULL) {
            return JNI_FALSE;
        }

        lens = (jint *)ENVPTR->GetIntArrayElements(ENVONLY, (jintArray)larr, &bb);

        for (i = 0; i < H4_MAX_VAR_DIMS; i++) {
            cinf->nbit.chunk_lengths[i] = (int32)lens[i];
        }

        ENVPTR->ReleaseIntArrayElements(ENVONLY, (jintArray)larr, (jint *)lens, JNI_ABORT);

        jf = ENVPTR->GetFieldID(ENVONLY, jc, "start_bit", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        cinf->nbit.start_bit = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);

        jf = ENVPTR->GetFieldID(ENVONLY, jc, "bit_len", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        cinf->nbit.bit_len = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);

        jf = ENVPTR->GetFieldID(ENVONLY, jc, "sign_ext", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        cinf->nbit.sign_ext = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);

        jf = ENVPTR->GetFieldID(ENVONLY, jc, "fill_one", "I");
        if (jf == NULL) {
            return JNI_FALSE;
        }
        cinf->nbit.fill_one = ENVPTR->GetIntField(ENVONLY, chunkobj, jf);
    }

    return JNI_TRUE;
}

/*
 *  Create C HDF_CHUNK_DEF struct from Java HDFChunkInfo object.
 *
 *  Determine the compression method, and create an appropriate subclass
 *  of HDFCompInfo.  Then call the constructor for HDFChunkInfo.
 */
jboolean
makeChunkInfo(JNIEnv *env, jobject chunkobj, int32 flgs, HDF_CHUNK_DEF *cinf)
{
    jclass    jc;
    jclass    jci;
    jmethodID jmi;
    jintArray rarray;
    jobject   compinfo;

    if ((rarray = ENVPTR->NewIntArray(ENVONLY, H4_MAX_VAR_DIMS)) == NULL)
        return JNI_FALSE;

    ENVPTR->SetIntArrayRegion(ENVONLY, rarray, 0, H4_MAX_VAR_DIMS, (jint *)cinf->chunk_lengths);

    /* release rarray? */

    if ((jci = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFChunkInfo")) == NULL)
        return JNI_FALSE;

    switch (flgs) {
        case HDF_CHUNK:
        default:
            if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFCompInfo")) == NULL)
                return JNI_FALSE;

            if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "()V")) == NULL)
                return JNI_FALSE;

            compinfo = ENVPTR->NewObject(ENVONLY, jc, jmi);
            break;
        case (HDF_CHUNK | HDF_COMP):
            switch (cinf->comp.comp_type) {
                case COMP_CODE_NONE:
                default:
                    if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFCompInfo")) == NULL)
                        return JNI_FALSE;

                    if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "()V")) == NULL)
                        return JNI_FALSE;

                    compinfo = ENVPTR->NewObject(ENVONLY, jc, jmi);
                    break;
                case COMP_CODE_JPEG:
                    /* new HDFJPEGCompInfo() */
                    if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFJPEGCompInfo")) == NULL)
                        return JNI_FALSE;

                    if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "(II)V")) == NULL)
                        return JNI_FALSE;

                    compinfo = ENVPTR->NewObject(ENVONLY, jc, jmi, cinf->comp.cinfo.jpeg.quality,
                                                 cinf->comp.cinfo.jpeg.force_baseline);
                    break;
                case COMP_CODE_DEFLATE:
                    if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFDeflateCompInfo")) == NULL)
                        return JNI_FALSE;

                    if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "(I)V")) == NULL)
                        return JNI_FALSE;

                    compinfo = ENVPTR->NewObject(ENVONLY, jc, jmi, cinf->comp.cinfo.deflate.level);
                    break;
                case COMP_CODE_SZIP:
                    if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFSZIPCompInfo")) == NULL)
                        return JNI_FALSE;

                    if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "(IIIII)V")) == NULL)
                        return JNI_FALSE;

                    compinfo = ENVPTR->NewObject(
                        ENVONLY, jc, jmi, cinf->comp.cinfo.szip.bits_per_pixel,
                        cinf->comp.cinfo.szip.options_mask, cinf->comp.cinfo.szip.pixels,
                        cinf->comp.cinfo.szip.pixels_per_block, cinf->comp.cinfo.szip.pixels_per_scanline);
                    break;
            }
            break;
        case (HDF_CHUNK | HDF_NBIT):
            /* new HDFCompInfo() */
            if ((jc = ENVPTR->FindClass(ENVONLY, "hdf/hdflib/HDFNBITChunkInfo")) == NULL)
                return JNI_FALSE;

            if ((jmi = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "([IIIII;)V")) == NULL)
                return JNI_FALSE;

            ENVPTR->CallVoidMethod(ENVONLY, chunkobj, jmi, rarray, cinf->nbit.start_bit, cinf->nbit.bit_len,
                                   cinf->nbit.sign_ext, cinf->nbit.fill_one);
            return JNI_TRUE;
            break;
    }

    if ((jmi = ENVPTR->GetMethodID(ENVONLY, jci, "<init>", "([IILhdf/hdflib/HDFCompInfo;)V")) == NULL)
        return JNI_FALSE;

    ENVPTR->CallVoidMethod(ENVONLY, chunkobj, jmi, rarray, cinf->comp.comp_type, compinfo);

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HCget_1config_1info(JNIEnv *env, jclass clss, jint coder_type)
{
    int    rval                    = FAIL;
    uint32 compression_config_info = 0;

    UNUSED(env);
    UNUSED(clss);

    /* check for success... */
    if ((rval = HCget_config_info((comp_coder_t)coder_type, (uint32 *)&compression_config_info)) == FAIL)
        return -1;

    return compression_config_info;
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
