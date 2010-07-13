cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_NCTEST)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES (${HDF4_MFHDF_SOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

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
  
ADD_EXECUTABLE (nctest ${nctest_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (nctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (nctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (nctest)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (nctest PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)

#-- Adding test for nctest

ADD_TEST (NAME nctest COMMAND $<TARGET_FILE:nctest>)
 
#-- Copy all the dat files from the test directory into the source directory
SET (HDF4_NC_TEST_FILES
    test_unlim.cdl
    test_unlim.nc
)
 
FOREACH (h4_file ${HDF4_NC_TEST_FILES})
  SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
  #MESSAGE (STATUS " Copying ${HDF4_SOURCE_DIR}/mfhdf/nctest/${h4_file} to ${PROJECT_BINARY_DIR}/")
  ADD_CUSTOM_COMMAND (
      TARGET     nctest 
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_SOURCE_DIR}/mfhdf/nctest/${h4_file} ${dest}
  )
      
ENDFOREACH (h4_file ${HDF4_NC_TEST_FILES})


