cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_NCDUMP)

if (HDF4_BUILD_XDR_LIB)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif ()

set (ncdump_SRCS
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/dumplib.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/ncdump.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/vardata.c
    ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
)

if (NOT ONLY_SHARED_LIBS)
  add_executable (ncdump ${ncdump_SRCS})
  target_include_directories (ncdump PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (ncdump STATIC)
  target_link_libraries (ncdump PRIVATE ${HDF4_MF_LIB_TARGET})
  set_target_properties (ncdump PROPERTIES COMPILE_DEFINITIONS "HDF")

  set (H4_DEP_EXECUTABLES ncdump)
endif ()

if (BUILD_SHARED_LIBS)
  add_executable (ncdump-shared ${ncdump_SRCS})
  target_include_directories (ncdump-shared PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (ncdump-shared SHARED)
  target_link_libraries (ncdump-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (ncdump-shared PROPERTIES COMPILE_DEFINITIONS "HDF")

  set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} ncdump-shared)
endif ()

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
