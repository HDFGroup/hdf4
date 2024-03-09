#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
test_INCLUDES=-I$(top_srcdir)/hdf/src         \
              -I$(top_srcdir)/mfhdf/src       \
              -I$(top_builddir)/mfhdf/src
DEFINES=-DHDF
AM_CPPFLAGS=$(test_INCLUDES) $(DEFINES)

#############################################################################
##                              Testing                                    ##
#############################################################################

TEST_PROG = cdftest hdfnctest hdftest
check_PROGRAMS = cdftest hdfnctest hdftest

cdftest_SOURCES = cdftest.c
cdftest_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@

hdfnctest_SOURCES = hdfnctest.c tutils.c tncvargetfill.c tunlim.c	\
		    tncunlim.c
hdfnctest_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@

hdftest_SOURCES = hdftest.c tutils.c tchunk.c tcomp.c tcoordvar.c	\
		  tdim.c temptySDSs.c tattributes.c texternal.c tfile.c	\
		  tmixed_apis.c tnetcdf.c trank0.c tsd.c tsdsprops.c	\
		  tszip.c tattdatainfo.c tdatainfo.c tdatasizes.c
hdftest_LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += *.new *.hdf *.cdf *.cdl netcdf.h This* onedimmultivars.nc \
               onedimonevar.nc multidimvar.nc SD_externals

DISTCLEANFILES =

include $(top_srcdir)/config/conclude.am
