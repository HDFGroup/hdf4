C*  This file is part of the Pablo Performance Analysis Environment
C* 
C*           (R)
C*  The Pablo    Performance Analysis Environment software is NOT in
C*  the public domain.  However, it is freely available without fee for
C*  education, research, and non-profit purposes.  By obtaining copies
C*  of this and other files that comprise the Pablo Performance Analysis
C*  Environment, you, the Licensee, agree to abide by the following
C*  conditions and understandings with respect to the copyrighted software:
C*  
C*  1.  The software is copyrighted in the name of the Board of Trustees
C*      of the University of Illinois (UI), and ownership of the software
C*      remains with the UI. 
C* 
C*  2.  Permission to use, copy, and modify this software and its documentation
C*      for education, research, and non-profit purposes is hereby granted
C*      to Licensee, provided that the copyright notice, the original author's
C*      names and unit identification, and this permission notice appear on
C*      all such copies, and that no charge be made for such copies.  Any
C*      entity desiring permission to incorporate this software into commercial
C*      products should contact:
C* 
C*           Professor Daniel A. Reed                 reed@cs.uiuc.edu
C*           University of Illinois
C*           Department of Computer Science
C*           2413 Digital Computer Laboratory
C*           1304 West Springfield Avenue
C*           Urbana, Illinois  61801
C*           USA
C* 
C*  3.  Licensee may not use the name, logo, or any other symbol of the UI
C*      nor the names of any of its employees nor any adaptation thereof in
C*      advertizing or publicity pertaining to the software without specific
C*      prior written approval of the UI.
C* 
C*  4.  THE UI MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE
C*      SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS
C*      OR IMPLIED WARRANTY.
C* 
C*  5.  The UI shall not be liable for any damages suffered by Licensee from
C*      the use of this software.
C* 
C*  6.  The software was developed under agreements between the UI and the
C*      Federal Government which entitle the Government to certain rights.
C* 
C* *************************************************************************
C* 
C*  Developed by: The Pablo Research Group
C*                University of Illinois at Urbana-Champaign
C*                Department of Computer Science
C*                1304 W. Springfield Avenue
C*                Urbana, IL     61801
C* 
C*                http://www-pablo.cs.uiuc.edu
C* 
C*  Send comments to: pablo-feedback@guitar.cs.uiuc.edu
C* 
C*  Copyright (c) 1987-1998
C*  The University of Illinois Board of Trustees.
C*       All Rights Reserved.
C* 
C*  PABLO is a registered trademark of
C*  The Board of Trustees of the University of Illinois
C*  registered in the U.S. Patent and Trademark Office.
C* 
C*  Project Manager and Principal Investigator:
C*       Daniel A. Reed (reed@cs.uiuc.edu)
C* 
C* Funded in part by the Defense Advanced Research Projects Agency under 
C* DARPA contracts DABT63-94-C0049 (SIO Initiative), F30602-96-C-0161,
C* and DABT63-96-C-0027 by the National Science Foundation under the PACI 
C* program and grants NSF CDA 94-01124 and ASC 97-20202, and by the 
C* Department of Energy under contracts DOE B-341494, W-7405-ENG-48, and 
C* 1-B-333164.
C--------------------------------------------------------------------------
C* File:  ProcMasks.inc
C* Purpose: define bit masks for tracing families of procedures
C*-------------------------------------------------------------------------

C* Each mask is a single bit identifying a family of procedures.  To
C* enable procedure tracing on specific procedure families, specify
C* the logical-or (|) of the desired masks in the procTraceMask
C* parameter of the HDFinitIOTrace function. 

	integer DFANmask, DFPmask, DFR8mask, DFSDmask, DF24mask,
     1          Hmask, HEmask, SDmask, VFmask, Vmask, VHmask, 
     2 		VSmask, ANmask, GRmask, HAmask, DAmask, HUlmask,
     3		allhdfmask
	parameter ( DFANmask = 1,   DFPmask = 2,   DFR8mask = 4,
     1              DFSDmask = 8,   DF24mask = 16, Hmask = 32,
     2		    HEmask = 64,    SDmask = 128,  VFmask = 256,
     3 		    Vmask = 512,    VHmask = 1024, VSmask = 2048,
     4		    ANmask = 4096,  GRmask = 8192, HAmask = 16384,
     5		    DAmask = 32768, HULmask = 65536, 
     6              AllHDFmask = 131071 )
