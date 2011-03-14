cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_TEST C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_HDFSOURCE_DIR}
)
LINK_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

SET (testhdf_SRCS
    ${HDF4_HDF_TEST_SOURCE_DIR}/testhdf.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/an.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/anfile.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/bitio.c  
    ${HDF4_HDF_TEST_SOURCE_DIR}/blocks.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/chunks.c  
    ${HDF4_HDF_TEST_SOURCE_DIR}/comp.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/conv.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/extelt.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/file.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/file1.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/litend.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/macros.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/man.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/mgr.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/nbit.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/rig.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdmms.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdnmms.c 
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdstr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/slab.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tattdatainfo.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdatainfo.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdfr8.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdupimgs.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrattr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrcomp.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tszip.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tutils.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvattr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvset.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvsfpack.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/vers.c
  )

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

#-- Adding test for testhdf
ADD_EXECUTABLE (testhdf ${testhdf_SRCS})
TARGET_NAMING (testhdf ${LIB_TYPE})
TARGET_LINK_LIBRARIES (testhdf ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

#-- Copy all the dat files from the test directory into the source directory
SET (HDF4_REFERENCE_TEST_FILES
    8bit.dat
    bitio.dat
    gr_r24.dat
    greyjpeg.dat
    jpeg.dat
    litend.dat
    nbit.dat
    tmgr.dat
    tvattr.dat
)

FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/test_files/${h4_file}")
   #MESSAGE(STATUS " Copying ${h4_file}")
   ADD_CUSTOM_COMMAND (
       TARGET     testhdf 
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_TEST_SOURCE_DIR}/test_files/${h4_file} ${dest}
   )
ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

# Remove any output file left over from previous test run
ADD_TEST (
    NAME testhdf-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        Block_info.hdf
        ChunkedGR.hdf
        ChunkedGR_DF.hdf
        ChunkedGR_NO.hdf
        ChunkedGR_RL.hdf
        ChunkedGR_SK.hdf
        datainfo_annot.hdf
        datainfo_dfri.hdf
        datainfo_images.hdf
        datainfo_linkblock.hdf
        datainfo_simple.hdf
        gr2.hdf
        gr_chunkcomp.hdf
        gr_comp.hdf
        gr_double_test.hdf
        gr_gzip.hdf
        gr_jpeg.hdf
        gr_r8.hdf
        nntcheck.hdf
        ntcheck.hdf
        RI_8.hdf
        RI_16.hdf
        RI_32.hdf
        RI_fl32.hdf
        RI_fl64.hdf
        RIchunkedziped.hdf
        s1w.hdf
        s3w.hdf
        s4w.hdf
        sdstrings.hdf
        swf32.hdf
        swf64.hdf
        swi8.hdf
        swi16.hdf
        swi32.hdf
        swin.hdf
        swui8.hdf
        swui16.hdf
        swui32.hdf
        swuin.hdf
        t.hdf
        t1.hdf
        t2.hdf
        t3.hdf
        t4.hdf
        tbitio.hdf
        tblocks.hdf
        tchunks.hdf
        tcomp.hdf
        tdf24.hdf
        tdfan.hdf
        temp.hdf
        thf.hdf
        tjpeg.hdf
        tlongnames.hdf
        tman.hdf
        tmgr.hdf
        tmgratt.hdf
        tmgrchk.hdf
        tnbit.hdf
        tref.hdf
        tuservds.hdf
        tuservgs.hdf
        tvattr.hdf
        tvpack.hdf
        tvsempty.hdf
        tvset.hdf
        tvsetext.hdf
        tx.hdf
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf-clearall-objects PROPERTIES DEPENDS ${last_test})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf-clearall-objects")

ADD_TEST (
    NAME testhdf_thf0-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        thf0.hdf
        thf1.hdf
        thf2.hdf
        thf3.hdf
        thf4.hdf
        thf5.hdf
        thf6.hdf
        thf7.hdf
        thf8.hdf
        thf9.hdf
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf_thf0-clearall-objects PROPERTIES DEPENDS ${last_test})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf_thf0-clearall-objects")

SET (thf_decade
    1 2 3 4 5 6 7 8 9 10
    11 12 13 14 15 16 17 18 19 20
    21 22 23 24 25 26 27 28 29 30
    31 32 33 34 35 36 37 38 39 40
    41 42 43 44 45 46 47 48 49 50
    51 52 53 54 55 56 57 58 59 60
)
FOREACH (decade ${thf_decade}) 
  ADD_TEST (
      NAME testhdf_thf${decade}-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          thf${decade}0.hdf
          thf${decade}1.hdf
          thf${decade}2.hdf
          thf${decade}3.hdf
          thf${decade}4.hdf
          thf${decade}5.hdf
          thf${decade}6.hdf
          thf${decade}7.hdf
          thf${decade}8.hdf
          thf${decade}9.hdf
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (testhdf_thf${decade}-clearall-objects PROPERTIES DEPENDS ${last_test})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "testhdf_thf${decade}-clearall-objects")
ENDFOREACH (decade ${thf_decade}) 

ADD_TEST (NAME testhdf COMMAND $<TARGET_FILE:testhdf>)
SET (passRegex "All tests were successful")
SET_PROPERTY (TEST testhdf PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (testhdf PROPERTIES DEPENDS ${last_test})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "testhdf")

#-- Adding test for buffer
IF (NOT WIN32)
  ADD_EXECUTABLE (buffer ${HDF4_HDF_TEST_SOURCE_DIR}/buffer.c)
  TARGET_NAMING (buffer ${LIB_TYPE})
  TARGET_LINK_LIBRARIES (buffer ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ADD_TEST (NAME buffer COMMAND $<TARGET_FILE:buffer>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (buffer PROPERTIES DEPENDS ${last_test})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "buffer")
ENDIF (NOT WIN32)
  
FILE (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)


