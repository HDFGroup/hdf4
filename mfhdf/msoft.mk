# $Id$
#
#          Makefile to build netcdf library and utilities on MSDOS and OS/2
#
# Anticipated, changeable macros (others won't be passed, recursively):
#
#   Set the definition of the OS2 macro to match OS as follows:
#         OS2 = 0     -> DOS
#         OS2 = 1     -> OS/2

OS2     = 0
CC        = cl
#CFLAGS    = /c /AL /Za
#CFLAGS    = /c /AH /DPC /DHDF /Zi
#CFLAGS    = /c /AH /DPC /DHDF /W4
CFLAGS    = /c /AH /DPC /DHDF /Ox
DESTDIR   = C:
F77   = fl
FFLAGS    = /c /AH
#FFLAGS    = /c /AL
FORTRAN_NETCDF = 1

# End of anticipated macros

# Macros for recursive make(1)s:
MY_MFLAGS   = \
    OS2="$(OS2)" \
    CC="$(CC)" \
    CFLAGS="$(CFLAGS)" \
    DESTDIR="$(DESTDIR)" \
    F77="$(F77)" \
    FFLAGS="$(FFLAGS)" \
    FORTRAN_NETCDF="$(FORTRAN_NETCDF)"

!IF $(FORTRAN_NETCDF)
FORTRANSUB       = fortran
!ENDIF

INSTSUBS           = xdr libsrc $(FORTRANSUB) ncgen ncdump
ALLSUBS            = xdr libsrc nctest $(FORTRANSUB) ncgen ncdump

all:
    @$(MAKE) /V /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

test:
    @$(MAKE) /V /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

clean:
    @$(MAKE) /V /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

install: all
    @$(MAKE) /V /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(INSTSUBS)

$(ALLSUBS):	
	cd $@
    $(MAKE) /V /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) $(TARG)
	cd ..
