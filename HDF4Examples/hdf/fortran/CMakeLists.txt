cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_HDF_FORTRAN Fortran)
# --------------------------------------------------------------------
# Notes: When creating examples they should be prefixed
# with "f_ex_". This allows for easier filtering of the examples.
# --------------------------------------------------------------------

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
  set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-argument-mismatch")
endif ()

#-----------------------------------------------------------------------------
# Add debug information (intel Fortran : JB)
#-----------------------------------------------------------------------------
if (CMAKE_Fortran_COMPILER MATCHES ifort)
  if (WIN32)
    set (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE STRING "flags" FORCE)
    set (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "flags" FORCE)
  endif ()
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

# Remove any output file left over from previous test run
add_test (
    NAME HDF_FORTRAN_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove
        General_HDFobjects.hdf
        General_RImages.hdf
        General_Vdatas.hdf
        General_Vgroups.hdf
        Image_with_Palette.hdf
        Packed_Vdata.hdf
        Two_Vdatas.hdf
        Two_Vgroups.hdf
)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
else ()
  set_tests_properties (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS EXAMPLES)
endif ()
set (last_test "HDF_FORTRAN_EXAMPLES-clearall-objects")

foreach (example ${examples})
  get_filename_component(example_name ${example} NAME_WE)
  add_executable (${EXAMPLE_VARNAME}_f_ex_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  set_target_properties (${EXAMPLE_VARNAME}_f_ex_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (${EXAMPLE_VARNAME}_f_ex_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (H4EX_BUILD_TESTING)
    add_test (
        NAME ${EXAMPLE_VARNAME}_f_extest_${example_name}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:${EXAMPLE_VARNAME}_f_ex_${example_name}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_SKIP_COMPARE=TRUE"
            -D "TEST_OUTPUT=${testname}.out"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${${EXAMPLE_PACKAGE_NAME}_RESOURCES_DIR}/runTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${EXAMPLE_VARNAME}_f_extest_${example_name} PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
    else ()
      set_tests_properties (${EXAMPLE_VARNAME}_f_extest_${example_name} PROPERTIES LABELS EXAMPLES)
    endif ()
    set (last_test "${EXAMPLE_VARNAME}_f_extest_${example_name}")
  endif ()
endforeach ()

add_executable (f_ex_h4ex_VG_add_sds_to_vgroup ${PROJECT_SOURCE_DIR}/h4ex_VG_add_sds_to_vgroup.f)
set_target_properties (f_ex_h4ex_VG_add_sds_to_vgroup PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries (f_ex_h4ex_VG_add_sds_to_vgroup ${H4EX_HDF4_LINK_LIBS})

if (BUILD_TESTING)
  add_test (
      NAME f_extest_h4ex_VG_add_sds_to_vgroup
      COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:f_ex_h4ex_VG_add_sds_to_vgroup>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_SKIP_COMPARE=TRUE"
            -D "TEST_OUTPUT=${testname}.out"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${${EXAMPLE_PACKAGE_NAME}_RESOURCES_DIR}/runTest.cmake"
  )
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (f_extest_h4ex_VG_add_sds_to_vgroup PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  else ()
    set_tests_properties (f_extest_h4ex_VG_add_sds_to_vgroup PROPERTIES LABELS EXAMPLES)
  endif ()
  set (last_test "f_extest_h4ex_VG_add_sds_to_vgroup")
endif ()

foreach (example ${skip_examples})
  get_filename_component(example_name ${example} NAME_WE)
  add_executable (f_ex_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  set_target_properties (f_ex_${example_name} PROPERTIES LINKER_LANGUAGE Fortran)
  target_link_libraries (f_ex_${example_name} ${H4EX_HDF4_LINK_LIBS})
  if (BUILD_TESTING)
    add_test (
        NAME f_extest_${example_name}
        COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_extest_${example_name}"
    )
  endif ()
endforeach ()
