PROJECT (HDF4_MFHDF_NCDUMP)

#-----------------------------------------------------------------------------
# Setup output Directories 
#-----------------------------------------------------------------------------
IF(NOT HDF4_EXTERNALLY_CONFIGURED)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
ENDIF(NOT HDF4_EXTERNALLY_CONFIGURED)

INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR}/lib )
INCLUDE_DIRECTORIES( ${HDF4_MFHDF_SOURCE_DIR} )

ADD_DEFINITIONS(-DHDF)

IF(WIN32)
  ADD_DEFINITIONS(-DDOS_FS)
ENDIF(WIN32)


IF (HDF4_BUILD_XDR_LIB)
  IF(WIN32)
    add_definitions(-DNO_SYS_XDR_INC)
  ENDIF()
  INCLUDE_DIRECTORIES( ${HDF4_MFHDF_XDR_DIR} )
ENDIF ( HDF4_BUILD_XDR_LIB )

SET (ncdump_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/ncdump/dumplib.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncdump/ncdump.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncdump/vardata.c
  ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
  )
  
ADD_EXECUTABLE(ncdump ${ncdump_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(ncdump ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(ncdump ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

#-- Adding test for ncdump

# ADD_TEST(testncdump ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testncdump)
# 
# #-- Copy all the dat files from the test directory into the source directory
# SET (HDF4_REFERENCE_TEST_FILES
#   sds_szipped.dat
#   smallslice.0000.nc
#   test1.nc
#   testout.sav
# )
# 
# FOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )
#    SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
#    #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
#    ADD_CUSTOM_COMMAND (
#      TARGET     ncdumptest 
#      POST_BUILD
#      COMMAND    ${CMAKE_COMMAND}
#      ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
#      )
#      
# ENDFOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )


