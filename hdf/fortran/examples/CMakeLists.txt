cmake_minimum_required (VERSION 3.10)
project (HDF4_HDF_FORTRAN_EXAMPLES C Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Add debug information (intel Fortran : JB)
#-----------------------------------------------------------------------------
if (CMAKE_Fortran_COMPILER MATCHES ifort)
  if (WIN32)
    set (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE STRING "flags" FORCE)
    set (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "flags" FORCE)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
LINK_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_MFHDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set (examples
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
set (mf_examples
    VG_add_sds_to_vgroup
)
set (skip_examples
    VD_set_get_vdata_attr
)

foreach (example ${examples})
  add_executable (f_ex_${example} ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  target_include_directories (f_ex_${example}
      PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
  )
  target_compile_options(f_ex_${example}
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if(MSVC)
    set_property(TARGET f_ex_${example} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif()
  set_target_properties (f_ex_${example} PROPERTIES
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries (f_ex_${example} PRIVATE ${HDF4_SRC_FORTRAN_LIB_TARGET})
  else ()
    target_link_libraries (f_ex_${example} PRIVATE ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
  endif ()
endforeach ()

add_executable (f_ex_VG_add_sds_to_vgroup ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/VG_add_sds_to_vgroup.f)
target_include_directories (f_ex_VG_add_sds_to_vgroup
    PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
)
target_compile_options(f_ex_VG_add_sds_to_vgroup
    PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
)
if(MSVC)
  set_property(TARGET f_ex_VG_add_sds_to_vgroup PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
endif()
set_target_properties (f_ex_VG_add_sds_to_vgroup PROPERTIES
    LINKER_LANGUAGE Fortran
    Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
)
if (NOT BUILD_SHARED_LIBS)
  target_link_libraries (f_ex_VG_add_sds_to_vgroup PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET})
else ()
  target_link_libraries (f_ex_VG_add_sds_to_vgroup PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
endif ()

foreach (example ${skip_examples})
  add_executable (f_ex_${example} ${HDF4_HDF_FORTRAN_EXAMPLES_SOURCE_DIR}/${example}.f)
  target_include_directories (f_ex_${example}
      PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
  )
  target_compile_options(f_ex_${example}
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if(MSVC)
    set_property(TARGET f_ex_${example} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif()
  set_target_properties (f_ex_${example} PROPERTIES
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries (f_ex_${example} PRIVATE ${HDF4_SRC_FORTRAN_LIB_TARGET})
  else ()
    target_link_libraries (f_ex_${example} PRIVATE ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
  endif ()
endforeach ()

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()
