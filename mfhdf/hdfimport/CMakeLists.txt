cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_HDFIMPORT)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

if (WIN32)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
endif (WIN32)

ADD_DEFINITIONS (-DHDF)

if (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
endif (WIN32)

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif (HDF4_BUILD_XDR_LIB)

set (hdfimport_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.c
)
  
ADD_EXECUTABLE (hdfimport ${hdfimport_SRCS})
TARGET_C_PROPERTIES (hdfimport " " " ")
if (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hdfimport ${LIB_TYPE})

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

INSTALL_PROGRAM_PDB (hdfimport ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

INSTALL (
    TARGETS
        hdfimport
    RUNTIME DESTINATION
        ${HDF4_INSTALL_TOOLS_BIN_DIR}
    COMPONENT
        toolsapplications
)
