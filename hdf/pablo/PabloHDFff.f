C *
C * This file is an extension to NCSA HDF to enable the use of the
C * Pablo trace library.
C *
C * Developed by: The TAPESTRY Parallel Computing Laboratory
C *		  University of Illinois at Urbana-Champaign
C *		  Department of Computer Science
C *		  1304 W. Springfield Avenue
C *		  Urbana, IL	61801
C *
C * Copyright (c) 1995
C * The University of Illinois Board of Trustees.
C *      All Rights Reserved.
C *
C * PABLO is a registered trademark of
C * The Board of Trustees of the University of Illinois
C * registered in the U.S. Patent and Trademark Office.
C *
C * Author: Jonathan M. Reid (jreid@cs.uiuc.edu)
C *
C * Project Manager and Principal Investigator:
C *	Daniel A. Reed (reed@cs.uiuc.edu)
C *
C * Funded by: National Science Foundation grants NSF CCR86-57696,
C * NSF CCR87-06653 and NSF CDA87-22836 (Tapestry), NASA ICLASS Contract
C * No. NAG-1-613, DARPA Contract No. DABT63-91-K-0004, by a grant
C * from the Digital Equipment Corporation External Research Program,
C * and by a collaborative research agreement with the Intel Supercomputer
C * Systems Division.
C *

C-----------------------------------------------------------------------------
C File:     PabloHDFff.f
C Purpose:  Fortran stubs for Pablo routines
C Invokes:  PabloHDFf.c 
C Contents: 
C   hinitiotrace :   Call ihinitiotrace to initialize Pablo tracing
C Remarks: none
C-----------------------------------------------------------------------------

C-----------------------------------------------------------------------------
C Name: hinitiotrace
C Purpose:  call ihinitiotrace to initialize tracing
C Inputs:   tracefn: Trace file name
C           detail: 
C           lifetime:
C           timew:
C           timews:
C           regiont:
C           regionts:
C           proctmask:
C Returns: 0 on success, FAIL on failure 
C Users:   
C Invokes: ihinitiotrace
C-----------------------------------------------------------------------------

      integer function hinitiotrace(tracefn,detail,lifetime,timew,timews,
     +                              regiont,regionts,proctmask)

      character*(*) tracefn
      integer       detail,lifetime,timew,regiont,regionts,proctmask
      real          timews

      hinitiotrace = ihinitiotrace(tracefn,detail,lifetime,timew,timews,
     +                        regiont,regionts,proctmask, len(tracefn))
      return
      end
