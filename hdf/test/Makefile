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

SRCS= 	nt_rig.c nt_sdstr.c nt_hblocks.c nt_an.c nt_anfile.c \
        nt_hextelt.c nt_hfile.c nt_hfile1.c nt_vers.c nt_sdmms.c \
        nt_sdnmms.c nt_slab.c testhdf.c

CTESTS= nt_rig.o nt_sdstr.o nt_hblocks.o nt_an.o nt_anfile.o \
        nt_hextelt.o nt_hfile.o nt_hfile1.o nt_vers.o nt_sdmms.o \
        nt_sdnmms.o nt_slab.o testhdf.o

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

.c.o: 
	 $(CC) $(CFLAGS) -c -I$(HDFINC) $*.c 

all: testhdf

testhdf: $(CTESTS)
	$(CC) $(CFLAGS) -o testhdf $(CTESTS) $(HDFLIB)/libdf.a

fortest: $(FTESTS) fortest.o
	$(CC) $(CFLAGS) -o fortest $(TESTF) $(HDFLIB)/libdf.a

ftests: $(FTESTS)

clean: 
	 -$(RM) $(RMFLAGS) $(CTESTS) testhdf $(FOBJS) $(FTESTS) fortest

depend: 
	 makedepend -I$(HDFINC) *.c

saber:	$(SRCS)
	 #load $(SRCS) -I../include -D$(MACHINE) 


# DO NOT DELETE THIS LINE -- make depend depends on it.

egchi.o: ../src/vg.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
egchi.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h /usr/include/ctype.h
egchi.o: ../src/vproto.h
fortest.o: ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h ../src/hcomp.h
fortest.o: ../src/hproto.h tproto.h tutils.h
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
tv1.o: ../src/vg.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
tv1.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h /usr/include/ctype.h
tv1.o: ../src/vproto.h
tv2.o: ../src/vg.h ../src/hdf.h ../src/hdfi.h /usr/include/stdio.h
tv2.o: ../src/hcomp.h ../src/hproto.h ../src/herr.h /usr/include/ctype.h
tv2.o: ../src/vproto.h
tvers.o: /usr/include/stdio.h ../src/hdf.h ../src/hdfi.h ../src/hcomp.h
tvers.o: ../src/hproto.h
