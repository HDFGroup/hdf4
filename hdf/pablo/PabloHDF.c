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
 * Author: George Xin Zhou (xzhou@cs.uiuc.edu)
 * Contributing Author: Jonathan M. Reid (jreid@cs.uiuc.edu)
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
 * File:  PabloHDFc
 * Purpose: support use of Pablo trace library to analyze HDF performance
 * Contents:
 *  HDFinitIOtrace  : initialize trace
 *  HDFendIOtrace   : complete trace
 *-------------------------------------------------------------------------*/

#ifdef HAVE_PABLO

#include "hdf.h"
#include "ProcIDs.h"


uint16 procTrace;

int procEntries[] = {
	ID_DFANaddfds,
	ID_DFANaddfid,
	ID_DFANgetdesc,
	ID_DFANgetdesclen,
	ID_DFANgetfds,
	ID_DFANgetfdslen,
	ID_DFANgetfid,
	ID_DFANgetfidlen,
	ID_DFANgetlabel,
	ID_DFANgetlablen,
	ID_DFANlablist,
	ID_DFANlastref,
	ID_DFANputdesc,
	ID_DFANputlabel,
	ID_DFPaddpal,
	ID_DFPgetpal,
	ID_DFPlastref,
	ID_DFPnpals,
	ID_DFPputpal,
	ID_DFPreadref,
	ID_DFPrestart,
	ID_DFPwriteref,
	ID_DFR8addimage,
	ID_DFR8getdims,
	ID_DFR8getimage,
	ID_DFR8lastref,
	ID_DFR8nimages,
	ID_DFR8putimage,
	ID_DFR8readref,
	ID_DFR8restart,
	ID_DFR8setcompress,
	ID_DFR8setpalette,
	ID_DFR8writeref,
	ID_DFSDadddata,
	ID_DFSDclear,
	ID_DFSDendslab,
	ID_DFSDendslice,
	ID_DFSDgetcal,
	ID_DFSDgetdata,
	ID_DFSDgetdatalen,
	ID_DFSDgetdatastrs,
	ID_DFSDgetdimlen,
	ID_DFSDgetdims,
	ID_DFSDgetdimscale,
	ID_DFSDgetdimstrs,
	ID_DFSDgetfillvalue,
	ID_DFSDgetNT,
	ID_DFSDgetrange,
	ID_DFSDgetslice,
	ID_DFSDlastref,
	ID_DFSDndatasets,
	ID_DFSDpre32sdg,
	ID_DFSDputdata,
	ID_DFSDputslice,
	ID_DFSDreadref,
	ID_DFSDreadslab,
	ID_DFSDrestart,
	ID_DFSDsetcal,
	ID_DFSDsetdatastrs,
	ID_DFSDsetdims,
	ID_DFSDsetdimscale,
	ID_DFSDsetdimstrs,
	ID_DFSDsetfillvalue,
	ID_DFSDsetlengths,
	ID_DFSDsetNT,
	ID_DFSDsetrange,
	ID_DFSDstartslab,
	ID_DFSDstartslice,
	ID_DFSDwriteref,
	ID_DFSDwriteslab,
	ID_DF24addimage,
	ID_DF24getdims,
	ID_DF24getimage,
	ID_DF24lastref,
	ID_DF24nimages,
	ID_DF24putimage,
	ID_DF24readref,
	ID_DF24reqil,
	ID_DF24restart,
	ID_DF24setcompress,
	ID_DF24setdims,
	ID_DF24setil,
	ID_Happendable,
	ID_Hclose,
	ID_Hdeldd,
	ID_Hdupdd,
	ID_Hendaccess,
	ID_Hfind,
	ID_Hgetelement,
	ID_Hgetfileversion,
	ID_Hgetlibversion,
	ID_Hinquire,
	ID_Hishdf,
	ID_HLcreate,
	ID_Hlength,
	ID_Hnewref,
	ID_Hnextread,
	ID_Hnumber,
	ID_Hoffset,
	ID_Hopen,
	ID_Hputelement,
	ID_Hread,
	ID_Hseek,
	ID_Hstartread,
	ID_Hstartwrite,
	ID_Hsync,
	ID_Htrunc,
	ID_Hwrite,
	ID_HXcreate,
	ID_HEclear,
	ID_HEprint,
	ID_HEpush,
	ID_HEreport,
	ID_HEstring,
	ID_SDattrinfo,
	ID_SDcreate,
	ID_SDdiminfo,
	ID_SDend,
	ID_SDendaccess,
	ID_SDfileinfo,
	ID_SDfindattr,
	ID_SDgetcal,
	ID_SDgetdatastrs,
	ID_SDgetdimid,
	ID_SDgetdimscale,
	ID_SDgetdimstrs,
	ID_SDgetfillvalue,
	ID_SDgetinfo,
	ID_SDgetrange,
	ID_SDidtoref,
	ID_SDiscoordvar,
	ID_SDnametoindex,
	ID_SDreadattr,
	ID_SDreaddata,
	ID_SDreftoindex,
	ID_SDselect,
	ID_SDsetattr,
	ID_SDsetcal,
	ID_SDsetdatastrs,
	ID_SDsetdimname,
	ID_SDsetdimscale,
	ID_SDsetdimstrs,
	ID_SDsetexternalfile,
	ID_SDsetfillvalue,
	ID_SDsetrange,
	ID_SDstart,
	ID_SDwritedata,
	ID_VFfieldesize,
	ID_VFfieldisize,
	ID_VFfieldname,
	ID_VFfieldorder,
	ID_VFfieldtype,
	ID_VFnfields,
	ID_Vaddtagref,
	ID_Vattach,
	ID_Vdetach,
	ID_Vgetclass,
	ID_Vgetid,
	ID_Vgetname,
	ID_Vgettagref,
	ID_Vgettagrefs,
	ID_Vinqtagref,
	ID_Vinquire,
	ID_Vinsert,
	ID_Vlone,
	ID_Vntagrefs,
	ID_Vsetclass,
	ID_Vsetname,
	ID_VHmakegroup,
	ID_VHstoredata,
	ID_VHstoredatam,
	ID_VSattach,
	ID_VSdetach,
	ID_VSelts,
	ID_VSfdefine,
	ID_VSfexist,
	ID_VSfind,
	ID_VSgetclass,
	ID_VSgetfields,
	ID_VSgetid,
	ID_VSgetinterlace,
	ID_VSgetname,
	ID_VSinquire,
	ID_VSlone,
	ID_VSread,
	ID_VSseek,
	ID_VSsetclass,
	ID_VSsetfields,
	ID_VSsetinterlace,
	ID_VSsetname,
	ID_VSsizeof,
	ID_VSwrite
};

