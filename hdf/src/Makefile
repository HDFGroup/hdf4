#
# $Header$
#
# $Log$
# Revision 1.1  1992/08/25 21:40:44  koziol
# Initial revision
#
#
# ##################################################################
#
#                   NCSA HDF Version 3.2 release Beta 1
#                               March 1992
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

# ==> For the IRIS and INDIGO, uncomment the following line
#CFLAGS=  -g -D${MACHINE} -ansi

# ==> For Convex, uncomment the following line
#CFLAGS= -g -D${MACHINE} -D__STDC__

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

# ==> The HP 9000 does not need ranlib. Uncomment the following line
#RANLIB=touch

RM=/bin/rm
RMFLAGS=-f
LINTFLAGS=
SHAR=shar
ARCHIVE=hdf.shar

HDRS=hfile.h hdf.h hdfi.h herr.h hproto.h dfan.h dfrig.h dfgr.h dfsd.h \
     dfgroup.h dfconvrt.h vg.h df.h dfi.h dfstubs.h dfufp2i.h vproto.h 

CSRCS=hfile.c herr.c hblocks.c hextelt.c hkit.c dfan.c dfr8.c dfcomp.c \
      dfimcomp.c dfp.c dfgr.c df24.c dfsd.c dfgroup.c dfconv.c dfufp2i.c  \
      vg.c vrw.c vio.c vparse.c vconv.c vsfld.c vgp.c vhi.c     \
      dfutil.c dfstubs.c

COBJS=hfile.o herr.o hblocks.o hextelt.o hkit.o dfan.o dfr8.o dfcomp.o \
      dfimcomp.o dfp.o dfgr.o df24.o dfsd.o dfgroup.o dfconv.o dfufp2i.o  \
      vg.o vrw.o vio.o vparse.o vconv.o vsfld.o vgp.o vhi.o     \
      dfutil.o dfstubs.o

FSRCS=hfilef.c herrf.c hfileff.f dfanf.c dfr8f.c dfpf.c df24f.c dfsdf.c \
      vgf.c dff.c dfanff.f df24ff.f dfpff.f dfr8ff.f dfsdff.f vgff.f dfff.f \
      dfutilf.c dfufp2if.f

FOBJS=hfilef.o herrf.o hfileff.o dfanf.o dfr8f.o dfpf.o df24f.o dfsdf.o \
      vgf.o dff.o dfanff.o df24ff.o dfpff.o dfr8ff.o dfsdff.o vgff.o dfff.o \
      dfutilf.o dfufp2if.o


.c.o:
	$(CC) $(CFLAGS) -c $<

.f.o:
	$(FC) $(FFLAGS) -c $<

all: libdf.a install_it

libdf.a: $(COBJS) $(FOBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a $(COBJS) $(FOBJS)
	$(RANLIB) libdf.a

libnofortran: $(COBJS)
	-$(RM) libdf.a
	$(AR) $(ARFLAGS) libdf.a $(COBJS)
	$(RANLIB) libdf.a

nofortran: libnofortran install_it

libnostub: nofortran

saber:
	#load -D$(MACHINE) $(CSRCS) $(TSRCS) -lc

hfile.o: hfile.h hdf.h herr.h hproto.h hdfi.h

herr.o: hdf.h herr.h hproto.h hdfi.h

hblocks.o: hfile.h hdf.h herr.h hproto.h hdfi.h

hextelt.o: hfile.h hdf.h herr.h hproto.h hdfi.h

hkit.o: hfile.h hdf.h herr.h hproto.h  hdfi.h

dfan.o: hdf.h herr.h dfan.h hproto.h hdfi.h hfile.h

dfanf.o: hdf.h herr.h dfan.h hproto.h hdfi.h

dfr8.o: hdf.h herr.h dfrig.h hproto.h hdfi.h

dfr8f.o: hdf.h herr.h dfrig.h hproto.h hdfi.h

dfcomp.o: hdf.h herr.h dfrig.h hproto.h hdfi.h

dfimcomp.o: hdf.h herr.h dfrig.h hproto.h hdfi.h

dfp.o: hdf.h herr.h hproto.h hdfi.h

dfpf.o: hdf.h herr.h hproto.h hdfi.h

dfgr.o: hdf.h herr.h dfgr.h hproto.h hdfi.h

df24.o: hdf.h herr.h dfgr.h hproto.h hdfi.h

df24f.o: hdf.h herr.h dfgr.h hproto.h hdfi.h

dfsd.o: hdf.h herr.h dfsd.h hproto.h hdfi.h dfconvrt.h dfgroup.h hfile.h

dfsdf.o: hdf.h herr.h dfsd.h hproto.h hdfi.h

dfgroup.o: hdf.h herr.h hfile.h hproto.h hdfi.h

dfconv.o: hdf.h herr.h hdfi.h

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

dfstubs.o: hdf.h herr.h dfstubs.h df.h dfi.h hproto.h hdfi.h

dff.o: hdf.h herr.h dfstubs.h df.h dfi.h hproto.h hdfi.h

install_it:
	-mkdir $(HDFINC)
	-cp $(HDRS) $(HDFINC)
	-mkdir $(HDFLIB)
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
 
 
