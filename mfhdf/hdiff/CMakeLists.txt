cmake_minimum_required (VERSION 3.1.0)
project (HDF4_MFHDF_HDIFF)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
endif (WIN32)

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif (HDF4_BUILD_XDR_LIB)

set (hdiff_SRCS
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_array.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_gr.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_list.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_main.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_mattbl.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_gattr.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_misc.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_sds.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_table.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_vs.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_dim.c
    ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
)

add_executable(hdiff ${hdiff_SRCS})
TARGET_C_PROPERTIES (hdiff STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hdiff STATIC)

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------

INSTALL_PROGRAM_PDB (hdiff ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

INSTALL (
    TARGETS
        hdiff
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
