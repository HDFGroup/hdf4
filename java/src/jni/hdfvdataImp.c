
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
#include "jni.h"
#include "h4jni.h"

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VSattach
(JNIEnv *env, jclass clss, jlong fid, jint vdata_ref, jstring accessmode)
{
    int32          retVal = -1;
    const char    *access;
    HFILEID        id = (HFILEID)fid;

    PIN_JAVA_STRING(accessmode, access);

    if (access != NULL) {
        /* open HDF file specified by hdf_HDF_file */
        retVal = VSattach(id, (int32)vdata_ref, access);

        UNPIN_JAVA_STRING(accessmode, access);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return (jlong)retVal;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSdetach
(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32   retVal;

    retVal = VSdetach((int32)vdata_id);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();
}

JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetid
(JNIEnv *env, jclass clss, jlong fid, jint vdata_ref)
{
    int32   retVal;
    HFILEID id = (HFILEID)fid;

    retVal = VSgetid(id, (int32)vdata_ref);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)retVal;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetclass
(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray hdfclass)
{
    char *className;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (hdfclass == NULL) {
        h4nullArgument(env, "VSgetclass: hdfclass is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR hdfclass) < 1) {
        h4badArgument(env, "VSgetclass: output array hdfclass < order 1");
    } /* end else if */
    else {
        className = (char *)HDmalloc(VSNAMELENMAX+1);
        if (className == NULL) {
            h4outOfMemory(env,  "VSgetclass");
        } /* end if */
        else {
            /* get the class class of the vgroup */
            if(VSgetclass((int32)vdata_id, className) < 0) {
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                className[VSNAMELENMAX] = '\0';

                /* convert it to java string */
                rstring = ENVPTR->NewStringUTF(ENVPAR className);

                o = ENVPTR->GetObjectArrayElement(ENVPAR hdfclass,0);
                if (o == NULL) {
                    HDfree(className);
                    return;
                }
                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (jc == NULL) {
                    HDfree(className);
                    return;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                if (bb == JNI_FALSE) {
                    HDfree(className);
                    return;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR hdfclass,0,(jobject)rstring);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            } /* end else */

            HDfree(className);
        } /* end else */
    } /* end else */
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetname
(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray hdfname)
{
    char nameName[VSNAMELENMAX+1];
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (hdfname == NULL) {
        h4nullArgument(env, "VSgetname: hdfname is NULL");
    } /* end if */
    else {
        /* get the name name of the vgroup */
        if(VSgetname((int32)vdata_id, nameName) < 0) {
            CALL_ERROR_CHECK();
        } /* end if */
        else {
            nameName[VSNAMELENMAX]='\0';

            /* convert it to java string */
            rstring = ENVPTR->NewStringUTF(ENVPAR nameName);

            o = ENVPTR->GetObjectArrayElement(ENVPAR hdfname,0);
            if (o == NULL) {
                return;
            }
            jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
            if (jc == NULL) {
                return;
            }
            bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
            if (bb == JNI_FALSE) {
                return;
            }
            ENVPTR->SetObjectArrayElement(ENVPAR hdfname,0,(jobject)rstring);
            ENVPTR->DeleteLocalRef(ENVPAR o);
        } /* end else */
    } /* end else */
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSelts
(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32        retVal;

    retVal = VSelts((int32)vdata_id);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSfdefine
(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname, jint numbertype, jint order)
{
    int32 retVal;
    const char  *fldName;

    PIN_JAVA_STRING(fieldname, fldName);

    if (fldName != NULL) {
        retVal = VSfdefine((int32)vdata_id,(char *)fldName,(int32)numbertype,(int32)order);

        UNPIN_JAVA_STRING(fieldname, fldName);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSfexist
(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname)
{
    int32 retVal;
    const char  *fldName;

    PIN_JAVA_STRING(fieldname, fldName);

    if (fldName != NULL) {
        /* Check the fields */
        retVal  = VSfexist((int32)vdata_id,(char *)fldName);

        UNPIN_JAVA_STRING(fieldname, fldName);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfind
(JNIEnv *env, jclass clss, jlong vdata_id, jstring fieldname)
{
    int32 retVal = -1;
    const char  *fldName;

    PIN_JAVA_STRING(fieldname, fldName);

    if (fldName != NULL) {
        /* Check the fields */
        retVal = VSfind((int32)vdata_id,(char *)fldName);

        UNPIN_JAVA_STRING(fieldname, fldName);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return(retVal);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetblocksize
(JNIEnv *env, jclass clss, jlong vdata_id, jint blocksize)
{
    int32 retVal;

    /* Check the fields */
    retVal = VSsetblocksize((int32)vdata_id,(int32)blocksize);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return(retVal);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetnumblocks
(JNIEnv *env, jclass clss, jlong vdata_id, jint numblocks)
{
    int32 retVal;

    /* Check the fields */
    retVal = VSsetnumblocks((int32)vdata_id,(int32)numblocks);

    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return(retVal);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetfields
(JNIEnv *env, jclass clss, jlong vdata_id, jobjectArray fields)
{
    int retVal = -1;
    jclass jc;
    char *flds;
    jstring rstring;
    jobject o;
    jboolean bb;

    if (fields == NULL) {
        h4nullArgument(env, "VSgetfields: fields is NULL");
    } /* end else if */
    else {
        flds = (char *)HDmalloc(25600);

        if (flds == NULL) {
            h4outOfMemory(env,  "VSgetfields");
        } /* end if */
        else {
            /* get the fields name in the vdata */
            retVal = VSgetfields((int32)vdata_id, flds);

            flds[25599] = '\0';

            if (retVal == FAIL) {
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                rstring = ENVPTR->NewStringUTF(ENVPAR  flds);
                o = ENVPTR->GetObjectArrayElement(ENVPAR fields,0);
                if (o == NULL) {
                    HDfree(flds);
                    return FAIL;
                }
                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (jc == NULL) {
                    HDfree(flds);
                    return FAIL;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                if (bb == JNI_FALSE) {
                    HDfree(flds);
                    return FAIL;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR fields,0,(jobject)rstring);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            } /* end else */

            HDfree(flds);
        } /* end else */
    } /* end else */

    return retVal;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetinterlace
(JNIEnv *env, jclass clss, jlong vdata_id)
{
    int32        retVal;

    retVal = VSgetinterlace((int32)vdata_id);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSinquire
(JNIEnv *env, jclass clss, jlong vdata_id, jintArray iargs, jobjectArray sargs)
{
    intn rval;
    char *flds;
    char *name;
    jclass jc;
    jstring rstring;
    jint * theIargs;
    jboolean bb;
    jobject o;


    if (iargs == NULL) {
        h4nullArgument(env, "VSinquire: iargs is NULL");
    } /* end if */
    else if (sargs == NULL) {
        h4nullArgument(env, "VSinquire: sargs is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR iargs) < 3) {
        h4badArgument(env, "VSinquire: output array iargs < order 3");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR sargs) < 2) {
        h4badArgument(env, "VSinquire: output array sargs < order 2");
    } /* end else if */
    else {
        flds = (char *)HDmalloc(MAX_FIELD_SIZE+1);

        if (flds == NULL) {
            h4outOfMemory(env,  "VSinquire");
        } /* end if */
        else {
            name = (char *)malloc(MAX_NC_NAME+1);

            if (name == NULL) {
                h4outOfMemory(env, "VSinquire");
            } /* end if */
            else {
                theIargs = ENVPTR->GetIntArrayElements(ENVPAR iargs,&bb);

                if (theIargs == NULL) {
                    h4JNIFatalError(env, "VSinquire: iargs not pinned");
                } /* end if */
                else {
                    rval = VSinquire((int32) vdata_id, (int32 *)&(theIargs[0]),
                            (int32 *)&(theIargs[1]), flds, (int32 *)&(theIargs[2]), name);

                    flds[MAX_FIELD_SIZE] = '\0';
                    name[MAX_NC_NAME] = '\0';

                    if (rval == FAIL) {
                        ENVPTR->ReleaseIntArrayElements(ENVPAR iargs,theIargs,JNI_ABORT);
                        CALL_ERROR_CHECK();
                    } /* end if */
                    else {
                        ENVPTR->ReleaseIntArrayElements(ENVPAR iargs,theIargs,0);

                        jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                        if (jc == NULL) {
                            HDfree(flds);
                            HDfree(name);
                            return JNI_FALSE;
                        }
                        o = ENVPTR->GetObjectArrayElement(ENVPAR sargs,0);
                        if (o == NULL) {
                            HDfree(flds);
                            HDfree(name);
                            return JNI_FALSE;
                        }
                        bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                        if (bb == JNI_FALSE) {
                            HDfree(flds);
                            return JNI_FALSE;
                        }
                        rstring = ENVPTR->NewStringUTF(ENVPAR flds);
                        ENVPTR->SetObjectArrayElement(ENVPAR sargs,0,(jobject)rstring);
                        ENVPTR->DeleteLocalRef(ENVPAR o);

                        o = ENVPTR->GetObjectArrayElement(ENVPAR sargs,1);
                        if (o == NULL) {
                            HDfree(flds);
                            HDfree(name);
                            return JNI_FALSE;
                        }
                        bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                        if (bb == JNI_FALSE) {
                            HDfree(flds);
                            HDfree(name);
                            return JNI_FALSE;
                        }
                        rstring = ENVPTR->NewStringUTF(ENVPAR name);
                        ENVPTR->SetObjectArrayElement(ENVPAR sargs,1,(jobject)rstring);
                        ENVPTR->DeleteLocalRef(ENVPAR o);
                    }
                } /* end else */

                HDfree(name);
            } /* end else */

            HDfree(flds);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetblockinfo
(JNIEnv *env, jclass clss, jlong vdata_id, jintArray iargs)
{
    intn rval;
    jint * theIargs;
    jboolean bb;

    if (iargs == NULL) {
        h4nullArgument(env, "VSgetblockinfo: iargs is NULL");
    } /* end if */
    else {
        theIargs = ENVPTR->GetIntArrayElements(ENVPAR iargs,&bb);

        if (theIargs == NULL) {
            h4JNIFatalError(env, "VSgetblockinfo: iargs not pinned");
        } /* end if */
        else {
            rval = VSgetblockinfo((int32) vdata_id, (int32 *)&(theIargs[0]),
                    (int32 *)&(theIargs[1]));

            if (rval == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR iargs,theIargs,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR iargs,theIargs,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSlone
(JNIEnv *env, jclass clss, jlong fid, jintArray ref_array, jint arraysize)
{
    int retVal = -1;
    jint * arr;
    jboolean bb;

    if (ref_array == NULL) {
        h4nullArgument(env, "VSlone: ref_array is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR ref_array,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "VSlone: ref_array not pinned");
        } /* end if */
        else {
            /* get the lone vdata reference number in the vdata */
            retVal = VSlone((int32)fid, (int32 *)arr, (int32)arraysize);

            if (retVal == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR ref_array,arr,JNI_ABORT);
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR ref_array,arr,0);
            } /* end else */
        } /* end else */
    } /* end else */

    return retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSread
(JNIEnv *env, jclass clss, jlong vdata_id, jbyteArray databuf, jint nrecords, jint interlace)
{
    int32   retVal = -1;
    jbyte  * data;
    jboolean bb;

    if (databuf == NULL) {
        h4nullArgument(env, "VSread: databuf is NULL");
    } /* end if */
    else {
        data = (jbyte *)ENVPTR->GetPrimitiveArrayCritical(ENVPAR databuf,&bb);

        if (data == NULL) {
            h4JNIFatalError(env, "VSread: databuf not pinned");
        } /* end if */
        else {
            /* retrieve the general info. */
            retVal = VSread((int32)vdata_id, (unsigned char *)data, nrecords, interlace);

            if (retVal == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR databuf,data,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleasePrimitiveArrayCritical(ENVPAR databuf,data,0);
            }
        } /* end else */
    } /* end else */

    return retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSseek
(JNIEnv *env, jclass clss, jlong vdata_id, jint nrecord)
{
    int32        retVal;

    retVal = VSseek((int32)vdata_id, (int32)nrecord);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetfields
(JNIEnv *env, jclass clss, jlong vdata_id, jstring fields)
{
    int32 retVal;
    const char *fldPtr;

    PIN_JAVA_STRING(fields, fldPtr);

    if (fldPtr != NULL) {
        /* set the vdata fields to read */
        retVal = VSsetfields((int32)vdata_id, (char *)fldPtr);

        UNPIN_JAVA_STRING(fields, fldPtr);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetinterlace
(JNIEnv *env, jclass clss, jlong vdata_id, jint interlace)
{
    int32 retVal;

    /* set the interlace for Vdata */
    retVal = VSsetinterlace((int32)vdata_id, (int32)interlace);
    if (retVal == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSsizeof
(JNIEnv *env, jclass clss, jlong vdata_id, jstring fields)
{
    int32 retVal = -1;
    const char *fldPtr;

    PIN_JAVA_STRING(fields, fldPtr);

    if (fldPtr != NULL) {
        /* get the size of a Vdata */
        retVal = VSsizeof((int32)vdata_id, (char *)fldPtr);

        UNPIN_JAVA_STRING(fields, fldPtr);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return(retVal);
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSappendable
(JNIEnv *env, jclass clss, jint vkey, jint block_size)
{
    int32 rval;

    rval = VSappendable((int32) vkey, (int32) block_size);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSdump
(JNIEnv *env, jclass clss, jint vkey)
{
    VSdump((int32) vkey);
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfindclass
(JNIEnv *env, jclass clss, jlong vdata_id, jstring hdfclassname)
{
    int32 rval = -1;
    const char *string;

    PIN_JAVA_STRING(hdfclassname, string);

    if (string != NULL) {
        /* get the class name of the vgroup */
        rval = VSfindclass((int32) vdata_id, string);

        UNPIN_JAVA_STRING(hdfclassname, string);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return rval;

}


/* no idea at all how to deal with 2-D arrays.... */
JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSfpack
(JNIEnv *env, jclass clss, jlong vdata_id, jint action, jstring fields_in_buf,
        jarray buf, jint bufsize, jstring fields, jarray bufptrs)
{

    /*
        VSfpack((int32) vdata_id, (intn) action, char
            *fields_in_buf, VOIDP buf, intn buf_size, intn
            n_records, char *fields, VOIDP bufptrs[]);
    */
    h4NotImplemented(env,  "VSfpack");
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetversion
(JNIEnv *env, jclass clss, jint key)
{
    int32        retVal;

    retVal = VSgetversion((int32) key);
    if (retVal <= 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetclass
(JNIEnv *env, jclass clss, jlong vdata_id, jstring vdata_class)
{
    int32 retVal;
    const char *string;

    PIN_JAVA_STRING(vdata_class, string);

    if (string != NULL) {
        retVal = VSsetclass((int32) vdata_id, (char *)string);

        UNPIN_JAVA_STRING(vdata_class, string);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetexternalfile
(JNIEnv *env, jclass clss, jint vkey, jstring filename, jint offset)
{
    intn rval;
    const char *string;

    PIN_JAVA_STRING(filename, string);

    if (string != NULL) {
        rval = VSsetexternalfile((int32) vkey, (char *)string, (int32) offset);

        UNPIN_JAVA_STRING(filename, string);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}


JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetname
(JNIEnv *env, jclass clss, jlong vdata_id, jstring vdata_name)
{
    int32 retVal;
    const char *string;

    PIN_JAVA_STRING(vdata_name, string);

    if (string != NULL) {
        retVal = VSsetname((int32) vdata_id, (char *)string);

        UNPIN_JAVA_STRING(vdata_name, string);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSwrite
(JNIEnv *env, jclass clss, jlong vdata_id, jbyteArray databuf, jint n_records, jint interlace)
{
    int32 rval = -1;
    jbyte *b;
    jboolean bb;

    if (databuf == NULL) {
        h4nullArgument(env, "VSwrite: databuf is NULL");
    } /* end if */
    else {
        b = ENVPTR->GetByteArrayElements(ENVPAR databuf,&bb);

        if (b == NULL) {
            h4JNIFatalError(env, "VSwrite: databuf not pinned");
        } /* end if */
        else {
            rval = VSwrite((int32) vdata_id, (unsigned char *)b, (int32) n_records, (int32) interlace);

            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR databuf,b,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR databuf,b,0);
            }
        } /* end else */
    } /* end else */

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSattrinfo
(JNIEnv *env, jclass clss, jlong id, jint index, jint attr_index, jobjectArray name, jintArray argv)
{
    int32 retVal;
    jint *theArgs;
    jboolean bb;
    jclass Sjc;
    jstring str;
    jobject o;
    char  nam[256];  /* what is the correct constant??? */

    if (name == NULL) {
        h4nullArgument(env, "VSattrinfo: name is NULL");
    } /* end if */
    else if (argv == NULL) {
        h4nullArgument(env, "VSattrinfo: argv is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR name) < 1) {
        h4badArgument(env, "VSattrinfo: output array name < order 1");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 3) {
        h4badArgument(env, "VSattrinfo: output array argv < order 3");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "VSattrinfo: argv not pinned");
        } /* end if */
        else {
            retVal = VSattrinfo((int32)id, (int32)index, (int32)attr_index,
                    nam,
                    (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                    (int32 *)&(theArgs[2]));

            nam[255] = '\0';

            if (retVal == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,0);

                str = ENVPTR->NewStringUTF(ENVPAR nam);
                o = ENVPTR->GetObjectArrayElement(ENVPAR name,0);
                if (o == NULL) {
                    return JNI_FALSE;
                }
                Sjc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (Sjc == NULL) {
                    return JNI_FALSE;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,Sjc);
                if (bb == JNI_FALSE) {
                    return JNI_FALSE;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR name,0,(jobject)str);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSfindex
(JNIEnv *env, jclass clss, jlong id, jstring name, jintArray findex)
{
    intn rval;
    const char *str;
    int copyMode;
    jint *arr;
    jboolean bb;

    copyMode = JNI_ABORT;

    if (findex == NULL) {
        h4nullArgument(env, "VSfindex: findex is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR findex,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "VSfindex: findex not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(name, str);

            if (str != NULL) {
                rval = VSfindex((int32) id, str, (int32 *)arr);

                UNPIN_JAVA_STRING(name, str);

                if (rval == FAIL) {
                    CALL_ERROR_CHECK();
                }
                else {
                    copyMode = 0;
                }
            } /* end if */

            ENVPTR->ReleaseIntArrayElements(ENVPAR findex,arr,copyMode);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfindattr
(JNIEnv *env, jclass clss, jlong id, jint index, jstring name)
{
    int32 retVal = -1;
    const char  *cname;

    PIN_JAVA_STRING(name, cname);

    if (cname != NULL) {
        retVal = VSfindattr((int32)id, (int32)index, cname);

        UNPIN_JAVA_STRING(name, cname);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSfnattrs
(JNIEnv *env, jclass clss, jlong id, jint attr)
{
    intn        retVal;

    retVal = VSfnattrs((int32)id, (int32)attr);
    if (retVal <= 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSgetattr
(JNIEnv *env, jclass clss, jlong id, jint field_index, jint attr_index, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "VSgetattr: values is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "VSgetattr: values not pinned");
        } /* end if */
        else {
            rval = VSgetattr((int32) id, (int32)field_index,
                    (int32) attr_index,  (VOIDP) arr);

            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSisattr
(JNIEnv *env, jclass clss, jlong id)
{
    intn rval;

    rval = VSisattr((int32) id);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_VSnattrs
(JNIEnv *env, jclass clss, jlong id)
{
    intn        retVal;

    retVal = VSnattrs((int32)id);
    if (retVal <= 0)
        CALL_ERROR_CHECK();

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JILjava_lang_String_2
(JNIEnv *env, jclass clss, jlong id, jint index, jstring attr_name, jlong data_type, jint count, jstring values)
{
    intn rval;
    const char *str;
    const char *val;

    PIN_JAVA_STRING_TWO(attr_name, str, values, val);

    if (str != NULL && val != NULL) {
        rval = VSsetattr((int32) id, (int32) index, (char *)str,
                (int32) data_type, (int32) count, (VOIDP) val);

        UNPIN_JAVA_STRING_TWO(attr_name, str, values, val);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_VSsetattr__JILjava_lang_String_2JI_3B
(JNIEnv *env, jclass clss, jlong id, jint index, jstring attr_name, jlong data_type, jint count, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    const char *str;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "VSsetattr: values is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "VSsetattr: values not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(attr_name, str);

            if (str != NULL) {
                rval = VSsetattr((int32) id, (int32) index, (char *)str,
                        (int32) data_type, (int32) count, (VOIDP) arr);

                UNPIN_JAVA_STRING(attr_name, str);

                if (rval == FAIL)
                    CALL_ERROR_CHECK();
            }

            ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

#ifdef __cplusplus
}
#endif
