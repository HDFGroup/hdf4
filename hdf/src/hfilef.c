#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.7  1993/04/19 22:48:11  koziol
General Code Cleanup to reduce/remove errors on the PC

 * Revision 1.6  1993/02/18  04:34:52  georgev
 * Minor syntax fix.
 *
 * Revision 1.5  1993/02/17  20:49:00  briand
 * added FORTRAN stub for Hnumber.
 *
 * Revision 1.4  1993/01/19  05:55:58  koziol
 * Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
 * port.  Lots of minor annoyances fixed.
 *
 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/09/11  14:15:04  koziol
 * Changed Fortran stubs' parameter passing to use a new typedef, intf,
 * which should be typed to the size of an INTEGER*4 in whatever Fortran
 * compiler the C functions need to be compatible with.  (This is mostly
 * for the PC and possibly for the Mac)
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
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

    FRETVAL(intf)
#ifdef PROTOTYPE
nhiopen(_fcd name, intf *access, intf *defdds, intf *namelen)
#else
nhiopen(name, access, defdds, namelen)
    _fcd name;
    intf *access;
    intf *defdds;
    intf *namelen;
#endif /* PROTOTYPE */
{
    char *fn;
    intf ret;
    
    fn = HDf2cstring(name, (intn)*namelen);
    ret = (intf) Hopen(fn, (intn)*access, (int16)*defdds);
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

    FRETVAL(intf)
#ifdef PROTOTYPE
nhclose(intf *file_id)
#else
nhclose(file_id)
    intf *file_id;
#endif /* PROTOTYPE */
{
    return(Hclose(*file_id));
}


/*-----------------------------------------------------------------------------
 * Name:    hnumber
 * Purpose: Call DFclose to close HDF file
 * Inputs:  file_id: handle to HDF file to close
 * Returns: 0 on success, FAIL on failure with error set
 * Users:   HDF Fortran programmers
 * Invokes: Hnumber
 *---------------------------------------------------------------------------*/

    FRETVAL(intf)
#ifdef PROTOTYPE
nhnumber(int32 file_id, uint16 tag)
#else
nhnumber(file_id, tag)
    int32 file_id;
    uint16 tag;
#endif /* PROTOTYPE */
{
    return(Hnumber(file_id,tag));
}




