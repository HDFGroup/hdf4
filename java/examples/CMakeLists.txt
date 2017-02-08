cmake_minimum_required (VERSION 3.2.2)
project (HDF4_JAVA_Examples Java)

set (CMAKE_VERBOSE_MAKEFILE 1)

INCLUDE_DIRECTORIES (
    ${HDF4_JAVA_JNI_BINARY_DIR}
    ${HDF4_JAVA_HDF_LIB_DIR}
)

set (HDFJNI_EXAMPLES
    HDF4FileCreate
    HDF4GroupCreate
    HDF4DatasetCreate
)

set (CMAKE_JAVA_INCLUDE_PATH "${HDF4_JAVA_JARS};${HDF4_JAVA_LOGGING_JAR};${HDF4_JAVA_LOGGING_SIMPLE_JAR}")

if (WIN32 AND NOT CYGWIN)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
else ()
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
endif ()

set (CMAKE_JAVA_CLASSPATH ".")
foreach (CMAKE_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH})
  set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_INCLUDE_PATH}")
endforeach ()

foreach (HCP_JAR ${CMAKE_JAVA_INCLUDE_PATH})
  get_filename_component (_HCP_FILE ${HCP_JAR} NAME)
  set (HDFJNI_CLASSJARS "${_HCP_FILE} ${HDFJNI_CLASSJARS}")
endforeach ()

foreach (example ${HDFJNI_EXAMPLES})
  file (WRITE ${HDF4_JAVA_Examples_BINARY_DIR}/Manifest.txt
  "Main-Class: examples.${example}
Class-Path: ${HDFJNI_CLASSJARS}
"
  )
  add_jar (${example} MANIFEST ${HDF4_JAVA_Examples_BINARY_DIR}/Manifest.txt ${example}.java)
  get_target_property (${example}_JAR_FILE ${example} JAR_FILE)
  add_dependencies (${example} ${HDF4_JAVA_HDF_LIB_TARGET})
  set_target_properties (${example} PROPERTIES FOLDER examples/java)
endforeach ()

if (BUILD_TESTING)
  macro (ADD_H4_TEST resultfile resultcode)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
      if (WIN32)
        set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=hdf_java_D;")
      else()
        set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=hdf_java_debug;")
      endif()
    endif()
    add_test (
        NAME JAVAEX-${resultfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_TESTER=${CMAKE_Java_RUNTIME};${CMAKE_Java_RUNTIME_FLAGS}"
            -D "TEST_PROGRAM=examples.${resultfile}"
            -D "TEST_ARGS:STRING=${CMD_ARGS}${ARGN}"
            -D "TEST_CLASSPATH:STRING=${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${${resultfile}_JAR_FILE}"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_OUTPUT_DIRECTORY}"
            -D "TEST_FOLDER=${HDF4_JAVA_Examples_BINARY_DIR}"
            -D "TEST_OUTPUT=${HDF4_JAVA_Examples_BINARY_DIR}/${resultfile}.out"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_SKIP_COMPARE=TRUE"
            -P "${HDF_RESOURCES_DIR}/jrunTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (JAVAEX-${resultfile} PROPERTIES DEPENDS ${last_test})
    endif ()
    set (last_test "JAVAEX-${resultfile}")
  endmacro ()

  foreach (example ${HDFJNI_EXAMPLES})
    add_test (
        NAME JAVAEX-${example}-clearall-objects
        COMMAND    ${CMAKE_COMMAND}
            -E remove
            ${HDF4_JAVA_Examples_BINARY_DIR}/${example}.hdf
            ${example}.out
            ${example}.out.err
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (JAVAEX-${example}-clearall-objects PROPERTIES DEPENDS ${last_test})
    endif ()
    set (last_test "JAVAEX-${example}-clearall-objects")
    ADD_H4_TEST (${example} 0)
  endforeach ()

endif ()
