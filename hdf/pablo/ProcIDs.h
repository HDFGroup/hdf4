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
 * File:  ProcIDs.h
 * Purpose: define IDs for identifying procedures
 *-------------------------------------------------------------------------*/

#ifndef PROCIDS_H		/* avoid re-inclusion */
#define PROCIDS_H

#ifdef PABLO

extern uint16 procTrace;

#define TRACE_ON(mask, eventID) \
	if (procTrace & mask) traceEvent(eventID, NULL, 0)

#define TRACE_OFF(mask, eventID) \
	if (procTrace & mask) traceEvent(-eventID, NULL, 0)

#define DFAN_mask	0x1
#define ID_DFANaddfds		101
#define ID_DFANaddfid		102
#define ID_DFANgetdesc		103
#define ID_DFANgetdesclen	104
#define ID_DFANgetfds		105
#define ID_DFANgetfdslen	106
#define ID_DFANgetfid		107
#define ID_DFANgetfidlen	108
#define ID_DFANgetlabel		109
#define ID_DFANgetlablen	110
#define ID_DFANlablist		111
#define ID_DFANlastref		112
#define ID_DFANputdesc		113
#define ID_DFANputlabel		114

#define DFP_mask	0x2
#define ID_DFPaddpal		201
#define ID_DFPgetpal		202
#define ID_DFPlastref		203
#define ID_DFPnpals		204
#define ID_DFPputpal		205
#define ID_DFPreadref		206
#define ID_DFPrestart		207
#define ID_DFPwriteref		208

#define DFR8_mask	0x4
#define ID_DFR8addimage		301
#define ID_DFR8getdims		302
#define ID_DFR8getimage		303
#define ID_DFR8lastref		304
#define ID_DFR8nimages		305
#define ID_DFR8putimage		306
#define ID_DFR8readref		307
#define ID_DFR8restart		308
#define ID_DFR8setcompress	309
#define ID_DFR8setpalette	310
#define ID_DFR8writeref		311

#define DFSD_mask	0x8
#define ID_DFSDadddata		401
#define ID_DFSDclear		402
#define ID_DFSDendslab		403
#define ID_DFSDendslice		404
#define ID_DFSDgetcal		405
#define ID_DFSDgetdata		406
#define ID_DFSDgetdatalen	407
#define ID_DFSDgetdatastrs	408
#define ID_DFSDgetdimlen	409
#define ID_DFSDgetdims		410
#define ID_DFSDgetdimscale	411
#define ID_DFSDgetdimstrs	412
#define ID_DFSDgetfillvalue	413
#define ID_DFSDgetNT		414
#define ID_DFSDgetrange		415
#define ID_DFSDgetslice		416
#define ID_DFSDlastref		417
#define ID_DFSDndatasets	418
#define ID_DFSDpre32sdg		419
#define ID_DFSDputdata		420
#define ID_DFSDputslice		421
#define ID_DFSDreadref		422
#define ID_DFSDreadslab		423
#define ID_DFSDrestart		424
#define ID_DFSDsetcal		425
#define ID_DFSDsetdatastrs	426
#define ID_DFSDsetdims		427
#define ID_DFSDsetdimscale	428
#define ID_DFSDsetdimstrs	429
#define ID_DFSDsetfillvalue	430
#define ID_DFSDsetlengths	431
#define ID_DFSDsetNT		432
#define ID_DFSDsetrange		433
#define ID_DFSDstartslab	434
#define ID_DFSDstartslice	435
#define ID_DFSDwriteref		436
#define ID_DFSDwriteslab	437

#define DF24_mask	0x10
#define ID_DF24addimage		501
#define ID_DF24getdims		502
#define ID_DF24getimage		503
#define ID_DF24lastref		504
#define ID_DF24nimages		505
#define ID_DF24putimage		506
#define ID_DF24readref		507
#define ID_DF24reqil		508
#define ID_DF24restart		509
#define ID_DF24setcompress	510
#define ID_DF24setdims		511
#define ID_DF24setil		512
/* DF24writeref does not exist! */

