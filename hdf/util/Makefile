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
UTILS = hdf24to8 hdfcomp hdfls hdfpack hdfrseq hdftopal hdftor8 paltohdf \
        r8tohdf ristosds vcompat vmake vshow make24 hdf2jpeg jpeg2hdf fp2hdf

HEOBJ = he_main.o he_cntrl.o he_disp.o he_file.o

#
# These values will typically be over ridden by values specified in
#   top level HDF 
#

# ==> Uncomment the line with your system type on it.
MACHINE = SUN
#MACHINE = UNICOS
#MACHINE = IRIS4
#MACHINE = ALPHA
#MACHINE = APOLLO
#MACHINE = IBM6000
#MACHINE = CONVEX
#MACHINE = VMS
#MACHINE = MIPSEL
#MACHINE = MAC
#MACHINE = PC
#MACHINE = HP9000

RM=/bin/rm
RMFLAGS=-f

HDFLIB = ../lib
HDFINC = ../include
HDFBIN = ../bin
#For IRIS4, include libgl_s.a
#GLLIB = -lgl_s
GLLIB = 

CFLAGS = -D$(MACHINE)

# ==> For ALPHA, specify CFLAGS as follows:
#CFLAGS=  -g -DALPHA -std1

# ==> For IRIS4, specify CFLAGS as follows:
#CFLAGS=  -g -D$(MACHINE) -ansi

.c.o: he.h he_proto.h $(HDFLIB)/libdf.a
	$(CC) -c $(CFLAGS) -I$(HDFINC) $(HDFLIB)/libdf.a $(GLLAB)  $<
#	$(CC) -c $(CFLAGS) -I$(HDFINC) $(GLLIB)  $<

all: hdfed utils fp2hdf fptest fptestf
	-if( test \! -d $(HDFBIN)) then (mkdir $(HDFBIN)) fi
	-cp $(UTILS) hdfed $(HDFBIN)

nofortran: hdfed utils fp2hdf fptest

hdfed: he.h he_proto.h $(HEOBJ) $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $(HEOBJ) $(HDFLIB)/libdf.a -o hdfed

.c: $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $< -o $@ $(HDFLIB)/libdf.a $(GLLIB)

.f: $(HDFLIB)/libdf.a
	$(FC) $(FFLAGS) $< $(HDFLIB)/libdf.a -o $@

utils: $(UTILS) $(HDFLIB)/libdf.a

clean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed fptest fptestf

distclean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed fptest fptestf
