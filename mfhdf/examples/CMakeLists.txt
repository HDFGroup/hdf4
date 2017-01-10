cmake_minimum_required (VERSION 3.2.2)
project (HDF4_MFHDF_EXAMPLES)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set (examples
    SD_create_sds
    SD_write_to_sds
    SD_write_slab
    SD_alter_sds_values
    SD_unlimited_sds
    SD_compress_sds
    SD_mv_sds_to_external
    SD_read_from_sds
    SD_read_subsets
    SD_get_info
    SD_find_sds_by_name
    SD_set_get_dim_info
    SD_dimscale_vs_sds
    SD_set_attr
    SD_get_attr
    SD_chunking_example
)

foreach (example ${examples})
  add_executable (mf_${example} ${HDF4_MFHDF_EXAMPLES_SOURCE_DIR}/${example}.c)
  TARGET_NAMING (mf_${example} STATIC)
  TARGET_C_PROPERTIES (mf_${example} STATIC " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (mf_${example} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  else ()
    target_link_libraries (mf_${example} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  endif ()
endforeach (example ${examples})

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()
