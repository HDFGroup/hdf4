PROJECT (HDF4_MFHDF_HDFIMPORT)

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
  foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
      string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
  endforeach(flag_var)
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
ENDIF()

ADD_DEFINITIONS(-DHDF)

IF (HDF4_BUILD_XDR_LIB)
  IF(WIN32)
    add_definitions(-DNO_SYS_XDR_INC)
  ENDIF()
  INCLUDE_DIRECTORIES( ${HDF4_MFHDF_XDR_DIR} )
ENDIF ( HDF4_BUILD_XDR_LIB )

SET (hdfimport_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/hdfimport.c
  )
  
ADD_EXECUTABLE(hdfimport ${hdfimport_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfimport ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfimport ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)


#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  SET (DART_TESTING_TIMEOUT 1200 CACHE INTEGER  
    "Timeout in seconds for each test (default 1200=20minutes)")
  ENABLE_TESTING()
  INCLUDE(CTest)

#-- Adding test for hdfimporttest

SET (hdfimporttest_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/hdfimporttest.c
  )
  
ADD_EXECUTABLE(hdfimporttest ${hdfimporttest_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfimporttest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfimporttest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(hdfimporttest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfimporttest)
 
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

#-- Adding test for gen_sds_floats

SET (gen_sds_floats_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/gen_sds_floats.c
  )
  
ADD_EXECUTABLE(gen_sds_floats ${gen_sds_floats_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(gen_sds_floats ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(gen_sds_floats ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(gen_sds_floats ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen_sds_floats)

  include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
ENDIF ( BUILD_TESTING )

