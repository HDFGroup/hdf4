
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

  foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
      set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
      #message (STATUS " Copying ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
      add_custom_command (
          TARGET     hdfimport
          POST_BUILD
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${h4_file} ${dest}
     )
  endforeach ()

  if (WIN32 AND MSVC_VERSION LESS 1900)
    add_custom_command (
        TARGET     hdfimport
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport-w.out2 ${PROJECT_BINARY_DIR}/hdfimport.out2
    )
  else ()
    add_custom_command (
        TARGET     hdfimport
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.out2 ${PROJECT_BINARY_DIR}/hdfimport.out2
    )
  endif ()

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

  foreach (ls_file ${HDF4_LS_TEST_FILES})
      set (dest "${PROJECT_BINARY_DIR}/${ls_file}")
      #message (STATUS " Copying ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${ls_file} to ${PROJECT_BINARY_DIR}/")
      ADD_CUSTOM_COMMAND (
          TARGET     hdfimport
          POST_BUILD
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/testfiles/${ls_file} ${dest}
     )
  endforeach ()

  #-- hdfimporttest
  set (hdfimporttest_SRCS
      ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimporttest.c
  )

  add_executable (hdfimporttest ${hdfimporttest_SRCS})
  TARGET_C_PROPERTIES (hdfimporttest STATIC " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  else ()
    target_link_libraries (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  endif ()
  TARGET_NAMING (hdfimporttest STATIC)

  #-- gen_sds_floats
  set (gen_sds_floats_SRCS
      ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/gen_sds_floats.c
  )

  add_executable (gen_sds_floats ${gen_sds_floats_SRCS})
  TARGET_C_PROPERTIES (gen_sds_floats STATIC " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  else ()
    target_link_libraries (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  endif ()
  TARGET_NAMING (gen_sds_floats STATIC)

##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

  macro (ADD_H4_TEST resultfile resultcode testtfile testtype)
    if (NOT ${testtype} STREQUAL "")
      if (${testtype} STREQUAL "N")
        add_test (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -n -o ${testtfile}.hdf)
      endif ()
      if (${testtype} STREQUAL "R")
        add_test (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -o ${testtfile}.hdf -raster ${ARGN})
      endif ()
    else ()
      add_test (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -o ${testtfile}.hdf)
    endif ()
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HIMPORT-${testtfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (HIMPORT-${testtfile} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()

    if (HDF4_ENABLE_USING_MEMCHECKER)
      add_test (NAME HIMPORTLS-${testtfile} COMMAND $<TARGET_FILE:hdfls> -l ${testtfile}.hdf)
    else ()
      add_test (
          NAME HIMPORTLS-${testtfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
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
    add_test (NAME HIMPORT-OUT-${resultfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile}.hdf -o ${resultfile}.out)
    if (NOT "${last_test}" STREQUAL "")
      set_tests_properties (HIMPORT-OUT-${resultfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    else ()
      set_tests_properties (HIMPORT-OUT-${resultfile} PROPERTIES LABELS ${PROJECT_NAME})
    endif ()

    if (HDF4_ENABLE_USING_MEMCHECKER)
      add_test (NAME HIMPORTLS-OUT-${resultfile} COMMAND $<TARGET_FILE:hdfls> ${resultfile}.out)
    else ()
      add_test (
          NAME HIMPORTLS-OUT-${resultfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
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
      add_test (NAME HIMPORT-EDIT COMMAND $<TARGET_FILE:hdfed> -batch)
    else ()
      add_test (
          NAME HIMPORT-EDIT
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdfed>"
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
      COMMAND    ${CMAKE_COMMAND}
          -E remove
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

  add_test (NAME HIMPORTtest COMMAND $<TARGET_FILE:hdfimporttest>)
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
