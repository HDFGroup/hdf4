
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE_TEST_FILES
    hdfimport.input1
    hdfimport.out1
#      hdfimport.out2
    hdfimport.out3
    SDSfloat2.hdf
    SDSfloat3.hdf
)
#-- Copy all the hdfls tst files from the test directory into the source directory
set (HDF4_LS_TEST_FILES
    ctxtr2.tst
    ctxtr3.tst
    cb32i2.tst
    cb32i3.tst
    cb16i2.tst
    cb16i3.tst
    cb32r2.tst
    cb32r3.tst
    cb64r2.tst
    cb64r3.tst
    cb64r2-n.tst
    cb64r3-n.tst
    ctxtr2_ris.tst
    cb64r2_ris.tst
    SDSfloat2.tst
    SDSfloat3.tst
)

foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
  HDFTEST_COPY_FILE("${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${h4_file}" "${PROJECT_BINARY_DIR}/${h4_file}" "hdfimport_files")
endforeach ()

foreach (h4_file ${HDF4_LS_TEST_FILES})
  HDFTEST_COPY_FILE("${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/testfiles/${h4_file}" "${PROJECT_BINARY_DIR}/${h4_file}" "hdfimport_files")
endforeach ()

if (WIN32 AND MSVC_VERSION LESS 1900)
  HDFTEST_COPY_FILE("${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport-w.out2" "${PROJECT_BINARY_DIR}/hdfimport.out2" "hdfimport_files")
else ()
  HDFTEST_COPY_FILE("${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.out2" "${PROJECT_BINARY_DIR}/hdfimport.out2" "hdfimport_files")
endif ()

add_custom_target(hdfimport_files ALL COMMENT "Copying files needed by hdfimport tests" DEPENDS ${hdfimport_files_list})

#-- hdfimporttest
set (hdfimporttest_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimporttest.c
)

