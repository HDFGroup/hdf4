#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.2  1992/09/11 14:15:04  koziol
Changed Fortran stubs' parameter passing to use a new typedef, intf,
which should be typed to the size of an INTEGER*4 in whatever Fortran
compiler the C functions need to be compatible with.  (This is mostly
for the PC and possibly for the Mac)

 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*-----------------------------------------------------------------------------
 * File:    herrF.c
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
#ifdef PROTOTYPE
nheprnt(intf *print_levels)
#else
nheprnt(print_levels)
    intf *print_levels;
#endif /* PROTOTYPE */
{
    HEprint(stderr, *print_levels);
}

