cmake_minimum_required (VERSION 3.10)
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
  if (NOT BUILD_SHARED_LIBS)
    TARGET_C_PROPERTIES (mf_${example} STATIC)
    target_link_libraries (mf_${example} PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (mf_${example} SHARED)
    target_link_libraries (mf_${example} PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
endforeach (example ${examples})

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()
