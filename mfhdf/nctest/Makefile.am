#############################################################################
##                      Setup for building programs                        ##
#############################################################################

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
CPPFLAGS=@CPPFLAGS@ $(INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

TESTS = nctest
check_PROGRAMS = nctest

## Information for building the "ncgen" program
nctest_SOURCES = varget.c vargetg.c varput.c varputg.c vardef.c vartests.c  \
                 vputget.c vputgetg.c driver.c cdftests.c dimtests.c rec.c  \
                 atttests.c misctest.c add.c error.c emalloc.c val.c slabs.c
nctest_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a
nctest_DEPENPENDENCIES = $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a
