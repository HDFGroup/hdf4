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
 * Funded by: National Aeronautics and Space Administration under NASA
 * Contracts NAG-1-613 and USRA 5555-22 and by the Advanced Research
 * Projects Agency under ARPA contracts DAVT63-91-C-0029 and
 * DABT63-93-C-0040.
 *
 */

/*--------------------------------------------------------------------------
 * File:  PabloHDFc
 * Purpose: support use of Pablo trace library to analyze HDF performance
 * Contents:
 *  HDFinitTrace  : initialize trace
 *  HDFendTrace   : complete trace
 *-------------------------------------------------------------------------*/
#include <stdio.h>

#ifdef HAVE_PABLO

#include "ProcIDs.h"
/*#include "ProcFamilies.h"*/
#include "hdf.h"

#include "SystemDepend.h"
#include "SDDFparam.h"
#include "TraceParam.h"
#include "Trace.h"
#include "HDFTrace.h"

#define PABLO 1
/* on the ipsc/860 we don't include unistd.h */
#ifndef __NX
#include <unistd.h>
#endif


#ifdef SWAVE_IO
#include "SwaveTrace.h"

static int      produceOwnOutput = 1;   /* Boolean flag for output state */

static int      procExitDim = -1;       /* Indices for metrics registered    */
                                        /* with the sliding window extension */
#define returnRecord(x)    if (produceOwnOutput){return x;}else {return nullRecordFunction();}

#else

#define returnRecord(x)    return x;

#endif

void _hdfTraceEntryDescriptor();
void _hdfTraceExitDescriptor();
void startHDFtraceEvent(int eventID);
void endHDFtraceEvent(int eventID, int setID, char *setName, int IDtype );
/************************************************************************/
/* Number of procedures being traced by this code		  	*/
/************************************************************************/
static int	numberProcedures = 0;
/************************************************************************/
/* Each procedure being traced has associated with it a distinct pair 	*/
/* of entry and exit event IDs.  This code maintains a vector of such  	*/
/* matchings which permits the ready identification of an event ID as  	*/
/* being either an entry event or an exit event and for which procedure.*/
/************************************************************************/
typedef struct procEventMatch {
	int			entryID;  /* procedure entry event ID 	*/
	int			exitID;	  /* procedure exit event ID  	*/
} PROC_EVENTS;

static PROC_EVENTS	*procEvents =	   /* array of event ID pairs 	*/
			(PROC_EVENTS *) 0;

static int		*procIdents =	   /* procedure identifiers    	*/
	 		      (int *) 0;
/************************************************************************/
/* For each procedure being traced this code maintains a stack of	*/
/* procedure entry times.  Each procedure entry event causes the	*/
/* corresponding procedure's stack to be pushed, each procedure exit	*/
/* event causes the corresponding procedure's stack to be popped, and	*/
/* from the difference in time between entry and exit the procedure	*/
/* duration may be calculated in a very straightforward subtraction.  	*/
/* The above procedure entry-exit event ID matching is used to map 	*/
/* events to their corresponding procedures.  In addition, the 		*/
/* cumulative total of these procedure durations is maintained for all 	*/
/* traced subprocedures	of each traced procedure.  That is, when a 	*/
/* traced procedure exits, it increases this duration sum for its most 	*/
/* immediate traced ancestor procedure.  By subtracting this 		*/
/* subprocedure duration sum from the traced procedure's inclusive 	*/
/* duration, we arrive at the exclusive duration of the procedure.	*/
/************************************************************************/
typedef struct procEntryTime {
	CLOCK			entryTime;	/* when proc entered 	*/
	CLOCK			subProcTime;	/* subproc duration    	*/
	struct procEntryTime	*nextTime;	/* stack pointer down	*/
	struct procEntryTime	*ancestorProc;	/* traced ancestor	*/
} PROC_ENTRY;


static PROC_ENTRY	**procEntryStack =	/* array of pointers to	*/
			(PROC_ENTRY **) 0;	/* stack top elements	*/

static PROC_ENTRY	*activeProc =		/* most recent proc    	*/
			(PROC_ENTRY *) 0;	/* entry structure    	*/

/************************************************************************/
/* Define data structure types for procedure entry and exit trace 	*/
/* records, similarly to record data structures in Trace.h	 	*/
/************************************************************************/

/************************************************************************/
/* FAMILY_PROCENTRY family Record Data packets:				*/
/************************************************************************/
struct procEntryTraceRecordData {
	int	packetLength;	/* bytes in packet		    	*/
	int	packetType;	/* == PKT_DATA			    	*/
	int	packetTag;	/* FAMILY_PROCENTRY | RECORD_TRACE  	*/
	int	eventID;	/* ID of corresponding event	    	*/
	double	seconds;	/* floating-point timestamp	    	*/
	int	nodeNumber;	/* occurred on which node	    	*/
	long	sourceByte;	/* source code byte offset in file  	*/
	int	sourceLine;	/* source code line number in file  	*/
};
#define procEntryTraceLen 6*sizeof(int) + sizeof(long) + sizeof(double)
/************************************************************************/
/* FAMILY_PROCEXIT family Record Data packets:				*/
/************************************************************************/
struct procExitTraceRecordData {
	int	packetLength;	   /* bytes in packet		    	*/
	int	packetType;	   /* == PKT_DATA		    	*/
	int	packetTag;	   /* FAMILY_PROCEXIT | RECORD_TRACE   	*/
	int	eventID;	   /* ID of corresponding event	    	*/
	double	seconds;	   /* floating-point timestamp	    	*/
	double	inclusiveSeconds;  /* floating-point incl duration	*/
	double	exclusiveSeconds;  /* floating-point excl duration    	*/
	int	nodeNumber;	   /* occurred on which node	    	*/
	int	setID;	           /* index of file | Data Set accessed	*/
	int	nameLen;	   /* Length of file or data set name	*/
	/* name comes next, but no space is allocated	*/
};
#define procExitTraceLen 7*sizeof(int) + 3*sizeof(double)
/************************************************************************/
/* Define data structures used to contain source code location data for */
/* Pablo instrumenting parser-generated code.				*/
/************************************************************************/
static long	procByteOffset = -1;	/* source code byte offset    	*/

static int	procLineNumber = -1;	/* source code line number 	*/

/* The procEntries and procExits arrays specify the event IDs of
   procedure entry and exit events, respectively.  In order to work
   with the TRACE_ON and TRACE_OFF macros defined in ProcIDs.h, the
   values in procExits must be the negated values of each entry in
   procEntries. */
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
	ID_DFANclear,
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
	ID_DFR8Istart,
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
	ID_HXsetcreatedir,
	ID_HXsetdir,
	ID_HLconvert,
	ID_Hstartaccess,
	ID_Hisappendable,
	ID_Htell,
	ID_Htagnewref,
	ID_Hcache,
	ID_Hsetaccesstype,
	ID_Hexist,
	ID_Hsetlength,
	ID_Hfidinquire,
	ID_HDdont_atexit,
	ID_HDreuse_tagref,
	ID_HIstart,
	ID_HPregister_term_func,
	ID_HIbitstart,
	ID_HMCcreate,
	ID_HMCsetMaxcache,
	ID_HMCPstread,
	ID_HMCPstwrite,
	ID_HMCPseek,
	ID_HMCPchunkread,
	ID_HMCreadChunk,
	ID_HMCPread,
	ID_HMCPchunkwrite,
	ID_HMCwriteChunk,
	ID_HMCPwrite,
	ID_HMCPendaccess,
	ID_HMCPinfo,
	ID_HMCPinquire,
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
        ID_SDgetchunkinfo,
        ID_SDgetdatastrs,
        ID_SDgetdimid,
        ID_SDgetdimscale,
        ID_SDgetdimstrs,
        ID_SDgetfillvalue,
        ID_SDgetinfo,
        ID_SDgetrange,
        ID_SDidtoref,
        ID_SDiscoordvar,
        ID_SDisdimval_bwcomp,
        ID_SDisrecord,
        ID_SDnametoindex,
        ID_SDreadattr,
        ID_SDreadchunk,
        ID_SDreaddata,
        ID_SDreftoindex,
        ID_SDselect,
        ID_SDsetaccesstype,
        ID_SDsetattr,
        ID_SDsetblocksize,
        ID_SDsetcal,
        ID_SDsetchunk,
        ID_SDsetchunkcache,
        ID_SDsetcompress,
        ID_SDsetdatastrs,
        ID_SDsetdimname,
        ID_SDsetdimscale,
        ID_SDsetdimstrs,
        ID_SDsetdimval_comp,
        ID_SDsetexternalfile,
        ID_SDsetfillmode,
        ID_SDsetfillvalue,
        ID_SDsetnbitdataset,
        ID_SDsetrag,
        ID_SDsetrange,
        ID_SDstart,
        ID_SDwritechunk,
        ID_SDwritedata,
	ID_VFfieldesize,
	ID_VFfieldisize,
	ID_VFfieldname,
	ID_VFfieldorder,
	ID_VFfieldtype,
	ID_VFnfields,
	ID_Vaddtagref,
	ID_Vattach,
	ID_Vdeletetagref,
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
	ID_Vinitialize,
	ID_Vfinish,
	ID_Vfind,
	ID_Vfindclass,
	ID_vpackvs,
	ID_vunpackvs,
	ID_vpackvg,
	ID_vunpackvg,
	ID_Vsetattr,
	ID_Vgetversion,
	ID_Vnattrs,
	ID_Vfindattr,
	ID_Vattrinfo,
	ID_Vgetattr,
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
	ID_VSwrite,
	ID_VSappendable,
	ID_VSdelete,
	ID_VSfindclass,
	ID_VSfpack,
	ID_VSfindex,
	ID_VSsetattr,
	ID_VSnattrs,
	ID_VSfnattrs,
	ID_VSfindattr,
	ID_VSattrinfo,
	ID_VSgetattr,
	ID_VSisattr,
	ID_ANstart,
	ID_ANfileinfo,
	ID_ANend,
	ID_ANcreate,
	ID_ANcreatef,
	ID_ANselect,
	ID_ANnumann,
	ID_ANannlist,
	ID_ANannlen,
	ID_ANreadann,
	ID_ANwriteann,
	ID_ANendaccess,
	ID_GRstart,
	ID_GRfileinfo,
	ID_GRend,
	ID_GRcreate,
	ID_GRselect,
	ID_GRnametoindex,
	ID_GRgetiminfo,
	ID_GRwriteimage,
	ID_GRreadimage,
	ID_GRendaccess,
	ID_GRgetdimid,
	ID_GRsetdimname,
	ID_GRdiminfo,
	ID_GRidtoref,
	ID_GRreftoindex,
	ID_GRreqlutil,
	ID_GRreqimageil,
	ID_GRgetlutid,
	ID_GRgetlutinfo,
	ID_GRwritelut,
	ID_GRreadlut,
	ID_GRsetexternalfile,
	ID_GRsetaccesstype,
	ID_GRsetcompress,
	ID_GRsetattr,
	ID_GRattrinfo,
	ID_GRgetattr,
	ID_GRfindattr,
	ID_GRluttoref,
	ID_GRsetchunk,
	ID_GRgetchunkinfo,
	ID_GRsetchunkcache,
	ID_HAinit_group,
	ID_HAdestroy_group,
	ID_HAregister_atom,
	ID_HAatom_object,
	ID_HAatom_group,
	ID_HAremove_atom,
	ID_HAsearch_atom,
	ID_DAcreate_array,
	ID_DAdestroy_array,
	ID_DAsize_array,
	ID_DAget_elem,
	ID_DAset_elem,
	ID_DAdel_elem,
	ID_HULcreate_list ,
	ID_HULdestroy_list,
	ID_HULadd_node,
	ID_HULsearch_node,
	ID_HULfirst_node,
	ID_HULnext_node,
	ID_HULremove_node,
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
	-ID_DFANclear,
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
	-ID_DFR8Istart,
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
	-ID_HXsetcreatedir,
	-ID_HXsetdir,
	-ID_HLconvert,
	-ID_Hstartaccess,
	-ID_Hisappendable,
	-ID_Htell,
	-ID_Htagnewref,
	-ID_Hcache,
	-ID_Hsetaccesstype,
	-ID_Hexist,
	-ID_Hsetlength,
	-ID_Hfidinquire,
	-ID_HDdont_atexit,
	-ID_HDreuse_tagref,
	-ID_HIstart,
	-ID_HPregister_term_func,
	-ID_HIbitstart,
	-ID_HMCcreate,
	-ID_HMCsetMaxcache,
	-ID_HMCPstread,
	-ID_HMCPstwrite,
	-ID_HMCPseek,
	-ID_HMCPchunkread,
	-ID_HMCreadChunk,
	-ID_HMCPread,
	-ID_HMCPchunkwrite,
	-ID_HMCwriteChunk,
	-ID_HMCPwrite,
	-ID_HMCPendaccess,
	-ID_HMCPinfo,
	-ID_HMCPinquire,
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
        -ID_SDgetchunkinfo,
        -ID_SDgetdatastrs,
        -ID_SDgetdimid,
        -ID_SDgetdimscale,
        -ID_SDgetdimstrs,
        -ID_SDgetfillvalue,
        -ID_SDgetinfo,
        -ID_SDgetrange,
        -ID_SDidtoref,
        -ID_SDiscoordvar,
        -ID_SDisdimval_bwcomp,
        -ID_SDisrecord,
        -ID_SDnametoindex,
        -ID_SDreadattr,
        -ID_SDreadchunk,
        -ID_SDreaddata,
        -ID_SDreftoindex,
        -ID_SDselect,
        -ID_SDsetaccesstype,
        -ID_SDsetattr,
        -ID_SDsetblocksize,
        -ID_SDsetcal,
        -ID_SDsetchunk,
        -ID_SDsetchunkcache,
        -ID_SDsetcompress,
        -ID_SDsetdatastrs,
        -ID_SDsetdimname,
        -ID_SDsetdimscale,
        -ID_SDsetdimstrs,
        -ID_SDsetdimval_comp,
        -ID_SDsetexternalfile,
        -ID_SDsetfillmode,
        -ID_SDsetfillvalue,
        -ID_SDsetnbitdataset,
        -ID_SDsetrag,
        -ID_SDsetrange,
        -ID_SDstart,
        -ID_SDwritechunk,
        -ID_SDwritedata,
	-ID_VFfieldesize,
	-ID_VFfieldisize,
	-ID_VFfieldname,
	-ID_VFfieldorder,
	-ID_VFfieldtype,
	-ID_VFnfields,
	-ID_Vaddtagref,
	-ID_Vattach,
	-ID_Vdeletetagref,
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
	-ID_Vinitialize,
	-ID_Vfinish,
	-ID_Vfind,
	-ID_Vfindclass,
	-ID_vpackvs,
	-ID_vunpackvs,
	-ID_vpackvg,
	-ID_vunpackvg,
	-ID_Vsetattr,
	-ID_Vgetversion,
	-ID_Vnattrs,
	-ID_Vfindattr,
	-ID_Vattrinfo,
	-ID_Vgetattr,
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
	-ID_VSwrite,
	-ID_VSappendable,
	-ID_VSdelete,
	-ID_VSfindclass,
	-ID_VSfpack,
	-ID_VSfindex,
	-ID_VSsetattr,
	-ID_VSnattrs,
	-ID_VSfnattrs,
	-ID_VSfindattr,
	-ID_VSattrinfo,
	-ID_VSgetattr,
	-ID_VSisattr,
	-ID_ANstart,
	-ID_ANfileinfo,
	-ID_ANend,
	-ID_ANcreate,
	-ID_ANcreatef,
	-ID_ANselect,
	-ID_ANnumann,
	-ID_ANannlist,
	-ID_ANannlen,
	-ID_ANreadann,
	-ID_ANwriteann,
	-ID_ANendaccess,
	-ID_GRstart,
	-ID_GRfileinfo,
	-ID_GRend,
	-ID_GRcreate,
	-ID_GRselect,
	-ID_GRnametoindex,
	-ID_GRgetiminfo,
	-ID_GRwriteimage,
	-ID_GRreadimage,
	-ID_GRendaccess,
	-ID_GRgetdimid,
	-ID_GRsetdimname,
	-ID_GRdiminfo,
	-ID_GRidtoref,
	-ID_GRreftoindex,
	-ID_GRreqlutil,
	-ID_GRreqimageil,
	-ID_GRgetlutid,
	-ID_GRgetlutinfo,
	-ID_GRwritelut,
	-ID_GRreadlut,
	-ID_GRsetexternalfile,
	-ID_GRsetaccesstype,
	-ID_GRsetcompress,
	-ID_GRsetattr,
	-ID_GRattrinfo,
	-ID_GRgetattr,
	-ID_GRfindattr,
	-ID_GRluttoref,
	-ID_GRsetchunk,
	-ID_GRgetchunkinfo,
	-ID_GRsetchunkcache,
	-ID_HAinit_group,
	-ID_HAdestroy_group,
	-ID_HAregister_atom,
	-ID_HAatom_object,
	-ID_HAatom_group,
	-ID_HAremove_atom,
	-ID_HAsearch_atom,
	-ID_DAcreate_array,
	-ID_DAdestroy_array,
	-ID_DAsize_array,
	-ID_DAget_elem,
	-ID_DAset_elem,
	-ID_DAdel_elem,
	-ID_HULcreate_list ,
	-ID_HULdestroy_list,
	-ID_HULadd_node,
	-ID_HULsearch_node,
	-ID_HULfirst_node,
	-ID_HULnext_node,
	-ID_HULremove_node,
};
int HDFinitTrace_SDDF( char *traceFileName, uint32 procTraceMask )
{
	int ret_value = 0;

	setTraceFileName(traceFileName);

	preInitHDFProcTrace();
	initHDFProcTrace( sizeof(procEntries)/sizeof(int), procEntries, 
			 				   procExits );

    	initIOTrace();

    	enableIOdetail();

    	disableLifetimeSummaries();

        disableTimeWindowSummaries();

	disableFileRegionSummaries();

    	procTrace = procTraceMask;

  	return ret_value;
}

