
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
# Remove any output file left over from previous test run
ADD_TEST (
    NAME MFHDF_TEST-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
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

ADD_TEST (NAME hdftest COMMAND $<TARGET_FILE:hdftest>)
set (passRegex "HDF-SD test passes")
SET_PROPERTY (TEST hdftest PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
SET_TESTS_PROPERTIES (hdftest PROPERTIES DEPENDS MFHDF_TEST-clearall-objects LABELS ${PROJECT_NAME})

ADD_TEST (NAME cdftest COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:cdftest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=cdfout.new"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=testout.sav"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
)
SET_TESTS_PROPERTIES (cdftest PROPERTIES DEPENDS hdftest LABELS ${PROJECT_NAME})

ADD_TEST (NAME hdfnctest COMMAND $<TARGET_FILE:hdfnctest>)
set (NCpassRegex "HDF-nc test passes")
SET_PROPERTY (TEST hdfnctest PROPERTY PASS_REGULAR_EXPRESSION "${NCpassRegex}")
SET_TESTS_PROPERTIES (hdfnctest PROPERTIES DEPENDS cdftest LABELS ${PROJECT_NAME})

#-- Adding test for xdrtest
if (HDF4_BUILD_XDR_LIB)
  ADD_EXECUTABLE (xdrtest ${HDF4_MFHDF_XDR_DIR}/xdrtest.c)
  TARGET_NAMING (xdrtest ${LIB_TYPE})
  TARGET_C_PROPERTIES (xdrtest " " " ")
  TARGET_LINK_LIBRARIES (xdrtest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})

  if (CYGWIN)
    ADD_CUSTOM_COMMAND (
        TARGET     xdrtest 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_XDR_DIR}/xdrtest.cyg ${PROJECT_BINARY_DIR}/xdrtest.out
    )
  else (CYGWIN)
    ADD_CUSTOM_COMMAND (
        TARGET     xdrtest 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_XDR_DIR}/xdrtest.out ${PROJECT_BINARY_DIR}/xdrtest.out
    )
  endif (CYGWIN)

  if (HDF4_ENABLE_USING_MEMCHECKER)
    ADD_TEST (NAME xdrtest COMMAND $<TARGET_FILE:xdrtest>)
  else (HDF4_ENABLE_USING_MEMCHECKER)
    ADD_TEST (
        NAME xdrtest
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:xdrtest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=xdrtest.tst"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=xdrtest.out"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
  endif (HDF4_ENABLE_USING_MEMCHECKER)
  SET_TESTS_PROPERTIES (xdrtest PROPERTIES DEPENDS hdfnctest LABELS ${PROJECT_NAME})
endif (HDF4_BUILD_XDR_LIB)
