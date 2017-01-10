
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST")
file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST/testdir")
file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST/testfiles")
if (BUILD_SHARED_LIBS)
  file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST-shared")
  file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST-shared/testdir")
  file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST-shared/testfiles")
endif ()

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
    grtdfui83.hdf
)

foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
  set (dest "${PROJECT_BINARY_DIR}/TEST/test_files/${h4_file}")
  add_custom_command (
      TARGET     testhdf
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_HDF_TEST_SOURCE_DIR}/test_files/${h4_file} ${dest}
  )
  if (BUILD_SHARED_LIBS)
    set (dest "${PROJECT_BINARY_DIR}/TEST-shared/test_files/${h4_file}")
    add_custom_command (
        TARGET     testhdf-shared
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_TEST_SOURCE_DIR}/test_files/${h4_file} ${dest}
    )
  endif ()
endforeach ()

# Remove any output file left over from previous test run
set (HDF4_TESTHDF_FILES
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
add_test (
    NAME HDF_TEST-testhdf-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove
        ${HDF4_TESTHDF_FILES}
    WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}/TEST
)
set_tests_properties (HDF_TEST-testhdf-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_TEST-testhdf-clearall-objects PROPERTIES DEPENDS ${last_test})
endif ()
set (last_test "HDF_TEST-testhdf-clearall-objects")

set (HDF4_TESTHDF_THF0_FILES
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
add_test (
    NAME HDF_TEST-testhdf_thf0-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove
        ${HDF4_TESTHDF_THF0_FILES}
    WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}/TEST
)
set_tests_properties (HDF_TEST-testhdf_thf0-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_TEST-testhdf_thf0-clearall-objects PROPERTIES DEPENDS ${last_test})
endif ()
set (last_test "HDF_TEST-testhdf_thf0-clearall-objects")

set (thf_decade
    1 2 3 4 5 6 7 8 9 10
    11 12 13 14 15 16 17 18 19 20
    21 22 23 24 25 26 27 28 29 30
    31 32 33 34 35 36 37 38 39 40
    41 42 43 44 45 46 47 48 49 50
    51 52 53 54 55 56 57 58 59 60
)
foreach (decade ${thf_decade})
  add_test (
      NAME HDF_TEST-testhdf_thf${decade}-clearall-objects
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
    WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}/TEST
  )
  set_tests_properties (HDF_TEST-testhdf_thf${decade}-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_TEST-testhdf_thf${decade}-clearall-objects PROPERTIES DEPENDS ${last_test})
  endif ()
  set (last_test "HDF_TEST-testhdf_thf${decade}-clearall-objects")
endforeach (decade ${thf_decade})

add_test (NAME HDF_TEST-testhdf COMMAND $<TARGET_FILE:testhdf>)
set (passRegex "All tests were successful")
set_tests_properties (HDF_TEST-testhdf PROPERTIES
    PASS_REGULAR_EXPRESSION "${passRegex}"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST
    LABELS ${PROJECT_NAME}
)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_TEST-testhdf PROPERTIES DEPENDS ${last_test})
endif ()
set (last_test "HDF_TEST-testhdf")

#-- Adding test for buffer
if (NOT WIN32)
  add_test (NAME HDF_TEST-buffer COMMAND $<TARGET_FILE:buffer>)
  set_tests_properties (HDF_TEST-buffer PROPERTIES
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST
      LABELS ${PROJECT_NAME}
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_TEST-buffer PROPERTIES DEPENDS ${last_test})
  endif ()
  set (last_test "HDF_TEST-buffer")
endif ()

if (BUILD_SHARED_LIBS)
  # Remove any output file left over from previous test run
  add_test (
      NAME HDF_TEST-testhdf-shared-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          ${HDF4_TESTHDF_FILES}
      WORKING_DIRECTORY
          ${PROJECT_BINARY_DIR}/TEST-shared
  )
  set_tests_properties (HDF_TEST-testhdf-shared-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_TEST-testhdf-shared-clearall-objects PROPERTIES DEPENDS ${last_test})
  endif ()
  set (last_test "HDF_TEST-testhdf-shared-clearall-objects")

  add_test (
      NAME HDF_TEST-testhdf_thf0-shared-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          ${HDF4_TESTHDF_THF0_FILES}
      WORKING_DIRECTORY
          ${PROJECT_BINARY_DIR}/TEST-shared
  )
  set_tests_properties (HDF_TEST-testhdf_thf0-shared-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_TEST-testhdf_thf0-shared-clearall-objects PROPERTIES DEPENDS ${last_test})
  endif ()
  set (last_test "HDF_TEST-testhdf_thf0-shared-clearall-objects")

  foreach (decade ${thf_decade})
    add_test (
        NAME HDF_TEST-testhdf_thf${decade}-shared-clearall-objects
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
      WORKING_DIRECTORY
          ${PROJECT_BINARY_DIR}/TEST-shared
    )
    set_tests_properties (HDF_TEST-testhdf_thf${decade}-shared-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HDF_TEST-testhdf_thf${decade}-shared-clearall-objects PROPERTIES DEPENDS ${last_test})
    endif ()
    set (last_test "HDF_TEST-testhdf_thf${decade}-shared-clearall-objects")
  endforeach ()

  add_test (NAME HDF_TEST-testhdf-shared COMMAND $<TARGET_FILE:testhdf-shared>)
  set (passRegex "All tests were successful")
  set_tests_properties (HDF_TEST-testhdf-shared PROPERTIES
      PASS_REGULAR_EXPRESSION "${passRegex}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST-shared
      LABELS ${PROJECT_NAME}
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_TEST-testhdf-shared PROPERTIES DEPENDS ${last_test})
  endif ()
  set (last_test "HDF_TEST-testhdf-shared")

  #-- Adding test for buffer
  if (NOT WIN32)
    add_test (NAME HDF_TEST-buffer-shared COMMAND $<TARGET_FILE:buffer-shared>)
    set_tests_properties (HDF_TEST-buffer-shared PROPERTIES
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST-shared
        LABELS ${PROJECT_NAME}
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HDF_TEST-buffer-shared PROPERTIES DEPENDS ${last_test})
    endif ()
    set (last_test "HDF_TEST-buffer-shared")
  endif ()
endif ()
