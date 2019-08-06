cmake_minimum_required (VERSION 3.10)
project (HDF4_JAVA_TEST Java)

set (CMAKE_VERBOSE_MAKEFILE 1)

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${HDF4_JAVA_JNI_BINARY_DIR};${HDF4_JAVA_HDF_LIB_DIR}")

set (HDF4_JAVA_TEST_SOURCES
    TestH4
    TestH4ANparams
    TestH4DFparams
    TestH4DFPparams
    TestH4DFRparams
    TestH4GRparams
    TestH4HCparams
    TestH4SDparams
    TestH4Vparams
    TestH4VSparams
)

set (CMAKE_JAVA_INCLUDE_PATH "${HDF4_JAVA_LIB_DIR}/junit.jar;${HDF4_JAVA_LIB_DIR}/hamcrest-core.jar;${HDF4_JAVA_JARS};${HDF4_JAVA_LOGGING_JAR};${HDF4_JAVA_LOGGING_SIMPLE_JAR}")

foreach (test_file ${HDF4_JAVA_TEST_SOURCES})

  file (WRITE ${PROJECT_BINARY_DIR}/${test_file}Manifest.txt
  "Main-Class: test.${test_file}
"
  )

  add_jar (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} MANIFEST ${PROJECT_BINARY_DIR}/${test_file}Manifest.txt ${test_file}.java)

  get_target_property (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file}_JAR_FILE ${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} JAR_FILE)
  #install_jar (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} ${HJAVA_INSTALL_DATA_DIR}/tests tests)
  #get_target_property (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file}_CLASSPATH ${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} CLASSDIR)

  add_dependencies (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} ${HDF4_JAVA_HDF_LIB_TARGET})
  set_target_properties (${HDF4_JAVA_TEST_LIB_TARGET}_${test_file} PROPERTIES FOLDER test/java)
endforeach ()

foreach (h4_file ${HDF4_JAVA_TEST_SOURCES})
  HDFTEST_COPY_FILE("${PROJECT_SOURCE_DIR}/testfiles/JUnit-${h4_file}.txt" "${PROJECT_BINARY_DIR}/JUnit-${h4_file}.txt" "${HDF4_JAVA_TEST_LIB_TARGET}_files")
endforeach ()

add_custom_target(${HDF4_JAVA_TEST_LIB_TARGET}_files ALL COMMENT "Copying files needed by ${HDF4_JAVA_TEST_LIB_TARGET} tests" DEPENDS ${${HDF4_JAVA_TEST_LIB_TARGET}_files_list})

if (WIN32 OR MINGW)
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ";")
else ()
  set (CMAKE_JAVA_INCLUDE_FLAG_SEP ":")
endif ()

get_property (target_name TARGET ${HDF4_JAVA_JNI_LIB_TARGET} PROPERTY OUTPUT_NAME)
set (CMD_ARGS "-Dhdf.hdflib.HDFLibrary.loadLibraryName=${target_name}$<$<CONFIG:Debug>:${CMAKE_DEBUG_POSTFIX}>;")

foreach (test_file ${HDF4_JAVA_TEST_SOURCES})

  set (CMAKE_JAVA_CLASSPATH ".")
  foreach (CMAKE_INCLUDE_PATH ${CMAKE_JAVA_INCLUDE_PATH})
    set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${CMAKE_INCLUDE_PATH}")
  endforeach ()
  set (CMAKE_JAVA_CLASSPATH "${CMAKE_JAVA_CLASSPATH}${CMAKE_JAVA_INCLUDE_FLAG_SEP}${${HDF4_JAVA_TEST_LIB_TARGET}_${test_file}_JAR_FILE}")

  add_test (
    NAME JUnit-${test_file}-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
        JUnit-${test_file}.out
        JUnit-${test_file}.out.err
  )
  add_test (
      NAME JUnit-${test_file}
      COMMAND "${CMAKE_COMMAND}"
          -D "TEST_TESTER=${CMAKE_Java_RUNTIME};${CMAKE_Java_RUNTIME_FLAGS}"
          -D "TEST_CLASSPATH:STRING=${CMAKE_JAVA_CLASSPATH}"
          -D "TEST_ARGS:STRING=${CMD_ARGS}-ea;org.junit.runner.JUnitCore"
          -D "TEST_PROGRAM=test.${test_file}"
          -D "TEST_LIBRARY_DIRECTORY=${CMAKE_TEST_OUTPUT_DIRECTORY}"
          -D "TEST_FOLDER=${HDF4_BINARY_DIR}/java/test"
          -D "TEST_OUTPUT=JUnit-${test_file}.out"
#          -D "TEST_LOG_LEVEL=trace"
          -D "TEST_EXPECT=0"
          -D "SKIP_APPEND=1"
          -D "TEST_MASK_ERROR=TRUE"
#          -D "TEST_FILTER:STRING=${testfilter}"
          -D "TEST_REFERENCE=JUnit-${test_file}.txt"
          -P "${HDF_RESOURCES_DIR}/jrunTest.cmake"
  )
  set_tests_properties (JUnit-${test_file} PROPERTIES
      DEPENDS "JUnit-${test_file}-clearall-objects"
  )
endforeach ()
