#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                          Library to build                               ##
#############################################################################

lib_LTLIBRARIES = libmfhdf.la

## Information for building the "libmfhdf.a" library
CSOURCES=array.c attr.c cdf.c dim.c file.c hdfsds.c iarray.c error.c    \
         globdef.c mfsd.c nssdc.c putget.c putgetg.c sharray.c string.c \
         var.c xdrposix.c

if HDF_BUILD_NETCDF
FSOURCES=../fortran/jackets.c ../fortran/mfsdf.c ../fortran/mfsdff.f
else
FSOURCES=../fortran/mfsdf.c ../fortran/mfsdff.f
endif

XDRSOURCES=../xdr/xdr.c ../xdr/xdrfloat.c ../xdr/xdrstdio.c ../xdr/xdrarray.c

if HDF_BUILD_FORTRAN

if HDF_BUILD_XDR
libmfhdf_la_SOURCES = $(CSOURCES) $(FSOURCES) $(XDRSOURCES)
else
libmfhdf_la_SOURCES = $(CSOURCES) $(FSOURCES)
endif

else

if HDF_BUILD_XDR
libmfhdf_la_SOURCES = $(CSOURCES) $(XDRSOURCES)
else
libmfhdf_la_SOURCES = $(CSOURCES)
endif

endif

if HDF_BUILD_NETCDF
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h netcdf.h mfhdfi.h
else
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h hdf4_netcdf.h mfhdfi.h
endif

# The following is a workaround. Since Fortran is included in this 
# Makefile.am, automake will always try to use the Fortran linker, even when
# fortran has been disabled in configure. The Fortran linker gets confused 
# when shared libraries are enabled, so when Fortran is not enabled,
# this replaces the Fortran link macro with the C link macro.
if HDF_BUILD_FORTRAN
    # do nothing
else
    F77LINK = $(LINK)
endif

#############################################################################
##                              Testing                                    ##
#############################################################################

check_PROGRAMS = cdftest hdftest

cdftest_SOURCES = cdftest.c
cdftest_LDADD = libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@

hdftest_SOURCES = hdftest.c tchunk.c tcomp.c tcoordvar.c tdim.c		\
		  temptySDSs.c tfile.c tidtypes.c tnetcdf.c trank0.c	\
		  tsd.c tsdsprops.c tszip.c tdatasizes.c
hdftest_LDADD = libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@

check:
	@echo "============================"
	@echo "HDF-SD C interfaces tests"
	@echo "============================"
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./hdftest > hdfout.new
	@cmd="$(DIFF) hdfout.new $(srcdir)/hdfout.sav";                 \
	  echo $$cmd;                                                   \
	  if $$cmd; then                                                \
	    echo "*** HDF passes formatted test ***";                   \
	  else                                                          \
	    echo "*** HDF fails formatted test ***";                    \
	    echo "The above differences are OK if small";               \
	    exit 0;                                                     \
	  fi
	@echo "========================="
	@echo "netCDF formatted tests"
	@echo "========================="
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./cdftest > cdfout.new
	@cmd="$(DIFF) cdfout.new $(srcdir)/testout.sav";                \
	  echo $$cmd;                                                   \
	  if $$cmd; then                                                \
	    echo "*** netCDF passes formatted test ***";                \
	  else                                                          \
	    echo "*** netCDF fails formatted test ***";                 \
	    echo "The above differences are OK if small";               \
	    exit 0;                                                     \
	  fi

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=*.new *.hdf *.cdf *.cdl netcdf.h