/*======================================================================*
// NAME									*
//     HDFendTrace_SDDF -- end HDF tracing				*
// USAGE								*
//     int HDFendTrace_SDDF(VOID)					*
// RETURNS								*
//     None.								*
//======================================================================*/
int HDFendTrace_SDDF(VOID)
{
    int ret_value = 0;

    endIOTrace();
    endTracing();

    return ret_value;
}
/************************************************************************/
/* SUBROUTINE INITHDFPROCTRACE (C function initProcTrace):		*/
/*	This function initializes data structures specific to		*/
/*	the procedure entry/exit tracing extensions of the Pablo	*/
/*	instrumentation library.  The argument numProcs specifies	*/
/*	how many procedures are to be traced.  The arguments procEntryID*/
/*	and procExitID, respectively, are vectors specifying the event 	*/ 
/*	IDs TO be used as entry and exit events for each of the 	*/
/*	procedures.  No event ID may be used as both an entry and an 	*/
/*	EXIt event.  Further, there must be a one-to-one mapping between*/
/*	entry and exit event IDs.  The order of appearance of the event */
/*	IDs in the two lists dictates which ones are paired as well as 	*/
/*	what procedure index is associated with each pair.		*/
/*									*/
/*	Sample Fortran invocation (using 27 as number of procedures):	*/
/*		INTEGER ENT(27),EXT(27)					*/
/*		DATA ENT/list of 27 procedure entry event IDs/		*/
/*		DATA EXT/list of 27 procedure exit event IDs/		*/
/*		CALL INITPROCTRACE(27,ENT,EXT)				*/
/*	Sample C invocation:						*/
/*		int entries[27] = { list of 27 entry IDs };		*/
/*		int exits[27] = { list of exit IDs };			*/
/*		initProcTrace( 27, entries, exits );			*/
/************************************************************************/
int inithdfproctrace_( int *numProcs, int *procEntryID, int *procExitID )

{
	return initProcTrace( *numProcs, procEntryID, procExitID );
}


int initHDFProcTrace( int numProcs, int *procEntryID, int *procExitID )

{
	extern TR_RECORD	*HDFprocEventRecord();
	int			procIndex;

	if (( numProcs <= 0 ) ||
	    ( procEntryID == (int *) 0 ) ||
	    ( procExitID == (int *) 0 ))
		return FAILURE;

	/* Allocate space to store a copy of the procedure entry-exit	    */
	/* event ID matchings and also the procedure entry stacks.	    */

	procEvents = (PROC_EVENTS *) TRgetBuffer(
					    numProcs * sizeof(PROC_EVENTS) );

	if ( procEvents == (PROC_EVENTS *) 0 )
		TRfailure( "cannot allocate procedure events matching" );

	procIdents = (int *) TRgetBuffer( numProcs * sizeof(int) );

	if ( procIdents == (int *) 0 )
		TRfailure( "cannot allocate procedure identifiers table" );

	procEntryStack = (PROC_ENTRY **) TRgetBuffer(
					    numProcs * sizeof(PROC_ENTRY *) );

	if ( procEntryStack == (PROC_ENTRY **) 0 )
		TRfailure( "cannot allocate procedure entry stack pointers" );

	/* Perform procedure tracing extension pre-initialization, if	    */
	/* not already done.						    */

	preInitHDFProcTrace();

	/* Initialize the procedure events matching from the arguments	    */
	/* passed.  Initialize the procedure entry stack pointers.	    */
	/* Configure the trace record-generating function for these events. */

	for ( procIndex = 0; procIndex < numProcs; procIndex++ ) {

		procEvents[ procIndex ].entryID = procEntryID[ procIndex ];
		procEvents[ procIndex ].exitID = procExitID[ procIndex ];

		procEntryStack[ procIndex ] = (PROC_ENTRY *) 0;

		setEventRecordFunction( procEntryID[ procIndex ],
					HDFprocEventRecord );
		setEventRecordFunction( procExitID[ procIndex ],
					HDFprocEventRecord );

		procIdents[ procIndex ] = procIndex;
	}

	numberProcedures = numProcs;

#ifdef SWAVE_IO
	procExitDim = SwaveRegisterDimension( "Procedure Exclusive Duration" );
#endif

/*#ifdef DEBUG
	fprintf( debugFile, "initHDFProcTrace done\n" );
	fflush( debugFile );
#endif*/ /* DEBUG */

	return SUCCESS;
}


/************************************************************************/
/*   preInitHDFProcTrace:						*/
/*   	This function calls the trace library interface function	*/
/*	setRecordDescriptor, which records the address of the		*/
/*	procedure that generates the record descriptors for the		*/
/*	procedure trace event families.  It is automatically		*/
/*	invoked by initProcTrace.  However, the user may elect to	*/
/*	explicitly call preInitProcTrace before initProcTrace does.	*/
/*	Since initProcTrace calls the setEventRecordFunction interface	*/
/*	function of the trace library, that will cause the trace	*/
/*	library to perform its basic initialization.  One of the	*/
/*	effects of trace library basic initialization is to output	*/
/*	record descriptors for all event families defined up to		*/
/*	that point, by invoking all of the functions passed as		*/
/*	arguments to setRecordDescriptor, and then to output the	*/
/*	predefined, built-in record descriptors for internal event	*/
/*	families.  If no user-defined record descriptors beyond		*/
/*	those needed by this procedure tracing extension are		*/
/*	to be used, then preInitProcTrace need not be invoked.		*/
/*	However, if there are other such record descriptors (such	*/
/*	as an application also using the message tracing extension)	*/
/*	and if the user desires all these record descriptors to be	*/
/*	output before the trace library's internal event family		*/
/*	record descriptors, then all the required setRecordDescriptor	*/
/*	interface calls must be done before the trace library basic	*/
/*	initialization is performed.  preInitProcTrace may be used	*/
/*	for this very purpose.						*/
/************************************************************************/
preinithdfproctrace_()
{
	preInitHDFProcTrace();
	return SUCCESS;
}

preInitHDFProcTrace()
{
	extern int	writeHDFProcRecordDescriptors();
	static int	preInitDone = FALSE;

	if ( preInitDone == TRUE )
		return SUCCESS;

	/* Give the instrumentation library a pointer to the function	    */
	/* in which we output the specialized record descriptors for	    */
	/* procedure entry/exit.					    */

	setRecordDescriptor( writeHDFProcRecordDescriptors );

	preInitDone = TRUE;
	return SUCCESS;
}

/************************************************************************/
/*	writeHDFProcRecordDescriptors:					*/
/*	   This function generates the record descriptors for the HDF	*/
/*	   procedure entry/exit event families.  It will be invoked	*/
/*	   by the instrumentation library initialization routines.	*/
/*	   Patterned after instrumentation library internal function	*/
/*	   writeRecordDescriptors.					*/
/************************************************************************/
writeHDFProcRecordDescriptors()
{
	TR_RECORD	*recordPointer, *HDFprocSDDFdescriptor();

#ifdef DEBUG
	fprintf( debugFile, "writeHDFProcRecordDescriptors\n" );
	fflush( debugFile );
#endif /* DEBUG */

	_hdfTraceEntryDescriptor() ;
	_hdfTraceExitDescriptor()  ;

#ifdef DEBUG
	fprintf( debugFile, "writeHDFProcRecordDescriptors done\n" );
	fflush( debugFile );
#endif /* DEBUG */
	return SUCCESS;
}
/************************************************************************/
/*   HDFprocEventRecord:						*/
/* 	This function generates trace records for events which are	*/
/*   	to produce procedure entry or exit event family trace records.	*/
/*   	Patterned after the instrumentation library internal functions	*/
/*	externalEventRecord and sddfRecord.				*/
/************************************************************************/
TR_RECORD *
HDFprocEventRecord( recordType, eventPointer, timeStamp,
		    dataPointer, dataLength )

