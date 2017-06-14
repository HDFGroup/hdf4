#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

# These are the Fortran test programs to build.
EXAMPLE_PROG=SD_create_sds SD_write_to_sds SD_write_slab SD_alter_sds_values SD_unlimited_sds \
	SD_mv_sds_to_external SD_read_from_sds SD_read_subsets SD_get_info \
	SD_find_sds_by_name SD_set_get_dim_info SD_dimscale_vs_sds SD_set_attr \
	SD_get_attr SD_compress_sds SD_chunking_example
TEST_PROG = $(EXAMPLE_PROG)

# These are the Fortran example files to be installed
INSTALL_FILES=SD_create_sds.f SD_write_to_sds.f SD_write_slab.f SD_alter_sds_values.f \
	SD_unlimited_sds.f SD_mv_sds_to_external.f SD_read_from_sds.f SD_read_subsets.f \
	SD_get_info.f SD_find_sds_by_name.f SD_set_get_dim_info.f SD_dimscale_vs_sds.f \
	SD_set_attr.f SD_get_attr.f SD_compress_sds.f SD_chunking_example.f

# Where to install example files
EXAMPLEDIR=${DESTDIR}$(exec_prefix)/share/hdf4_examples/fortran

# How to build Fortran programs using h4fc
$(EXTRA_PROG): $(H4FC)
	$(H4FC) $(H4FCFLAGS) $(FCFLAGS) -o $@ $(srcdir)/$@.f;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal Fortran compiler, not h4fc.
SD_create_sds: $(srcdir)/SD_create_sds.f
SD_write_to_sds: $(srcdir)/SD_write_to_sds.f
SD_write_slab: $(srcdir)/SD_write_slab.f
SD_alter_sds_values: $(srcdir)/SD_alter_sds_values.f
SD_unlimited_sds: $(srcdir)/SD_unlimited_sds.f
SD_mv_sds_to_external: $(srcdir)/SD_mv_sds_to_external.f
SD_read_from_sds: $(srcdir)/SD_read_from_sds.f
SD_read_subsets: $(srcdir)/SD_read_subsets.f
SD_get_info: $(srcdir)/SD_get_info.f
SD_find_sds_by_name: $(srcdir)/SD_find_sds_by_name.f
SD_set_get_dim_info: $(srcdir)/SD_set_get_dim_info.f
SD_dimscale_vs_sds: $(srcdir)/SD_dimscale_vs_sds.f
SD_set_attr: $(srcdir)/SD_set_attr.f
SD_get_attr: $(srcdir)/SD_get_attr.f
SD_compress_sds: $(srcdir)/SD_compress_sds.f
SD_chunking_example: $(srcdir)/SD_chunking_example.f

CHECK_CLEANFILES += ExternalSDS

DISTCLEANFILES = *.chkexe *.chklog

if BUILD_SHARED_SZIP_CONDITIONAL
LD_LIBRARY_PATH=$(LL_PATH)
endif

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am
