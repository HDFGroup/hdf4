PROJECT (HDF4_MFHDF_TEST)

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

SET (hdftest_SRCS
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdftest.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tchunk.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcomp.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcoordvar.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdatasizes.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdim.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/temptySDSs.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tfile.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tidtypes.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tnetcdf.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/trank0.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsd.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsdsprops.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tszip.c
  )

#-- Adding test for hdftest
ADD_EXECUTABLE(hdftest ${hdftest_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdftest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${HDF4_MF_XDR_LIB_NAME}-static ${LINK_LIBS} )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdftest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(hdftest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdftest)
#IF(WIN32)
#SET_TEST_PROPERTIES(hdftest PROPERTIES ENVIRONMENT "srcdir=${PROJECT_BINARY_DIR}")
#ENDIF(WIN32)

#-- Copy all the dat files from the test directory into the source directory
SET (HDF4_REFERENCE_TEST_FILES
  sds_szipped.dat
  smallslice.0000.nc
  test1.nc
  testout.sav
)

FOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
     TARGET     hdftest 
     POST_BUILD
     COMMAND    ${CMAKE_COMMAND}
     ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
     )
     
ENDFOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )

#-- Adding test for cdftest
ADD_EXECUTABLE(cdftest ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(cdftest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(cdftest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(cdftest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cdftest)

SET (hdfnctest_SRCS
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdfnctest.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tunlim.c
  ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncunlim.c
  )

#-- Adding test for hdfnctest
ADD_EXECUTABLE(hdfnctest ${hdfnctest_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfnctest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdfnctest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST(hdfnctest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfnctest)

#-- Adding test for xdrtest
IF (HDF4_BUILD_XDR_LIB)
  ADD_EXECUTABLE(xdrtest ${HDF4_MFHDF_XDR_DIR}/xdrtest.c)
  TARGET_LINK_LIBRARIES(xdrtest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
  ADD_TEST(xdrtest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xdrtest)
ENDIF (HDF4_BUILD_XDR_LIB)


