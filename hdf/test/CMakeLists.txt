cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_TEST C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${CMAKE_Fortran_MODULE_DIRECTORY} ${HDF4_HDF_BINARY_DIR} ${HDF4_HDF_SOURCE_DIR}/src)
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
IF (WIN32)
  ADD_EXECUTABLE (testhdf ${testhdf_SRCS})
ELSE (WIN32)
  ADD_EXECUTABLE (testhdf ${testhdf_SRCS}
      ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
      ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c
  )
ENDIF (WIN32)
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

IF (HDF4_BUILD_FORTRAN)
  ENABLE_LANGUAGE (Fortran)
  #-----------------------------------------------------------------------------
  # Detect name mangling convention used between Fortran and C
  #-----------------------------------------------------------------------------
  INCLUDE (FortranCInterface)
  FortranCInterface_HEADER (
      ${HDF4_HDF_BINARY_DIR}/F77Mangle.h
      MACRO_NAMESPACE "H4_F77_"
      SYMBOL_NAMESPACE "H4_F77_"
      SYMBOLS mysub mymod:my_sub
  )

  FILE (STRINGS ${HDF4_HDF_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL\\(.*,.*\\) +(.*)")
  STRING (REGEX MATCH "H4_F77_GLOBAL\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
  SET (H4_FC_FUNC "H4_F77_FUNC(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

  FILE (STRINGS ${HDF4_HDF_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)")
  STRING (REGEX MATCH "H4_FC_GLOBAL_\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
  SET (H4_FC_FUNC_ "H4_F77_FUNC_(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

  #-----------------------------------------------------------------------------
  # Make sure generated files and modules are picked up correctly
  #-----------------------------------------------------------------------------
  INCLUDE_DIRECTORIES ( 
      ${CMAKE_Fortran_MODULE_DIRECTORY}
      ${HDF4_HDF_BINARY_DIR}
  )

  #-----------------------------------------------------------------------------
  # Add debug information (intel Fortran : JB)
  #-----------------------------------------------------------------------------
  IF (CMAKE_Fortran_COMPILER MATCHES ifort)
    IF (WIN32)
      SET (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE "flags" STRING FORCE)
      SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE "flags" STRING FORCE)
    ENDIF (WIN32)
  ENDIF (CMAKE_Fortran_COMPILER MATCHES ifort)
  
  #-- Adding test for fortest
  IF (NOT WIN32)
    ADD_EXECUTABLE (fortest ${HDF4_HDF_TEST_SOURCE_DIR}/fortest.c)
    TARGET_NAMING (fortest)
    TARGET_LINK_LIBRARIES (fortest ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET})
    ADD_TEST (NAME fortest COMMAND $<TARGET_FILE:fortest>)
    IF (WIN32)
      IF (MSVC)
        #TARGET_LINK_LIBRARIES (fortest "ws2_32.lib")
        IF (BUILD_SHARED_LIBS)
          SET_TARGET_PROPERTIES (fortest
              PROPERTIES
                  COMPILE_FLAGS "/dll"
                  LINK_FLAGS "/SUBSYSTEM:CONSOLE"
          )
        ELSE (BUILD_SHARED_LIBS)
          IF (MSVC)
            SET_TARGET_PROPERTIES (fortest PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
          ENDIF (MSVC)
        ENDIF (BUILD_SHARED_LIBS)
      ENDIF (MSVC)
    ENDIF (WIN32)
    SET_TARGET_PROPERTIES (fortest PROPERTIES LINKER_LANGUAGE Fortran)
  ENDIF (NOT WIN32)

  #-----------------------------------------------------------------------------
  #-- Adding test for fortestF
  IF (WIN32)
    ADD_LIBRARY (fortestF_cstub ${LIB_TYPE} ${HDF4_HDF_TEST_SOURCE_DIR}/forsupf.c)
    H4_SET_LIB_OPTIONS (fortestF_cstub "fortestF" ${LIB_TYPE})
 
    SET (FORTEST_FSRCS 
        ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/fortestFp.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/forsupffp.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/manpf.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/mgrf.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/tvattrf.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/tvsetf.f
        ${HDF4_HDF_TEST_SOURCE_DIR}/fortest.inc
    )
  ELSE (WIN32)
    SET (FORTEST_FSRCS 
        ${HDF4_HDF_TEST_SOURCE_DIR}/fortestF.f
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
  ENDIF (WIN32)

  ADD_EXECUTABLE (fortestF ${FORTEST_FSRCS} )
  TARGET_NAMING (fortestF)
  IF (WIN32)
    IF (MSVC)
      #TARGET_LINK_LIBRARIES (fortestF "ws2_32.lib")
      IF (BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (fortestF
            PROPERTIES
                COMPILE_FLAGS "/dll"
                LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        )
      ELSE (BUILD_SHARED_LIBS)
        IF (MSVC)
          SET_TARGET_PROPERTIES (fortestF PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
        ENDIF (MSVC)
      ENDIF (BUILD_SHARED_LIBS)
    ENDIF (MSVC)
    TARGET_LINK_LIBRARIES (fortestF fortestF_cstub ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} )
  ELSE (WIN32)
    TARGET_LINK_LIBRARIES (fortestF ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} )
  ENDIF (WIN32)
  SET_TARGET_PROPERTIES (fortestF PROPERTIES LINKER_LANGUAGE Fortran)

  ADD_TEST (NAME fortestF COMMAND $<TARGET_FILE:fortestF>)
  
ENDIF (HDF4_BUILD_FORTRAN)
  
IF (BUILD_TESTING)
	FILE (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)
ENDIF (BUILD_TESTING)


