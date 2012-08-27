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
##                          Programs to build                              ##
#############################################################################
TESTS = testncdump.sh
check_SCRIPTS = testncdump.sh

bin_PROGRAMS = ncdump
man1_MANS = ncdump.1

## Information for building the "ncdump" program
ncdump_SOURCES = dumplib.c ncdump.c vardata.c
ncdump_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
ncdump_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

include $(top_srcdir)/config/conclude.am

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=test0.nc
