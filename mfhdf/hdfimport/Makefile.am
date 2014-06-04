#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
hdfimport_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(hdfimport_INCLUDES) $(DEFINES)

## Add hdfimport specific linker flags here
hdfimport_LDFLAGS = $(LT_STATIC_EXEC) $(AM_LDFLAGS)

#############################################################################
##                      Libraries to link against                          ##
#############################################################################

LDADD = $(LIBMFHDF) $(LIBHDF) @LIBS@

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hdfimport

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TEST_PROG = hdfimporttest
TEST_SCRIPT = testutil.sh
check_PROGRAMS = hdfimporttest gen_sds_floats
check_SCRIPTS = testutil.sh

include $(top_srcdir)/config/conclude.am
