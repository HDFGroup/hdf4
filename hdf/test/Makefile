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

RM=/bin/rm
RMFLAGS=-f

SRCS= 	rig.c sdstr.c blocks.c an.c anfile.c \
        extelt.c file.c file1.c vers.c sdmms.c \
        sdnmms.c slab.c litend.c tvset.c tree.c conv.c testhdf.c

CTESTS= rig.o sdstr.o blocks.o an.o anfile.o \
        extelt.o file.o file1.o vers.o sdmms.o \
        sdnmms.o slab.o litend.o tvset.o tree.o conv.o testhdf.o

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

debug: ptesthdf

#.c.o: 
#	 $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 
#
.c.o: 
	 $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 

ptesthdf: $(CTESTS) $(HDFLIB)/libdf.a
	purify $(CC) $(CFLAGS) -o ptesthdf $(CTESTS) $(HDFLIB)/libdf.a

testhdf.tc: $(CTESTS) $(HDFLIB)/libdf.a
	proof $(CC) $(CFLAGS) -o testhdf.tc $(CTESTS) $(HDFLIB)/libdf.a

testhdf: $(CTESTS) $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -o testhdf $(CTESTS) $(HDFLIB)/libdf.a

fortest: $(FTESTS) $(HDFLIB)/libdf.a fortest.o
	$(CC) $(CFLAGS) -o fortest $(TESTF) $(HDFLIB)/libdf.a

ftests: $(FTESTS)

clean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest core

distclean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest core *.hdf \	*.o

depend: 
	 makedepend -I$(HDFINC) *.c

saber:	$(SRCS)
	 #load -I$(HDFINC) -D$(MACHINE) $(SRCS)


# DO NOT DELETE THIS LINE -- make depend depends on it.
