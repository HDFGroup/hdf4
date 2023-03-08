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
 *  For details of the HDF libraries, see the HDF Documentation at:
 *    https://portal.hdfgroup.org/display/HDF4/HDF4
 *
 */

#include <jni.h>
#include <string.h>

#ifndef Included_h4jni
#define Included_h4jni

#ifdef __cplusplus
#define ENVPTR (env)
#define ENVONLY
#else /* __cplusplus */
#define ENVPTR  (*env)
#define ENVONLY env
#endif /* __cplusplus */

/*
 * Used to silence compiler when a particular
 * function parameter is not used.
 */
#define UNUSED(o) (void)o

/*
 * Macro to check for a JNI exception after a JNI method is called.
 * If an exception occurred, the value of 'clearException' will determine
 * whether or not the exception will be cleared in order for the native
 * method to do its own error handling.
 *
 * If the exception does not get cleared, this macro will skip to the
 * cleanup+return section of the native method, since at that point
 * cleaning up and returning is the only safe thing that can be done.
 */
#define CHECK_JNI_EXCEPTION(envptr, clearException)                                                          \
    do {                                                                                                     \
        if (JNI_TRUE == (*envptr)->ExceptionCheck(envptr)) {                                                 \
            if (JNI_TRUE == clearException)                                                                  \
                (*envptr)->ExceptionClear(envptr);                                                           \
            else                                                                                             \
                goto done;                                                                                   \
        }                                                                                                    \
    } while (0)

/* Macros for class access */
/* Calling code must define ret_obj as jobject */
#define CALL_CONSTRUCTOR(envptr, classname, classsig, args, ret_obj)                                         \
    do {                                                                                                     \
        jmethodID constructor;                                                                               \
        jclass    cls;                                                                                       \
                                                                                                             \
        if (NULL == (cls = (*envptr)->FindClass(envptr, (classname)))) {                                     \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, "JNI error: GetObjectClass");                                         \
        }                                                                                                    \
        if (NULL == (constructor = (*envptr)->GetMethodID(envptr, cls, "<init>", (classsig)))) {             \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, "JNI error: GetMethodID failed");                                     \
        }                                                                                                    \
        if (NULL == (ret_obj = (*envptr)->NewObjectA(envptr, cls, constructor, (args)))) {                   \
            printf("FATAL ERROR: %s: Creation failed\n", classname);                                         \
            CHECK_JNI_EXCEPTION(envptr, JNI_FALSE);                                                          \
        }                                                                                                    \
    } while (0)

/*
 * Macros for pinning/unpinning objects.
 */