int		recordType;
TR_EVENT	*eventPointer;
CLOCK		timeStamp;
HDFsetInfo	*dataPointer;
unsigned int	dataLength;
{
	static TR_RECORD		traceRecord;
	static char			*recordBuffer = (char *) 0;
	static int			bufferLength = 0;
	struct procEntryTraceRecordData	*entryTraceRecordHeader;
	struct procEntryCountRecordData	*entryCountRecordHeader;
	struct procExitTraceRecordData	*exitTraceRecordHeader;
	struct procExitCountRecordData	*exitCountRecordHeader;
	int				procIndex;
	int				recordFamily;
	CLOCK				incTime;
	CLOCK				excTime;
	char				*namePtr;
	
/*
#ifdef ASSERT_NO_BACKSLASH
	Assert(
	       ( recordType == RECORD_TRACE ) ||
	       ( recordType == RECORD_COUNT )
	      );

	Assert(
	       ( eventPointer != (TR_EVENT *) 0 ) &&
	       ( dataLength == 0 )
	      );
#else 
	Assert( \
	       ( recordType == RECORD_TRACE ) || \ 
	       ( recordType == RECORD_COUNT ) \
	      );

	Assert( \
	       ( eventPointer != (TR_EVENT *) 0 ) && \
	       ( dataLength == 0 ) \
	      );
#endif */ /* ASSERT_NO_BACKSLASH */

#ifdef DEBUG
	fprintf( debugFile, "HDFprocEventRecord\n" );
	fflush( debugFile );
#endif /* DEBUG */

	/****************************************************************/
	/* Determine whether this is a procedure entry or a procedure 	*/
	/* exit family event by lookup in the procedure event ID 	*/
	/* matching.  							*/
	/****************************************************************/
	procIndex = findHDFProcEvent( eventPointer->eventID );

	if ( procIndex < 0 ) {
	   return nullRecordFunction( recordType, eventPointer,
				timeStamp, dataPointer, dataLength );
	}
	if ( procEvents[ procIndex ].entryID == eventPointer->eventID ) {
	   recordFamily = FAMILY_PROCENTRY;
	} else {
	   recordFamily = FAMILY_PROCEXIT;
	}
	/****************************************************************/
	/* The time stamp stored in the event descriptor will be used	*/
	/* unless one is specified in the timeStamp parameter.	    	*/
	/****************************************************************/
	if ( clockCompare( timeStamp, noSuchClock ) == 0 ) {
	   timeStamp = eventPointer->eventLast;
	}
	/****************************************************************/
	/* Determine how many bytes of storage will be needed for the 	*/
	/* contents of the trace record.			    	*/
	/****************************************************************/
	switch (( recordFamily | recordType )) {

	   case FAMILY_PROCENTRY | RECORD_TRACE:
		traceRecord.recordLength = sizeof *entryTraceRecordHeader;
		break;

	   case FAMILY_PROCEXIT | RECORD_TRACE:
		traceRecord.recordLength = sizeof *exitTraceRecordHeader;
		break;
	}
        if ( dataPointer != NULL && dataPointer->setName != NULL ) {
	   traceRecord.recordLength += strlen( dataPointer->setName );
        }
	/****************************************************************/
	/* If there is a previously-allocated buffer and its size will	*/
	/* hold this record, re-use the buffer.  Otherwise, deallocate	*/
	/* the buffer (if allocated) and allocate a bigger one.	    	*/
	/****************************************************************/
	if ( bufferLength < traceRecord.recordLength ) {

	   if ( recordBuffer != (char *) 0 ) {
	      TRfreeBuffer( recordBuffer );
	   }

	   recordBuffer = TRgetBuffer( traceRecord.recordLength );

	   if ( recordBuffer == (char *) 0 ) {
	      TRfailure( "cannot allocate storage for trace record" );
	   }
	   bufferLength = traceRecord.recordLength;
	}

	traceRecord.recordContents = recordBuffer;
	/****************************************************************/
	/* Load the trace record fields into the allocated buffer 	*/
	/****************************************************************/
	switch (( recordFamily | recordType )) {

	   case FAMILY_PROCENTRY | RECORD_TRACE:
		entryTraceRecordHeader = (struct procEntryTraceRecordData *)
							recordBuffer;
		entryTraceRecordHeader->packetLength =
				traceRecord.recordLength;
		entryTraceRecordHeader->packetType = PKT_DATA;
		entryTraceRecordHeader->packetTag = recordFamily | recordType;
		entryTraceRecordHeader->seconds = clockToSeconds( timeStamp );
		entryTraceRecordHeader->eventID = eventPointer->eventID;
		entryTraceRecordHeader->nodeNumber = TRgetNode();
		entryTraceRecordHeader->sourceByte = procByteOffset;
		entryTraceRecordHeader->sourceLine = procLineNumber;
		pushHDFProcEntry( procIndex, timeStamp );
		break;

	   case FAMILY_PROCEXIT | RECORD_TRACE:
		exitTraceRecordHeader = (struct procExitTraceRecordData *)
							recordBuffer;
		exitTraceRecordHeader->packetLength =
				                  traceRecord.recordLength;
		exitTraceRecordHeader->packetType = PKT_DATA;
		exitTraceRecordHeader->packetTag = recordFamily | recordType;
		exitTraceRecordHeader->seconds = clockToSeconds( timeStamp );
		exitTraceRecordHeader->eventID = eventPointer->eventID;
		exitTraceRecordHeader->nodeNumber = TRgetNode();
		/********************************************************/
		/* The popHDFProcEntry function fills in the 		*/
	        /* inclusiveDuration and the exclusiveDuration fields 	*/
		/* of the record.	    				*/
		/********************************************************/
		popHDFProcEntry( procIndex, timeStamp, &incTime, &excTime );

		exitTraceRecordHeader->inclusiveSeconds =
		    clockToSeconds( incTime) ;
		exitTraceRecordHeader->exclusiveSeconds =
		    clockToSeconds( excTime );

		if ( dataPointer != 0 ) {
	           exitTraceRecordHeader->setID = dataPointer->setID;
		   if (dataPointer->setName != NULL ) {
		      exitTraceRecordHeader->nameLen 
					= strlen( dataPointer->setName );
		      /**************************************************/
		      /* copy name directly into the end of the buffer.	*/
		      /**************************************************/
		      namePtr = (char *)exitTraceRecordHeader + procExitTraceLen;
	              memcpy( namePtr, dataPointer->setName,
		              exitTraceRecordHeader->nameLen );
	           } else {
		      exitTraceRecordHeader->nameLen = 0;
	           }   
		} else {
	           exitTraceRecordHeader->setID = NoDSid;
		   exitTraceRecordHeader->nameLen = 0;
		} 
		break;
	}

#ifdef SWAVE_IO
        {
                int mask = recordFamily | recordType;

                if( mask == (FAMILY_PROCEXIT | RECORD_TRACE) ) {
                        SwaveRegisterValue( procExitDim,
                                exitTraceRecordHeader->seconds,
                                exitTraceRecordHeader->exclusiveSeconds );
                }
                else if( mask == (FAMILY_PROCEXIT | RECORD_COUNT) ) {
                        SwaveRegisterValue( procExitDim,
                                exitCountRecordHeader->seconds,
                                exitCountRecordHeader->exclusiveSeconds );
                }
        }

#endif

#ifdef DEBUG
	fprintf( debugFile, "HDFprocEventRecord done\n" );
	fflush( debugFile );
#endif /* DEBUG */
	returnRecord(&traceRecord);
}

/************************************************************************/
/*	findHDFProcEvent:						*/
/*	   Search the procedure entry/exit event ID matching data	*/
/*	   structure for an event ID (either entry or exit) which is	*/
/*	   the same as the argument eventID.  If found, return the	*/
/*	   index from that table, which will be between 0 and		*/
/*	   numberProcedures - 1, inclusive.  If not found, return -1;	*/
/************************************************************************/
findHDFProcEvent( int eventID )
{
	int	procIndex;

#ifdef DEBUG
	fprintf( debugFile, "findHDFProcEvent\n" );
	fflush( debugFile );
#endif /* DEBUG */

	for ( procIndex = 0; procIndex < numberProcedures; procIndex++ ) {
		if ( procEvents[ procIndex ].entryID == eventID ||
		     procEvents[ procIndex ].exitID == eventID )
			return procIndex;
	}

	return -1;
}

/************************************************************************/
/*	pushHDFProcEntry:						*/
/*	   Push a procedure entry time onto the corresponding stacks.	*/
/************************************************************************/
pushHDFProcEntry( procIndex, entryTime )

int	procIndex;
CLOCK	entryTime;
{
	PROC_ENTRY	*timeElement;

#ifdef DEBUG
	fprintf( debugFile, "pushHDFProcEntry\n" );
	fflush( debugFile );
#endif /* DEBUG */
/*	Assert(( procIndex >= 0 ) && ( procIndex < numberProcedures )); */

	/* Allocate a new procedure entry element and push it onto the	    */
	/* corresponding procedure's stack and the activeProc stack.	    */

	timeElement = (PROC_ENTRY *) TRgetBuffer( sizeof(PROC_ENTRY) );
	if ( timeElement == (PROC_ENTRY *) 0 )
		TRfailure( "unable to allocate procedure entry element" );

	timeElement->entryTime = entryTime;
	timeElement->subProcTime = zeroClock;
	timeElement->nextTime = procEntryStack[ procIndex ];
	procEntryStack[ procIndex ] = timeElement;
	timeElement->ancestorProc = activeProc;
	activeProc = timeElement;

#ifdef DEBUG
	fprintf( debugFile, "pushHDFProcEntry done\n" );
	fflush( debugFile );
#endif /* DEBUG */
	return SUCCESS;
}


/************************************************************************/
/*	popHDFProcEntry:						*/
/*	   Pop a procedure entry time off the corresponding stacks,	*/
/*	   compute and store the procedure's inclusive and exclusive	*/
/*	   durations.							*/
/************************************************************************/
popHDFProcEntry( procIndex, exitTime, inclusiveDuration, exclusiveDuration )

int	procIndex;
CLOCK	exitTime;
CLOCK	*inclusiveDuration;
CLOCK	*exclusiveDuration;
{
	PROC_ENTRY	*timeElement;
	CLOCK		duration;

/*	Assert(( procIndex >= 0 ) && ( procIndex < numberProcedures )); */

/*
#ifdef ASSERT_NO_BACKSLASH
	Assert(
	       ( inclusiveDuration != (CLOCK *) 0 ) &&
	       ( exclusiveDuration != (CLOCK *) 0 )
	      );
#else 
	Assert( \
	       ( inclusiveDuration != (CLOCK *) 0 ) && \
	       ( exclusiveDuration != (CLOCK *) 0 ) \
	      );
#endif */ /* ASSERT_NO_BACKSLASH */

#ifdef DEBUG
	fprintf( debugFile, "popHDFProcEntry\n" );
	fflush( debugFile );
#endif /* DEBUG */
	timeElement = procEntryStack[ procIndex ];

	/* If corresponding procedure's stack is empty, this is an error.   */

	if ( timeElement == (PROC_ENTRY *) 0 ) {
		*inclusiveDuration = noSuchClock;
		*exclusiveDuration = noSuchClock;
		return FAILURE;
	}

	/* If this procedure is not the active procedure, the user has	    */
	/* made an error, probably by failing to issue the procedure	    */
	/* exit event for one or more subprocedures.  Unfortunately,	    */
	/* this user error is more difficult to recover from.  Simply	    */
	/* deleting the indicated procedure entry structure would lead	    */
	/* to dangling pointers.  Deleting all subprocedure entry event	    */
	/* structures is reasonable, but could lead to unexpected	    */
	/* behavior, especially if the user's only error was to get	    */
	/* confused about which event IDs correspond to which procedure	    */
	/* entries and exits.  Better to inform the user he has made a	    */
	/* big mistake which must be corrected.				    */

	if ( timeElement != activeProc )
		TRfailure( "Improperly nested procedure entry/exit events" );

	/* The active procedure now becomes the ancestor of the one being   */
	/* popped.  This effectively pops the current procedure off of	    */
	/* the activeProc stack.  Now pop the top entry time off of the	    */
	/* corresponding procedure's stack.				    */

	activeProc = timeElement->ancestorProc;

	procEntryStack[ procIndex ] = timeElement->nextTime;

	/* Compute the difference between this procedure's entry and	    */
	/* exit times; this is its inclusive duration.			    */

        if ( exitTime.clkLow < (timeElement->entryTime).clkLow &&
             exitTime.clkHigh <= (timeElement->entryTime).clkHigh ) {
	     exitTime = timeElement->entryTime;
        }
	duration = clockSubtract( exitTime, timeElement->entryTime );
	*inclusiveDuration = duration;

	/* This procedure's exclusive duration is less than its		    */
	/* inclusive duration by an amount equal to the sum of its	    */
	/* subprocedure's inclusive durations.				    */

	*exclusiveDuration = clockSubtract( duration,
					    timeElement->subProcTime );

	/* Likewise, add this procedure's inclusive duration to its	    */
	/* ancestor's sum of ITS subprocedure's inclusive durations.	    */

	if ( activeProc != (PROC_ENTRY *) 0 )
		activeProc->subProcTime = clockAdd( activeProc->subProcTime,
						    duration );

	/* Finally, deallocate this structure and return.		    */

	TRfreeBuffer( (char *) timeElement );

#ifdef DEBUG
	fprintf( debugFile, "popHDFProcEntry done\n" );
	fflush( debugFile );
#endif /* DEBUG */

	return SUCCESS;
}
/******************************************************************************/

