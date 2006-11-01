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
 * File:    dfutilf.c
 * Purpose: C stubs for Fortran utility routines
 * Invokes: dfutil.c
 * Contents:
 *  dfindnr_:       For a given tag, find the next ref after the given ref
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "hproto.h"
/*-----------------------------------------------------------------------------
 * Name:    dfindnr
 * Purpose: For this tag, find the ref after lref
 * Inputs:  dfile: ptr to open DF file
 *          tag:   tag to look for
 *          lref:  ref after which to search
 *
 * Returns: the desired ref if successful, on failure with  DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfindnextref
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
ndfindnr(intf * dfile, intf * tag, intf * lref)
{
    return ((intf)DFfindnextref(*dfile, (uint16) *tag, (uint16) *lref));
}

/*
   CEND7MAX
 */

/*-----------------------------------------------------------------------------
 * Name:    dffindnextref
 * Purpose: For this tag, find the ref after lref
 * Inputs:  dfile: ptr to open DF file
 *          tag:   tag to look for
 *          lref:  ref after which to search
 *
 * Returns: the desired ref if successful, on failure with  DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfindnextref
 *---------------------------------------------------------------------------*/

FRETVAL(intf)
ndffindnextref(intf * dfile, intf * tag, intf * lref)
{
    return ((intf)DFfindnextref(*dfile, (uint16) *tag, (uint16) *lref));
}
