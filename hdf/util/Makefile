#
# $Header$
#
# $Log$
# Revision 1.15.2.1  1993/10/10 22:11:39  koziol
# Updated makefile to be a little more friendly
#
# Revision 1.15  1993/09/16  15:31:53  koziol
# Changes for updating the Fujitsu VP
#
# Revision 1.14  1993/04/23  21:58:58  koziol
# Cleaned up some more warnings on the Cray
#
# Revision 1.13  1993/04/13  17:45:43  koziol
# Made the installation check for a previous bin directory before trying to
# re-create it.
#
# Revision 1.12  1993/01/19  06:00:06  koziol
# Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
# port.  Lots of minor annoyances fixed.
#
# Revision 1.11  1992/10/09  20:58:15  chouck
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
        r8tohdf ristosds vcompat vmake vshow make24 hdf2jpeg jpeg2hdf 

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

all: hdfed utils 
	-if( test \! -d $(HDFBIN)) then (mkdir $(HDFBIN)) fi
	-cp $(UTILS) hdfed $(HDFBIN)

nofortran: all

hdfed: he.h he_proto.h $(HEOBJ) $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $(HEOBJ) $(HDFLIB)/libdf.a -o hdfed

.c: $(HDFLIB)/libdf.a
	$(CC) $(CFLAGS) -I$(HDFINC) $< -o $@ $(HDFLIB)/libdf.a $(GLLIB)

utils: $(UTILS) $(HDFLIB)/libdf.a

clean:
	-$(RM) $(RMFLAGS) *.o $(UTILS) hdfed
