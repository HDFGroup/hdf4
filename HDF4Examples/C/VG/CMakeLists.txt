cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_VG C)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
include (C_sourcefiles.cmake)

foreach (example_name ${h4examples})
  add_executable (hdf_h4ex_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.c)
  target_link_libraries (hdf_h4ex_${example_name} ${H4EX_HDF4_LINK_LIBS})
endforeach ()

foreach (example_name ${h4examples_use})
  add_executable (hdf_h4ex_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.c)
  target_link_libraries (hdf_h4ex_${example_name} ${H4EX_HDF4_LINK_LIBS})
endforeach ()

foreach (example_name ${h4examples_mf})
  add_executable (mf_h4ex_${example_name} ${PROJECT_SOURCE_DIR}/h4ex_${example_name}.c)
  target_link_libraries (mf_h4ex_${example_name} ${H4EX_HDF4_LINK_LIBS})
endforeach ()

if (H4EX_BUILD_TESTING)
  macro (ADD_H4_TEST testname testddl dumparg testprefix)
    add_test (
        NAME ${testprefix}_${testname}-clearall
        COMMAND    ${CMAKE_COMMAND}
            -E remove
            ${testname}.hdf
    )
    add_test (
        NAME ${testprefix}_${testname}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:${testprefix}_${testname}>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_EXPECT=0"
            -D "TEST_OUTPUT=${testprefix}_${testname}.out"
            -D "TEST_REFERENCE=${testprefix}_${testname}.tst"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -P "${H4EX_RESOURCES_DIR}/runTest.cmake"
    )
    set_tests_properties (${testprefix}_${testname} PROPERTIES DEPENDS ${testprefix}_${testname}-clearall)
    if (HDF4_BUILD_TOOLS AND ${testddl} STREQUAL "1")
      add_test (
          NAME ${testprefix}_HDP-${testname}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=${H4EX_HDF4_DUMP_EXECUTABLE}"
              -D "TEST_ARGS:STRING=${dumparg};${testname}.hdf"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_OUTPUT=${testprefix}_${testname}.ddl.out"
              -D "TEST_EXPECT=0"
              -D "TEST_REFERENCE=${testprefix}_${testname}.ddl"
              -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
              -P "${H4EX_RESOURCES_DIR}/runTest.cmake"
      )
      set_tests_properties (${testprefix}_HDP-${testname} PROPERTIES DEPENDS ${testprefix}_${testname})
    endif ()
  endmacro ()

  foreach (example_name ${h4examples})
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/h4ex_${example_name}")
    set (testdest "${PROJECT_BINARY_DIR}/h4ex_${example_name}")
    add_custom_command (
        OUTPUT     "${testdest}.tst"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}.tst" "${testdest}.tst"
        DEPENDS    "${testsrc}.tst"
    )
    list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}.tst")
    if (HDF4_BUILD_TOOLS)
      add_custom_command (
          OUTPUT     "${testdest}.ddl"
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different "${testsrc}.ddl" "${testdest}.ddl"
          DEPENDS    "${testsrc}.ddl"
      )
      list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}.ddl")
    endif ()
  endforeach ()

  foreach (example_name ${h4examples_use})
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/h4ex_${example_name}")
    set (testdest "${PROJECT_BINARY_DIR}/h4ex_${example_name}")
    add_custom_command (
        OUTPUT     "${testdest}.tst"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}.tst" "${testdest}.tst"
        DEPENDS    "${testsrc}.tst"
    )
    list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}.tst")
  endforeach ()

  foreach (example_name ${h4examples_mf})
    set (testsrc "${PROJECT_SOURCE_DIR}/testfiles/h4ex_${example_name}")
    set (testdest "${PROJECT_BINARY_DIR}/h4ex_${example_name}")
    add_custom_command (
        OUTPUT     "${testdest}.tst"
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different "${testsrc}.tst" "${testdest}.tst"
        DEPENDS    "${testsrc}.tst"
    )
    list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}.tst")
    if (HDF4_BUILD_TOOLS)
      add_custom_command (
          OUTPUT     "${testdest}.ddl"
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different "${testsrc}.ddl" "${testdest}.ddl"
          DEPENDS    "${testsrc}.ddl"
      )
      list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}.ddl")
    endif ()
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
    list (APPEND HDF4Examples_VG_TEST_FILES "${testdest}")
  endforeach ()

  add_custom_target (HDF4Examples_HDF_VG-files ALL COMMENT "Copying files needed by VG tests"
                    DEPENDS ${HDF4Examples_VG_TEST_FILES})

  foreach (example_name ${h4examples})
    ADD_H4_TEST (h4ex_${example_name} 1 dumpvg hdf)
  endforeach ()

  foreach (example_name ${h4examples_use})
    ADD_H4_TEST (h4ex_${example_name} 0 dumpvg hdf)
  endforeach ()

  foreach (example_name ${h4examples_mf})
    ADD_H4_TEST (h4ex_${example_name} 1 dumpvg mf)
  endforeach ()
endif ()
