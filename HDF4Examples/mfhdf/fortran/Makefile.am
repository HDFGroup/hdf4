#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

# These are the Fortran test programs to build.
TEST_PROG=h4ex_SD_create_sds h4ex_SD_write_to_sds h4ex_SD_write_slab h4ex_SD_alter_sds_values h4ex_SD_unlimited_sds \
	h4ex_SD_mv_sds_to_external h4ex_SD_read_from_sds h4ex_SD_read_subsets h4ex_SD_get_info \
	h4ex_SD_find_sds_by_name h4ex_SD_set_get_dim_info h4ex_SD_dimscale_vs_sds h4ex_SD_set_attr \
	h4ex_SD_get_attr h4ex_SD_compress_sds h4ex_SD_chunking_example

# These are the Fortran example files to be installed
INSTALL_FILES=h4ex_SD_create_sds.f h4ex_SD_write_to_sds.f h4ex_SD_write_slab.f h4ex_SD_alter_sds_values.f \
	h4ex_SD_unlimited_sds.f h4ex_SD_mv_sds_to_external.f h4ex_SD_read_from_sds.f h4ex_SD_read_subsets.f \
	h4ex_SD_get_info.f h4ex_SD_find_sds_by_name.f h4ex_SD_set_get_dim_info.f h4ex_SD_dimscale_vs_sds.f \
	h4ex_SD_set_attr.f h4ex_SD_get_attr.f h4ex_SD_compress_sds.f h4ex_SD_chunking_example.f

# Where to install example files
EXAMPLEDIR=$(prefix)/examples/fortran

# How to build Fortran programs using h4fc
$(EXTRA_PROG): $(H4FC)
	$(H4FC) $(H4FCFLAGS) $(FCFLAGS) -o $@ $(srcdir)/$@.f;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal Fortran compiler, not h4fc.
h4ex_SD_create_sds: $(srcdir)/h4ex_SD_create_sds.f
h4ex_SD_write_to_sds: $(srcdir)/h4ex_SD_write_to_sds.f
h4ex_SD_write_slab: $(srcdir)/h4ex_SD_write_slab.f
h4ex_SD_alter_sds_values: $(srcdir)/h4ex_SD_alter_sds_values.f
h4ex_SD_unlimited_sds: $(srcdir)/h4ex_SD_unlimited_sds.f
h4ex_SD_mv_sds_to_external: $(srcdir)/h4ex_SD_mv_sds_to_external.f
h4ex_SD_read_from_sds: $(srcdir)/h4ex_SD_read_from_sds.f
h4ex_SD_read_subsets: $(srcdir)/h4ex_SD_read_subsets.f
h4ex_SD_get_info: $(srcdir)/h4ex_SD_get_info.f
h4ex_SD_find_sds_by_name: $(srcdir)/h4ex_SD_find_sds_by_name.f
h4ex_SD_set_get_dim_info: $(srcdir)/h4ex_SD_set_get_dim_info.f
h4ex_SD_dimscale_vs_sds: $(srcdir)/h4ex_SD_dimscale_vs_sds.f
h4ex_SD_set_attr: $(srcdir)/h4ex_SD_set_attr.f
h4ex_SD_get_attr: $(srcdir)/h4ex_SD_get_attr.f
h4ex_SD_compress_sds: $(srcdir)/h4ex_SD_compress_sds.f
h4ex_SD_chunking_example: $(srcdir)/h4ex_SD_chunking_example.f

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = *.chkexe *.chklog ExternalSDS
