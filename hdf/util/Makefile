#
# $Header$
#
# $Log$
# Revision 1.1  1992/07/14 20:08:47  dilg
# Initial revision
#
#
UTILS = hdf24to8 hdfcomp hdfls hdfpack hdfrseq hdftopal hdftor8 paltohdf \
        r8tohdf ristosds vcompat vmake vshow

HEOBJ = he_main.o he_cntrl.o he_disp.o he_file.o

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

LIBDIR = ../lib
INCDIR = ../include

CFLAGS = -D$(MACHINE) -I$(INCDIR) -L$(LIBDIR)


.c.o: he.h he_proto.h
	$(CC) -c $(CFLAGS) $<

all: hdfed utils

hdfed: he.h he_proto.h $(HEOBJ)
	$(CC) $(CFLAGS) $(HEOBJ) -ldf -o hdfed

.c: $(LIBDIR)/libdf.a
	cc $(CFLAGS) $< -o $@ -ldf

utils: $(UTILS)
