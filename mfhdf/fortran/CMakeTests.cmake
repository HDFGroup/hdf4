
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  FILE (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)

  #-----------------------------------------------------------------------------
  # test programs
  #-----------------------------------------------------------------------------
  IF (HDF4_ENABLE_NETCDF)
    #-- Adding test for ftest
    ADD_EXECUTABLE (ftest ${HDF4_BINARY_DIR}/ftest.f ${HDF4_HDF_TESTSOURCE_DIR}/forsupff.f ${HDF4_BINARY_DIR}/netcdf.inc)
    TARGET_NAMING (ftest ${LIB_TYPE})
    TARGET_FORTRAN_PROPERTIES (ftest " " " ")
    SET_TARGET_PROPERTIES (ftest PROPERTIES LINKER_LANGUAGE Fortran)
    IF (HDF4_BUILD_XDR_LIB)
      TARGET_LINK_LIBRARIES (ftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
    ELSE (HDF4_BUILD_XDR_LIB)
      TARGET_LINK_LIBRARIES (ftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
    ENDIF (HDF4_BUILD_XDR_LIB)

    #-- Copy all the dat files from the test directory into the source directory
    SET (HDF4_REFERENCE_TEST_FILES
      test_nc.cdl
      test_nc.nc
    )
    FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
      SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
      #MESSAGE (STATUS " Copying ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
      ADD_CUSTOM_COMMAND (
          TARGET     ftest 
          POST_BUILD
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/${h4_file} ${dest}
      )
    ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

    ADD_TEST (NAME ftest COMMAND $<TARGET_FILE:ftest>)
  ENDIF (HDF4_ENABLE_NETCDF)

  #-- Adding test for f_hdftest
  ADD_EXECUTABLE (f_hdftest hdftest.f)
  TARGET_NAMING (f_hdftest ${LIB_TYPE})
  TARGET_FORTRAN_PROPERTIES (f_hdftest " " " ")
  SET_TARGET_PROPERTIES (f_hdftest PROPERTIES LINKER_LANGUAGE Fortran)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_hdftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_hdftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)

  #-- Adding test for f_hdftest1
  ADD_EXECUTABLE (f_hdftest1 hdftest1.f)
  TARGET_NAMING (f_hdftest1 ${LIB_TYPE})
  TARGET_FORTRAN_PROPERTIES (f_hdftest1 " " " ")
  SET_TARGET_PROPERTIES (f_hdftest1 PROPERTIES LINKER_LANGUAGE Fortran)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_hdftest1 ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_hdftest1 ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)

  #-- Adding test for f_tszip
  ADD_EXECUTABLE (f_tszip tszip.f)
  TARGET_NAMING (f_tszip ${LIB_TYPE})
  TARGET_FORTRAN_PROPERTIES (f_tszip " " " ")
  SET_TARGET_PROPERTIES (f_tszip PROPERTIES LINKER_LANGUAGE Fortran)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_tszip ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (f_tszip ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME MFHDF_FORTRAN-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          cchunk_gz.hdf
          cchunk_nb.hdf
          cchunk_no.hdf
          cchunk_rl.hdf
          cchunk_sk.hdf
          chunk_gz.hdf
          chunk_nb.hdf
          chunk_no.hdf
          chunk_rl.hdf
          chunk_sk.hdf
          comp_gz.hdf
          comp_no.hdf
          comp_rl.hdf
          comp_sk.hdf
          comp_szip.hdf
          copy.nc
          test.nc
          test1.hdf
          test2.hdf
          test_empty.hdf
          test_var.hdf
          test_file.hdf
          test_file01.hdf
          test_file02.hdf
          test_file03.hdf
          test_file04.hdf
          test_file05.hdf
          test_file06.hdf
          test_file07.hdf
          test_file08.hdf
          test_file09.hdf
          test_file10.hdf
          test_file11.hdf
          test_file12.hdf
          test_file13.hdf
          test_file14.hdf
          test_file15.hdf
          test_file16.hdf
          test_file17.hdf
          test_file18.hdf
          test_file19.hdf
          test_file20.hdf
          test_file21.hdf
          test_file22.hdf
          test_file23.hdf
          test_file24.hdf
          test_file25.hdf
          test_file26.hdf
          test_file27.hdf
          test_file28.hdf
          test_file29.hdf
          test_file30.hdf
          test_file31.hdf
          test_file32.hdf
          test_file33.hdf
          test_file34.hdf
          test_file35.hdf
  )
  IF (HDF4_ENABLE_NETCDF)
    SET_TESTS_PROPERTIES (MFHDF_FORTRAN-clearall-objects PROPERTIES DEPENDS ftest LABELS ${PROJECT_NAME})
  ENDIF (HDF4_ENABLE_NETCDF)

  ADD_TEST (NAME f_hdftest COMMAND $<TARGET_FILE:f_hdftest>)
  SET (passRegex "Total errors : [ ]+0")
  SET_PROPERTY (TEST f_hdftest PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
  SET_TESTS_PROPERTIES (f_hdftest PROPERTIES DEPENDS MFHDF_FORTRAN-clearall-objects LABELS ${PROJECT_NAME})

  ADD_TEST (NAME f_hdftest1 COMMAND $<TARGET_FILE:f_hdftest1>)
  SET (failRegex "failed" "errors")
  SET_PROPERTY (TEST f_hdftest1 PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")
  SET_TESTS_PROPERTIES (f_hdftest1 PROPERTIES DEPENDS f_hdftest LABELS ${PROJECT_NAME})

  ADD_TEST (NAME f_tszip COMMAND $<TARGET_FILE:f_tszip>)
  SET (failRegex "failed" "errors")
  SET_PROPERTY (TEST f_tszip PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")
  SET_TESTS_PROPERTIES (f_tszip PROPERTIES DEPENDS f_hdftest1 LABELS ${PROJECT_NAME})
