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
ARFLAGS=cr

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
     tbbt.h crle.h cnbit.h mstdio.h cnone.h

CSRCS=hfile.c herr.c hblocks.c hextelt.c hkit.c dfan.c dfr8.c dfcomp.c \
      dfrle.c dfimcomp.c dfjpeg.c dfunjpeg.c dfp.c dfgr.c df24.c dfsd.c \
      dfgroup.c dfconv.c dfufp2i.c vg.c vrw.c vio.c vparse.c vconv.c vsfld.c \
      vgp.c vhi.c dfutil.c dfstubs.c dfknat.c dfkswap.c dfkcray.c dfkvms.c \
      dfkconv.c dfkfuji.c hbitio.c tbbt.c hdfalloc.c hcomp.c crle.c cnbit.c \
      mstdio.c hbigext.c cnone.c

COBJS=hfile.o herr.o hblocks.o hextelt.o hkit.o dfan.o dfr8.o dfcomp.o \
      dfrle.o dfimcomp.o dfjpeg.o dfunjpeg.o dfp.o dfgr.o df24.o dfsd.o \
      dfgroup.o dfconv.o dfufp2i.o vg.o vrw.o vio.o vparse.o vconv.o vsfld.o \
      vgp.o vhi.o dfutil.o dfstubs.o dfknat.o dfkswap.o dfkcray.o dfkvms.o \
      dfkconv.o dfkfuji.o hbitio.o tbbt.o hdfalloc.o hcomp.o crle.o cnbit.o \
      mstdio.o hbigext.o cnone.o

FSRCS=hfilef.c herrf.c hfileff.f dfanf.c dfr8f.c dfpf.c df24f.c dfsdf.c \
      vgf.c dff.c dfanff.f df24ff.f dfpff.f dfr8ff.f dfsdff.f vgff.f dfff.f \
      dfutilf.c dfufp2if.f

FOBJS=hfilef.o herrf.o hfileff.o dfanf.o dfr8f.o dfpf.o df24f.o dfsdf.o \
      vgf.o dff.o dfanff.o df24ff.o dfpff.o dfr8ff.o dfsdff.o vgff.o dfff.o \
      dfutilf.o dfufp2if.o

#JFSRCS= jdmain.c jcmain.c jrdjfif.c jrdgif.c jrdppm.c jrdrle.c jrdtarga.c \
#        jwrjfif.c jwrgif.c jwrppm.c jwrrle.c jwrtarga.c
#
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

.PHONY : all nofortran libnostub clean distclean debug saber lint

all: libdf.a .installed

