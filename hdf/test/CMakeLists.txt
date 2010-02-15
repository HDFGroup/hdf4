PROJECT (HDF4_HDF_TEST)

#-----------------------------------------------------------------------------
# Setup output Directories 
#-----------------------------------------------------------------------------
IF(NOT HDF4_EXTERNALLY_CONFIGURED)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
ENDIF(NOT HDF4_EXTERNALLY_CONFIGURED)

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
  ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
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

#-- Adding test for testhdf
ADD_EXECUTABLE(testhdf ${testhdf_SRCS})
TARGET_LINK_LIBRARIES(testhdf ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ADD_TEST(testhdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testhdf)

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

FOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )
   SET (dest "${PROJECT_BINARY_DIR}/test_files/${h4_file}")
   #MESSAGE(STATUS " Copying ${h4_file}")
   ADD_CUSTOM_COMMAND (
     TARGET     testhdf 
     POST_BUILD
     COMMAND    ${CMAKE_COMMAND}
     ARGS       -E copy_if_different ${HDF4_HDF_TEST_SOURCE_DIR}/test_files/${h4_file} ${dest}
     )
     
ENDFOREACH ( h4_file ${HDF4_REFERENCE_TEST_FILES} )

#-- Adding test for buffer
IF(NOT WIN32)
  ADD_EXECUTABLE(buffer ${HDF4_HDF_TEST_SOURCE_DIR}/buffer.c)
  TARGET_LINK_LIBRARIES(buffer ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
  ADD_TEST(buffer ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/buffer)
ENDIF(NOT WIN32)

IF (HDF4_BUILD_FORTRAN)

#-- Adding test for fortest
 ADD_EXECUTABLE(fortest ${HDF4_HDF_TEST_SOURCE_DIR}/fortest.c)
 TARGET_LINK_LIBRARIES(fortest ${HDF4_SRC_LIB_NAME}-static )
 ADD_TEST(fortest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortest)

SET (fortestF_FORTRAN_SRCS
  ${HDF4_HDF_TEST_SOURCE_DIR}/forsupff.f
#  ${HDF4_HDF_TEST_SOURCE_DIR}/forsupf.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/fortestF.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/manf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/mgrf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/slabwf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/t24f.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tanf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tanfilef.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tpf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tr8f.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tsdmmsf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tsdnmmsf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tsdnntf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tsdntf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tsdstrf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tstubsf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tvattrf.f
  ${HDF4_HDF_TEST_SOURCE_DIR}/tvsetf.f
  )

#SET_SOURCE_FILES_PROPERTIES( ${HDF4_HDF_SRC_FORT_PS_CSRCS} PROPERTIES LANGUAGE C) 
SET_SOURCE_FILES_PROPERTIES( ${fortestF_FORTRAN_SRCS} PROPERTIES LANGUAGE FORTRAN) 

#-- Adding test for fortestF
 ADD_EXECUTABLE(fortestF ${fortestF_FORTRAN_SRCS})
 TARGET_LINK_LIBRARIES(fortestF ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static )
 ADD_TEST(fortestF ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortestF)
  
ENDIF (HDF4_BUILD_FORTRAN)
  
IF(BUILD_TESTING)
	FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)
ENDIF(BUILD_TESTING)


