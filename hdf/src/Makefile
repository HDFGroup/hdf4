# This file was generated automatically by configure.  Do not edit.
VPATH = .
links = 
host_alias = mips-sgi-irix5.3
host_cpu = mips
host_vendor = sgi
host_os = irix5.3
host_canonical = mips-sgi-irix5.3
target_alias = mips-sgi-irix5.3
target_cpu = mips
target_vendor = sgi
target_os = irix5.3
target_canonical = mips-sgi-irix5.3
host_makefile_frag = ../../config/mh-irix5
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
### Start of system configuration section. ####
# Set shell so we know what it is
SHELL = /bin/sh

# 'srcdir' Gets set to directory this Makefile is found in
srcdir = .
TOP_SRCDIR = ../..

prefix = /usr/local

exec_prefix = $(prefix)
tooldir = $(exec_prefix)/mips-sgi-irix5.3

program_transform_name = 

datadir = $(prefix)/lib
mandir = $(prefix)/man
man1dir = $(mandir)/man1
man2dir = $(mandir)/man2
man3dir = $(mandir)/man3
man4dir = $(mandir)/man4
man5dir = $(mandir)/man5
man6dir = $(mandir)/man6
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8
man9dir = $(mandir)/man9
docdir = $(datadir)/doc

INSTALL = ${TOP_SRCDIR}/install-sh -c
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# /usr/local/bin/m4 (GNU version?)
M4   = /usr/local/bin/m4
# /usr/local/bin/perl (GNU version?)
PERL = /usr/local/bin/perl

# Name of achive randomizer, usually ranlib (use 'touch' if non-existant)
RANLIB = ranlib

# Name of library archiver and flags to send, default 'AR=ar', 'ARFLAGS=r'
AR = ar
ARFLAGS = r

# Name of remove utility, default 'RM=/bin/rm', 'RMFLAGS=-f'
RM = /bin/rm
RMFLAGS = -f

#Compilier defaults overriden by top level make
AS = as
CC = gcc
CFLAGS = -ansi
FC = f77
FFLAGS=

# Set for document generation
MAKEINFO = makeinfo
TEXI2DVI = texi2dvi

# Directory in which to install scripts.
bindir = $(exec_prefix)/bin

# Directory in which to install library files.
libdir = $(prefix)/lib

# Directory in which to install include files.
includedir = $(prefix)/include

# Directory in which to install documentation info files.
infodir = $(prefix)/info

# Cstar objects for CM5 parallel I/O
# CMOBJS is set in the makefile fragments section
CMSRCS=cmextelt.cs
CMOBJS=

TARGETLIB = libdf.a

#### ----- host and target specific makefile fragments come in here.-------
# You can override the following variables here
#
# Compiliers:
# For gcc version
#CC=gcc
#CFLAGS=-ansi 
#
#Using ANSI compilier
CC=cc
CFLAGS=-ansi

# Fortran compilier
FC = f77
FFLAGS =

# Name of achive randomizer, usually ranlib (use 'touch' if non-existant)
RANLIB = ranlib

# Name of library archiver and flags to send, default 'AR=ar', 'ARFLAGS=r'
AR = ar
ARFLAGS = rs

# Name of remove utility, default 'RM=/bin/rm', 'RMFLAGS=-f'
RM = /bin/rm
RMFLAGS = -f
### ------------------- end of makefile fragments -------------------------

# ############# End of system configuration section. ###############

# ==> specify directories where to find inludes and library
#     HDF library, and HDF utilities
HDFINC= $(srcdir)/../src
HDFLIB= $(srcdir)/../src
HDFBIN= $(srcdir)/../bin

# ==> specify where to find the JPEG include files and library
JPEGINC=$(srcdir)/../jpeg
JPEGLIB=$(srcdir)/../jpeg

ALL_CFLAGS = -I. $(CFLAGS) -I$(JPEGINC)
LINTFLAGS=
SHAR=shar
ARCHIVE=hdf.shar

#
#include the source file definitions
#
include src.inc

HINCS = $(HDRS) $(FINCS)

#.SUFFIXES:
.SUFFIXES: .c.o .f.o .installed .finstalled

# This rule causes ansi2knr to be invoked.
#@ISANSICOM@.c.o:
#@ISANSICOM@	./ansi2knr $(srcdir)/$*.c T$*.c
#@ISANSICOM@	$(CC) $(CFLAGS) -c T$*.c
#@ISANSICOM@	$(RM) $(RMFLAGS) T$*.c $*.o
#@ISANSICOM@	$(MV) T$*.o $*.o

#ansi2knr: ansi2knr.c
#	$(CC) $(CFLAGS) $(ANSI2KNRFLAGS) -o ansi2knr ansi2knr.c

.c.o:
	$(CC) $(CPPFLAGS) $(ALL_CFLAGS) -c $<

.f.o:
	$(FC) $(FFLAGS) -c $<

# Should use a more general definition for the following.
cmextelt.o: cmextelt.cs
	$(CS) $(CSFLAGS) -c cmextelt.cs