add_executable (hdfimporttest ${hdfimporttest_SRCS})
if (NOT ONLY_SHARED_LIBS)
  TARGET_C_PROPERTIES (hdfimporttest STATIC)
  target_link_libraries (hdfimporttest PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (hdfimporttest SHARED)
  target_link_libraries (hdfimporttest PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()

#-- gen_sds_floats
set (gen_sds_floats_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/gen_sds_floats.c
)

add_executable (gen_sds_floats ${gen_sds_floats_SRCS})
if (NOT ONLY_SHARED_LIBS)
  TARGET_C_PROPERTIES (gen_sds_floats STATIC)
  target_link_libraries (gen_sds_floats PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (gen_sds_floats SHARED)
  target_link_libraries (gen_sds_floats PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()

##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

if (NOT BUILD_SHARED_LIBS)
  set (tgt_ext "")
else ()
  set (tgt_ext "-shared")
endif ()

macro (ADD_H4_TEST resultfile resultcode testtfile testtype)
  if (NOT ${testtype} STREQUAL "")
    if (${testtype} STREQUAL "N")
      add_test (NAME HIMPORT-${testtfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfimport${tgt_ext}> ${resultfile} -n -o ${testtfile}.hdf)
    endif ()
    if (${testtype} STREQUAL "R")
      add_test (NAME HIMPORT-${testtfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfimport${tgt_ext}> ${resultfile} -o ${testtfile}.hdf -raster ${ARGN})
    endif ()
  else ()
    add_test (NAME HIMPORT-${testtfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfimport${tgt_ext}> ${resultfile} -o ${testtfile}.hdf)
  endif ()
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HIMPORT-${testtfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (HIMPORT-${testtfile} PROPERTIES LABELS ${PROJECT_NAME})
  endif ()

  if (HDF4_ENABLE_USING_MEMCHECKER)
    add_test (NAME HIMPORTLS-${testtfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfls${tgt_ext}> -l ${testtfile}.hdf)
  else ()
    add_test (
        NAME HIMPORTLS-${testtfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfls${tgt_ext}>"
            -D "TEST_ARGS:STRING=-l;${testtfile}.hdf"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=${testtfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
            -D "TEST_REFERENCE=${testtfile}.tst"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
  endif ()
  set_tests_properties (HIMPORTLS-${testtfile} PROPERTIES DEPENDS HIMPORT-${testtfile} LABELS ${PROJECT_NAME})
  set (last_test "HIMPORTLS-${testtfile}")
endmacro ()

macro (ADD_H4_TEST_OUT resultfile resultcode)
  add_test (NAME HIMPORT-OUT-${resultfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfimport${tgt_ext}> ${resultfile}.hdf -o ${resultfile}.out)
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HIMPORT-OUT-${resultfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (HIMPORT-OUT-${resultfile} PROPERTIES LABELS ${PROJECT_NAME})
  endif ()

  if (HDF4_ENABLE_USING_MEMCHECKER)
    add_test (NAME HIMPORTLS-OUT-${resultfile} COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfls${tgt_ext}> ${resultfile}.out)
  else ()
    add_test (
        NAME HIMPORTLS-OUT-${resultfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfls${tgt_ext}>"
            -D "TEST_ARGS:STRING=${resultfile}.out"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=${resultfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
            -D "TEST_REFERENCE=${resultfile}.tst"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
  endif ()
  set_tests_properties (HIMPORTLS-OUT-${resultfile} PROPERTIES DEPENDS HIMPORT-OUT-${resultfile} LABELS ${PROJECT_NAME})
  set (last_test "HIMPORTLS-OUT-${resultfile}")
endmacro ()

macro (ADD_H4_TEST_ED testfile resultfile resultcode)
  if (HDF4_ENABLE_USING_MEMCHECKER)
    add_test (NAME HIMPORT-EDIT COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfed${tgt_ext}> -batch)
  else ()
    add_test (
        NAME HIMPORT-EDIT
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfed${tgt_ext}>"
            -D "TEST_ARGS:STRING=-batch"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_INPUT=${testfile}"
            -D "TEST_OUTPUT=${testfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_REFERENCE=${resultfile}"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
  endif ()
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HIMPORT-EDIT PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (HIMPORT-EDIT PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "HIMPORT-EDIT")
endmacro ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
    NAME hdfimport-clear-refs
    COMMAND ${CMAKE_COMMAND} -E remove
        ctxtr2
        ctxtr3
        cb32i2
        cb32i3
        cb16i2
        cb16i3
        cb32r2
        cb32r3
        cb64r2
        cb64r3
        cb64r2-n
        cb64r3-n
        ctxtr2_ris
        cb64r2_ris
        ctxtr2.hdf
        ctxtr3.hdf
        cb32i2.hdf
        cb32i3.hdf
        cb16i2.hdf
        cb16i3.hdf
        cb32r2.hdf
        cb32r3.hdf
        cb64r2.hdf
        cb64r3.hdf
        cb64r2-n.hdf
        cb64r3-n.hdf
        ctxtr2_ris.hdf
        cb64r2_ris.hdf
        ctxtr2.tmp
        ctxtr3.tmp
        cb32i2.tmp
        cb32i3.tmp
        cb16i2.tmp
        cb16i3.tmp
        cb32r2.tmp
        cb32r3.tmp
        cb64r2.tmp
        cb64r3.tmp
        cb64r2-n.tmp
        cb64r3-n.tmp
        ctxtr2_ris.tmp
        cb64r2_ris.tmp
        SDSfloat2.tmp
        SDSfloat3.tmp
        ctxtr2.tmp.err
        ctxtr3.tmp.err
        cb32i2.tmp.err
        cb32i3.tmp.err
        cb16i2.tmp.err
        cb16i3.tmp.err
        cb32r2.tmp.err
        cb32r3.tmp.err
        cb64r2.tmp.err
        cb64r3.tmp.err
        cb64r2-n.tmp.err
        cb64r3-n.tmp.err
        ctxtr2_ris.tmp.err
        cb64r2_ris.tmp.err
        SDSfloat2.tmp.err
        SDSfloat3.tmp.err
)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (hdfimport-clear-refs PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
else ()
  set_tests_properties (hdfimport-clear-refs PROPERTIES LABELS ${PROJECT_NAME})
endif ()
set (last_test "hdfimport-clear-refs")

add_test (NAME HIMPORTtest COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:hdfimporttest>)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HIMPORTtest PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
else ()
  set_tests_properties (HIMPORTtest PROPERTIES LABELS ${PROJECT_NAME})
endif ()
set (last_test "HIMPORTtest")

# "Testing for 32-bit floating point ASCII (2D data)"
ADD_H4_TEST (ctxtr2 0 ctxtr2 "")
# "Testing for 32-bit floating point ASCII (3D data)"
ADD_H4_TEST (ctxtr3 0 ctxtr3 "")
# "Testing for 32-bit integer binary (2D data)"
ADD_H4_TEST (cb32i2 0 cb32i2 "")
# "Testing for 32-bit integer  binary (3D data)"
ADD_H4_TEST (cb32i3 0 cb32i3 "")
# "Testing for 16-bit integer binary (2D data)"
ADD_H4_TEST (cb16i2 0 cb16i2 "")
# "Testing for 16-bit integer (3D data)"
ADD_H4_TEST (cb16i3 0 cb16i3 "")
# "Testing for 32-bit floating point binary (2D data)"
ADD_H4_TEST (cb32r2 0 cb32r2 "")
# "Testing for 32-bit floating point binary (3D data)"
ADD_H4_TEST (cb32r3 0 cb32r3 "")
# "Testing for 64-bit floating point binary (2D data) - Default Behaviour (Conversion to 32 bit FP SDS)"
ADD_H4_TEST (cb64r2 0 cb64r2 "")
# "Testing for 64-bit floating point binary (3D data) - Default Behaviour (Conversion to 32-bit FP SDS)"
ADD_H4_TEST (cb64r3 0 cb64r3 "")
# "Testing for 64-bit floating point binary (2D data) - Conversion to 64-bit FP SDS"
ADD_H4_TEST (cb64r2 0 cb64r2-n "N")
# "Testing for 64-bit floating point binary (3D data) - Conversion to 64-bit FP SDS"
ADD_H4_TEST (cb64r3 0 cb64r3-n "N")
# "Testing for raster options"
ADD_H4_TEST (ctxtr2 0 ctxtr2_ris "R" -e 50 50)
ADD_H4_TEST (cb64r2 0 cb64r2_ris "R" -i 50 50 -f)
#
# test with hdf files
# "Testing for reading from hdf files"
ADD_H4_TEST_OUT (SDSfloat2 0)
ADD_H4_TEST_OUT (SDSfloat3 0)

ADD_H4_TEST_ED (hdfimport.input1 hdfimport.out2 0)
