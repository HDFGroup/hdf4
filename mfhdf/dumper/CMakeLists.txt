cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_DUMPER)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

SET (hdp_SRCS
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_dump.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_gr.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_list.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_rig.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_sds.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_util.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vd.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vg.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/show.c
)
  
ADD_EXECUTABLE (hdp ${hdp_SRCS})
IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)
IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  TARGET_LINK_LIBRARIES (hdp ${HDF4_MF_XDR_LIB_TARGET})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_LINK_LIBRARIES (hdp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
TARGET_NAMING (hdp ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

IF (BUILD_TESTING)
  
  #-- Copy all the dat files from the test directory into the source directory
  SET (HDF4_REFERENCE_TEST_FILES
      ctxtr2r.hdf
      Example6.hdf
      grtdfi322.hdf
      grtdfui162.hdf
      grtdfui82.hdf
      grtdfui83.hdf
      grtdfui84.hdf
      Image_with_Palette.hdf
      LongDataset.hdf
      sds_compressed.hdf
      sds_empty_many.hdf
      sds1_dim1_samename.hdf
      sds2_dim1_samename.hdf
      SDSlongname.hdf
      star.hdf
      swf32_fileattr.hdf
      swf32.hdf
      swi16.hdf
      swi8.hdf
      tdata.hdf
      tdf24.hdf
      tdfr8f.hdf
      test.hdf
      tvattr.hdf
      tvset.hdf
      VGlongname.hdf
      vslongname.hdf
  )
  
  SET (HDF4_REFERENCE_FILES
      dumpgr-1.out
      dumpgr-10.out
      dumpgr-11.out
      dumpgr-12.out
      dumpgr-13.out
      dumpgr-14.out
      dumpgr-15.out
      dumpgr-16.out
      dumpgr-17.out
      dumpgr-18.out
      dumpgr-19.out
      dumpgr-2.out
      dumpgr-3.out
      dumpgr-4.out
      dumpgr-5.out
      dumpgr-6.out
      dumpgr-7.out
      dumpgr-8.out
      dumpgr-9.out
      dumprig-1.out
      dumprig-2.out
      dumprig-3.out
      dumprig-4.out
      dumprig-5.out
      dumprig-6.out
      dumpsds-1.out
      dumpsds-10.out
      dumpsds-11.out
      dumpsds-12.out
      dumpsds-13.out
      dumpsds-14.out
      dumpsds-15.out
      dumpsds-15szip.out
      dumpsds-16.out
      dumpsds-17.out
      dumpsds-2.out
      dumpsds-3.out
      dumpsds-4.out
      dumpsds-5.out
      dumpsds-6.out
      dumpsds-7.out
      dumpsds-8.out
      dumpsds-9.out
      dumpvd-1.out
      dumpvd-10.out
      dumpvd-11.out
      dumpvd-12.out
      dumpvd-13.out
      dumpvd-2.out
      dumpvd-3.out
      dumpvd-4.out
      dumpvd-5.out
      dumpvd-6.out
      dumpvd-7.out
      dumpvd-8.out
      dumpvd-9.out
      dumpvg-1.out
      dumpvg-10.out
      dumpvg-11.out
      dumpvg-12.out
      dumpvg-13.out
      dumpvg-14.out
      dumpvg-15.out
      dumpvg-2.out
      dumpvg-3.out
      dumpvg-4.out
      dumpvg-5.out
      dumpvg-6.out
      dumpvg-7.out
      dumpvg-8.out
      dumpvg-9.out
      list-1.out
      list-10.out
      list-2.out
      list-3.out
      list-4.out
      list-5.out
      list-6.out
      list-7.out
      list-8.out
      list-9.out
  )
 
  FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
    SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #MESSAGE (STATUS " Copying ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdp 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

  FOREACH (out_file ${HDF4_REFERENCE_FILES})
    SET (outdest "${PROJECT_BINARY_DIR}/${out_file}")
    #MESSAGE (STATUS " Translating ${out_file}")
    ADD_CUSTOM_COMMAND (
        TARGET     hdp
        POST_BUILD
        COMMAND    ${XLATE_UTILITY}
        ARGS       ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/testfiles/${out_file} ${outdest} -l3
    )
  ENDFOREACH (out_file ${HDF4_REFERENCE_FILES})
  
##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

  MACRO (ADD_H4_TEST resultfile resultcode)
    IF (HDF4_ENABLE_USING_MEMCHECKER)
      ADD_TEST (NAME HDP-${resultfile} COMMAND $<TARGET_FILE:hdp> ${ARGN})
    ELSE (HDF4_ENABLE_USING_MEMCHECKER)
      ADD_TEST (
          NAME HDP-${resultfile}
          COMMAND "${CMAKE_COMMAND}"
              -D "TEST_PROGRAM=$<TARGET_FILE:hdp>"
              -D "TEST_ARGS:STRING=${ARGN}"
              -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
              -D "TEST_OUTPUT=${resultfile}.tst"
              -D "TEST_EXPECT=${resultcode}"
              -D "TEST_REFERENCE=${resultfile}.out"
              -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
      )
    ENDIF (HDF4_ENABLE_USING_MEMCHECKER)
    IF (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (HDP-${resultfile} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
    ELSE (NOT "${last_test}" STREQUAL "")
      SET_TESTS_PROPERTIES (HDP-${resultfile} PROPERTIES LABELS ${PROJECT_NAME})
    ENDIF (NOT "${last_test}" STREQUAL "")
    SET (last_test "HDP-${resultfile}")
  ENDMACRO (ADD_H4_TEST file)

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
  
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME HDP_tst-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          dumpgr-1.tst
          dumpgr-10.tst
          dumpgr-11.tst
          dumpgr-12.tst
          dumpgr-13.tst
          dumpgr-14.tst
          dumpgr-15.tst
          dumpgr-16.tst
          dumpgr-17.tst
          dumpgr-18.tst
          dumpgr-19.tst
          dumpgr-2.tst
          dumpgr-3.tst
          dumpgr-4.tst
          dumpgr-5.tst
          dumpgr-6.tst
          dumpgr-7.tst
          dumpgr-8.tst
          dumpgr-9.tst
          dumprig-1.tst
          dumprig-2.tst
          dumprig-3.tst
          dumprig-4.tst
          dumprig-5.tst
          dumprig-6.tst
          dumpsds-1.tst
          dumpsds-10.tst
          dumpsds-11.tst
          dumpsds-12.tst
          dumpsds-13.tst
          dumpsds-14.tst
          dumpsds-15.tst
          dumpsds-15szip.tst
          dumpsds-16.tst
          dumpsds-17.tst
          dumpsds-2.tst
          dumpsds-3.tst
          dumpsds-4.tst
          dumpsds-5.tst
          dumpsds-6.tst
          dumpsds-7.tst
          dumpsds-8.tst
          dumpsds-9.tst
          dumpvd-1.tst
          dumpvd-10.tst
          dumpvd-11.tst
          dumpvd-12.tst
          dumpvd-13.tst
          dumpvd-2.tst
          dumpvd-3.tst
          dumpvd-4.tst
          dumpvd-5.tst
          dumpvd-6.tst
          dumpvd-7.tst
          dumpvd-8.tst
          dumpvd-9.tst
          dumpvg-1.tst
          dumpvg-10.tst
          dumpvg-11.tst
          dumpvg-12.tst
          dumpvg-13.tst
          dumpvg-14.tst
          dumpvg-15.tst
          dumpvg-2.tst
          dumpvg-3.tst
          dumpvg-4.tst
          dumpvg-5.tst
          dumpvg-6.tst
          dumpvg-7.tst
          dumpvg-8.tst
          dumpvg-9.tst
          list-1.tst
          list-10.tst
          list-2.tst
          list-3.tst
          list-4.tst
          list-5.tst
          list-6.tst
          list-7.tst
          list-8.tst
          list-9.tst
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP_tst-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP_tst-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDP_tst-clearall-objects")
  
  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME HDP_tst_err-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          dumpgr-1.tst.err
          dumpgr-10.tst.err
          dumpgr-11.tst.err
          dumpgr-12.tst.err
          dumpgr-13.tst.err
          dumpgr-14.tst.err
          dumpgr-15.tst.err
          dumpgr-16.tst.err
          dumpgr-17.tst.err
          dumpgr-18.tst.err
          dumpgr-19.tst.err
          dumpgr-2.tst.err
          dumpgr-3.tst.err
          dumpgr-4.tst.err
          dumpgr-5.tst.err
          dumpgr-6.tst.err
          dumpgr-7.tst.err
          dumpgr-8.tst.err
          dumpgr-9.tst.err
          dumprig-1.tst.err
          dumprig-2.tst.err
          dumprig-3.tst.err
          dumprig-4.tst.err
          dumprig-5.tst.err
          dumprig-6.tst.err
          dumpsds-1.tst.err
          dumpsds-10.tst.err
          dumpsds-11.tst.err
          dumpsds-12.tst.err
          dumpsds-13.tst.err
          dumpsds-14.tst.err
          dumpsds-15.tst.err
          dumpsds-15szip.tst.err
          dumpsds-16.tst.err
          dumpsds-17.tst.err
          dumpsds-2.tst.err
          dumpsds-3.tst.err
          dumpsds-4.tst.err
          dumpsds-5.tst.err
          dumpsds-6.tst.err
          dumpsds-7.tst.err
          dumpsds-8.tst.err
          dumpsds-9.tst.err
          dumpvd-1.tst.err
          dumpvd-10.tst.err
          dumpvd-11.tst.err
          dumpvd-12.tst.err
          dumpvd-13.tst.err
          dumpvd-2.tst.err
          dumpvd-3.tst.err
          dumpvd-4.tst.err
          dumpvd-5.tst.err
          dumpvd-6.tst.err
          dumpvd-7.tst.err
          dumpvd-8.tst.err
          dumpvd-9.tst.err
          dumpvg-1.tst.err
          dumpvg-10.tst.err
          dumpvg-11.tst.err
          dumpvg-12.tst.err
          dumpvg-13.tst.err
          dumpvg-14.tst.err
          dumpvg-15.tst.err
          dumpvg-2.tst.err
          dumpvg-3.tst.err
          dumpvg-4.tst.err
          dumpvg-5.tst.err
          dumpvg-6.tst.err
          dumpvg-7.tst.err
          dumpvg-8.tst.err
          dumpvg-9.tst.err
          list-1.tst.err
          list-10.tst.err
          list-2.tst.err
          list-3.tst.err
          list-4.tst.err
          list-5.tst.err
          list-6.tst.err
          list-7.tst.err
          list-8.tst.err
          list-9.tst.err
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP_tst_err-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP_tst_err-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDP_tst_err-clearall-objects")

  ADD_H4_TEST (list-1 0 list tdata.hdf)
  ADD_H4_TEST (list-2 0 list -l tdata.hdf)
  ADD_H4_TEST (list-3 0 list -d tdata.hdf)
  ADD_H4_TEST (list-4 0 list -e tdata.hdf)
  ADD_H4_TEST (list-5 0 list -t 720 tdata.hdf)
  ADD_H4_TEST (list-6 0 list -d -t "Numeric Data Group" tdata.hdf)
  ADD_H4_TEST (list-7 0 list -g tdata.hdf)
  ADD_H4_TEST (list-8 0 list -a tdata.hdf)
  ADD_H4_TEST (list-9 0 list -a Example6.hdf)
  ADD_H4_TEST (list-10 0 list -n Example6.hdf)

  # Test 1 prints all datasets
  ADD_H4_TEST (dumpsds-1 0 dumpsds swf32.hdf)

  # Tests 2 and 3 print datasets given their indices
  ADD_H4_TEST (dumpsds-2 0 dumpsds -i 2 swf32.hdf)
  ADD_H4_TEST (dumpsds-3 0 dumpsds -i 1,3 swf32.hdf)

  # Test 4 should fail with error message: "SD with name Time: not found"
  ADD_H4_TEST (dumpsds-4 0 dumpsds -n Time swf32.hdf)

  # Test 5 prints datasets given their names 
  ADD_H4_TEST (dumpsds-5 0 dumpsds -n fakeDim0,Data-Set-2 swf32.hdf)

  # Test 6 prints datasets given their ref numbers
  ADD_H4_TEST (dumpsds-6 0 dumpsds -r 3,2 swf32.hdf)

  # Test 7 prints only data of the datasets selected by their ref numbers
  ADD_H4_TEST (dumpsds-7 0 dumpsds -r 3,2 -d swf32.hdf)

  # Test 8 prints only header information
  ADD_H4_TEST (dumpsds-8 0 dumpsds -h swf32_fileattr.hdf)

  # Test 9 prints data in clean format, no \digit's
  ADD_H4_TEST (dumpsds-9 0 dumpsds -c swf32_fileattr.hdf)

  # Test 10 prints contents of file without file attribute's data
  ADD_H4_TEST (dumpsds-10 0 dumpsds -g swf32_fileattr.hdf)

  # Test 11 prints contents of file without local attribute's data
  ADD_H4_TEST (dumpsds-11 0 dumpsds -l swf32_fileattr.hdf)

  # Test 12 prints a dataset by name and the name is very long
  ADD_H4_TEST (dumpsds-12 0 dumpsds -h -n "The name of this dataset is long and it is used to test the new variable length name feature." SDSlongname.hdf)

  # Test 13 prints contents of file when a dimension has the same name as its SDS
  ADD_H4_TEST (dumpsds-13 0 dumpsds sds1_dim1_samename.hdf)

  # Test 14 prints contents of file when a dimension has the same name as 
  # that of another SDS
  ADD_H4_TEST (dumpsds-14 0 dumpsds sds2_dim1_samename.hdf)

  # Test 15 prints headers of all data sets with various compression method to
  # test displaying compression information

  IF (HDF4_ENABLE_SZIP_SUPPORT)
    ADD_H4_TEST (dumpsds-15szip 0 dumpsds sds_compressed.hdf)
  ELSE (HDF4_ENABLE_SZIP_SUPPORT)
    ADD_H4_TEST (dumpsds-15 0 dumpsds sds_compressed.hdf)
  ENDIF (HDF4_ENABLE_SZIP_SUPPORT)

  # Test 16 prints SDSs in index order, by default
  ADD_H4_TEST (dumpsds-16 0 dumpsds -h -i 39,36 -n data34,data27 -r 36,37 -i 0,1 -n data6,data9,data4,data3 -r 16,17,15 -i 23,22,21 sds_empty_many.hdf)

  # Test 17 prints SDSs in the order they were specified, when flag -k is present
  # as a request to 'k'eep the specified order
  ADD_H4_TEST (dumpsds-17 0 dumpsds -k -h -i 39,36 -n data34,data27 -r 36,37 -i 0,1 -n data6,data9,data4,data3 -r 16,17,15 -i 23,22,21 sds_empty_many.hdf)

  ADD_H4_TEST (dumprig-1 0 dumprig tdf24.hdf)
  ADD_H4_TEST (dumprig-2 0 dumprig -i 1,2 tdf24.hdf)
  ADD_H4_TEST (dumprig-3 0 dumprig -i 1,3 tdf24.hdf)     # '-i 3' is invalid
  ADD_H4_TEST (dumprig-4 0 dumprig -m 24 tdf24.hdf)
  ADD_H4_TEST (dumprig-5 0 dumprig -r 3,4 tdf24.hdf)
  ADD_H4_TEST (dumprig-6 0 dumprig -r 3,4 -d tdf24.hdf)

  # Test command dumpvd
  ADD_H4_TEST (dumpvd-1 0 dumpvd tvset.hdf)
  ADD_H4_TEST (dumpvd-2 0 dumpvd -i 1,3,5 tvset.hdf)
  ADD_H4_TEST (dumpvd-3 0 dumpvd -r 1238,1239,1251,1252 tvset.hdf)
  ADD_H4_TEST (dumpvd-4 0 dumpvd -n "Multi-Order Vdata" tvset.hdf)
  ADD_H4_TEST (dumpvd-5 0 dumpvd -n "Mixed Vdata,Integer Vdata" tvset.hdf)
  ADD_H4_TEST (dumpvd-6 0 dumpvd -c "Test object,No class specified" tvset.hdf)
  ADD_H4_TEST (dumpvd-7 0 dumpvd -f B tvset.hdf)
  ADD_H4_TEST (dumpvd-8 0 dumpvd -f "STATION_NAME,FLOATS" tvset.hdf)
  ADD_H4_TEST (dumpvd-9 0 dumpvd -f "STATION_NAME,FLOATS" -d tvset.hdf)
  ADD_H4_TEST (dumpvd-10 0 dumpvd tvattr.hdf)

  # Tests 11 and 12 print out the vdatas of classes "SDSVar" and "CoordVar"
  # to test the fix of bugzilla 624 (these are new classes used to distinguish
  # between SDS and coordinate variables)
  ADD_H4_TEST (dumpvd-11 0 dumpvd -c "SDSVar" sds1_dim1_samename.hdf)
  ADD_H4_TEST (dumpvd-12 0 dumpvd -c "CoordVar" sds1_dim1_samename.hdf)

  # Added test #13 to test long vdata's name and class, HDFFR-1267 - BMR 5/15/11
  ADD_H4_TEST (dumpvd-13 0 dumpvd vslongname.hdf)

  # Test command dumpvg
  ADD_H4_TEST (dumpvg-1 0 dumpvg tvset.hdf)
  ADD_H4_TEST (dumpvg-2 0 dumpvg -i 0,1 tvset.hdf)
  ADD_H4_TEST (dumpvg-3 0 dumpvg -r 3 tvset.hdf)
  ADD_H4_TEST (dumpvg-4 0 dumpvg -n "Simple Vgroup" tvset.hdf)
  ADD_H4_TEST (dumpvg-5 0 dumpvg -c "Test object" tvset.hdf)
  ADD_H4_TEST (dumpvg-6 0 dumpvg -i 1,3,5 tdata.hdf)
  ADD_H4_TEST (dumpvg-7 0 dumpvg -r 32,39 tdata.hdf)
  ADD_H4_TEST (dumpvg-8 0 dumpvg -n nsamp,tdata.hdf tdata.hdf)
  ADD_H4_TEST (dumpvg-9 0 dumpvg -c CDF0.0 tdata.hdf)

  # Added option -h to the following test; this option has always 
  # failed; just fixed it - BMR 8/1/00
  ADD_H4_TEST (dumpvg-10 0 dumpvg -h -c Dim0.0,Var0.0 tdata.hdf)

  # this following test is removed since option -d is removed
  #  ADD_H4_TEST (dumpvg-11 0 dumpvg -c Dim0.0,Var0.0 -d tdata.hdf)

  # moved test #12 up to #11, consequently - BMR 7/25/00
  ADD_H4_TEST (dumpvg-11 0 dumpvg tvattr.hdf)

  # Added these two tests for the new feature: vgroup has variable length 
  # name - BMR 10/27/06
  # Note that the dumpvg-13 test searches for an SDS also
  ADD_H4_TEST (dumpvg-12 0 dumpvg VGlongname.hdf)
  ADD_H4_TEST (dumpvg-13 0 dumpvg -n "SD Vgroup - this vgroup has an sds as a member and it is actually meant to test long vgroup name" VGlongname.hdf)

  # Prints contents of file when a dimension has the same name as its SDS 
  ADD_H4_TEST (dumpvg-14 0 dumpvg sds1_dim1_samename.hdf)

  # Prints contents of file when a dimension has the same name as that 
  # of another SDS
  ADD_H4_TEST (dumpvg-15 0 dumpvg sds2_dim1_samename.hdf)
  
  # Test command dumpgr
  ADD_H4_TEST (dumpgr-1 0 dumpgr grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-2 0 dumpgr -i 0,1,3 grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-3 0 dumpgr -i 0 grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-4 0 dumpgr -n Image_array_5 grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-5 0 dumpgr -r 6,2,3 grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-6 0 dumpgr -r 6 -d  grtdfui82.hdf)
  ADD_H4_TEST (dumpgr-7 0 dumpgr -o my.dat grtdfui82.hdf)
  ADD_TEST (
      NAME HDP-clear-my.dat
      COMMAND    ${CMAKE_COMMAND}
      -E remove 
      my.dat
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP-clear-my.dat PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP-clear-my.dat PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDP-clear-my.dat")
  ADD_H4_TEST (dumpgr-8 0 dumpgr -o mybin.dat  -b grtdfui82.hdf)
  ADD_TEST (
      NAME HDP-clear-mybin.dat
      COMMAND    ${CMAKE_COMMAND}
      -E remove 
      mybin.dat
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP-clear-mybin.dat PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (HDP-clear-mybin.dat PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "HDP-clear-mybin.dat")
  ADD_H4_TEST (dumpgr-9 0 dumpgr grtdfui83.hdf)
  ADD_H4_TEST (dumpgr-10 0 dumpgr grtdfui84.hdf)
  ADD_H4_TEST (dumpgr-11 0 dumpgr grtdfui162.hdf)

  # Test 12 shows that file attribute is printed
  ADD_H4_TEST (dumpgr-12 0 dumpgr grtdfi322.hdf)

  # Tests 13, 14, and 15 test option -h, which was not included in any
  # of the previous tests, and the new options -p and -pd, printing palette
  # with or without palette information 
  ADD_H4_TEST (dumpgr-13 0 dumpgr -p Image_with_Palette.hdf)
  ADD_H4_TEST (dumpgr-14 0 dumpgr -h Image_with_Palette.hdf)
  ADD_H4_TEST (dumpgr-15 0 dumpgr -r 2,4 -pd Image_with_Palette.hdf)

  # Test 16: to test new option -s, printing data as stream
  ADD_H4_TEST (dumpgr-16 0 dumpgr -r 6 -d -s grtdfui82.hdf)

  # Test 17: to test new option -m (interlace mode = LINE)
  ADD_H4_TEST (dumpgr-17 0 dumpgr -r 6 -m 1 grtdfui82.hdf)

  # Test 18: to test new option -c (printing attribute data in clean format)
  ADD_H4_TEST (dumpgr-18 0 dumpgr -c grtdfi322.hdf)

  # Test 19: to test new options -g and -l (suppress all attribute data)
  ADD_H4_TEST (dumpgr-19 0 dumpgr -g -l grtdfi322.hdf)
  
ENDIF (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        hdp
    RUNTIME DESTINATION
        ${HDF4_INSTALL_BIN_DIR}/tools
    COMPONENT
        toolsapplications
)
