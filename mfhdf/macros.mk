DESTDIR   = \hdf\mfhdf
INCDIR1=\hdf\hdf\include
INCDIR2=$(DESTDIR)\xdr
INCDIR3=$(DESTDIR)\libsrc
INCDIR4=$(DESTDIR)
LIBDIR=$(DESTDIR)\lib
BINDIR=$(DESTDIR)\bin
FORTDIR=$(DESTDIR)\fortran

OS2     = 0
IFLAGS=   /I$(INCDIR1) /I$(INCDIR2) /I$(INCDIR3) /I$(INCDIR4) /I.
FIFLAGS=   /I$(INCDIR1) /I$(INCDIR2) /I$(INCDIR3) /I.
CC        = cl
#CFLAGS    = /c /AL /Za
CFLAGS    = /DPC /DHDF /c /Gt64 /AH /W4 $(IFLAGS) /Zp1 /Zl
#CFLAGS=  /D$(MACHINE) /Gt64 /AH /W4 /Ozaxb2 $(IFLAGS) /Zp1 /Zl

F77   = fl
#FFLAGS    = /c /AL
FFLAGS= /c /Gt64 /AH $(FIFLAGS) /W2 /Od /Tf

AR        = lib
ARFLAGS   =

ASM       = masm

WANT_FORTRAN_NETCDF = 1


