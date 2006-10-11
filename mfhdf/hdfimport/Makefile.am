#############################################################################
##                      Setup for building programs                        ##
#############################################################################

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                      Libraries to link against                          ##
#############################################################################

LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a @LIBS@

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hdfimport

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

check_PROGRAMS = hdfimporttest gen_sds_floats
check_SCRIPTS = testutil.sh

check: hdfimporttest$(EXEEXT)
	@$(SHELL) ./testutil.sh
