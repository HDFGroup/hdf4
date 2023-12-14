#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

TEST_PROG = h4ex_VD_create_vdatas h4ex_VD_write_mixed_vdata       \
	h4ex_VD_write_to_vdata h4ex_VD_create_onefield_vdatas         \
	h4ex_VD_read_from_vdata h4ex_VD_read_mixed_vdata              \
	h4ex_VD_get_vdata_info h4ex_VD_locate_vdata h4ex_VG_create_vgroup  \
	h4ex_VG_add_sds_to_vgroup h4ex_VG_insert_vdatas_to_vgroup     \
	h4ex_VG_set_get_vgroup_attr h4ex_VG_vgroup_contents           \
	h4ex_VG_get_vgroup_info h4ex_GR_create_and_write_image        \
	h4ex_GR_modify_image h4ex_GR_set_attribute h4ex_GR_get_attribute   \
	h4ex_GR_write_palette h4ex_GR_image_info h4ex_GR_read_image        \
	h4ex_AN_create_annotation h4ex_AN_get_annotation_info h4ex_AN_read_annotation
    # h4ex_VD_set_get_vdata_attr

# These are the Fortran example files to be installed
INSTALL_FILES= h4ex_VD_read_from_vdata.f h4ex_VD_read_mixed_vdata.f \
	h4ex_VD_write_mixed_vdata.f h4ex_VD_write_to_vdata.f            \
	h4ex_VD_create_onefield_vdatas.f h4ex_VD_create_vdatas.f        \
	h4ex_VD_get_vdata_info.f h4ex_VD_locate_vdata.f                 \
	h4ex_VG_insert_vdatas_to_vgroup.f h4ex_VG_set_get_vgroup_attr.f \
	h4ex_VG_vgroup_contents.f h4ex_VG_add_sds_to_vgroup.f           \
	h4ex_VG_create_vgroup.f h4ex_VG_get_vgroup_info.f               \
	h4ex_GR_create_and_write_image.f h4ex_GR_get_attribute.f        \
	h4ex_GR_image_info.f h4ex_GR_modify_image.f h4ex_GR_read_image.f     \
	h4ex_GR_read_palette.f h4ex_GR_set_attribute.f                  \
	h4ex_GR_write_palette.f h4ex_AN_create_annotation.f             \
	h4ex_AN_get_annotation_info.f h4ex_AN_read_annotation.f         \
	#h4ex_VD_set_get_vdata_attr.f

# Where to install Fortran example files
EXAMPLEDIR=$(prefix)/examples/fortran

# How to build Fortran programs using h4fc
$(EXTRA_PROG): $(H4FC)
	$(H4FC) $(H4FCFLAGS) $(FFLAGS) -o $@ $(srcdir)/$@.f;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal Fortran compiler, not h4fc.

# Fortran Programs
h4ex_VD_read_from_vdata: $(srcdir)/h4ex_VD_read_from_vdata.f
h4ex_VD_read_mixed_vdata: $(srcdir)/h4ex_VD_read_mixed_vdata.f
#h4ex_VD_set_get_vdata_attr: $(srcdir)/h4ex_VD_set_get_vdata_attr.f
h4ex_VD_write_mixed_vdata: $(srcdir)/h4ex_VD_write_mixed_vdata.f
h4ex_VD_write_to_vdata: $(srcdir)/h4ex_VD_write_to_vdata.f
h4ex_VD_create_onefield_vdatas: $(srcdir)/h4ex_VD_create_onefield_vdatas.f
h4ex_VD_create_vdatas: $(srcdir)/h4ex_VD_create_vdatas.f
h4ex_VD_get_vdata_info: $(srcdir)/h4ex_VD_get_vdata_info.f
h4ex_VD_locate_vdata: $(srcdir)/h4ex_VD_locate_vdata.f
h4ex_VG_insert_vdatas_to_vgroup: $(srcdir)/h4ex_VG_insert_vdatas_to_vgroup.f
h4ex_VG_set_get_vgroup_attr: $(srcdir)/h4ex_VG_set_get_vgroup_attr.f
h4ex_VG_vgroup_contents: $(srcdir)/h4ex_VG_vgroup_contents.f
h4ex_VG_add_sds_to_vgroup: $(srcdir)/h4ex_VG_add_sds_to_vgroup.f
h4ex_VG_create_vgroup: $(srcdir)/h4ex_VG_create_vgroup.f
h4ex_VG_get_vgroup_info: $(srcdir)/h4ex_VG_get_vgroup_info.f
h4ex_GR_create_and_write_image: $(srcdir)/h4ex_GR_create_and_write_image.f
h4ex_GR_get_attribute: $(srcdir)/h4ex_GR_get_attribute.f
h4ex_GR_image_info: $(srcdir)/h4ex_GR_image_info.f
h4ex_GR_modify_image: $(srcdir)/h4ex_GR_modify_image.f
h4ex_GR_read_image: $(srcdir)/h4ex_GR_read_image.f
h4ex_GR_read_palette: $(srcdir)/h4ex_GR_read_palette.f
h4ex_GR_set_attribute: $(srcdir)/h4ex_GR_set_attribute.f
h4ex_GR_write_palette: $(srcdir)/h4ex_GR_write_palette.f
h4ex_AN_create_annotation: $(srcdir)/h4ex_AN_create_annotation.f
h4ex_AN_get_annotation_info: $(srcdir)/h4ex_AN_get_annotation_info.f
h4ex_AN_read_annotation: $(srcdir)/h4ex_AN_read_annotation.f

include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am

DISTCLEANFILES = *.chkexe *.chklog
