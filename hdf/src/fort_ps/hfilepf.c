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
 * File:    hfilepf.c. based on hfilef.c,v 1.19
 * Purpose: C stubs for Fortran PowerStation low level routines
 * Invokes: hfile.c
 * Contents:
 *  hiopen_:   call Hopen to open HDF file
 *  hclose_:   call Hclose to close HDF file
 *---------------------------------------------------------------------------*/

#include "hdf.h"

#ifndef HFILE_FNAMES
#   define HFILE_FNAMES
#ifdef DF_CAPFNAMES
#   define nhiopen   FNAME(HIOPEN)
#   define nhiclose   FNAME(HICLOSE)
#   define nhinumbr   FNAME(HINUMBR)
#   define nhxisdir  FNAME(HXISDIR)
#   define nhxiscdir FNAME(HXISCDIR)
#else
#   define nhiopen   FNAME(hiopen)
#   define nhiclose   FNAME(hiclose)
#   define nhinumbr   FNAME(hinumbr)
#   define nhxisdir  FNAME(hxisdir)
#   define nhxiscdir FNAME(hxiscdir)
#endif /* DF_CAPFNAMES */
#endif /* HFILE_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    hiopen
 * Purpose: call Hopen to open HDF file
 * Inputs:  name: name of file to open
 *          acc_mode: access mode - integer with value DFACC_READ etc.
 *          defdds: default number of DDs per header block
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hopen
 * Method:  Convert filename to C string, call Hopen
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhiopen(_fcd name, intf * acc_mode, intf * defdds, intf * namelen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(name, (intn) *namelen);
    if (!fn)
	return(FAIL);
    ret = (intf) Hopen(fn, (intn) *acc_mode, (int16) *defdds);
    HDfree(fn);
    return (ret);
}

/*-----------------------------------------------------------------------------
 * Name:    hiclose
 * Purpose: Call DFclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   hclose
 * Invokes: Hclose
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhiclose(intf * file_id)
{
    return (Hclose(*file_id));
}

/*-----------------------------------------------------------------------------
 * Name:    hinumbr
 * Purpose: Get numer of elements with tag
 * Inputs:  file_id: handle to HDF file to close
 *          tag: the tag which the elements have.
 * Returns: number of elements on success, FAIL on failure with error set
 * Users:   hnumber
 * Invokes: Hnumber
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhinumbr(int32 file_id, uint16 tag)
{
    return (Hnumber(file_id, tag));
}

/*-----------------------------------------------------------------------------
 * Name:    hxisdir
 * Purpose: call HXsetdir to set the directory variable for locating an external file
 * Inputs:  dir: names of directory separated by colons
 * Returns: SUCCEED if no error, else FAIL
 * Users:   HDF Fortran programmers
 * Invokes: HXsetdir
 * Method:  Convert dir to C string, call HXsetdir
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhxisdir(_fcd dir, intf * dirlen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(dir, (intn) *dirlen);
    if (!fn)
	return(FAIL);
    ret = (intf) HXsetdir(fn);
    HDfree(fn);
    return (ret);
}

/*-----------------------------------------------------------------------------
 * Name:    hxiscdir
 * Purpose: call HXsetcreatedir to set the directory variable for creating an external file
 * Inputs:  dir: name of directory
 * Returns: SUCCEED if no error, else FAIL
 * Users:   HDF Fortran programmers
 * Invokes: HXsetcreatedir
 * Method:  Convert dir to C string, call HXsetcdir
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhxiscdir(_fcd dir, intf * dirlen)
{
    char       *fn;
    intf        ret;

    fn = HDf2cstring(dir, (intn) *dirlen);
    if (!fn)
	return(FAIL);
    ret = (intf) HXsetcreatedir(fn);
    HDfree(fn);
    return (ret);
}

