/*
 * This file is an extension to NCSA HDF to enable the use of the
 * Pablo trace library.
 *
 * Developed by: The TAPESTRY Parallel Computing Laboratory
 *		 University of Illinois at Urbana-Champaign
 *		 Department of Computer Science
 *		 1304 W. Springfield Avenue
 *		 Urbana, IL	61801
 *
 * Copyright (c) 1995
 * The University of Illinois Board of Trustees.
 *      All Rights Reserved.
 *
 * PABLO is a registered trademark of
 * The Board of Trustees of the University of Illinois
 * registered in the U.S. Patent and Trademark Office.
 *
 * Author: Jonathan M. Reid (jreid@cs.uiuc.edu)
 *
 * Project Manager and Principal Investigator:
 *	Daniel A. Reed (reed@cs.uiuc.edu)
 *
 * Funded by: National Science Foundation grants NSF CCR86-57696,
 * NSF CCR87-06653 and NSF CDA87-22836 (Tapestry), NASA ICLASS Contract
 * No. NAG-1-613, DARPA Contract No. DABT63-91-K-0004, by a grant
 * from the Digital Equipment Corporation External Research Program,
 * and by a collaborative research agreement with the Intel Supercomputer
 * Systems Division.
 *
 */

/*--------------------------------------------------------------------------
 * File:  PabloHDFf.c
 * Purpose: support use of Pablo trace library to analyze HDF performance
 * Contents: Fortran C-stubs
 *  nihinitiotrace  : initialize trace
 *  nhendiotrace    : complete trace
 *-------------------------------------------------------------------------*/

#ifdef HAVE_PABLO

#include "hdf.h"
#include "ProcIDs.h"

extern int HDFinitIOTrace(char *traceFileName, intn detail, intn lifetime,
                          intn timeWindow, float64 timeWindowSize,
                          intn regionTrace, intn regionSize,
                          uint16 procTraceMask );
extern int HDFendIOTrace(VOID);

#ifndef PABLO_FNAMES
#   define PABLO_FNAMES
#ifdef DF_CAPFNAMES
#   define nihinitiotrace FNAME(IHINITIOTRACE)
#   define nhendiotrace   FNAME(HENDIOTRACE)
#else  /* !DF_CAPFNAMES */
#   define nihinitiotrace FNAME(ihinitiotrace)
#   define nhendiotrace   FNAME(hendiotrace)
#endif /* DF_CAPFNAMES */
#endif /* PABLO_FNAMES */

FRETVAL(intf)
nihinitiotrace(_fcd traceFileNname, intf * detail, intf * lifetime, 
               intf * timeWindow, float64 * timeWindowSize, intf * regionTrace,
               intf * regionSize, uint16 * procTraceMask, intf * namelen)
{
    char  *fn;
    intf  ret;

    fn = HDf2cstring(traceFileName, (intn) *namelen);
    ret = (intf) HDFinitIOTrace(fn,detail,lifetime,timeWindow,timeWindowSize,
                                regionTrace,regionSize,procTraceMask);
    HDfree(fn);
    return (ret);
} /* nihiotrace() */

FRETVAL(intf)
nhendiotrace(VOID)
{
    return (HDFendIOTrace());
} /* nhendiotrace() */

#endif
