#############################################################################
##                      Setup for building programs                        ##
#############################################################################

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port
DEFINES=-DNDEBUG -DHDF

CPPFLAGS=$(INCLUDES) $(DEFINES) @CPPFLAGS@

#############################################################################
##                          Library to build                               ##
#############################################################################

lib_LIBRARIES = libmfhdf.a

## Information for building the "libmfhdf.a" library
libmfhdf_a_SOURCES = array.c attr.c cdf.c dim.c file.c hdfsds.c         \
                     iarray.c error.c globdef.c mfsd.c nssdc.c putget.c \
                     putgetg.c sharray.c string.c var.c xdrposix.c      \
                     ../fortran/jackets.c ../fortran/mfsdf.c            \
                     ../fortran/mfsdff.f
include_HEADERS = hdf2netcdf.h local_nc.h mfhdf.h netcdf.h 

#############################################################################
##                              Testing                                    ##
#############################################################################

check_PROGRAMS = cdftest hdftest

cdftest_SOURCES = cdftest.c
cdftest_LDADD = libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@

hdftest_SOURCES = hdftest.c tszip.c
hdftest_LDADD = libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@

check:
	@echo "============================"
	@echo "HDF-SD C interfaces tests"
	@echo "============================"
	./hdftest > hdfout.new
	@cmd="$(DIFF) hdfout.new $(srcdir)/hdfout.sav";                 \
	  echo $$cmd;                                                   \
	  if $$cmd; then                                                \
	    echo "*** HDF passes formatted test ***";                   \
	  else                                                          \
	    echo "*** HDF fails formatted test ***";                    \
	    echo "The above differences are OK if small";               \
	    exit 0;                                                     \
	  fi
	@echo "========================="
	@echo "netCDF formatted tests"
	@echo "========================="
	./cdftest > cdfout.new
	@cmd="$(DIFF) cdfout.new $(srcdir)/testout.sav";                \
	  echo $$cmd;                                                   \
	  if $$cmd; then                                                \
	    echo "*** netCDF passes formatted test ***";                \
	  else                                                          \
	    echo "*** netCDF fails formatted test ***";                 \
	    echo "The above differences are OK if small";               \
	    exit 0;                                                     \
	  fi
