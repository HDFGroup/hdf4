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
 * File:    hfileF.c
 * Purpose: C stubs for Fortran low level routines
 * Invokes: hfile.c
 * Contents: 
 *  hiopen_:   call Hopen to open HDF file
 *  hclose_:   call Hclose to close HDF file
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "vg.h"

#ifndef HFILE_FNAMES
#   define HFILE_FNAMES
#ifdef DF_CAPFNAMES
#   define nhiopen   FNAME(HIOPEN)
#   define nhclose   FNAME(HCLOSE)
#else
#   define nhiopen   FNAME(hiopen)
#   define nhclose  FNAME(hclose)
#endif /* DF_CAPFNAMES */
#endif /* HFILE_FNAMES */

/*-----------------------------------------------------------------------------
 * Name:    hiopen
 * Purpose: call Hopen to open HDF file
 * Inputs:  name: name of file to open
 *          access: access mode - integer with value DFACC_READ etc. 
 *          defdds: default number of DDs per header block
 *          namelen: length of name
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hopen
 * Method:  Convert filename to C string, call Hopen
 *---------------------------------------------------------------------------*/

    FRETVAL(int32)
#ifdef PROTOTYPE
nhiopen(_fcd name, int *access, int16 *defdds, int *namelen)
#else
nhiopen(name, access, defdds, namelen)
    _fcd name;
    int *access;
    int16 *defdds;
    int *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    int32 ret;
    
    fn = HDf2cstring(name, *namelen);
    ret = (int32) Hopen(fn, *access, *defdds);
    HDfreespace(fn);
    return(ret);
}


/*-----------------------------------------------------------------------------
 * Name:    hclose
 * Purpose: Call DFclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hclose
 *---------------------------------------------------------------------------*/

    FRETVAL(intn)
#ifdef PROTOTYPE
nhclose(int32 *file_id)
#else
nhclose(file_id)
    int32 *file_id;
#endif /* PROTOTYPE */
{
    return(Hclose(*file_id));
}

