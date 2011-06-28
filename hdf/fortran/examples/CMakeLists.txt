cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_FORTRAN_EXAMPLES C CXX Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Add debug information (intel Fortran : JB)
#-----------------------------------------------------------------------------
IF (CMAKE_Fortran_COMPILER MATCHES ifort)
  IF (WIN32)
    SET (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE "flags" STRING FORCE)
    SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE "flags" STRING FORCE)
  ENDIF (WIN32)
ENDIF (CMAKE_Fortran_COMPILER MATCHES ifort)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_HDFSOURCE_DIR}
)
LINK_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_MFHDF_BINARY_DIR}
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
    #VG_add_sds_to_vgroup
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
    #VD_set_get_vdata_attr
)
SET (mf_examples
    VG_add_sds_to_vgroup
)
SET (skip_examples
    VD_set_get_vdata_attr
)

IF (BUILD_TESTING)
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME HDF_FORTRAN_EXAMPLES-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          General_HDFobjects.hdf
          General_RImages.hdf
          General_Vdatas.hdf
          General_Vgroups.hdf
          Image_with_Palette.hdf
          Packed_Vdata.hdf
          Two_Vdatas.hdf
          Two_Vgroups.hdf
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS EXAMPLES)
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDF_FORTRAN_EXAMPLES-clearall-objects")
ENDIF (BUILD_TESTING)

FOREACH (example ${examples})
  ADD_EXECUTABLE (f_ex_${example} ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  TARGET_NAMING (f_ex_${example} ${LIB_TYPE})
  TARGET_FORTRAN_WIN_PROPERTIES (f_ex_${example} "")
  SET_TARGET_PROPERTIES (f_ex_${example} PROPERTIES LINKER_LANGUAGE Fortran)
  TARGET_LINK_LIBRARIES (f_ex_${example} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  IF (BUILD_TESTING)
    ADD_TEST (NAME f_extest_${example} COMMAND $<TARGET_FILE:f_ex_${example}>)
    IF (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (f_extest_${example} PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
    ELSE (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (f_extest_${example} PROPERTIES LABELS EXAMPLES)
    ENDIF (NOT "${last_test}" STREQUAL "")
    SET (last_test "f_extest_${example}")
  ENDIF (BUILD_TESTING)
ENDFOREACH (example ${examples})

ADD_EXECUTABLE (f_ex_VG_add_sds_to_vgroup ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/VG_add_sds_to_vgroup.f)
TARGET_NAMING (f_ex_VG_add_sds_to_vgroup ${LIB_TYPE})
TARGET_FORTRAN_WIN_PROPERTIES (f_ex_VG_add_sds_to_vgroup "")
SET_TARGET_PROPERTIES (f_ex_VG_add_sds_to_vgroup PROPERTIES LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES (f_ex_VG_add_sds_to_vgroup ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
IF (BUILD_TESTING)
  ADD_TEST (NAME f_extest_VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:f_ex_VG_add_sds_to_vgroup>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_extest_VG_add_sds_to_vgroup PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_extest_VG_add_sds_to_vgroup PROPERTIES LABELS EXAMPLES)
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "f_extest_VG_add_sds_to_vgroup")
ENDIF (BUILD_TESTING)

FOREACH (example ${skip_examples})
  ADD_EXECUTABLE (f_ex_${example} ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  TARGET_NAMING (f_ex_${example} ${LIB_TYPE})
  TARGET_FORTRAN_WIN_PROPERTIES (f_ex_${example} "")
  SET_TARGET_PROPERTIES (f_ex_${example} PROPERTIES LINKER_LANGUAGE Fortran)
  TARGET_LINK_LIBRARIES (f_ex_${example} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  IF (BUILD_TESTING)
    ADD_TEST (NAME f_extest_${example} COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_extest_${example}")
  ENDIF (BUILD_TESTING)
ENDFOREACH (example ${skip_examples})
