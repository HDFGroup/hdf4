#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

EXAMPLE_PROG = VD_create_vdatas VD_write_mixed_vdata       \
	VD_write_to_vdata VD_create_onefield_vdatas         \
	VD_read_from_vdata VD_read_mixed_vdata              \
	VD_get_vdata_info VD_locate_vdata VG_create_vgroup  \
	VG_add_sds_to_vgroup VG_insert_vdatas_to_vgroup     \
	VG_set_get_vgroup_attr VG_vgroup_contents           \
	VG_get_vgroup_info GR_create_and_write_image        \
	GR_modify_image GR_set_attribute GR_get_attribute   \
	GR_write_palette GR_image_info GR_read_image        \
	AN_create_annotation AN_get_annotation_info AN_read_annotation
    # VD_set_get_vdata_attr
TEST_PROG = $(EXAMPLE_PROG)

# These are the Fortran example files to be installed
INSTALL_FILES= VD_read_from_vdata.f VD_read_mixed_vdata.f \
	VD_write_mixed_vdata.f VD_write_to_vdata.f            \
	VD_create_onefield_vdatas.f VD_create_vdatas.f        \
	VD_get_vdata_info.f VD_locate_vdata.f                 \
	VG_insert_vdatas_to_vgroup.f VG_set_get_vgroup_attr.f \
	VG_vgroup_contents.f VG_add_sds_to_vgroup.f           \
	VG_create_vgroup.f VG_get_vgroup_info.f               \
	GR_create_and_write_image.f GR_get_attribute.f        \
	GR_image_info.f GR_modify_image.f GR_read_image.f     \
	GR_read_palette.f GR_set_attribute.f                  \
	GR_write_palette.f AN_create_annotation.f             \
	AN_get_annotation_info.f AN_read_annotation.f         \
	#VD_set_get_vdata_attr.f

INSTALL_SCRIPT_FILES = run-fortran-ex.sh

# Where to install Fortran example files
EXAMPLEDIR=${DESTDIR}$(exec_prefix)/share/hdf4_examples/fortran

# How to build Fortran programs using h4fc
$(EXTRA_PROG): $(H4FC)
	$(H4FC) $(H4FCFLAGS) $(FFLAGS) -o $@ $(srcdir)/$@.f;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal Fortran compiler, not h4fc.

# Fortran Programs
VD_read_from_vdata: $(srcdir)/VD_read_from_vdata.f
VD_read_mixed_vdata: $(srcdir)/VD_read_mixed_vdata.f
#VD_set_get_vdata_attr: $(srcdir)/VD_set_get_vdata_attr.f
VD_write_mixed_vdata: $(srcdir)/VD_write_mixed_vdata.f
VD_write_to_vdata: $(srcdir)/VD_write_to_vdata.f
VD_create_onefield_vdatas: $(srcdir)/VD_create_onefield_vdatas.f
VD_create_vdatas: $(srcdir)/VD_create_vdatas.f
VD_get_vdata_info: $(srcdir)/VD_get_vdata_info.f
VD_locate_vdata: $(srcdir)/VD_locate_vdata.f
VG_insert_vdatas_to_vgroup: $(srcdir)/VG_insert_vdatas_to_vgroup.f
VG_set_get_vgroup_attr: $(srcdir)/VG_set_get_vgroup_attr.f
VG_vgroup_contents: $(srcdir)/VG_vgroup_contents.f
VG_add_sds_to_vgroup: $(srcdir)/VG_add_sds_to_vgroup.f
VG_create_vgroup: $(srcdir)/VG_create_vgroup.f
VG_get_vgroup_info: $(srcdir)/VG_get_vgroup_info.f
GR_create_and_write_image: $(srcdir)/GR_create_and_write_image.f
GR_get_attribute: $(srcdir)/GR_get_attribute.f
GR_image_info: $(srcdir)/GR_image_info.f
GR_modify_image: $(srcdir)/GR_modify_image.f
GR_read_image: $(srcdir)/GR_read_image.f
GR_read_palette: $(srcdir)/GR_read_palette.f
GR_set_attribute: $(srcdir)/GR_set_attribute.f
GR_write_palette: $(srcdir)/GR_write_palette.f
AN_create_annotation: $(srcdir)/AN_create_annotation.f
AN_get_annotation_info: $(srcdir)/AN_get_annotation_info.f
AN_read_annotation: $(srcdir)/AN_read_annotation.f

DISTCLEANFILES = *.chkexe *.chklog

if BUILD_SHARED_SZIP_CONDITIONAL
LD_LIBRARY_PATH=$(LL_PATH)
endif

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am
