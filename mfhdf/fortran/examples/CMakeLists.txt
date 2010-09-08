cmake_minimum_required (VERSION 2.8)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------
PROJECT (HDF4_MFHDF_FORTRAN_EXAMPLES C CXX Fortran)

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
    ${HDF4_MFHDF_BINARY_DIR}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
SET (examples
    SD_create_sds 
    SD_write_to_sds 
    SD_write_slab 
    SD_alter_sds_values 
    SD_unlimited_sds
    SD_mv_sds_to_external 
    SD_read_from_sds 
    SD_read_subsets 
    SD_get_info
    SD_find_sds_by_name 
    SD_set_get_dim_info 
    SD_dimscale_vs_sds 
    SD_set_attr
    SD_get_attr 
    SD_compress_sds 
    SD_chunking_example
)

FOREACH (example ${examples})
  ADD_EXECUTABLE (f_exmf_${example} ${HDF4_MFHDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  TARGET_NAMING (f_exmf_${example})
  TARGET_FORTRAN_WIN_PROPERTIES (f_exmf_${example})
  SET_TARGET_PROPERTIES (f_exmf_${example} PROPERTIES LINKER_LANGUAGE Fortran)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_exmf_${example} ${HDF4_MF_FORTRAN_LIB_TARGET}  ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_exmf_${example} ${HDF4_MF_FORTRAN_LIB_TARGET}  ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  ADD_TEST (NAME f_exmftest_${example} COMMAND $<TARGET_FILE:f_exmf_${example}>)
ENDFOREACH (example ${examples})
