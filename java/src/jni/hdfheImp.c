
/****************************************************************************
 ****************************************************************************/
/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif


#include "hdf.h"
#include "hfile.h"
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

JNIEXPORT jint JNICALL Java_hdf_hdflib_HDFLibrary_HEvalue
( JNIEnv *env,
jclass clss,
jint level)
{
    return HEvalue((int32) level);
}

JNIEXPORT void JNICALL Java_hdf_hdflib_HDFDeprecated_HEprint
( JNIEnv *env,
jclass clss,
jobject stream,  /* FILE * to output to? */
jint level)
{
printf("HEprint called:\n");fflush(stdout);
    /* HEprint is not implemented
        because I don't know HOW to implement it !*/
    /* Exception....*/
    h4NotImplemented( env, "HEprint");
}

JNIEXPORT jstring JNICALL Java_hdf_hdflib_HDFLibrary_HEstring
( JNIEnv *env,
jclass clss,
jshort error_code)
{
    char * str;
    jstring rstring;

    str = (char *)HEstring((hdf_err_code_t)error_code);

    rstring = ENVPTR->NewStringUTF(ENVPAR str);

    return rstring;
}

#ifdef __cplusplus
}
#endif
