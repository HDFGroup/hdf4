cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_NCGEN)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR} ${HDF4_MFHDFSOURCE_DIR} ${HDF4_MFHDF_NCGEN_SOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS(-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

IF (WIN32 AND NOT CYGWIN)
  SET (ncgen_tab_SRCS
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msofttab.h
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msofttab.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msoftyy.c
  )
  SET (NCGEN_UTILITY 1 PARENT_SCOPE)
ELSE (WIN32 AND NOT CYGWIN)
  FIND_PACKAGE (FLEX)
  IF (FLEX_FOUND)
      FLEX_TARGET (NCGEN_SCANNER ncgen.l ${CMAKE_CURRENT_BINARY_DIR}/ncgenyy.c)
  ENDIF (FLEX_FOUND)

  FIND_PACKAGE (BISON)
  IF (BISON_FOUND)
    BISON_TARGET (NCGEN_PARSER ncgen.y ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c COMPILE_FLAGS -y)
  ENDIF (BISON_FOUND)

  IF (BISON_NCGEN_PARSER_DEFINED AND FLEX_NCGEN_SCANNER_DEFINED)
    SET (NCGEN_UTILITY 1 PARENT_SCOPE)
  ENDIF (BISON_NCGEN_PARSER_DEFINED AND FLEX_NCGEN_SCANNER_DEFINED)
#  ADD_FLEX_BISON_DEPENDENCY (NCGEN_SCANNER NCGEN_PARSER)
  SET (ncgen_tab_SRCS
      ${CMAKE_CURRENT_BINARY_DIR}/ncgenyy.c
      ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c
      ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.h
  )
ENDIF (WIN32 AND NOT CYGWIN)
 
#IF (${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c) 
  SET (ncgen_SRCS
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/close.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/escapes.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/generate.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/genlib.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/getfill.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/init.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/load.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/main.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/ncgen.h
  )
  
  ADD_EXECUTABLE (ncgen ${ncgen_SRCS} ${ncgen_tab_SRCS})
  TARGET_C_PROPERTIES (ncgen " " " ")
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (ncgen ${LIB_TYPE})

IF (BUILD_TESTING)
  INCLUDE (CMakeTests.cmake)
ENDIF (BUILD_TESTING)

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
#ENDIF (${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c)
