/*****************************************************************
 * NCSA HDF                                                      *
 * Software Development Group                                    *
 * National Center for Supercomputing Applications               *
 * University of Illinois at Urbana-Champaign                    *
 * 605 E. Springfield, Champaign IL 61820                        *
 *                                                               *
 * For conditions of distribution and use, see the accompanying  *
 * hdf/COPYING file.                                             *
 *                                                               *
 *****************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$ "
#endif

/***********************************************************
 * vattr.h - vdata/vgroup attribute interface
 *
 ***********************************************************  */

/* $Id$ */

#ifndef _VATTR_H
#define _VATTR_H

#include "hdf.h"
#include "vg.h"

/*********************************************
NAME
      VSfindex - find index of a named field in a vdata

DESCRIPTION
      This routine searchs field names only. It doesn't
      check against the vdata name. 

RETURNS
      Returns SUCCEED if successful;
              FAIL otherwise.

FORTRAN
       integer function vsffidx(vsid, fldnm, fldidx)
         integer vsid
         character*(*) fldnm
         integer fldidx

 ****************************************************/
extern intn VSfindex(
       int32 vsid, /*  IN: vdata id which contains this field */
       char *fieldname, /*  IN: field name */
       int32 *findex /*  OUT: field index */   
       );

/*************************************************** 
NAME
      VSsetattr - Set attribute for a vdata or a field of a
                     vdata

DESCRIPTION
        Create a vdata to store this attribute.
        If the field already has an attribute with the same name,
         replace the current values with the new values if the
         new data type and order are the same as the current ones;
         changes in data type or order will be considered as
         errors.
        No limit on max number of attributes. (int32 is the final
           limit.

RETURNS
     Returns SUCCEED if successful, FAIL otherwise.

FORTRAN
     integer function vsfsnat(vsid,findex,attrnm,dtype,count,values) --
            set a numeric attribute
       integer vsid, findex, dtype, count
       character*(*) attrnm
       integer  values(*)
     integer function vsfscat(vsid,findex,attrnm,dtype,count,values) --
            set a character type attribute
       integer vsid, findex, dtype, count
       character*(*) attrnm
       character*(*) values

***********************************************************/
extern intn VSsetattr(
       int32 vsid,     /*  IN: vdata access id  */
       int32 findex,   /* IN: number determined by assinging each 
                        field in a record a number starting with
                        0; ENTIRE_VDATA represents the vdata itself. */
       char *attrname, /*  IN: name of the attribute  */
       int32 datatype, /*  IN: data type of the attribute  */
       int32 count,    /*  IN: number of values the attribute has  */
       VOIDP values    /*  IN: a buffer which contains the values of
                           the attribute  */ 
     );

/***************************************************
NAME
      VSnattrs - get total number of attributes assigned for
                  this vdata and its fields

DESCRIPTION
      Use VSfnattrs to get number of attributes for a field
          or for the vdata ifself.

RETURNS
      Returns total number of attributes assigned to this vdata
      and its fields when successful, FAIL otherwise.

FORTRAN
      integer function vsfnat(vsid)
        integer vsid

***********************************************************/
extern intn VSnattrs(
       int32 vsid /* IN: access id of the vdata */
       );

/***************************************************
NAME
      VSfnattrs - get the number of attributes assigned to
                  a vdata or a field of a vdata.

DESCRIPTION
      Use VSnattrs to get total number of attributes for all
      fields and the vdata ifself.

RETURNS
      Returns the number of attributes assigned to
      the specified field (use ENTIRE_VDATA for the vdata itself)
      when successful, FAIL otherwise.

FORTRAN 
      integer function vsffnat(vsid, findex)
         integer vsid, findex

***********************************************************/
extern intn VSfnattrs(
       int32 vsid,   /* IN: access id of the vdata */
       int32 findex  /* IN: index of the field, 0 based.  
                        Use ENTIRE_VDATA for the vdata itself. */
       );

/***************************************************
NAME
      VSfindattr - get index of an attribute with given name
                   for a field of a vdata or for the vdata itself

DESCRIPTION
 
RETURNS
        Returns the index of the attr when successful,
        FAIL otherwise.

FORTRAN
     integer function vsffdat(vsid, findex, attrnm)
       integer vsid, findex
       character*(*) attrnm

***********************************************************/
extern intn VSfindattr(
       int32 vsid,     /* IN: access id of the vdata  */
       int32 findex,   /* IN: index of the field starting from 0;
                               ENTIRE_VDATA for the vdata  */
       char *attrname  /* IN: name of the attr   */
       );

/***************************************************
NAME
       VSattrinfo - get info of an attribute of a vdata/field

DESCRIPTION
        name, datatype or count can be NULL if which is
        not interested.

RETURNS
        Returns SUCCEED when successful, FAIL otherwise.

FORTRAN
       integer function vsfainf(vsid,findex,aindex,attrname,
                                dtype,count, size)
         integer vsid,findex,aindex,dtype,count, size
         character*(*) attrname

***********************************************************/
extern intn VSattrinfo(
       int32 vsid,      /* IN: vdata id  */
       int32 findex,    /* IN: field index. ENTIRE_VDATA for the vdata */
       intn attrindex,  /* IN: which attr of the field/vdata
                           attrindex is 0-based */
       char *name,      /* OUT: attribute name   */
       int32 *datatype, /* OUT: datatype of the attribute */
       int32 *count,    /*  OUT: number of values   */
       int32 *size   /*  OUT: size of the attr values on local machine */
       );

