cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_NCGEN)

INCLUDE_DIRECTORIES (
    ${HDF4_HDFSOURCE_DIR}
    ${HDF4_MFHDFSOURCE_DIR}
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_DEFINITIONS (-DHDF)

if (WIN32)
  ADD_DEFINITIONS(-DDOS_FS)
endif (WIN32)

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif (HDF4_BUILD_XDR_LIB)

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
endif (WIN32)
 
ADD_EXECUTABLE (ncgen ${ncgen_SRCS})
TARGET_C_PROPERTIES (ncgen " " " ")
if (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
else (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (ncgen ${LIB_TYPE})

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL_PROGRAM_PDB (ncgen ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

INSTALL (
    TARGETS
        ncgen
    RUNTIME DESTINATION
        ${HDF4_INSTALL_TOOLS_BIN_DIR}
    COMPONENT
        toolsapplications
)
