
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################
  
FILE (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)

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
    grtdfui83.hdf
)

FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/test_files/${h4_file}")
   #MESSAGE(STATUS " Copying ${h4_file}")
   ADD_CUSTOM_COMMAND (
       TARGET     testhdf 
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_TEST_SOURCE_DIR}/test_files/${h4_file} ${dest}
   )
ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

# Remove any output file left over from previous test run
ADD_TEST (
    NAME testhdf-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        Block_info.hdf
        ChunkedGR.hdf
        ChunkedGR_DF.hdf
        ChunkedGR_NO.hdf
        ChunkedGR_RL.hdf
        ChunkedGR_SK.hdf
        datainfo_annot.hdf
        datainfo_dfri.hdf
        datainfo_images.hdf
        datainfo_linkblock.hdf
        datainfo_simple.hdf
        gr2.hdf
        gr_chunkcomp.hdf
        gr_comp.hdf
        gr_double_test.hdf
        gr_gzip.hdf
        gr_jpeg.hdf
        gr_r8.hdf
        nntcheck.hdf
        ntcheck.hdf
        RI_8.hdf
        RI_16.hdf
        RI_32.hdf
        RI_fl32.hdf
        RI_fl64.hdf
        RIchunkedziped.hdf
        s1w.hdf
        s3w.hdf
        s4w.hdf
        sdstrings.hdf
        swf32.hdf
        swf64.hdf
        swi8.hdf
        swi16.hdf
        swi32.hdf
        swin.hdf
        swui8.hdf
        swui16.hdf
        swui32.hdf
        swuin.hdf
        t.hdf
        t1.hdf
        t2.hdf
        t3.hdf
        t4.hdf
        tbitio.hdf
        tblocks.hdf
        tchunks.hdf
        tcomp.hdf
        tdf24.hdf
        tdfan.hdf
        temp.hdf
        thf.hdf
        tjpeg.hdf
        tlongnames.hdf
        tman.hdf
        tmgr.hdf
        tmgratt.hdf
        tmgrchk.hdf
        tnbit.hdf
        tref.hdf
        tuservds.hdf
        tuservgs.hdf
        tvattr.hdf
        tvpack.hdf
        tvsempty.hdf
        tvset.hdf
        tvsetext.hdf
        tx.hdf
        Tables_External_File
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
ELSE (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf-clearall-objects")

ADD_TEST (
    NAME testhdf_thf0-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        thf0.hdf
        thf1.hdf
        thf2.hdf
        thf3.hdf
        thf4.hdf
        thf5.hdf
        thf6.hdf
        thf7.hdf
        thf8.hdf
        thf9.hdf
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf_thf0-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
ELSE (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf_thf0-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf_thf0-clearall-objects")

SET (thf_decade
    1 2 3 4 5 6 7 8 9 10
    11 12 13 14 15 16 17 18 19 20
    21 22 23 24 25 26 27 28 29 30
    31 32 33 34 35 36 37 38 39 40
    41 42 43 44 45 46 47 48 49 50
    51 52 53 54 55 56 57 58 59 60
)
FOREACH (decade ${thf_decade}) 
  ADD_TEST (
      NAME testhdf_thf${decade}-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          thf${decade}0.hdf
          thf${decade}1.hdf
          thf${decade}2.hdf
          thf${decade}3.hdf
          thf${decade}4.hdf
          thf${decade}5.hdf
          thf${decade}6.hdf
          thf${decade}7.hdf
          thf${decade}8.hdf
          thf${decade}9.hdf
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (testhdf_thf${decade}-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (testhdf_thf${decade}-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "testhdf_thf${decade}-clearall-objects")
ENDFOREACH (decade ${thf_decade}) 

ADD_TEST (NAME testhdf COMMAND $<TARGET_FILE:testhdf>)
SET (passRegex "All tests were successful")
SET_PROPERTY (TEST testhdf PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
ELSE (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf PROPERTIES LABELS ${PROJECT_NAME})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf")

#-- Adding test for buffer
IF (NOT WIN32)
  ADD_TEST (NAME buffer COMMAND $<TARGET_FILE:buffer>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (buffer PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (buffer PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "buffer")
ENDIF (NOT WIN32)


