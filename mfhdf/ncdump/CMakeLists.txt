cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_NCDUMP)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (ncdump_SRCS
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/dumplib.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/ncdump.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/vardata.c
    ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
)
  
ADD_EXECUTABLE (ncdump ${ncdump_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncdump ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncdump ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (ncdump ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

IF (BUILD_TESTING)
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME ncdump-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          test0.nc
          test1.nc
          test1.cdl
          test2.cdl
  )

  #-- Copy all the data files from the test directory into the source directory
  #MESSAGE (STATUS " Copying ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/test0.cdl to ${PROJECT_BINARY_DIR}/")
  ADD_CUSTOM_COMMAND (
      TARGET     ncdump
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/test0.cdl ${PROJECT_BINARY_DIR}/test0.cdl
  )

  IF (NCGEN_UTILITY)
#    ncdump test0.cdl
#    $<TARGET_FILE:ncgen> -o test0.nc -n test0.cdl
#    $<TARGET_FILE:ncdump> test0.nc > test1.cdl
#    $<TARGET_FILE:ncgen> -o test1.nc -n test1.cdl
#    $<TARGET_FILE:ncdump> -n test0 test1.nc > test2.cdl
#    ADD_TEST (
#        NAME  ncdump-comp
#        COMMAND  ${CMAKE_COMMAND}
#            -E compare test1.cdl test2.cdl
#    )
  ENDIF (NCGEN_UTILITY)

ENDIF (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        ncdump
    RUNTIME DESTINATION
        ${HDF4_INSTALL_BIN_DIR}/tools
    COMPONENT
        toolsapplications
)


