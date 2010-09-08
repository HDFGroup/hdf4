cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_EXAMPLES C CXX)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
SET (examples
    VD_create_vdatas 
    VD_write_mixed_vdata
    VD_write_mixed_vdata_struct 
    VD_write_to_vdata
    VD_read_from_vdata 
    VD_read_mixed_vdata
    VD_set_get_vdata_attr 
    VD_create_onefield_vdatas
    VD_get_vdata_info 
    VD_locate_vdata 
    VG_create_vgroup
#    VG_add_sds_to_vgroup 
    VG_insert_vdatas_to_vgroup
    VG_set_get_vgroup_attr 
    VG_vgroup_contents
    VG_get_vgroup_info 
    GR_create_and_write_image
    GR_modify_image 
    GR_set_attribute 
    GR_get_attribute
    GR_write_palette 
    GR_image_info 
    GR_read_image
    AN_create_annotation 
    AN_get_annotation_info 
    AN_read_annotation
)

FOREACH (example ${examples})
  ADD_EXECUTABLE (hdf_${example} ${HDF4_HDF_EXAMPLES_SOURCE_DIR}/${example}.c)
  TARGET_NAMING (hdf_${example})
  TARGET_WIN_PROPERTIES (hdf_${example})
  TARGET_LINK_LIBRARIES (hdf_${example} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ADD_TEST (NAME hdftest_${example} COMMAND $<TARGET_FILE:hdf_${example}>)
ENDFOREACH (example ${examples})

ADD_EXECUTABLE (hdf_VG_add_sds_to_vgroup ${HDF4_HDF_EXAMPLES_SOURCE_DIR}/VG_add_sds_to_vgroup.c)
TARGET_NAMING (hdf_VG_add_sds_to_vgroup)
  TARGET_WIN_PROPERTIES (hdf_VG_add_sds_to_vgroup)
TARGET_LINK_LIBRARIES (hdf_VG_add_sds_to_vgroup ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ADD_TEST (NAME hdftest_VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:hdf_VG_add_sds_to_vgroup>)
