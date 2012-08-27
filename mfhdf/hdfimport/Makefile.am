#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                      Libraries to link against                          ##
#############################################################################

LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la @LIBS@

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hdfimport

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TESTS = hdfimporttest testutil.sh
check_PROGRAMS = hdfimporttest gen_sds_floats
check_SCRIPTS = testutil.sh

include $(top_srcdir)/config/conclude.am

