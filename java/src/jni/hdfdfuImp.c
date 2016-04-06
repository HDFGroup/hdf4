
/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/
/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4.1 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif


#include "hdf.h"
#include "jni.h"

#ifdef __cplusplus
#define ENVPTR (env)
#define ENVPAR
#define ENVONLY
#else
#define ENVPTR (*env)
#define ENVPAR env,
#define ENVONLY env
#endif

extern jboolean h4NotImplemented( JNIEnv *env, char *functName);

JNIEXPORT jboolean JNICALL Java_hdf_hdflib_HDFLibrary_DFUfptoimage
( JNIEnv *env,
jclass clss,
jint hdim,
jint vdim,
jfloat max,
jfloat min,
jfloatArray hscale, /* IN: float [] */
jfloatArray vscale, /* IN: float [] */
jfloatArray data,  /* IN:  float[][] -- > bytes? */
jbyteArray palette,  /* IN: byte[] */
jstring outfile,   /* IN */
jint ct_method,  /* IN */
jint hres, /* IN */
jint vres,  /* IN */
jint compress) /* IN */
{
    h4NotImplemented(env, "DFUfptoimage (windows)");
    return JNI_TRUE;
}


#ifdef __cplusplus
}
#endif
