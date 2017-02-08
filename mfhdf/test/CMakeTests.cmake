
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################
file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST")
if (BUILD_SHARED_LIBS)
  file (MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/TEST-shared")
endif ()

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE_TEST_FILES
    sds_szipped.dat
    smallslice.0000.nc
    test1.nc
    Roy.nc
    Roy-64.nc
)

foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
   set (dest "${PROJECT_BINARY_DIR}/TEST/${h4_file}")
   add_custom_command (
       TARGET     hdftest
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
   )
   if (BUILD_SHARED_LIBS)
     set (dest "${PROJECT_BINARY_DIR}/TEST-shared/${h4_file}")
     add_custom_command (
         TARGET     hdftest-shared
         POST_BUILD
         COMMAND    ${CMAKE_COMMAND}
         ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
   )
   endif ()
endforeach ()

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE2_TEST_FILES
    testout.sav
)

foreach (h4_file ${HDF4_REFERENCE2_TEST_FILES})
   set (dest "${PROJECT_BINARY_DIR}/TEST/${h4_file}")
   add_custom_command (
       TARGET     cdftest
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
   )
   if (BUILD_SHARED_LIBS)
     set (dest "${PROJECT_BINARY_DIR}/TEST-shared/${h4_file}")
     add_custom_command (
         TARGET     cdftest-shared
         POST_BUILD
         COMMAND    ${CMAKE_COMMAND}
         ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
     )
   endif ()
endforeach ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
# Remove any output file left over from previous test run
set (HDF4_TESTMFHDF_FILES
    b150.hdf
    bug376.hdf
    cdfout.new
    cdfout.new.err
    chkbit.hdf
    chktst.hdf
    comptst1.hdf
    comptst2.hdf
    comptst3.hdf
    comptst4.hdf
    comptst5.hdf
    comptst6.hdf
    comptst7.hdf
    datainfo_chk.hdf
    datainfo_chkcmp.hdf
    datainfo_cmp.hdf
    datainfo_extend.hdf
    datainfo_nodata.hdf
    datainfo_simple.hdf
    datasizes.hdf
    dim.hdf
    emptySDSs.hdf
    extfile.hdf
    exttst.hdf
    idtypes.hdf
    multidimvar.nc
    nbit.hdf
    onedimmultivars.nc
    onedimonevar.nc
    scaletst.hdf
    sds1_dim1_samename.hdf
    sds2_dim1_samename.hdf
    SDS_8_sziped.hdf
    SDS_16_sziped.hdf
    SDS_32_sziped.hdf
    sds_compressed.hdf
    SD_externals
    SDS_fl32_sziped.hdf
    SDS_fl64_sziped.hdf
    sds_szipped.hdf
    SDSchunkedsziped.hdf
    SDSchunkedsziped3d.hdf
    SDSlongname.hdf
    SDSunlimitedsziped.hdf
    test.cdf
    test1.hdf
    test2.hdf
    test_arguments.hdf
    'This file name has quite a few characters because it is used to test the fix of bugzilla 1331. It has to be at least this long to see.'
    Unlim_dim.hdf
    Unlim_inloop.hdf
    vars_samename.hdf
    tdfanndg.hdf
    tdfansdg.hdf
)
add_test (
    NAME MFHDF_TEST-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove
        ${HDF4_TESTMFHDF_FILES}
    WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}/TEST
)

add_test (NAME MFHDF_TEST-hdftest COMMAND $<TARGET_FILE:hdftest>)
set (passRegex "HDF-SD test passes")
set_tests_properties (MFHDF_TEST-hdftest PROPERTIES
    PASS_REGULAR_EXPRESSION "${passRegex}"
    DEPENDS MFHDF_TEST-clearall-objects
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST
    LABELS ${PROJECT_NAME}
)

