#  Copyright 1993, UCAR/Unidata
#
#  DOS and OS/2 Makefile for netcdf exhaustive test on PS/2
#
#  NOTE:  Set the definition of the OS2 macro to match OS as follows:
#         OS2 = 0     -> DOS
#         OS2 = 1     -> OS/2

OS2      = 0

AR       = LIB
ARFLAGS  = 

CC        = cl
CFLAGS	  = /c /AL /Za

LINK      = link
LFLAGS    = /st:10000 /nod

INCDIR    =  ..\libsrc
INCLUDES  = /I$(INCDIR)

DESTDIR   = C:

BINDIR    = $(DESTDIR)\bin
LIBDIR    = $(DESTDIR)\lib
NCTESTLIB = nctest.lib
NETCDFLIB = ..\libsrc\netcdf.lib
CLIB      = llibc7.lib
!IF $(OS2)
OS2LIB    = os2.lib
!ELSE
OS2LIB    =
!ENDIF
XDRLIB    = ..\xdr\xdr.lib
LIBS      = $(NCTESTLIB) $(NETCDFLIB) $(XDRLIB) $(OS2LIB) $(CLIB)

.c.obj:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $<

GOAL = nctest.exe

SRCS =  varget.c vargetg.c varput.c varputg.c vardef.c vartests.c \
	vputget.c vputgetg.c driver.c cdftests.c dimtests.c rec.c \
	atttests.c misctest.c add.c error.c emalloc.c val.c slabs.c

MAIN =  driver.obj

OBJS =  varget.obj vargetg.obj varput.obj varputg.obj vardef.obj vartests.obj \
	vputget.obj vputgetg.obj cdftests.obj dimtests.obj rec.obj \
	atttests.obj misctest.obj add.obj error.obj emalloc.obj val.obj \
	slabs.obj

LOBJS1 =  -+varget.obj -+vargetg.obj -+varput.obj -+varputg.obj -+vardef.obj \
          -+vartests.obj -+vputget.obj
LOBJS2 =  -+vputgetg.obj -+cdftests.obj -+dimtests.obj -+rec.obj \
	  -+atttests.obj -+misctest.obj
LOBJS3 =  -+add.obj -+error.obj -+emalloc.obj -+val.obj -+slabs.obj

all:	$(GOAL)

test:	$(GOAL) FORCE
	$(GOAL)

FORCE:

$(GOAL): $(MAIN) $(NCTESTLIB) $(NETCDFLIB) $(XDRLIB)
	$(LINK) $(LFLAGS) $(MAIN),$(GOAL),,$(LIBS);

$(NCTESTLIB):	$(OBJS)
	$(AR) $@ $(ARFLAGS) $(LOBJS1),LIB.LST;
	$(AR) $@ $(ARFLAGS) $(LOBJS2),LIB.LST;
	$(AR) $@ $(ARFLAGS) $(LOBJS3),LIB.LST;

install:

clean:
	rm -f *.obj *.lst *.map *.bak nctest.lib nctest.exe *.cdf

# DO NOT DELETE THIS LINE -- make depend depends on it.

add.obj:      $(INCDIR)\netcdf.h testcdf.h
atttests.obj: $(INCDIR)\netcdf.h testcdf.h
cdftests.obj: $(INCDIR)\netcdf.h testcdf.h
dimtests.obj: $(INCDIR)\netcdf.h testcdf.h
driver.obj:   $(INCDIR)\netcdf.h
emalloc.obj:                     
misctest.obj: $(INCDIR)\netcdf.h testcdf.h
slabs.obj:    $(INCDIR)\netcdf.h testcdf.h
val.obj:      $(INCDIR)\netcdf.h testcdf.h
vardef.obj:   $(INCDIR)\netcdf.h testcdf.h
varget.obj:   $(INCDIR)\netcdf.h testcdf.h
varput.obj:   $(INCDIR)\netcdf.h testcdf.h
vputget.obj:  $(INCDIR)\netcdf.h testcdf.h
vartests.obj: $(INCDIR)\netcdf.h testcdf.h
