#
# $Header$
#
# $Log$
# Revision 1.5  1992/07/31 21:20:39  chouck
# Fixed a couple CFLAGS problems
#
# Revision 1.3  1992/07/15  21:39:54  sxu
# Added _STDC_ to make CONVEX happy.
#
# Revision 1.2  1992/07/15  19:36:00  chouck
# Modified to conform to having a top level Makefile
#
# Revision 1.1  1992/07/14  20:08:47  dilg
# Initial revision
#
#
UTILS = hdf24to8 hdfcomp hdfls hdfpack hdfrseq hdftopal hdftor8 paltohdf \
        r8tohdf ristosds vcompat vmake vshow

HEOBJ = he_main.o he_cntrl.o he_disp.o he_file.o

#
# These values will typically be over ridden by values specified in
#   top level HDF 
#

# ==> Uncomment the line with your system type on it.
MACHINE = SUN
#MACHINE = UNICOS
#MACHINE = IRIS4
#MACHINE = APOLLO
#MACHINE = IBM6000
#MACHINE = CONVEX
#MACHINE = VMS
#MACHINE = MIPSEL
#MACHINE = MAC
#MACHINE = PC
#MACHINE = HP9000

HDFLIB = ../lib
HDFINC = ../include

CFLAGS = -D$(MACHINE) -I$(HDFINC) 

# ==> For Convex, specify CFLAGS as follows:
# CFLAGS=  -g -D$(MACHINE) -I$(INCDIR)  -L$(LIBDIR) -D__STDC__

.c.o: he.h he_proto.h
	$(CC) -c $(CFLAGS) -I$(HDFINC) $(HDFLIB)/libdf.a $<

all: hdfed utils

nofortran: all

hdfed: he.h he_proto.h $(HEOBJ)
	$(CC) $(CFLAGS) -I$(HDFINC) $(HEOBJ) $(HDFLIB)/libdf.a -o hdfed

.c: $(HDFLIB)/libdf.a
	cc $(CFLAGS) -I$(HDFINC) $< -o $@ $(HDFLIB)/libdf.a

utils: $(UTILS)

clean:
	-$(RM) $(RMFLAGS) *.o
