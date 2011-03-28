cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_FORTRAN C CXX Fortran)

#-----------------------------------------------------------------------------
# Make sure generated files and modules are picked up correctly
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES ( 
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
)
  
SET (HDF4_HDF_SRC_CSTUB_FSRCS
    ${HDF4_HDFSOURCE_DIR}/dfanf.c
    ${HDF4_HDFSOURCE_DIR}/dff.c
    ${HDF4_HDFSOURCE_DIR}/dfpf.c
    ${HDF4_HDFSOURCE_DIR}/dfr8f.c
    ${HDF4_HDFSOURCE_DIR}/dfsdf.c
    ${HDF4_HDFSOURCE_DIR}/dfufp2i.c
    ${HDF4_HDFSOURCE_DIR}/dfutilf.c
    ${HDF4_HDFSOURCE_DIR}/df24f.c
    ${HDF4_HDFSOURCE_DIR}/dfufp2if.c
    ${HDF4_HDFSOURCE_DIR}/herrf.c
    ${HDF4_HDFSOURCE_DIR}/hfilef.c
    ${HDF4_HDFSOURCE_DIR}/mfanf.c
    ${HDF4_HDFSOURCE_DIR}/mfgrf.c
    ${HDF4_HDFSOURCE_DIR}/vattrf.c
    ${HDF4_HDFSOURCE_DIR}/vgf.c
)

SET (HDF4_HDF_SRC_FHDRS
    ${HDF4_HDFSOURCE_DIR}/dffunc.inc
    ${HDF4_HDFSOURCE_DIR}/hdf.inc
)

SET_SOURCE_FILES_PROPERTIES (${HDF4_HDF_SRC_CSTUB_FSRCS} PROPERTIES LANGUAGE C) 

SET (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_SRC_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_SRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h ${HDF4_BINARY_DIR}/h4config.h)
SET_TARGET_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)
TARGET_LINK_LIBRARIES (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FCSTUB_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_NAME} ${LIB_TYPE})

SET (HDF4_F_FORTRAN_SRCS 
    ${HDF4_HDFSOURCE_DIR}/df24ff.f  
    ${HDF4_HDFSOURCE_DIR}/dfanff.f 
    ${HDF4_HDFSOURCE_DIR}/dfpff.f 
    ${HDF4_HDFSOURCE_DIR}/dfr8ff.f
    ${HDF4_HDFSOURCE_DIR}/dfsdff.f 
    ${HDF4_HDFSOURCE_DIR}/dfufp2iff.f
    ${HDF4_HDFSOURCE_DIR}/dfff.f 
    ${HDF4_HDFSOURCE_DIR}/hfileff.f 
    ${HDF4_HDFSOURCE_DIR}/mfgrff.f
    ${HDF4_HDFSOURCE_DIR}/vattrff.f 
    ${HDF4_HDFSOURCE_DIR}/vgff.f 
)
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_SRC_FORTRAN_LIB_TARGET} ${LIB_TYPE} ${HDF4_F_FORTRAN_SRCS})
SET (SHARED_LINK_FLAGS " ")
IF (WIN32)
  IF (${LIB_TYPE} MATCHES "SHARED")
    IF (MSVC)
      SET (SHARED_LINK_FLAGS "/DLL /DEF:${HDF4_HDF_FORTRAN_SOURCE_DIR}/hdf_fortrandll.def")
    ENDIF (MSVC)
  ENDIF (${LIB_TYPE} MATCHES "SHARED")
ENDIF (WIN32)
TARGET_FORTRAN_WIN_PROPERTIES (${HDF4_SRC_FORTRAN_LIB_TARGET} ${SHARED_LINK_FLAGS})
SET_TARGET_PROPERTIES (${HDF4_SRC_FORTRAN_LIB_TARGET} PROPERTIES LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FORTRAN_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_NAME} ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################
  
IF (BUILD_TESTING)

  SET (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

  #-----------------------------------------------------------------------------
  # Add test fortran stub library
  #-----------------------------------------------------------------------------
  ADD_LIBRARY (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  SET_TARGET_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DDOS_FS)
  ENDIF (WIN32 AND NOT CYGWIN)
  TARGET_LINK_LIBRARIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET}  ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} ${LIB_TYPE})
  
  #-- Adding test for fortest
  ADD_EXECUTABLE (fortest ${HDF4_HDF_TESTSOURCE_DIR}/fortest.c)
  TARGET_NAMING (fortest ${LIB_TYPE})
  TARGET_LINK_LIBRARIES (fortest ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET})
  IF (WIN32 AND MSVC)
    TARGET_LINK_LIBRARIES (fortest "ws2_32.lib")
  ENDIF (WIN32 AND MSVC)
  SET_TARGET_PROPERTIES (fortest PROPERTIES LINKER_LANGUAGE C)

  #-----------------------------------------------------------------------------
  #-- Adding test for fortestF
  SET (FORTEST_FSRCS 
      ${HDF4_HDF_TESTSOURCE_DIR}/fortestF.f
      ${HDF4_HDF_TESTSOURCE_DIR}/forsupff.f
      ${HDF4_HDF_TESTSOURCE_DIR}/manf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/mgrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/slabwf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/t24f.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tanf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tanfilef.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tpf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tr8f.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdmmsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdnmmsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdnntf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdntf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdstrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tstubsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tvattrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tvsetf.f
  )

  ADD_EXECUTABLE (fortestF ${FORTEST_FSRCS} )
  TARGET_NAMING (fortestF ${LIB_TYPE})
  TARGET_FORTRAN_WIN_PROPERTIES (fortestF "")
  TARGET_LINK_LIBRARIES (fortestF ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} )
  SET_TARGET_PROPERTIES (fortestF PROPERTIES LINKER_LANGUAGE Fortran)

  #-- Copy all the dat files from the test directory into the source directory
  SET (HDF4_REFERENCE_TEST_FILES
    8bit.dat
    bitio.dat
    gr_r24.dat
    greyjpeg.dat
    jpeg.dat
    litend.dat
    nbit.dat
    tmgr.dat
    tvattr.dat
  )
  FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
    SET (dest "${PROJECT_BINARY_DIR}/test_files/${h4_file}")
    #MESSAGE (STATUS " Copying ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} to ${PROJECT_BINARY_DIR}/test_files/")
    ADD_CUSTOM_COMMAND (
        TARGET     fortestF 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} ${dest}
    )
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME HDF_FORTRAN-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          fortest.arg
          Fortran_err.dat
          GRcompressed.hdf
          GRchunked1.hdf
          GRchunked2.hdf
          manf.hdf
          slab1wf.hdf
          slab4wf.hdf
          slabwf.hdf
          tdf24f.hdf
          tdfanF.hdf
          tdfanflF.hdf
          tmgrf.hdf
  )

  ADD_TEST (NAME fortest COMMAND $<TARGET_FILE:fortest>)

  ADD_TEST (NAME fortestF COMMAND $<TARGET_FILE:fortestF>)
  SET (passRegex "All Fortran Interface Tests Passed")
  SET_PROPERTY (TEST fortestF PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
  
ENDIF (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (HDF4_EXPORTED_TARGETS)
  INSTALL (
      TARGETS 
          ${HDF4_SRC_FCSTUB_LIB_TARGET}
          ${HDF4_SRC_FORTRAN_LIB_TARGET}
      EXPORT
          ${HDF4_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries 
      ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries
      RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT fortlibraries
  )
ENDIF (HDF4_EXPORTED_TARGETS)
    