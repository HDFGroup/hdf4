#############################################################################
##                      Setup for building programs                        ##
#############################################################################

include $(top_srcdir)/config/commence.am

DEFINES=-DNDEBUG -DHDF

#############################################################################
##                          Programs to build                              ##
#############################################################################

EXAMPLE_PROG = VD_create_vdatas VD_write_mixed_vdata		\
	VD_write_mixed_vdata_struct VD_write_to_vdata		\
	VD_read_from_vdata VD_read_mixed_vdata			\
	VD_set_get_vdata_attr VD_create_onefield_vdatas		\
	VD_get_vdata_info VD_locate_vdata VG_create_vgroup	\
	VG_add_sds_to_vgroup VG_insert_vdatas_to_vgroup		\
	VG_set_get_vgroup_attr VG_vgroup_contents		\
	VG_get_vgroup_info GR_create_and_write_image		\
	GR_modify_image GR_set_attribute GR_get_attribute	\
	GR_write_palette GR_read_palette GR_image_info          \
	GR_read_image GR_write_chunks GR_read_chunks            \
	AN_create_annotation AN_get_annotation_info AN_read_annotation
TEST_PROG = $(EXAMPLE_PROG)

# These are the C example files to be installed
INSTALL_FILES= VD_read_from_vdata.c VD_read_mixed_vdata.c	\
	VD_set_get_vdata_attr.c VD_write_mixed_vdata.c		\
	VD_write_mixed_vdata_struct.c VD_write_to_vdata.c	\
	VD_create_onefield_vdatas.c VD_create_vdatas.c		\
	VD_get_vdata_info.c VD_locate_vdata.c			\
	VG_insert_vdatas_to_vgroup.c VG_set_get_vgroup_attr.c	\
	VG_vgroup_contents.c VG_add_sds_to_vgroup.c		\
	VG_create_vgroup.c VG_get_vgroup_info.c			\
	GR_create_and_write_image.c GR_get_attribute.c		\
	GR_image_info.c GR_modify_image.c GR_read_image.c	\
	GR_read_palette.c GR_set_attribute.c			\
	GR_write_palette.c GR_write_chunks.c GR_read_chunks.c	\
	AN_create_annotation.c AN_get_annotation_info.c		\
	AN_read_annotation.c

INSTALL_SCRIPT_FILES = run-c-ex.sh

INSTALL_TOP_SCRIPT_FILES = run-all-ex.sh
INSTALL_TOP_FILES = README

# Where to install example files
EXAMPLEDIR=${DESTDIR}$(exec_prefix)/share/hdf4_examples/c
EXAMPLETOPDIR=${DESTDIR}$(exec_prefix)/share/hdf4_examples

# How to build C programs using h4cc
$(EXTRA_PROG): $(H4CC)
	$(H4CC) $(H4CCFLAGS) $(CFLAGS) -o $@ $(srcdir)/$@.c;

# List dependencies for each program.  Normally, automake would take
# care of this for us, but if we tell automake about the programs it
# will try to build them with the normal C compiler, not h4cc.

# C Programs
VD_read_from_vdata: $(srcdir)/VD_read_from_vdata.c
VD_read_mixed_vdata: $(srcdir)/VD_read_mixed_vdata.c
VD_set_get_vdata_attr: $(srcdir)/VD_set_get_vdata_attr.c
VD_write_mixed_vdata: $(srcdir)/VD_write_mixed_vdata.c
VD_write_mixed_vdata_struct: $(srcdir)/VD_write_mixed_vdata_struct.c
VD_write_to_vdata: $(srcdir)/VD_write_to_vdata.c
VD_create_onefield_vdatas: $(srcdir)/VD_create_onefield_vdatas.c
VD_create_vdatas: $(srcdir)/VD_create_vdatas.c
VD_get_vdata_info: $(srcdir)/VD_get_vdata_info.c
VD_locate_vdata: $(srcdir)/VD_locate_vdata.c
VG_insert_vdatas_to_vgroup: $(srcdir)/VG_insert_vdatas_to_vgroup.c
VG_set_get_vgroup_attr: $(srcdir)/VG_set_get_vgroup_attr.c
VG_vgroup_contents: $(srcdir)/VG_vgroup_contents.c
VG_add_sds_to_vgroup: $(srcdir)/VG_add_sds_to_vgroup.c
VG_create_vgroup: $(srcdir)/VG_create_vgroup.c
VG_get_vgroup_info: $(srcdir)/VG_get_vgroup_info.c
GR_create_and_write_image: $(srcdir)/GR_create_and_write_image.c
GR_get_attribute: $(srcdir)/GR_get_attribute.c
GR_image_info: $(srcdir)/GR_image_info.c
GR_modify_image: $(srcdir)/GR_modify_image.c
GR_read_chunks: $(srcdir)/GR_read_chunks.c
GR_read_image: $(srcdir)/GR_read_image.c
GR_read_palette: $(srcdir)/GR_read_palette.c
GR_set_attribute: $(srcdir)/GR_set_attribute.c
GR_write_chunks: $(srcdir)/GR_write_chunks.c
GR_write_palette: $(srcdir)/GR_write_palette.c
AN_create_annotation: $(srcdir)/AN_create_annotation.c
AN_get_annotation_info: $(srcdir)/AN_get_annotation_info.c
AN_read_annotation: $(srcdir)/AN_read_annotation.c

DISTCLEANFILES = *.chkexe *.chklog

if BUILD_SHARED_SZIP_CONDITIONAL
LD_LIBRARY_PATH=$(LL_PATH)
endif
    
include $(top_srcdir)/config/examples.am
include $(top_srcdir)/config/conclude.am
