#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
ncdump_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(ncdump_INCLUDES) $(DEFINES)

## Add ncdump specific linker flags here
ncdump_LDFLAGS = $(LT_STATIC_EXEC) $(AM_LDFLAGS)

#############################################################################
##                          Programs to build                              ##
#############################################################################
TEST_SCRIPT = testncdump.sh
check_SCRIPTS = testncdump.sh

bin_PROGRAMS = ncdump
man1_MANS = ncdump.1

## Information for building the "ncdump" program
ncdump_SOURCES = dumplib.c ncdump.c vardata.c
ncdump_LDADD = $(LIBMFHDF) $(LIBHDF)
ncdump_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += test0.nc nctest0.cdl

DISTCLEANFILES =

include $(top_srcdir)/config/conclude.am
