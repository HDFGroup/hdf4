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
 *  For details of the HDF libraries, see the HDF Documentation at:
 *    http://hdfdfgroup.org/HDF/doc/
 *
 */

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


/* Macros for string access */
#define PIN_JAVA_STRING(javastr,localstr,retdefault) {                           \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return (retdefault);                                                     \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return (retdefault);                                                     \
    }                                                                            \
}

#define PIN_JAVA_STRING0(javastr,localstr) {                                     \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return;                                                                  \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return;                                                                  \
    }                                                                            \
}

#define UNPIN_JAVA_STRING(javastr,localstr) {                                      \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));                \
}

#define PIN_JAVA_STRING_TWO(javastr,localstr,java2str,local2str,retdefault) {    \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return (retdefault);                                                     \
    }                                                                            \
    if ((java2str) == NULL) {                                                    \
        h4buildException(env, "second java string is NULL");                       \
        return (retdefault);                                                     \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return (retdefault);                                                     \
    }                                                                            \
    (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy);         \
    if ((local2str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        h4JNIFatalError(env, "second local c string is not pinned");             \
        return (retdefault);                                                     \
    }                                                                            \
}

#define PIN_JAVA_STRING_TWO0(javastr,localstr,java2str,local2str) {              \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return;                                                                  \
    }                                                                            \
    if ((java2str) == NULL) {                                                    \
        h4buildException(env, "second java string is NULL");                       \
        return;                                                                  \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return;                                                                  \
    }                                                                            \
    (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy);         \
    if ((local2str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        h4JNIFatalError(env, "second local c string is not pinned");             \
        return;                                                                  \
    }                                                                            \
}

#define UNPIN_JAVA_STRING_TWO(javastr,localstr,java2str,local2str) {             \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));                \
     ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));              \
}

#define PIN_JAVA_STRING_THREE(javastr,localstr,java2str,local2str,java3str,local3str,retdefault) {       \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return (retdefault);                                                     \
    }                                                                            \
    if ((java2str) == NULL) {                                                    \
        h4buildException(env, "second java string is NULL");                       \
        return (retdefault);                                                     \
    }                                                                            \
    if ((java3str) == NULL) {                                                    \
        h4buildException(env, "third java string is NULL");                        \
        return (retdefault);                                                     \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return (retdefault);                                                     \
    }                                                                            \
    (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy);         \
    if ((local2str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        h4JNIFatalError(env, "second local c string is not pinned");             \
        return (retdefault);                                                     \
    }                                                                            \
    (local3str) = ENVPTR->GetStringUTFChars(ENVPAR (java3str), &isCopy);         \
    if ((local3str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));           \
        h4JNIFatalError(env, "third local c string is not pinned");              \
        return (retdefault);                                                     \
    }                                                                            \
}

#define PIN_JAVA_STRING_THREE0(javastr,localstr,java2str,local2str,java3str,local3str) {       \
    jboolean isCopy;                                                             \
    if ((javastr) == NULL) {                                                     \
        h4buildException(env, "java string is NULL");                              \
        return;                                                                  \
    }                                                                            \
    if ((java2str) == NULL) {                                                    \
        h4buildException(env, "second java string is NULL");                       \
        return;                                                                  \
    }                                                                            \
    if ((java3str) == NULL) {                                                    \
        h4buildException(env, "third java string is NULL");                        \
        return;                                                                  \
    }                                                                            \
    (localstr) = ENVPTR->GetStringUTFChars(ENVPAR (javastr), &isCopy);           \
    if ((localstr) == NULL) {                                                    \
        h4JNIFatalError(env, "local c string is not pinned");                    \
        return;                                                                  \
    }                                                                            \
    (local2str) = ENVPTR->GetStringUTFChars(ENVPAR (java2str), &isCopy);         \
    if ((local2str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        h4JNIFatalError(env, "second local c string is not pinned");             \
        return;                                                                  \
    }                                                                            \
    (local3str) = ENVPTR->GetStringUTFChars(ENVPAR (java3str), &isCopy);         \
    if ((local3str) == NULL) {                                                   \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (javastr), (localstr));             \
        ENVPTR->ReleaseStringUTFChars(ENVPAR (java2str), (local2str));           \
        h4JNIFatalError(env, "third local c string is not pinned");              \
        return;                                                                  \
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
extern jboolean h4NotImplemented (JNIEnv *, const char *);
extern jboolean h4outOfMemory (JNIEnv *, const char *);
extern jboolean h4raiseException(JNIEnv *, const char *);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */

#endif /* _Included_h4jni */
