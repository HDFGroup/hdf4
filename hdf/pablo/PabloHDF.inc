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
      integer ID_HDdont_atexit, ID_HIstart, ID_HPregister_term_func
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
      integer ID_SDwritedata

      integer ID_VFfieldesize,ID_VFfieldisize,ID_VFfieldname,ID_VFfieldorder
      integer ID_VFfieldtype,ID_VFnfields

      integer ID_Vaddtagref,ID_Vattach,ID_Vdetach,ID_Vgetclass,ID_Vgetid
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
      integer ID_GRluttoref

      integer ID_HAinit_group, ID_HAdestroy_group, ID_HAregister_atom,
      integer ID_HAatom_object, ID_HAatom_group, ID_HAremove_atom 
      integer ID_HAsearch_atom

      integer ID_DAcreate_array, ID_DAdestroy_array, ID_DAsize_array, 
      integer ID_DAget_elem, ID_DAset_elem, ID_DAdel_elem 

      integer ID_HULcreate_list, ID_HULdestroy_list, ID_HULaddnode
      integer ID_HULsearch_node, ID_HULfirst_node, ID_HULnext_node
      integer ID_remove_node

      parameter(ID_DFANaddfds           = 101,
     *          ID_DFANaddfid           = 102,
     *          ID_DFANgetdesc          = 103,
     *          ID_DFANgetdesclen       = 104,
     *          ID_DFANgetfds           = 105,
     *          ID_DFANgetfdslen        = 106,
     *          ID_DFANgetfid           = 107,
     *          ID_DFANgetfidlen        = 108,
     *          ID_DFANgetlabel         = 109,
     *          ID_DFANgetlablen        = 110,
     *          ID_DFANlablist          = 111, 
     *          ID_DFANlastref          = 112,
     *          ID_DFANputdesc          = 113,
     *          ID_DFANputlabel         = 114,
     *          ID_DFANclear            = 115)

      parameter(ID_DFPaddpal            = 201,
     *          ID_DFPgetpal            = 202,
     *          ID_DFPlastref           = 203,
     *          ID_DFPnpals             = 204,
     *          ID_DFPputpal            = 205,
     *          ID_DFPreadref           = 206,
     *          ID_DFPrestart           = 207,
     *          ID_DFPwriteref          = 208)

      parameter(ID_DFR8addimage         = 301,
     *          ID_DFR8getdims          = 302,
     *          ID_DFR8getimage         = 303,
     *          ID_DFR8lastref          = 304,
     *          ID_DFR8nimages          = 305,
     *          ID_DFR8putimage         = 306,
     *          ID_DFR8readref          = 307,
     *          ID_DFR8restart          = 308,
     *          ID_DFR8setcompress      = 309,
     *          ID_DFR8setpalette       = 310,
     *          ID_DFR8writeref         = 311,
     *          ID_DFR8Istart           = 312)

      parameter(ID_DFSDadddata          = 401,
     *          ID_DFSDclear            = 402,
     *          ID_DFSDendslab          = 403,
     *          ID_DFSDendslice         = 404,
     *          ID_DFSDgetcal           = 405,
     *          ID_DFSDgetdata          = 406,
     *          ID_DFSDgetdatalen       = 407,
     *          ID_DFSDgetdatastrs      = 408,
     *          ID_DFSDgetdimlen        = 409,
     *          ID_DFSDgetdims          = 410,
     *          ID_DFSDgetdimscale      = 411,
     *          ID_DFSDgetdimstrs       = 412,
     *          ID_DFSDgetfillvalue     = 413,
     *          ID_DFSDgetNT            = 414,
     *          ID_DFSDgetrange         = 415,
     *          ID_DFSDgetslice         = 416,
     *          ID_DFSDlastref          = 417,
     *          ID_DFSDndatasets        = 418,
     *          ID_DFSDpre32sdg         = 419,
     *          ID_DFSDputdata          = 420,
     *          ID_DFSDputslice         = 421,
     *          ID_DFSDreadref          = 422,
     *          ID_DFSDreadslab         = 423,
     *          ID_DFSDrestart          = 424,
     *          ID_DFSDsetcal           = 425,
     *          ID_DFSDsetdatastrs      = 426,
     *          ID_DFSDsetdims          = 427,
     *          ID_DFSDsetdimscale      = 428,
     *          ID_DFSDsetdimstrs       = 429,
     *          ID_DFSDsetfillvalue     = 430,
     *          ID_DFSDsetlengths       = 431,
     *          ID_DFSDsetNT            = 432,
     *          ID_DFSDsetrange         = 433,
     *          ID_DFSDstartslab        = 434,
     *          ID_DFSDstartslice       = 435,
     *          ID_DFSDwriteref         = 436,
     *          ID_DFSDwriteslab        = 437)

      parameter(ID_DF24addimage         = 501,
     *          ID_DF24getdims          = 502,
     *          ID_DF24getimage         = 503,
     *          ID_DF24lastref          = 504,
     *          ID_DF24nimages          = 505,
     *          ID_DF24putimage         = 506,
     *          ID_DF24readref          = 507,
     *          ID_DF24reqil            = 508,
     *          ID_DF24restart          = 509,
     *          ID_DF24setcompress      = 510,
     *          ID_DF24setdims          = 511,
     *          ID_DF24setil            = 512)

      parameter(ID_Happendable          = 601,
     *          ID_Hclose               = 602,
     *          ID_Hdeldd               = 603,
     *          ID_Hdupdd               = 604,
     *          ID_Hendaccess           = 605,
     *          ID_Hfind                = 606,
     *          ID_Hgetelement          = 607,
     *          ID_Hgetfileversion      = 608,
     *          ID_Hgetlibversion       = 609,
     *          ID_Hinquire             = 610,
     *          ID_Hishdf               = 611,
     *          ID_HLcreate             = 612,
     *          ID_Hlength              = 613,
     *          ID_Hnewref              = 614,
     *          ID_Hnextread            = 615,
     *          ID_Hnumber              = 616,
     *          ID_Hoffset              = 617,
     *          ID_Hopen                = 618,
     *          ID_Hputelement          = 619,
     *          ID_Hread                = 620,
     *          ID_Hseek                = 621,
     *          ID_Hstartread           = 622,
     *          ID_Hstartwrite          = 623,
     *          ID_Hsync                = 624,
     *          ID_Htrunc               = 625,
     *          ID_Hwrite               = 626,
     *          ID_HXcreate             = 627,
     *          ID_HXsetcreatedir       = 628,
     *          ID_HXsetdir             = 629,
     *          ID_HLconvert            = 630,
     *          ID_Hstartaccess         = 631,
     *          ID_Hisappendable        = 632,
     *          ID_Htell                = 633,
     *          ID_Htagnewref           = 634,
     *          ID_Hcache               = 635,
     *          ID_Hsetaccesstype       = 636,
     *          ID_Hexist               = 637,
     *          ID_Hsetlength           = 638,
     *          ID_Hfindinquire         = 639,
     *          ID_HDdont_atexit        = 640,
     *          ID_HIstart              = 641,
     *          ID_HPregister_term_func  = 642,
     *          ID_HIbitstart           = 643,
     *          ID_HMCcreate            = 644,
     *          ID_HMCsetMaxcache       = 645,
     *          ID_HMCPstread           = 646,
     *          ID_HMCPstwrite          = 647,
     *          ID_HMCPseek             = 648,
     *          ID_HMCPchunkread        = 649,
     *          ID_HMCreadChunk         = 650,
     *          ID_HMCPread             = 651,
     *          ID_HMCPchunkwrite       = 652,
     *          ID_HMCwriteChunk        = 653,
     *          ID_HMCPwrite            = 654,
     *          ID_HMCPendaccess        = 655,
     *          ID_HMCPinfo             = 656,
     *          ID_HMCPinquire          = 657)

      parameter(ID_HEclear              = 701,
     *          ID_HEprint              = 702,
     *          ID_HEpush               = 703,
     *          ID_HEreport             = 704,
     *          ID_HEstring             = 705)

      parameter(ID_SDattrinfo           = 801,
     *          ID_SDcreate             = 802,
     *          ID_SDdiminfo            = 803,
     *          ID_SDend                = 804,
     *          ID_SDendaccess          = 805,
     *          ID_SDfileinfo           = 806,
     *          ID_SDfindattr           = 807,
     *          ID_SDgetcal             = 808,
     *          ID_SDgetdatastrs        = 809,
     *          ID_SDgetdimid           = 810,
     *          ID_SDgetdimscale        = 811,
     *          ID_SDgetdimstrs         = 812,
     *          ID_SDgetfillvalue       = 813,
     *          ID_SDgetinfo            = 814,
     *          ID_SDgetrange           = 815,
     *          ID_SDidtoref            = 816,
     *          ID_SDiscoordvar         = 817,
     *          ID_SDnametoindex        = 818,
     *          ID_SDreadattr           = 819,
     *          ID_SDreaddata           = 820,
     *          ID_SDreftoindex         = 821,
     *          ID_SDselect             = 822,
     *          ID_SDsetattr            = 823,
     *          ID_SDsetcal             = 824,
     *          ID_SDsetdatastrs        = 825,
     *          ID_SDsetdimname         = 826,
     *          ID_SDsetdimscale        = 827,
     *          ID_SDsetdimstrs         = 828,
     *          ID_SDsetexternalfile    = 829,
     *          ID_SDsetfillvalue       = 830,
     *          ID_SDsetrange           = 831,
     *          ID_SDstart              = 832,
     *          ID_SDwritedata          = 833)

      parameter(ID_VFfieldesize         = 901,
     *          ID_VFfieldisize         = 902,
     *          ID_VFfieldname          = 903,
     *          ID_VFfieldorder         = 904,
     *          ID_VFfieldtype          = 905,
     *          ID_VFnfields            = 906)

      parameter(ID_Vaddtagref           = 1001,
     *          ID_Vattach              = 1002,
     *          ID_Vdetach              = 1003,
     *          ID_Vgetclass            = 1004,
     *          ID_Vgetid               = 1005,
     *          ID_Vgetname             = 1006,
     *          ID_Vgettagref           = 1007,
     *          ID_Vgettagrefs          = 1008,
     *          ID_Vinqtagref           = 1009,
     *          ID_Vinquire             = 1010,
     *          ID_Vinsert              = 1011,
     *          ID_Vlone                = 1012,
     *          ID_Vntagrefs            = 1013,
     *          ID_Vsetclass            = 1014,
     *          ID_Vsetname             = 1015,
     *          ID_Vinitialize          = 1016,
     *          ID_Vfinish              = 1017,
     *          ID_Vfind                = 1018,
     *          ID_Vfindclass           = 1019,
     *          ID_vpackvs              = 1020,
     *          ID_vunpackvs            = 1021,
     *          ID_vpackvg              = 1022,
     *          ID_vunpackvg            = 1023,
     *          ID_Vsetattr             = 1024,
     *          ID_Vgetversion          = 1025,
     *          ID_Vnattrs              = 1026,
     *          ID_Vfindattr            = 1027,
     *          ID_Vattrinfo            = 1028,
     *          ID_Vgetattr             = 1029)

      parameter(ID_VHmakegroup          = 1101,
     *          ID_VHstoredata          = 1102,
     *          ID_VHstoredatam         = 1103)

      parameter(ID_VSattach             = 1201,
     *          ID_VSdetach             = 1202,
     *          ID_VSelts               = 1203,
     *          ID_VSfdefine            = 1204,
     *          ID_VSfexist             = 1205,
     *          ID_VSfind               = 1206,
     *          ID_VSgetclass           = 1207,
     *          ID_VSgetfields          = 1208,
     *          ID_VSgetid              = 1209,
     *          ID_VSgetinterlace       = 1210,
     *          ID_VSgetname            = 1211,
     *          ID_VSinquire            = 1212,
     *          ID_VSlone               = 1213,
     *          ID_VSread               = 1214,
     *          ID_VSseek               = 1215,
     *          ID_VSsetclass           = 1216,
     *          ID_VSsetfields          = 1217,
     *          ID_VSsetinterlace       = 1218,
     *          ID_VSsetname            = 1219,
     *          ID_VSsizeof             = 1220,
     *          ID_VSwrite              = 1221,
     *          ID_VSappendable         = 1222,
     *          ID_VSdelete             = 1223,
     *          ID_VSfindclass          = 1224,
     *          ID_VSfpack              = 1225,
     *          ID_VSfindex             = 1226,
     *          ID_VSsetattr            = 1227,
     *          ID_VSnattrs             = 1228,
     *          ID_VSfnattrs            = 1229,
     *          ID_VSfindattr           = 1230,
     *          ID_VSattrinfo           = 1231,
     *          ID_VSgetattr            = 1232,
     *          ID_VSisattr             = 1233)

      parameter(ID_ANstart              = 1300,
     *          ID_ANfileinfo           = 1301,
     *          ID_ANend                = 1302,
     *          ID_ANcreate             = 1303,
     *          ID_ANcreatef            = 1304,
     *          ID_ANselect             = 1305,
     *          ID_ANnumann             = 1306,
     *          ID_ANannlist            = 1307,
     *          ID_ANannlen             = 1308,
     *          ID_ANreadann            = 1309,
     *          ID_ANwriteann           = 1310,
     *          ID_ANendaccess          = 1311)

      parameter(ID_GRstart              = 1400,
     *          ID_GRfileinfo           = 1401,
     *          ID_GRend                = 1402,
     *          ID_GRcreate             = 1403,
     *          ID_GRselect             = 1404,
     *          ID_GRnametoindex        = 1405,
     *          ID_GRgetiminfo          = 1406,
     *          ID_GRwriteimage         = 1407,
     *          ID_GRreadimage          = 1408,
     *          ID_GRendaccess          = 1409,
     *          ID_GRgetdimid           = 1410,
     *          ID_GRsetdimname         = 1411,
     *          ID_GRdiminfo            = 1412,
     *          ID_GRidtoref            = 1413,
     *          ID_GRreftoindex         = 1414,
     *          ID_GRreqlutil           = 1415,
     *          ID_GRreqimageil         = 1416,
     *          ID_GRgetlutid           = 1417,
     *          ID_GRgetlutinfo         = 1418,
     *          ID_GRwritelut           = 1419,
     *          ID_GRreadlut            = 1420,
     *          ID_GRsetexternalfile    = 1421,
     *          ID_GRsetaccesstype      = 1422,
     *          ID_GRsetcompress        = 1423,
     *          ID_GRsetattr            = 1424,
     *          ID_GRattrinfo           = 1425,
     *          ID_GRgetattr            = 1426,
     *          ID_GRfindattr           = 1427,
     *          ID_GRluttoref           = 1428)

      parameter(ID_HAinit_group         = 1501,
     *          ID_HAdestroy_group      = 1502,
     *          ID_HAregister_atom      = 1503,
     *          ID_HAatom_object        = 1504,
     *          ID_HAatom_group         = 1505,
     *          ID_HAremove_atom        = 1506,
     *          ID_HAsearch_atom        = 1507)

      parameter(ID_DAcreate_array       = 1601,
     *          ID_DAdestroy_array      = 1602,
     *          ID_DAsize_array         = 1603,
     *          ID_DAget_elem           = 1604,
     *          ID_DAset_elem           = 1605,
     *          ID_DAdel_elem           = 1606)

      parameter(ID_HULcreate_list       = 10001,
     *          ID_HULdestroy_list      = 10002,
     *          ID_HULadd_node          = 10003,
     *          ID_HULsearch_node       = 10004,
     *          ID_HULfirst_node        = 10005,
     *          ID_HULnext_node         = 10006,
     *          ID_HULremove_node       = 10007)