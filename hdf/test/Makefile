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
# ****************************************************************************
#
# $Id$
#
# ##################################################################
#
#                   NCSA HDF Version 3.3 release Beta 1
#                               January 1993
#                  MAKEFILE for testing HDF Version 3.3
#
#                          ABOUT THIS MAKEFILE
#
# This makefile creates HDF test programs.  There are three categories
# of test programs:
#
#              application layer C test programs
#              application layer FORTRAN test programs
#              lower level C test programs
#
# The application layer C test programs are:
#
#          trig    (8-bit, 24-bit raster image sets and palette interface)
#          tsdnt  (scientific data sets, data only)
#          tsdnnt (same as tdfsd_nt with native number types)
#          tsdmms (scientific data sets, data, max/min and scales)
#          tsdnmms(same as tdfsd_mms with native number types)
#          tsdstr (data and dimension strings: label, unit, format and coordsys)
#          tan     (annotations for HDF objects)
#          tanfile (annotations for HDF files)
#          tstubs  (emulation of old lower level i/o routines)
#
# The application layer FORTRAN test programs are:
#
#          tr8F     (8-bit raster image sets)
#          t24F     (24-bit raster image sets)
#          tpF      (Palettes)
#          tsdntF   (scientific data sets, data only)
#          tsdnntF  (same as tdfsd_nntF except all data types are native
#                           machine number types)
#          tsdmmsF  (scientific data sets, data, max/min and scales)
#          tsdnmmsF (same as tdfsd_mmsF with native number types)
#          tsdstrF  (data and dimenstrings: label, unit, format and coordsys)
#          tanF     (annotations for HDF objects)
#          tanfileF (annotations for HDF files)
#          tstubsF  (emulation of old lower level i/o routines)
#
# Vset test programs:
#
#          egchi.c  (high level VSET write routines)
#          tv1.c    (the basic vgroup and vdata creation routines)
#          tv2.c    (creates vsets in 2 files simultaneously)
#          tvers.c  (get version string from an HDF file)
#          egfhi.f  (Fortran version of egchi.c)
#
# The lower level C test programs are:
#
#              thfile     (basic i/o)
#              terr       (error handling)
#              thblocks   (linked blocks)
#              thextelt   (i/o involving external files)
#              tvers      (storing and retrieving version strings)
#              thlinkage  (linkage of functions)
#
# Hyperslab C test programs
#              slabw      (write data as 5 slabs )
#              slab1w     (write data as first 3 of 5 slabs )
#              slab2w     (write data as last 2 of 5 slabs )
#              slab3w     (write data as 24 slabs i.e. each element )
#              slab4w     (write data as 1 slab )
#              slabwf     (write data as 5 slabs )
#
# Hyperslab Fortran test programs
#              slabw      (write data as 5 slabs )
#              slab1wf    (write data as first 3 of 5 slabs )
#              slab2wf    (write data as last 2 of 5 slabs )
#              slab3wf    (write data as 24 slabs i.e. each element )
#              slab4wf    (write data as 1 slab )
#
# ##################################################################
#
#                      HOW TO USE THIS MAKEFILE
#
# You need libdf.a to compile any of these test programs.  You will
# also need selected header files as described below.
#
# To compile the application layer C test programs, enter "make ctests".
# If there are test programs that you do not want to compile, do this
# by removing them from the lines "COBJS=" and "CSRCS=" below.
# You need access to the header files indicated in the "CHDRS=" line
# below to compile these programs.
#
# To compile the application layer FORTRAN test programs, enter
# "make ftests".  You need access to the header files indicated in
# the "FHDRS=" line below to compile these programs.
# If there are test programs that you do not want to compile, do this
# by removing them from the lines "FOBJS=" and "FSRCS=" below.
#
# To compile the lower level C test programs, enter "make htests".
# You need access to the header files indicated in the "HHDRS=" line
# below to compile these programs.  If there are test programs that
# you do not want to compile, do this by removing them from the lines
# "HOBJS=" and "HSRCS=" below.
#
# To remove all object files created by this makefile, enter "make clean"
#
# ##################################################################
#
#                       PORTING INSTRUCTIONS
#
# You must modify the lines containing the symbol "==>" to make
# this makefile compatible with your platform.
#
# ##################################################################
# ==> specify your machine
MACHINE=SUN

