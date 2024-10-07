#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
lib_INCLUDES = -I$(top_srcdir)/hdf/src        \
               -I$(top_srcdir)/mfhdf/src
AM_CPPFLAGS = $(lib_INCLUDES)

#############################################################################
##                          Library to build                               ##
#############################################################################

lib_LTLIBRARIES = libmfhdf.la

## Information for building the "libmfhdf.la" library
CSOURCES = array.c attr.c cdf.c dim.c file.c hdfsds.c iarray.c error.c	\
           globdef.c mfsd.c mfdatainfo.c nssdc.c putget.c putgetg.c		\
           sharray.c string.c var.c hdf_xdr.c

FSOURCES = $(top_srcdir)/mfhdf/fortran/mfsdf.c $(top_srcdir)/mfhdf/fortran/mfsdff.f

if HDF_BUILD_FORTRAN
libmfhdf_la_SOURCES = $(CSOURCES) $(FSOURCES)
else
libmfhdf_la_SOURCES = $(CSOURCES)
endif

libmfhdf_la_LIBADD = $(top_builddir)/hdf/src/libdf.la

include_HEADERS = mfhdf.h mfdatainfo.h

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

DISTCLEANFILES = *.new *.hdf *.cdf *.cdl This*
