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
 * File:    herrf.c
 * Purpose: C stubs for error-handling Fortran routines
 * Invokes: herr.c.
 * Contents:
 *     heprnt_:    Call HEprint to print error message
 * Remarks: none
 *---------------------------------------------------------------------------*/

#include "hdf.h"

#ifndef HERR_FNAMES
#   define HERR_FNAMES
#ifdef DF_CAPFNAMES
#   define nheprnt   FNAME(HEPRNT)
#else
#   define nheprnt   FNAME(heprnt)
#endif /* DF_CAPFNAMES */
#endif /* HERR_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    heprnt
 * Purpose: call HEprint to print error messages, starting from top of stack
 * Inputs:  print_levels: number of levels to print
 * Returns: 0 on success, FAIL on failure
 * Users:   Fortran stub routine
 * Invokes: HEprint
 * Remarks: This routine has one less parameter than HEprint, because it
 *          doesn't allow the user to specify the stream to print to.
 *          Instead it prints automatically to stdout.
 *---------------------------------------------------------------------------*/

FRETVAL(VOID)
nheprnt(intf * print_levels)
{
    HEprint(stderr, *print_levels);
}
/*-----------------------------------------------------------------------------
 * Name: hestringc
 * Purpose:  Calls HEstring
 * Inputs:   error_code - HDF error code
 * Outputs: error_message - error message assocoated with the error code
 * Retruns: SUCCEED (0) if successful and FAIL(-1) otherwise
 *----------------------------------------------------------------------------*/
 
 
 FRETVAL(intf)
#ifdef PROTOTYPE
nhestringc(intf *error_code,
            _fcd error_message, intf *len)
#else
nhestringc(error_code, error_message, len)
           intf *error_code;
           _fcd  error_message;
           intf  *len;
#endif /* PROTOTYPE */
{
   char *cstring;
   intn   status;
 
   cstring = NULL;
   status = -1;
   if (*len) cstring = (char *) HDmalloc((uint32)*len + 1);
   cstring = (char *)HEstring((hdf_err_code_t) *error_code);
   if (cstring) status = 0;
 
   HDpackFstring(cstring,  _fcdtocp(error_message),  *len);
 
   if(cstring)  HDfree((VOIDP)cstring);
 
 
   return((intf)status);
 
 
}