.PHONY : all nofortran libnostub clean distclean debug saber lint

#all: libdf.a .installed
all: libdf.a 

libdf.a: $(COBJS) $(FOBJS) $(CMOBJS) #$(JOBJS)
	-$(RM) libdf.a
#	$(AR) $(ARFLAGS) libdf.a `lorder '$(COBJS) $(FOBJS)' | tsort `
	$(AR) $(ARFLAGS) libdf.a $(COBJS) $(FOBJS) $(CMOBJS)
	$(RANLIB) libdf.a

libnofortran: $(COBJS) $(CMOBJS) #$(JOBJS) 
	-$(RM) libdf.a
#	$(AR) $(ARFLAGS) libdf.a `lorder $(COBJS) | tsort `
	$(AR) $(ARFLAGS) libdf.a $(COBJS) $(CMOBJS)
	$(RANLIB) libdf.a

#libnostub nofortran: libnofortran .finstalled
libnostub nofortran: libnofortran 

rebuild:
	@$(MAKE) all $(HDF_FLAGS) FRC=force_rebuild

rebuildnofortran:
	@$(MAKE) nofortran $(MFLAGS) $(HDF_FLAGS) FRC=force_rebuild

# Leave this target blank with no actions, the rebuild macros above depend on it
force_rebuild:

$(FOBJS): $(FRC)

#debug: libdf.a .installed
debug: libdf.a 

saber:
	#load -D$(MACHINE) $(CSRCS) $(TSRCS) -lc

install: install-lib install-headers

install-lib: install-headers
	$(INSTALL_DATA) $(TARGETLIB) $(libdir)/$(TARGETLIB).n
	( cd $(libdir) ; $(RANLIB) $(libdir)/$(TARGETLIB).n )
	mv -f $(libdir)/$(TARGETLIB).n $(libdir)/$(TARGETLIB)

install-headers:
	for p in $(HINCS); do \
	    $(INSTALL_DATA) \
		`pwd`/$$p $(includedir)/$$p; \
	done

.installed: libdf.a
	-if (test \! -d $(HDFINC)) then (mkdir $(HDFINC) ) fi
	-cp $(HDRS) $(FINCS) $(HDFINC)
	-if (test \! -d $(HDFLIB)) then (mkdir $(HDFLIB) ) fi
	-cp libdf.a $(HDFLIB)
	-$(RANLIB) $(HDFLIB)/libdf.a
	-touch .installed

.finstalled: libnofortran
	-if (test \! -d $(HDFINC)) then (mkdir $(HDFINC) ) fi
	-cp $(HDRS) $(FINCS) $(HDFINC)
	-if (test \! -d $(HDFLIB)) then (mkdir $(HDFLIB) ) fi
	-cp libdf.a $(HDFLIB)
	-$(RANLIB) $(HDFLIB)/libdf.a
	-touch .finstalled

depend:
	makedepend -fMakefile -- $(CSRCS)

clean:
	-$(RM) $(RMFLAGS) *.o

distclean:
	-$(RM) $(RMFLAGS) *.o libdf.a config.status Makefile

lint: $(SRCS) $(R8SRCS)
	$(LINT) $(LINTFLAGS) $(SRCS)

TAGS: $(SRCS) $(HDRS)
	etags $(SRCS) $(HDRS)

# for distribution creation
distdir = $(TOP_SRCDIR)/$(PACKAGE)-$(VERSION)/$(curdir)
curdir = hdf/src
dist: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
	    || { echo copying $$file instead; \
	    cp -p $(srcdir)/$$file $(distdir);}; \
	done

# for auto remaking, see toplevel Makefile
#Makefile: $(TOP_SRCDIR)/config.status Makefile.in
#	CONFIG_FILES=$@ CONFIG_HEADERS= $(TOP_SRCDIR)/config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:


help:
	@echo ""
	@echo "Make supports the following targets"
	@echo "make help    - prints this usage section"
	@echo "make all     - (DEFAULT) makes HDF library"
	@echo "make libdf.a      - makes HDF library "
	@echo "make libnofortran - makes HDF library excluding fortran support"
	@echo "make rebuild      - rebuilds HDF library "
	@echo "make rebuildnofortran - rebuilds HDF library excluding"
	@echo "                        fortran support "
	@echo "make install          - install HDF library, "
	@echo "                        and includes " 
	@echo "make clean     - cleans up HDF library(*.o,libdf.a,..)"
	@echo "make distclean - cleans up HDF library including"
	@echo "                 configuration(Makefiles,config.status,..)"
	@echo ""

#$(ARCHIVE): ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#       $(RM) $(RMFLAGS) $(ARCHIVE)
#       $(SHAR) $(ARCHIVE) ChangeLog Makefile $(SRCS) $(HDRS) \
#               $(TSRCS) $(USRCS)

#archive: $(ARCHIVE)

#backup: ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS)
#       cp ChangeLog Makefile $(SRCS) $(HDRS) $(TSRCS) $(USRCS) $(BDIR)

# DO NOT DELETE THIS LINE -- make depend depends on it.

