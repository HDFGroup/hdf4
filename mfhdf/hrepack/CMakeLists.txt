cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_HREPACK)

#INCLUDE_DIRECTORIES (${HDF4_SOURCE_DIR}/mfhdf/hdiff)

set (hrepack_SRCS
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_an.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_gr.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_list.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_lsttable.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_main.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_opttable.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_parse.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_sds.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_utils.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_vg.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_vs.c
    ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_dim.c
)

add_executable (hrepack ${hrepack_SRCS})
target_include_directories(hrepack
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
)
TARGET_C_PROPERTIES (hrepack STATIC)
target_link_libraries (hrepack PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (hrepack PROPERTIES COMPILE_DEFINITIONS "HDF")
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hrepack")

add_executable (hrepack_check ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_check.c)
target_include_directories(hrepack_check
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
)
TARGET_C_PROPERTIES (hrepack_check STATIC)
target_link_libraries (hrepack_check PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (hrepack_check PROPERTIES COMPILE_DEFINITIONS "HDF")

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

INSTALL_PROGRAM_PDB (hrepack ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

install (
    TARGETS
        hrepack hrepack_check
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
