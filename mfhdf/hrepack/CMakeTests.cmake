
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

#-- Copy all the dat files from the test directory into the source directory
  set (HDF4_REPACK_TEST_FILES
      hrepack_help.txt
      hrepack_check_help.txt
      image24pixel.txt
      image24plane.txt
      image8.txt
      info.txt
  )

  foreach (h4_file ${HDF4_REPACK_TEST_FILES})
    set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
    #message (STATUS " Copying ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
    add_custom_command (
        TARGET     hrepack_check
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/${h4_file} ${dest}
    )
  endforeach ()

#-- Adding test for test_hrepack for generating testfiles
  add_executable (test_hrepack ${HDF4_MFHDF_HREPACK_SOURCE_DIR}/hrepacktst.c)
  TARGET_C_PROPERTIES (test_hrepack STATIC " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (test_hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  else ()
    target_link_libraries (test_hrepack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  endif ()
  TARGET_NAMING (test_hrepack STATIC)

  macro (ADD_H4_TEST testname testtype testfile)
    if ("${testtype}" STREQUAL "SKIP")
      if (NOT HDF4_ENABLE_USING_MEMCHECKER)
        add_test (
            NAME HREPACK-${testname}-SKIPPED
            COMMAND ${CMAKE_COMMAND} -E echo "SKIP  -v -i ${PROJECT_BINARY_DIR}/${testfile} -o ${PROJECT_BINARY_DIR}/out-${testname}.${testfile} ${ARGN}"
        )
      endif (NOT HDF4_ENABLE_USING_MEMCHECKER)
    else ()
      add_test (
          NAME HREPACK-${testname}-clearall-objects
          COMMAND    ${CMAKE_COMMAND}
              -E remove
              out-${testname}.${testfile}
      )
      set_tests_properties (HREPACK-${testname}-clearall-objects PROPERTIES DEPENDS HREPACK-test_hrepack LABELS ${PROJECT_NAME})
      add_test (
          NAME HREPACK-${testname}
          COMMAND $<TARGET_FILE:hrepack> -v -i ${PROJECT_BINARY_DIR}/${testfile} -o ${PROJECT_BINARY_DIR}/out-${testname}.${testfile} ${ARGN}
      )
      set_tests_properties (HREPACK-${testname} PROPERTIES DEPENDS HREPACK-${testname}-clearall-objects LABELS ${PROJECT_NAME})
      add_test (
          NAME HREPACK-${testname}_DFF
          COMMAND $<TARGET_FILE:hdiff> ${PROJECT_BINARY_DIR}/${testfile} ${PROJECT_BINARY_DIR}/out-${testname}.${testfile}
      )
      set_tests_properties (HREPACK-${testname}_DFF PROPERTIES DEPENDS HREPACK-${testname} LABELS ${PROJECT_NAME})
    endif ()
  endmacro ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
  # Remove any output file left over from previous test run
  add_test (
      NAME HREPACK-hrepack-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove
          hrepack_help.out
          hrepack_check_help.out
          hrepacktst1.hdf
          hrepacktst2.hdf
          hrepacktst3.hdf
  )

  if (NOT HDF4_ENABLE_USING_MEMCHECKER)
    add_test (
        NAME HREPACK-hrepack_check
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hrepack_check>"
            -D "TEST_ARGS:STRING=${ARGN}"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=hrepack_check_help.out"
            -D "TEST_EXPECT=1"
            -D "TEST_REFERENCE=hrepack_check_help.txt"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
    set_tests_properties (HREPACK-hrepack_check PROPERTIES DEPENDS HREPACK-hrepack-clearall-objects LABELS ${PROJECT_NAME})
  endif ()

  if (NOT HDF4_ENABLE_USING_MEMCHECKER)
    add_test (
        NAME HREPACK-hrepack_help
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hrepack>"
            -D "TEST_ARGS:STRING=${ARGN}"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=hrepack_help.out"
            -D "TEST_EXPECT=0"
            -D "TEST_REFERENCE=hrepack_help.txt"
            -P "${HDF_RESOURCES_EXT_DIR}/runTest.cmake"
    )
    set_tests_properties (HREPACK-hrepack_help PROPERTIES DEPENDS HREPACK-hrepack-clearall-objects LABELS ${PROJECT_NAME})
  endif ()

  add_test (NAME HREPACK-test_hrepack COMMAND $<TARGET_FILE:test_hrepack>)
  set_tests_properties (HREPACK-test_hrepack PROPERTIES DEPENDS HREPACK-hrepack-clearall-objects LABELS ${PROJECT_NAME})

  set (HREPACK_FILE1 hrepacktst1.hdf)
  set (HREPACK_FILE2 hrepacktst2.hdf)
  set (HREPACK_FILE3 hrepacktst3.hdf)
   #-------------------------------------------------------------------------
   # test1:
   # HUFF
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST (HUFF "TEST" ${HREPACK_FILE1} -t "dset7:HUFF 1" -c dset7:10x8x6)

#    if ( sds_verifiy_comp("dset7",COMP_CODE_SKPHUFF, 1) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset7",HDF_CHUNK|HDF_COMP,3,in_chunk_lengths) == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test2:
   # RLE
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(RLE "TEST" ${HREPACK_FILE1} -t dset4:RLE -c dset4:10x8)

#    if ( sds_verifiy_comp("dset4",COMP_CODE_RLE, 0) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#        goto out;

   #-------------------------------------------------------------------------
   # test3:
   # SDS SELECTED with GZIP, chunking SELECTED
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(SDSGZIP_CHUNK "TEST" ${HREPACK_FILE1} -t "dset4:GZIP 6" -c dset4:10x8)

#    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 6) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test4:
   # SDS SELECTED with SZIP, chunking SELECTED
   #-------------------------------------------------------------------------
   #
   if (H4_HAVE_SZIP_ENCODER)
     ADD_H4_TEST(SDSSZIP_CHUNK "TEST" ${HREPACK_FILE1} -t "dset4:SZIP 8,EC" -c dset4:10x8)
   else ()
     ADD_H4_TEST(SDSSZIP_CHUNK "SKIP" ${HREPACK_FILE1} -c dset4:10x8)
   endif ()
#if defined (H4_HAVE_LIBSZ)
#    if (SZ_encoder_enabled())
#    {
#
#        if ( sds_verifiy_comp("dset4",COMP_CODE_SZIP, 0) == -1)
#            goto out;
#        if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#            goto out;

   #-------------------------------------------------------------------------
   # test4:
   # SDS SELECTED with NONE, chunking SELECTED NONE
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(SDSNONE_CHUNKNONE "TEST" ${HREPACK_FILE1} -t dset_chunk_comp:NONE -t dset_chunk:NONE -c dset_chunk_comp:NONE -c dset_chunk:NONE)

#    if ( sds_verifiy_comp("dset_chunk_comp",COMP_CODE_NONE, 0) == -1)
#        goto out;
#    if ( sds_verifiy_comp("dset_chunk",COMP_CODE_NONE, 0) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset_chunk_comp",HDF_NONE,0,0) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset_chunk",HDF_NONE,0,0) == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test5:
   # SDS SELECTED with all types, chunking SELECTED
   #-------------------------------------------------------------------------
   #
   if (H4_HAVE_SZIP_ENCODER)
     ADD_H4_TEST(SDS_CHUNK "TEST" ${HREPACK_FILE1} -t "dset4:GZIP 9" -t dset5:RLE -t "dset6:HUFF 2" -t "dset7:SZIP 8,EC" -c dset4:10x8 -c dset5:10x8 -c dset6:10x8)
   else ()
     ADD_H4_TEST(SDS_CHUNK "TEST" ${HREPACK_FILE1} -t "dset4:GZIP 9" -t dset5:RLE -t "dset6:HUFF 2" -c dset4:10x8 -c dset5:10x8 -c dset6:10x8)
   endif ()

#    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1)
#        goto out;
#    if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1)
#        goto out;
#    if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1)
#        goto out;
#if defined (H4_HAVE_LIBSZ)
#    if (SZ_encoder_enabled()) {
#        if ( sds_verifiy_comp("dset7",COMP_CODE_SZIP, 0) == -1)
#            goto out;
#    }
#endif
#    if ( sds_verifiy_chunk("dset4",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset5",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset6",HDF_CHUNK|HDF_COMP,2,in_chunk_lengths) == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test6:
   # SDS SELECTED with all types, no chunking
   #-------------------------------------------------------------------------
   #
   if (H4_HAVE_SZIP_ENCODER)
     ADD_H4_TEST(SEL_NOCHUNK "TEST" ${HREPACK_FILE1} -t "dset4:GZIP 9" -t dset5:RLE -t "dset6:HUFF 2" -t "dset7:SZIP 4,EC")
   else ()
     ADD_H4_TEST(SEL_NOCHUNK "TEST" ${HREPACK_FILE1} -t "dset4:GZIP 9" -t dset5:RLE -t "dset6:HUFF 2")
   endif ()

#    if ( sds_verifiy_comp("dset4",COMP_CODE_DEFLATE, 9) == -1)
#        goto out;
#    if ( sds_verifiy_comp("dset5",COMP_CODE_RLE, 0) == -1)
#        goto out;
#    if ( sds_verifiy_comp("dset6",COMP_CODE_SKPHUFF, 2) == -1)
#        goto out;
#if defined (H4_HAVE_LIBSZ)
#    if (SZ_encoder_enabled()) {
#        if ( sds_verifiy_comp("dset7",COMP_CODE_SZIP, 0) == -1)
#            goto out;
#    }
#endif


   #-------------------------------------------------------------------------
   # test7:
   # compressing SDS ALL, chunking SELECTED NONE
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(COMPALL_CHUNKNONE "TEST" ${HREPACK_FILE1} -t "*:GZIP 1" -c dset_chunk_comp:NONE -c dset_chunk:NONE)

#    if ( sds_verifiy_comp_all(COMP_CODE_DEFLATE, 1) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset_chunk_comp",HDF_NONE,0,0) == -1)
#        goto out;
#    if ( sds_verifiy_chunk("dset_chunk",HDF_NONE,0,0) == -1)
#        goto out;

   #-------------------------------------------------------------------------
   # test8:
   # no compressing, chunking ALL
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(NOCOMP_CHUNKALL "TEST" ${HREPACK_FILE1} -c *:10x8)

#    if ( sds_verifiy_chunk_all(HDF_CHUNK,2,in_chunk_lengths,"dset7") == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test9:
   # compressing SDS ALL with GZIP
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(GZIP "TEST" ${HREPACK_FILE1} -t "*:GZIP 1")

#    if ( sds_verifiy_comp_all(COMP_CODE_DEFLATE, 1) == -1)
#        goto out;


   #-------------------------------------------------------------------------
   # test10:
   # repack a big file using hyperslab reading/writing
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(HYPERSLAB "TEST" ${HREPACK_FILE2})

   #-------------------------------------------------------------------------
   # test11:
   # repack a file with vgroups
   #-------------------------------------------------------------------------
   #
    ADD_H4_TEST(VGROUP "TEST" ${HREPACK_FILE3})

#    if (vg_verifygrpdep(HREPACK_FILE3,HREPACK_FILE3_OUT) != 0 )
#        goto out;
