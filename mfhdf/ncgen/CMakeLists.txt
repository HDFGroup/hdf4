cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_NCGEN C)

INCLUDE_DIRECTORIES (
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ncgen_SRCS
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/close.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/escapes.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/generate.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/genlib.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/getfill.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/init.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/load.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/main.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/ncgentab.c
    ${HDF4_MFHDF_UTIL_DIR}/h4getopt.c
)

add_executable (ncgen ${ncgen_SRCS})
target_include_directories(ncgen PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_MFHDF_UTIL_DIR};${HDF4_BINARY_DIR}")
if (HDF4_BUILD_STATIC_TOOLS)
  TARGET_C_PROPERTIES (ncgen STATIC)
  target_link_libraries (ncgen PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (ncgen SHARED)
  target_link_libraries (ncgen PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (ncgen PROPERTIES COMPILE_DEFINITIONS "HDF")

set (H4_DEP_EXECUTABLES ncgen)

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------

foreach (exec ${H4_DEP_EXECUTABLES})
  INSTALL_PROGRAM_PDB (${exec} ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)
endforeach ()

install (
    TARGETS
        ${H4_DEP_EXECUTABLES}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