/*-     Portable routines for building record descriptors                    -*/
/*-     -------------------------------------------------                    -*/
/*-                                                                          -*/

/*-     Utility Routine:   _hdfDescriptor()                                   -*/
/*-                        Generates a SDDF binary format record             -*/
/*-                        descriptor for the portable I/O trace event       -*/
/*-                        specified in the calling parameters.              -*/
/*-                                                                          -*/
static void _hdfDescriptor( recordName, recordDescription, recordFamily, 
							  numberFields )
char	*recordName;
char	*recordDescription;
int	recordFamily;
int 	numberFields;
{

    static char	recordBuffer[ 4096 ];
    int		recordLength;
    char	*eventIdAttrPtr;
    char	*hdfRecordPointer;
    int		eventIdAttrCnt;
    char        cbuf[128]; 

    hdfRecordPointer = recordBuffer;
    /********************************************************************/
    /* Allow space at the beginning of the record for the packet 	*/
    /*length which will be computed after the packet is complete.	*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 0 );
    /********************************************************************/
    /* The record type, tag, and name 					*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, PKT_DESCRIPTOR );
    sddfWriteInteger( &hdfRecordPointer, ( recordFamily | RECORD_TRACE ) );
    sddfWriteString( &hdfRecordPointer, recordName );
    /********************************************************************/
    /* The record attribute count and string pair 			*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 1 );
    sddfWriteString( &hdfRecordPointer, "description" );
    sddfWriteString( &hdfRecordPointer, recordDescription );
    /********************************************************************/
    /* The record field count 						*/
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, numberFields );
    /********************************************************************/
    /* The "Event Identifier" field appears in every record, but the	*/
    /* values it can assume will vary depending on the particular 	*/
    /* family and perhaps the system where we are running.  Here we use	*/
    /* '1' as the attribute count which we'll update later if necessary.*/
    /*  Then we write out the first attribute pair which is always the 	*/
    /*  same.								*/
    /********************************************************************/
    sddfWriteString( &hdfRecordPointer, "Event Identifier" ); 
    eventIdAttrPtr = hdfRecordPointer;	/* save this addr for later use */
    eventIdAttrCnt = 1;
    sddfWriteInteger( &hdfRecordPointer, eventIdAttrCnt );
    WRITE_HDF_ATTR( "Event ID", "Corresponding event" );
    /********************************************************************/
    /* Based on the Record Family, complete the attributes for the 	*/
    /* Event Identifier field and finish the generating the record 	*/
    /* descriptor							*/
    /********************************************************************/
    switch ( recordFamily ) {

      case FAMILY_DFAN:
      case FAMILY_DFAN | SHORT_TRACE:
        eventIdAttrCnt += 15;
	sprintf( cbuf, "%d", ID_DFANaddfds );
	WRITE_HDF_ATTR( cbuf, "DFANaddfds");
	sprintf( cbuf, "%d", ID_DFANaddfid );
	WRITE_HDF_ATTR( cbuf, "DFANaddfid");
	sprintf( cbuf, "%d", ID_DFANgetdesc );
	WRITE_HDF_ATTR( cbuf, "DFANgetdesc");
	sprintf( cbuf, "%d", ID_DFANgetdesclen );
	WRITE_HDF_ATTR( cbuf, "DFANgetdesclen");
	sprintf( cbuf, "%d", ID_DFANgetfds );
	WRITE_HDF_ATTR( cbuf, "DFANaddfds");
	sprintf( cbuf, "%d", ID_DFANgetfdslen );
	WRITE_HDF_ATTR( cbuf, "DFANgetfdslen");
	sprintf( cbuf, "%d", ID_DFANgetfid );
	WRITE_HDF_ATTR( cbuf, "DFANgetfid");
	sprintf( cbuf, "%d", ID_DFANgetfidlen );
	WRITE_HDF_ATTR( cbuf, "DFANgetfidlen");
	sprintf( cbuf, "%d", ID_DFANgetlabel );
	WRITE_HDF_ATTR( cbuf, "DFANgetlabel");
	sprintf( cbuf, "%d", ID_DFANgetlablen );
	WRITE_HDF_ATTR( cbuf, "DFANgetlablen");
	sprintf( cbuf, "%d", ID_DFANlablist );
	WRITE_HDF_ATTR( cbuf, "DFANlablist");
	sprintf( cbuf, "%d", ID_DFANlastref );
	WRITE_HDF_ATTR( cbuf, "DFANlastref");
	sprintf( cbuf, "%d", ID_DFANputdesc );
	WRITE_HDF_ATTR( cbuf, "DFANputdesc");
	sprintf( cbuf, "%d", ID_DFANputlabel );
	WRITE_HDF_ATTR( cbuf, "DFANputlabel");
	sprintf( cbuf, "%d", ID_DFANclear );
	WRITE_HDF_ATTR( cbuf, "DFANclear");

        break; 

      case FAMILY_DFP:
      case FAMILY_DFP | SHORT_TRACE:
        eventIdAttrCnt += 8;
	sprintf( cbuf, "%d", ID_DFPaddpal );
	WRITE_HDF_ATTR( cbuf, "DFPaddpal");
	sprintf( cbuf, "%d", ID_DFPgetpal );
	WRITE_HDF_ATTR( cbuf, "DFPgetpal");
	sprintf( cbuf, "%d", ID_DFPlastref );
	WRITE_HDF_ATTR( cbuf, "DFPlastref");
	sprintf( cbuf, "%d", ID_DFPnpals );
	WRITE_HDF_ATTR( cbuf, "DFPnpals");
	sprintf( cbuf, "%d", ID_DFPputpal );
	WRITE_HDF_ATTR( cbuf, "DFPputpal");
	sprintf( cbuf, "%d", ID_DFPreadref );
	WRITE_HDF_ATTR( cbuf, "DFPreadref");
	sprintf( cbuf, "%d", ID_DFPrestart );
	WRITE_HDF_ATTR( cbuf, "DFPrestart");
	sprintf( cbuf, "%d", ID_DFPwriteref );
	WRITE_HDF_ATTR( cbuf, "DFPwriteref");

        break; 

      case FAMILY_DFR8:
      case FAMILY_DFR8 | SHORT_TRACE:
        eventIdAttrCnt += 12;
	sprintf( cbuf, "%d", ID_DFR8addimage );
	WRITE_HDF_ATTR( cbuf, "DFR8addimage");
	sprintf( cbuf, "%d", ID_DFR8getdims );
	WRITE_HDF_ATTR( cbuf, "DFR8getdims");
	sprintf( cbuf, "%d", ID_DFR8getimage );
	WRITE_HDF_ATTR( cbuf, "DFR8getimage");
	sprintf( cbuf, "%d", ID_DFR8lastref );
	WRITE_HDF_ATTR( cbuf, "DFR8lastref");
	sprintf( cbuf, "%d", ID_DFR8nimages );
	WRITE_HDF_ATTR( cbuf, "DFR8nimages");
	sprintf( cbuf, "%d", ID_DFR8putimage );
	WRITE_HDF_ATTR( cbuf, "DFR8putimage");
	sprintf( cbuf, "%d", ID_DFR8readref );
	WRITE_HDF_ATTR( cbuf, "DFR8readref");
	sprintf( cbuf, "%d", ID_DFR8restart );
	WRITE_HDF_ATTR( cbuf, "DFR8restart");
	sprintf( cbuf, "%d", ID_DFR8setcompress );
	WRITE_HDF_ATTR( cbuf, "DFR8setcompress");
	sprintf( cbuf, "%d", ID_DFR8setpalette );
	WRITE_HDF_ATTR( cbuf, "DFR8setpalette");
	sprintf( cbuf, "%d", ID_DFR8writeref );
	WRITE_HDF_ATTR( cbuf, "DFR8writeref");
	sprintf( cbuf, "%d", ID_DFR8Istart );
	WRITE_HDF_ATTR( cbuf, "DFR8Istart");

        break; 

      case FAMILY_DFSD:
      case FAMILY_DFSD | SHORT_TRACE:
        eventIdAttrCnt += 37;
	sprintf( cbuf, "%d", ID_DFSDadddata );
	WRITE_HDF_ATTR( cbuf, "DFSDadddata");
	sprintf( cbuf, "%d", ID_DFSDclear );
	WRITE_HDF_ATTR( cbuf, "DFSDclear");
	sprintf( cbuf, "%d", ID_DFSDendslab );
	WRITE_HDF_ATTR( cbuf, "DFSDendslab");
	sprintf( cbuf, "%d", ID_DFSDendslice );
	WRITE_HDF_ATTR( cbuf, "DFSDendslice");
	sprintf( cbuf, "%d", ID_DFSDgetcal );
	WRITE_HDF_ATTR( cbuf, "DFSDgetcal");
	sprintf( cbuf, "%d", ID_DFSDgetdata );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdata");
	sprintf( cbuf, "%d", ID_DFSDgetdatalen );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdatalen");
	sprintf( cbuf, "%d", ID_DFSDgetdatastrs );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdatastrs");
	sprintf( cbuf, "%d", ID_DFSDgetdimlen );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdimlen");
	sprintf( cbuf, "%d", ID_DFSDgetdims );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdims");
	sprintf( cbuf, "%d", ID_DFSDgetdimscale );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdimscale");
	sprintf( cbuf, "%d", ID_DFSDgetdimstrs );
	WRITE_HDF_ATTR( cbuf, "DFSDgetdimstrs");
	sprintf( cbuf, "%d", ID_DFSDgetfillvalue );
	WRITE_HDF_ATTR( cbuf, "DFSDgetfillvalue");
	sprintf( cbuf, "%d", ID_DFSDgetNT );
	WRITE_HDF_ATTR( cbuf, "DFSDgetNT");
	sprintf( cbuf, "%d", ID_DFSDgetrange );
	WRITE_HDF_ATTR( cbuf, "DFSDgetrange");
	sprintf( cbuf, "%d", ID_DFSDgetslice );
	WRITE_HDF_ATTR( cbuf, "DFSDgetslice");
	sprintf( cbuf, "%d", ID_DFSDlastref );
	WRITE_HDF_ATTR( cbuf, "DFSDlastref");
	sprintf( cbuf, "%d", ID_DFSDndatasets );
	WRITE_HDF_ATTR( cbuf, "DFSDndatasets");
	sprintf( cbuf, "%d", ID_DFSDpre32sdg );
	WRITE_HDF_ATTR( cbuf, "DFSDpre32sdg");
	sprintf( cbuf, "%d", ID_DFSDputdata );
	WRITE_HDF_ATTR( cbuf, "DFSDputdata");
	sprintf( cbuf, "%d", ID_DFSDputslice );
	WRITE_HDF_ATTR( cbuf, "DFSDputslice");
	sprintf( cbuf, "%d", ID_DFSDreadref );
	WRITE_HDF_ATTR( cbuf, "DFSDreadref");
	sprintf( cbuf, "%d", ID_DFSDreadslab );
	WRITE_HDF_ATTR( cbuf, "DFSDreadslab");
	sprintf( cbuf, "%d", ID_DFSDrestart );
	WRITE_HDF_ATTR( cbuf, "DFSDrestart");
	sprintf( cbuf, "%d", ID_DFSDsetcal );
	WRITE_HDF_ATTR( cbuf, "DFSDsetcal");
	sprintf( cbuf, "%d", ID_DFSDsetdatastrs );
	WRITE_HDF_ATTR( cbuf, "DFSDsetdatastrs");
	sprintf( cbuf, "%d", ID_DFSDsetdims );
	WRITE_HDF_ATTR( cbuf, "DFSDsetdims");
	sprintf( cbuf, "%d", ID_DFSDsetdimscale );
	WRITE_HDF_ATTR( cbuf, "DFSDsetdimscale");
	sprintf( cbuf, "%d", ID_DFSDsetdimstrs );
	WRITE_HDF_ATTR( cbuf, "DFSDsetdimstrs");
	sprintf( cbuf, "%d", ID_DFSDsetfillvalue );
	WRITE_HDF_ATTR( cbuf, "DFSDsetfillvalue");
	sprintf( cbuf, "%d", ID_DFSDsetlengths );
	WRITE_HDF_ATTR( cbuf, "DFSDsetlengths");
	sprintf( cbuf, "%d", ID_DFSDsetNT );
	WRITE_HDF_ATTR( cbuf, "DFSDsetNT");
	sprintf( cbuf, "%d", ID_DFSDsetrange );
	WRITE_HDF_ATTR( cbuf, "DFSDsetrange");
	sprintf( cbuf, "%d", ID_DFSDstartslab );
	WRITE_HDF_ATTR( cbuf, "DFSDstartslab");
	sprintf( cbuf, "%d", ID_DFSDstartslice );
	WRITE_HDF_ATTR( cbuf, "DFSDstartslice");
	sprintf( cbuf, "%d", ID_DFSDwriteref );
	WRITE_HDF_ATTR( cbuf, "DFSDwriteref");
	sprintf( cbuf, "%d", ID_DFSDwriteslab );
	WRITE_HDF_ATTR( cbuf, "DFSDwriteslab");

        break; 

      case FAMILY_DF24:
      case FAMILY_DF24 | SHORT_TRACE:
        eventIdAttrCnt += 12;
	sprintf( cbuf, "%d", ID_DF24addimage );
	WRITE_HDF_ATTR( cbuf, "DF24addimage");
	sprintf( cbuf, "%d", ID_DF24getdims );
	WRITE_HDF_ATTR( cbuf, "DF24getdims");
	sprintf( cbuf, "%d", ID_DF24getimage );
	WRITE_HDF_ATTR( cbuf, "DF24getimage");
	sprintf( cbuf, "%d", ID_DF24lastref );
	WRITE_HDF_ATTR( cbuf, "DF24lastref");
	sprintf( cbuf, "%d", ID_DF24nimages );
	WRITE_HDF_ATTR( cbuf, "DF24nimages");
	sprintf( cbuf, "%d", ID_DF24putimage );
	WRITE_HDF_ATTR( cbuf, "DF24putimage");
	sprintf( cbuf, "%d", ID_DF24readref );
	WRITE_HDF_ATTR( cbuf, "DF24readref");
	sprintf( cbuf, "%d", ID_DF24reqil );
	WRITE_HDF_ATTR( cbuf, "DF24reqil");
	sprintf( cbuf, "%d", ID_DF24restart );
	WRITE_HDF_ATTR( cbuf, "DF24restart");
	sprintf( cbuf, "%d", ID_DF24setcompress );
	WRITE_HDF_ATTR( cbuf, "DF24setcompress");
	sprintf( cbuf, "%d", ID_DF24setdims );
	WRITE_HDF_ATTR( cbuf, "DF24setdims");
	sprintf( cbuf, "%d", ID_DF24setil );
	WRITE_HDF_ATTR( cbuf, "DF24setil");

        break; 

      case FAMILY_H:
      case FAMILY_H | SHORT_TRACE:
        eventIdAttrCnt += 57;
	sprintf( cbuf, "%d", ID_Happendable );
	WRITE_HDF_ATTR( cbuf, "Happendable");
	sprintf( cbuf, "%d", ID_Hclose );
	WRITE_HDF_ATTR( cbuf, "Hclose");
	sprintf( cbuf, "%d", ID_Hdeldd );
	WRITE_HDF_ATTR( cbuf, "Hdeldd");
	sprintf( cbuf, "%d", ID_Hdupdd );
	WRITE_HDF_ATTR( cbuf, "Hdupdd");
	sprintf( cbuf, "%d", ID_Hendaccess );
	WRITE_HDF_ATTR( cbuf, "Hendaccess");
	sprintf( cbuf, "%d", ID_Hfind );
	WRITE_HDF_ATTR( cbuf, "Hfind");
	sprintf( cbuf, "%d", ID_Hgetelement );
	WRITE_HDF_ATTR( cbuf, "Hgetelement");
	sprintf( cbuf, "%d", ID_Hgetfileversion );
	WRITE_HDF_ATTR( cbuf, "Hgetfileversion");
	sprintf( cbuf, "%d", ID_Hgetlibversion );
	WRITE_HDF_ATTR( cbuf, "Hgetlibversion");
	sprintf( cbuf, "%d", ID_Hinquire );
	WRITE_HDF_ATTR( cbuf, "Hinquire");
	sprintf( cbuf, "%d", ID_Hishdf );
	WRITE_HDF_ATTR( cbuf, "Hishdf");
	sprintf( cbuf, "%d", ID_HLcreate );
	WRITE_HDF_ATTR( cbuf, "HLcreate");
	sprintf( cbuf, "%d", ID_Hlength );
	WRITE_HDF_ATTR( cbuf, "Hlength");
	sprintf( cbuf, "%d", ID_Hnewref );
	WRITE_HDF_ATTR( cbuf, "Hnewref");
	sprintf( cbuf, "%d", ID_Hnextread );
	WRITE_HDF_ATTR( cbuf, "Hnextread");
	sprintf( cbuf, "%d", ID_Hnumber );
	WRITE_HDF_ATTR( cbuf, "Hnumber");
	sprintf( cbuf, "%d", ID_Hoffset );
	WRITE_HDF_ATTR( cbuf, "Hoffset");
	sprintf( cbuf, "%d", ID_Hopen );
	WRITE_HDF_ATTR( cbuf, "Hopen");
	sprintf( cbuf, "%d", ID_Hputelement );
	WRITE_HDF_ATTR( cbuf, "Hputelement");
	sprintf( cbuf, "%d", ID_Hread );
	WRITE_HDF_ATTR( cbuf, "Hread");
	sprintf( cbuf, "%d", ID_Hseek );
	WRITE_HDF_ATTR( cbuf, "Hseek");
	sprintf( cbuf, "%d", ID_Hstartread );
	WRITE_HDF_ATTR( cbuf, "Hstartread");
	sprintf( cbuf, "%d", ID_Hstartwrite );
	WRITE_HDF_ATTR( cbuf, "Hstartwrite");
	sprintf( cbuf, "%d", ID_Hsync );
	WRITE_HDF_ATTR( cbuf, "Hsync");
	sprintf( cbuf, "%d", ID_Htrunc );
	WRITE_HDF_ATTR( cbuf, "Htrunc");
	sprintf( cbuf, "%d", ID_Hwrite );
	WRITE_HDF_ATTR( cbuf, "Hwrite");
	sprintf( cbuf, "%d", ID_HXcreate );
	WRITE_HDF_ATTR( cbuf, "HXcreate");
	sprintf( cbuf, "%d", ID_HXsetcreatedir );
	WRITE_HDF_ATTR( cbuf, "HXsetcreatedir");
	sprintf( cbuf, "%d", ID_HXsetdir );
	WRITE_HDF_ATTR( cbuf, "HXsetdir");
	sprintf( cbuf, "%d", ID_HLconvert );
	WRITE_HDF_ATTR( cbuf, "HLconvert");
	sprintf( cbuf, "%d", ID_Hstartaccess );
	WRITE_HDF_ATTR( cbuf, "Hstartaccess");
	sprintf( cbuf, "%d", ID_Hisappendable );
	WRITE_HDF_ATTR( cbuf, "Hisappendable");
	sprintf( cbuf, "%d", ID_Htell );
	WRITE_HDF_ATTR( cbuf, "Htell");
	sprintf( cbuf, "%d", ID_Htagnewref );
	WRITE_HDF_ATTR( cbuf, "Htagnewref");
	sprintf( cbuf, "%d", ID_Hcache );
	WRITE_HDF_ATTR( cbuf, "Hcache");
	sprintf( cbuf, "%d", ID_Hsetaccesstype );
	WRITE_HDF_ATTR( cbuf, "Hsetaccesstype");
	sprintf( cbuf, "%d", ID_Hexist );
	WRITE_HDF_ATTR( cbuf, "Hexist");
	sprintf( cbuf, "%d", ID_Hsetlength );
	WRITE_HDF_ATTR( cbuf, "Hsetlength");
	sprintf( cbuf, "%d", ID_Hfidinquire );
	WRITE_HDF_ATTR( cbuf, "Hfidinquire");
	sprintf( cbuf, "%d", ID_HDdont_atexit );
	WRITE_HDF_ATTR( cbuf, "HDdont_atexit");
	sprintf( cbuf, "%d", ID_HIstart );
	WRITE_HDF_ATTR( cbuf, "HIstart");
	sprintf( cbuf, "%d", ID_HPregister_term_func );
	WRITE_HDF_ATTR( cbuf, "HPregister_term_func");
	sprintf( cbuf, "%d", ID_HIbitstart );
	WRITE_HDF_ATTR( cbuf, "HIbitstart");
	sprintf( cbuf, "%d", ID_HMCcreate );
	WRITE_HDF_ATTR( cbuf, "HMCcreate");
	sprintf( cbuf, "%d", ID_HMCsetMaxcache );
	WRITE_HDF_ATTR( cbuf, "HMCsetMaxcache");
	sprintf( cbuf, "%d", ID_HMCPstread );
	WRITE_HDF_ATTR( cbuf, "HMCPstread");
	sprintf( cbuf, "%d", ID_HMCPstwrite );
	WRITE_HDF_ATTR( cbuf, "HMCPstwrite");
	sprintf( cbuf, "%d", ID_HMCPseek );
	WRITE_HDF_ATTR( cbuf, "HMCPseek");
	sprintf( cbuf, "%d", ID_HMCPchunkread );
	WRITE_HDF_ATTR( cbuf, "HMCPchunkread");
	sprintf( cbuf, "%d", ID_HMCreadChunk );
	WRITE_HDF_ATTR( cbuf, "HMCreadChunk");
	sprintf( cbuf, "%d", ID_HMCPread );
	WRITE_HDF_ATTR( cbuf, "HMCPread");
	sprintf( cbuf, "%d", ID_HMCPchunkwrite );
	WRITE_HDF_ATTR( cbuf, "HMCPchunkwrite");
	sprintf( cbuf, "%d", ID_HMCwriteChunk );
	WRITE_HDF_ATTR( cbuf, "HMCwriteChunk");
	sprintf( cbuf, "%d", ID_HMCPwrite );
	WRITE_HDF_ATTR( cbuf, "HMCPwrite");
	sprintf( cbuf, "%d", ID_HMCPendaccess );
	WRITE_HDF_ATTR( cbuf, "HMCPendaccess");
	sprintf( cbuf, "%d", ID_HMCPinfo );
	WRITE_HDF_ATTR( cbuf, "HMCPinfo");
	sprintf( cbuf, "%d", ID_HMCPinquire );
	WRITE_HDF_ATTR( cbuf, "HMCPinquire");

        break; 

      case FAMILY_HE:
      case FAMILY_HE | SHORT_TRACE:
        eventIdAttrCnt += 5;
	sprintf( cbuf, "%d", ID_HEclear );
	WRITE_HDF_ATTR( cbuf, "HEclear");
	sprintf( cbuf, "%d", ID_HEprint );
	WRITE_HDF_ATTR( cbuf, "HEprint");
	sprintf( cbuf, "%d", ID_HEpush );
	WRITE_HDF_ATTR( cbuf, "HEpush");
	sprintf( cbuf, "%d", ID_HEreport );
	WRITE_HDF_ATTR( cbuf, "HEreport");
	sprintf( cbuf, "%d", ID_HEstring );
	WRITE_HDF_ATTR( cbuf, "HEstring");

        break; 

      case FAMILY_SD:
      case FAMILY_SD | SHORT_TRACE:
        eventIdAttrCnt += 47;
	sprintf( cbuf, "%d", ID_SDattrinfo );
	WRITE_HDF_ATTR( cbuf, "SDattrinfo");
	sprintf( cbuf, "%d", ID_SDcreate );
	WRITE_HDF_ATTR( cbuf, "SDcreate");
	sprintf( cbuf, "%d", ID_SDdiminfo );
	WRITE_HDF_ATTR( cbuf, "SDdiminfo");
	sprintf( cbuf, "%d", ID_SDend );
	WRITE_HDF_ATTR( cbuf, "SDend");
	sprintf( cbuf, "%d", ID_SDendaccess );
	WRITE_HDF_ATTR( cbuf, "SDendaccess");
	sprintf( cbuf, "%d", ID_SDfileinfo );
	WRITE_HDF_ATTR( cbuf, "SDfileinfo");
	sprintf( cbuf, "%d", ID_SDfindattr );
	WRITE_HDF_ATTR( cbuf, "SDfindattr");
	sprintf( cbuf, "%d", ID_SDgetcal );
	WRITE_HDF_ATTR( cbuf, "SDgetcal");
	sprintf( cbuf, "%d", ID_SDgetchunkinfo );
	WRITE_HDF_ATTR( cbuf, "SDgetchunkinfo");
	sprintf( cbuf, "%d", ID_SDgetdatastrs );
	WRITE_HDF_ATTR( cbuf, "SDgetdatastrs");
	sprintf( cbuf, "%d", ID_SDgetdimid );
	WRITE_HDF_ATTR( cbuf, "SDgetdimid");
	sprintf( cbuf, "%d", ID_SDgetdimscale );
	WRITE_HDF_ATTR( cbuf, "SDgetdimscale");
	sprintf( cbuf, "%d", ID_SDgetdimstrs );
	WRITE_HDF_ATTR( cbuf, "SDgetdimstrs");
	sprintf( cbuf, "%d", ID_SDgetfillvalue );
	WRITE_HDF_ATTR( cbuf, "SDgetfillvalue");
	sprintf( cbuf, "%d", ID_SDgetinfo );
	WRITE_HDF_ATTR( cbuf, "SDgetinfo");
	sprintf( cbuf, "%d", ID_SDgetrange );
	WRITE_HDF_ATTR( cbuf, "SDgetrange");
	sprintf( cbuf, "%d", ID_SDidtoref );
	WRITE_HDF_ATTR( cbuf, "SDidtoref");
	sprintf( cbuf, "%d", ID_SDiscoordvar );
	WRITE_HDF_ATTR( cbuf, "SDiscoordvar");
	sprintf( cbuf, "%d", ID_SDisdimval_bwcomp );
	WRITE_HDF_ATTR( cbuf, "SDisdimval_bwcomp");
	sprintf( cbuf, "%d", ID_SDisrecord );
	WRITE_HDF_ATTR( cbuf, "SDisrecord");
	sprintf( cbuf, "%d", ID_SDnametoindex );
	WRITE_HDF_ATTR( cbuf, "SDnametoindex");
	sprintf( cbuf, "%d", ID_SDreadattr );
	WRITE_HDF_ATTR( cbuf, "SDreadattr");
	sprintf( cbuf, "%d", ID_SDreadchunk );
	WRITE_HDF_ATTR( cbuf, "SDreadchunk");
	sprintf( cbuf, "%d", ID_SDreaddata );
	WRITE_HDF_ATTR( cbuf, "SDreaddata");
	sprintf( cbuf, "%d", ID_SDreftoindex );
	WRITE_HDF_ATTR( cbuf, "SDreftoindex");
	sprintf( cbuf, "%d", ID_SDselect );
	WRITE_HDF_ATTR( cbuf, "SDselect");
	sprintf( cbuf, "%d", ID_SDsetaccesstype );
	WRITE_HDF_ATTR( cbuf, "SDsetaccesstype");
	sprintf( cbuf, "%d", ID_SDsetattr );
	WRITE_HDF_ATTR( cbuf, "SDsetattr");
	sprintf( cbuf, "%d", ID_SDsetblocksize );
	WRITE_HDF_ATTR( cbuf, "SDsetblocksize");
	sprintf( cbuf, "%d", ID_SDsetcal );
	WRITE_HDF_ATTR( cbuf, "SDsetcal");
	sprintf( cbuf, "%d", ID_SDsetchunk );
	WRITE_HDF_ATTR( cbuf, "SDsetchunk");
	sprintf( cbuf, "%d", ID_SDsetchunkcache );
	WRITE_HDF_ATTR( cbuf, "SDsetchunkcache");
	sprintf( cbuf, "%d", ID_SDsetcompress );
	WRITE_HDF_ATTR( cbuf, "SDsetcompress");
	sprintf( cbuf, "%d", ID_SDsetdatastrs );
	WRITE_HDF_ATTR( cbuf, "SDsetdatastrs");
	sprintf( cbuf, "%d", ID_SDsetdimname );
	WRITE_HDF_ATTR( cbuf, "SDsetdimname");
	sprintf( cbuf, "%d", ID_SDsetdimscale );
	WRITE_HDF_ATTR( cbuf, "SDsetdimscale");
	sprintf( cbuf, "%d", ID_SDsetdimstrs );
	WRITE_HDF_ATTR( cbuf, "SDsetdimstrs");
	sprintf( cbuf, "%d", ID_SDsetdimval_comp );
	WRITE_HDF_ATTR( cbuf, "SDsetdimval_comp");
	sprintf( cbuf, "%d", ID_SDsetexternalfile );
	WRITE_HDF_ATTR( cbuf, "SDsetexternalfile");
	sprintf( cbuf, "%d", ID_SDsetfillmode );
	WRITE_HDF_ATTR( cbuf, "SDsetfillmode");
	sprintf( cbuf, "%d", ID_SDsetfillvalue );
	WRITE_HDF_ATTR( cbuf, "SDsetfillvalue");
	sprintf( cbuf, "%d", ID_SDsetnbitdataset );
	WRITE_HDF_ATTR( cbuf, "SDsetnbitdataset");
	sprintf( cbuf, "%d", ID_SDsetrag );
	WRITE_HDF_ATTR( cbuf, "SDsetrag");
	sprintf( cbuf, "%d", ID_SDsetrange );
	WRITE_HDF_ATTR( cbuf, "SDsetrange");
	sprintf( cbuf, "%d", ID_SDstart );
	WRITE_HDF_ATTR( cbuf, "SDstart");
	sprintf( cbuf, "%d", ID_SDwritechunk );
	WRITE_HDF_ATTR( cbuf, "SDwritechunk");
	sprintf( cbuf, "%d", ID_SDwritedata );
	WRITE_HDF_ATTR( cbuf, "SDwritedata");

        break; 

      case FAMILY_VF:
      case FAMILY_VF | SHORT_TRACE:
        eventIdAttrCnt += 6;
	sprintf( cbuf, "%d", ID_VFfieldesize );
	WRITE_HDF_ATTR( cbuf, "VFfieldesize");
	sprintf( cbuf, "%d", ID_VFfieldisize );
	WRITE_HDF_ATTR( cbuf, "VFfieldisize");
	sprintf( cbuf, "%d", ID_VFfieldname );
	WRITE_HDF_ATTR( cbuf, "VFfieldname");
	sprintf( cbuf, "%d", ID_VFfieldorder );
	WRITE_HDF_ATTR( cbuf, "VFfieldorder");
	sprintf( cbuf, "%d", ID_VFfieldtype );
	WRITE_HDF_ATTR( cbuf, "VFfieldtype");
	sprintf( cbuf, "%d", ID_VFnfields );
	WRITE_HDF_ATTR( cbuf, "VFnfields");

        break; 

      case FAMILY_V:
      case FAMILY_V | SHORT_TRACE:
        eventIdAttrCnt += 29;
	sprintf( cbuf, "%d", ID_Vaddtagref );
	WRITE_HDF_ATTR( cbuf, "Vaddtagref");
	sprintf( cbuf, "%d", ID_Vattach );
	WRITE_HDF_ATTR( cbuf, "Vattach");
	sprintf( cbuf, "%d", ID_Vdetach );
	WRITE_HDF_ATTR( cbuf, "Vdetach");
	sprintf( cbuf, "%d", ID_Vgetclass );
	WRITE_HDF_ATTR( cbuf, "Vgetclass");
	sprintf( cbuf, "%d", ID_Vgetid );
	WRITE_HDF_ATTR( cbuf, "Vgetid");
	sprintf( cbuf, "%d", ID_Vgetname );
	WRITE_HDF_ATTR( cbuf, "Vgetname");
	sprintf( cbuf, "%d", ID_Vgettagref );
	WRITE_HDF_ATTR( cbuf, "Vgettagref");
	sprintf( cbuf, "%d", ID_Vgettagrefs );
	WRITE_HDF_ATTR( cbuf, "Vgettagrefs");
	sprintf( cbuf, "%d", ID_Vinqtagref );
	WRITE_HDF_ATTR( cbuf, "Vinqtagref");
	sprintf( cbuf, "%d", ID_Vinquire );
	WRITE_HDF_ATTR( cbuf, "Vinquire");
	sprintf( cbuf, "%d", ID_Vinsert );
	WRITE_HDF_ATTR( cbuf, "Vinsert");
	sprintf( cbuf, "%d", ID_Vlone );
	WRITE_HDF_ATTR( cbuf, "Vlone");
	sprintf( cbuf, "%d", ID_Vntagrefs );
	WRITE_HDF_ATTR( cbuf, "Vntagrefs");
	sprintf( cbuf, "%d", ID_Vsetclass );
	WRITE_HDF_ATTR( cbuf, "Vsetclass");
	sprintf( cbuf, "%d", ID_Vsetname );
	WRITE_HDF_ATTR( cbuf, "Vsetname");
	sprintf( cbuf, "%d", ID_Vinitialize );
	WRITE_HDF_ATTR( cbuf, "Vinitialize");
	sprintf( cbuf, "%d", ID_Vfinish );
	WRITE_HDF_ATTR( cbuf, "Vfinish");
	sprintf( cbuf, "%d", ID_Vfind );
	WRITE_HDF_ATTR( cbuf, "Vfind");
	sprintf( cbuf, "%d", ID_Vfindclass );
	WRITE_HDF_ATTR( cbuf, "Vfindclass");
	sprintf( cbuf, "%d", ID_vpackvs );
	WRITE_HDF_ATTR( cbuf, "vpackvs");
	sprintf( cbuf, "%d", ID_vunpackvs );
	WRITE_HDF_ATTR( cbuf, "vunpackvs");
	sprintf( cbuf, "%d", ID_vpackvg );
	WRITE_HDF_ATTR( cbuf, "vpackvg");
	sprintf( cbuf, "%d", ID_vunpackvg );
	WRITE_HDF_ATTR( cbuf, "vunpackvg");
	sprintf( cbuf, "%d", ID_Vsetattr );
	WRITE_HDF_ATTR( cbuf, "Vsetattr");
	sprintf( cbuf, "%d", ID_Vgetversion );
	WRITE_HDF_ATTR( cbuf, "Vgetversion");
	sprintf( cbuf, "%d", ID_Vnattrs );
	WRITE_HDF_ATTR( cbuf, "Vnattrs");
	sprintf( cbuf, "%d", ID_Vfindattr );
	WRITE_HDF_ATTR( cbuf, "Vfindattr");
	sprintf( cbuf, "%d", ID_Vattrinfo );
	WRITE_HDF_ATTR( cbuf, "Vattrinfo");
	sprintf( cbuf, "%d", ID_Vgetattr );
	WRITE_HDF_ATTR( cbuf, "Vgetattr");

        break; 

      case FAMILY_VH:
      case FAMILY_VH | SHORT_TRACE:
        eventIdAttrCnt += 3;
	sprintf( cbuf, "%d", ID_VHmakegroup );
	WRITE_HDF_ATTR( cbuf, "VHmakegroup");
	sprintf( cbuf, "%d", ID_VHstoredata );
	WRITE_HDF_ATTR( cbuf, "VHstoredata");
	sprintf( cbuf, "%d", ID_VHstoredatam );
	WRITE_HDF_ATTR( cbuf, "VHstoredatam");

        break; 

      case FAMILY_VS:
      case FAMILY_VS | SHORT_TRACE:
        eventIdAttrCnt += 33;
	sprintf( cbuf, "%d", ID_VSattach );
	WRITE_HDF_ATTR( cbuf, "VSattach");
	sprintf( cbuf, "%d", ID_VSdetach );
	WRITE_HDF_ATTR( cbuf, "VSdetach");
	sprintf( cbuf, "%d", ID_VSelts );
	WRITE_HDF_ATTR( cbuf, "VSelts");
	sprintf( cbuf, "%d", ID_VSfdefine );
	WRITE_HDF_ATTR( cbuf, "VSfdefine");
	sprintf( cbuf, "%d", ID_VSfexist );
	WRITE_HDF_ATTR( cbuf, "VSfexist");
	sprintf( cbuf, "%d", ID_VSfind );
	WRITE_HDF_ATTR( cbuf, "VSfind");
	sprintf( cbuf, "%d", ID_VSgetclass );
	WRITE_HDF_ATTR( cbuf, "VSgetclass");
	sprintf( cbuf, "%d", ID_VSgetfields );
	WRITE_HDF_ATTR( cbuf, "VSgetfields");
	sprintf( cbuf, "%d", ID_VSgetid );
	WRITE_HDF_ATTR( cbuf, "VSgetid");
	sprintf( cbuf, "%d", ID_VSgetinterlace );
	WRITE_HDF_ATTR( cbuf, "VSgetinterlace");
	sprintf( cbuf, "%d", ID_VSgetname );
	WRITE_HDF_ATTR( cbuf, "VSgetname");
	sprintf( cbuf, "%d", ID_VSinquire );
	WRITE_HDF_ATTR( cbuf, "VSinquire");
	sprintf( cbuf, "%d", ID_VSlone );
	WRITE_HDF_ATTR( cbuf, "VSlone");
	sprintf( cbuf, "%d", ID_VSread );
	WRITE_HDF_ATTR( cbuf, "VSread");
	sprintf( cbuf, "%d", ID_VSseek );
	WRITE_HDF_ATTR( cbuf, "VSseek");
	sprintf( cbuf, "%d", ID_VSsetclass );
	WRITE_HDF_ATTR( cbuf, "VSsetclass");
	sprintf( cbuf, "%d", ID_VSsetfields );
	WRITE_HDF_ATTR( cbuf, "VSsetfields");
	sprintf( cbuf, "%d", ID_VSsetinterlace );
	WRITE_HDF_ATTR( cbuf, "VSsetinterlace");
	sprintf( cbuf, "%d", ID_VSsetname );
	WRITE_HDF_ATTR( cbuf, "VSsetname");
	sprintf( cbuf, "%d", ID_VSsizeof );
	WRITE_HDF_ATTR( cbuf, "VSsizeof");
	sprintf( cbuf, "%d", ID_VSwrite );
	WRITE_HDF_ATTR( cbuf, "VSwrite");
	sprintf( cbuf, "%d", ID_VSappendable );
	WRITE_HDF_ATTR( cbuf, "VSappendable");
	sprintf( cbuf, "%d", ID_VSdelete );
	WRITE_HDF_ATTR( cbuf, "VSdelete");
	sprintf( cbuf, "%d", ID_VSfindclass );
	WRITE_HDF_ATTR( cbuf, "VSfindclass");
	sprintf( cbuf, "%d", ID_VSfpack );
	WRITE_HDF_ATTR( cbuf, "VSfpack");
	sprintf( cbuf, "%d", ID_VSfindex );
	WRITE_HDF_ATTR( cbuf, "VSfindex");
	sprintf( cbuf, "%d", ID_VSsetattr );
	WRITE_HDF_ATTR( cbuf, "VSsetattr");
	sprintf( cbuf, "%d", ID_VSnattrs );
	WRITE_HDF_ATTR( cbuf, "VSnattrs");
	sprintf( cbuf, "%d", ID_VSfnattrs );
	WRITE_HDF_ATTR( cbuf, "VSfnattrs");
	sprintf( cbuf, "%d", ID_VSfindattr );
	WRITE_HDF_ATTR( cbuf, "VSfindattr");
	sprintf( cbuf, "%d", ID_VSattrinfo );
	WRITE_HDF_ATTR( cbuf, "VSattrinfo");
	sprintf( cbuf, "%d", ID_VSgetattr );
	WRITE_HDF_ATTR( cbuf, "VSgetattr");
	sprintf( cbuf, "%d", ID_VSisattr );
	WRITE_HDF_ATTR( cbuf, "VSisattr");

        break; 

      case FAMILY_AN:
      case FAMILY_AN | SHORT_TRACE:
        eventIdAttrCnt += 12;
	sprintf( cbuf, "%d", ID_ANstart );
	WRITE_HDF_ATTR( cbuf, "ANstart");
	sprintf( cbuf, "%d", ID_ANfileinfo );
	WRITE_HDF_ATTR( cbuf, "ANfileinfo");
	sprintf( cbuf, "%d", ID_ANend );
	WRITE_HDF_ATTR( cbuf, "ANend");
	sprintf( cbuf, "%d", ID_ANcreate );
	WRITE_HDF_ATTR( cbuf, "ANcreate");
	sprintf( cbuf, "%d", ID_ANcreatef );
	WRITE_HDF_ATTR( cbuf, "ANcreatef");
	sprintf( cbuf, "%d", ID_ANselect );
	WRITE_HDF_ATTR( cbuf, "ANselect");
	sprintf( cbuf, "%d", ID_ANnumann );
	WRITE_HDF_ATTR( cbuf, "ANnumann");
	sprintf( cbuf, "%d", ID_ANannlist );
	WRITE_HDF_ATTR( cbuf, "ANannlist");
	sprintf( cbuf, "%d", ID_ANannlen );
	WRITE_HDF_ATTR( cbuf, "ANannlen");
	sprintf( cbuf, "%d", ID_ANreadann );
	WRITE_HDF_ATTR( cbuf, "ANreadann");
	sprintf( cbuf, "%d", ID_ANwriteann );
	WRITE_HDF_ATTR( cbuf, "ANwriteann");
	sprintf( cbuf, "%d", ID_ANendaccess );
	WRITE_HDF_ATTR( cbuf, "ANendaccess");

        break; 

      case FAMILY_GR:
      case FAMILY_GR | SHORT_TRACE:
        eventIdAttrCnt += 29;
	sprintf( cbuf, "%d", ID_GRstart );
	WRITE_HDF_ATTR( cbuf, "GRstart");
	sprintf( cbuf, "%d", ID_GRfileinfo );
	WRITE_HDF_ATTR( cbuf, "GRfileinfo");
	sprintf( cbuf, "%d", ID_GRend );
	WRITE_HDF_ATTR( cbuf, "GRend");
	sprintf( cbuf, "%d", ID_GRcreate );
	WRITE_HDF_ATTR( cbuf, "GRcreate");
	sprintf( cbuf, "%d", ID_GRselect );
	WRITE_HDF_ATTR( cbuf, "GRselect");
	sprintf( cbuf, "%d", ID_GRnametoindex );
	WRITE_HDF_ATTR( cbuf, "GRnametoindex");
	sprintf( cbuf, "%d", ID_GRgetiminfo );
	WRITE_HDF_ATTR( cbuf, "GRgetiminfo");
	sprintf( cbuf, "%d", ID_GRwriteimage );
	WRITE_HDF_ATTR( cbuf, "GRwriteimage");
	sprintf( cbuf, "%d", ID_GRreadimage );
	WRITE_HDF_ATTR( cbuf, "GRreadimage");
	sprintf( cbuf, "%d", ID_GRendaccess );
	WRITE_HDF_ATTR( cbuf, "GRendaccess");
	sprintf( cbuf, "%d", ID_GRgetdimid );
	WRITE_HDF_ATTR( cbuf, "GRgetdimid");
	sprintf( cbuf, "%d", ID_GRsetdimname );
	WRITE_HDF_ATTR( cbuf, "GRsetdimname");
	sprintf( cbuf, "%d", ID_GRdiminfo );
	WRITE_HDF_ATTR( cbuf, "GRdiminfo");
	sprintf( cbuf, "%d", ID_GRidtoref );
	WRITE_HDF_ATTR( cbuf, "GRidtoref");
	sprintf( cbuf, "%d", ID_GRreftoindex );
	WRITE_HDF_ATTR( cbuf, "GRreftoindex");
	sprintf( cbuf, "%d", ID_GRreqlutil );
	WRITE_HDF_ATTR( cbuf, "GRreqlutil");
	sprintf( cbuf, "%d", ID_GRreqimageil );
	WRITE_HDF_ATTR( cbuf, "GRreqimageil");
	sprintf( cbuf, "%d", ID_GRgetlutid );
	WRITE_HDF_ATTR( cbuf, "GRgetlutid");
	sprintf( cbuf, "%d", ID_GRgetlutinfo );
	WRITE_HDF_ATTR( cbuf, "GRgetlutinfo");
	sprintf( cbuf, "%d", ID_GRwritelut );
	WRITE_HDF_ATTR( cbuf, "GRwritelut");
	sprintf( cbuf, "%d", ID_GRreadlut );
	WRITE_HDF_ATTR( cbuf, "GRreadlut");
	sprintf( cbuf, "%d", ID_GRsetexternalfile );
	WRITE_HDF_ATTR( cbuf, "GRsetexternalfile");
	sprintf( cbuf, "%d", ID_GRsetaccesstype );
	WRITE_HDF_ATTR( cbuf, "GRsetaccesstype");
	sprintf( cbuf, "%d", ID_GRsetcompress );
	WRITE_HDF_ATTR( cbuf, "GRsetcompress");
	sprintf( cbuf, "%d", ID_GRsetattr );
	WRITE_HDF_ATTR( cbuf, "GRsetattr");
	sprintf( cbuf, "%d", ID_GRattrinfo );
	WRITE_HDF_ATTR( cbuf, "GRattrinfo");
	sprintf( cbuf, "%d", ID_GRgetattr );
	WRITE_HDF_ATTR( cbuf, "GRgetattr");
	sprintf( cbuf, "%d", ID_GRfindattr );
	WRITE_HDF_ATTR( cbuf, "GRfindattr");
	sprintf( cbuf, "%d", ID_GRluttoref );
	WRITE_HDF_ATTR( cbuf, "GRluttoref");

        break; 

      case FAMILY_HA:
      case FAMILY_HA | SHORT_TRACE:
        eventIdAttrCnt += 7;
	sprintf( cbuf, "%d", ID_HAinit_group );
	WRITE_HDF_ATTR( cbuf, "HAinit_group");
	sprintf( cbuf, "%d", ID_HAdestroy_group );
	WRITE_HDF_ATTR( cbuf, "HAdestroy_group");
	sprintf( cbuf, "%d", ID_HAregister_atom );
	WRITE_HDF_ATTR( cbuf, "HAregister_atom");
	sprintf( cbuf, "%d", ID_HAatom_object );
	WRITE_HDF_ATTR( cbuf, "HAatom_object");
	sprintf( cbuf, "%d", ID_HAatom_group );
	WRITE_HDF_ATTR( cbuf, "HAatom_group");
	sprintf( cbuf, "%d", ID_HAremove_atom );
	WRITE_HDF_ATTR( cbuf, "HAremove_atom");
	sprintf( cbuf, "%d", ID_HAsearch_atom );
	WRITE_HDF_ATTR( cbuf, "HAsearch_atom");

        break; 

      case FAMILY_DA:
      case FAMILY_DA | SHORT_TRACE:
        eventIdAttrCnt += 6;
	sprintf( cbuf, "%d", ID_DAcreate_array );
	WRITE_HDF_ATTR( cbuf, "DAcreate_array");
	sprintf( cbuf, "%d", ID_DAdestroy_array );
	WRITE_HDF_ATTR( cbuf, "DAdestroy_array");
	sprintf( cbuf, "%d", ID_DAsize_array );
	WRITE_HDF_ATTR( cbuf, "DAsize_array");
	sprintf( cbuf, "%d", ID_DAget_elem );
	WRITE_HDF_ATTR( cbuf, "DAget_elem");
	sprintf( cbuf, "%d", ID_DAset_elem );
	WRITE_HDF_ATTR( cbuf, "DAset_elem");
	sprintf( cbuf, "%d", ID_DAdel_elem );
	WRITE_HDF_ATTR( cbuf, "DAdel_elem");

        break; 

      case FAMILY_HU:
      case FAMILY_HU | SHORT_TRACE:
        eventIdAttrCnt += 7;
	sprintf( cbuf, "%d", ID_HULcreate_list );
	WRITE_HDF_ATTR( cbuf, "HULcreate_list");
	sprintf( cbuf, "%d", ID_HULdestroy_list );
	WRITE_HDF_ATTR( cbuf, "HULdestroy_list");
	sprintf( cbuf, "%d", ID_HULadd_node );
	WRITE_HDF_ATTR( cbuf, "HULadd_node");
	sprintf( cbuf, "%d", ID_HULsearch_node );
	WRITE_HDF_ATTR( cbuf, "HULsearch_node");
	sprintf( cbuf, "%d", ID_HULfirst_node );
	WRITE_HDF_ATTR( cbuf, "HULfirst_node");
	sprintf( cbuf, "%d", ID_HULnext_node );
	WRITE_HDF_ATTR( cbuf, "HULnext_node");
	sprintf( cbuf, "%d", ID_HULremove_node );
	WRITE_HDF_ATTR( cbuf, "HULremove_node");

        break; 


      default:
	fprintf( stderr, "Unexpected Family in _hdfDescriptor() - exiting\n" );
	exit( -1 );
    }

    sddfWriteInteger( &hdfRecordPointer, INTEGER ); 
    sddfWriteInteger( &hdfRecordPointer, 0 ); 
    /********************************************************************/
    /* The remaining fields are the same for all of the records 	*/
    /********************************************************************/
    WRITE_HDF_FIELD( "Seconds", 
	   	     "Seconds", 
                     "Floating Point Timestamp", 
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Inclusive Duration", 
  	             "Inclusive Duration", 
	             "Inclusive Duration of this Procedure",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "Exclusive Duration", 
  	             "Exclusive Duration", 
	             "Excludes IO, MPI-IO and other HDF calls",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "HDF Duration", 
 	             "HDF Duration", 
                     "Total Time of other HDF procs called by this Proc",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "MPIIO Duration", 
		     "MPIIO Duration", 
	             "All MPI-IO time only in the procedure",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD( "IO Duration", 
		     "IO Duration", 
	             "All IO time only in the procedure",
		      DOUBLE, 0 );
    /********************************************************************/
    /*  Generate a full record record for cases when full I/O is	*/
    /*  performed.							*/
    /********************************************************************/
    if ( !( recordFamily & SHORT_TRACE ) ) {
       WRITE_HDF_FIELD( "Open Duration", 
     	                "Open Duration", 
                        "Open and gOpen time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Close Duration", 
   	                "Close Duration", 
   	                "Close time only in this procedure",
   	                DOUBLE, 0 );
       WRITE_HDF_FIELD( "Read Duration", 
   	                "Read Duration", 
   	                "Read time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Asynch Read Duration", 
   	                "Asynch Read Duration", 
                        "Asynch Read time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Write Duration", 
                        "Write Duration", 
                        "Write time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Asynch Write Duration", 
   		        "Asynch Write Duration", 
                        "Asynch Write time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Seek Duration", 
                        "Seek Duration", 
                        "Seek time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Wait Duration", 
                        "Wait Duration", 
                        "Wait time only in this procedure",
   		        DOUBLE, 0 );
       WRITE_HDF_FIELD( "Misc IO Duration", 
                        "Misc IO Duration", 
                        "Misc IO time only in this procedure",
   		        DOUBLE, 0 );
    } 
    /********************************************************************/
    /* These fields always get generated.				*/
    /********************************************************************/
    WRITE_HDF_FIELD( "HDF Operation Count", 
		     "HDF Operation Count", 
	             "Count HDF calls in the procedure", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "MPIIO Operation Count", 
		     "MPIIO Operation Count", 
	             "Count MPI-IO calls in this procedure", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "IO Operation Count", 
		     "IO Operation Count", 
	             "Count All IO ops done in this procedure", 
		     INTEGER, 0 );
    
    /********************************************************************/
    /* These fields only get generated for the full trace.		*/
    /********************************************************************/
    if ( !( recordFamily & SHORT_TRACE ) ) {
       WRITE_HDF_FIELD( "Open Count", 
   		        "Open Count", 
   	                "Count Opens/gOpens done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Close Count", 
   		        "Close Count", 
   	                "Count Closes done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Read Count", 
   		        "Read Count",
   	                "Count Reads done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Asynch Read Count", 
   		        "Asynch Read Count", 
   	                "Count Asynch Reads done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Write Count", 
   		        "Write Count",
   	                "Count Writes done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Asynch Write Count", 
   		        "Asynch Write Count", 
   	                "Count Asynch Writes done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Seek Count", 
   		        "Seek Count",
   	                "Count Seeks done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Wait Count", 
   		        "Wait Count",
   	                "Count Waits done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Misc IO Count", 
   		        "Misc IO Count",
   	                "Count Misc IO ops done in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Read Number Bytes", 
   		        "Read Number Bytes",
   	                "Bytes Read in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Asynch Read Number Bytes", 
   		        "Asynch Read Number Bytes", 
   	                "Bytes Asynch Read in this procedure",
   		        INTEGER, 0 );
       WRITE_HDF_FIELD( "Write Number Bytes", 
   		        "Write Number Bytes",
   	                "Bytes Written in this procedure",
   		        INTEGER, 0 ); 
       WRITE_HDF_FIELD( "Asynch Write Number Bytes", 
   		        "Asynch Write Number Bytes", 
   	                "Bytes Asynch Written in this proc",
   		        INTEGER, 0 ); 
    }
    /********************************************************************/
    /* these fields always get generated.				*/
    /********************************************************************/
    WRITE_HDF_FIELD( "Processor Number", 
		     "Node", 
		     "Processor number", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Number of Calls", 
		     "Number of Calls", 
		     "Number of Calls Summarized in this Packet", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD2("HDF ID",
                     "HDF ID", "Identifier number",
                     "0", "No HDF ID specified",
                     INTEGER, 0 );
    WRITE_HDF_FIELD( "Xref ID",
                     "Cross Reference", 
                     "Index of related HDF ID or 0 if none",
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "HDF Name",
                     "HDF Name", 
	             "Name of File,Data Set or Dim accessed",
                     CHARACTER, 1 ); 
    /********************************************************************/
    /* The entire record descriptor packet has been written.		*/
    /* Compute and update the record length.				*/
    /* Write the completed record.					*/
    /********************************************************************/
    if ( eventIdAttrCnt != 1 )  {
	sddfWriteInteger( &eventIdAttrPtr, eventIdAttrCnt );
    }

    recordLength = hdfRecordPointer - recordBuffer;

    hdfRecordPointer = recordBuffer;
    sddfWriteInteger( &hdfRecordPointer, recordLength );

    putBytes( recordBuffer, (unsigned) recordLength );
}

/************************************************************************/
/*   Internal Routine:  writeIORecordDescriptors                        */
/*                      Writes record descriptors for the portable I/O  */
/*                      events. Calls writeIORecordDesciptors_SD to     */
/*                      output the descriptors for the system dependent */
/*                      events.  Called by the instrumentation library  */
/*                      initialization routines.                        */
/************************************************************************/
void writeHDFRecordDescriptors()
{
	_hdfDescriptor( "HDF DFAN Trace","HDF DFAN Procedure",
			FAMILY_DFAN, DFAN_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFP Trace","HDF DFP Procedure",
			FAMILY_DFP, DFP_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFR8 Trace","HDF DFR8 Procedure",
			FAMILY_DFR8, DFR8_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFSD Trace","HDF DFSD Procedure",
			FAMILY_DFSD, DFSD_NUM_FIELDS );  
	_hdfDescriptor( "HDF DF24 Trace","HDF DF24 Procedure",
			FAMILY_DF24, DF24_NUM_FIELDS );  
	_hdfDescriptor( "HDF H Trace","HDF H Procedure",
			FAMILY_H, H_NUM_FIELDS );  
	_hdfDescriptor( "HDF HE Trace","HDF HE Procedure",
			FAMILY_HE, HE_NUM_FIELDS );  
	_hdfDescriptor( "HDF SD Trace","HDF SD Procedure",
			FAMILY_SD, SD_NUM_FIELDS );  
	_hdfDescriptor( "HDF VF Trace","HDF VF Procedure",
			FAMILY_VF, VF_NUM_FIELDS );  
	_hdfDescriptor( "HDF V Trace","HDF V Procedure",
			FAMILY_V, V_NUM_FIELDS );  
	_hdfDescriptor( "HDF VH Trace","HDF VH Procedure",
			FAMILY_VH, VH_NUM_FIELDS );  
	_hdfDescriptor( "HDF VS Trace","HDF VS Procedure",
			FAMILY_VS, VS_NUM_FIELDS );  
	_hdfDescriptor( "HDF AN Trace","HDF AN Procedure",
			FAMILY_AN, AN_NUM_FIELDS );  
	_hdfDescriptor( "HDF GR Trace","HDF GR Procedure",
			FAMILY_GR, GR_NUM_FIELDS );  
	_hdfDescriptor( "HDF HA Trace","HDF HA Procedure",
			FAMILY_HA, HA_NUM_FIELDS );  
	_hdfDescriptor( "HDF DA Trace","HDF DA Procedure",
			FAMILY_DA, DA_NUM_FIELDS );  
	_hdfDescriptor( "HDF HU Trace","HDF HU Procedure",
			FAMILY_HU, HU_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFAN Short Trace",
			"HDF DFAN Procedure with no IO",
			FAMILY_DFAN | SHORT_TRACE, 
			DFAN_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFP Short Trace",
			"HDF DFP Procedure with no IO",
			FAMILY_DFP | SHORT_TRACE, 
			DFP_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFR8 Short Trace",
			"HDF DFR8 Procedure with no IO",
			FAMILY_DFR8 | SHORT_TRACE, 
			DFR8_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF DFSD Short Trace",
			"HDF DFSD Procedure with no IO",
			FAMILY_DFSD | SHORT_TRACE, 
			DFSD_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF DF24 Short Trace",
			"HDF DF24 Procedure with no IO",
			FAMILY_DF24 | SHORT_TRACE, 
			DF24_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF H Short Trace",
			"HDF H Procedure with no IO",
			FAMILY_H | SHORT_TRACE, 
			H_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF HE Short Trace",
			"HDF HE Procedure with no IO",
			FAMILY_HE | SHORT_TRACE, 
			HE_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF SD Short Trace",
			"HDF SD Procedure with no IO",
			FAMILY_SD | SHORT_TRACE, 
			SD_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF VF Short Trace",
			"HDF VF Procedure with no IO",
			FAMILY_VF | SHORT_TRACE, 
			VF_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF V Short Trace",
			"HDF V Procedure with no IO",
			FAMILY_V | SHORT_TRACE, 
			V_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF VH Short Trace",
			"HDF VH Procedure with no IO",
			FAMILY_VH | SHORT_TRACE, 
			VH_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF VS Short Trace",
			"HDF VS Procedure with no IO",
			FAMILY_VS | SHORT_TRACE, 
			VS_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF AN Short Trace",
			"HDF AN Procedure with no IO",
			FAMILY_AN | SHORT_TRACE, 
			AN_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF GR Short Trace",
			"HDF GR Procedure with no IO",
			FAMILY_GR | SHORT_TRACE, 
			GR_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF HA Short Trace",
			"HDF HA Procedure with no IO",
			FAMILY_HA | SHORT_TRACE, 
			HA_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF DA Short Trace",
			"HDF DA Procedure with no IO",
			FAMILY_DA | SHORT_TRACE, 
			DA_SHORT_NUM_FIELDS );  
	_hdfDescriptor( "HDF HU Short Trace",
			"HDF HU Procedure with no IO",
			FAMILY_HU | SHORT_TRACE, 
			HU_SHORT_NUM_FIELDS );  
	return;
}
/************************************************************************/
/*	_hdfTraceEntryDescriptor					*/
/*	   Generate a SDDF binary format record descriptor for the	*/
/*	   full trace class of events in the HDF procedure entry 	*/
/************************************************************************/
void _hdfTraceEntryDescriptor()
{
    static char recordBuffer[ 4096 ];
    char	*hdfRecordPointer;
    int         recordLength;
    char        cbuf[128];

#ifdef DEBUG
	fprintf( debugFile, "_hdfTraceEntryDescriptor entered\n" );
	fflush( debugFile );
#endif /* DEBUG */
    hdfRecordPointer = recordBuffer;
    /********************************************************************/
    /* Allow space at the beginning of the record for the packet        */
    /*length which will be computed after the packet is complete.       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 0 );
    /********************************************************************/
    /* The record type, tag, and name                                   */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, PKT_DESCRIPTOR );
    sddfWriteInteger( &hdfRecordPointer, ( FAMILY_PROCENTRY | RECORD_TRACE ) );
    sddfWriteString( &hdfRecordPointer, "HDF Procedure Entry Trace" );
    /********************************************************************/
    /* The record attribute count and string pair                       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 1 );
    sddfWriteString( &hdfRecordPointer, "description" );
    sddfWriteString( &hdfRecordPointer, "HDF Procedure Entry Trace Record" );
    /********************************************************************/
    /* The record field count                                           */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 5);
    /********************************************************************/
    /* Create fields                                               	*/
    /********************************************************************/
    WRITE_HDF_FIELD( "Event Identifier", 
		     "Event ID", 
		     "Event Identifier Number", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Seconds", 
		     "Seconds", 
		     "Floating Point Timestamp", 
		     DOUBLE, 0 );
    WRITE_HDF_FIELD( "Processor Number", 
		     "Node", 
		     "Processor number", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Source Byte", 
		     "Byte", 
		     "Source Byte Offset", 
		     INTEGER, 0 );
    WRITE_HDF_FIELD( "Source Line", 
		     "Line", 
		     "Source Line Number",
		     INTEGER, 0 );

    recordLength = hdfRecordPointer - recordBuffer;

    hdfRecordPointer = recordBuffer;
    sddfWriteInteger( &hdfRecordPointer, recordLength );

    putBytes( recordBuffer, (unsigned) recordLength );
}
/************************************************************************/
/*	_hdfExitTraceDescriptor	     					*/
/*	   Generate a SDDF binary format record descriptor for the	*/
/*	   full trace class of events in the HDF procedure exit  	*/
/************************************************************************/
void _hdfTraceExitDescriptor()
{
    static char recordBuffer[ 4096 ];
    int         recordLength;
    char	*hdfRecordPointer;
    char        cbuf[128];

#ifdef DEBUG
	fprintf( debugFile, "_hdfExitTraceDescriptor entered\n" );
	fflush( debugFile );
#endif /* DEBUG */
    hdfRecordPointer = recordBuffer;
    /********************************************************************/
    /* Allow space at the beginning of the record for the packet        */
    /*length which will be computed after the packet is complete.       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 0 );
    /********************************************************************/
    /* The record type, tag, and name                                   */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, PKT_DESCRIPTOR );
    sddfWriteInteger( &hdfRecordPointer, ( FAMILY_PROCEXIT | RECORD_TRACE ) );
    sddfWriteString( &hdfRecordPointer, "HDF Procedure Exit Trace" );
    /********************************************************************/
    /* The record attribute count and string pair                       */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 1 );
    sddfWriteString( &hdfRecordPointer, "description" );
    sddfWriteString( &hdfRecordPointer, "HDF Procedure Exit Trace Record" );
    /********************************************************************/
    /* The record field count                                           */
    /********************************************************************/
    sddfWriteInteger( &hdfRecordPointer, 7);
    /********************************************************************/
    /* Create fields                                               	*/
    /********************************************************************/
    WRITE_HDF_FIELD(  "Event Identifier", 
		      "Event ID", 
		      "Event Identifier Number", 
		      INTEGER, 0 );
    WRITE_HDF_FIELD(  "Seconds", 
		      "Seconds", 
		      "Floating Point Timestamp", 
		      DOUBLE, 0 );
    WRITE_HDF_FIELD(  "Inclusive Seconds", 
                      "Inclusive Seconds",
         	      "Floating Point Inclusive Duration",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD(  "Exclusive Seconds", 
                      "Exclusive Seconds",
         	      "Floating Point Exclusive Duration",
		      DOUBLE, 0 );
    WRITE_HDF_FIELD(  "Processor Number", 
		      "Node", 
		      "Processor number", 
		      INTEGER, 0 );
    WRITE_HDF_FIELD2( "HDF ID",
                      "HDF ID", "File, Data Set or Dim Identifier number",
                      "0", "No HDF ID specified",
                      INTEGER, 0 ); 
    WRITE_HDF_FIELD( "HDF Name",
                     "HDF Name", "Name of File, Data Set or Dim",
                      CHARACTER, 1 );

    recordLength = hdfRecordPointer - recordBuffer;

    hdfRecordPointer = recordBuffer;
    sddfWriteInteger( &hdfRecordPointer, recordLength );

    putBytes( recordBuffer, (unsigned) recordLength );
}

#endif /* HAVE_PABLO */
