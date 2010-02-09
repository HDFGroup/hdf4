PROJECT (HDF4_MFHDF_NCTEST)

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

SET (nctest_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/add.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/atttests.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/cdftests.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/dimtests.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/driver.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/emalloc.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/error.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/misctest.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/rec.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/slabs.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/val.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/varget_unlim.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/varget.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/vargetg.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/varput.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/varputg.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/vardef.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/vartests.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/vputget.c
  ${HDF4_SOURCE_DIR}/mfhdf/nctest/vputgetg.c
  )
  
ADD_EXECUTABLE(nctest ${nctest_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(nctest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} ${HDF4_MF_XDR_LIB_NAME}-static )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(nctest ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ENDIF (HDF4_BUILD_XDR_LIB)

#-- Adding test for nctest

 ADD_TEST(nctest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nctest)
 
#-- Copy all the dat files from the test directory into the source directory
 SET (HDF4_NC_TEST_FILES
   test_unlim.cdl
   test_unlim.nc
 )
 
 FOREACH ( h4_file ${HDF4_NC_TEST_FILES} )
    SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #MESSAGE(STATUS " Copying ${HDF4_SOURCE_DIR}/mfhdf/nctest/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
      TARGET     nctest 
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_SOURCE_DIR}/mfhdf/nctest/${h4_file} ${dest}
      )
      
 ENDFOREACH ( h4_file ${HDF4_NC_TEST_FILES} )


