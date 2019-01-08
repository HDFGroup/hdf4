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

add_executable (ncdump ${ncdump_SRCS})
target_include_directories (ncdump
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
)
TARGET_C_PROPERTIES (ncdump STATIC )
target_link_libraries (ncdump PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (ncdump PROPERTIES
    COMPILE_DEFINITIONS "HDF"
)
if (WIN32)
  set_property (TARGET ncdump APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS;SWAP")
  if (HDF4_BUILD_XDR_LIB)
    set_property (TARGET ncdump APPEND PROPERTY COMPILE_DEFINITIONS "NO_SYS_XDR_INC")
  endif ()
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

INSTALL_PROGRAM_PDB (ncdump ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

INSTALL (
    TARGETS
        ncdump
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)


