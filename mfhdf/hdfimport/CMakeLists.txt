cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_HDFIMPORT)

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
endif ()

set (hdfimport_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.c
)

add_executable (hdfimport ${hdfimport_SRCS})
target_include_directories(hdfimport
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
)
TARGET_C_PROPERTIES (hdfimport STATIC)
target_link_libraries (hdfimport PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (hdfimport PROPERTIES COMPILE_DEFINITIONS "HDF")
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdfimport")

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

INSTALL_PROGRAM_PDB (hdfimport ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

install (
    TARGETS
        hdfimport
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
