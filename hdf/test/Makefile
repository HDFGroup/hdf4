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
HDFLIB=../lib

RM=/bin/rm
RMFLAGS=-f

SRCS= 	rig.c sdstr.c blocks.c an.c anfile.c \
        extelt.c file.c file1.c vers.c sdmms.c \
        sdnmms.c slab.c litend.c testhdf.c tvset.c tv1.c tv2.c egchi.c

CTESTS= rig.o sdstr.o blocks.o an.o anfile.o \
        extelt.o file.o file1.o vers.o sdmms.o \
        sdnmms.o slab.o litend.o tvset.o testhdf.o  tv1.o tv2.o egchi.o

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
	 #load $(SRCS) -I../include -D$(MACHINE) 


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
