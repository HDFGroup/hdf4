#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

fort_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/hdf/test       \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(fort_INCLUDES) $(DEFINES)

DIST_SUBDIRS = examples

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

if HDF_BUILD_NETCDF
include_HEADERS = netcdf.inc netcdf.f90 mffunc.inc mffunc.f90
check_PROGRAMS = ftest hdftest tszip hdftest1
else
include_HEADERS = mffunc.inc mffunc.f90
check_PROGRAMS =  hdftest tszip hdftest1
endif

TEST_SCRIPT = testfortran.sh
check_SCRIPTS = testfortran.sh

if HDF_BUILD_NETCDF
ftest_SOURCES = ftest.f
ftest_LDADD = $(top_builddir)/hdf/test/forsupff.o $(top_builddir)/hdf/test/forsupf.o \
              $(LIBMFHDF) $(LIBHDF) @LIBS@
ftest_DEPENDENCIES = $(top_builddir)/hdf/test/forsupff.o $(top_builddir)/hdf/test/forsupf.o \
                     $(LIBMFHDF) $(LIBHDF)
endif

hdftest_SOURCES = hdftest.f
hdftest_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@
hdftest_DEPENDENCIES = testdir $(LIBMFHDF) $(LIBHDF)

hdftest1_SOURCES = hdftest1.f
hdftest1_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@
hdftest1_DEPENDENCIES = testdir $(LIBMFHDF) $(LIBHDF)

tszip_SOURCES = tszip.f
tszip_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@
tszip_DEPENDENCIES = testdir $(LIBMFHDF) $(LIBHDF)

testdir:
	-mkdir testdir

SUFFIXES = .inc .f90
.inc.f90:
	sed -e 's/^[cC]/!/' -e 's/^     [^ ]/     \&/' < $< > $*.f90

# Recurse into examples directory for examples tests.
installcheck-local:
	@(cd examples && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1;

# 'make install-all' also installs examples
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
	@$(MAKE) $(AM_MAKEFLAGS) install-examples
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall
	@$(MAKE) $(AM_MAKEFLAGS) uninstall-examples
	
# Install examples recursively
install-examples uninstall-examples:
	@@SETX@; for d in examples; do \
	    (cd $$d && $(MAKE) $(AM_MAKEFLAGS) $@) || exit 1; \
	done

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += test.nc copy.nc *.hdf testdir/testext.hdf

# Automake's distclean won't remove directories, so we can add an additional
# hook target which will do so during 'make distclean'.
distclean-local:
	-rm -rf testdir

DISTCLEANFILES = ftest.f jackets.c netcdf.inc mffunc.f90 netcdf.f90

include $(top_srcdir)/config/conclude.am
