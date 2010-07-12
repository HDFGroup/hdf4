cmake_minimum_required (VERSION 2.8)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------
PROJECT (HDF4_HDF_FORTRAN_EXAMPLES C CXX Fortran)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${CMAKE_Fortran_MODULE_DIRECTORY} ${HDF4_HDF_BINARY_DIR} ${HDF4_HDF_SOURCE_DIR}/src)
LINK_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
SET (examples
    VD_create_vdatas 
    VD_write_mixed_vdata
    VD_write_to_vdata 
    VD_create_onefield_vdatas
    VD_read_from_vdata 
    VD_read_mixed_vdata
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
    # VD_set_get_vdata_attr
)

FOREACH (example ${examples})
  ADD_EXECUTABLE (f_ex_${example} ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  TARGET_NAMING (f_ex_${example})
  IF (WIN32)
    IF (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (f_ex_${example}
            PROPERTIES
                COMPILE_FLAGS "/dll"
                LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
      ENDIF (MSVC)
    ENDIF (BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES (f_ex_${example} PROPERTIES LINKER_LANGUAGE Fortran)
  ENDIF (WIN32)
  TARGET_LINK_LIBRARIES (f_ex_${example} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ADD_TEST (NAME f_extest_${example} COMMAND $<TARGET_FILE:f_ex_${example}>)
ENDFOREACH (example ${examples})

ADD_EXECUTABLE (f_ex_VG_add_sds_to_vgroup ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/VG_add_sds_to_vgroup.f)
TARGET_NAMING (f_ex_VG_add_sds_to_vgroup)
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_ex_VG_add_sds_to_vgroup
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE"
      ) 
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES (f_ex_VG_add_sds_to_vgroup PROPERTIES LINKER_LANGUAGE Fortran)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (f_ex_VG_add_sds_to_vgroup ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ADD_TEST (NAME f_extest_VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:f_ex_VG_add_sds_to_vgroup>)
