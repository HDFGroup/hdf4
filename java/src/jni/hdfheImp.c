
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
#include "h4jni.h"

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_HEvalue
(JNIEnv *env, jclass clss, jint level)
{
    return HEvalue((int32) level);
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_HEprint
(JNIEnv *env, jclass clss, jobject stream, jint level)
{
    HEprint((FILE *) stream, (int32) level);
}

JNIEXPORT jstring JNICALL
Java_hdf_hdflib_HDFLibrary_HEstring
(JNIEnv *env, jclass clss, jshort error_code)
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
