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

TESTS = nctest
check_PROGRAMS = nctest

# Information for building the "ncgen" program
nctest_SOURCES = add.c atttests.c cdftests.c dimtests.c driver.c emalloc.c  \
                 error.c misctest.c rec.c slabs.c val.c varget_unlim.c      \
                 varget.c vargetg.c varput.c varputg.c vardef.c vartests.c  \
                 vputget.c vputgetg.c
nctest_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
nctest_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

include $(top_srcdir)/config/conclude.am

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES=test2.nc test.nc
