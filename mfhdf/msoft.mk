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
CFLAGS    = /c /AL /Za
DESTDIR   = C:
F77	  = fl
FFLAGS	  = /c /AL
WANT_FORTRAN_NETCDF = 1

# End of anticipated macros

# Macros for recursive make(1)s:
MY_MFLAGS	= \
	OS2="$(OS2)" \
	CC="$(CC)" \
	CFLAGS="$(CFLAGS)" \
	DESTDIR="$(DESTDIR)" \
	F77="$(F77)" \
	FFLAGS="$(FFLAGS)" \
	WANT_FORTRAN_NETCDF="$(WANT_FORTRAN_NETCDF)"

!IF $(WANT_FORTRAN_NETCDF)
FORTRANSUB       = fortran
!ENDIF

INSTSUBS           = xdr libsrc $(FORTRANSUB) ncgen ncdump
ALLSUBS            = xdr libsrc nctest $(FORTRANSUB) ncgen ncdump

all:
	@$(MAKE) /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

test:
	@$(MAKE) /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

clean:
	@$(MAKE) /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(ALLSUBS)

install: all
	@$(MAKE) /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) TARG=$@ $(INSTSUBS)

$(ALLSUBS):	
	cd $@
	$(MAKE) /F msoft.mk $(MAKEFLAGS) $(MY_MFLAGS) $(TARG)
	cd ..
