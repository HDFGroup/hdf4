#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
INCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/hdiff    \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_srcdir)/mfhdf/port     \
         -I../libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(INCLUDES) $(DEFINES)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hrepack

hrepack_SOURCES = hrepack.c hrepack_an.c hrepack_gr.c                       \
                  hrepack_list.c hrepack_lsttable.c hrepack_main.c          \
                  hrepack_opttable.c hrepack_parse.c                        \
                  hrepack_sds.c hrepack_sdutil.c hrepack_utils.c            \
                  hrepack_vg.c hrepack_vs.c hrepack_dim.c
hrepack_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
hrepack_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

check_PROGRAMS = hrepack_check test_hrepack
TESTS = test_hrepack

test_hrepack_SOURCES = hrepack.c hrepack_an.c hrepack_gr.c                  \
                       hrepack_list.c hrepack_lsttable.c hrepack_opttable.c \
                       hrepack_parse.c hrepack_sds.c                        \
                       hrepack_sdutil.c hrepack_utils.c hrepack_vg.c        \
                       hrepack_dim.c                                        \
                       hrepack_vs.c hrepacktst_add.c hrepacktst.c           \
                       hrepacktst_verify.c ../hdiff/hdiff.c                 \
                       ../hdiff/hdiff_array.c ../hdiff/hdiff_gattr.c        \
                       ../hdiff/hdiff_gr.c ../hdiff/hdiff_list.c            \
                       ../hdiff/hdiff_mattbl.c ../hdiff/hdiff_misc.c        \
                       ../hdiff/hdiff_sds.c ../hdiff/hdiff_table.c          \
                       ../hdiff/hdiff_vs.c ../hdiff/hdiff_dim.c
test_hrepack_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
test_hrepack_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

hrepack_check_SOURCES = hrepack_check.c
hrepack_check_LDADD = ../libsrc/libmfhdf.la $(top_builddir)/hdf/src/libdf.la
hrepack_check_DEPENDENCIES = $(top_builddir)/hdf/src/libdf.la ../libsrc/libmfhdf.la

#############################################################################
##                          And the cleanup                                ##
#############################################################################

DISTCLEANFILES = *.hdf
