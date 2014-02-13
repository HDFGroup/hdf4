#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

## Setup the different includes and preprocessor #defines we need.
amINCLUDES=-I$(top_srcdir)/hdf/src        \
         -I$(top_srcdir)/mfhdf/hdiff    \
         -I$(top_srcdir)/mfhdf/libsrc   \
         -I$(top_builddir)/mfhdf/libsrc
DEFINES=-DNDEBUG -DHDF
AM_CPPFLAGS=$(amINCLUDES) $(DEFINES)

## Add hrepack specific linker flags here
hrepack_LDFLAGS = $(LT_STATIC_EXEC) $(AM_LDFLAGS)

#############################################################################
##                          Programs to build                              ##
#############################################################################

bin_PROGRAMS = hrepack

hrepack_SOURCES = hrepack.c hrepack_an.c hrepack_gr.c                       \
                  hrepack_list.c hrepack_lsttable.c hrepack_main.c          \
                  hrepack_opttable.c hrepack_parse.c                        \
                  hrepack_sds.c hrepack_utils.c                             \
                  hrepack_vg.c hrepack_vs.c hrepack_dim.c
hrepack_LDADD = $(LIBMFHDF) $(LIBHDF)
hrepack_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

#############################################################################
##                   Testing -- Here there be dragons.                     ##
#############################################################################

check_PROGRAMS = hrepack_check test_hrepack
TEST_PROG = test_hrepack

test_hrepack_SOURCES = hrepack.c hrepack_an.c hrepack_gr.c                  \
                       hrepack_list.c hrepack_lsttable.c hrepack_opttable.c \
                       hrepack_parse.c hrepack_sds.c                        \
                       hrepack_utils.c hrepack_vg.c                         \
                       hrepack_dim.c                                        \
                       hrepack_vs.c  hrepacktst.c                           \
                       $(top_srcdir)/mfhdf/hdiff/hdiff.c                                     \
                       $(top_srcdir)/mfhdf/hdiff/hdiff_array.c $(top_srcdir)/mfhdf/hdiff/hdiff_gattr.c        \
                       $(top_srcdir)/mfhdf/hdiff/hdiff_gr.c $(top_srcdir)/mfhdf/hdiff/hdiff_list.c            \
                       $(top_srcdir)/mfhdf/hdiff/hdiff_mattbl.c $(top_srcdir)/mfhdf/hdiff/hdiff_misc.c        \
                       $(top_srcdir)/mfhdf/hdiff/hdiff_sds.c $(top_srcdir)/mfhdf/hdiff/hdiff_table.c          \
                       $(top_srcdir)/mfhdf/hdiff/hdiff_vs.c $(top_srcdir)/mfhdf/hdiff/hdiff_dim.c
test_hrepack_LDADD = $(LIBMFHDF) $(LIBHDF) -lm
test_hrepack_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

hrepack_check_SOURCES = hrepack_check.c
hrepack_check_LDADD = $(LIBMFHDF) $(LIBHDF)
hrepack_check_DEPENDENCIES = $(LIBMFHDF) $(LIBHDF)

#############################################################################
##                          And the cleanup                                ##
#############################################################################

CHECK_CLEANFILES += *.hdf

DISTCLEANFILES =

include $(top_srcdir)/config/conclude.am
