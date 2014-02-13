
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################
  
  SET (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

  #-----------------------------------------------------------------------------
  # Add test fortran stub library
  #-----------------------------------------------------------------------------
  ADD_LIBRARY (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  SET_TARGET_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DDOS_FS)
  ENDIF (WIN32 AND NOT CYGWIN)
  TARGET_C_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} " " " ")
  TARGET_LINK_LIBRARIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET}  ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} ${LIB_TYPE})
  
  #-- Adding test for fortest
  ADD_EXECUTABLE (fortest ${HDF4_HDF_TESTSOURCE_DIR}/fortest.c)
  TARGET_NAMING (fortest ${LIB_TYPE})
  TARGET_C_PROPERTIES (fortest " " " ")
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
  TARGET_FORTRAN_PROPERTIES (fortestF " " " ")
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
    