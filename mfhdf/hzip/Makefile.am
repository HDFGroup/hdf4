#############################################################################
##                      Setup for building programs                        ##
#############################################################################

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/hdiff    \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
CPPFLAGS=@CPPFLAGS@ $(INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hzip

hzip_SOURCES = copy.c hdfzip.c hdfzip_main.c list.c parse.c table.c utils.c
hzip_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a
hzip_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

TESTS = hziptst
check_PROGRAMS = hziptst

hziptst_SOURCES = add.c copy.c hdfzip.c hdfziptst_main.c list.c parse.c     \
                  table.c utils.c verify.c ../hdiff/dumplib.c               \
                  ../hdiff/hdiff.c ../hdiff/hdiff_gattr.c                   \
                  ../hdiff/hdiff_misc.c ../hdiff/hdiff_sdattr.c             \
                  ../hdiff/hdiff_array.c ../hdiff/hdiff_list.c              \
                  ../hdiff/hdiff_mattbl.c ../hdiff/hdiff_table.c
hziptst_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a
hziptst_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

noinst_PROGRAMS = hzipck

hzipck_SOURCES = hdfzipck.c
hzipck_LDADD = ../libsrc/libmfhdf.a $(top_builddir)/hdf/src/libdf.a
hzipck_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.a ../libsrc/libmfhdf.a

check: hziptst hzip
	@srcdir="$(srcdir)" $(srcdir)/testhzip.sh