libdf.a: $(COBJS) $(FOBJS) $(JOBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a `lorder '$(COBJS) $(FOBJS) $(JOBJS)' | tsort `
	$(RANLIB) libdf.a

libnofortran: $(COBJS) $(JOBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a `lorder $(COBJS) $(JOBJS) | tsort `
	$(RANLIB) libdf.a

nofortran: libnofortran .finstalled

libnostub: nofortran

debug: libdf.a .installed

saber:
	#load -D$(MACHINE) $(CSRCS) $(TSRCS) $(JCSRCS) -lc

.installed: libdf.a
	-if (test \! -d $(HDFINC)) then (mkdir $(HDFINC) ) fi
	-cp $(HDRS) $(JINCS) $(HDFINC)
	-if (test \! -d $(HDFLIB)) then (mkdir $(HDFLIB) ) fi
	-cp libdf.a $(HDFLIB)
	-$(RANLIB) $(HDFLIB)/libdf.a
	touch .installed

.finstalled: libnofortran
	-if (test \! -d $(HDFINC)) then (mkdir $(HDFINC) ) fi
	-cp $(HDRS) $(JINCS) $(HDFINC)
	-if (test \! -d $(HDFLIB)) then (mkdir $(HDFLIB) ) fi
	-cp libdf.a $(HDFLIB)
	-$(RANLIB) $(HDFLIB)/libdf.a
	touch .finstalled

depend:
	makedepend -fMakefile -- $(JCSRCS) $(CSRCS)

clean:
	-$(RM) $(RMFLAGS) *.o

distclean:
	-$(RM) $(RMFLAGS) *.o libdf.a

lint: $(SRCS) $(R8SRCS)
	$(LINT) $(LINTFLAGS) $(SRCS)

TAGS: $(SRCS) $(HDRS)
	etags $(SRCS) $(HDRS)

#$(ARCHIVE): ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#       $(RM) $(RMFLAGS) $(ARCHIVE)
#       $(SHAR) $(ARCHIVE) ChangeLog Makefile $(SRCS) $(HDRS) \
#               $(TSRCS) $(USRCS)

#archive: $(ARCHIVE)

#backup: ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#       cp ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS) $(BDIR)

# DO NOT DELETE THIS LINE -- make depend depends on it.

jbsmooth.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jbsmooth.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcarith.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcarith.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jccolor.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jccolor.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcdeflts.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcdeflts.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcexpand.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcexpand.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jchuff.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jchuff.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcmaster.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcmaster.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcmcu.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
jcmcu.o: hproto.h vproto.h jconfig.h jpegdata.h
jcpipe.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcpipe.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jcsample.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jcsample.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdarith.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jdarith.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdcolor.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jdcolor.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jddeflts.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jddeflts.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdhuff.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jdhuff.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdmaster.o: jinclude.h hdf.h hdfi.h  hbitio.h hcomp.h
jdmaster.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdmcu.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
jdmcu.o: hproto.h vproto.h jconfig.h jpegdata.h
jdpipe.o: jinclude.h hdf.h hdfi.h  hbitio.h hcomp.h
jdpipe.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jdsample.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jdsample.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jerror.o: jinclude.h hdf.h hdfi.h  hbitio.h hcomp.h
jerror.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jquant1.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jquant1.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jquant2.o: jinclude.h hdf.h hdfi.h  hbitio.h hcomp.h
jquant2.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jfwddct.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jfwddct.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jrevdct.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jrevdct.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jutils.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jutils.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
jmemmgr.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jmemmgr.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h jmemsys.h
jmemsys.o: jinclude.h hdf.h hdfi.h hbitio.h hcomp.h
jmemsys.o: herr.h hproto.h vproto.h jconfig.h jpegdata.h
hfile.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hfile.o: vproto.h hfile.h
herr.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
herr.o: vproto.h 
hblocks.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hblocks.o: vproto.h hfile.h
hextelt.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hextelt.o: vproto.h hfile.h
hkit.o: hdf.h hdfi.h hbitio.h
hkit.o: hcomp.h herr.h hproto.h vproto.h hfile.h hkit.h
dfan.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfan.o: vproto.h hfile.h dfan.h
dfr8.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
dfr8.o: vproto.h dfrig.h
dfcomp.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
dfcomp.o: vproto.h
dfrle.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfrle.o: vproto.h
dfimcomp.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h
dfimcomp.o: hproto.h vproto.h
dfjpeg.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfjpeg.o: vproto.h jinclude.h jconfig.h jpegdata.h
dfunjpeg.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h
dfunjpeg.o: hproto.h vproto.h jinclude.h jconfig.h jpegdata.h
dfp.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfp.o: vproto.h hfile.h
dfgr.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfgr.o: vproto.h dfgr.h
df24.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
df24.o: vproto.h dfgr.h
dfsd.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfsd.o: vproto.h dfsd.h hfile.h
dfgroup.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfgroup.o: vproto.h hfile.h
dfconv.o: hdf.h hdfi.h hbitio.h
dfconv.o: hcomp.h herr.h hproto.h vproto.h hconv.h
dfufp2i.o: hdf.h hdfi.h hbitio.h
dfufp2i.o: hcomp.h herr.h hproto.h vproto.h dfufp2i.h dfsd.h
vg.o: vg.h hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
vg.o: vproto.h tbbt.h
vrw.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
vrw.o: vproto.h vg.h tbbt.h hfile.h
vio.o: vg.h hdf.h hdfi.h  hbitio.h hcomp.h herr.h
vio.o: hproto.h vproto.h tbbt.h hfile.h
vparse.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vparse.o: hproto.h vproto.h tbbt.h
vconv.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vconv.o: hproto.h vproto.h tbbt.h
vsfld.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vsfld.o: hproto.h vproto.h tbbt.h
vgp.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vgp.o: hproto.h vproto.h tbbt.h hfile.h
vhi.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vhi.o: hproto.h vproto.h tbbt.h
dfutil.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfutil.o: vproto.h
dfstubs.o: dfstubs.h df.h hdf.h hdfi.h hbitio.h hcomp.h
dfstubs.o: herr.h hproto.h vproto.h 
dfknat.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfknat.o: vproto.h hconv.h
dfkswap.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkswap.o: vproto.h hconv.h
dfkcray.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkcray.o: vproto.h hconv.h
dfkvms.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkvms.o: vproto.h hconv.h
dfkconv.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkconv.o: vproto.h hconv.h
dfkfuji.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkfuji.o: vproto.h hconv.h
hbitio.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hbitio.o: vproto.h hfile.h
tbbt.o:  hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
tbbt.o: vproto.h tbbt.h
hdfalloc.o: hdf.h hdfi.h  hbitio.h
hdfalloc.o: hcomp.h herr.h hproto.h vproto.h hfile.h
hcomp.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hcomp.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h
crle.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
crle.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h
cnbit.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cnbit.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h
mstdio.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
mstdio.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h
hbigext.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hbigext.o: vproto.h hfile.h
cnone.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cnone.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h
