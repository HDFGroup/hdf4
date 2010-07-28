cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_HREPACK)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES (${HDF4_MFHDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_SOURCE_DIR}/mfhdf/hdiff)

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

SET (hrepack_SRCS
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_an.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_gr.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_list.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_lsttable.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_main.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_opttable.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_parse.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_sds.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_utils.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_vg.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_vs.c
    ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_dim.c
)
  
ADD_EXECUTABLE (hrepack ${hrepack_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hrepack)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hrepack PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
#-- Adding test for hrepack_check

  SET (hrepack_check_SRCS
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_check.c
  )
  
  ADD_EXECUTABLE (hrepack_check ${hrepack_check_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hrepack_check ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hrepack_check ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (hrepack_check)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (hrepack_check PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)

#  ADD_TEST (NAME hrepack_check COMMAND $<TARGET_FILE:hrepack_check>)

#-- Adding test for test_hrepack

  SET (test_hrepack_SRCS
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_an.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_gr.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_list.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_lsttable.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_opttable.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_parse.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_sds.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_utils.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_vg.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_dim.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepack_vs.c
      ${HDF4_SOURCE_DIR}/mfhdf/hrepack/hrepacktst.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_array.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_gattr.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_gr.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_list.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_mattbl.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_misc.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_sds.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_table.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_vs.c
      ${HDF4_SOURCE_DIR}/mfhdf/hdiff/hdiff_dim.c
  )
  
  ADD_EXECUTABLE (test_hrepack ${test_hrepack_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (test_hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (test_hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (test_hrepack)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (test_hrepack PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)
  
  ADD_TEST (NAME test_hrepack COMMAND $<TARGET_FILE:test_hrepack>)
 
#-- Copy all the dat files from the test directory into the source directory
  SET (HDF4_REPACK_TEST_FILES
      image24pixel.txt
      image24plane.txt
      image8.txt
      info.txt
  )
 
  FOREACH (h4_file ${HDF4_REPACK_TEST_FILES})
    SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #MESSAGE (STATUS " Copying ${HDF4_SOURCE_DIR}/mfhdf/hrepack/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     test_hrepack 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_SOURCE_DIR}/mfhdf/hrepack/${h4_file} ${dest}
    )
      
  ENDFOREACH (h4_file ${HDF4_REPACK_TEST_FILES})

ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        hrepack
    RUNTIME DESTINATION
        bin/tools
    COMPONENT
        toolsapplications
)
