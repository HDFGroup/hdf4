cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_DUMPER)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

SET (hdp_SRCS
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
  
ADD_EXECUTABLE (hdp ${hdp_SRCS})
IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)
IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  TARGET_LINK_LIBRARIES (hdp ${HDF4_MF_XDR_LIB_TARGET})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_C_PROPERTIES (hdp " " " ")
TARGET_LINK_LIBRARIES (hdp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
TARGET_NAMING (hdp ${LIB_TYPE})

IF (BUILD_TESTING)
  INCLUDE (CMakeTests.cmake)
ENDIF (BUILD_TESTING)

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
    RUNTIME DESTINATION
        ${HDF4_INSTALL_TOOLS_BIN_DIR}
    COMPONENT
        toolsapplications
)
