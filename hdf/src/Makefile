#
# $Header$
#
# $Log$
# Revision 1.14  1993/05/03 21:38:39  chouck
# Removed purify directive
#
# Revision 1.13  1993/05/03  21:32:01  koziol
# First half of fixes to make Purify happy
#
# Revision 1.12  1993/04/12  13:26:59  koziol
# Changed Macros for the JPEG stuff so the unused sources don't get
# included into Codecenter and have to be removed.  Also, added
# smarter "mkdir" lines so the installation won't try to re-create
# the directories if they exist already.
#
# Revision 1.11  1993/04/08  20:42:43  koziol
# Minor Cray tweaks
#
# Revision 1.10  1993/03/29  16:47:10  koziol
# Updated JPEG code to new JPEG 4 code.
# Changed VSets to use Threaded-Balanced-Binary Tree for internal
# 	(in memory) representation.
# Changed VGROUP * and VDATA * returns/parameters for all VSet functions
# 	to use 32-bit integer keys instead of pointers.
# Backed out speedups for Cray, until I get the time to fix them.
# Fixed a bunch of bugs in the little-endian support in DFSD.
#
# Revision 1.9  1993/02/16  19:19:37  koziol
# Deleted obsolete JPEG file from JINCS line
#
# Revision 1.8  1993/02/16  18:26:17  chouck
# Added JPeg files to 'make saber' option
#
# Revision 1.7  1993/01/26  19:42:14  koziol
# Added support for reading and writing Little-Endian data on all
# platforms.  This has been tested on: Cray, Sun, and PCs so far.
#
# Revision 1.6  1993/01/19  05:53:51  koziol
# Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
# port.  Lots of minor annoyances fixed.
#
# Revision 1.4  1992/11/02  18:07:57  chouck
# Added hcomp.h into the list of include files to move
#
# Revision 1.3  1992/11/02  16:35:41  koziol
# Updates from 3.2r2 -> 3.3
#
# Revision 1.2  1992/09/17  19:49:16  koziol
# Made changes for ANSI compilation on the Convex
#
# Revision 1.1  1992/08/25  21:40:44  koziol
# Initial revision
#
#
# ##################################################################
#
#                   NCSA HDF Version 3.3 release Beta 1
#                            January 1993
#
#           MAKEFILE for creating the HDF library, libdf.a,
#
# This makefile creates the following FORTRAN and C interfaces 
# for HDF in a library called "libdf.a": 
#
#              DFR8   (8-bit raster image sets)
#              DF24   (24-bit raster image sets)
#              DFP    (palettes)
#              DFSD   (scientific data sets)
#              DFAN   (annotations)
#              V      (vsets)
#
# See the file INSTALL for further information on how to invoke
# this makefile.
#
# This release of HDF marks the first use of a new set of lower
# level routines.  The makefile also creates these interfaces,
# which are available in C only.  These routines are categorized 
# as follows:
#
#              H      (new lower level i/o)
#              DF     (emulation of old lower level i/o routines)
#              HD     (lower level utilities for developers)
#              HE     (lower level error-handling)
#              HD     (lower level utilities, for developers)
#              DFK    (conversion routines)
#
# ##################################################################
#
#
# PORTING INSTRUCTIONS
# You must modify the lines marked "# ==>".
#
#
# These values will typically be over ridden by values passed 
#   down from the top level HDF makefile
#
#

# ==> specify your machine
MACHINE=SUN

# ==> specify directories in which to store HDF include files,
#     HDF library, and HDF utilities
HDFINC=../include
HDFLIB=../lib
HDFBIN=../bin

# ==> specify your FORTRAN compiler (Convex: fc)
FC=f77
FFLAGS= -g

# ==> specify your C compiler
CC=cc
CFLAGS=  -g -D${MACHINE}

# ==> For the ALPHA, uncomment the following line
#CFLAGS=  -g -DALPHA -std1

# ==> For the IRIS and INDIGO, and the NEXT, uncomment the following line
#CFLAGS=  -g -D${MACHINE} -ansi

# ==> where are your hdf.h, hdfi.h and vg.h files located?
#IFLAGS=   -I../h -I.

#BDIR=../backup

# ==> specify your archiver
AR=ar
ARFLAGS=r

