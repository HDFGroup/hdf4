#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

fort_INCLUDES=-I$(top_srcdir)/hdf/src     \
         -I$(top_srcdir)/hdf/test         \
         -I$(top_srcdir)/mfhdf/src        \
         -I$(top_builddir)/mfhdf/src
DEFINES=-DHDF
AM_CPPFLAGS=$(fort_INCLUDES) $(DEFINES)

DIST_SUBDIRS =

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

include_HEADERS = mffunc.inc mffunc.f90
check_PROGRAMS =  hdftest tszip hdftest1

TEST_SCRIPT = testfortran.sh
check_SCRIPTS = testfortran.sh

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

# 'make install-all'
install-all:
	@$(MAKE) $(AM_MAKEFLAGS) install
uninstall-all:
	@$(MAKE) $(AM_MAKEFLAGS) uninstall

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += test.nc copy.nc *.hdf testdir/testext.hdf

# Automake's distclean won't remove directories, so we can add an additional
# hook target which will do so during 'make distclean'.
distclean-local:
	-rm -rf testdir

DISTCLEANFILES = mffunc.f90

include $(top_srcdir)/config/conclude.am
