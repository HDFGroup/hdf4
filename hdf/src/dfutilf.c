#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.1  1992/08/25 21:40:44  koziol
Initial revision

*/
/*-----------------------------------------------------------------------------
 * File:    dfutilF.c
 * Purpose: C stubs for Fortran utility routines
 * Invokes: dfutil.c
 * Contents: 
 *  dfindnr_:       For a given tag, find the next ref after the given ref
 *---------------------------------------------------------------------------*/

#include "hdf.h"

#ifndef DFUTIL_FNAMES
#   define DFUTIL_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfindnr          FNAME(DFINDNR)
#   define ndffindnextref    FNAME(DFFINDNEXTREF)
#else
#   define ndfindnr          FNAME(dfindnr)
#   define ndffindnextref    FNAME(dffindnextref)
#endif /* DF_CAPFNAMES */
#endif /* DFUTIL_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    dfindnr
 * Purpose: For this tag, find the ref after lref
 * Inputs:  dfile: ptr to open DF file
 *          tag:   tag to look for
 *          lref:  ref after which to search
 *
 * Returns: the desired ref if successful, on failure with	DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfindnextref
 *---------------------------------------------------------------------------*/

    FRETVAL(uint16)
#ifdef PROTOTYPE
ndfindnr(int32 *dfile, uint16 *tag, uint16 *lref)
#else
ndfindnr(dfile, tag, lref)
     int32 *dfile;
     uint16 *tag,
         *lref;
#endif /* PROTOTYPE */
{
    return(DFfindnextref(*dfile, *tag, *lref));
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
 * Returns: the desired ref if successful, on failure with	DFerror set
 * Users:   HDF Fortran programmers
 * Invokes: DFfindnextref
 *---------------------------------------------------------------------------*/

    FRETVAL(uint16)
#ifdef PROTOTYPE
ndffindnextref(int32 *dfile, uint16 *tag, uint16 *lref)
#else
ndffindnextref(dfile, tag, lref)
     int32 *dfile;
     uint16 *tag,
         *lref;
#endif /* PROTOTYPE */
{
    return(DFfindnextref(*dfile, *tag, *lref));
}
