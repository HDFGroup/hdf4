/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                      *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/*-----------------------------------------------------------------------------
 * File:  dfutil.c
 *
 * Purpose:
 *    General purpose utility routines, and callable versions of hdf utilities
 *
 * Invokes:
 *    latest libdf.a
 *
 * Public functions:
 *    DFUfindnextref - For this tag, find the ref after given ref
 *
 * Lower level functions:
 *
 * Private functions:
 *
 * Remarks:
 *    This version assumes that all the values are floating point.
 *--------------------------------------------------------------------------*/

#include "hdf.h"
#include "herr.h"

/*-----------------------------------------------------------------------------
 * Name:    DFfindnextref
 * Purpose: For this tag, find the ref after lref
 * Inputs:
 *          file_id: handle to open HDF file
 *          tag: tag to look for
 *          lref: ref after which to search
 * Returns: The desired ref if success, and FAIL on failure
 * Users:   HDF users, utilities, other routines
 * Invokes: HDvalidfid, 
 * Remarks:
 *---------------------------------------------------------------------------*/


#ifdef PROTOTYPE
uint16 DFfindnextref(int32 file_id, uint16 tag, uint16 lref)
#else
uint16 DFfindnextref(file_id, tag, lref)
    int32 file_id;
    uint16 tag, lref;
#endif /* PROTOTYPE */
{
    char *FUNC="DFfindnextref";
    uint16 newtag, newref;
    int32 aid;

    HEclear();

    if (!HDvalidfid(file_id)) {
        HERROR(DFE_ARGS);
        return (uint16)FAIL;
    }

    aid = Hstartread(file_id, tag, lref);
    if (aid == FAIL) 
        return (uint16)FAIL;

    if(lref != DFREF_WILDCARD)
        if (Hnextread(aid, tag, DFREF_WILDCARD, DF_CURRENT) == FAIL) 
            return (uint16)FAIL;
    
    if (HQuerytagref(aid, &newtag, &newref) == FAIL)
        return (uint16)FAIL;

    Hendaccess(aid);
    return (newref);
}

