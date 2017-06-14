/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 ****************************************************************************/
/*
 *  This is a utility program used by the HDF Java-C wrapper layer to
 *  generate exceptions.  This may be called from any part of the
 *  Java-C interface.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "hdf.h"
#include "h4jni.h"
#include <stdlib.h>

/********************/
/* Local Macros     */
/********************/

#define THROWEXCEPTION(className,args) {                                    \
    jclass     jc;                                                          \
    jmethodID  jm;                                                          \
    jobject    ex;                                                          \
    jc = ENVPTR->FindClass(ENVPAR (className));                             \
    if (jc == NULL) {                                                       \
        return JNI_FALSE;                                                   \
    }                                                                       \
    jm = ENVPTR->GetMethodID(ENVPAR jc, "<init>", "(Ljava/lang/String;)V"); \
    if (jm == NULL) {                                                       \
        return JNI_FALSE;                                                   \
    }                                                                       \
    ex = ENVPTR->NewObjectA (ENVPAR jc, jm, (jvalue*)(args));               \
    if (ENVPTR->Throw(ENVPAR (jthrowable)ex) < 0) {                         \
        printf("FATAL ERROR:  %s: Throw failed\n", (className));            \
        return JNI_FALSE;                                                   \
    }                                                                       \
    return JNI_TRUE;                                                        \
}

jboolean
h4buildException
(JNIEnv *env, jint HDFerr)
{
jmethodID jm;
jclass jc;
int args[2];
jobject ex;
int rval;


    jc = ENVPTR->FindClass(ENVPAR  "hdf/hdflib/HDFLibraryException");
    if (jc == NULL) {
        return JNI_FALSE;
    }
    jm = ENVPTR->GetMethodID(ENVPAR  jc, "<init>", "(I)V");
    if (jm == NULL) {
        return JNI_FALSE;
    }
    args[0] = HDFerr;
    args[1] = 0;

    ex = ENVPTR->NewObjectA (ENVPAR jc, jm, (jvalue *)args );

    rval = ENVPTR->Throw(ENVPAR  (jthrowable)ex );

    return JNI_TRUE;
}

/*
 *  Routine to raise particular Java exceptions from C
 */
static
jboolean
H4JNIErrorClass
(JNIEnv *env, const char *message, const char *className)
{
    char *args[2];
    jstring str = ENVPTR->NewStringUTF(ENVPAR message);
    args[0] = (char *)str;
    args[1] = 0;

    THROWEXCEPTION(className, args);
} /* end H5JNIErrorClass() */

/*
 *  A bad argument in an HDF call
 *  Create and throw an 'IllegalArgumentException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4badArgument
    (JNIEnv *env, const char *functName)
{
    return H4JNIErrorClass(env, functName, "java/lang/IllegalArgumentException");
} /* end h4badArgument() */

/*
 *  A NULL argument in an HDF call
 *  Create and throw an 'NullPointerException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4nullArgument
    (JNIEnv *env, const char *functName)
{
    return H4JNIErrorClass(env, functName, "java/lang/NullPointerException");
} /* end h4nullArgument() */

jboolean
h4NotImplemented
(JNIEnv *env, const char *functName)
{
    return H4JNIErrorClass(env, functName, "hdf/hdflib/HDFNotImplementedException");
}

jboolean
h4outOfMemory
(JNIEnv *env, const char *functName)
{
    return H4JNIErrorClass(env, functName, "java/lang/OutOfMemoryError");
}

/*
 *  A fatal error in a JNI call
 */
jboolean
h4JNIFatalError
(JNIEnv *env, const char *functName)
{
    return H4JNIErrorClass(env, functName, "java/lang/InternalError");
}

jboolean
h4raiseException
(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "hdf/hdflib/HDFLibraryException");
}
/*
 * Class:     hdf_hdflib_HDFLibraryException
 * Method:    printStackTrace0
 * Signature: (Ljava/lang/Object;)V
 *
 *  Call the HDF library to print the HDF error stack to 'file_name'.
 */
JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibraryException_printStackTrace0
(JNIEnv *env, jclass clss, jstring file_name)
{
    FILE       *stream = NULL;
    const char *file = NULL;

    if(file_name == NULL) {
        HEprint(stderr, 0);
    } /* end if */
    else {
        file = ENVPTR->GetStringUTFChars(ENVPAR file_name, 0);
        stream = fopen(file, "a+");
        if(stream) {
            HEprint(stream, 0);
            fclose(stream);
        } /* end if */
        ENVPTR->ReleaseStringUTFChars(ENVPAR file_name, file);
    } /* end else */
} /* end  Java_hdf_hdflib_HDFLibraryException_printStackTrace0() */

#ifdef __cplusplus
}
#endif
