#  Copyright 1993, UCAR/Unidata
#
#  DOS and OS/2 Makefile for ncgen
#
#  NOTE:  Set the definition of the OS2 macro to match OS as follows:
#         OS2 = 0     -> DOS
#         OS2 = 1     -> OS/2

OS2      = 0

WANT_FORTRAN_NETCDF = 1

AR        = lib
ARFLAGS   = 

ASM       = masm

CC	      = cl
CFLAGS    = /c /AL /Za

!IF $(WANT_FORTRAN_NETCDF)
F77       = fl
FFLAGS    = /c /AL
FORTLIB   = llibfor7.lib
FTEST     = ftest
!ENDIF

LINK      = link
LFLAGS    = /st:10000 /nod /noe

INCDIR    = ..\libsrc
INCLUDES  = /I$(INCDIR)

DESTDIR   = C:
FORTDIR   = ..\FORTRAN

BINDIR    = $(DESTDIR)\bin
LIBDIR    = $(DESTDIR)\lib
NCGENLIB  = ncgen.lib
NETCDFLIB = ..\libsrc\netcdf.lib
CLIB      = llibc7.lib

!IF $(OS2)
OS2LIB    = os2.lib
!ELSE
OS2LIB    = 
!ENDIF
XDRLIB    = ..\xdr\xdr.lib
LIBS      = $(NCGENLIB) $(NETCDFLIB) $(XDRLIB) $(OS2LIB) $(CLIB)

.c.obj:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $<

LOADLIBS =

GOAL =  ncgen.exe

SRCS =  main.c generate.c load.c ncgentab.c escapes.c \
	getfill.c init.c close.c genlib.c

MAIN =  main.obj

OBJS =	generate.obj load.obj ncgentab.obj escapes.obj \
	getfill.obj init.obj close.obj genlib.obj

LOBJS1 =  -+generate.obj -+load.obj -+ncgentab.obj -+escapes.obj 
LOBJS2 = -+getfill.obj -+init.obj -+close.obj -+genlib.obj

all:	$(GOAL)

$(GOAL): $(MAIN) $(NCGENLIB) $(NETCDFLIB)
	$(LINK) $(LFLAGS) $(MAIN),$(GOAL),,$(LIBS);

$(NCGENLIB): $(OBJS)
	$(AR) $@ $(ARFLAGS) $(LOBJS1),LIB.LST;
	$(AR) $@ $(ARFLAGS) $(LOBJS2),LIB.LST;

install: $(GOAL)
	copy $(GOAL) $(BINDIR)

test:   ncgen.exe test0.cdl ntest ctest $(FTEST) FORCE

FORCE:

# test "-n" option of ncgen
ntest:	ncgen.exe test0.cdl test1.cdl
	ncgen -n test1.cdl
	..\ncdump\ncdump test1.cdf > test2.cdl
 	diff test1.cdl test2.cdl
	@echo "*** ncgen -n test successful ***"

# test "-c" option of ncgen
ctest:	ncgen.exe test0.cdl test1.cdl
	ncgen -c test0.cdl > test0.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) test0.c
	$(LINK) $(LFLAGS) test0,test0.exe,,$(LIBS);
	test0
	..\ncdump\ncdump -n test1 ctest0.cdf > ctest1.cdl
 	diff test1.cdl ctest1.cdl
	@echo "*** ncgen -c test successful ***"

# test "-f" option of ncgen
ftest:	ncgen.exe test0.cdl netcdf.inc test1.cdl msoft.int jackets.obj fslen.obj
	ncgen -f test0.cdl > test0.for
	$(F77) $(FFLAGS) test0.for
	$(LINK) $(LFLAGS) test0 jackets fslen,test0.exe,,$(LIBS) $(FORTLIB);
	test0
	..\ncdump\ncdump -n test1 ftest0.cdf > ftest1.cdl
 	diff test1.cdl ftest1.cdl
	@echo "*** ncgen -f test successful ***" 

test1.cdl: test0.cdf
	..\ncdump\ncdump -n test1 test0.cdf > test1.cdl

test0.cdf: ncgen.exe test0.cdl
	ncgen -n test0.cdl

netcdf.inc: $(FORTDIR)\netcdf.inc
	rm -f netcdf.inc
	copy $(FORTDIR)\netcdf.inc

fslen.obj: $(FORTDIR)\fslen.asm
	rm -f fslen.asm
	copy $(FORTDIR)\fslen.asm
	$(ASM) fslen;

jackets.obj: $(FORTDIR)\jackets.c
	rm -f jackets.c
	copy $(FORTDIR)\jackets.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) jackets.c

msoft.int: $(FORTDIR)\msoft.int
	rm -f msoft.int
	copy $(FORTDIR)\msoft.int

ncgentab.c : msofttab.c
	cp $? $@

ncgentab.h : msofttab.h
	cp $? $@

ncgenyy.c : msoftyy.c
	cp $? $@

ncgen: ncgen.exe

clean:
	rm -f *.obj *.map *.lst netcdf.inc msoft.int jackets.c fslen.asm \
		*.bak ncgen.lib $(GOAL)
	rm -f test0.cdf test0.for test0.exe test1.cdf test1.cdl test2.cdl \
		ftest0.cdf ftest1.cdl test0.c ctest0.cdf ctest1.cdl

# DO NOT DELETE THIS LINE -- make depend depends on it.

generate.obj: $(INCDIR)\netcdf.h generic.h ncgen.h
load.obj: $(INCDIR)\netcdf.h generic.h ncgen.h
ncgentab.obj: $(INCDIR)\netcdf.h generic.h ncgen.h ncgenyy.c ncgentab.h
getfill.obj: $(INCDIR)\netcdf.h generic.h
init.obj: $(INCDIR)\netcdf.h ncgen.h
init.obj: generic.h
close.obj: $(INCDIR)\netcdf.h ncgen.h
close.obj: generic.h
