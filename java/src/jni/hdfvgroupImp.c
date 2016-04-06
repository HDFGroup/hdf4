
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
Java_hdf_hdflib_HDFLibrary_Vstart(JNIEnv *env, jclass clss, jlong fid)
{
    intn rval;
    rval = Vstart((int32)fid);
    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vattach(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref, jstring accessmode)
{
    int   retVal;
    char  *access;

    access = (char *)ENVPTR->GetStringUTFChars(ENVPAR accessmode,0);

    /* open HDF file specified by hdf_HDF_file */
    retVal = Vattach(fid, vgroup_ref, (char *)access);

    ENVPTR->ReleaseStringUTFChars(ENVPAR accessmode,access);
    return retVal;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vdetach(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    Vdetach((int32)vgroup_id);
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vend(JNIEnv *env, jclass clss, jlong fid)
{
    Vend(fid);
}


JNIEXPORT jlong JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetid(JNIEnv *env, jclass clss, jlong fid, jint vgroup_ref)
{
    return(jlong)Vgetid((int32)fid, (int32)vgroup_ref);
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetclass(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfclassname)
{
    char *className;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;
    int r;

    if (hdfclassname == NULL) {
        /* nullpointer exception */
        return;
    }
    r = ENVPTR->GetArrayLength(ENVPAR hdfclassname);
    if (r == 0) {
        /* invalid arg exception */
        return;
    }

    className = (char *)HDmalloc(H4_MAX_NC_CLASS+1);
    if (className == NULL) {
        h4outOfMemory(env,  "Vgetclass");
        return;
    }
    /* get the class name of the vgroup */
    r = Vgetclass((int32)vgroup_id, className);

    if (r < 0) {
        /* exception -- failed */
        return;
    }

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

    HDfree(className);
    return;
}

JNIEXPORT void JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetname(JNIEnv *env, jclass clss, jlong vgroup_id, jobjectArray hdfname)
{
    char *name;
    jstring rstring;
    jclass jc;
    jobject o;
    jboolean bb;

    name = (char *)HDmalloc(H4_MAX_NC_NAME+1);
    if (name == NULL) {
        h4outOfMemory(env,  "Vgetname");
        return;
    }
    Vgetname((int32)vgroup_id, name);

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

    HDfree(name);
    return;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvg(JNIEnv *env, jclass clss, jlong vgroup_id, jint vgroup_ref)
{

    int rval;
    rval = Visvg((int32)vgroup_id, vgroup_ref);

    if (rval == TRUE || rval == 1) {
        return JNI_TRUE;
    }
    else {
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Visvs(JNIEnv *env, jclass clss, jlong vgroup_id, jint vdata_ref)
{

    int rval;
    rval = Visvs((int32)vgroup_id, vdata_ref);

    if (rval == TRUE || rval == 1) {
        return JNI_TRUE;
    }
    else {
        return JNI_FALSE;
    }

}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagrefs(JNIEnv *env, jclass clss, jlong vgroup_id,
        jintArray tags, jintArray refs, jint size)
{

    jint *tagVal;
    jint *refVal;
    int retVal;

    jboolean iscopy;

    tagVal = ENVPTR->GetIntArrayElements(ENVPAR tags,&iscopy);
    refVal = ENVPTR->GetIntArrayElements(ENVPAR refs,&iscopy);

    if (tagVal == NULL || refVal == NULL) {
        /* exception */
        return FAIL;
    }

    /* get the tag/ref pairs number in the vgroup */
    retVal = Vgettagrefs((int32)vgroup_id, (int32 *)tagVal, (int32 *)refVal, size);

    if ( retVal == FAIL ) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR tags,tagVal,JNI_ABORT);
        ENVPTR->ReleaseIntArrayElements(ENVPAR refs,refVal,JNI_ABORT);
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR tags,tagVal,0);
        ENVPTR->ReleaseIntArrayElements(ENVPAR refs,refVal,0);
    }

    return retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgettagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint index, jintArray tagref)
{
    int retVal;
    jint * theArgs;
    jboolean bb;

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR tagref,&bb);

    /* get the tag/ref pairs number in the vgroup */
    retVal = Vgettagref((int32)vgroup_id, index, (int32 *)&(theArgs[0]),
            (int32 *)&(theArgs[1]));

    if (retVal == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR tagref,theArgs,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseIntArrayElements(ENVPAR tagref,theArgs,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vntagrefs(JNIEnv *env, jclass clss, jlong vgroup_id)
{
    return (Vntagrefs((int32)vgroup_id));
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinqtagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    return(Vinqtagref((int32)vgroup_id, (int32)tag, (int32)ref));
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vlone(JNIEnv *env, jclass clss, jlong fid, jintArray ref_array, jint arraysize)
{
    int retVal;
    jint * arr;
    jboolean bb;

    if (ref_array == NULL) {
        arr = NULL;
    }
    else {
        arr = ENVPTR->GetIntArrayElements(ENVPAR ref_array,&bb);
    }

    /* get the lone group reference number in the vgroup */
    retVal = Vlone((int32)fid, (int32 *)arr, arraysize);

    if (arr != NULL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR ref_array,arr, 0);
    }

    return retVal;
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vaddtagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    return (Vaddtagref((int32) vgroup_id, (int32) tag, (int32) ref));
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vdeletetagref(JNIEnv *env, jclass clss, jlong vgroup_id, jint tag, jint ref)
{
    return (Vdeletetagref((int32) vgroup_id, (int32) tag, (int32) ref));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vclose_I(JNIEnv *env, jclass clss, jlong file_id)
{
    intn rval;
    rval =  Vclose((int32) file_id);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfind(JNIEnv *env, jclass clss, jlong file_id, jstring vgname)
{
    int32 rval;
    char * vgn;
    vgn = (char *)ENVPTR->GetStringUTFChars(ENVPAR vgname,0);

    rval = Vfind((int32)file_id, (char *)vgn);

    ENVPTR->ReleaseStringUTFChars(ENVPAR vgname,vgn);

    return rval;

}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindclass(JNIEnv *env, jclass clss, jlong file_id, jstring vgclassname)
{
    int32 rval;
    char * vgcn;
    vgcn = (char *)ENVPTR->GetStringUTFChars(ENVPAR vgclassname,0);

    rval = Vfindclass((int32)file_id, (char *)vgcn);

    ENVPTR->ReleaseStringUTFChars(ENVPAR vgclassname,vgcn);

    return rval;

}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vflocate(JNIEnv *env, jclass clss, jint key, jstring field)
{
    int32 rval;
    char * fld;
    fld = (char *)ENVPTR->GetStringUTFChars(ENVPAR field,0);

    rval = Vflocate((int32)key, (char *)fld);

    ENVPTR->ReleaseStringUTFChars(ENVPAR field,fld);

    return rval;

}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetnext(JNIEnv *env, jclass clss, jint vkey, jint elem_ref)
{
    return(Vgetnext((int32) vkey, (int32) elem_ref));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vinquire(JNIEnv *env, jclass clss, jlong vgroup_id, jintArray n_entries, jobjectArray vgroup_name)
{
    intn rval;
    jclass jc;
    jstring rstring;
    char *name;
    jint * theArg;
    jobject o;
    jboolean bb;

    name = (char *)HDmalloc(H4_MAX_NC_NAME+1);
    if (name == NULL) {
        h4outOfMemory(env,  "Vinquire");
        return JNI_FALSE;
    }
    theArg = ENVPTR->GetIntArrayElements(ENVPAR n_entries,&bb);

    rval = Vinquire((int32) vgroup_id, (int32 *)&(theArg[0]), name);

    name[H4_MAX_NC_NAME] = '\0';

    if (rval == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR n_entries,theArg, JNI_ABORT);
        HDfree(name);
        return JNI_FALSE;
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

        HDfree(name);
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vinsert(JNIEnv *env, jclass clss, jlong vgroup_id, jint v_id)
{
    return (Vinsert((int32) vgroup_id, (int32) v_id));
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnrefs(JNIEnv *env, jclass clss, jint vkey, jint tag)
{
    return(Vnrefs((int32) vkey, (int32) tag));
}


JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vopen(JNIEnv *env, jclass clss, jstring filename, jint access, jshort ndds)
{
    intn rval;
    char * str;
    str = (char *)ENVPTR->GetStringUTFChars(ENVPAR filename,0);


    rval = Vopen((char *)str, (intn) access, (int16) ndds);

    ENVPTR->ReleaseStringUTFChars(ENVPAR filename,str);

    return rval;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetclass(JNIEnv *env, jclass clss, jlong vgroup_id, jstring hdfclassname)
{
    intn rval;
    char * str;
    str = (char *)ENVPTR->GetStringUTFChars(ENVPAR hdfclassname,0);

    rval = Vsetclass((int32)vgroup_id, (char *)str);

    ENVPTR->ReleaseStringUTFChars(ENVPAR hdfclassname,str);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}


JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetname(JNIEnv *env, jclass clss, jlong vgroup_id, jstring name)
{
    intn rval;
    char *str;
    str =  (char *)ENVPTR->GetStringUTFChars(ENVPAR name,0);

    rval = Vsetname((int32)vgroup_id, (char *)str);

    ENVPTR->ReleaseStringUTFChars(ENVPAR name,str);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary__1Vattrinfo(JNIEnv *env, jclass clss, jlong id, jint index, jobjectArray name, jintArray argv)
{
    int32 retVal;
    jint *theArgs;
    jboolean bb;
    jclass Sjc;
    jstring str;
    jobject o;
    char  nam[256];  /* what is the correct constant??? */

    theArgs = ENVPTR->GetIntArrayElements(ENVPAR argv,&bb);

    retVal = Vattrinfo2((int32)id, (int32)index, nam,
        (int32 *)&(theArgs[0]), (int32 *)&(theArgs[1]),
        (int32 *)&(theArgs[2]), (int32 *)&(theArgs[3]), (uint16 *)&(theArgs[4]));

    nam[255] = '\0';

    if (retVal == FAIL) {
        ENVPTR->ReleaseIntArrayElements(ENVPAR argv,theArgs,JNI_ABORT);
        return JNI_FALSE;
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

        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vfindattr(JNIEnv *env, jclass clss, jlong id, jstring name)
{
    int32 retVal;
    char  *cname;

    cname =(char *) ENVPTR->GetStringUTFChars(ENVPAR name,0);

    retVal = Vfindattr((int32)id, cname);

    ENVPTR->ReleaseStringUTFChars(ENVPAR name,cname);

    return retVal;
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetattr(JNIEnv *env, jclass clss, jlong gr_id, jint attr_index, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    jboolean bb;

    arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);
    rval = Vgetattr2((int32) gr_id, (int32) attr_index,  (VOIDP) arr);
    if (rval == FAIL) {
        ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);
        return JNI_FALSE;
    }
    else {
        ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,0);
        return JNI_TRUE;
    }
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vgetversion(JNIEnv *env, jclass clss, jlong id)
{
    return (Vgetversion((int32) id));
}

JNIEXPORT jint JNICALL
Java_hdf_hdflib_HDFLibrary_Vnattrs(JNIEnv *env, jclass clss, jlong id)
{
    return (Vnattrs2((int32) id));
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__ILjava_lang_String_2IILjava_lang_String_2(JNIEnv *env, jclass clss, jlong gr_id,
        jstring attr_name, jlong data_type, jint count, jstring values)
{
    intn rval;
    char *str;
    char *val;

    str =(char *) ENVPTR->GetStringUTFChars(ENVPAR attr_name,0);
    val =(char *) ENVPTR->GetStringUTFChars(ENVPAR values,0);

    rval = Vsetattr((int32) gr_id, (char *)str, (int32) data_type,
        (int32) count, (VOIDP) val);

    ENVPTR->ReleaseStringUTFChars(ENVPAR attr_name,str);
    ENVPTR->ReleaseStringUTFChars(ENVPAR values,val);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_Vsetattr__ILjava_lang_String_2II_3B(JNIEnv *env, jclass clss, jlong id, jstring attr_name,
        jlong data_type, jint count, jbyteArray values)
{
    intn rval;
    jbyte *arr;
    char *str;
    jboolean bb;

    arr = ENVPTR->GetByteArrayElements(ENVPAR values,&bb);
    str =(char *) ENVPTR->GetStringUTFChars(ENVPAR attr_name,0);

    rval = Vsetattr((int32) id, (char *)str, (int32) data_type,
        (int32) count, (VOIDP) arr);

    ENVPTR->ReleaseStringUTFChars(ENVPAR attr_name,str);
    ENVPTR->ReleaseByteArrayElements(ENVPAR values,arr,JNI_ABORT);

    if (rval == FAIL) {
        return JNI_FALSE;
    }
    else {
        return JNI_TRUE;
    }
}

#ifdef __cplusplus
}
#endif
