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
UTILS = hdf24to8 hdfcomp hdfls hdfpack hdftopal hdftor8 paltohdf \
        r8tohdf ristosds vcompat vmake vshow hdf8to24 hdf2jpeg jpeg2hdf fp2hdf \
	hdfunpac

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
JPEGLIB = ../jpeg
JPEGINC = ../jpeg
#For IRIS4, include libgl_s.a
GLLIB = -lgl_s
#For the HP9000 include libm.a
MATHLIB = -lm

CFLAGS = -D$(MACHINE)

# ==> For ALPHA, specify CFLAGS as follows:
#CFLAGS=  -g -DALPHA -std1

# ==> For IRIS4, specify CFLAGS as follows:
#CFLAGS=  -g -D$(MACHINE) -ansi

.c.o: he.h he_proto.h $(HDFLIB)/libdf.a $(FRC)
	$(CC) -c $(CFLAGS) -I$(HDFINC) $<

all: hdfed utils fptest
#	-if( test \! -d $(HDFBIN)) then (mkdir $(HDFBIN)) fi
#	-cp $(UTILS) hdfed $(HDFBIN)

nofortran: hdfed utils fptest

rebuild:
	@$(MAKE) all $(HDF_FLAGS) FRC=force_rebuild

rebuildnofortran:
	@$(MAKE) nofortran $(MFLAGS) $(HDF_FLAGS) FRC=force_rebuild

# Leave this target blank with no actions, the rebuild macros above depend on it
force_rebuild:

hdfed: he.h he_proto.h $(HEOBJ) $(HDFLIB)/libdf.a $(FRC)
	$(CC) $(CFLAGS) -I$(HDFINC) $(HEOBJ) $(HDFLIB)/libdf.a -o hdfed $(JPEGLIB)/libjpeg.a

fp2hdf: $(HDFLIB)/libdf.a fp2hdf.c
	$(CC) $(CFLAGS) -I$(HDFINC) fp2hdf.c -o $@ $(HDFLIB)/libdf.a $(MATHLIB) $(JPEGLIB)/libjpeg.a

.c: $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $< -o $@ $(HDFLIB)/libdf.a $(JPEGLIB)/libjpeg.a

.f: $(HDFLIB)/libdf.a 
	$(FC) $(FFLAGS) $< $(HDFLIB)/libdf.a -o $@ $(JPEGLIB)/libjpeg.a

utils: $(UTILS) $(HDFLIB)/libdf.a

$(HEOBJ): $(FRC)

$(UTILS): $(FRC)

clean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed fptest

distclean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed fptest
