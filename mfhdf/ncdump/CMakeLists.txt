cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_NCDUMP C)

set (ncdump_SRCS
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/dumplib.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/ncdump.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/vardata.c
    ${HDF4_MFHDF_UTIL_DIR}/h4getopt.c
)

add_executable (ncdump ${ncdump_SRCS})
target_include_directories (ncdump PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_MFHDF_UTIL_DIR};${HDF4_BINARY_DIR}")
if (HDF4_BUILD_STATIC_TOOLS)
  TARGET_C_PROPERTIES (ncdump STATIC)
  target_link_libraries (ncdump PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (ncdump SHARED)
  target_link_libraries (ncdump PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (ncdump PROPERTIES COMPILE_DEFINITIONS "HDF")

set (H4_DEP_EXECUTABLES ncdump)

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
