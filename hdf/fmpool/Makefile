# Makefile for file memory pool 

LIB=	libfmpool.a
OBJ=	fmpool.o fmpio.o
CSRCS=	fmpool.c fmpio.c
HDRS= fmpool.h fmpio.h fmptypes.h fmpconf.h compat.h cdefs.h 

${LIB}: ${OBJ} ${HDRS}
	rm -f $@
	ar cq $@ \
	    `lorder ${OBJ} | tsort`
	ranlib $@

clean:
	rm -f ${LIB} ${OBJ} tsmp *.mp

#CC=cc
#CFLAGS= -g -Aa -D_HPUX_SOURCE

CC=gcc
CFLAGS= -O -ansi -D_HPUX_SOURCE -DSTATISTICS
#CFLAGS= -g -ansi -DSTATISTICS -D_HPUX_SOURCE
#CFLAGS= -O -ansi -DNEED_SYNC_LASTPAGE
#CFLAGS= -g -ansi -Wall -pedantic -DSTATISTICS
#CFLAGS= -g -ansi -D_POSIX_SOURCE
#AUXFLAGS= -DPURIFY -DSTATISTICS -DMPOOL_DEBUG -DMP_DEBUG -DDEBUG
#AUXFLAGS= -DPURIFY -DSTATISTICS -DMPOOL_DEBUG -DMP_DEBUG -DCIRCLEQ_DEBUG
#AUXFLAGS= -DPURIFY -DSTATISTICS -DMP_DEBUG -DDEBUG
#AUXFLAGS= -DPURIFY -DSTATISTICS -DPOSIX_DEBUG
#AUXFLAGS= -DSTATISTICS=1 -DHAVE_GETRUSAGE -DUSE_INLINE
#AUXFLAGS= -DHAVE_GETRUSAGE -DSTATISTICS=1
#AUXFLAGS=	

CL=	${CC} $(CFLAGS) -c -D__FMPINTERFACE_PRIVATE ${AUXFLAGS} -I. 

debug: ptfmp ptfmpr ptfmpw

tfmp: ${HDRS} test_fmpio.c libfmpool.a
	${CC} $(CFLAGS)  ${AUXFLAGS} -I. -o tfmp test_fmpio.c libfmpool.a

tfmpr: ${HDRS} tfmpio_read.c libfmpool.a
	${CC} $(CFLAGS)  ${AUXFLAGS} -I. -o tfmpr tfmpio_read.c libfmpool.a

ptfmpr: ${HDRS} tfmpio_read.c libfmpool.a
	purify ${CC} $(CFLAGS)  ${AUXFLAGS} -I. -o ptfmpr tfmpio_read.c libfmpool.a

tfmpw: ${HDRS} tfmpio_write.c libfmpool.a
	${CC} $(CFLAGS)  ${AUXFLAGS} -I. -o tfmpw tfmpio_write.c libfmpool.a

ptfmpw: ${HDRS} tfmpio_write.c libfmpool.a
	purify ${CC} $(CFLAGS)  ${AUXFLAGS} -I. -o ptfmpw tfmpio_write.c libfmpool.a

ptfmp: ${HDRS} test_fmpio.c libfmpool.a
	purify ${CC} $(CFLAGS)  -DPURIFY -DSTATISTICS ${AUXFLAGS} -I. -o ptfmp test_fmpio.c libfmpool.a

fmpool.o: ${HDRS} fmpool.c 
	${CL} fmpool.c

fmpio.o:  ${HDRS} fmpio.c 
	${CL} fmpio.c

saber-write:
	#load $(CFLAGS) -c -D__FMPINTERFACE_PRIVATE ${AUXFLAGS} -I. $(CSRCS) tfmpio_write.c -lc

saber-read:
	#load $(CFLAGS) -c -D__FMPINTERFACE_PRIVATE ${AUXFLAGS} -I. $(CSRCS) tfmpio_read.c -lc
