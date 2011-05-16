cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_TEST)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (hdftest_SRCS
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdftest.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tchunk.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcomp.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcoordvar.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/temptySDSs.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tfile.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tidtypes.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tnetcdf.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/trank0.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsd.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsdsprops.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tszip.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tattdatainfo.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdatainfo.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdatasizes.c
)

#-- Adding test for hdftest
ADD_EXECUTABLE (hdftest ${hdftest_SRCS})
TARGET_NAMING (hdftest ${LIB_TYPE})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

#-- Copy all the dat files from the test directory into the source directory
SET (HDF4_REFERENCE_TEST_FILES
    sds_szipped.dat
    smallslice.0000.nc
    test1.nc
    testout.sav
)

FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
     TARGET     hdftest
     POST_BUILD
     COMMAND    ${CMAKE_COMMAND}
     ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
     )

ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

#-- Adding test for cdftest
ADD_EXECUTABLE (cdftest ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
TARGET_NAMING (cdftest ${LIB_TYPE})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (hdfnctest_SRCS
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdfnctest.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tunlim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncunlim.c
)

#-- Adding test for hdfnctest
ADD_EXECUTABLE (hdfnctest ${hdfnctest_SRCS})
TARGET_NAMING (hdfnctest ${LIB_TYPE})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

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
)

ADD_TEST (NAME hdftest COMMAND $<TARGET_FILE:hdftest>)
SET (passRegex "HDF-SD test passes")
SET_PROPERTY (TEST hdftest PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
SET_TESTS_PROPERTIES (hdftest PROPERTIES DEPENDS MFHDF_TEST-clearall-objects LABELS ${PROJECT_NAME})

ADD_TEST (NAME cdftest COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:cdftest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=cdfout.new"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=testout.sav"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
)
SET_TESTS_PROPERTIES (cdftest PROPERTIES DEPENDS hdftest LABELS ${PROJECT_NAME})

ADD_TEST (NAME hdfnctest COMMAND $<TARGET_FILE:hdfnctest>)
SET (NCpassRegex "HDF-nc test passes")
SET_PROPERTY (TEST hdfnctest PROPERTY PASS_REGULAR_EXPRESSION "${NCpassRegex}")
SET_TESTS_PROPERTIES (hdfnctest PROPERTIES DEPENDS cdftest LABELS ${PROJECT_NAME})

#-- Adding test for xdrtest
IF (HDF4_BUILD_XDR_LIB)
  ADD_EXECUTABLE (xdrtest ${HDF4_MFHDF_XDR_DIR}/xdrtest.c)
  TARGET_NAMING (xdrtest ${LIB_TYPE})
  TARGET_LINK_LIBRARIES (xdrtest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})

  ADD_CUSTOM_COMMAND (
      TARGET     xdrtest 
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_XDR_DIR}/xdrtest.out ${PROJECT_BINARY_DIR}/xdrtest.out
  )

  IF (HDF4_ENABLE_USING_MEMCHECKER)
    ADD_TEST (NAME xdrtest COMMAND $<TARGET_FILE:xdrtest>)
  ELSE (HDF4_ENABLE_USING_MEMCHECKER)
    ADD_TEST (
        NAME xdrtest
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:xdrtest>"
            -D "TEST_ARGS:STRING="
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=xdrtest.tst"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=xdrtest.out"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
    )
  ENDIF (HDF4_ENABLE_USING_MEMCHECKER)
  SET_TESTS_PROPERTIES (xdrtest PROPERTIES DEPENDS hdfnctest LABELS ${PROJECT_NAME})
ENDIF (HDF4_BUILD_XDR_LIB)
