#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
nctest_INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(nctest_INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

TEST_PROG = nctest
check_PROGRAMS = nctest

# Information for building the "ncgen" program
nctest_SOURCES = add.c atttests.c cdftests.c dimtests.c driver.c emalloc.c  \
                 error.c misctest.c rec.c slabs.c val.c varget_unlim.c      \
                 varget.c vargetg.c varput.c varputg.c vardef.c vartests.c  \
                 vputget.c vputgetg.c
nctest_LDADD = $(LIBMFHDF) $(LIBHDF)
nctest_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += test2.nc test.nc

DISTCLEANFILES =

include $(top_srcdir)/config/conclude.am
