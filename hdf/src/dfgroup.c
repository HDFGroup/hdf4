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
 * File:    dfgroup.c
 * Purpose: Low level functions for implementing groups
 * Invokes: df.c df.h
 * Contents:
 *  DFdiread: read in the data identifier list from the group
 *  DFdiget: get next data identifier from list
 *  DFdisetup: get ready to store a list of data identifiers to write out
 *  DFdiput: add a data identifier to the list to be written out
 *  DFdiwrite: write out the list of data identifiers
 *  DFDIputgroup: write out a group (array of tag/refs)
 *  DFDIgetgroup: read in a group (array of tag/refs)
 * Remarks: A group is a way of associating data elements with each other.
 *          It is a tag whose data is a list of tag/refs
 *          Each tag/ref combination is called a data identifier (DI).
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "herr.h"
#include "hfile.h"

static uint8 *Dilist=NULL;      /* buffer for list of tag/refs
                                  constituting group */
static int32 Dinlist;           /* no of tag/refs in list */
static int32 Ndi;               /* current position in list */

/*-----------------------------------------------------------------------------
 * Name:    DFdiread
 * Purpose: Read a list of DIs into memory
 * Inputs:  file_id: HDF file pointer
 *          tag, ref: id of group which is to be read in
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8getrig, other routines
 * Invokes: HDvalidfid, DFIfind, DFgetelement
 * Remarks: assumes tag is a group
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiread(int32 file_id, uint16 tag, uint16 ref)
#else
int DFdiread(file_id, tag, ref)
    int32 file_id;
    uint16 tag, ref;           /* tag, ref of group */
#endif
{
    char *FUNC="DFdiread";
    int32 length;

    HEclear();

    if (!HDvalidfid(file_id)) {
       HERROR(DFE_ARGS);
       return FAIL;
    }

    /* Find the group. */
    length = Hlength(file_id, tag, ref);
    if (length == FAIL) {
       return FAIL;
    }
    if (Dilist) HDfreespace((VOIDP)Dilist); /* ensure earlier allocs freed */
    Dilist = (uint8 *) HDgetspace((uint32)length);
    if (!Dilist) {
        HERROR(DFE_NOSPACE);
        return FAIL;
    }

    Dinlist = length / 4;      /* 4==sizeof DFdi */
    Ndi = 0;                   /* no DIs returned so far */

    /* read in group */
    if (Hgetelement(file_id, tag, ref, (uint8 *)Dilist)<0) {
        HDfreespace(Dilist);
        Dilist = NULL;         /* flag value */
        return FAIL;
    }
    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiget
 * Purpose: return next DI from list
 * Inputs:  di: space to return DI
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8getrig, other routines
 * Invokes: none
 * Remarks: frees Dilist space when all DIs returned
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiget(uint16 *ptag, uint16 *pref)
#else
int DFdiget(ptag, pref)
    uint16 *ptag;
    uint16 *pref;
#endif
{
    char *FUNC="DFdiget";
    uint8 *p;
    if (Ndi>=Dinlist) {
       return FAIL;
    }

    /* compute address of Ndi'th di */
    p = (uint8 *) Dilist + 4 * Ndi;
    Ndi++;
    UINT16DECODE(p, *ptag);
    UINT16DECODE(p, *pref);

    if (Ndi==Dinlist) {
        HDfreespace(Dilist);          /* if all returned, free storage */
        Dilist = NULL;         /* flag value */
    }
    return SUCCEED;
}


/*-----------------------------------------------------------------------------
 * Name:    DFdisetup
 * Purpose: setup space for storing a list of DIs to be written out
 * Inputs:  maxsize: maximum number of DIs expected in the list
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: This call should go away sometime.  Need better way to allocate
 *          space, possibly just use a big block of static space
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdisetup(int maxsize)
#else
int DFdisetup(maxsize)
    int maxsize;
#endif
{
    char *FUNC="DFdisetup";
    if (Dilist) HDfreespace(Dilist);
    Dilist = (uint8 *) HDgetspace((uint32)(maxsize * 4));
                               /* 4==sizeof(DFdi) */
    if (!Dilist) {
        HERROR(DFE_NOSPACE);
        return FAIL;
    }
    Dinlist = maxsize;         /* maximum size of list */
    Ndi = 0;                    /* current size of list */
    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiput
 * Purpose: add a DI to the list to be written out
 * Inputs:  tag, ref: DI to add
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: arg is tag/ref rather than DI for convenience
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiput(uint16 tag, uint16 ref)
#else
int DFdiput(tag, ref)
    uint16 tag, ref;
#endif
{
    char *FUNC="DFdiput";
    uint8 *p;

    if (Ndi>=Dinlist) {
        return FAIL;
    }

    /* compute address of Ndi'th di to put tag/ref in */
    p = (uint8 *) Dilist + 4 * Ndi++;
    UINT16ENCODE(p, tag);
    UINT16ENCODE(p, ref);

    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiwrite
 * Purpose: Write DI list out to HDF file
 * Inputs:  file_id: HDF file pointer
 *          tag, ref: tag and ref of group whose contents is the list
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: frees storage for Dilist
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiwrite(int32 file_id, uint16 tag, uint16 ref)
#else
int DFdiwrite(file_id, tag, ref)
    int32 file_id;
    uint16 tag, ref;
#endif
{
    char *FUNC="DFdiwrite";
    int ret;                   /* return value */

    if (!HDvalidfid(file_id)) {
       HERROR(DFE_ARGS);
        return FAIL;
    }

    ret = Hputelement(file_id, tag, ref, Dilist, (int32)Ndi*4);
                               /* 4==sizeof(DFdi) */
    HDfreespace(Dilist);
    Dilist = NULL;             /* flag value */
    Dinlist = Ndi = 0;
    return ret;
}

