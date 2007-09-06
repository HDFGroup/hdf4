#############################################################################
##                      Setup for building programs                        ##
#############################################################################

INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/hdf/test       \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

if HDF_BUILD_NETCDF
include_HEADERS = netcdf.inc netcdf.f90 mffunc.inc mffunc.f90
check_PROGRAMS = ftest hdftest tszip
else
include_HEADERS = mffunc.inc mffunc.f90
check_PROGRAMS =  hdftest tszip
endif

if HDF_BUILD_NETCDF
ftest_SOURCES = ftest.f
ftest_LDADD = $(top_builddir)/hdf/test/forsupff.o $(top_builddir)/hdf/test/forsupf.o \
              ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@
ftest_DEPENDENCIES = $(top_builddir)/hdf/test/forsupff.o $(top_builddir)/hdf/test/forsupf.o \
                     $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a
endif

hdftest_SOURCES = hdftest.f
hdftest_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@
hdftest_DEPENDENCIES = testdir $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

tszip_SOURCES = tszip.f
tszip_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@
tszip_DEPENDENCIES = testdir $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

testdir:
	-mkdir testdir

SUFFIXES = .inc .f90
.inc.f90:
	sed -e 's/^[cC]/!/' -e 's/^     [^ ]/     \&/' < $< > $*.f90
check:
	@echo "==================================="
	@echo "HDF-SD Fortran interfaces tests"
	@echo "==================================="
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./hdftest
if HDF_BUILD_NETCDF
	@echo "==================================="
	@echo "HDF-NetCDF Fortran interfaces tests"
	@echo "==================================="
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./ftest
	@echo "==================================="
endif
	@echo "SZIP Fortran interfaces tests"
	@echo "==================================="
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./tszip

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = ftest.f jackets.c netcdf.inc mffunc.f90 netcdf.f90 test.nc copy.nc *.hdf testdir/testext.hdf
