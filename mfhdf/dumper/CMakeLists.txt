cmake_minimum_required (VERSION 3.1.0)
project (HDF4_MFHDF_DUMPER)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

add_definitions (-DHDF)

set (hdp_SRCS
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_dump.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_gr.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_list.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_rig.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_sds.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_util.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vd.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vg.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/show.c
)

add_executable (hdp ${hdp_SRCS})
if (WIN32)
  add_definitions (-DDOS_FS)
endif (WIN32)
if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  target_link_libraries (hdp ${HDF4_MF_XDR_LIB_TARGET})
endif (HDF4_BUILD_XDR_LIB)
TARGET_C_PROPERTIES (hdp STATIC " " " ")
target_link_libraries (hdp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
TARGET_NAMING (hdp STATIC)
if (WIN32)
  set_target_properties (hdp PROPERTIES LINK_FLAGS "/STACK:10000000")
endif (WIN32)

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

INSTALL_PROGRAM_PDB (hdp ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

INSTALL (
    TARGETS
        hdp
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
