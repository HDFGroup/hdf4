# ****************************************************************************
# * NCSA HDF                                                                 *
# * Software Development Group                                               *
# * National Center for Supercomputing Applications                          *
# * University of Illinois at Urbana-Champaign                               *
# * 605 E. Springfield, Champaign IL 61820                                   *
# *                                                                          *
# * For conditions of distribution and use, see the accompanying             *
# * hdf/COPYING file.                                                        *
# *                                                                          *
# ***************************************************************************
#
# $Id$
#
# ##################################################################
#
#           Part of MAKEFILE for creating the HDF library, libdf.a,
#
# This file contains the definitions of all the source code and headers
# used in the actual makefile.  They are located here for ease of maintainence.
#
# ##################################################################
#
#

HDRS=hfile.h hdf.h hdfi.h herr.h hproto.h dfan.h dfrig.h dfgr.h dfsd.h \
     vg.h df.h dfi.h dfstubs.h dfufp2i.h hcomp.h hconv.h hbitio.h \
     tbbt.h crle.h cnbit.h mstdio.h cnone.h cskphuff.h cdeflate.h mfgr.h \
     mfan.h hlimits.h hntdefs.h htags.h hkit.h hcompi.h bitvect.h atom.h \
     dynarray.h

CSRCS = $(HCSRCS) $(VCSRCS) $(DFCSRCS) $(MFCSRCS)
HCSRCS=hfile.c herr.c hblocks.c hextelt.c hkit.c hdfalloc.c hcomp.c \
    hbitio.c tbbt.c hdfalloc.c hcomp.c crle.c cnbit.c mstdio.c cnone.c \
    cskphuff.c cdeflate.c bitvect.c atom.c hfiledd.c dynarray.c 

VCSRCS=vg.c vrw.c vio.c vparse.c vconv.c vsfld.c vgp.c vhi.c 

DFCSRCS=dfan.c dfr8.c dfcomp.c dfrle.c dfimcomp.c dfjpeg.c dfunjpeg.c dfp.c \
    dfgr.c df24.c dfsd.c dfgroup.c dfconv.c dfufp2i.c dfutil.c dfstubs.c \
    dfknat.c dfkswap.c dfkcray.c dfkvms.c dfkconv.c dfkfuji.c 

MFCSRCS= mfan.c mfgr.c

COBJS = $(HCOBJS) $(VCOBJS) $(DFCOBJS) $(MFCOBJS)
HCOBJS=hfile.o herr.o hblocks.o hextelt.o hkit.o hdfalloc.o hcomp.o \
    hbitio.o tbbt.o crle.o cnbit.o mstdio.o cnone.o cskphuff.o cdeflate.o \
    bitvect.o atom.o hfiledd.o dynarray.o

VCOBJS=vg.o vrw.o vio.o vparse.o vconv.o vsfld.o vgp.o vhi.o 

DFCOBJS=dfan.o dfr8.o dfcomp.o dfrle.o dfimcomp.o dfjpeg.o dfunjpeg.o dfp.o \
    dfgr.o df24.o dfsd.o dfgroup.o dfconv.o dfufp2i.o dfutil.o dfstubs.o \
    dfknat.o dfkswap.o dfkcray.o dfkvms.o dfkconv.o dfkfuji.o 

MFCOBJS= mfan.o mfgr.o

FSRCS = $(FCSRCS) $(FFSRCS)
FCSRCS=hfilef.c herrf.c dfanf.c dfr8f.c dfpf.c df24f.c dfsdf.c vgf.c dff.c \
    dfutilf.c mfanf.c mfgrf.c hkitf.c

FFSRCS=hfileff.f dfanff.f df24ff.f dfpff.f dfr8ff.f dfsdff.f vgff.f dfff.f \
    dfufp2if.f mfgrff.f hkitff.f

FOBJS = $(FCOBJS) $(FFOBJS)
FCOBJS=hfilef.o herrf.o dfanf.o dfr8f.o dfpf.o df24f.o dfsdf.o vgf.o dff.o \
    dfutilf.o mfanf.o mfgrf.o hkitf.o

FFOBJS=hfileff.o dfanff.o df24ff.o dfpff.o dfr8ff.o dfsdff.o vgff.o dfff.o \
    dfufp2if.o mfgrff.o hkitff.o

FINCS=dffunc.inc hdf.inc


# Pablo Instrumentation Library support files
#PCHDRS=$(PABLO_SRC)/ProcIDs.h
#PCSRCS=$(PABLO_SRC)/PabloHDF.c
#PCOBJS=PabloHDF.o
#PFINCS=$(PABLO_SRC)/PabloHDF.inc
#PFSRCS=$(PABLO_SRC)/PabloHDFf.c $(PABLO_SRC)/PabloHDFff.f
#PFOBJS=PabloHDFf.o PabloHDFff.o
#POBJS=PabloHDF.o PabloHDFf.o PabloHDFff.o
#PSRCS= $(PCHDRS) $(PCSRCS) $(PFINCS) $(PFSRCS)

#JFSRCS= jdmain.c jcmain.c jrdjfif.c jrdgif.c jrdppm.c jrdrle.c jrdtarga.c \
#        jwrjfif.c jwrgif.c jwrppm.c jwrrle.c jwrtarga.c
#
#JCSRCS= jbsmooth.c jcarith.c jccolor.c jcdeflts.c jcexpand.c jchuff.c \
#        jcmaster.c jcmcu.c jcpipe.c jcsample.c jdarith.c jdcolor.c \
#        jddeflts.c jdhuff.c jdmaster.c jdmcu.c jdpipe.c jdsample.c \
#        jerror.c jquant1.c jquant2.c jfwddct.c jrevdct.c jutils.c jmemmgr.c \
#        jmemsys.c
#
#JINCS= jinclude.h jconfig.h jpegdata.h jversion.h jmemsys.h 
#
# objectfiles common to both JPEG compression and decompression
#COMOBJECTS= jutils.o jerror.o jmemmgr.o jmemsys.o 
# compression objectfiles
#CLIBOBJECTS= jcmaster.o jcdeflts.o jcarith.o jccolor.o jcexpand.o \
#        jchuff.o jcmcu.o jcpipe.o jcsample.o jfwddct.o
#        
#CFILEOBJECTS=jwrjfif.o jrdgif.o jrdppm.o jrdrle.o jrdtarga.o
#
# decompression objectfiles
#DLIBOBJECTS= jdmaster.o jddeflts.o jbsmooth.o jdarith.o jdcolor.o \
#        jdhuff.o jdmcu.o jdpipe.o jdsample.o jquant1.o \
#        jquant2.o jrevdct.o
#
#DFILEOBJECTS=jrdjfif.o jwrgif.o jwrppm.o jwrrle.o jwrtarga.o
#
# These objectfiles are included in libjpeg.lib
#JOBJS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)
#
