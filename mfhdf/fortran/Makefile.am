#############################################################################
##                      Setup for building programs                        ##
#############################################################################

INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

include_HEADERS = netcdf.inc netcdf.f90
check_PROGRAMS = ftest

ftest_SOURCES = ftest.f
ftest_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@
ftest_DEPENPENDENCIES = $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

SUFFIXES = .inc .f90
.inc.f90:
	sed -e 's/^[cC]/!/' -e 's/^     [^ ]/     \&/' < $< > $*.f90

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = ftest.f jackets.c netcdf.inc netcdf.f90
