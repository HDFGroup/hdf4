cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_HDF_GR C)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
include (C_sourcefiles.cmake)

foreach (example ${h4examples})
  get_filename_component (example_name ${example} NAME_WE)
  add_executable (hdf_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  target_link_libraries (hdf_${example_name} ${H4EX_HDF4_LINK_LIBS})
endforeach ()

foreach (example ${h4examples_use})
  get_filename_component (example_name ${example} NAME_WE)
  add_executable (hdf_${example_name} ${PROJECT_SOURCE_DIR}/${example})
  target_link_libraries (hdf_${example_name} ${H4EX_HDF4_LINK_LIBS})
endforeach ()

if (H4EX_BUILD_TESTING)
  macro (ADD_H4_TEST testname testcreate)
    add_test (
        NAME hdf_${testname}-clearall
        COMMAND    ${CMAKE_COMMAND}
            -E remove
            ${testname}.hdf
    )
    add_test (
        NAME hdf_${testname}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdf_${testname}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_OUTPUT=${testname}.out"
            -D "TEST_REFERENCE=${testname}.tst"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${HDF4EX_RESOURCES_DIR}/runTest.cmake"
    )
    set_tests_properties (hdf_${testname} PROPERTIES DEPENDS hdf_${testname}-clearall)
    if (HDF4_BUILD_TOOLS AND ${testcreate} STREQUAL "1")
      add_test (
          NAME hdf_HDP-${testname}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=${H4EX_HDF4_DUMP_EXECUTABLE}"
              -D "TEST_ARGS:STRING=${ARGN};${testname}.hdf"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_OUTPUT=${testname}.ddl.out"
              -D "TEST_EXPECT=0"
              -D "TEST_REFERENCE=${testname}.ddl"
              -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
              -P "${HDF4EX_RESOURCES_DIR}/runTest.cmake"
      )
      set_tests_properties (hdf_HDP-${testname} PROPERTIES DEPENDS hdf_${testname})
    endif ()
  endmacro ()

  foreach (example ${h4examples})
    get_filename_component (example_name ${example} NAME_WE)
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/${example_name}")
    set (testdest "${PROJECT_BINARY_DIR}/${example_name}")
    add_custom_command (
        OUTPUT     "${testdest}.tst"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}.tst" "${testdest}.tst"
        DEPENDS    "${testsrc}.tst"
    )
    list (APPEND HDF4Examples_HDF_GR_TEST_FILES "${testdest}.tst")
    if (HDF4_BUILD_TOOLS)
      add_custom_command (
          OUTPUT     "${testdest}.ddl"
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different "${testsrc}.ddl" "${testdest}.ddl"
          DEPENDS    "${testsrc}.ddl"
      )
      list (APPEND HDF4Examples_HDF_GR_TEST_FILES "${testdest}.ddl")
    endif ()
  endforeach ()

  foreach (example ${h4examples_use})
    get_filename_component (example_name ${example} NAME_WE)
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/${example_name}")
    set (testdest "${PROJECT_BINARY_DIR}/${example_name}")
    add_custom_command (
        OUTPUT     "${testdest}.tst"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}.tst" "${testdest}.tst"
        DEPENDS    "${testsrc}.tst"
    )
    list (APPEND HDF4Examples_HDF_GR_TEST_FILES "${testdest}.tst")
  endforeach ()

  foreach (ex_file ${h4examples_files})
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/${ex_file}")
    set (testdest "${PROJECT_BINARY_DIR}/${ex_file}")
    add_custom_command (
        OUTPUT     "${testdest}"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}" "${testdest}"
        DEPENDS    "${testsrc}"
    )
    list (APPEND HDF4Examples_HDF_GR_TEST_FILES "${testdest}")
  endforeach ()

  add_custom_target (HDF4Examples_HDF_GR-files ALL COMMENT "Copying files needed by GR tests"
                    DEPENDS ${HDF4Examples_HDF_GR_TEST_FILES})

  foreach (example ${h4examples})
    get_filename_component (example_name ${example} NAME_WE)
    ADD_H4_TEST (${example_name} 1 dumpvg)
  endforeach ()

  foreach (example ${h4examples_use})
    get_filename_component (example_name ${example} NAME_WE)
    ADD_H4_TEST (${example_name} 0 dumpvg)
  endforeach ()
endif ()
