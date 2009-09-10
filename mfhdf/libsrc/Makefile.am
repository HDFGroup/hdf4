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

## Information for building the "libmfhdf.la" library
CSOURCES=array.c attr.c cdf.c dim.c file.c hdfsds.c iarray.c error.c    \
         globdef.c mfsd.c nssdc.c putget.c putgetg.c sharray.c string.c \
         var.c xdrposix.c

if HDF_BUILD_NETCDF
FSOURCES=../fortran/jackets.c ../fortran/mfsdf.c ../fortran/mfsdff.f
else
FSOURCES=../fortran/mfsdf.c ../fortran/mfsdff.f
endif

if HDF_BUILD_FORTRAN
libmfhdf_la_SOURCES = $(CSOURCES) $(FSOURCES)
else
libmfhdf_la_SOURCES = $(CSOURCES)
endif

if HDF_BUILD_XDR
XDRLIB = ../xdr/libxdr.la
libmfhdf_la_LIBADD = $(XDRLIB)
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
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=*.new *.hdf *.cdf *.cdl netcdf.h This*
