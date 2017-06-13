#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

EXAMPLE_PROG = SD_create_sds SD_write_to_sds SD_write_slab             \
	SD_alter_sds_values SD_unlimited_sds SD_compress_sds        \
	SD_mv_sds_to_external SD_read_from_sds SD_read_subsets      \
	SD_get_info SD_find_sds_by_name SD_set_get_dim_info         \
	SD_dimscale_vs_sds SD_set_attr SD_get_attr SD_chunking_example
TEST_PROG = $(EXAMPLE_PROG)

INSTALL_FILES = SD_create_sds.c SD_write_to_sds.c SD_write_slab.c        \
	SD_alter_sds_values.c SD_unlimited_sds.c SD_compress_sds.c       \
	SD_mv_sds_to_external.c SD_read_from_sds.c SD_read_subsets.c     \
	SD_get_info.c SD_find_sds_by_name.c SD_set_get_dim_info.c        \
	SD_dimscale_vs_sds.c SD_set_attr.c SD_get_attr.c SD_chunking_example.c

EXAMPLEDIR=${DESTDIR}$(exec_prefix)/share/hdf4_examples/c

# How to build programs using h4cc
$(EXTRA_PROG): $(H4CC)
	$(H4CC) $(H4CCFLAGS) $(CFLAGS) -o $@ $(srcdir)/$@.c;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal C++ compiler, not h4cc.  This is
# an inelegant way of solving the problem, unfortunately.
create_sds: $(srcdir)/SD_create_sds.c
write_to_sds: $(srcdir)/SD_write_to_sds.c
write_slab: $(srcdir)/SD_write_slab.c
alter_sds_values: $(srcdir)/SD_alter_sds_values.c
unlimited_sds: $(srcdir)/SD_unlimited_sds.c
compress_sds: $(srcdir)/SD_compress_sds.c
mv_sds_to_external: $(srcdir)/SD_mv_sds_to_external.c
read_from_sds: $(srcdir)/SD_read_from_sds.c
read_subsets: $(srcdir)/SD_read_subsets.c
get_info: $(srcdir)/SD_get_info.c
find_sds_by_name: $(srcdir)/SD_find_sds_by_name.c
set_get_dim_info: $(srcdir)/SD_set_get_dim_info.c
dimscale_vs_sds: $(srcdir)/SD_dimscale_vs_sds.c
set_attr: $(srcdir)/SD_set_attr.c
get_attr: $(srcdir)/SD_get_attr.c
chunking_example: $(srcdir)/SD_chunking_example.c

CHECK_CLEANFILES += ExternalSDS

DISTCLEANFILES = *.chklog *.chkexe .deps

if BUILD_SHARED_SZIP_CONDITIONAL
LD_LIBRARY_PATH=$(LL_PATH)
endif

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am
