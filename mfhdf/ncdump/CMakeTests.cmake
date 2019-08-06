
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

if (NOT BUILD_SHARED_LIBS)
  set (tgt_ext "")
else ()
  set (tgt_ext "-shared")
endif ()

# Remove any output file left over from previous test run
add_test (
    NAME NCDUMP-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
        test0.nc
        test1.nc
        test1.cdl
        test2.cdl
)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (NCDUMP-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
else ()
  set_tests_properties (NCDUMP-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
endif ()
set (last_test "NCDUMP-clearall-objects")

HDFTEST_COPY_FILE("${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/test0.cdl" "${PROJECT_BINARY_DIR}/test0.cdl" "ncdump_files")
add_custom_target(ncdump_files ALL COMMENT "Copying files needed by ncdump tests" DEPENDS ${ncdump_files_list})

if (NCGEN_UTILITY)
  add_test (
      NAME NCDUMP-test0.nc
      COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:ncgen${tgt_ext}> -o test0.nc -n test0.cdl
  )
  set_tests_properties (NCDUMP-test0.nc PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  set (last_test "NCDUMP-test0.nc")

  add_test (
      NAME NCDUMP-test1.cdl
      COMMAND "${CMAKE_COMMAND}"
          -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
          -D "TEST_PROGRAM=$<TARGET_FILE:ncdump${tgt_ext}>"
          -D "TEST_ARGS:STRING=test0.nc"
          -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
          -D "TEST_OUTPUT=test1.cdl"
          -D "TEST_EXPECT=0"
          -D "TEST_SKIP_COMPARE=TRUE"
          -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
  )
  set_tests_properties (NCDUMP-test1.cdl PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  set (last_test "NCDUMP-test1.cdl")

  add_test (
      NAME NCDUMP-test1.nc
      COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:ncgen${tgt_ext}> -o test1.nc -n test1.cdl
  )
  set_tests_properties (NCDUMP-test1.nc PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  set (last_test "NCDUMP-test1.nc")

  add_test (
      NAME NCDUMP-test2.cdl
      COMMAND "${CMAKE_COMMAND}"
          -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
          -D "TEST_PROGRAM=$<TARGET_FILE:ncdump${tgt_ext}>"
          -D "TEST_ARGS:STRING=-n;test0;test1.nc"
          -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
          -D "TEST_OUTPUT=test2.cdl"
          -D "TEST_EXPECT=0"
          -D "TEST_REFERENCE=test1.cdl"
          -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
  )
  set_tests_properties (NCDUMP-test2.cdl PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  set (last_test "NCDUMP-test2.cdl")
endif ()
