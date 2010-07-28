cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_HDFIMPORT)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES (${HDF4_MFHDF_SOURCE_DIR})

IF (WIN32)
  SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
ENDIF (WIN32)

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

SET (hdfimport_SRCS
    ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/hdfimport.c
)
  
ADD_EXECUTABLE (hdfimport ${hdfimport_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hdfimport)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfimport PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)

IF (BUILD_TESTING)
  #-- Adding test for hdfimporttest
  SET (hdfimporttest_SRCS
      ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/hdfimporttest.c
  )
  
  ADD_EXECUTABLE (hdfimporttest ${hdfimporttest_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (hdfimporttest)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (hdfimporttest PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)

  ADD_TEST (NAME hdfimporttest COMMAND $<TARGET_FILE:hdfimporttest>)
 
  #-- Copy all the dat files from the test directory into the source directory
  # SET (HDF4_REFERENCE_TEST_FILES
  #    sds_szipped.dat
  #    smallslice.0000.nc
  #    test1.nc
  #    testout.sav
  # )
  # 
  # FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
  #    SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
  #    #MESSAGE (STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
  #    ADD_CUSTOM_COMMAND (
  #        TARGET     hdftest 
  #        POST_BUILD
  #        COMMAND    ${CMAKE_COMMAND}
  #        ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
  #    )
  #      
  # ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

  #-- Adding test for gen_sds_floats

  SET (gen_sds_floats_SRCS
      ${HDF4_SOURCE_DIR}/mfhdf/hdfimport/gen_sds_floats.c
  )
  
  ADD_EXECUTABLE (gen_sds_floats ${gen_sds_floats_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (gen_sds_floats)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (gen_sds_floats PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)

  ADD_TEST (NAME gen_sds_floats COMMAND $<TARGET_FILE:gen_sds_floats>)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        hdfimport
    RUNTIME DESTINATION
        bin/tools
    COMPONENT
        toolsapplications
)