hfile.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hfile.o: vproto.h hfile.h $(FRC)
herr.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
herr.o: vproto.h  $(FRC)
hblocks.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hblocks.o: vproto.h hfile.h $(FRC)
hextelt.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hextelt.o: vproto.h hfile.h $(FRC)
cmextelt.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cmextelt.o: vproto.h hfile.h $(FRC)
hkit.o: hdf.h hdfi.h hbitio.h
hkit.o: hcomp.h herr.h hproto.h vproto.h hfile.h hkit.h $(FRC)
dfan.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfan.o: vproto.h hfile.h dfan.h $(FRC)
dfr8.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
dfr8.o: vproto.h dfrig.h $(FRC)
dfcomp.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
dfcomp.o: vproto.h $(FRC)
dfrle.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfrle.o: vproto.h $(FRC)
dfimcomp.o: hdf.h hdfi.h  hbitio.h hcomp.h herr.h
dfimcomp.o: hproto.h vproto.h $(FRC)
dfjpeg.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfjpeg.o: vproto.h  $(FRC)
dfunjpeg.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h
dfunjpeg.o: hproto.h vproto.h  $(FRC)
dfp.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfp.o: vproto.h hfile.h $(FRC)
dfgr.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfgr.o: vproto.h dfgr.h $(FRC)
df24.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
df24.o: vproto.h dfgr.h $(FRC)
dfsd.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfsd.o: vproto.h dfsd.h hfile.h $(FRC)
dfgroup.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfgroup.o: vproto.h hfile.h $(FRC)
dfconv.o: hdf.h hdfi.h hbitio.h
dfconv.o: hcomp.h herr.h hproto.h vproto.h hconv.h $(FRC)
dfufp2i.o: hdf.h hdfi.h hbitio.h
dfufp2i.o: hcomp.h herr.h hproto.h vproto.h dfufp2i.h dfsd.h $(FRC)
vg.o: vg.h hdf.h hdfi.h  hbitio.h hcomp.h herr.h hproto.h
vg.o: vproto.h tbbt.h $(FRC)
vrw.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
vrw.o: vproto.h vg.h tbbt.h hfile.h $(FRC)
vio.o: vg.h hdf.h hdfi.h  hbitio.h hcomp.h herr.h
vio.o: hproto.h vproto.h tbbt.h hfile.h $(FRC)
vparse.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vparse.o: hproto.h vproto.h tbbt.h $(FRC)
vconv.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vconv.o: hproto.h vproto.h tbbt.h $(FRC)
vsfld.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vsfld.o: hproto.h vproto.h tbbt.h $(FRC)
vgp.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vgp.o: hproto.h vproto.h tbbt.h hfile.h $(FRC)
vhi.o: vg.h hdf.h hdfi.h hbitio.h hcomp.h herr.h
vhi.o: hproto.h vproto.h tbbt.h $(FRC)
dfutil.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfutil.o: vproto.h $(FRC)
dfstubs.o: dfstubs.h df.h hdf.h hdfi.h hbitio.h hcomp.h
dfstubs.o: herr.h hproto.h vproto.h  $(FRC)
dfknat.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfknat.o: vproto.h hconv.h $(FRC)
dfkswap.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkswap.o: vproto.h hconv.h $(FRC)
dfkcray.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkcray.o: vproto.h hconv.h $(FRC)
dfkvms.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkvms.o: vproto.h hconv.h $(FRC)
dfkconv.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkconv.o: vproto.h hconv.h $(FRC)
dfkfuji.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
dfkfuji.o: vproto.h hconv.h $(FRC)
hbitio.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hbitio.o: vproto.h hfile.h $(FRC)
tbbt.o:  hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
tbbt.o: vproto.h tbbt.h $(FRC)
hdfalloc.o: hdf.h hdfi.h  hbitio.h
hdfalloc.o: hcomp.h herr.h hproto.h vproto.h hfile.h $(FRC)
hcomp.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hcomp.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h crle.h cnbit.h cskphuff.h $(FRC)
crle.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
crle.o: vproto.h hfile.h hcompi.h mstdio.h crle.h  $(FRC)
cnbit.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cnbit.o: vproto.h hfile.h hcompi.h mstdio.h cnbit.h $(FRC)
mstdio.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
mstdio.o: vproto.h hfile.h hcompi.h mstdio.h $(FRC)
cnone.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cnone.o: vproto.h hfile.h hcompi.h mstdio.h cnone.h $(FRC)
cskphuff.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
cskphuff.o: vproto.h hfile.h hcompi.h mstdio.h cskphuff.h $(FRC)
hblocks.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
hblocks.o: vproto.h hfile.h $(FRC)
mfan.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
mfan.o: vproto.h hfile.h mfan.h $(FRC)
mfanf.o: hdf.h hdfi.h hbitio.h hcomp.h herr.h hproto.h
mfanf.o: vproto.h hfile.h mfan.h $(FRC)
mfgr.o: hdf.h hdfi.h herr.h hproto.h vproto.h hfile.h mfgr.h $(FRC)
