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

SET (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

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

IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)

#-- Adding test for testhdf
ADD_EXECUTABLE (testhdf ${testhdf_SRCS}
    ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c
)
TARGET_NAMING (testhdf)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (testhdf PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (testhdf ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ADD_TEST (NAME testhdf COMMAND $<TARGET_FILE:testhdf>)

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

#-- Adding test for buffer
IF (NOT WIN32)
  ADD_EXECUTABLE (buffer ${HDF4_HDF_TEST_SOURCE_DIR}/buffer.c)
  TARGET_NAMING (buffer)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (buffer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)
  TARGET_LINK_LIBRARIES (buffer ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ADD_TEST (NAME buffer COMMAND $<TARGET_FILE:buffer>)
ENDIF (NOT WIN32)
  
FILE (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)