#define PIN_BYTE_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                       \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetByteArrayElements(envptr, arrayToPin, isCopy))) {                \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_BYTE_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                              \
    do {                                                                                                     \
        if (NULL == (outBuf = (jbyte *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) {  \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_BYTE_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                        \
    do {                                                                                                     \
        (*envptr)->ReleaseByteArrayElements(envptr, pinnedArray, (jbyte *)bufToRelease, freeMode);           \
    } while (0)

#define PIN_SHORT_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                      \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetShortArrayElements(envptr, arrayToPin, isCopy))) {               \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_SHORT_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                             \
    do {                                                                                                     \
        if (NULL == (outBuf = (jshort *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) { \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_SHORT_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                       \
    do {                                                                                                     \
        (*envptr)->ReleaseShortArrayElements(envptr, pinnedArray, (jshort *)bufToRelease, freeMode);         \
    } while (0)

#define PIN_INT_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                        \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetIntArrayElements(envptr, arrayToPin, isCopy))) {                 \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_INT_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                               \
    do {                                                                                                     \
        if (NULL == (outBuf = (jint *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) {   \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_INT_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                         \
    do {                                                                                                     \
        (*envptr)->ReleaseIntArrayElements(envptr, pinnedArray, (jint *)bufToRelease, freeMode);             \
    } while (0)

#define PIN_LONG_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                       \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetLongArrayElements(envptr, arrayToPin, isCopy))) {                \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_LONG_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                              \
    do {                                                                                                     \
        if (NULL == (outBuf = (jlong *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) {  \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_LONG_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                        \
    do {                                                                                                     \
        (*envptr)->ReleaseLongArrayElements(envptr, pinnedArray, (jlong *)bufToRelease, freeMode);           \
    } while (0)

#define PIN_FLOAT_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                      \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetFloatArrayElements(envptr, arrayToPin, isCopy))) {               \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_FLOAT_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                             \
    do {                                                                                                     \
        if (NULL == (outBuf = (jfloat *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) { \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_FLOAT_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                       \
    do {                                                                                                     \
        (*envptr)->ReleaseFloatArrayElements(envptr, pinnedArray, (jfloat *)bufToRelease, freeMode);         \
    } while (0)

#define PIN_DOUBLE_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                     \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetDoubleArrayElements(envptr, arrayToPin, isCopy))) {              \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_DOUBLE_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                            \
    do {                                                                                                     \
        if (NULL ==                                                                                          \
            (outBuf = (jdouble *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) {        \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_DOUBLE_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                      \
    do {                                                                                                     \
        (*envptr)->ReleaseDoubleArrayElements(envptr, pinnedArray, (jdouble *)bufToRelease, freeMode);       \
    } while (0)

#define PIN_BOOL_ARRAY(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                                       \
    do {                                                                                                     \
        if (NULL == (outBuf = (*envptr)->GetBooleanArrayElements(envptr, arrayToPin, isCopy))) {             \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define PIN_BOOL_ARRAY_CRITICAL(envptr, arrayToPin, outBuf, isCopy, failErrMsg)                              \
    do {                                                                                                     \
        if (NULL ==                                                                                          \
            (outBuf = (jboolean *)(*envptr)->GetPrimitiveArrayCritical(envptr, arrayToPin, isCopy))) {       \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_BOOL_ARRAY(envptr, pinnedArray, bufToRelease, freeMode)                                        \
    do {                                                                                                     \
        (*envptr)->ReleaseBooleanArrayElements(envptr, pinnedArray, (jboolean *)bufToRelease, freeMode);     \
    } while (0)

#define UNPIN_ARRAY_CRITICAL(envptr, pinnedArray, bufToRelease, freeMode)                                    \
    do {                                                                                                     \
        (*envptr)->ReleasePrimitiveArrayCritical(envptr, pinnedArray, bufToRelease, freeMode);               \
    } while (0)

/* Macros for string access */
#define PIN_JAVA_STRING(envptr, stringToPin, outString, isCopy, failErrMsg)                                  \
    do {                                                                                                     \
        if (NULL == (outString = (*envptr)->GetStringUTFChars(envptr, stringToPin, isCopy))) {               \
            CHECK_JNI_EXCEPTION(envptr, JNI_TRUE);                                                           \
            H4_JNI_FATAL_ERROR(envptr, failErrMsg);                                                          \
        }                                                                                                    \
    } while (0)

#define UNPIN_JAVA_STRING(envptr, pinnedString, stringToRelease)                                             \
    do {                                                                                                     \
        (*envptr)->ReleaseStringUTFChars(envptr, pinnedString, stringToRelease);                             \
    } while (0)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern jboolean h4JNIFatalError(JNIEnv *, const char *);
extern jboolean h4nullArgument(JNIEnv *, const char *);
extern jboolean h4badArgument(JNIEnv *, const char *);
extern jboolean h4outOfMemory(JNIEnv *, const char *);
extern jboolean h4assertion(JNIEnv *env, const char *);
extern jboolean h4NotImplemented(JNIEnv *, const char *);
extern jboolean h4libraryError(JNIEnv *env);
extern jboolean h4raiseException(JNIEnv *, const char *);

/*
 * The following macros are to facilitate immediate cleanup+return
 * from a native JNI method when an exception is to be thrown.
 * Since, in general, the "cleanup" methods are the only safe JNI
 * methods to call once an exception has occurred, we want to immediately
 * cleanup and return instead of letting the native method continue.
 *
 * Note that a native method can clear the exception when one occurs and
 * then do its own error handling, but we instead opt to immediately return.
 */
#define H4_JNI_FATAL_ERROR(env, message)                                                                     \
    do {                                                                                                     \
        h4JNIFatalError(env, message);                                                                       \
        goto done;                                                                                           \
    } while (0)

#define H4_NULL_ARGUMENT_ERROR(env, message)                                                                 \
    do {                                                                                                     \
        h4nullArgument(env, message);                                                                        \
        goto done;                                                                                           \
    } while (0)

#define H4_BAD_ARGUMENT_ERROR(env, message)                                                                  \
    do {                                                                                                     \
        h4badArgument(env, message);                                                                         \
        goto done;                                                                                           \
    } while (0)

#define H4_OUT_OF_MEMORY_ERROR(env, message)                                                                 \
    do {                                                                                                     \
        h4outOfMemory(env, message);                                                                         \
        goto done;                                                                                           \
    } while (0)

#define H4_ASSERTION_ERROR(env, message)                                                                     \
    do {                                                                                                     \
        h4assertion(env, message);                                                                           \
        goto done;                                                                                           \
    } while (0)

#define H4_UNIMPLEMENTED(env, message)                                                                       \
    do {                                                                                                     \
        h4NotImplemented(env, message);                                                                      \
        goto done;                                                                                           \
    } while (0)

#define H4_LIBRARY_ERROR(env)                                                                                \
    do {                                                                                                     \
        h4libraryError(env);                                                                                 \
        goto done;                                                                                           \
    } while (0)

#define H4_RAISE_EXCEPTION(env, message)                                                                     \
    do {                                                                                                     \
        h4raiseException(env, message);                                                                      \
        goto done;                                                                                           \
    } while (0)

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* Included_h4jni */
