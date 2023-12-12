cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_MFHDF_FORTRAN Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
  set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-argument-mismatch")
endif ()

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}${HDF_MOD_EXT}
    ${PROJECT_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
include (Fortran_sourcefiles.cmake)

if (H4EX_BUILD_TESTING)
  # Remove any output file left over from previous test run
  add_test (
      NAME MFHDF_FORTRAN_EXAMPLES-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          SDS.hdf
          SDSchunked.hdf
          SDScompressed.hdf
          SDSUNLIMITED.hdf
          SLABS.hdf
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (MFHDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (MFHDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "MFHDF_FORTRAN_EXAMPLES-clearall-objects")
endif ()

foreach (example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  add_executable (f_exmf_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  set_target_properties (f_exmf_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (f_exmf_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (BUILD_TESTING)
    add_test (
        NAME f_exmftest_${example_name}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:f_exmf_${example_name}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_SKIP_COMPARE=TRUE"
            -D "TEST_OUTPUT=${testname}.out"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${HDF4EX_RESOURCES_DIR}/runTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (f_exmftest_${example_name} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (f_exmftest_${example_name} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "f_exmftest_${example_name}")
  endif ()
endforeach ()

foreach (example ${skip_examples})
  get_filename_component(example_name ${example} NAME_WE)
  add_executable (f_exmf_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  set_target_properties (f_exmf_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (f_exmf_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (BUILD_TESTING)
    add_test (
        NAME f_exmftest_${example_name}
        COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_exmftest_${example_name}"
    )
  endif ()
endforeach ()