/***************************************************
NAME
       VSgetattr - get values of a specified attribute

DESCRIPTION

RETURNS
       Returns SUCCEED if successful, FAIL otherwise

FORTRAN
       integer function vsfgnat(vsid,findex,aindex,values) --
              get values of a numeric attribute
         integer vsid, findex, aindex
         integer values(*)
       integer function vsfgcat((vsid,findex,aindex,values) --
              get values of a character type attribute
         integer vsid, findex, aindex
         character*(*) values

***********************************************************/
extern intn VSgetattr(
       int32 vsid,     /* IN: vdata access id  */
       int32 findex,   /* IN: field index; ENTIRE_VDATA for vdata */
       intn attrindex, /* IN: attribute index  */
       VOIDP values    /* OUT: buffer holding attribute values. */
       );
/***************************************************
NAME
        VSisattr - test if a vdata is an attribute of
                    other object

DESCRIPTION

RETURNS
        Returns TRUE if the vdata is an attribute,
        FALSE otherwise

FORTRAN
       integer function vsfisat(vsid)
         integer vsid

***********************************************************/
extern intn VSisattr(
       int32 vsid    /* IN: vdata access id */
       );

/***************************************************
NAME
        Vsetattr - set an attribute for a vgroup

DESCRIPTION
        Create a vdata to store this attribute.
        If the vgroup already has an attribute with the same name
           and if the type and order are the same, use the new
           values to replace the current values. Any changes in
           datatype or order will be considered as an error.
        No limit on max number of attributes. (int32 is the final
           limit.

RETURNS
        Returns SUCCEED when successful, FAIL otherwise.

FORTRAN
       integer function vfsnatt(vgid,attrnm,dtype,count,values) --
           set numeric attributes
         integer vgid, dtype, count
         integer values(*)
         character*(*) attrnm
       integer function vfscatt(vgid,attrnm,dtype,count,values) --
           set character type attributes
         integer vgid, dtype, count
         character*(*) attrnm, values

***********************************************************/
extern intn Vsetattr(
       int32 vgid,     /* IN: access id of the vgroup  */
       char *attrname, /* IN: name of the attr     */
       int32 datatype, /* IN: datatype of the attr */
       int32 count,    /* IN: number of values the attr has */
       VOIDP values    /* IN: values of the attr  */
       );

/***************************************************
 NAME
        Vgetversion - gets vset version of a vgroup

DESCRIPTION
     Version 4 (VSET_NEW_VERSION) allows attrs.
     Version 3 (VSET_VERSION) is the version between
                 HDF3.2 through HDF4.0r2.
     Version 2 (VSET_OLD_VERSION) is the old vset (before
                 it was merged into HDF3.2?)

RETURNS
     Returns vset version number if successful, FAIL otherwise.

FORTRAN
       integer function vfgver(vgid)
         integer vgid

***********************************************************/
extern int32 Vgetversion(
       int32 vgid   /*  IN: vgroup access id */
       );

/***************************************************
NAME
      Vnattrs  - get number of attributes for a vgroup

DESCRIPTION

RETURNS
      Returns number of attributes when successful, Fail otherwise.

FORTRAN
       integer function vfnatts(vgid)
         integer vgid

***********************************************************/
extern intn Vnattrs(
       int32 vgid   /*    IN: access id of the vgroup */
       );

/***************************************************
NAME
        Vfindattr - get index of an attribute with given name

DESCRIPTION

RETURNS
        Returns the index of the attr when successful,
        FAIL otherwise.

FORTRAN
       integer function vffdatt(vg, attrnm)
         integer vg
         character*(*)   attrnm

***********************************************************/
extern intn Vfindattr(
       int32 vgid,      /* IN: access id of the vgroup  */
       char *attrname   /* IN: name of the attr  */
       ); 

/***************************************************
NAME
       Vattrinfo - get info of a vgroup attribute

DESCRIPTION
        name, datatype or count can be NULL if which is
        not interested.

RETURNS
        Returns SUCCEED when successful, FAIL otherwise.

FORTRAN
       integer function vfainfo(vgid, aindex, attrname, dtype,
                 count, size)
         integer vgid, vindex, dtype, count, size
         character*(*) attrname
***********************************************************/
extern intn Vattrinfo(
       int32 vgid,      /* IN: vgroup id   */
       intn attrindex,  /* IN: which attr's info we want
                            attrindex is 0-based     */
       char *name,      /* OUT: attribute name  */
       int32 *datatype, /* OUT: datatype of the attribute  */
       int32 *count,    /* OUT: number of values   */
       int32 *size      /* OUT: size of the attr values on local machine. */
       );

/***************************************************
NAME
      Vgetattr - read values of a vgroup attribute

DESCRIPTION

RETURNS
      Returns SUCCEED when successful, FAIL otherwise

FORTRAN 
       integer function vfgnatt(vgid,aindex,values) --
              get values of a numeric attribute
         integer vgid, aindex
         integer values(*)
       integer function vfgcatt(vgid,aindex,values) --
              get values of a character attribute
         integer vgid, aindex
         character*(*) values
    
***********************************************************/
extern intn Vgetattr(
       int32 vgid,     /* IN: vgroup id               */
       intn attrindex, /* IN: index of the attribute  */
       VOIDP values    /* OUT: where the values go.   */
       );











#endif


