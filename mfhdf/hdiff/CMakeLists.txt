cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_HDIFF C)

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
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
    ${HDF4_MFHDF_UTIL_DIR}/h4getopt.c
)

add_executable(hdiff ${hdiff_SRCS})
target_include_directories(hdiff PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_BINARY_DIR};${HDF4_MFHDF_UTIL_DIR}")
if (HDF4_BUILD_STATIC_TOOLS)
  TARGET_C_PROPERTIES (hdiff STATIC)
  target_link_libraries(hdiff PRIVATE ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
else ()
  TARGET_C_PROPERTIES (hdiff SHARED)
  target_link_libraries(hdiff PRIVATE ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${LINK_LIBS})
endif ()
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdiff")

set (H4_DEP_EXECUTABLES hdiff)

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
