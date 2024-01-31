#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

TEST_PROG = h4ex_SD_create_sds h4ex_SD_write_to_sds h4ex_SD_write_slab             \
	h4ex_SD_alter_sds_values h4ex_SD_unlimited_sds h4ex_SD_compress_sds        \
	h4ex_SD_mv_sds_to_external h4ex_SD_read_from_sds h4ex_SD_read_subsets      \
	h4ex_SD_get_info h4ex_SD_find_sds_by_name h4ex_SD_set_get_dim_info         \
	h4ex_SD_dimscale_vs_sds h4ex_SD_set_attr h4ex_SD_get_attr h4ex_SD_chunking_example

INSTALL_FILES = h4ex_SD_create_sds.c h4ex_SD_write_to_sds.c h4ex_SD_write_slab.c        \
	h4ex_SD_alter_sds_values.c h4ex_SD_unlimited_sds.c h4ex_SD_compress_sds.c       \
	h4ex_SD_mv_sds_to_external.c h4ex_SD_read_from_sds.c h4ex_SD_read_subsets.c     \
	h4ex_SD_get_info.c h4ex_SD_find_sds_by_name.c h4ex_SD_set_get_dim_info.c        \
	h4ex_SD_dimscale_vs_sds.c h4ex_SD_set_attr.c h4ex_SD_get_attr.c h4ex_SD_chunking_example.c

EXAMPLEDIR=$(prefix)/examples/c

# How to build programs using h4cc
$(EXTRA_PROG): $(H4CC)
	$(H4CC) $(H4CCFLAGS) $(CFLAGS) -o $@ $(srcdir)/$@.c;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal C++ compiler, not h4cc.  This is
# an inelegant way of solving the problem, unfortunately.
create_sds: $(srcdir)/h4ex_SD_create_sds.c
write_to_sds: $(srcdir)/h4ex_SD_write_to_sds.c
write_slab: $(srcdir)/h4ex_SD_write_slab.c
alter_sds_values: $(srcdir)/h4ex_SD_alter_sds_values.c
unlimited_sds: $(srcdir)/h4ex_SD_unlimited_sds.c
compress_sds: $(srcdir)/h4ex_SD_compress_sds.c
mv_sds_to_external: $(srcdir)/h4ex_SD_mv_sds_to_external.c
read_from_sds: $(srcdir)/h4ex_SD_read_from_sds.c
read_subsets: $(srcdir)/h4ex_SD_read_subsets.c
get_info: $(srcdir)/h4ex_SD_get_info.c
find_sds_by_name: $(srcdir)/h4ex_SD_find_sds_by_name.c
set_get_dim_info: $(srcdir)/h4ex_SD_set_get_dim_info.c
dimscale_vs_sds: $(srcdir)/h4ex_SD_dimscale_vs_sds.c
set_attr: $(srcdir)/h4ex_SD_set_attr.c
get_attr: $(srcdir)/h4ex_SD_get_attr.c
chunking_example: $(srcdir)/h4ex_SD_chunking_example.c

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = *.chklog *.chkexe ExternalSDS .deps
