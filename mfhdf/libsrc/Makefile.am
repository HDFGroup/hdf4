#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am
include $(top_srcdir)/config/lt_vers.am

## Setup the different includes and preprocessor #defines we need.
lib_INCLUDES = -I$(top_srcdir)/hdf/src        \
               -I$(top_srcdir)/mfhdf/libsrc
DEFINES = -DHDF
AM_CPPFLAGS = $(lib_INCLUDES) $(XDRINC) $(DEFINES)

#############################################################################
##                          Library to build                               ##
#############################################################################

lib_LTLIBRARIES = libmfhdf.la

# Add libtool numbers to the HDF4 library (from config/lt_vers.am)
libmfhdf_la_LDFLAGS= -version-info $(LT_MF_VERS_INTERFACE):$(LT_MF_VERS_REVISION):$(LT_MF_VERS_AGE) $(AM_LDFLAGS)

## Information for building the "libmfhdf.la" library
CSOURCES = array.c attr.c cdf.c dim.c file.c hdfsds.c iarray.c error.c    \
         globdef.c mfsd.c mfdatainfo.c nssdc.c putget.c putgetg.c	\
	 sharray.c string.c var.c xdrposix.c

if HDF_BUILD_NETCDF
FSOURCES = $(top_builddir)/mfhdf/fortran/jackets.c $(top_srcdir)/mfhdf/fortran/mfsdf.c $(top_srcdir)/mfhdf/fortran/mfsdff.f
else
FSOURCES = $(top_srcdir)/mfhdf/fortran/mfsdf.c $(top_srcdir)/mfhdf/fortran/mfsdff.f
endif

if HDF_BUILD_FORTRAN
libmfhdf_la_SOURCES = $(CSOURCES) $(FSOURCES)
else
libmfhdf_la_SOURCES = $(CSOURCES)
endif

libmfhdf_la_LIBADD = $(XDRLIB) $(top_builddir)/hdf/src/libdf.la

if HDF_BUILD_NETCDF
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h netcdf.h mfhdfi.h mfdatainfo.h
else
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h hdf4_netcdf.h mfhdfi.h mfdatainfo.h
# mfdatainfo.h should be added conditionally only; should local_nc.h even be here? -BMR
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

DISTCLEANFILES = *.new *.hdf *.cdf *.cdl netcdf.h hdf4_netcdf.h This*
