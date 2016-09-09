
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

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vstart
(JNIEnv *env, jclass clss, jlong fid)
{
    intn rval;

    rval = Vstart((int32)fid);

    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}


JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Vattach
(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref, jstring accessmode)
{
    int   retVal = -1;
    const char  *access;
    HFILEID        id = (HFILEID)fid;

    PIN_JAVA_STRING(accessmode, access);

    if (access != NULL) {
        /* open HDF file specified by hdf_HDF_file */
        retVal = Vattach(id, (int32)vgroup_ref, access);

        UNPIN_JAVA_STRING(accessmode, access);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return (jlong)retVal;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vdetach
(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    int32 rval;

    rval = Vdetach((int32)vgroup_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vend
(JNIEnv *env, jclass clss, jlong fid)
{
    HFILEID        id = (HFILEID)fid;
    Vend(id);
}


JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetid
(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref)
{
    int32 rval;

    rval = Vgetid((int32)fid, (int32)vgroup_ref);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jlong)rval;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetclass
(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfclassname)
{
    char *className;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (hdfclassname == NULL) {
        h4nullArgument(env, "Vgetclass: hdfclassname is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR hdfclassname) < 1) {
        h4badArgument(env, "Vgetclass: output array hdfclassname < order 1");
    } /* end else if */
    else {
        className = (char *)HDmalloc(H4_MAX_NC_CLASS+1);

        if (className == NULL) {
            h4outOfMemory(env,  "Vgetclass");
        } /* end if */
        else {
            /* get the class name of the vgroup */
            if (Vgetclass((int32)vgroup_id, className) < 0) {
                /* exception -- failed */
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                className[H4_MAX_NC_CLASS] = '\0';

                /* convert it to java string */
                rstring = ENVPTR->NewStringUTF(ENVPAR className);

                /*  create a Java String object in the calling environment... */
                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (jc == NULL) {
                    HDfree(className);
                    return ; /* exception is raised */
                }

                o = ENVPTR->GetObjectArrayElement(ENVPAR hdfclassname,0);
                if (o == NULL) {
                    HDfree(className);
                    return ;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                if (bb == JNI_FALSE) {
                    HDfree(className);
                    return ;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR hdfclassname,0,(jobject)rstring);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            } /* end else */

            HDfree(className);
        } /* end else */
    } /* end else */

    return;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetname
(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfname)
{
    char *name;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    if (hdfname == NULL) {
        h4nullArgument(env, "Vgetname: hdfname is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR hdfname) < 1) {
        h4badArgument(env, "Vgetname: array hdfname < order 1");
    } /* end else if */
    else {
        name = (char *)HDmalloc(H4_MAX_NC_NAME+1);
        if (name == NULL) {
            h4outOfMemory(env,  "Vgetname");
        } /* end if */
        else {
            if (Vgetname((int32)vgroup_id, name) == FAIL) {
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                name[H4_MAX_NC_NAME] = '\0';

                rstring = ENVPTR->NewStringUTF(ENVPAR name);

                jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                if (jc == NULL) {
                    HDfree(name);
                    return ; /* exception is raised */
                }
                o = ENVPTR->GetObjectArrayElement(ENVPAR hdfname,0);
                if (o == NULL) {
                    HDfree(name);
                    return ;
                }
                bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                if (bb == JNI_FALSE) {
                    HDfree(name);
                    return ;
                }
                ENVPTR->SetObjectArrayElement(ENVPAR hdfname,0,(jobject)rstring);
                ENVPTR->DeleteLocalRef(ENVPAR o);
            } /* end else */

            HDfree(name);
        } /* end else */
    } /* end else */

    return;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvg
(JNIEnv *env, jclass clss, jlong vgroup_id, jint vgroup_ref)
{
    intn rval;

    rval = Visvg((int32)vgroup_id, vgroup_ref);

    if (rval == FALSE)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvs
(JNIEnv *env, jclass clss, jlong vgroup_id, jint vdata_ref)
{
    intn rval;

    rval = Visvs((int32)vgroup_id, vdata_ref);

    if (rval == FALSE)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagrefs
(JNIEnv *env, jclass clss, jlong vgroup_id, jintArray tags, jintArray refs, jint size)
{
    jint *tagVal;
    jint *refVal;
    int32 retVal = -1;
    int copyMode = JNI_ABORT;

    jboolean iscopy;

    if (tags == NULL) {
        h4nullArgument(env, "Vgettagrefs: tags is NULL");
    } /* end if */
    else if (refs == NULL) {
        h4nullArgument(env, "Vgettagrefs: refs is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR tags) < size) {
        h4badArgument(env, "Vgettagrefs: output array tags < order 'size'");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR refs) < size) {
        h4badArgument(env, "Vgettagrefs: output array refs < order 'size'");
    } /* end else if */
    else {
        tagVal = ENVPTR->GetIntArrayElements(ENVPAR tags,&iscopy);

        if (tagVal == NULL) {
            h4JNIFatalError(env, "Vgettagrefs: tags not pinned");
        } /* end if */
        else {
            refVal = ENVPTR->GetIntArrayElements(ENVPAR refs,&iscopy);

            if (refVal == NULL) {
                h4JNIFatalError(env, "Vgettagrefs: refs not pinned");
            } /* end if */
            else {
                /* get the tag/ref pairs number in the vgroup */
                retVal = Vgettagrefs((int32)vgroup_id, (int32 *)tagVal, (int32 *)refVal, size);

                if (retVal == FAIL) {
                    CALL_ERROR_CHECK();
                }
                else {
                    copyMode = 0;
                }

                ENVPTR->ReleaseIntArrayElements(ENVPAR refs,refVal,copyMode);
            } /* end else */

            ENVPTR->ReleaseIntArrayElements(ENVPAR tags,tagVal,copyMode);
        } /* end else */
    } /* end else */

    return (jint)retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagref
(JNIEnv *env, jclass clss, jlong vgroup_id, jint index, jintArray tagref)
{
    intn retVal;
    jint * theArgs;
    jboolean bb;

    if (tagref == NULL) {
        h4nullArgument(env, "Vgettagref: tagref is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR tagref) < 2) {
        h4badArgument(env, "Vgettagref: output array tagref < order 2");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR tagref,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "Vgettagref: tagref not pinned");
        } /* end if */
        else {
            /* get the tag/ref pairs number in the vgroup */
            retVal = Vgettagref((int32)vgroup_id, index, (int32 *)&(theArgs[0]),
                    (int32 *)&(theArgs[1]));

            if (retVal == FAIL) {
                ENVPTR->ReleaseIntArrayElements(ENVPAR tagref,theArgs,JNI_ABORT);
                CALL_ERROR_CHECK();
            }
            else {
                ENVPTR->ReleaseIntArrayElements(ENVPAR tagref,theArgs,0);
            }
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vntagrefs
(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    int32 rval;

    rval = Vntagrefs((int32)vgroup_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinqtagref
(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    int32 rval;

    rval = Vinqtagref((int32)vgroup_id, (int32)tag, (int32)ref);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vlone
(JNIEnv *env, jclass clss, jlong fid, jintArray ref_array, jint arraysize)
{
    int retVal = -1;
    jint * arr;
    jboolean bb;

    if (ref_array == NULL) {
        h4nullArgument(env, "Vlone: ref_array is NULL");
    } /* end if */
    else if (ENVPTR->GetArrayLength(ENVPAR ref_array) < arraysize) {
        h4badArgument(env, "Vlone: output array ref_array < order 'arraysize'");
    } /* end else if */
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR ref_array,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "Vlone: ref_array not pinned");
        } /* end if */
        else {
            /* get the lone group reference number in the vgroup */
            retVal = Vlone((int32)fid, (int32 *)arr, arraysize);

            if (retVal == FAIL)
                CALL_ERROR_CHECK();

            ENVPTR->ReleaseIntArrayElements(ENVPAR ref_array,arr, 0);
        } /* end else */
    } /* end else */

    return retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vaddtagref
(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    intn rval;

    rval = Vaddtagref((int32) vgroup_id, (int32) tag, (int32) ref);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vdeletetagref
(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    intn rval;

    rval = Vdeletetagref((int32) vgroup_id, (int32) tag, (int32) ref);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vclose_I
(JNIEnv *env, jclass clss, jlong file_id)
{
    intn rval;

    rval =  Vclose((int32) file_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfind
(JNIEnv *env, jclass clss, jlong file_id, jstring vgname)
{
    int32 rval = -1;
    const char * vgn;

    PIN_JAVA_STRING(vgname, vgn);

    if (vgn != NULL) {
        rval = Vfind((int32)file_id, (char *)vgn);

        UNPIN_JAVA_STRING(vgname, vgn);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return rval;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindclass
(JNIEnv *env, jclass clss, jlong file_id, jstring vgclassname)
{
    int32 rval = -1;
    const char * vgcn;

    PIN_JAVA_STRING(vgclassname, vgcn);

    if (vgcn != NULL) {
        rval = Vfindclass((int32)file_id, (char *)vgcn);

        UNPIN_JAVA_STRING(vgclassname, vgcn);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vflocate
(JNIEnv *env, jclass clss, jint key, jstring field)
{
    int32 rval = -1;
    const char * fld;

    PIN_JAVA_STRING(field, fld);

    if (fld != NULL) {
        rval = Vflocate((int32)key, (char *)fld);

        UNPIN_JAVA_STRING(field, fld);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetnext
(JNIEnv *env, jclass clss, jint vkey, jint elem_ref)
{
    int32 rval;

    rval = Vgetnext((int32) vkey, (int32) elem_ref);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vinquire
(JNIEnv *env, jclass clss, jlong vgroup_id, jintArray n_entries, jobjectArray vgroup_name)
{
    intn rval;
    jclass jc;
    jstring rstring;
    char *name;
    jint * theArg;
    jobject o;
    jboolean bb;

    if (n_entries == NULL) {
        h4nullArgument(env, "Vinquire: n_entries is NULL");
    } /* end if */
    else if (vgroup_name == NULL) {
        h4nullArgument(env, "Vinquire: vgroup_name is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR n_entries) < 1) {
        h4badArgument(env, "Vinquire: output array n_entries < order 1");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR vgroup_name) < 1) {
        h4badArgument(env, "Vinquire: output array vgroup_name < order 1");
    } /* end else if */
    else {
        name = (char *)HDmalloc(H4_MAX_NC_NAME+1);

        if (name == NULL) {
            h4outOfMemory(env,  "Vinquire");
        } /* end if */
        else {
            theArg = ENVPTR->GetIntArrayElements(ENVPAR n_entries,&bb);

            if (theArg == NULL) {
                h4JNIFatalError(env, "Vinquire: n_entries not pinned");
            } /* end if */
            else {
                rval = Vinquire((int32) vgroup_id, (int32 *)&(theArg[0]), name);

                name[H4_MAX_NC_NAME] = '\0';

                if (rval == FAIL) {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR n_entries,theArg, JNI_ABORT);
                    CALL_ERROR_CHECK();
                }
                else {
                    ENVPTR->ReleaseIntArrayElements(ENVPAR n_entries,theArg, 0);

                    jc = ENVPTR->FindClass(ENVPAR  "java/lang/String");
                    if (jc == NULL) {
                        HDfree(name);
                        return JNI_FALSE;
                    }
                    o = ENVPTR->GetObjectArrayElement(ENVPAR vgroup_name,0);
                    if (o == NULL) {
                        HDfree(name);
                        return JNI_FALSE;
                    }
                    bb = ENVPTR->IsInstanceOf(ENVPAR o,jc);
                    if (bb == JNI_FALSE) {
                        HDfree(name);
                        return JNI_FALSE;
                    }
                    rstring = ENVPTR->NewStringUTF(ENVPAR name);
                    ENVPTR->SetObjectArrayElement(ENVPAR vgroup_name,0,(jobject)rstring);
                    ENVPTR->DeleteLocalRef(ENVPAR o);
                } /* end else */
            } /* end else */

            HDfree(name);
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinsert
(JNIEnv *env, jclass clss, jlong vgroup_id, jint v_id)
{
    int32 rval;

    rval = Vinsert((int32) vgroup_id, (int32) v_id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnrefs
(JNIEnv *env, jclass clss, jint vkey, jint tag)
{
    int32 rval;

    rval = Vnrefs((int32) vkey, (int32) tag);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vopen
(JNIEnv *env, jclass clss, jstring filename, jint access, jshort ndds)
{
    intn rval = -1;
    const char * str;

    PIN_JAVA_STRING(filename, str);

    if (str != NULL) {
        rval = Vopen((char *)str, (intn) access, (int16) ndds);

        UNPIN_JAVA_STRING(filename, str);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetclass
(JNIEnv *env, jclass clss, jlong vgroup_id, jstring hdfclassname)
{
    intn rval;
    const char * str;

    PIN_JAVA_STRING(hdfclassname, str);

    if (str != NULL) {
        rval = Vsetclass((int32)vgroup_id, (char *)str);

        UNPIN_JAVA_STRING(hdfclassname, str);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetname
(JNIEnv *env, jclass clss, jlong vgroup_id, jstring name)
{
    intn rval;
    const char *str;

    PIN_JAVA_STRING(name, str);

    if (str != NULL) {
        rval = Vsetname((int32)vgroup_id, (char *)str);

        UNPIN_JAVA_STRING(name, str);

        if (rval == FAIL)
            CALL_ERROR_CHECK();
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vattrinfo
(JNIEnv *env, jclass clss, jlong id, jint index, jobjectArray name, jintArray argv)
{
    int32 retVal;
    jint *theArgs;
    jboolean bb;
    jclass Sjc;
    jstring str;
    jobject o;
    char  nam[256];  /* what is the correct constant??? */

    if (name == NULL) {
        h4nullArgument(env, "Vattrinfo: name is NULL");
    } /* end if */
    else if (argv == NULL) {
        h4nullArgument(env, "Vattrinfo: argv is NULL");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR name) < 1) {
        h4badArgument(env, "Vattrinfo: output array name < order 1");
    } /* end else if */
    else if (ENVPTR->GetArrayLength(ENVPAR argv) < 5) {
        h4badArgument(env, "Vattrinfo: output array argv < order 5");
    } /* end else if */
    else {
        theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

        if (theArgs == NULL) {
            h4JNIFatalError(env, "Vattrinfo: argv not pinned");
        } /* end if */
        else {
            retVal = Vattrinfo2((int32)id, (int32)index, nam,
                    (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
                    (int32 *)&(theArgs[2]), (int32 *)&(theArgs[3]), (uint16 *)&(theArgs[4]));

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

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindattr
(JNIEnv *env, jclass clss, jlong id, jstring name)
{
    int32 retVal = -1;
    const char  *cname;

    PIN_JAVA_STRING(name, cname);

    if (cname != NULL) {
        retVal = Vfindattr((int32)id, cname);

        UNPIN_JAVA_STRING(name, cname);

        if (retVal == FAIL)
            CALL_ERROR_CHECK();
    }

    return retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetattr
(JNIEnv *env, jclass clss, jlong gr_id, jint attr_index, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "Vgetattr: values is NULL");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "Vgetattr: values not pinned");
        } /* end if */
        else {
            rval = Vgetattr2((int32) gr_id, (int32) attr_index,  (VOIDP) arr);

            if (rval == FAIL) {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);
                CALL_ERROR_CHECK();
            } /* end if */
            else {
                ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,0);
            } /* end else */
        } /* end else */
    } /* end else */

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetversion
(JNIEnv *env, jclass clss, jlong id)
{
    int32 rval;

    rval = Vgetversion((int32) id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnattrs
(JNIEnv *env, jclass clss, jlong id)
{
    int32 rval;

    rval = Vnattrs2((int32) id);
    if (rval == FAIL)
        CALL_ERROR_CHECK();

    return (jint)rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JILjava_lang_String_2
(JNIEnv *env, jclass clss, jlong gr_id, jstring attr_name, jlong data_type, jint count, jstring values)
{
    intn rval;
    const char *str;
    const char *val;

    PIN_JAVA_STRING_TWO(attr_name, str, values, val);

    if (str != NULL && val != NULL) {
        rval = Vsetattr((int32) gr_id, (char *)str, (int32) data_type,
                (int32) count, (VOIDP) val);

        if (rval == FAIL)
            CALL_ERROR_CHECK();

        UNPIN_JAVA_STRING_TWO(attr_name, str, values, val);
    }

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__JLjava_lang_String_2JI_3B
(JNIEnv *env, jclass clss, jlong id, jstring attr_name, jlong data_type, jint count, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    const char *str;
    jboolean bb;

    if (values == NULL) {
        h4nullArgument(env, "Vsetattr: values is null");
    } /* end if */
    else {
        arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);

        if (arr == NULL) {
            h4JNIFatalError(env, "Vsetattr: values not pinned");
        } /* end if */
        else {
            PIN_JAVA_STRING(attr_name, str);

            if (str != NULL) {
                rval = Vsetattr((int32) id, (char *)str, (int32) data_type,
                        (int32) count, (VOIDP) arr);

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
