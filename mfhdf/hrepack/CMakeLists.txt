cmake_minimum_required (VERSION 3.2.2)
project (HDF4_MFHDF_HREPACK)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_SOURCE_DIR}/mfhdf/hdiff)

add_definitions (-DHDF)

if (WIN32)
  add_definitions (-DDOS_FS)
endif ()

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif ()
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif ()

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
TARGET_C_PROPERTIES (hrepack STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else ()
  target_link_libraries (hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
TARGET_NAMING (hrepack STATIC)

add_executable (hrepack_check ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepack_check.c)
TARGET_C_PROPERTIES (hrepack_check STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hrepack_check ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else ()
  target_link_libraries (hrepack_check ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
TARGET_NAMING (hrepack_check STATIC)
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hrepack")

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
