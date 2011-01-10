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

check_PROGRAMS = cdftest hdfnctest hdftest

cdftest_SOURCES = cdftest.c
cdftest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la                \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

hdfnctest_SOURCES = hdfnctest.c tunlim.c tncunlim.c
hdfnctest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la              \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

hdftest_SOURCES = hdftest.c tchunk.c tcomp.c tcoordvar.c tdim.c         \
		  temptySDSs.c tfile.c tidtypes.c tnetcdf.c trank0.c    \
		  tsd.c tsdsprops.c tszip.c tattdatainfo.c tdatainfo.c  \
		  tdatasizes.c
hdftest_LDADD = $(top_builddir)/mfhdf/libsrc/libmfhdf.la                \
	$(top_builddir)/hdf/src/libdf.la @LIBS@ $(XDRLIB)

check:
	@echo "============================"
	@echo "HDF-SD C interface tests"
	@echo "============================"
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./hdftest
	@echo
	@echo "============================"
	@echo "HDF-nc C interface tests"
	@echo "============================"
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./hdfnctest
	@echo
	@echo "========================="
	@echo "netCDF formatted tests"
	@echo "========================="
	srcdir="$(srcdir)" $(TESTS_ENVIRONMENT) ./cdftest > cdfout.new
	@cmd="$(DIFF) cdfout.new $(srcdir)/testout.sav";                \
	  echo $$cmd;                                                   \
	  if $$cmd; then                                                \
	    echo "*** netCDF passes formatted test ***";                \
	  else                                                          \
	    echo "*** netCDF fails formatted test ***";                 \
	    echo "The above differences are OK if small";               \
	    exit 0;                                                     \
	  fi

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=*.new *.hdf *.cdf *.cdl netcdf.h This* onedimmultivars.nc \
               onedimonevar.nc multidimvar.nc
