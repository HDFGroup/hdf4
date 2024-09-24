cmake_minimum_required (VERSION 3.18)
project (HDF4Examples_JAVA_J Java)

set (CMAKE_VERBOSE_MAKEFILE 1)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------
set (H4EX_JAVA_EXAMPLES
    HDF4FileCreate.java
    HDF4GroupCreate.java
    HDF4DatasetCreate.java
    VDWriteToVdata.java
    VDReadFromVdata.java
)

if (WIN32)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
else ()
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
endif ()

set (CMAKE_JAVA_INCLUDE_PATH ".")
foreach (CMAKE_JINCLUDE_PATH ${HDF4_JAVA_INCLUDE_DIRS})
  set (CMAKE_JAVA_INCLUDE_PATH "${CMAKE_JAVA_INCLUDE_PATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_JINCLUDE_PATH}")
endforeach ()
set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=${H4EX_JAVA_LIBRARY}$<$<CONFIG:Debug>:${CMAKE_DEBUG_POSTFIX}>;")

set (CMAKE_JAVA_CLASSPATH ".")
foreach (CMAKE_INCLUDE_PATH ${HDF4_JAVA_INCLUDE_DIRS})
  set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_INCLUDE_PATH}")
endforeach ()

foreach (HCP_JAR ${HDF4_JAVA_INCLUDE_DIRS})
  get_filename_component (_HCP_FILE ${HCP_JAR} NAME)
  set (HDFJAVA_CLASSJARS "${_HCP_FILE} ${HDFJAVA_CLASSJARS}")
endforeach ()

foreach (example ${H4EX_JAVA_EXAMPLES})
  get_filename_component (example_name ${example} NAME_WE)
  file (WRITE ${PROJECT_BINARY_DIR}/${example_name}_Manifest.txt
  "Main-Class: ${example_name}
Class-Path: ${HDFJAVA_CLASSJARS}
"
  )
  add_jar (${EXAMPLE_VARNAME}J_${example_name}
      SOURCES ${example}
      MANIFEST ${PROJECT_BINARY_DIR}/${example_name}_Manifest.txt
  )
  get_target_property (${EXAMPLE_VARNAME}J_${example_name}_JAR_FILE ${EXAMPLE_VARNAME}J_${example_name} JAR_FILE)
  get_target_property (${EXAMPLE_VARNAME}J_${example_name}_CLASSPATH ${EXAMPLE_VARNAME}J_${example_name} CLASSDIR)
  add_dependencies (${EXAMPLE_VARNAME}J_${example_name} ${H4EX_JAVA_LIBRARIES})
endforeach ()

if (H4EX_BUILD_TESTING)
  macro (ADD_H4_TEST resultfile resultcode)
    add_test (
        NAME ${EXAMPLE_VARNAME}_jnative-h4-${resultfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_TESTER=${CMAKE_Java_RUNTIME};${CMAKE_Java_RUNTIME_FLAGS}"
            -D "TEST_PROGRAM=${resultfile}"
            -D "TEST_ARGS:STRING=${ARGN};${CMD_ARGS}"
            -D "TEST_CLASSPATH:STRING=${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${${EXAMPLE_VARNAME}J_${resultfile}_JAR_FILE}"
            -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_LIB_DIRECTORY}"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=${PROJECT_BINARY_DIR}/${resultfile}.out"
            -D "TEST_REFERENCE=${resultfile}.txt"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_SKIP_COMPARE=TRUE"
            -P "${H4EX_RESOURCES_DIR}/jrunTest.cmake"
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${EXAMPLE_VARNAME}_jnative-h4-${resultfile} PROPERTIES DEPENDS ${last_test})
    endif ()
    set (last_test "${EXAMPLE_VARNAME}_jnative-h4-${resultfile}")
  endmacro ()

  foreach (example ${H4EX_JAVA_EXAMPLES})
    get_filename_component (example_name ${example} NAME_WE)
    add_test (
        NAME ${EXAMPLE_VARNAME}_jnative-h4-${example_name}-clearall-objects
        COMMAND    ${CMAKE_COMMAND}
            -E remove
            ${PROJECT_BINARY_DIR}/${example_name}.hdf
            ${example_name}.out
            ${example_name}.out.err
    )
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (${EXAMPLE_VARNAME}_jnative-h4-${example_name}-clearall-objects PROPERTIES DEPENDS ${last_test})
    endif ()
    add_test (
        NAME ${EXAMPLE_VARNAME}_jnative-h4-${example_name}-copy-objects
        COMMAND    ${CMAKE_COMMAND}
            -E copy_if_different
            ${PROJECT_SOURCE_DIR}/testfiles/${example_name}.txt
            ${PROJECT_BINARY_DIR}/${example_name}.txt
    )
    set_tests_properties (${EXAMPLE_VARNAME}_jnative-h4-${example_name}-copy-objects PROPERTIES DEPENDS ${EXAMPLE_VARNAME}_jnative-h4-${example_name}-clearall-objects)
    set (last_test "${EXAMPLE_VARNAME}_jnative-h4-${example_name}-copy-objects")
    ADD_H4_TEST (${example_name} 0)
  endforeach ()

endif ()
