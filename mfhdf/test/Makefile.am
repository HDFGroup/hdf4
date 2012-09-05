#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc	\
	 -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                              Testing                                    ##
#############################################################################

TEST_PROG = cdftest hdfnctest hdftest
TEST_SCRIPT = testmfhdf.sh
check_PROGRAMS = cdftest hdfnctest hdftest
check_SCRIPTS = testmfhdf.sh

cdftest_SOURCES = cdftest.c
cdftest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la                \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

hdfnctest_SOURCES = hdfnctest.c tunlim.c tncunlim.c
hdfnctest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la              \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

hdftest_SOURCES = hdftest.c tutils.c tchunk.c tcomp.c tcoordvar.c	\
		  tdim.c temptySDSs.c tattributes.c tfile.c		\
		  tmixed_apis.c tnetcdf.c trank0.c tsd.c tsdsprops.c	\
		  tszip.c tattdatainfo.c tdatainfo.c tdatasizes.c
hdftest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la                \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=*.new *.hdf *.cdf *.cdl netcdf.h This* onedimmultivars.nc \
               onedimonevar.nc multidimvar.nc

include $(top_srcdir)/config/conclude.am
               