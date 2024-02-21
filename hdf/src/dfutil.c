/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including       *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-----------------------------------------------------------------------------
 * File:  dfutil.c
 *
 * Purpose:
 *    General purpose utility routines, and callable versions of hdf utilities
 *
 * Public functions:
 *    DFUfindnextref - For this tag, find the ref after given ref
 *
 * Remarks:
 *    This version assumes that all the values are floating point.
 *--------------------------------------------------------------------------*/

#include "hdf_priv.h"

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

uint16
DFfindnextref(int32 file_id, uint16 tag, uint16 lref)
{
    uint16 newtag = DFTAG_NULL, newref = DFTAG_NULL;
    int32  aid;

    HEclear();

    if (!HDvalidfid(file_id)) {
        HERROR(DFE_ARGS);
        return (uint16)FAIL;
    }

    aid = Hstartread(file_id, tag, lref);
    if (aid == FAIL)
        return (uint16)FAIL;

    if (lref != DFREF_WILDCARD)
        if (Hnextread(aid, tag, DFREF_WILDCARD, DF_CURRENT) == FAIL)
            return (uint16)FAIL;

    if (HQuerytagref(aid, &newtag, &newref) == FAIL)
        return (uint16)FAIL;

    Hendaccess(aid);
    return newref;
}
