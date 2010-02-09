PROJECT (HDF4_MFHDF_HDIFF)

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

IF(WIN32)
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
ENDIF()

IF (HDF4_BUILD_XDR_LIB)
  IF(WIN32)
    add_definitions(-DNO_SYS_XDR_INC)
  ENDIF()
  INCLUDE_DIRECTORIES( ${HDF4_MFHDF_XDR_DIR} )
ENDIF ( HDF4_BUILD_XDR_LIB )

SET (hdiff_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_array.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_gr.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_list.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_main.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_mattbl.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_gattr.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_misc.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_sds.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_table.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_vs.c
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_dim.c
  ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
  )
  
ADD_EXECUTABLE(hdiff ${hdiff_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)


#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  SET (DART_TESTING_TIMEOUT 1200 CACHE INTEGER  
    "Timeout in seconds for each test (default 1200=20minutes)")
  ENABLE_TESTING()
  INCLUDE(CTest)

#-- Adding test for hdifftst

SET (hdifftst_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdifftst.c
  )
  
ADD_EXECUTABLE(hdifftst ${hdifftst_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdifftst ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdifftst ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(hdifftst ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdifftst)
 
#-- Copy all the dat files from the test directory into the source directory
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
#      TARGET     hdftest 
#      POST_BUILD
#      COMMAND    ${CMAKE_COMMAND}
#      ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
#      )
#      
# ENDFOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )

  include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
ENDIF ( BUILD_TESTING )