#define H_mask		0x20
#define ID_Happendable		601
#define ID_Hclose		602
#define ID_Hdeldd		603
#define ID_Hdupdd		604
#define ID_Hendaccess		605
/* Hfidinquire does not exist! */
#define ID_Hfind		606
#define ID_Hgetelement		607
#define ID_Hgetfileversion	608
#define ID_Hgetlibversion	609
#define ID_Hinquire		610
#define ID_Hishdf		611
#define ID_HLcreate		612
#define ID_Hlength		613
#define ID_Hnewref		614
#define ID_Hnextread		615
#define ID_Hnumber		616
#define ID_Hoffset		617
#define ID_Hopen		618
#define ID_Hputelement		619
#define ID_Hread		620
#define ID_Hseek		621
#define ID_Hstartread		622
#define ID_Hstartwrite		623
#define ID_Hsync		624
#define ID_Htrunc		625
#define ID_Hwrite		626
#define ID_HXcreate		627

/* HDFclose and HDFopen are implemented as macros. */

#define HE_mask		0x40
#define ID_HEclear		701
#define ID_HEprint		702
#define ID_HEpush		703
#define ID_HEreport		704
#define ID_HEstring		705

/* SD functions are defined in mfhdf/libsrc/mfsd.c. */
#define SD_mask		0x80
#define ID_SDattrinfo		801
#define ID_SDcreate		802
#define ID_SDdiminfo		803
#define ID_SDend		804
#define ID_SDendaccess		805
#define ID_SDfileinfo		806
#define ID_SDfindattr		807
#define ID_SDgetcal		808
#define ID_SDgetdatastrs	809
#define ID_SDgetdimid		810
#define ID_SDgetdimscale	811
#define ID_SDgetdimstrs		812
#define ID_SDgetfillvalue	813
#define ID_SDgetinfo		814
#define ID_SDgetrange		815
#define ID_SDidtoref		816
#define ID_SDiscoordvar		817
#define ID_SDnametoindex	818
#define ID_SDreadattr		819
#define ID_SDreaddata		820
#define ID_SDreftoindex		821
#define ID_SDselect		822
#define ID_SDsetattr		823
#define ID_SDsetcal		824
#define ID_SDsetdatastrs	825
#define ID_SDsetdimname		826
#define ID_SDsetdimscale	827
#define ID_SDsetdimstrs		828
#define ID_SDsetexternalfile	829
#define ID_SDsetfillvalue	830
#define ID_SDsetrange		831
#define ID_SDstart		832
#define ID_SDwritedata		833

#define VF_mask		0x100
#define ID_VFfieldesize		901
#define ID_VFfieldisize		902
#define ID_VFfieldname		903
#define ID_VFfieldorder		904
#define ID_VFfieldtype		905
#define ID_VFnfields		906

#define V_mask		0x200
#define ID_Vaddtagref		1001
#define ID_Vattach		1002
#define ID_Vdetach		1003
/* Vend is implemented as a macro. */
#define ID_Vgetclass		1004
#define ID_Vgetid		1005
#define ID_Vgetname		1006
#define ID_Vgettagref		1007
#define ID_Vgettagrefs		1008
#define ID_Vinqtagref		1009
#define ID_Vinquire		1010
#define ID_Vinsert		1011
#define ID_Vlone		1012
#define ID_Vntagrefs		1013
#define ID_Vsetclass		1014
#define ID_Vsetname		1015
/* Vstart is implemented as a macro. */

#define VH_mask		0x400
#define ID_VHmakegroup		1101
#define ID_VHstoredata		1102
#define ID_VHstoredatam		1103

#define VS_mask		0x800
#define ID_VSattach		1201
#define ID_VSdetach		1202
#define ID_VSelts		1203
#define ID_VSfdefine		1204
#define ID_VSfexist		1205
#define ID_VSfind		1206
#define ID_VSgetclass		1207
#define ID_VSgetfields		1208
#define ID_VSgetid		1209
#define ID_VSgetinterlace	1210
#define ID_VSgetname		1211
#define ID_VSinquire		1212
#define ID_VSlone		1213
#define ID_VSread		1214
#define ID_VSseek		1215
#define ID_VSsetclass		1216
#define ID_VSsetfields		1217
#define ID_VSsetinterlace	1218
#define ID_VSsetname		1219
#define ID_VSsizeof		1220
#define ID_VSwrite		1221

#endif /* PABLO */

#endif /* PROCIDS_H */