# ==> specify your archive randomizer (if needed, see below)
RANLIB=ranlib

# ==> The IRIS does not have ranlib.  Instead, it uses the 's' flag to $(AR)
# ==> Uncomment the following lines for the IRIS or INDIGO platforms
#ARFLAGS=rs
#RANLIB=echo "AR substitied for RANLIB on library"

# ==> The HP 9000 and the Cray do not need ranlib. Uncomment the following line
#RANLIB=touch

RM=/bin/rm
RMFLAGS=-f
LINTFLAGS=
SHAR=shar
ARCHIVE=hdf.shar

HDRS=hfile.h hdf.h hdfi.h herr.h hproto.h dfan.h dfrig.h dfgr.h dfsd.h \
     vg.h df.h dfi.h dfstubs.h dfufp2i.h vproto.h hcomp.h hconv.h hbitio.h \
     tbbt.h

CSRCS=hfile.c herr.c hblocks.c hextelt.c hkit.c dfan.c dfr8.c dfcomp.c \
      dfrle.c dfimcomp.c dfjpeg.c dfunjpeg.c dfp.c dfgr.c df24.c dfsd.c \
      dfgroup.c dfconv.c dfufp2i.c vg.c vrw.c vio.c vparse.c vconv.c vsfld.c \
      vgp.c vhi.c dfutil.c dfstubs.c dfknat.c dfkswap.c dfkcray.c dfkvms.c \
      dfkconv.c dfkfuji.c hbitio.c tbbt.c

COBJS=hfile.o herr.o hblocks.o hextelt.o hkit.o dfan.o dfr8.o dfcomp.o \
      dfrle.o dfimcomp.o dfjpeg.o dfunjpeg.o dfp.o dfgr.o df24.o dfsd.o \
      dfgroup.o dfconv.o dfufp2i.o vg.o vrw.o vio.o vparse.o vconv.o vsfld.o \
      vgp.o vhi.o dfutil.o dfstubs.o dfknat.o dfkswap.o dfkcray.o dfkvms.o \
      dfkconv.o dfkfuji.o hbitio.o tbbt.o

FSRCS=hfilef.c herrf.c hfileff.f dfanf.c dfr8f.c dfpf.c df24f.c dfsdf.c \
      vgf.c dff.c dfanff.f df24ff.f dfpff.f dfr8ff.f dfsdff.f vgff.f dfff.f \
      dfutilf.c dfufp2if.f

FOBJS=hfilef.o herrf.o hfileff.o dfanf.o dfr8f.o dfpf.o df24f.o dfsdf.o \
      vgf.o dff.o dfanff.o df24ff.o dfpff.o dfr8ff.o dfsdff.o vgff.o dfff.o \
      dfutilf.o dfufp2if.o

JFSRCS= jdmain.c jcmain.c jrdjfif.c jrdgif.c jrdppm.c jrdrle.c jrdtarga.c \
        jwrjfif.c jwrgif.c jwrppm.c jwrrle.c jwrtarga.c

JCSRCS= jbsmooth.c jcarith.c jccolor.c jcdeflts.c jcexpand.c jchuff.c \
        jcmaster.c jcmcu.c jcpipe.c jcsample.c jdarith.c jdcolor.c \
        jddeflts.c jdhuff.c jdmaster.c jdmcu.c jdpipe.c jdsample.c \
        jerror.c jquant1.c jquant2.c jfwddct.c jrevdct.c jutils.c jmemmgr.c \
        jmemsys.c

JINCS= jinclude.h jconfig.h jpegdata.h jversion.h jmemsys.h 

# objectfiles common to both JPEG compression and decompression
COMOBJECTS= jutils.o jerror.o jmemmgr.o jmemsys.o 
# compression objectfiles
CLIBOBJECTS= jcmaster.o jcdeflts.o jcarith.o jccolor.o jcexpand.o \
        jchuff.o jcmcu.o jcpipe.o jcsample.o jfwddct.o
        
CFILEOBJECTS=jwrjfif.o jrdgif.o jrdppm.o jrdrle.o jrdtarga.o

# decompression objectfiles
DLIBOBJECTS= jdmaster.o jddeflts.o jbsmooth.o jdarith.o jdcolor.o \
        jdhuff.o jdmcu.o jdpipe.o jdsample.o jquant1.o \
        jquant2.o jrevdct.o

