
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  set (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

  #-----------------------------------------------------------------------------
  # Add test fortran stub library
  #-----------------------------------------------------------------------------
  add_library (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} STATIC ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  set_target_properties (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  if (WIN32)
    add_definitions (-DDOS_FS)
  endif ()
  TARGET_C_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} STATIC " " " ")
  target_link_libraries (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} STATIC)

  #-- Adding test for fortest
  add_executable (fortest ${HDF4_HDF_TESTSOURCE_DIR}/fortest.c)
  TARGET_NAMING (fortest STATIC)
  TARGET_C_PROPERTIES (fortest STATIC " " " ")
  target_link_libraries (fortest ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET})
  if (WIN32)
    target_link_libraries (fortest "ws2_32.lib")
  endif ()
  set_target_properties (fortest PROPERTIES LINKER_LANGUAGE C)

  #-----------------------------------------------------------------------------
  #-- Adding test for fortestF
  set (FORTEST_FSRCS
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

  add_executable (fortestF ${FORTEST_FSRCS} )
  TARGET_NAMING (fortestF STATIC)
  TARGET_FORTRAN_PROPERTIES (fortestF STATIC " " " ")
  target_link_libraries (fortestF ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} )
  set_target_properties (fortestF PROPERTIES LINKER_LANGUAGE Fortran)

  #-- Copy all the dat files from the test directory into the source directory
  set (HDF4_REFERENCE_TEST_FILES
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
  foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/test_files/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} to ${PROJECT_BINARY_DIR}/test_files/")
    add_custom_command (
        TARGET     fortestF
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} ${dest}
    )
  endforeach ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  add_test (
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

  add_test (NAME fortest COMMAND $<TARGET_FILE:fortest>)

  add_test (NAME fortestF COMMAND $<TARGET_FILE:fortestF>)
  set (passRegex "All Fortran Interface Tests Passed")
  set_tests_properties (fortestF PROPERTIES DEPENDS fortest PASS_REGULAR_EXPRESSION "${passRegex}")

