#   Copyright 1993, University Corporation for Atmospheric Research
#
#  DOS and OS/2 Makefile for netCDF library
#
#  NOTE:  Set the definition of the OS2 macro to match OS as follows:
#         OS2 = 0     -> DOS
#         OS2 = 1     -> OS/2

OS2      = 0

AR        = LIB
ARFLAGS   = 

CC        = cl
CFLAGS    = /c /AL /Za

LINK      = link
LFLAGS    = /nod

DESTDIR   = C:
LIBDIR    = $(DESTDIR)\lib

INCDIR    = ..\xdr
INCLUDES  = /I$(INCDIR)

NETCDFLIB = netcdf.lib
CLIBS     = llibc7.lib
!IF $(OS2)
OS2LIB    = os2.lib
!ELSE
OS2LIB    =
!ENDIF
XDRLIB    = ..\xdr\xdr.lib
LIBS      = $(NETCDFLIB) $(XDRLIB) $(OS2LIB) $(CLIBS)

MANIFEST = Makefile alloc.h array.c attr.c cdf.c \
	cdftest.c descrip.mms dim.c error.c error.h file.c \
	htons.mar iarray.c local_nc.h netcdf.h ntohs.mar \
	putget.c sharray.c string.c test.cdf.sav var.c globdef.c

CSRCS = array.c attr.c cdf.c dim.c error.c file.c globdef.c iarray.c \
	putget.c putgetg.c sharray.c string.c var.c xdrposix.c

COBJS = array.obj attr.obj cdf.obj dim.obj error.obj file.obj globdef.obj \
	iarray.obj putget.obj putgetg.obj sharray.obj string.obj var.obj \
	xdrposix.obj

LOBJS = -+array -+attr -+cdf -+dim -+error -+file -+globdef -+iarray \
	-+putget -+putgetg -+sharray -+string -+var -+xdrposix

.c.obj:
	$(CC) $(CFLAGS) $(INCLUDES) $<

all:		$(NETCDFLIB)

$(NETCDFLIB): netcdf.h $(COBJS) 
	$(AR) $@ $(ARFLAGS) $(LOBJS),LIB.LST;

test:		cdftest.exe FORCE
	cdftest

FORCE:

cdftest.exe: cdftest.obj $(COBJS)
	$(LINK) $(LFLAGS) cdftest.obj,,,$(LIBS);

clean  :
	rm -f *.obj *.map *.lst *.bak netcdf.lib cdftest.exe test.cdf

install : $(NETCDFLIB)
	copy $(NETCDFLIB) $(LIBDIR)

array.o: array.c
array.o: ./local_nc.h
array.o: ./netcdf.h
array.o: ./alloc.h
attr.o: attr.c
attr.o: ./local_nc.h
attr.o: ./netcdf.h
attr.o: ./alloc.h
cdf.o: cdf.c
cdf.o: ./local_nc.h
cdf.o: ./netcdf.h
cdf.o: ./alloc.h
cdftest.o: cdftest.c
cdftest.o: ./netcdf.h
dim.o: dim.c
dim.o: ./local_nc.h
dim.o: ./netcdf.h
dim.o: ./alloc.h
error.o: error.c
error.o: ./local_nc.h
error.o: ./netcdf.h
file.o: file.c
file.o: ./local_nc.h
file.o: ./netcdf.h
file.o: ./alloc.h
globdef.o: globdef.c
globdef.o: ./netcdf.h
iarray.o: iarray.c
iarray.o: ./local_nc.h
iarray.o: ./netcdf.h
iarray.o: ./alloc.h
putget.o: putget.c
putget.o: ./local_nc.h
putget.o: ./netcdf.h
putget.o: ./alloc.h
putgetg.o: putgetg.c
putgetg.o: ./local_nc.h
putgetg.o: ./netcdf.h
sharray.o: sharray.c
sharray.o: ./local_nc.h
sharray.o: ./netcdf.h
sharray.o: ./alloc.h
string.o: string.c
string.o: ./local_nc.h
string.o: ./netcdf.h
string.o: ./alloc.h
var.o: var.c
var.o: ./local_nc.h
var.o: ./netcdf.h
var.o: ./alloc.h
xdrposix.o: xdrposix.c
xdrposix.o: ./netcdf.h
xdrposix.o: ./local_nc.h
xdrposix.o: ./netcdf.h


