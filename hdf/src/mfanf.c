/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*-----------------------------------------------------------------------------
 * File:    mfanf.c
 * Purpose: Fortran stubs for multi-file annotation routines
 * Invokes: 
 * Contents:
 *
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "mfan.h"

/*-----------------------------------------------------------------------------

  This file contains the HDF-style C stubs for the multi-file Annotation
  interface.

  The basic routines called by fortran will be of the form afxxxx

  If only a C stub is needed it will be named nafxxxx have the FNAME()
  function applied to it.

  If a Fortran stub is also required the fortran stub will be called
  afxxxx and the one in here will be nacxxx and again be FNAME()ed

-----------------------------------------------------------------------------*/

FRETVAL(intf)
nacstart(_fcd filename, intf *acc_mode, intf *namelen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(filename, (intn) *namelen);
    ret = ANstart(fn, (int32) *acc_mode);
    HDfreespace((VOIDP) fn);

    return (ret);
} /* nacstart() */

FRETVAL(intf)
naffileinfo(intf *file_id, intf *num_flabel, intf *num_fdesc, intf *num_olabel,
            intf *num_odesc)
{
  intf ret;
  int32 nflabel, nfdesc, nolabel, nodesc;

  ret = ANfileinfo((int32)*file_id,&nflabel,&nfdesc,&nolabel,&nodesc);

  *num_flabel = nflabel;
  *num_fdesc  = nfdesc;
  *num_olabel = nolabel;
  *num_odesc  = nodesc;

  return ret;
} /* naffileinfo() */

FRETVAL(intf)
nafend(intf *file_id)
{
  intf ret;

  ret = ANend((int32) *file_id);

  return ret;
} /* nafend() */

FRETVAL(intf)
nafcreate(intf *file_id, intf *etag, intf *eref, intf *atype)
{
  intf ret;

  ret = ANcreate((int32)*file_id,(uint16)*etag,(uint16)*eref,(ann_type)*atype);

  return ret;
} /* nafcreate() */

FRETVAL(intf)
naffcreate(intf *file_id, intf *atype)
{
  intf ret;

  ret = ANcreatef((int32)*file_id,(ann_type)*atype);

  return ret;
} /* naffcreate() */

FRETVAL(intf)
nafselect(intf *file_id, intf *index, intf *atype)
{
  intf ret;

  ret = ANselect((int32)*file_id,(int32)*index, (ann_type)*atype);

  return ret;
} /* nafselect() */

FRETVAL(intf)
nafnumann(intf *file_id, intf *atype, intf *etag, intf *eref)
{
  intf ret;

  ret = ANnumann((int32)*file_id,(ann_type)*atype,(uint16)*etag,(uint16)*eref);

  return ret;
} /* nafnumann() */

FRETVAL(intf)
nafannlist(intf *file_id, intf *atype, intf *etag, intf *eref, intf alist[])
{
  intf ret;
  int32 *tempanlist;
  intf nanns;
  intn i;


  nanns = ANnumann((int32)*file_id,(ann_type)*atype,(uint16)*etag,(uint16)*eref);

  if (ret < 0)
    return FAIL;

  /* create reflist with true uint16s to maintain compatibility
  ** with machines that allocate more than 16 bits per uint16. */
  if ((tempanlist = (int32 *) HDgetspace(nanns * sizeof(int32))) == NULL)
    return FAIL;

  ret = ANannlist((int32)*file_id,(ann_type)*atype,(uint16)*etag,(uint16)*eref,
                  tempanlist);
  if (ret < 0)
    return FAIL;

  /* move ref numbers into caller's reflist */
  for (i = 0; i < nanns; i++)
    alist[i] = tempanlist[i];

  HDfreespace((VOIDP) tempanlist);

  return ret;
} /* nafannlist() */

FRETVAL(intf)
nafannlen(intf *an_id)
{
  intf ret;

  ret = ANannlen((int32)*an_id);

  return ret;
} /* nafannlen() */

FRETVAL(intf)
nafwriteann(intf *an_id,_fcd *ann, intf *annlen)
{
  intf ret;

  ret = ANwriteann((int32)*an_id,(uint8 *) _fcdtocp(ann), (int32) *annlen);

  return ret;
} /* nafwriteann() */

FRETVAL(intf)
nafreadann(intf *an_id,_fcd *ann, intf *maxlen)
{
  intf ret;

  ret = ANreadann((int32)*an_id,(uint8 *) _fcdtocp(ann), (int32) *maxlen);

  return ret;
} /* nafreadann() */

FRETVAL(intf)
nafendaccess(intf *an_id)
{
  intf ret;

  ret = ANendaccess((int32)*an_id);

  return ret;
} /* nafendaccess() */



