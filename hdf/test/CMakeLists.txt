PROJECT (HDF4_HDF_TEST C CXX Fortran)

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

IF(WIN32)
  ADD_DEFINITIONS(-D_HDFLIB_ -DDOS_FS)
ENDIF(WIN32)

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
  ${HDF4_HDF_TEST_SOURCE_DIR}/tdupimgs.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrattr.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrcomp.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tszip.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tutils.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tvattr.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tvset.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tvsfpack.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/vers.c
  )

#-- Adding test for testhdf
IF(WIN32)
  ADD_EXECUTABLE(testhdf ${testhdf_SRCS})
ELSE(WIN32)
  ADD_EXECUTABLE(testhdf ${testhdf_SRCS}
  ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
  ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c)
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(testhdf ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )
ADD_TEST(testhdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/testhdf)

IF (BUILD_SHARED_LIBS)
  IF(WIN32)
    add_definitions(-D_HDFDLL_ )
    ADD_EXECUTABLE(dlltesthdf ${testhdf_SRCS})
  ELSE(WIN32)
    ADD_EXECUTABLE(dlltesthdf ${testhdf_SRCS}
    ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c)
  ENDIF(WIN32)
  TARGET_LINK_LIBRARIES(dlltesthdf ${HDF4_SRC_LIB_NAME} ${LINK_LIBS} )
  ADD_TEST(dlltesthdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dlltesthdf)
  IF(MSVC)
    SET_TARGET_PROPERTIES(dlltesthdf PROPERTIES COMPILER_FLAGS "${SHARED_C_FLAGS_RELEASE}")
  ENDIF(MSVC)
ENDIF (BUILD_SHARED_LIBS)

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
  SET(FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

SET (TEST_MODULE_LIST ${HDF4_SRC_FORTRAN_LIB_NAME}-static
  H4DF24F H4DFAN H4DFP H4DFR8 
  H4DFSD H4DFUFP2I H4HFILE H4MFGR 
  H4VATTR H4FG
  ${HDF4_SRC_FCSTUB_LIB_NAME}-static
)
#-- Adding test for fortest
IF(NOT WIN32)
 ADD_EXECUTABLE(fortest ${HDF4_HDF_TEST_SOURCE_DIR}/fortest.c)
 TARGET_LINK_LIBRARIES(fortest ${HDF4_SRC_LIB_NAME}-static ${HDF4_MF_LIB_NAME}-static )
 ADD_TEST(fortest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortest)
ENDIF(NOT WIN32)

#-----------------------------------------------------------------------------
#-- Adding test for fortestF
IF(WIN32)
  ADD_LIBRARY(${HDF4_SRC_TEST_LIB_NAME}-static STATIC ${HDF4_HDF_TEST_SOURCE_DIR}/forsupf.c )
  SET_TARGET_PROPERTIES(${HDF4_SRC_TEST_LIB_NAME}-static PROPERTIES OUTPUT_NAME "${HDF4_SRC_TEST_LIB_NAME}")
  SET_TARGET_PROPERTIES(${HDF4_SRC_TEST_LIB_NAME}-static PROPERTIES PREFIX "lib")

  ADD_EXECUTABLE(fortestF ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/fortestFp.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/forsupffp.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/manpf.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/mgrf.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/tvattrf.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/tvsetf.f
 			   ${HDF4_HDF_TEST_SOURCE_DIR}/fortest.inc
 )
  TARGET_LINK_LIBRARIES(fortestF ${HDF4_SRC_TEST_LIB_NAME}-static ${TEST_MODULE_LIST} ${LINK_LIBS} ${HDF4_MF_LIB_NAME}-static )
ELSE(WIN32)
  ADD_EXECUTABLE(fortestF ${HDF4_HDF_TEST_SOURCE_DIR}/fortestF.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/forsupff.f
               ${HDF4_HDF_TEST_SOURCE_DIR}/forsupf.c
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
  TARGET_LINK_LIBRARIES(fortestF  ${TEST_MODULE_LIST} ${LINK_LIBS} ${HDF4_MF_LIB_NAME}-static )
ENDIF(WIN32)

ADD_TEST(fortestF ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fortestF)
  
ENDIF (HDF4_BUILD_FORTRAN)
  
IF(BUILD_TESTING)
	FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)
ENDIF(BUILD_TESTING)


