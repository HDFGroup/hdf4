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
 * File:  ProcIDs.h
 * Purpose: define IDs for identifying procedures in traces
 *-------------------------------------------------------------------------*/

#ifndef PROCIDS_H		/* avoid re-inclusion */
#define PROCIDS_H

/*#if defined HAVE_PABLO || defined PABLO*/

#include "ProcMasks.h"
extern unsigned procTrace;
/*
 * Define the event IDs that will be used for the various I/O events
 */

/*======================================================================*/
/* Assign HDF identifier routine tags					*/
/*======================================================================*/
enum HDF_IDS {
	DUMMY_HDF = 10000,
	BEGIN_HDF = 10001,
	ID_DFAN = BEGIN_HDF,
	ID_DFANaddfds = ID_DFAN,
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
	ID_DFP,
	ID_DFPaddpal = ID_DFP,
	ID_DFPgetpal,
	ID_DFPlastref,
	ID_DFPnpals,
	ID_DFPputpal,
	ID_DFPreadref,
	ID_DFPrestart,
	ID_DFPwriteref,
	ID_DFR8,
	ID_DFR8addimage = ID_DFR8,
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
	ID_DFSD,
	ID_DFSDadddata = ID_DFSD,
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
	ID_DF24,
	ID_DF24addimage = ID_DF24,
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
	ID_H,
	ID_Happendable = ID_H,
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
	ID_HBcreate,
	ID_HBconvert,
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
	ID_HE,
	ID_HEclear = ID_HE,
	ID_HEprint,
	ID_HEpush,
	ID_HEreport,
	ID_HEstring,
	ID_HRPconvert,
	ID_SD,
        ID_SDattrinfo = ID_SD,
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
	ID_VF,
	ID_VFfieldesize = ID_VF,
	ID_VFfieldisize,
	ID_VFfieldname,
	ID_VFfieldorder,
	ID_VFfieldtype,
	ID_VFnfields,
	ID_V,
	ID_Vaddtagref = ID_V,
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
	ID_VH,
	ID_VHmakegroup = ID_VH,
	ID_VHstoredata,
	ID_VHstoredatam,
	ID_VS,
	ID_VSattach = ID_VS,
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
	ID_AN,
	ID_ANstart = ID_AN,
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
	ID_GR,
	ID_GRstart = ID_GR,
	ID_GRfileinfo,
	ID_GRend,
	ID_GRcreate,
	ID_GRselect,
	ID_GRnametoindex,
	ID_GRgetiminfo,
	ID_GRendaccess,
	ID_GRgetdimid,
	ID_GRsetdimname,
	ID_GRdiminfo,
	ID_GRidtoref,
	ID_GRreadchunk,
	ID_GRreadimage,
	ID_GRreftoindex,
	ID_GRreqlutil,
	ID_GRreqimageil,
	ID_GRgetlutid,
	ID_GRgetlutinfo,
	ID_GRwritechunk,
	ID_GRwriteimage,
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
	ID_HA,
	ID_HAinit_group = ID_HA,
	ID_HAdestroy_group,
	ID_HAregister_atom,
	ID_HAatom_object,
	ID_HAatom_group,
	ID_HAremove_atom,
	ID_HAsearch_atom,
	ID_DA,
	ID_DAcreate_array = ID_DA,
	ID_DAdestroy_array,
	ID_DAsize_array,
	ID_DAget_elem,
	ID_DAset_elem,
	ID_DAdel_elem,
	ID_HUL,
	ID_HULcreate_list = ID_HUL,
	ID_HULdestroy_list,
	ID_HULadd_node,
	ID_HULsearch_node,
	ID_HULfirst_node,
	ID_HULnext_node,
	ID_HULremove_node,
	ID_HDFnewentry,
	ID_SDcheckempty,
	END_HDF
} ;

#define NumHDFProcs ( END_HDF - BEGIN_HDF )
/*======================================================================*/
/* HDF attribute, file, data_set, etc ID codes.	May not be used.	*/
/*======================================================================*/
#define NoDSid                     0            /* no data set id       */
#define HDF_NULL_ID 		   0
#define HDF_File_ID 		   1
#define HDF_SDS_ID 		   2
#define HDF_Dim_ID 		   3
#define HDF_Attribute_ID 	   4
#define HDF_Label_ID 		   5
#define HDF_Access_ID 		   6
#define HDF_Directory_ID 	   7
#define HDF_Annotation_ID          8
#define HDF_Gen_Raster_ID          9
#define HDF_Look_Up_Table_ID      10
#define HDF_Vdata_ID 		  11
/*======================================================================*/
/* Macros to tell if the ID is that of an HDF Entry or Exit             */
/*======================================================================*/
#define isBeginHDFEvent( ID ) ( BEGIN_HDF < (ID) && (ID) < END_HDF )
#define isEndHDFEvent( ID )   isBeginHDFEvent(-ID)
#define isBeginIOEvent( ID )  \
        ( IOerrorID < (ID) && (ID) <= fsetposEndID && (ID)%2 == 1 )
#define isEndIOEvent( ID ) \
        ( IOerrorID < (ID) && (ID) <= fsetposEndID && (ID)%2 == 0 )
#define ProcIndex( ID ) ( (ID) - BEGIN_HDF )
#define ProcIndexForHDFEntry( ID ) ( (ID) - BEGIN_HDF )
#define ProcIndexForHDFExit( ID )  ProcIndexForHDFEntry(-ID)
#define HDFProcIXtoEventID( ID ) ( (ID) + BEGIN_HDF )
 
#endif /* PROCIDS_H */
