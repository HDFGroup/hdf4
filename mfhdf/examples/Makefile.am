#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

TEST_PROG = create_sds write_to_sds write_slab             \
	alter_sds_values unlimited_sds compress_sds            \
	mv_sds_to_external read_from_sds read_subsets get_info \
	find_sds_by_name set_get_dim_info dimscale_vs_sds      \
	set_attr get_attr chunking_example

INSTALL_FILES = create_sds.c write_to_sds.c write_slab.c           \
	alter_sds_values.c unlimited_sds.c compress_sds.c              \
	mv_sds_to_external.c read_from_sds.c read_subsets.c get_info.c \
	find_sds_by_name.c set_get_dim_info.c dimscale_vs_sds.c        \
	set_attr.c get_attr.c chunking_example.c

EXAMPLEDIR=$(prefix)/examples/c

# How to build programs using h4cc
$(EXTRA_PROG): $(H4CC)
	$(H4CC) $(H4CCFLAGS) $(CFLAGS) -o $@ $(srcdir)/$@.c;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal C++ compiler, not h4cc.  This is
# an inelegant way of solving the problem, unfortunately.
create_sds: $(srcdir)/create_sds.c
write_to_sds: $(srcdir)/write_to_sds.c
write_slab: $(srcdir)/write_slab.c
alter_sds_values: $(srcdir)/alter_sds_values.c
unlimited_sds: $(srcdir)/unlimited_sds.c
compress_sds: $(srcdir)/compress_sds.c
mv_sds_to_external: $(srcdir)/mv_sds_to_external.c
read_from_sds: $(srcdir)/read_from_sds.c
read_subsets: $(srcdir)/read_subsets.c
get_info: $(srcdir)/get_info.c
find_sds_by_name: $(srcdir)/find_sds_by_name.c
set_get_dim_info: $(srcdir)/set_get_dim_info.c
dimscale_vs_sds: $(srcdir)/dimscale_vs_sds.c
set_attr: $(srcdir)/set_attr.c
get_attr: $(srcdir)/get_attr.c
chunking_example: $(srcdir)/chunking_example.c

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = *.chklog *.chkexe ExternalSDS .deps
