
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  # --------------------------------------------------------------------
  # This executable is used to generate test files for the hdiff tests.
  # It should only be run during development when new test files are needed
  # --------------------------------------------------------------------
  IF (HDF4_BUILD_GENERATORS AND NOT BUILD_SHARED_LIBS)
    SET (hdifftst_SRCS
        ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdifftst.c
    )
  
    ADD_EXECUTABLE (hdifftst ${hdifftst_SRCS})
    TARGET_C_PROPERTIES (hdifftst " " " ")
    IF (HDF4_BUILD_XDR_LIB)
      TARGET_LINK_LIBRARIES (hdifftst ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
    ELSE (HDF4_BUILD_XDR_LIB)
      TARGET_LINK_LIBRARIES (hdifftst ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
    ENDIF (HDF4_BUILD_XDR_LIB)
    TARGET_NAMING (hdifftst ${LIB_TYPE})

    # Remove any output file left over from previous test run
    ADD_TEST (
        NAME HDIFF-GEN-clearall-objects
        COMMAND    ${CMAKE_COMMAND}
            -E remove 
            hdifftst1.hdf
            hdifftst2.hdf
            hdifftst3.hdf
            hdifftst4.hdf
            hdifftst5.hdf
            hdifftst6.hdf
            hdifftst7.hdf
    )
    SET (last_test "HDIFF-GEN-clearall-objects")
    
    ADD_TEST (NAME hdifftst COMMAND $<TARGET_FILE:hdifftst>)
    SET_TESTS_PROPERTIES (hdifftst PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    SET (last_test "hdifftst")
  ENDIF (HDF4_BUILD_GENERATORS AND NOT BUILD_SHARED_LIBS)

  MACRO (ADD_H4_TEST resultfile resultcode)
    IF (HDF4_ENABLE_USING_MEMCHECKER)
      ADD_TEST (NAME HDIFF-${resultfile} COMMAND $<TARGET_FILE:hdiff> ${ARGN})
      IF (NOT ${resultcode} STREQUAL "0")
        SET_TESTS_PROPERTIES (HDIFF-${resultfile} PROPERTIES LABELS ${PROJECT_NAME} WILL_FAIL "true")
      ELSE (NOT ${resultcode} STREQUAL "0")
        SET_TESTS_PROPERTIES (HDIFF-${resultfile} PROPERTIES LABELS ${PROJECT_NAME})
      ENDIF (NOT ${resultcode} STREQUAL "0")
    ELSE (HDF4_ENABLE_USING_MEMCHECKER)
      ADD_TEST (
          NAME HDIFF-${resultfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdiff>"
              -D "TEST_ARGS:STRING=${ARGN}"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}/testfiles"
              -D "TEST_OUTPUT=${resultfile}.out"
              -D "TEST_EXPECT=${resultcode}"
              -D "TEST_REFERENCE=${resultfile}.txt"
              -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
      )
    ENDIF (HDF4_ENABLE_USING_MEMCHECKER)
    IF (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (HDIFF-${resultfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    ELSE (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (HDIFF-${resultfile} PROPERTIES LABELS ${PROJECT_NAME})
    ENDIF (NOT "${last_test}" STREQUAL "")
    SET (last_test "HDIFF-${resultfile}")
  ENDMACRO (ADD_H4_TEST file)
 
  #-- Copy all the data files from the test directory into the source directory
  SET (HDF4_REFERENCE_TEST_FILES
      hdifftst1.hdf
      hdifftst2.hdf
      hdifftst3.hdf
      hdifftst4.hdf
      hdifftst5.hdf
      hdifftst6.hdf
      hdifftst7.hdf
  )
  SET (HDF4_REFERENCE_FILES
      hdiff_01.txt
      hdiff_02.txt
      hdiff_03.txt
      hdiff_04.txt
      hdiff_05.txt
#      hdiff_06.txt
      hdiff_07.txt
      hdiff_08.txt
      hdiff_09.txt
      hdiff_10.txt
      hdiff_11.txt
      hdiff_12.txt
      hdiff_13.txt
      hdiff_14.txt
      hdiff_15.txt
  )
 
  FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
    SET (dest "${PROJECT_BINARY_DIR}/testfiles/${h4_file}")
    #MESSAGE(STATUS " Copying ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/testfiles/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

  FOREACH (out_file ${HDF4_REFERENCE_FILES})
    SET (outdest "${PROJECT_BINARY_DIR}/testfiles/${out_file}")
    #MESSAGE (STATUS " Translating ${out_file}")
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${out_file} ${outdest}
    )
  ENDFOREACH (out_file ${HDF4_REFERENCE_FILES})

  IF (WIN32 AND NOT CYGWIN)
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/hdiff_06w.txt ${PROJECT_BINARY_DIR}/testfiles/hdiff_06.txt
    )
  ELSE (WIN32 AND NOT CYGWIN)
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/hdiff_06.txt ${PROJECT_BINARY_DIR}/testfiles/hdiff_06.txt
    )
  ENDIF (WIN32 AND NOT CYGWIN)

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME HDIFF-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          hdiff_01.out
          hdiff_02.out
          hdiff_03.out
          hdiff_04.out
          hdiff_05.out
          hdiff_06.out
          hdiff_07.out
          hdiff_08.out
          hdiff_09.out
          hdiff_10.out
          hdiff_11.out
          hdiff_12.out
          hdiff_13.out
          hdiff_14.out
          hdiff_15.out
          hdiff_01.out.err
          hdiff_02.out.err
          hdiff_03.out.err
          hdiff_04.out.err
          hdiff_05.out.err
          hdiff_06.out.err
          hdiff_07.out.err
          hdiff_08.out.err
          hdiff_09.out.err
          hdiff_10.out.err
          hdiff_11.out.err
          hdiff_12.out.err
          hdiff_13.out.err
          hdiff_14.out.err
          hdiff_15.out.err
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDIFF-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDIFF-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDIFF-clearall-objects")

  # help message
  ADD_H4_TEST (hdiff_01 1) 

  # Compare global attributes only
  ADD_H4_TEST (hdiff_02 1 -g hdifftst1.hdf hdifftst2.hdf)

  # Compare SD local attributes only
  ADD_H4_TEST (hdiff_03 1 -s hdifftst1.hdf hdifftst2.hdf)

  # Compare SD data only
  ADD_H4_TEST (hdiff_04 1 -d hdifftst1.hdf hdifftst2.hdf)

  # Compare Vdata data only
  ADD_H4_TEST (hdiff_05 1 -D hdifftst1.hdf hdifftst2.hdf)

  # Print statistics
  ADD_H4_TEST (hdiff_06 1 -d -S hdifftst1.hdf hdifftst2.hdf)

  # Compare SD data on variable(s)
  ADD_H4_TEST (hdiff_07 1 -d -v dset1 hdifftst1.hdf hdifftst2.hdf)

  # Compare vdata on variable(s) 
  ADD_H4_TEST (hdiff_08 1 -D -u vdata1 hdifftst1.hdf hdifftst2.hdf)

  # Print difference up to count number
  ADD_H4_TEST (hdiff_09 1 -d -e 2 hdifftst1.hdf hdifftst2.hdf)

  # Print difference when it is greater than limit
  ADD_H4_TEST (hdiff_10 1 -d -t 2 hdifftst1.hdf hdifftst2.hdf)

  # no options
  ADD_H4_TEST (hdiff_11 1 hdifftst1.hdf hdifftst2.hdf)

  # percent (relative)
  ADD_H4_TEST (hdiff_12 1 -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf)

  # hyperslab reading 
  ADD_H4_TEST (hdiff_13 0 hdifftst3.hdf hdifftst4.hdf)

  # lone dim
  ADD_H4_TEST (hdiff_14 1 hdifftst5.hdf hdifftst6.hdf)

  # group loop
  ADD_H4_TEST (hdiff_15 0 -b hdifftst7.hdf hdifftst7.hdf)
