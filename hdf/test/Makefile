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

#
# ##################################################################

# ==> specify your machine
#MACHINE=IBM6000
#MACHINE=HP9000
MACHINE=SUN

# ==> specify your fortran compiler  (Convex: fc)
FC=f77 
FFLAGS=  -g

# ==> specify your C compiler
#CC=gct -test-dir ../src
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

SRCS= 	rig.c sdstr.c blocks.c an.c anfile.c \
        extelt.c file.c file1.c vers.c sdmms.c \
        sdnmms.c slab.c litend.c testhdf.c tvset.c comp.c bitio.c

CTESTS= rig.o sdstr.o blocks.o an.o anfile.o \
        extelt.o file.o file1.o vers.o sdmms.o \
        sdnmms.o slab.o litend.o tvset.o comp.o bitio.o testhdf.o

FTESTS= tr8f t24f tpf tanf tanfilef tsdstrf tsdntf tsdnntf \
        tsdmmsf tsdnmmsf tstubsf \
        slabwf slab1wf slab2wf slab3wf slab4wf
TESTF = fortest.o
FOBJS = tr8f.o t24f.o tpf.o tanf.o tanfilef.o tsdstrf.o tsdntf.o \
        tsdnntf.o tsdmmsf.o tsdnmmsf.o tstubsf.o fortest.o \
        slabwf.o slab1wf.o slab2wf.o slab3wf.o slab4wf.o 

.f: $< $(HDFLIB)/libdf.a
	$(FC) $(FFLAGS) $< -o $@ $(HDFLIB)/libdf.a

all: testhdf fortest 

nofortran: testhdf

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

ftests: $(FTESTS)

clean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest core

depend: 
	 makedepend -I$(HDFINC) *.c

saber:	$(SRCS)
	 #load -I$(HDFINC) -D$(MACHINE) $(SRCS) 


# DO NOT DELETE THIS LINE -- make depend depends on it.

fortest.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
fortest.o: ../src/hproto.h tproto.h tutils.h
an.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
an.o: ../src/hcomp.h ../src/hproto.h tutils.h
anfile.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
anfile.o: ../src/hcomp.h ../src/hproto.h tutils.h
err.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
err.o: ../src/hproto.h
hblocks.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
hblocks.o: ../src/hcomp.h ../src/hproto.h tutils.h
hextelt.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
hextelt.o: ../src/hcomp.h ../src/hproto.h tutils.h
hfile.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
hfile.o: ../src/hcomp.h ../src/hproto.h tutils.h
hfile1.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
hfile1.o: ../src/hcomp.h ../src/hproto.h tutils.h
litend.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
litend.o: ../src/hcomp.h ../src/hproto.h tutils.h
rig.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
rig.o: ../src/hcomp.h ../src/hproto.h tutils.h
sdmms.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
sdmms.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
sdnmms.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
sdnmms.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
sdstr.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
sdstr.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
slab.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
slab.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
stubs.o: /usr/include/stdio.h ../src/df.h ../src/hdf.h ../src/hdfi.h
stubs.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h
vers.o: /usr/include/stdio.h tproto.h ../src/hdf.h ../src/hdfi.h
vers.o: ../src/hcomp.h ../src/hproto.h tutils.h
slab1w.o: ../include/hdf.h
slab2w.o: ../include/hdf.h
slab3w.o: ../include/hdf.h
slab4w.o: ../include/hdf.h
slabw.o: ../include/hdf.h
tan.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tan.o: ../src/hproto.h ../src/dfsd.h
tanfile.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tanfile.o: ../src/hproto.h
terr.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
terr.o: ../src/hproto.h
testhdf.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
testhdf.o: ../src/hproto.h tproto.h tutils.h
thblocks.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
thblocks.o: ../src/hproto.h
thextelt.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
thextelt.o: ../src/hproto.h
thfile.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
thfile.o: ../src/hproto.h
thfile1.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
thfile1.o: ../src/hproto.h
tlitend.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tlitend.o: ../src/hproto.h ../src/dfsd.h
trig.o: /usr/include/stdlib.h /usr/include/sys/stdtypes.h ../src/hdf.h
trig.o: ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h ../src/hproto.h
tsdmms.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tsdmms.o: ../src/hproto.h ../src/dfsd.h
tsdnmms.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tsdnmms.o: ../src/hproto.h ../src/dfsd.h
tsdnnt.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tsdnnt.o: ../src/hproto.h ../src/dfsd.h
tsdnt.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tsdnt.o: ../src/hproto.h ../src/dfsd.h
tsdstr.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
tsdstr.o: ../src/hproto.h ../src/dfsd.h
tstubs.o: /usr/include/stdio.h ../src/df.h ../src/hdf.h ../src/hdfi.h
tstubs.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h
tvset.o: ../src/vg.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
tvset.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h /usr/include/ctype.h
tvset.o: ../src/vproto.h
tvers.o: /usr/include/stdio.h ../src/hdf.h ../src/hdfi.h ../src/hcomp.h
tvers.o: ../src/hproto.h
