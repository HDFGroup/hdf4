cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_SD_F Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_sd_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
  set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-argument-mismatch")
endif ()

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
    "${CMAKE_Fortran_MODULE_DIRECTORY}${HDF_MOD_EXT};${PROJECT_BINARY_DIR};${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
include (Fortran_sourcefiles.cmake)

if (H4EX_BUILD_TESTING)
  # Remove any output file left over from previous test run
  add_test (
      NAME MF_SD_FORTRAN_EXAMPLES-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          SDS.hdf
          SDSchunked.hdf
          SDScompressed.hdf
          SDSUNLIMITED.hdf
          SLABS.hdf
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (MF_SD_FORTRAN_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (MF_SD_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "MF_SD_FORTRAN_EXAMPLES-clearall-objects")
endif ()

foreach (example_name ${examples})
  add_executable (${EXAMPLE_VARNAME}_f_mf_sd_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.f)
  set_target_properties (${EXAMPLE_VARNAME}_f_mf_sd_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (${EXAMPLE_VARNAME}_f_mf_sd_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (BUILD_TESTING)
    add_test (
        NAME ${EXAMPLE_VARNAME}_f_mf_sd_${example_name}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:${EXAMPLE_VARNAME}_f_mf_sd_${example_name}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_SKIP_COMPARE=TRUE"
            -D "TEST_OUTPUT=${testname}.out"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${H4EX_RESOURCES_DIR}/runTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${EXAMPLE_VARNAME}_f_mf_sd_${example_name} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (${EXAMPLE_VARNAME}_f_mf_sd_${example_name} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()
    set (last_test "${EXAMPLE_VARNAME}_f_mf_sd_${example_name}")
  endif ()
endforeach ()

foreach (example_name ${skip_examples})
  add_executable (${EXAMPLE_VARNAME}_f_mf_sd_skip_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.f)
  set_target_properties (${EXAMPLE_VARNAME}_f_mf_sd_skip_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (${EXAMPLE_VARNAME}_f_mf_sd_skip_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (BUILD_TESTING)
    add_test (
        NAME ${EXAMPLE_VARNAME}_f_mf_sd_skip_${example_name}
        COMMAND ${CMAKE_COMMAND} -E echo "SKIP ${EXAMPLE_VARNAME}_f_mf_sd_skip_${example_name}"
    )
  endif ()
endforeach ()
