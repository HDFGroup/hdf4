cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_HDFIMPORT C)

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
endif ()

set (hdfimport_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.c
)

add_executable (hdfimport ${hdfimport_SRCS})
target_include_directories(hdfimport PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_BINARY_DIR}")
if (HDF4_BUILD_STATIC_TOOLS)
  TARGET_C_PROPERTIES (hdfimport STATIC)
  target_link_libraries (hdfimport PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (hdfimport SHARED)
  target_link_libraries (hdfimport PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (hdfimport PROPERTIES COMPILE_DEFINITIONS "HDF")
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdfimport")

set (H4_DEP_EXECUTABLES hdfimport)

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------

foreach (exec ${H4_DEP_EXECUTABLES})
  INSTALL_PROGRAM_PDB (${exec} ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)
endforeach ()

install (
    TARGETS
        ${H4_DEP_EXECUTABLES}
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)

