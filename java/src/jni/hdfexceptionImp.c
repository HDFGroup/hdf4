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
 *  This is a utility program used by the HDF Java-C wrapper layer to
 *  generate exceptions.  This may be called from any part of the
 *  Java-C interface.
 *
 */

#include <jni.h>

#include "hdf.h"
#include "h4jni.h"
#include "hdfexceptionImp.h"

/********************/
/* Local Macros     */
/********************/

#define THROWEXCEPTION(className, args)                                                                      \
    {                                                                                                        \
        jmethodID jm;                                                                                        \
        jclass    jc;                                                                                        \
        jobject   ex;                                                                                        \
                                                                                                             \
        if (NULL == (jc = ENVPTR->FindClass(ENVONLY, (className))))                                          \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
                                                                                                             \
        if (NULL == (jm = ENVPTR->GetMethodID(ENVONLY, jc, "<init>", "(Ljava/lang/String;)V"))) {            \
            printf("THROWEXCEPTION FATAL ERROR: GetMethodID failed\n");                                      \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
                                                                                                             \
        if (NULL == (ex = ENVPTR->NewObjectA(ENVONLY, jc, jm, (jvalue *)(args)))) {                          \
            printf("THROWEXCEPTION FATAL ERROR: Class %s: Creation failed\n", (className));                  \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
                                                                                                             \
        if (ENVPTR->Throw(ENVONLY, (jthrowable)ex) < 0) {                                                    \
            printf("THROWEXCEPTION FATAL ERROR: Class %s: Throw failed\n", (className));                     \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
    }

#define THROWINTEXCEPTION(className, args)                                                                   \
    {                                                                                                        \
        jmethodID jm;                                                                                        \
        jclass    jcls;                                                                                      \
        jobject   ex;                                                                                        \
                                                                                                             \
        if (NULL == (jcls = ENVPTR->FindClass(ENVONLY, (className))))                                        \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
                                                                                                             \
        if (NULL == (jm = ENVPTR->GetMethodID(ENVONLY, jcls, "<init>", "(I)V"))) {                           \
            printf("THROWEXCEPTION FATAL ERROR: GetMethodID failed\n");                                      \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
                                                                                                             \
        if (NULL == (ex = ENVPTR->NewObjectA(ENVONLY, jcls, jm, (jvalue *)(args)))) {                        \
            printf("THROWEXCEPTION FATAL ERROR: Class %s: Creation failed\n", (className));                  \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
                                                                                                             \
        if (ENVPTR->Throw(ENVONLY, (jthrowable)ex) < 0) {                                                    \
            printf("THROWEXCEPTION FATAL ERROR: Class %s: Throw failed\n", (className));                     \
            CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);                                                         \
        }                                                                                                    \
    }

/********************/
/* Local Prototypes */
/********************/
static jboolean H4JNIErrorClass(JNIEnv *env, const char *message, const char *className);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Class:     hdf_hdflib_HDFLibraryException
 * Method:    printStackTrace0
 * Signature: (Ljava/lang/Object;)V
 *
 *  Call the HDF library to print the HDF error stack to 'file_name'.
 */
JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibraryException_printStackTrace0(JNIEnv *env, jobject obj, jstring file_name)
{
    FILE       *stream = NULL;
    const char *file   = NULL;

    UNUSED(obj);

    if (NULL == file_name) {
        HEprint(stderr, 0);
    }
    else {
        PIN_JAVA_STRING(ENVONLY, file_name, file, NULL, "printStackTrace0: file name not pinned");

        if ((stream = fopen(file, "a+"))) {
            HEprint(stream, 0);
            fclose(stream);
        }
    }

done:
    if (file)
        UNPIN_JAVA_STRING(ENVONLY, file_name, file);

    return;
} /* end  Java_hdf_hdflib_HDFLibraryException_printStackTrace0() */

/*
 *  Routine to raise particular Java exceptions from C.
 */
static jboolean
H4JNIErrorClass(JNIEnv *env, const char *message, const char *className)
{
    jstring  str;
    char    *args[2];
    jboolean rval = JNI_FALSE;

    if (NULL == (str = ENVPTR->NewStringUTF(ENVONLY, message)))
        CHECK_JNI_EXCEPTION(ENVONLY, JNI_FALSE);

    args[0] = (char *)str;
    args[1] = 0;

    THROWEXCEPTION(className, args);

    rval = JNI_TRUE;

done:
    return rval;
} /* end H4JNIErrorClass() */

/*
 *  Create and throw an 'outOfMemoryException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4outOfMemory(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "java/lang/OutOfMemoryError");
} /* end h4outOfMemory() */

/*
 *  Create and throw an 'AssertionError'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4assertion(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "java/lang/AssertionError");
} /* end h4assertion() */

/*
 *  A fatal error in a JNI call
 *  Create and throw an 'InternalError'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4JNIFatalError(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "java/lang/InternalError");
} /* end h4JNIFatalError() */

/*
 *  A NULL argument in an HDF call
 *  Create and throw an 'NullPointerException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4nullArgument(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "java/lang/NullPointerException");
} /* end h4nullArgument() */

/*
 *  A bad argument in an HDF call
 *  Create and throw an 'IllegalArgumentException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4badArgument(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "java/lang/IllegalArgumentException");
} /* end h4badArgument() */

/*
 *  Some feature Not implemented yet
 *  Create and throw an 'UnsupportedOperationException'
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4NotImplemented(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "hdf/hdflib/HDFNotImplementedException");
} /* end h4NotImplemented() */

/*  h4raiseException().  This routine is called to generate
 *  an arbitrary Java exception with a particular message.
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4raiseException(JNIEnv *env, const char *message)
{
    return H4JNIErrorClass(env, message, "hdf/hdflib/HDFLibraryException");
} /* end h4raiseException() */

/* for now:  use top of exception stack:  fix this to do whole stack */
/*
 *  h4libraryError()   creates and throws the appropriate sub-class of
 *  HDFLibraryException().  This routine should be called
 *  whenever a call to the HDF library fails, i.e., when
 *  the return is -1.
 *
 *  Note:  This routine never returns from the 'throw',
 *  and the Java native method immediately raises the
 *  exception.
 */
jboolean
h4libraryError(JNIEnv *env)
{
    int16       errval;
    jclass      jc;
    int         args[2];
    jboolean    rval      = JNI_FALSE;
    const char *exception = "hdf/hdflib/HDFLibraryException";

    errval = HEvalue((int32)1);
    /*
     * No error detected in error stack.
     */
    if (errval == DFE_NONE)
        goto done;

    args[0] = errval;
    args[1] = 0;

    THROWINTEXCEPTION(exception, args);

    jc = ENVPTR->FindClass(ENVONLY, exception);
    if (jc != NULL)
        ENVPTR->ThrowNew(ENVONLY, jc, HEstring((hdf_err_code_t)errval));

    rval = JNI_TRUE;

done:
    return rval;
} /* end h4libraryError() */

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
