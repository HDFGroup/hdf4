# ==> specify your machine
#MACHINE=IBM6000
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
HDFLIB=../../develop/lib

RM=/bin/rm
RMFLAGS=-f

CTESTS= nt_rig.o nt_sdstr.o nt_hblocks.o nt_an.o nt_anfile.o \
        nt_hextelt.o nt_hfile.o nt_hfile1.o nt_vers.o nt_sdmms.o \
        nt_sdnmms.o testhdf.o
#        tsdmms tsdnmms tsdnnt tsdnt tan \
#        tanfile tstubs tv1 tv2 tvers egchi

.c.o: ; $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 

all: testhdf

testhdf: $(CTESTS)
	$(CC) $(CFLAGS) -o testhdf $(CTESTS) $(HDFLIB)/libdf.a

clean: ; -$(RM) $(RMFLAGS) $(CTESTS)

depend: ; makedepend -I$(HDFINC) *.c



# DO NOT DELETE THIS LINE -- make depend depends on it.

nt_an.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_an.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_anfile.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_anfile.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_err.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
nt_err.o: ../src/hproto.h
nt_hblocks.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_hblocks.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_hextelt.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_hextelt.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_hfile.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_hfile.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_hfile1.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_hfile1.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_rig.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_rig.o: ../src/hcomp.h ../src/hproto.h tutils.h
nt_sdmms.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_sdmms.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
nt_sdnmms.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_sdnmms.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
nt_sdstr.o: tproto.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
nt_sdstr.o: ../src/hcomp.h ../src/hproto.h tutils.h ../src/dfsd.h
nt_stubs.o: /usr/include/stdio.h ../src/df.h ../src/hdf.h ../src/hdfi.h
nt_stubs.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h
nt_vers.o: /usr/include/stdio.h tproto.h ../src/hdf.h ../src/hdfi.h
nt_vers.o: ../src/hcomp.h ../src/hproto.h tutils.h
testhdf.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
testhdf.o: ../src/hproto.h tproto.h tutils.h
