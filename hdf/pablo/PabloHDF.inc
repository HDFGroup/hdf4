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
C * Funded by: National Aeronautics and Space Administration under NASA
C * Contracts NAG-1-613 and USRA 5555-22 and by the Advanced Research
C * Projects Agency under ARPA contracts DAVT63-91-C-0029 and
C * DABT63-93-C-0040.
C *

C *---------------------------------------------------------------------------
C * File: 	PabloHDF.inc
C * Purpose:	Fortran header file for Pablo routines
C * Contents: 
C *    	Procedure IDs - obtained from ProcIDs.h
C * Remarks: This file can be included with Fortran user programs who 
C *          want to enable Pablo tracing.  
C *---------------------------------------------------------------------------

C IDs for annotation procedures

      integer ID_DFANaddfds,ID_DFANaddfid,ID_DFANgetdesc,ID_DFANgetdesclen
      integer ID_DFANgetfds,ID_DFANgetfdslen,ID_DFANgetfid,ID_DFANgetfidlen
      integer ID_DFANgetlabel,ID_DFANgetlablen,ID_DFANlablist,ID_DFANlastref
      integer ID_DFANputdesc,ID_DFANputlabel, ID_DFANclear

      integer ID_DFPaddpal,ID_DFPgetpal,ID_DFPlastref,ID_DFPnpals
      integer ID_DFPputpal,ID_DFPreadref,ID_DFPrestart,ID_DFPwriteref

      integer ID_DFR8addimage,ID_DFR8getdims,ID_DFR8getimage,ID_DFR8lastref
      integer ID_DFR8nimages,ID_DFR8putimage,ID_DFR8readref,ID_DFR8restart
      integer ID_DFR8setcompress,ID_DFR8setpalette,ID_DFR8writeref
      integer ID_DFR8Istart

      integer ID_DFSDadddata,ID_DFSDclear,ID_DFSDendslab,ID_DFSDendslice
      integer ID_DFSDgetcal,ID_DFSDgetdata,ID_DFSDgetdatalen
      integer ID_DFSDgetdatastrs,ID_DFSDgetdimlen,ID_DFSDgetdims
      integer ID_DFSDgetdimscale,ID_DFSDgetdimstrs,ID_DFSDgetfillvalue
      integer ID_DFSDgetNT,ID_DFSDgetrange,ID_DFSDgetslice,ID_DFSDlastref
      integer ID_DFSDndatasets,ID_DFSDpre32sdg,ID_DFSDputdata,ID_DFSDputslice
      integer ID_DFSDreadref,ID_DFSDreadslab,ID_DFSDrestart,ID_DFSDsetcal
      integer ID_DFSDsetdatastrs,ID_DFSDsetdims,ID_DFSDsetdimscale
      integer ID_DFSDsetdimstrs,ID_DFSDsetfillvalue,ID_DFSDsetlengths
      integer ID_DFSDsetNT,ID_DFSDsetrange,ID_DFSDstartslab,ID_DFSDstartslice
      integer ID_DFSDwriteref,ID_DFSDwriteslab

      integer ID_DF24addimage,ID_DF24getdims,ID_DF24getimage,ID_DF24lastref
      integer ID_DF24nimages,ID_DF24putimage,ID_DF24readref,ID_DF24reqil
      integer ID_DF24restart,ID_DF24setcompress,ID_DF24setdims,ID_DF24setil

      integer ID_Happendable,ID_Hclose,ID_Hdeldd,ID_Hdupdd,ID_Hendaccess
      integer ID_Hfind,ID_Hgetelement,ID_Hgetfileversion,ID_Hgetlibversion
      integer ID_Hinquire,ID_Hishdf,ID_HLcreate,ID_Hlength,ID_Hnewref
      integer ID_Hnextread,ID_Hnumber,ID_Hoffset,ID_Hopen,ID_Hputelement
      integer ID_Hread,ID_Hseek,ID_Hstartread,ID_Hstartwrite,ID_Hsync
      integer ID_Htrunc,ID_Hwrite,ID_HXcreate, ID_HXsetcreatedir,
      integer ID_HXsetdir, ID_HLconvert, ID_Hstartaccess, ID_Hisappendable
      integer ID_Htell, ID_Htagnewref, ID_Hcaceh, ID_Hsetaccesstype
      integer ID_Hexist, ID_Hsetlength, ID_Hfidinquire
      integer ID_HDdont_atexit, ID_HDreuse_tagref, ID_HIstart 
      integer ID_HPregister_term_func
      integer ID_HIbitstart, ID_HMCcreate, ID_HMCsetMaxcache
      integer ID_HMCPstread, ID_HMCPstwrite, ID_HMCPseek, ID_HMCPchunkread
      integer ID_HMCreadChunk, ID_HMCPread, ID_HMCPchunkwrite
      integer ID_HMCwriteChunk, ID_HMCPwrite, ID_HMCPendaccess
      integer ID_HMCPinfo, ID_HMCPinquire

      integer ID_HEclear,ID_HEprint,ID_HEpush,ID_HEreport,ID_HEstring

      integer ID_SDattrinfo,ID_SDcreate,ID_SDdiminfo,ID_SDend,ID_SDendaccess
      integer ID_SDfileinfo,ID_SDfindattr,ID_SDgetcal,ID_SDgetdatastrs
      integer ID_SDgetdimid,ID_SDgetdimscale,ID_SDgetdimstrs,ID_SDgetfillvalue
      integer ID_SDgetinfo,ID_SDgetrange,ID_SDidtoref,ID_SDiscoordvar
      integer ID_SDnametoindex,ID_SDreadattr,ID_SDreaddata,ID_SDreftoindex
      integer ID_SDselect,ID_SDsetattr,ID_SDsetcal,ID_SDsetdatastrs
      integer ID_SDsetdimname,ID_SDsetdimscale,ID_SDsetdimstrs
      integer ID_SDsetexternalfile,ID_SDsetfillvalue,ID_SDsetrange,ID_SDstart
      integer ID_SDwritedata, ID_SDcheckempty

      integer ID_VFfieldesize,ID_VFfieldisize,ID_VFfieldname,ID_VFfieldorder
      integer ID_VFfieldtype,ID_VFnfields

      integer ID_Vaddtagref,ID_Vattach, ID_Vdeletetagref

      integer ID_Vdetach,ID_Vgetclass,ID_Vgetid
      integer ID_Vgetname,ID_Vgettagref,ID_Vgettagrefs,ID_Vinqtagref
      integer ID_Vinquire,ID_Vinsert,ID_Vlone,ID_Vntagrefs,ID_Vsetclass
      integer ID_Vsetname, ID_Vinitialize, ID_Vfinish, ID_Vfind
      integer ID_Vfindclass, ID_vpackvs, ID_vunpackvs, ID_vpackvg
      integer ID_vunpackvg, ID_Vsetattr, ID_Vgetversion, ID_vnattrs
      integer ID_Vfindattr, ID_Vattrinfo, ID_Vgetattr

      integer ID_VHmakegroup,ID_VHstoredata,ID_VHstoredatam

      integer ID_VSattach,ID_VSdetach,ID_VSelts,ID_VSfdefine,ID_VSfexist
      integer ID_VSfind,ID_VSgetclass,ID_VSgetfields,ID_VSgetid
      integer ID_VSgetinterlace,ID_VSgetname,ID_VSinquire,ID_VSlone,ID_VSread
      integer ID_VSseek,ID_VSsetclass,ID_VSsetfields,ID_VSsetinterlace
      integer ID_VSsetname,ID_VSsizeof,ID_VSwrite, ID_VSappendable
      integer ID_VSdelete, ID_VSfindclass, ID_VSfpack, ID_VSfindex
      integer ID_VSsetattr, ID_VSnattrs, ID_VSfnattrs, ID_VSfindattr
      integer ID_VSattrinfo, ID_VSgetattr, ID_VSisattr

      integer ID_ANstart, ID_ANfileinfo, ID_ANend, ID_ANcreate, ID_ANcreatef
      integer ID_ANselect, ID_ANnumann, ID_ANannlist, ID_ANannlen
      integer ID_ANreadann, ID_ANwriteann, ID_ANendaccess

      integer ID_GRstart, ID_GRfileinfo, ID_GRend, ID_GRcreate, ID_GRselect
      integer ID_GRnametoindex, ID_GRgetiminfo, ID_GRwriteimage
      integer ID_GRreadimage, ID_GRendaccess, ID_GRgetdimid
      integer ID_GRsetdimname, ID_GRdiminfo, ID_GRidtoref, ID_GRreftoindex
      integer ID_GRreqlutil, ID_GRreqimageil, ID_GRgetlutid
      integer ID_GRgetlutinfo, ID_GRwritelut, ID_GRreadlut
      integer ID_GRsetexternalfile, ID_GRsetaccesstype, ID_GRsetcompress
      integer ID_GRsetattr, ID_GRattrinfo, ID_GRgetattr, ID_GRfindattr
      integer ID_GRluttoref, ID_GRsetchunk, ID_GRgetchunkinfo
      integer ID_GRsetchunkcache

      integer ID_HAinit_group, ID_HAdestroy_group, ID_HAregister_atom,
      integer ID_HAatom_object, ID_HAatom_group, ID_HAremove_atom 
      integer ID_HAsearch_atom

      integer ID_DAcreate_array, ID_DAdestroy_array, ID_DAsize_array, 
      integer ID_DAget_elem, ID_DAset_elem, ID_DAdel_elem 

      integer ID_HULcreate_list, ID_HULdestroy_list, ID_HULaddnode
      integer ID_HULsearch_node, ID_HULfirst_node, ID_HULnext_node
      integer ID_HULremove_node, ID_HDFnewentry

      parameter(DUMMY_HDF		= 1000,
     *          BEGIN_HDF		= 1001)
      parameter(ID_DFAN 		= BEGIN_HDF,
     *          ID_DFANaddfds           = ID_DFAN,
     *          ID_DFANaddfid           = ID_DFAN+1,
     *          ID_DFANgetdesc          = ID_DFAN+2,
     *          ID_DFANgetdesclen       = ID_DFAN+3,
     *          ID_DFANgetfds           = ID_DFAN+4,
     *          ID_DFANgetfdslen        = ID_DFAN+5,
     *          ID_DFANgetfid           = ID_DFAN+6,
     *          ID_DFANgetfidlen        = ID_DFAN+7,
     *          ID_DFANgetlabel         = ID_DFAN+8,
     *          ID_DFANgetlablen        = ID_DFAN+9,
     *          ID_DFANlablist          = ID_DFAN+10, 
     *          ID_DFANlastref          = ID_DFAN+11,
     *          ID_DFANputdesc          = ID_DFAN+12,
     *          ID_DFANputlabel         = ID_DFAN+13,
     *          ID_DFANclear            = ID_DFAN+14)

      parameter(ID_DFP  		= ID_DFAN+15,
     *          ID_DFPaddpal            = ID_DFP,
     *          ID_DFPgetpal            = ID_DFP+1,
     *          ID_DFPlastref           = ID_DFP+2,
     *          ID_DFPnpals             = ID_DFP+3,
     *          ID_DFPputpal            = ID_DFP+4,
     *          ID_DFPreadref           = ID_DFP+5,   
     *          ID_DFPrestart           = ID_DFP+6,   
     *          ID_DFPwriteref          = ID_DFP+7)

      parameter(ID_DFR8 		= ID_DFP+8,
     *          ID_DFR8addimage         = ID_DFR8,
     *          ID_DFR8getdims          = ID_DFR8+1,
     *          ID_DFR8getimage         = ID_DFR8+2,
     *          ID_DFR8lastref          = ID_DFR8+3,
     *          ID_DFR8nimages          = ID_DFR8+4,
     *          ID_DFR8putimage         = ID_DFR8+5,
     *          ID_DFR8readref          = ID_DFR8+6,
     *          ID_DFR8restart          = ID_DFR8+7,
     *          ID_DFR8setcompress      = ID_DFR8+8,
     *          ID_DFR8setpalette       = ID_DFR8+9,
     *          ID_DFR8writeref         = ID_DFR8+10,
     *          ID_DFR8Istart           = ID_DFR8+11)

      parameter(ID_DFSD                 = ID_DFR8+12,
     *          ID_DFSDadddata          = ID_DFSD,
     *          ID_DFSDclear            = ID_DFSD+1,
     *          ID_DFSDendslab          = ID_DFSD+2,
     *          ID_DFSDendslice         = ID_DFSD+3,
     *          ID_DFSDgetcal           = ID_DFSD+4,
     *          ID_DFSDgetdata          = ID_DFSD+5,
     *          ID_DFSDgetdatalen       = ID_DFSD+6,
     *          ID_DFSDgetdatastrs      = ID_DFSD+7,
     *          ID_DFSDgetdimlen        = ID_DFSD+8,
     *          ID_DFSDgetdims          = ID_DFSD+9,
     *          ID_DFSDgetdimscale      = ID_DFSD+10,
     *          ID_DFSDgetdimstrs       = ID_DFSD+11,
     *          ID_DFSDgetfillvalue     = ID_DFSD+12,
     *          ID_DFSDgetNT            = ID_DFSD+13,
     *          ID_DFSDgetrange         = ID_DFSD+14,
     *          ID_DFSDgetslice         = ID_DFSD+15,
     *          ID_DFSDlastref          = ID_DFSD+16,
     *          ID_DFSDndatasets        = ID_DFSD+17,
     *          ID_DFSDpre32sdg         = ID_DFSD+18,
     *          ID_DFSDputdata          = ID_DFSD+19,
     *          ID_DFSDputslice         = ID_DFSD+20,
     *          ID_DFSDreadref          = ID_DFSD+21,
     *          ID_DFSDreadslab         = ID_DFSD+22,
     *          ID_DFSDrestart          = ID_DFSD+23,
     *          ID_DFSDsetcal           = ID_DFSD+24,
     *          ID_DFSDsetdatastrs      = ID_DFSD+25,
     *          ID_DFSDsetdims          = ID_DFSD+26,
     *          ID_DFSDsetdimscale      = ID_DFSD+27,
     *          ID_DFSDsetdimstrs       = ID_DFSD+28,
     *          ID_DFSDsetfillvalue     = ID_DFSD+29,
     *          ID_DFSDsetlengths       = ID_DFSD+30,
     *          ID_DFSDsetNT            = ID_DFSD+31,
     *          ID_DFSDsetrange         = ID_DFSD+32,
     *          ID_DFSDstartslab        = ID_DFSD+33,
     *          ID_DFSDstartslice       = ID_DFSD+34,
     *          ID_DFSDwriteref         = ID_DFSD+35,
     *          ID_DFSDwriteslab        = ID_DFSD+36)

      parameter(ID_DF24                 = ID_DFSD+37,
     *          ID_DF24addimage         = ID_DF24,
     *          ID_DF24getdims          = ID_DF24+1,
     *          ID_DF24getimage         = ID_DF24+2,
     *          ID_DF24lastref          = ID_DF24+3,
     *          ID_DF24nimages          = ID_DF24+4,
     *          ID_DF24putimage         = ID_DF24+5,
     *          ID_DF24readref          = ID_DF24+6,
     *          ID_DF24reqil            = ID_DF24+7,
     *          ID_DF24restart          = ID_DF24+8,
     *          ID_DF24setcompress      = ID_DF24+9,
     *          ID_DF24setdims          = ID_DF24+10,
     *          ID_DF24setil            = ID_DF24+11)

      parameter(ID_H                    = ID_DF24+12,
     *          ID_Happendable          = ID_H
     *          ID_Hclose               = ID_H+1,
     *          ID_Hdeldd               = ID_H+2,
     *          ID_Hdupdd               = ID_H+3,
     *          ID_Hendaccess           = ID_H+4,
     *          ID_Hfind                = ID_H+5,
     *          ID_Hgetelement          = ID_H+6,
     *          ID_Hgetfileversion      = ID_H+7,
     *          ID_Hgetlibversion       = ID_H+8,
     *          ID_Hinquire             = ID_H+9,
     *          ID_Hishdf               = ID_H+10,
     *          ID_HLcreate             = ID_H+11,
     *          ID_Hlength              = ID_H+12,
     *          ID_Hnewref              = ID_H+13,
     *          ID_Hnextread            = ID_H+14,
     *          ID_Hnumber              = ID_H+15,
     *          ID_Hoffset              = ID_H+16,
     *          ID_Hopen                = ID_H+17,
     *          ID_Hputelement          = ID_H+18,
     *          ID_Hread                = ID_H+19,
     *          ID_Hseek                = ID_H+20,
     *          ID_Hstartread           = ID_H+21,
     *          ID_Hstartwrite          = ID_H+22,
     *          ID_Hsync                = ID_H+23,
     *          ID_Htrunc               = ID_H+24,
     *          ID_Hwrite               = ID_H+25,
     *          ID_HXcreate             = ID_H+26,
     *          ID_HXsetcreatedir       = ID_H+27,
     *          ID_HXsetdir             = ID_H+28,
     *          ID_HLconvert            = ID_H+29,
     *          ID_Hstartaccess         = ID_H+30,
     *          ID_Hisappendable        = ID_H+31,
     *          ID_Htell                = ID_H+32,
     *          ID_Htagnewref           = ID_H+33,
     *          ID_Hcache               = ID_H+34,
     *          ID_Hsetaccesstype       = ID_H+35,
     *          ID_Hexist               = ID_H+36,
     *          ID_Hsetlength           = ID_H+37,
     *          ID_Hfindinquire         = ID_H+38,
     *          ID_HDdont_atexit        = ID_H+39,
     *          ID_HDreuse_tagref       = ID_H+40,
     *          ID_HIstart              = ID_H+41,
     *          ID_HPregister_term_func  = ID_H+42,
     *          ID_HIbitstart           = ID_H+43,
     *          ID_HMCcreate            = ID_H+44,
     *          ID_HMCsetMaxcache       = ID_H+45,
     *          ID_HMCPstread           = ID_H+46,
     *          ID_HMCPstwrite          = ID_H+47,
     *          ID_HMCPseek             = ID_H+48,
     *          ID_HMCPchunkread        = ID_H+49,
     *          ID_HMCreadChunk         = ID_H+50,
     *          ID_HMCPread             = ID_H+51,
     *          ID_HMCPchunkwrite       = ID_H+52,
     *          ID_HMCwriteChunk        = ID_H+53,
     *          ID_HMCPwrite            = ID_H+54,
     *          ID_HMCPendaccess        = ID_H+55,
     *          ID_HMCPinfo             = ID_H+56,
     *          ID_HMCPinquire          = ID_H+57)

      parameter(ID_HE                   = ID_H+58,
     *          ID_HEclear              = ID_HE,
     *          ID_HEprint              = ID_HE+1,
     *          ID_HEpush               = ID_HE+2,
     *          ID_HEreport             = ID_HE+3,
     *          ID_HEstring             = ID_HE+4)

      parameter(ID_SD                   = ID_HE+5,
     *          ID_SDattrinfo           = ID_SD,
     *          ID_SDcreate             = ID_SD+1,
     *          ID_SDdiminfo            = ID_SD+2,
     *          ID_SDend                = ID_SD+3,
     *          ID_SDendaccess          = ID_SD+4,
     *          ID_SDfileinfo           = ID_SD+5,
     *          ID_SDfindattr           = ID_SD+6,
     *          ID_SDgetcal             = ID_SD+7,
     *          ID_SDgetdatastrs        = ID_SD+8,
     *          ID_SDgetdimid           = ID_SD+9,
     *          ID_SDgetdimscale        = ID_SD+10,
     *          ID_SDgetdimstrs         = ID_SD+11,
     *          ID_SDgetfillvalue       = ID_SD+12,
     *          ID_SDgetinfo            = ID_SD+13,
     *          ID_SDgetrange           = ID_SD+14,
     *          ID_SDidtoref            = ID_SD+15,
     *          ID_SDiscoordvar         = ID_SD+16,
     *          ID_SDnametoindex        = ID_SD+17,
     *          ID_SDreadattr           = ID_SD+18,
     *          ID_SDreaddata           = ID_SD+19,
     *          ID_SDreftoindex         = ID_SD+20,
     *          ID_SDselect             = ID_SD+21,
     *          ID_SDsetattr            = ID_SD+22,
     *          ID_SDsetcal             = ID_SD+23,
     *          ID_SDsetdatastrs        = ID_SD+24,
     *          ID_SDsetdimname         = ID_SD+25,
     *          ID_SDsetdimscale        = ID_SD+26,
     *          ID_SDsetdimstrs         = ID_SD+27,
     *          ID_SDsetexternalfile    = ID_SD+28,
     *          ID_SDsetfillvalue       = ID_SD+29,
     *          ID_SDsetrange           = ID_SD+30,
     *          ID_SDstart              = ID_SD+31,
     *          ID_SDwritedata          = ID_SD+32)

      parameter(ID_VF                   = ID_SD+33,
     *          ID_VFfieldesize         = ID_VF,
     *          ID_VFfieldisize         = ID_VF+1,
     *          ID_VFfieldname          = ID_VF+2,
     *          ID_VFfieldorder         = ID_VF+3,
     *          ID_VFfieldtype          = ID_VF+4,
     *          ID_VFnfields            = ID_VF+5)

      parameter(ID_V                    = ID_VF+6,
     *          ID_Vaddtagref           = ID_V,
     *          ID_Vattach              = ID_V+1,
     *		ID_Vdeletetagref	= ID_V+2
     *          ID_Vdetach              = ID_V+3,
     *          ID_Vgetclass            = ID_V+4,
     *          ID_Vgetid               = ID_V+5,
     *          ID_Vgetname             = ID_V+6,
     *          ID_Vgettagref           = ID_V+7,
     *          ID_Vgettagrefs          = ID_V+8,
     *          ID_Vinqtagref           = ID_V+9,
     *          ID_Vinquire             = ID_V+10,
     *          ID_Vinsert              = ID_V+11,
     *          ID_Vlone                = ID_V+12,
     *          ID_Vntagrefs            = ID_V+13,
     *          ID_Vsetclass            = ID_V+14,
     *          ID_Vsetname             = ID_V+15,
     *          ID_Vinitialize          = ID_V+16,
     *          ID_Vfinish              = ID_V+17,
     *          ID_Vfind                = ID_V+18,
     *          ID_Vfindclass           = ID_V+19,
     *          ID_vpackvs              = ID_V+20,
     *          ID_vunpackvs            = ID_V+21,
     *          ID_vpackvg              = ID_V+22,
     *          ID_vunpackvg            = ID_V+23,
     *          ID_Vsetattr             = ID_V+24,
     *          ID_Vgetversion          = ID_V+25,
     *          ID_Vnattrs              = ID_V+26,
     *          ID_Vfindattr            = ID_V+27,
     *          ID_Vattrinfo            = ID_V+28,
     *          ID_Vgetattr             = ID_V+29)

      parameter(ID_VH                   = ID_V+30,
     *          ID_VHmakegroup          = ID_VH,
     *          ID_VHstoredata          = ID_VH+1,
     *          ID_VHstoredatam         = ID_VH+2)

      parameter(ID_VS                   = ID_VH+3,
     *          ID_VSattach             = ID_VS,
     *          ID_VSdetach             = ID_VS+1,
     *          ID_VSelts               = ID_VS+2,
     *          ID_VSfdefine            = ID_VS+3,
     *          ID_VSfexist             = ID_VS+4,
     *          ID_VSfind               = ID_VS+5,
     *          ID_VSgetclass           = ID_VS+6,
     *          ID_VSgetfields          = ID_VS+7,
     *          ID_VSgetid              = ID_VS+8,
     *          ID_VSgetinterlace       = ID_VS+9,
     *          ID_VSgetname            = ID_VS+10,
     *          ID_VSinquire            = ID_VS+11,
     *          ID_VSlone               = ID_VS+12,
     *          ID_VSread               = ID_VS+13,
     *          ID_VSseek               = ID_VS+14,
     *          ID_VSsetclass           = ID_VS+15,
     *          ID_VSsetfields          = ID_VS+16,
     *          ID_VSsetinterlace       = ID_VS+17,
     *          ID_VSsetname            = ID_VS+18,
     *          ID_VSsizeof             = ID_VS+19,
     *          ID_VSwrite              = ID_VS+20,
     *          ID_VSappendable         = ID_VS+21,
     *          ID_VSdelete             = ID_VS+22,
     *          ID_VSfindclass          = ID_VS+23,
     *          ID_VSfpack              = ID_VS+24,
     *          ID_VSfindex             = ID_VS+25,
     *          ID_VSsetattr            = ID_VS+26,
     *          ID_VSnattrs             = ID_VS+27,
     *          ID_VSfnattrs            = ID_VS+28,
     *          ID_VSfindattr           = ID_VS+29,
     *          ID_VSattrinfo           = ID_VS+30,
     *          ID_VSgetattr            = ID_VS+31,
     *          ID_VSisattr             = ID_VS+32)

      parameter(ID_AN                   = ID_VS+33,
     *          ID_ANstart              = ID_AN+0,
     *          ID_ANfileinfo           = ID_AN+1,
     *          ID_ANend                = ID_AN+2,
     *          ID_ANcreate             = ID_AN+3,
     *          ID_ANcreatef            = ID_AN+4,
     *          ID_ANselect             = ID_AN+5,
     *          ID_ANnumann             = ID_AN+6,
     *          ID_ANannlist            = ID_AN+7,
     *          ID_ANannlen             = ID_AN+8,
     *          ID_ANreadann            = ID_AN+9,
     *          ID_ANwriteann           = ID_AN+10,
     *          ID_ANendaccess          = ID_AN+11)

      parameter(ID_GR                   = ID_AN+12,
     *          ID_GRstart              = ID_GR,
     *          ID_GRfileinfo           = ID_GR+1,
     *          ID_GRend                = ID_GR+2,
     *          ID_GRcreate             = ID_GR+3,
     *          ID_GRselect             = ID_GR+4,
     *          ID_GRnametoindex        = ID_GR+5,
     *          ID_GRgetiminfo          = ID_GR+6,
     *          ID_GRwriteimage         = ID_GR+7,
     *          ID_GRreadimage          = ID_GR+8,
     *          ID_GRendaccess          = ID_GR+9,
     *          ID_GRgetdimid           = ID_GR+10,
     *          ID_GRsetdimname         = ID_GR+11,
     *          ID_GRdiminfo            = ID_GR+12,
     *          ID_GRidtoref            = ID_GR+13,
     *          ID_GRreftoindex         = ID_GR+14,
     *          ID_GRreqlutil           = ID_GR+15,
     *          ID_GRreqimageil         = ID_GR+16,
     *          ID_GRgetlutid           = ID_GR+17,
     *          ID_GRgetlutinfo         = ID_GR+18,
     *          ID_GRwritelut           = ID_GR+19,
     *          ID_GRreadlut            = ID_GR+20,
     *          ID_GRsetexternalfile    = ID_GR+21,
     *          ID_GRsetaccesstype      = ID_GR+22,
     *          ID_GRsetcompress        = ID_GR+23,
     *          ID_GRsetattr            = ID_GR+24,
     *          ID_GRattrinfo           = ID_GR+25,
     *          ID_GRgetattr            = ID_GR+26,
     *          ID_GRfindattr           = ID_GR+27,
     *          ID_GRluttoref           = ID_GR+28,
     *	   	ID_GRsetchunk		= ID_GR+29, 
     *		ID_GRgetchunkinfo	= ID_GR+30,
     *		ID_GRsetchunkcache	= ID_GR+31)


      parameter(ID_HA                   = ID_GR+32,
     *          ID_HAinit_group         = ID_HA+0,
     *          ID_HAdestroy_group      = ID_HA+1,
     *          ID_HAregister_atom      = ID_HA+2,
     *          ID_HAatom_object        = ID_HA+3,
     *          ID_HAatom_group         = ID_HA+4,
     *          ID_HAremove_atom        = ID_HA+5,
     *          ID_HAsearch_atom        = ID_HA+6)

      parameter(ID_DA                   = ID_HA+7,
     *          ID_DAcreate_array       = ID_DA,
     *          ID_DAdestroy_array      = ID_DA+1,
     *          ID_DAsize_array         = ID_DA+2,
     *          ID_DAget_elem           = ID_DA+3,
     *          ID_DAset_elem           = ID_DA+4,
     *          ID_DAdel_elem           = ID_DA+5)

      parameter(ID_HUL                  = ID_DA+6,
     *          ID_HULcreate_list       = ID_HUL,
     *          ID_HULdestroy_list      = ID_HUL+1,
     *          ID_HULadd_node          = ID_HUL+2,
     *          ID_HULsearch_node       = ID_HUL+3,
     *          ID_HULfirst_node        = ID_HUL+4,
     *          ID_HULnext_node         = ID_HUL+5,
     *          ID_HULremove_node       = ID_HUL+6)

      parameter ( ID_HDFnewentry 	= ID_HUL+7,
		  ID_SDcheckempty	= ID_HUL+8)
