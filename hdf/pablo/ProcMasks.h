/*  This file is part of the Pablo Performance Analysis Environment
// 
//           (R)
//  The Pablo    Performance Analysis Environment software is NOT in
//  the public domain.  However, it is freely available without fee for
//  education, research, and non-profit purposes.  By obtaining copies
//  of this and other files that comprise the Pablo Performance Analysis
//  Environment, you, the Licensee, agree to abide by the following
//  conditions and understandings with respect to the copyrighted software:
//  
//  1.  The software is copyrighted in the name of the Board of Trustees
//      of the University of Illinois (UI), and ownership of the software
//      remains with the UI. 
// 
//  2.  Permission to use, copy, and modify this software and its documentation
//      for education, research, and non-profit purposes is hereby granted
//      to Licensee, provided that the copyright notice, the original author's
//      names and unit identification, and this permission notice appear on
//      all such copies, and that no charge be made for such copies.  Any
//      entity desiring permission to incorporate this software into commercial
//      products should contact:
// 
//           Professor Daniel A. Reed                 reed@cs.uiuc.edu
//           University of Illinois
//           Department of Computer Science
//           2413 Digital Computer Laboratory
//           1304 West Springfield Avenue
//           Urbana, Illinois  61801
//           USA
// 
//  3.  Licensee may not use the name, logo, or any other symbol of the UI
//      nor the names of any of its employees nor any adaptation thereof in
//      advertizing or publicity pertaining to the software without specific
//      prior written approval of the UI.
// 
//  4.  THE UI MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE
//      SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS
//      OR IMPLIED WARRANTY.
// 
//  5.  The UI shall not be liable for any damages suffered by Licensee from
//      the use of this software.
// 
//  6.  The software was developed under agreements between the UI and the
//      Federal Government which entitle the Government to certain rights.
// 
// *************************************************************************
// 
//  Developed by: The Pablo Research Group
//                University of Illinois at Urbana-Champaign
//                Department of Computer Science
//                1304 W. Springfield Avenue
//                Urbana, IL     61801
// 
//                http://www-pablo.cs.uiuc.edu
// 
//  Send comments to: pablo-feedback@guitar.cs.uiuc.edu
// 
//  Copyright (c) 1987-1998
//  The University of Illinois Board of Trustees.
//       All Rights Reserved.
// 
//  PABLO is a registered trademark of
//  The Board of Trustees of the University of Illinois
//  registered in the U.S. Patent and Trademark Office.
// 
//  Project Manager and Principal Investigator:
//       Daniel A. Reed (reed@cs.uiuc.edu)
// 
// Funded in part by the Defense Advanced Research Projects Agency under 
// DARPA contracts DABT63-94-C0049 (SIO Initiative), F30602-96-C-0161,
// and DABT63-96-C-0027 by the National Science Foundation under the PACI 
// program and grants NSF CDA 94-01124 and ASC 97-20202, and by the 
// Department of Energy under contracts DOE B-341494, W-7405-ENG-48, and 
// 1-B-333164.
//-------------------------------------------------------------------------*/
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
   Hsetlength, Hfidinquire */

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

#define AN_mask         0x1000
/* ANstart, ANfileinfo, ANend, ANcreate, ANcreatef, ANselect, ANnumann,
   ANannlist, ANannlen, ANreadann, ANwriteann, ANendacess */

#define GR_mask         0x2000
/* GRstart, GRfileinfo, GRend, GRcreate, GRselect, GRnametoindex,
   GRgetiminfo, GRwriteimage, GRreadimage, GRendaccess, GRgetdimid,
   GRsetdimname, GRdiminfo, GRidtoref, GRreftoindex, GRreqlutil,
   GRreqimageil, GRgetlutid, GRgetlutinfo, GRwritelut, GRreadlut,
   GRsetexternalfile, GRsetaccestype, GRsetcompress, GRsetattr, 
   GRattrinfo, GRgetattr, GRfindattr */

#define HA_mask         0x4000
/* HAinit_group, HAdestroy_group, HAregister_atom, HAatom_object,
   HAatom_group, HAremove_atom */
   
#define DA_mask         0x8000
/* DAcreate_array, DAdestroy_array, DAsize_array, DAget_elem,
   DAset_elem, DAdel_elem */

#define HUL_mask        0x10000

#define AllHDF_mask 	0x1FFFF

#endif /* PROCMASK_H */
