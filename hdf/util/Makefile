#
# $Header$
#
# $Log$
# Revision 1.11  1992/10/09 20:58:15  chouck
# Fixed problems passing down compiler name
#
# Revision 1.10  1992/09/17  19:51:13  koziol
# Made changes for ANSI prototypes on the Convex
#
# Revision 1.9  1992/09/04  15:20:18  chouck
# Fixed rules for MISPEL compiler
#
# Revision 1.8  1992/08/14  20:11:36  sxu
# Added -lgl_s and -cckr (or -ansi) for SGI
#
# Revision 1.7  1992/08/12  15:58:11  sxu
# Removed redundant -I for CONVEX
#
# Revision 1.6  1992/08/05  21:27:55  chouck
# Added support for moving utils to HDFBIN
#
# Revision 1.5  1992/07/31  21:20:39  chouck
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

RM=/bin/rm
RMFLAGS=-f

HDFLIB = ../lib
HDFINC = ../include
HDFBIN = ../bin
#For IRIS4, include libgl_s.a
#GLLIB = -lgl_s
GLLIB = 

CFLAGS = -D$(MACHINE)

# ==> For IRIS4, specify CFLAGS as follows:
#CFLAGS=  -g -D$(MACHINE) -cckr

.c.o: he.h he_proto.h
#	$(CC) -c $(CFLAGS) -I$(HDFINC) $(HDFLIB)/libdf.a $(GLLAB)  $<
	$(CC) -c $(CFLAGS) -I$(HDFINC) $(GLLIB)  $<

all: hdfed utils
	-mkdir $(HDFBIN)
	-cp $(UTILS) hdfed $(HDFBIN)

nofortran: all

hdfed: he.h he_proto.h $(HEOBJ)
	$(CC) $(CFLAGS) -I$(HDFINC) $(HEOBJ) $(HDFLIB)/libdf.a -o hdfed

.c: $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $< -o $@ $(HDFLIB)/libdf.a $(GLLIB)

utils: $(UTILS)

clean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed
