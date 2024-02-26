cmake_minimum_required (VERSION 3.18)
project (HDF4Examples_SD C)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
include (C_sourcefiles.cmake)

if (H4EX_BUILD_TESTING)
  # Remove any output file left over from previous test run
  add_test (
      NAME MFHDF_EXAMPLES-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          SDS.hdf
          SDSchunked.hdf
          SDScompressed.hdf
          SDSUNLIMITED.hdf
          SLABS.hdf
          ExternalSDS
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (MFHDF_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (MFHDF_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "MFHDF_EXAMPLES-clearall-objects")
endif ()

foreach (example_name ${examples})
  add_executable (mf_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.c)
  target_link_libraries (mf_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (H4EX_BUILD_TESTING)
    add_test (
        NAME mftest_${example_name}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:mf_${example_name}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_SKIP_COMPARE=TRUE"
            -D "TEST_OUTPUT=${example_name}.out"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${H4EX_RESOURCES_DIR}/runTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (mftest_${example_name} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (mftest_${example_name} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "mftest_${example_name}")
  endif ()
endforeach ()
