PROJECT (HDF4_MFHDF_NCGEN)

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

SET (ncgen_SRCS
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/close.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/escapes.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/generate.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/genlib.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/getfill.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/init.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/load.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/main.c
  ${HDF4_SOURCE_DIR}/mfhdf/ncgen/ncgentab.c
  )
  
ADD_EXECUTABLE(ncgen ${ncgen_SRCS})
TARGET_LINK_LIBRARIES(ncgen ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#-- Adding test for ncgen

# ADD_TEST(testncgen ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testncgen)
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
#      TARGET     ncgentest 
#      POST_BUILD
#      COMMAND    ${CMAKE_COMMAND}
#      ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
#      )
#      
# ENDFOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )


