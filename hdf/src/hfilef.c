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
 * File:    hfilef.c
 * Purpose: C stubs for Fortran low level routines
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
#   define nhclose   FNAME(HCLOSE)
#   define nhxisdir  FNAME(HXISDIR)
#   define nhxiscdir FNAME(HXISCDIR)
#   define nhddontatexit FNAME(HDDONTATEXIT)
#else
#   define nhiopen   FNAME(hiopen)
#   define nhclose   FNAME(hclose)
#   define nhxisdir  FNAME(hxisdir)
#   define nhxiscdir FNAME(hxiscdir)
#   define nhddontatexit FNAME(hddontatexit)
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
 * Name:    hclose
 * Purpose: Call DFclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hclose
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhclose(intf * file_id)
{
    return (Hclose(*file_id));
}

/*-----------------------------------------------------------------------------
 * Name:    hnumber
 * Purpose: Get numer of elements with tag
 * Inputs:  file_id: handle to HDF file to close
 * Returns: the number of objects of type 'tag' else FAIL
 * Users:   HDF Fortran programmers
 * Invokes: Hnumber
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhnumber(intf * file_id, intf *tag)
{
    return (Hnumber((int32) *file_id, (uint16) *tag));
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

/*-----------------------------------------------------------------------------
 * Name:    hddontatexit
 * Purpose: Call HDdont_atexit
 * Inputs:  
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: HDdont_atexit 
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
nhddontatexit(void)
{
   return((intf)(HDdont_atexit()));
}
