cmake_minimum_required (VERSION 3.2.2)
project (HDF4_MFHDF_HDIFF)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
endif ()

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif ()
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif ()

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
else ()
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
TARGET_NAMING (hdiff STATIC)
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdiff")

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

INSTALL_PROGRAM_PDB (hdiff ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

install (
    TARGETS
        hdiff
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
