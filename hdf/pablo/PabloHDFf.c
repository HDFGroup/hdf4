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
 * Funded by: National Aeronautics and Space Administration under NASA
 * Contracts NAG-1-613 and USRA 5555-22 and by the Advanced Research
 * Projects Agency under ARPA contracts DAVT63-91-C-0029 and
 * DABT63-93-C-0040.
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


FRETVAL(intf)
nihinitiotrace(_fcd traceFileName, intf * detail, intf * lifetime, 
               intf * timeWindow, float64 * timeWindowSize, intf * regionTrace,
               intf * regionSize, uint16 * procTraceMask, intf * namelen)
{
    char  *fn;
    intf  ret;

    fn = HDf2cstring(traceFileName, (intn) *namelen);
    ret = (intf) HDFinitIOTrace(fn,(intn)*detail,(intn)*lifetime,(intn)*timeWindow,
                                (float64)*timeWindowSize,(intn)*regionTrace,
                                (intn)*regionSize,(uint16)*procTraceMask);
    HDfree(fn);
    return (ret);
} /* nihiotrace() */

FRETVAL(intf)
nhendiotrace(VOID)
{
    return (HDFendIOTrace());
} /* nhendiotrace() */

#endif /* HAVE_PABLO */
