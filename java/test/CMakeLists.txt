cmake_minimum_required (VERSION 3.2.2)
PROJECT (HDF4_JAVA_TEST Java)

set (CMAKE_VERBOSE_MAKEFILE 1)

INCLUDE_DIRECTORIES (
    ${HDF4_JAVA_JNI_BINARY_DIR}
    ${HDF4_JAVA_HDF_LIB_DIR}
)

set (HDF4_JAVA_TEST_SRCS
    TestH4.java
    TestH4ANparams.java
    TestH4DFparams.java
    TestH4DFPparams.java
    TestH4DFRparams.java
    TestH4GRparams.java
    TestH4HCparams.java
    TestH4SDparams.java
    TestH4Vparams.java
    TestH4VSparams.java
    TestAll.java
)

file (WRITE ${PROJECT_BINARY_DIR}/Manifest.txt
"Main-Class: test.TestAll
"
)

set (CMAKE_JAVA_INCLUDE_PATH "${HDF4_JAVA_LIB_DIR}/junit.jar;${HDF4_JAVA_LIB_DIR}/hamcrest-core.jar;${HDF4_JAVA_JARS};${HDF4_JAVA_LOGGING_JAR};${HDF4_JAVA_LOGGING_SIMPLE_JAR}")

add_jar (${HDF4_JAVA_TEST_LIB_TARGET} MANIFEST ${PROJECT_BINARY_DIR}/Manifest.txt ${HDF4_JAVA_TEST_SRCS})

get_target_property (${HDF4_JAVA_TEST_LIB_TARGET}_JAR_FILE ${HDF4_JAVA_TEST_LIB_TARGET} JAR_FILE)
#install_jar (${HDF4_JAVA_TEST_LIB_TARGET} ${HJAVA_INSTALL_DATA_DIR}/tests tests)
#get_target_property (${HDF4_JAVA_TEST_LIB_TARGET}_CLASSPATH ${HDF4_JAVA_TEST_LIB_TARGET} CLASSDIR)

add_dependencies (${HDF4_JAVA_TEST_LIB_TARGET} ${HDF4_JAVA_HDF_LIB_TARGET})
set_target_properties (${HDF4_JAVA_TEST_LIB_TARGET} PROPERTIES FOLDER test/java)

set (HDF_JAVA_TEST_FILES
      JUnit-interface.txt
      JUnit-interface.ert
)

foreach (h4_file ${HDF_JAVA_TEST_FILES})
  set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
  #message (STATUS " Copying ${h4_file}")
  add_custom_command (
      TARGET     ${HDF4_JAVA_TEST_LIB_TARGET}
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${PROJECT_SOURCE_DIR}/${h4_file} ${dest}
  )
endforeach ()

if (WIN32)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
else ()
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
endif ()

set (CMAKE_JAVA_CLASSPATH ".")
foreach (CMAKE_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH})
  set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_INCLUDE_PATH}")
endforeach ()
set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${${HDF4_JAVA_TEST_LIB_TARGET}_JAR_FILE}")
set (testfilter "OK (598 tests)")

if (CMAKE_BUILD_TYPE MATCHES Debug)
  if (WIN32)
    set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=hdf_java_D;")
  else()
    set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=hdf_java_debug;")
  endif()
endif()

add_test (
  NAME JUnit-interface-clearall-objects
  COMMAND    ${CMAKE_COMMAND}
      -E remove
      JUnit-interface.out
      JUnit-interface.out.err
)

add_test (
    NAME JUnit-interface
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_TESTER=${CMAKE_Java_RUNTIME};${CMAKE_Java_RUNTIME_FLAGS}"
        -D "TEST_CLASSPATH:STRING=${CMAKE_JAVA_CLASSPATH}"
        -D "TEST_ARGS:STRING=${CMD_ARGS}-ea;org.junit.runner.JUnitCore"
        -D "TEST_PROGRAM=test.TestAll"
        -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_OUTPUT_DIRECTORY}"
        -D "TEST_FOLDER=${HDF4_BINARY_DIR}/java/test"
        -D "TEST_OUTPUT=JUnit-interface.out"
#        -D "TEST_LOG_LEVEL=trace"
        -D "TEST_EXPECT=0"
        -D "SKIP_APPEND=1"
        -D "TEST_MASK_ERROR=TRUE"
        -D "TEST_FILTER:STRING=${testfilter}"
        -D "TEST_REFERENCE=JUnit-interface.txt"
        -P "${HDF_RESOURCES_DIR}/jrunTest.cmake"
)
set_tests_properties (JUnit-interface PROPERTIES DEPENDS "JUnit-interface-clearall-objects")