DFILEOBJECTS=jrdjfif.o jwrgif.o jwrppm.o jwrrle.o jwrtarga.o

# These objectfiles are included in libjpeg.lib
JOBJS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.f.o:
	$(FC) $(FFLAGS) -c $<

all: libdf.a install_it

libdf.a: $(COBJS) $(FOBJS) $(JOBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a $(COBJS) $(FOBJS) $(JOBJS)
	$(RANLIB) libdf.a

libnofortran: $(COBJS) $(JOBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a $(COBJS) $(JOBJS)
	$(RANLIB) libdf.a

nofortran: libnofortran install_it

libnostub: nofortran

saber:
	#load -D$(MACHINE) $(CSRCS) $(TSRCS) $(JCSRCS) -lc

hfile.o: hfile.h hdf.h herr.h hdfi.h

herr.o: hdf.h herr.h hdfi.h

hblocks.o: hfile.h hdf.h herr.h hdfi.h

hextelt.o: hfile.h hdf.h herr.h hdfi.h

hkit.o: hfile.h hdf.h herr.h hdfi.h

hbitio.o: hbitio.h hfile.h hdf.h herr.h hdfi.h

dfan.o: hdf.h herr.h dfan.h hdfi.h hfile.h

dfanf.o: hdf.h herr.h dfan.h hdfi.h

dfr8.o: hdf.h herr.h dfrig.h hdfi.h

dfr8f.o: hdf.h herr.h dfrig.h hdfi.h

dfcomp.o: hdf.h herr.h dfrig.h hdfi.h

dfimcomp.o: hdf.h herr.h dfrig.h hdfi.h

dfrle.o: dfrle.c hdf.h herr.h dfrig.h hdfi.h

dfjpeg.o: dfjpeg.c hdf.h herr.h dfrig.h hdfi.h

dfunjpeg.o: dfunjpeg.c hdf.h herr.h dfrig.h hdfi.h

dfp.o: hdf.h herr.h hdfi.h

dfpf.o: hdf.h herr.h hdfi.h

dfgr.o: hdf.h herr.h dfgr.h hdfi.h

df24.o: hdf.h herr.h dfgr.h hdfi.h

df24f.o: hdf.h herr.h dfgr.h hdfi.h

dfsd.o: hdf.h herr.h dfsd.h hdfi.h hfile.h

dfsdf.o: hdf.h herr.h dfsd.h hdfi.h

dfgroup.o: hdf.h herr.h hfile.h hdfi.h

dfconv.o: hdf.h herr.h hdfi.h hconv.h

dfknat.o: hdf.h herr.h hdfi.h hconv.h

dfkswap.o: hdf.h herr.h hdfi.h hconv.h

dfkcray.o: hdf.h herr.h hdfi.h hconv.h

dfkvms.o: hdf.h herr.h hdfi.h hconv.h

dfkconv.o: hdf.h herr.h hdfi.h hconv.h

dfkfuji.o: hdf.h herr.h hdfi.h hconv.h

dfutil.o: hdf.h herr.h hdfi.h

dfufp2i.o: hdf.h herr.h hdfi.h dfufp2i.h

vg.o: vg.h hdf.h hdfi.h

vgf.o: vg.h hdf.h hdfi.h

vrw.o: vg.h hdf.h hdfi.h

vio.o: vg.h hdf.h hdfi.h

vparse.o: vg.h hdf.h hdfi.h

vconv.o: vg.h hdf.h hdfi.h

vsfld.o: vg.h hdf.h hdfi.h

vgp.o: vg.h hdf.h hdfi.h hfile.h

vhi.o: vg.h hdf.h hdfi.h

dfstubs.o: hdf.h herr.h dfstubs.h df.h dfi.h hdfi.h

dff.o: hdf.h herr.h dfstubs.h df.h dfi.h hdfi.h

# JPEG source files
jbsmooth.o : jbsmooth.c jinclude.h jconfig.h jpegdata.h

jcarith.o : jcarith.c jinclude.h jconfig.h jpegdata.h

jccolor.o : jccolor.c jinclude.h jconfig.h jpegdata.h

jcdeflts.o : jcdeflts.c jinclude.h jconfig.h jpegdata.h

jcexpand.o : jcexpand.c jinclude.h jconfig.h jpegdata.h

jchuff.o : jchuff.c jinclude.h jconfig.h jpegdata.h

#jcmain.o : jcmain.c jinclude.h jconfig.h jpegdata.h jversion.h egetopt.c

jcmaster.o : jcmaster.c jinclude.h jconfig.h jpegdata.h

jcmcu.o : jcmcu.c jinclude.h jconfig.h jpegdata.h

jcpipe.o : jcpipe.c jinclude.h jconfig.h jpegdata.h

jcsample.o : jcsample.c jinclude.h jconfig.h jpegdata.h

jdarith.o : jdarith.c jinclude.h jconfig.h jpegdata.h

jdcolor.o : jdcolor.c jinclude.h jconfig.h jpegdata.h

jddeflts.o : jddeflts.c jinclude.h jconfig.h jpegdata.h

jdhuff.o : jdhuff.c jinclude.h jconfig.h jpegdata.h

#jdmain.o : jdmain.c jinclude.h jconfig.h jpegdata.h jversion.h egetopt.c

jdmaster.o : jdmaster.c jinclude.h jconfig.h jpegdata.h

jdmcu.o : jdmcu.c jinclude.h jconfig.h jpegdata.h

jdpipe.o : jdpipe.c jinclude.h jconfig.h jpegdata.h

jdsample.o : jdsample.c jinclude.h jconfig.h jpegdata.h

jerror.o : jerror.c jinclude.h jconfig.h jpegdata.h

jquant1.o : jquant1.c jinclude.h jconfig.h jpegdata.h

jquant2.o : jquant2.c jinclude.h jconfig.h jpegdata.h

jfwddct.o : jfwddct.c jinclude.h jconfig.h jpegdata.h

jrevdct.o : jrevdct.c jinclude.h jconfig.h jpegdata.h

jutils.o : jutils.c jinclude.h jconfig.h jpegdata.h

jmemmgr.o : jmemmgr.c jinclude.h jconfig.h jpegdata.h jmemsys.h

jrdjfif.o : jrdjfif.c jinclude.h jconfig.h jpegdata.h

jrdgif.o : jrdgif.c jinclude.h jconfig.h jpegdata.h

jrdppm.o : jrdppm.c jinclude.h jconfig.h jpegdata.h

jrdrle.o : jrdrle.c jinclude.h jconfig.h jpegdata.h

jrdtarga.o : jrdtarga.c jinclude.h jconfig.h jpegdata.h

jwrjfif.o : jwrjfif.c jinclude.h jconfig.h jpegdata.h

jwrgif.o : jwrgif.c jinclude.h jconfig.h jpegdata.h

jwrppm.o : jwrppm.c jinclude.h jconfig.h jpegdata.h

jwrrle.o : jwrrle.c jinclude.h jconfig.h jpegdata.h

jwrtarga.o : jwrtarga.c jinclude.h jconfig.h jpegdata.h

jmemsys.o : jmemsys.c jinclude.h jconfig.h jpegdata.h jmemsys.h

install_it:
	-if (test \! -d $(HDFINC)) then (mkdir $(HDFINC) ) fi
	-cp $(HDRS) $(JINCS) $(HDFINC)
	-if (test \! -d $(HDFLIB)) then (mkdir $(HDFLIB) ) fi
	-cp libdf.a $(HDFLIB)
	-$(RANLIB) $(HDFLIB)/libdf.a

clean:
	-$(RM) $(RMFLAGS) *.o

cleanup:
	-$(RM) $(RMFLAGS) libdf.a 

lint: $(SRCS) $(R8SRCS)
	$(LINT) $(LINTFLAGS) $(SRCS)

TAGS: $(SRCS) $(HDRS)
	etags $(SRCS) $(HDRS)

#$(ARCHIVE): ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#	$(RM) $(RMFLAGS) $(ARCHIVE)
#	$(SHAR) $(ARCHIVE) ChangeLog Makefile $(SRCS) $(HDRS) \
#		$(TSRCS) $(USRCS)

#archive: $(ARCHIVE)

#backup: ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#	cp ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS) $(BDIR)
 
 
