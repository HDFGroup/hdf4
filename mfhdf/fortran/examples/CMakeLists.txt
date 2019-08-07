cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_FORTRAN_EXAMPLES C Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

LINK_DIRECTORIES (
    ${HDF4_MFHDF_BINARY_DIR}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set (examples
    SD_create_sds
    SD_write_to_sds
    SD_write_slab
    SD_alter_sds_values
    SD_unlimited_sds
    SD_mv_sds_to_external
    SD_read_from_sds
    SD_read_subsets
    #SD_get_info
    SD_find_sds_by_name
    SD_set_get_dim_info
    #SD_dimscale_vs_sds
    SD_set_attr
    #SD_get_attr
    SD_compress_sds
    SD_chunking_example
)
set (skip_examples
    SD_get_info
    SD_dimscale_vs_sds
    SD_get_attr
)

foreach (example ${examples})
  add_executable (f_exmf_${example} ${HDF4_MFHDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  target_include_directories (f_exmf_${example}
      PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
  )
  target_compile_options(f_exmf_${example}
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if(MSVC)
    set_property(TARGET f_exmf_${example} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif()
  set_target_properties (f_exmf_${example} PROPERTIES
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries (f_exmf_${example} PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET})
  else ()
    target_link_libraries (f_exmf_${example} PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET})
  endif ()
endforeach ()

foreach (example ${skip_examples})
  add_executable (f_exmf_${example} ${HDF4_MFHDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  target_include_directories (f_exmf_${example}
      PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
  )
  target_compile_options(f_exmf_${example}
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if(MSVC)
    set_property(TARGET f_exmf_${example} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif()
  set_target_properties (f_exmf_${example} PROPERTIES
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries (f_exmf_${example} PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET})
  else ()
    target_link_libraries (f_exmf_${example} PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET})
  endif ()
endforeach ()

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()
