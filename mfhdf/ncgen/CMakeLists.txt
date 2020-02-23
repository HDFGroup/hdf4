cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_NCGEN)

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
)
if (WIN32)
  set (ncgen_SRCS ${ncgen_SRCS} ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c)
endif ()

if (NOT ONLY_SHARED_LIBS)
  add_executable (ncgen ${ncgen_SRCS})
  target_include_directories(ncgen PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (ncgen STATIC)
  target_link_libraries (ncgen PRIVATE ${HDF4_MF_LIB_TARGET})
  set_target_properties (ncgen PROPERTIES COMPILE_DEFINITIONS "HDF")

  set (H4_DEP_EXECUTABLES ncgen)
endif ()

if (BUILD_SHARED_LIBS)
  add_executable (ncgen-shared ${ncgen_SRCS})
  target_include_directories(ncgen-shared PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (ncgen-shared SHARED)
  target_link_libraries (ncgen-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (ncgen-shared PROPERTIES COMPILE_DEFINITIONS "HDF")

  set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} ncgen-shared)
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