int procExits[] = {
	-ID_DFANaddfds,
	-ID_DFANaddfid,
	-ID_DFANgetdesc,
	-ID_DFANgetdesclen,
	-ID_DFANgetfds,
	-ID_DFANgetfdslen,
	-ID_DFANgetfid,
	-ID_DFANgetfidlen,
	-ID_DFANgetlabel,
	-ID_DFANgetlablen,
	-ID_DFANlablist,
	-ID_DFANlastref,
	-ID_DFANputdesc,
	-ID_DFANputlabel,
	-ID_DFPaddpal,
	-ID_DFPgetpal,
	-ID_DFPlastref,
	-ID_DFPnpals,
	-ID_DFPputpal,
	-ID_DFPreadref,
	-ID_DFPrestart,
	-ID_DFPwriteref,
	-ID_DFR8addimage,
	-ID_DFR8getdims,
	-ID_DFR8getimage,
	-ID_DFR8lastref,
	-ID_DFR8nimages,
	-ID_DFR8putimage,
	-ID_DFR8readref,
	-ID_DFR8restart,
	-ID_DFR8setcompress,
	-ID_DFR8setpalette,
	-ID_DFR8writeref,
	-ID_DFSDadddata,
	-ID_DFSDclear,
	-ID_DFSDendslab,
	-ID_DFSDendslice,
	-ID_DFSDgetcal,
	-ID_DFSDgetdata,
	-ID_DFSDgetdatalen,
	-ID_DFSDgetdatastrs,
	-ID_DFSDgetdimlen,
	-ID_DFSDgetdims,
	-ID_DFSDgetdimscale,
	-ID_DFSDgetdimstrs,
	-ID_DFSDgetfillvalue,
	-ID_DFSDgetNT,
	-ID_DFSDgetrange,
	-ID_DFSDgetslice,
	-ID_DFSDlastref,
	-ID_DFSDndatasets,
	-ID_DFSDpre32sdg,
	-ID_DFSDputdata,
	-ID_DFSDputslice,
	-ID_DFSDreadref,
	-ID_DFSDreadslab,
	-ID_DFSDrestart,
	-ID_DFSDsetcal,
	-ID_DFSDsetdatastrs,
	-ID_DFSDsetdims,
	-ID_DFSDsetdimscale,
	-ID_DFSDsetdimstrs,
	-ID_DFSDsetfillvalue,
	-ID_DFSDsetlengths,
	-ID_DFSDsetNT,
	-ID_DFSDsetrange,
	-ID_DFSDstartslab,
	-ID_DFSDstartslice,
	-ID_DFSDwriteref,
	-ID_DFSDwriteslab,
	-ID_DF24addimage,
	-ID_DF24getdims,
	-ID_DF24getimage,
	-ID_DF24lastref,
	-ID_DF24nimages,
	-ID_DF24putimage,
	-ID_DF24readref,
	-ID_DF24reqil,
	-ID_DF24restart,
	-ID_DF24setcompress,
	-ID_DF24setdims,
	-ID_DF24setil,
	-ID_Happendable,
	-ID_Hclose,
	-ID_Hdeldd,
	-ID_Hdupdd,
	-ID_Hendaccess,
	-ID_Hfind,
	-ID_Hgetelement,
	-ID_Hgetfileversion,
	-ID_Hgetlibversion,
	-ID_Hinquire,
	-ID_Hishdf,
	-ID_HLcreate,
	-ID_Hlength,
	-ID_Hnewref,
	-ID_Hnextread,
	-ID_Hnumber,
	-ID_Hoffset,
	-ID_Hopen,
	-ID_Hputelement,
	-ID_Hread,
	-ID_Hseek,
	-ID_Hstartread,
	-ID_Hstartwrite,
	-ID_Hsync,
	-ID_Htrunc,
	-ID_Hwrite,
	-ID_HXcreate,
	-ID_HEclear,
	-ID_HEprint,
	-ID_HEpush,
	-ID_HEreport,
	-ID_HEstring,
	-ID_SDattrinfo,
	-ID_SDcreate,
	-ID_SDdiminfo,
	-ID_SDend,
	-ID_SDendaccess,
	-ID_SDfileinfo,
	-ID_SDfindattr,
	-ID_SDgetcal,
	-ID_SDgetdatastrs,
	-ID_SDgetdimid,
	-ID_SDgetdimscale,
	-ID_SDgetdimstrs,
	-ID_SDgetfillvalue,
	-ID_SDgetinfo,
	-ID_SDgetrange,
	-ID_SDidtoref,
	-ID_SDiscoordvar,
	-ID_SDnametoindex,
	-ID_SDreadattr,
	-ID_SDreaddata,
	-ID_SDreftoindex,
	-ID_SDselect,
	-ID_SDsetattr,
	-ID_SDsetcal,
	-ID_SDsetdatastrs,
	-ID_SDsetdimname,
	-ID_SDsetdimscale,
	-ID_SDsetdimstrs,
	-ID_SDsetexternalfile,
	-ID_SDsetfillvalue,
	-ID_SDsetrange,
	-ID_SDstart,
	-ID_SDwritedata,
	-ID_VFfieldesize,
	-ID_VFfieldisize,
	-ID_VFfieldname,
	-ID_VFfieldorder,
	-ID_VFfieldtype,
	-ID_VFnfields,
	-ID_Vaddtagref,
	-ID_Vattach,
	-ID_Vdetach,
	-ID_Vgetclass,
	-ID_Vgetid,
	-ID_Vgetname,
	-ID_Vgettagref,
	-ID_Vgettagrefs,
	-ID_Vinqtagref,
	-ID_Vinquire,
	-ID_Vinsert,
	-ID_Vlone,
	-ID_Vntagrefs,
	-ID_Vsetclass,
	-ID_Vsetname,
	-ID_VHmakegroup,
	-ID_VHstoredata,
	-ID_VHstoredatam,
	-ID_VSattach,
	-ID_VSdetach,
	-ID_VSelts,
	-ID_VSfdefine,
	-ID_VSfexist,
	-ID_VSfind,
	-ID_VSgetclass,
	-ID_VSgetfields,
	-ID_VSgetid,
	-ID_VSgetinterlace,
	-ID_VSgetname,
	-ID_VSinquire,
	-ID_VSlone,
	-ID_VSread,
	-ID_VSseek,
	-ID_VSsetclass,
	-ID_VSsetfields,
	-ID_VSsetinterlace,
	-ID_VSsetname,
	-ID_VSsizeof,
	-ID_VSwrite
};