# ==> specify your fortran compiler  (Convex: fc)
FC=f77 
FFLAGS=  -g

# ==> specify your C compiler
CC=cc

# ==> where are your hdf.h, hdfi.h and vg.h files located?
HDFINC=   ../src

CFLAGS=  -g -D${MACHINE}

# ==> For Convex, specify CFLAGS as follows:
#CFLAGS=  -g -D${MACHINE} $(IFLAGS)

# ==> For IRIS4, specify CFLAGS as follows: 
#CFLAGS=  -g -D${MACHINE} $(IFLAGS) -ansi

# ==> where is your HDF library (libdf.a) located?
HDFLIB=../lib
HDFSRC=../src

RM=/bin/rm
RMFLAGS=-f

SRCS= 	rig.c sdstr.c blocks.c an.c anfile.c extelt.c file.c file1.c vers.c \
	sdmms.c sdnmms.c slab.c litend.c tvset.c comp.c bitio.c tree.c conv.c \
	testhdf.c

CTESTS= rig.o sdstr.o blocks.o an.o anfile.o extelt.o file.o file1.o vers.o \
	sdmms.o sdnmms.o slab.o litend.o tvset.o comp.o bitio.o tree.o conv.o \
	testhdf.o

FTESTS= tr8f t24f tpf tanf tanfilef tsdstrf tsdntf tsdnntf \
        tsdmmsf tsdnmmsf tstubsf \
        slabwf slab1wf slab2wf slab3wf slab4wf
TESTF = fortest.o
FOBJS = tr8f.o t24f.o tpf.o tanf.o tanfilef.o tsdstrf.o tsdntf.o \
        tsdnntf.o tsdmmsf.o tsdnmmsf.o tstubsf.o fortest.o \
        slabwf.o slab1wf.o slab2wf.o slab3wf.o slab4wf.o 

.f: $< $(HDFLIB)/libdf.a
	$(FC) $(FFLAGS) $< -o $@ $(HDFLIB)/libdf.a

all: testhdf fortest #gentest

nofortran: testhdf

debug: ptesthdf

#.c.o: 
#	 $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 
#
.c.o: 
	 $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 

qtesthdf: $(CTESTS) $(HDFLIB)/libdf.a
	quantify $(CC) $(CFLAGS) -o qtesthdf $(CTESTS) $(HDFSRC)/libdf.a

ptesthdf: $(CTESTS) $(HDFLIB)/libdf.a
	purify $(CC) $(CFLAGS) -o ptesthdf $(CTESTS) $(HDFSRC)/libdf.a

testhdf.tc: $(CTESTS) $(HDFLIB)/libdf.a
	proof $(CC) $(CFLAGS) -o testhdf.tc $(CTESTS) $(HDFLIB)/libdf.a

testhdf: $(CTESTS) $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -o testhdf $(CTESTS) $(HDFLIB)/libdf.a

fortest: $(FTESTS) $(HDFLIB)/libdf.a fortest.o
	$(CC) $(CFLAGS) -o fortest $(TESTF) $(HDFLIB)/libdf.a

gentest: gentest.c $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -o gentest $(HDFLIB)/libdf.a

ftests: $(FTESTS)

clean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest core \
	ptesthdf qtesthdf gentest

distclean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest core *.hdf

depend: 
	 makedepend -I$(HDFINC) *.c

saber:	$(SRCS)
	 #load -I$(HDFINC) -D$(MACHINE) $(SRCS) 


# DO NOT DELETE THIS LINE -- make depend depends on it.
