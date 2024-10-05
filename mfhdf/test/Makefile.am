#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
test_INCLUDES=-I$(top_srcdir)/hdf/src         \
              -I$(top_srcdir)/mfhdf/src       \
              -I$(top_builddir)/mfhdf/src
AM_CPPFLAGS=$(test_INCLUDES)

#############################################################################
##                              Testing                                    ##
#############################################################################

TEST_PROG = hdfnctest hdftest
check_PROGRAMS = hdfnctest hdftest

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

CHECK_CLEANFILES += *.new *.hdf *.cdf *.cdl This* onedimmultivars.nc \
               onedimonevar.nc multidimvar.nc SD_externals

DISTCLEANFILES =

include $(top_srcdir)/config/conclude.am