add_test (NAME MFHDF_TEST-cdftest COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:cdftest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}/TEST"
            -D "TEST_OUTPUT=cdfout.new"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=testout.sav"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
)
set_tests_properties (MFHDF_TEST-cdftest PROPERTIES
    DEPENDS MFHDF_TEST-hdftest
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST
    LABELS ${PROJECT_NAME}
)

add_test (NAME MFHDF_TEST-hdfnctest COMMAND $<TARGET_FILE:hdfnctest>)
set (NCpassRegex "HDF-nc test passes")
set_tests_properties (MFHDF_TEST-hdfnctest PROPERTIES
    PASS_REGULAR_EXPRESSION "${NCpassRegex}"
    DEPENDS MFHDF_TEST-cdftest
    LABELS ${PROJECT_NAME}
)
if (BUILD_SHARED_LIBS)
  add_test (
      NAME MFHDF_TEST-shared-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          ${HDF4_TESTMFHDF_FILES}
      WORKING_DIRECTORY
          ${PROJECT_BINARY_DIR}/TEST-shared
  )

  add_test (NAME MFHDF_TEST-hdftest-shared COMMAND $<TARGET_FILE:hdftest-shared>)
  set (passRegex "HDF-SD test passes")
  set_tests_properties (MFHDF_TEST-hdftest-shared PROPERTIES
      PASS_REGULAR_EXPRESSION "${passRegex}"
      DEPENDS MFHDF_TEST-shared-clearall-objects
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST-shared
      LABELS ${PROJECT_NAME}
  )

  add_test (NAME MFHDF_TEST-cdftest-shared COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:cdftest-shared>"
              -D "TEST_ARGS:STRING="
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}/TEST-shared"
              -D "TEST_OUTPUT=cdfout.new"
              -D "TEST_EXPECT=0"
              -D "TEST_REFERENCE=testout.sav"
              -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
  )
  set_tests_properties (MFHDF_TEST-cdftest-shared PROPERTIES
      DEPENDS MFHDF_TEST-hdftest-shared
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST-shared
      LABELS ${PROJECT_NAME}
  )

  add_test (NAME MFHDF_TEST-hdfnctest-shared COMMAND $<TARGET_FILE:hdfnctest-shared>)
  set (NCpassRegex "HDF-nc test passes")
  set_tests_properties (MFHDF_TEST-hdfnctest-shared PROPERTIES
      PASS_REGULAR_EXPRESSION "${NCpassRegex}"
      DEPENDS MFHDF_TEST-cdftest-shared
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/TEST-shared
      LABELS ${PROJECT_NAME}
  )
endif ()

#-- Adding test for xdrtest
if (HDF4_BUILD_XDR_LIB)
  add_executable (xdrtest ${HDF4_MFHDF_XDR_DIR}/xdrtest.c)
  TARGET_NAMING (xdrtest STATIC)
  TARGET_C_PROPERTIES (xdrtest STATIC " " " ")
  target_link_libraries (xdrtest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})

  if (MSVC_VERSION LESS 1900)
    add_custom_command (
        TARGET     xdrtest
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_XDR_DIR}/xdrtest.out ${PROJECT_BINARY_DIR}/TEST/xdrtest.out
    )
  else ()
    add_custom_command (
        TARGET     xdrtest
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_XDR_DIR}/xdrtest.cyg ${PROJECT_BINARY_DIR}/TEST/xdrtest.out
    )
  endif ()

  if (HDF4_ENABLE_USING_MEMCHECKER)
    add_test (NAME MFHDF_TEST-xdrtest COMMAND $<TARGET_FILE:xdrtest>)
  else ()
    add_test (
        NAME MFHDF_TEST-xdrtest
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:xdrtest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}/TEST"
            -D "TEST_OUTPUT=xdrtest.tst"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=xdrtest.out"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
  endif ()
  set_tests_properties (MFHDF_TEST-xdrtest PROPERTIES DEPENDS hdfnctest LABELS ${PROJECT_NAME})
endif ()
