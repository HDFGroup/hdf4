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
 * File:  ProcMasks.h
 * Purpose: define bit masks for tracing families of procedures
 *-------------------------------------------------------------------------*/

#ifndef PROCMASKS_H		/* avoid re-inclusion */
#define PROCMASKS_H

/* Each mask is a single bit identifying a family of procedures.  To
   enable procedure tracing on specific procedure families, specify
   the logical-or (|) of the desired masks in the procTraceMask
   parameter of the HDFinitIOTrace function. */


#define DFAN_mask	0x1
/* DFANaddfds, DFANaddfid, DFANgetdesc, DFANgetdesclen, DFANgetfds,
   DFANgetfdslen, DFANgetfid, DFANgetfidlen, DFANgetlabel,
   DFANgetlablen, DFANlablist, DFANlastref, DFANputdesc, DFANputlabel 
   DFANclear */

#define DFP_mask	0x2
/* DFPaddpal, DFPgetpal, DFPlastref, DFPnpals, DFPputpal, DFPreadref,
   DFPrestart, DFPwriteref */

#define DFR8_mask	0x4
/* DFR8addimage, DFR8getdims, DFR8getimage, DFR8lastref, DFR8nimages,
   DFR8putimage, DFR8readref, DFR8restart, DFR8setcompress,
   DFR8setpalette, DFR8writeref */

#define DFSD_mask	0x8
/* DFSDadddata, DFSDclear, DFSDendslab, DFSDendslice, DFSDgetcal,
   DFSDgetdata, DFSDgetdatalen, DFSDgetdatastrs, DFSDgetdimlen,
   DFSDgetdims, DFSDgetdimscale, DFSDgetdimstrs, DFSDgetfillvalue,
   DFSDgetNT, DFSDgetrange, DFSDgetslice, DFSDlastref, DFSDndatasets,
   DFSDpre32sdg, DFSDputdata, DFSDputslice, DFSDreadref, DFSDreadslab,
   DFSDrestart, DFSDsetcal, DFSDsetdatastrs, DFSDsetdims,
   DFSDsetdimscale, DFSDsetdimstrs, DFSDsetfillvalue, DFSDsetlengths,
   DFSDsetNT, DFSDsetrange, DFSDstartslab, DFSDstartslice,
   DFSDwriteref, DFSDwriteslab */

#define DF24_mask	0x10
/* DF24addimage, DF24getdims, DF24getimage, DF24lastref, DF24nimages,
   DF24putimage, DF24readref, DF24reqil, DF24restart, DF24setcompress,
   DF24setdims, DF24setil */

#define H_mask		0x20
/* Happendable, Hclose, Hdeldd, Hdupdd, Hendaccess, Hfind,
   Hgetelement, Hgetfileversion, Hgetlibversion, Hinquire, Hishdf,
   HLcreate, Hlength, Hnewref, Hnextread, Hnumber, Hoffset, Hopen,
   Hputelement, Hread, Hseek, Hstartread, Hstartwrite, Hsync, Htrunc,
   Hwrite, HXcreate, HXsetcreatedir, HXsetdir, HLconvert, Hstartaccess,
   Hisappendable, Htell, Htagnewref, Hcache, Hsetaccesstype, Hexist,
   Hsetlength */

#define HE_mask		0x40
/* HEclear, HEprint, HEpush, HEreport, HEstring */

#define SD_mask		0x80
/* SDattrinfo, SDcreate, SDdiminfo, SDend, SDendaccess, SDfileinfo,
   SDfindattr, SDgetcal, SDgetdatastrs, SDgetdimid, SDgetdimscale,
   SDgetdimstrs, SDgetfillvalue, SDgetinfo, SDgetrange, SDidtoref,
   SDiscoordvar, SDnametoindex, SDreadattr, SDreaddata, SDreftoindex,
   SDselect, SDsetattr, SDsetcal, SDsetdatastrs, SDsetdimname,
   SDsetdimscale, SDsetdimstrs, SDsetexternalfile, SDsetfillvalue,
   SDsetrange, SDstart, SDwritedata */

#define VF_mask		0x100
/* VFfieldesize, VFfieldisize, VFfieldname, VFfieldorder, VFfieldtype,
   VFnfields */

#define V_mask		0x200
/* Vaddtagref, Vattach, Vdetach, Vgetclass, Vgetid, Vgetname,
   Vgettagref, Vgettagrefs, Vinqtagref, Vinquire, Vinsert, Vlone,
   Vntagrefs, Vsetclass, Vsetname, Vinitialize, Vfinish, Vfind, Vfindclass */

#define VH_mask		0x400
/* VHmakegroup, VHstoredata, VHstoredatam */

#define VS_mask		0x800
/* VSattach, VSdetach, VSelts, VSfdefine, VSfexist, VSfind,
   VSgetclass, VSgetfields, VSgetid, VSgetinterlace, VSgetname,
   VSinquire, VSlone, VSread, VSseek, VSsetclass, VSsetfields,
   VSsetinterlace, VSsetname, VSsizeof, VSwrite, VSappendable, VSdelete
   VSfindclass */

#define AN_mask         0x900
/* ANstart, ANfileinfo, ANend, ANcreate, ANcreatef, ANselect, ANnumann,
   ANannlist, ANannlen, ANreadann, ANwriteann, ANendacess */

#define GR_mask         0x1000
/* GRstart, GRfileinfo, GRend, GRcreate, GRselect, GRnametoindex,
   GRgetiminfo, GRwriteimage, GRreadimage, GRendaccess, GRgetdimid,
   GRsetdimname, GRdiminfo, GRidtoref, GRreftoindex, GRreqlutil,
   GRreqimageil, GRgetlutid, GRgetlutinfo, GRwritelut, GRreadlut,
   GRsetexternalfile, GRsetaccestype, GRsetcompress, GRsetattr, 
   GRattrinfo, GRgetattr, GRfindattr */

#endif /* PROCMASK_H */
