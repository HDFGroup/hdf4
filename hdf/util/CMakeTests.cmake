
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

##############################################################################
# --------------------------------------------------------------------
# Copy all the files from the test directory into the source directory
# --------------------------------------------------------------------
##############################################################################

if (HDF4_BUILD_TOOLS)
  #-- Copy all the hdfls data files from the source directory into the test directory
  set (HDF4_LS_TEST_FILES
      hdfcomp.out1.1
      hdfcomp.out1.2
      hdfed.input1
#      hdfed.out1
      hdfpack.out1.1
      hdfpack.out1.2
      hdftor8.out1
      jpeg2hdf.out1
      ristosds.input1
      ristosds.out1
  )

  foreach (h4_file ${HDF4_LS_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdfls
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/${h4_file} ${dest}
    )
  endforeach ()

  if (WIN32 AND MSVC_VERSION LESS 1900)
    ADD_CUSTOM_COMMAND (
        TARGET     hdfls
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfed-w.out1 ${PROJECT_BINARY_DIR}/hdfed.out1
    )
  else ()
    ADD_CUSTOM_COMMAND (
        TARGET     hdfls
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfed.out1 ${PROJECT_BINARY_DIR}/hdfed.out1
    )
  endif ()

  #-- Copy all the hdfed data files from the source directory into the test directory
  set (HDF4_HDFED_TEST_FILES
      storm110.hdf
      ntcheck.hdf
  )
  foreach (h4_file ${HDF4_HDFED_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdfed
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()
endif ()

  #-- Copy all the hdf2gif data files from the source directory into the test directory
  set (HDF4_HDF2GIF_TEST_FILES
      skull.hdf
  )
  foreach (h4_file ${HDF4_HDF2GIF_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdf2gif
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the gif2hdf data files from the source directory into the test directory
  set (HDF4_GIF2HDF_TEST_FILES
      SunWheel.gif
      bttrfly.gif
  )
  foreach (h4_file ${HDF4_GIF2HDF_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     gif2hdf
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the hdftor8 data files from the source directory into the test directory
  set (HDF4_HDFTOR8_TEST_FILES
      head.r24
      head.r8
  )
  foreach (h4_file ${HDF4_HDFTOR8_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdftor8
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the ristosds data files from the source directory into the test directory
  set (HDF4_RISTOSDS_TEST_FILES
      storm110.hdf
      storm120.hdf
      storm130.hdf
  )
  foreach (h4_file ${HDF4_RISTOSDS_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     ristosds
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the hdfpack data files from the source directory into the test directory
  set (HDF4_HDFPACK_TEST_FILES
      test.hdf
  )
  foreach (h4_file ${HDF4_HDFPACK_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdfpack
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the paltohdf data files from the source directory into the test directory
  set (HDF4_PALTOHDF_TEST_FILES
      palette.raw
  )
  foreach (h4_file ${HDF4_PALTOHDF_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     paltohdf
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the r8tohdf data files from the source directory into the test directory
  set (HDF4_R8TOHDF_TEST_FILES
      storm110.raw
      storm120.raw
      storm130.raw
      storm140.raw
  )
  foreach (h4_file ${HDF4_R8TOHDF_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     r8tohdf
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the hdfcomp data files from the source directory into the test directory
  set (HDF4_HDFCOMP_TEST_FILES
      storm110.hdf
      storm120.hdf
      storm130.hdf
  )
  foreach (h4_file ${HDF4_HDFCOMP_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdfcomp
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

  #-- Copy all the jpeg2hdf data files from the source directory into the test directory
  set (HDF4_JPEG2HDF_TEST_FILES
      jpeg_img.jpg
  )
  foreach (h4_file ${HDF4_JPEG2HDF_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     jpeg2hdf
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  endforeach ()

##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

if (HDF4_BUILD_TOOLS)
  MACRO (ADD_LS_TEST_NOL testfile resultfile resultcode)
    if (HDF4_ENABLE_USING_MEMCHECKER)
      add_test (NAME HDFLS_NOL-${testfile} COMMAND $<TARGET_FILE:hdfls> ${testfile})
    else ()
      add_test (
          NAME HDFLS_NOL-${testfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
              -D "TEST_ARGS:STRING=${testfile}"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_OUTPUT=${testfile}.tmp"
              -D "TEST_EXPECT=${resultcode}"
              -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
              -D "TEST_REFERENCE=${resultfile}"
              -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
      )
    endif ()
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HDFLS_NOL-${testfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (HDFLS_NOL-${testfile} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "HDFLS_NOL-${testfile}")
  ENDMACRO ()

  MACRO (ADD_LS_TEST testfile resultfile resultcode)
    if (HDF4_ENABLE_USING_MEMCHECKER)
      add_test (NAME HDFLS-${testfile} COMMAND $<TARGET_FILE:hdfls> -l ${testfile})
    else ()
      add_test (
          NAME HDFLS-${testfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
              -D "TEST_ARGS:STRING=-l;${testfile}"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_OUTPUT=${testfile}.tmp"
              -D "TEST_EXPECT=${resultcode}"
              -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
              -D "TEST_REFERENCE=${resultfile}"
              -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
      )
    endif ()
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HDFLS-${testfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (HDFLS-${testfile} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "HDFLS-${testfile}")
  ENDMACRO ()

  MACRO (ADD_H4_TEST_ED testfile resultfile resultcode)
    if (HDF4_ENABLE_USING_MEMCHECKER)
      add_test (NAME HEDIT-${testfile} COMMAND $<TARGET_FILE:hdfed> -batch)
    else ()
      add_test (
          NAME HEDIT-${testfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfed>"
              -D "TEST_ARGS:STRING=-batch"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_INPUT=${testfile}"
              -D "TEST_OUTPUT=${testfile}.tmp"
              -D "TEST_EXPECT=${resultcode}"
              -D "TEST_REFERENCE=${resultfile}"
              -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
      )
    endif ()
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HEDIT-${testfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (HEDIT-${testfile} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "HEDIT-${testfile}")
  ENDMACRO ()
endif ()

  MACRO (ADD_H4_TEST testname testfile)
    add_test (NAME ${testname} COMMAND $<TARGET_FILE:${testfile}> ${ARGN})
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${testname} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (${testname} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "${testname}")
  ENDMACRO ()

  MACRO (ADD_CMP_TEST testname reffile testfile)
    add_test (NAME ${testname} COMMAND ${CMAKE_COMMAND} -E compare_files ${reffile} ${testfile})
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${testname} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (${testname} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "${testname}")
  ENDMACRO ()

  MACRO (ADD_H4Q_TEST testname testfile)
    add_test (NAME ${testname} COMMAND $<TARGET_FILE:${testfile}> ${ARGN} > /dev/null 2>&1)
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${testname} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (${testname} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "${testname}")
  ENDMACRO ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  add_test (
      NAME hdfgif-clear-refs
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          skull.gif
          SunWheel.hdf
          bttrfly.hdf
          head8.hdf
          img001-263.328
          hdfed.input1.tmp
          hdfed.input1.tmp.err
          storm.hdf
          ristosds.input1.tmp
          ristosds.input1.tmp.err
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (hdfgif-clear-refs PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (hdfgif-clear-refs PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "hdfgif-clear-refs")

  ADD_H4_TEST (testhdf2gif hdf2gif skull.hdf skull.gif)
  ADD_H4_TEST (testgif2hdf-sunwheel gif2hdf SunWheel.gif SunWheel.hdf)
  ADD_H4_TEST (testgif2hdf-butterfly gif2hdf bttrfly.gif bttrfly.hdf)

  ADD_H4_TEST (testhdf24to8 hdf24to8 head.r24 head8.hdf)
  ADD_H4_TEST (testhdftor8 hdftor8 head8.hdf)
  ADD_CMP_TEST (hdfr8comp img001-263.328 head.r8)

  if (HDF4_BUILD_TOOLS)
    ADD_H4_TEST_ED (hdfed.input1 hdfed.out1 0)
  endif ()

  ADD_H4Q_TEST (testristosds ristosds storm110.hdf storm120.hdf storm130.hdf -o storm.hdf)
  if (HDF4_BUILD_TOOLS)
    ADD_H4_TEST_ED (ristosds.input1 ristosds.out1 0)
  endif ()

  # Remove any output file left over from previous test run
  add_test (
      NAME hdfpack-clear-refs
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          test.pck
          test.blk
          test.hdf.tmp
          test.hdf.tmp.err
          test.pck.tmp
          test.pck.tmp.err
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (hdfpack-clear-refs PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (hdfpack-clear-refs PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "hdfpack-clear-refs")

  ADD_H4_TEST (testhdfpack hdfpack test.hdf test.pck)
  ADD_H4_TEST (testhdfpack-block hdfpack -b test.hdf test.blk)
  if (HDF4_BUILD_TOOLS)
    ADD_LS_TEST_NOL (test.hdf hdfpack.out1.1 0)
    ADD_LS_TEST_NOL (test.pck hdfpack.out1.2 0)
  endif ()

  # Remove any output file left over from previous test run
  add_test (
      NAME hdfpalette-clear-refs
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          pal001
          pal005
          palette.hdf
          palette.raw.new
          storm.hdf
          storm.hdf.tmp
          storm.hdf.tmp.err
          img001-057.057
          img002-057.057
          img003-057.057
          img004-057.057
          img005-057.057
          allstorms.hdf
          allstorms.hdf.tmp
          allstorms.hdf.tmp.err
          allcomp.hdf
          allcomp.hdf.tmp
          allcomp.hdf.tmp.err
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (hdfpalette-clear-refs PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (hdfpalette-clear-refs PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "hdfpalette-clear-refs")

  ADD_H4_TEST (testpaltohdf paltohdf palette.raw palette.hdf)
  ADD_H4_TEST (testhdftopal hdftopal palette.hdf palette.raw.new)
  ADD_CMP_TEST (palettecomp palette.raw palette.raw.new)

  ADD_H4_TEST (testr8tohdf-storm r8tohdf 57 57 storm.hdf storm110.raw storm120.raw storm130.raw storm140.raw)
  ADD_H4_TEST (testr8tohdf-palette r8tohdf 57 57 storm.hdf -p palette.raw -i storm110.raw)
  ADD_H4_TEST (testhdftor8-storm hdftor8 storm.hdf)
  if (HDF4_BUILD_TOOLS)
    ADD_LS_TEST (storm.hdf hdftor8.out1 0)
  endif ()

  ADD_CMP_TEST (storm110comp img001-057.057  storm110.raw)
  ADD_CMP_TEST (storm120comp img002-057.057  storm120.raw)
  ADD_CMP_TEST (storm130comp img003-057.057  storm130.raw)
  ADD_CMP_TEST (storm140comp img004-057.057  storm140.raw)

  ADD_H4_TEST (testhdfcomp-storms hdfcomp allstorms.hdf storm110.hdf storm120.hdf storm130.hdf)
  ADD_H4_TEST (testhdfcomp hdfcomp allcomp.hdf -c storm110.hdf storm120.hdf storm130.hdf)
  if (HDF4_BUILD_TOOLS)
    ADD_LS_TEST (allstorms.hdf hdfcomp.out1.1 0)
    ADD_LS_TEST (allcomp.hdf hdfcomp.out1.2 0)
  endif ()

  # Remove any output file left over from previous test run
  add_test (
      NAME hdfjpeg-clear-refs
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          jpeg.hdf
          jpeg.hdf.tmp
          jpeg.hdf.tmp.err
          jpeg2.jpg
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (hdfjpeg-clear-refs PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (hdfjpeg-clear-refs PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "hdfjpeg-clear-refs")

  ADD_H4_TEST (testjpeg2hdf jpeg2hdf jpeg_img.jpg jpeg.hdf)
  ADD_H4_TEST (testhdf2jpeg hdf2jpeg jpeg.hdf jpeg2.jpg)
  if (HDF4_BUILD_TOOLS)
    ADD_LS_TEST (jpeg.hdf jpeg2hdf.out1 0)
  endif ()
  ADD_CMP_TEST (jpeg2comp jpeg_img.jpg jpeg2.jpg)

#  ADD_H4_TEST (hdf8to24 hdf8to24)

#  ADD_H4_TEST (hdfunpac hdfunpac)

#  ADD_H4_TEST (vmake vmake)

#  ADD_H4_TEST (vshow vshow)
