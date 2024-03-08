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
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */

#include <jni.h>

#include "hdf.h"
#include "h4jni.h"
#include "hdfhxImp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HXsetcreatedir(JNIEnv *env, jclass clss, jstring dir)
{
    int   rval = FAIL;
    char *str  = NULL;

    UNUSED(clss);

    if (dir == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "HXsetcreatedir:  dir is null");

    PIN_JAVA_STRING(ENVONLY, dir, str, NULL, "HXsetcreatedir:  dir is not pinned");

    rval = HXsetcreatedir((char *)str);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, dir, str);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_HXsetdir(JNIEnv *env, jclass clss, jstring dir)
{
    int   rval = FAIL;
    char *str  = NULL;

    UNUSED(clss);

    if (dir == NULL)
        H4_NULL_ARGUMENT_ERROR(ENVONLY, "HXsetdir:  dir is null");

    PIN_JAVA_STRING(ENVONLY, dir, str, NULL, "HXsetdir:  dir is not pinned");

    rval = HXsetdir(str);

done:
    if (str)
        UNPIN_JAVA_STRING(ENVONLY, dir, str);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
