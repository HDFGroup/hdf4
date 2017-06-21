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

#include <jni.h>

#ifndef _Included_h4jni
#define _Included_h4jni

#ifdef __cplusplus
  #define ENVPTR (env)
  #define ENVPAR
  #define ENVONLY
#else /* __cplusplus */
  #define ENVPTR (*env)
  #define ENVPAR env,
  #define ENVONLY env
#endif /* __cplusplus */

/* Macros for class access */
/* Calling code must define ret_obj as jobject */
#define CALL_CONSTRUCTOR(classname,classsig,args) {                              \
    jclass     cls;                                                              \
    jmethodID  constructor;                                                      \
    cls = ENVPTR->FindClass(ENVPAR (classname));                                 \
    if (cls == 0) {                                                              \
        h4JNIFatalError(env, "JNI error: GetObjectClass\n");                     \
        ret_obj = NULL;                                                          \
    }                                                                            \
    constructor = ENVPTR->GetMethodID(ENVPAR cls, "<init>", (classsig));         \
    if (constructor == 0) {                                                      \
        h4JNIFatalError(env, "JNI error: GetMethodID failed\n");                 \
        ret_obj = NULL;                                                          \
    }                                                                            \
    ret_obj = ENVPTR->NewObjectA(ENVPAR cls, constructor, (args));               \
}

/* Macros for error check */
/* for now:  use top of exception stack:  fix this to do whole stack */
#define CALL_ERROR_CHECK() {                                                     \
    int16 errval;                                                                \
    jclass jc;                                                                   \
    errval = HEvalue((int32)1);                                                  \
    if (errval != DFE_NONE) {                                                    \
        h4buildException(env, errval);                                           \
        jc = ENVPTR->FindClass(ENVPAR  "hdf/hdflib/HDFLibraryException");        \
        if (jc != NULL)                                                          \
            ENVPTR->ThrowNew(ENVPAR jc,HEstring((hdf_err_code_t)errval));        \
    }                                                                            \
}


/* Macros for string access */
#define PIN_JAVA_STRING(javastr,localstr) {                                      \
    jboolean isCopy;                                                             \
    (localstr) = NULL;                                                           \
    if ((javastr) == NULL) {                                                     \
        h4nullArgument(env, "java string is NULL");                              \
    }                                                                            \
    else {                                                                       \
        (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);       \
        if ((localstr) == NULL) {                                                \
            h4JNIFatalError(env, "local c string is not pinned");                \
        }                                                                        \
    }                                                                            \
}

#define UNPIN_JAVA_STRING(javastr,localstr) {                                    \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));                \
}

#define PIN_JAVA_STRING_TWO(javastr,localstr,java2str,local2str) {               \
    jboolean isCopy;                                                             \
    (localstr) = NULL;                                                           \
    (local2str) = NULL;                                                          \
    if ((javastr) == NULL) {                                                     \
        h4nullArgument(env, "java string is NULL");                              \
    }                                                                            \
    else if ((java2str) == NULL) {                                               \
        h4nullArgument(env, "second java string is NULL");                       \
    }                                                                            \
    else {                                                                       \
        (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);       \
        if ((localstr) == NULL) {                                                \
            h4JNIFatalError(env, "local c string is not pinned");                \
        }                                                                        \
        else {                                                                   \
            (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy); \
            if ((local2str) == NULL) {                                           \
                ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));     \
                h4JNIFatalError(env, "second local c string is not pinned");     \
            }                                                                    \
        }                                                                        \
    }                                                                            \
}

#define UNPIN_JAVA_STRING_TWO(javastr,localstr,java2str,local2str) {             \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));                \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));              \
}

#define PIN_JAVA_STRING_THREE(javastr,localstr,java2str,local2str,java3str,local3str) {       \
    jboolean isCopy;                                                             \
    (localstr) = NULL;                                                           \
    (local2str) = NULL;                                                          \
    (local3str) = NULL;                                                          \
    if ((javastr) == NULL) {                                                     \
        h4nullArgument(env, "java string is NULL");                              \
    }                                                                            \
    else if ((java2str) == NULL) {                                               \
        h4nullArgument(env, "second java string is NULL");                       \
    }                                                                            \
    else if ((java3str) == NULL) {                                               \
        h4nullArgument(env, "third java string is NULL");                        \
    }                                                                            \
    else {                                                                       \
        (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);       \
        if ((localstr) == NULL) {                                                \
            h4JNIFatalError(env, "local c string is not pinned");                \
        }                                                                        \
        else {                                                                   \
            (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy); \
            if ((local2str) == NULL) {                                           \
                ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));     \
                h4JNIFatalError(env, "second local c string is not pinned");     \
            }                                                                    \
            else {                                                               \
                (local3str) = ENVPTR->GetStringUTFChars(ENVPAR (java3str), &isCopy);   \
                if ((local3str) == NULL) {                                       \
                    ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr)); \
                    ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));     \
                    h4JNIFatalError(env, "third local c string is not pinned");  \
                }                                                                \
            }                                                                    \
        }                                                                        \
    }                                                                            \
}

#define UNPIN_JAVA_STRING_THREE(javastr,localstr,java2str,local2str,java3str,local3str) {        \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));                \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));              \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (java3str), (local3str));              \
}

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern jboolean h4JNIFatalError(JNIEnv *, const char *);
extern jboolean h4buildException(JNIEnv *, jint);
extern jboolean h4badArgument (JNIEnv *, const char *);
extern jboolean h4nullArgument(JNIEnv *, const char *);
extern jboolean h4NotImplemented (JNIEnv *, const char *);
extern jboolean h4outOfMemory (JNIEnv *, const char *);
extern jboolean h4raiseException(JNIEnv *, const char *);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* _Included_h4jni */