/* ------------------------------------------------------------------
NAME
       HDFinitIOTrace -- initialize I/O tracing
USAGE
       VOID HDFinitIOTrace(traceFileName, detail, lifetime, timeWindow,
			   timeWindowSize, regionTrace, regionSize,
			   procTraceMask)
       char *traceFileName;	IN: trace file name
       intn detail;		IN: if non-zero, do detailed trace
       intn lifetime;		IN: if non-zero, do lifetime summaries
       intn timeWindow;		IN: if non-zero, do time window summaries
       intn timeWindowSize;	IN: size of time window
       intn regionTrace;	IN: if non-zero, do file region summaries
       intn regionSize;		IN: size of region
       uint16 procTraceMask;	IN: families of procedures to trace
RETURNS
       None.
--------------------------------------------------------------------- */
PUBLIC VOID HDFinitIOTrace(char *traceFileName, intn detail, intn lifetime,
			   intn timeWindow, float64 timeWindowSize,
			   intn regionTrace, intn regionSize,
			   uint16 procTraceMask )
{
    setTraceFileName(traceFileName);
    preInitProcTrace();
    initProcTrace(sizeof(procEntries)/sizeof(int), procEntries, procExits);

    initIOTrace();

    if (detail)
      enableIOdetail();
    else
      disableIOdetail();

    if (lifetime)
      enableLifetimeSummaries();
    else
      disableLifetimeSummaries();

    if (timeWindow) {
      if ( timeWindowSize <= 0 )
	timeWindowSize = 30.0;
      enableTimeWindowSummaries(timeWindowSize);
    } else
      disableTimeWindowSummaries();


    if (regionTrace) {
      if ( regionSize <= 0 )
	regionSize = 1000;
      enableFileRegionSummaries(regionSize);
    } else 
      disableFileRegionSummaries();

    procTrace = procTraceMask;
}


/* ------------------------------------------------------------------
NAME
       HDFendIOTrace -- end I/O tracing
USAGE
       VOID HDFendIOTrace(VOID)
RETURNS
       None.
--------------------------------------------------------------------- */
PUBLIC VOID HDFendIOTrace(VOID)
{
    endIOTrace();
    endTracing();
}

#endif /* PABLO */
