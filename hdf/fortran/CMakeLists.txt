cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_FORTRAN C CXX Fortran)

ENABLE_LANGUAGE (Fortran)

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
  
SET (HDF4_HDF_SRC_CSTUB_FSRCS
    ${HDF4_HDFSOURCE_DIR}/dfanf.c
    ${HDF4_HDFSOURCE_DIR}/dff.c
    ${HDF4_HDFSOURCE_DIR}/dfpf.c
    ${HDF4_HDFSOURCE_DIR}/dfr8f.c
    ${HDF4_HDFSOURCE_DIR}/dfsdf.c
    ${HDF4_HDFSOURCE_DIR}/dfufp2i.c
    ${HDF4_HDFSOURCE_DIR}/dfutilf.c
    ${HDF4_HDFSOURCE_DIR}/df24f.c
    ${HDF4_HDFSOURCE_DIR}/dfufp2if.c
    ${HDF4_HDFSOURCE_DIR}/herrf.c
    ${HDF4_HDFSOURCE_DIR}/hfilef.c
    ${HDF4_HDFSOURCE_DIR}/mfanf.c
    ${HDF4_HDFSOURCE_DIR}/mfgrf.c
    ${HDF4_HDFSOURCE_DIR}/vattrf.c
    ${HDF4_HDFSOURCE_DIR}/vgf.c
)

SET (HDF4_HDF_SRC_FHDRS
    ${HDF4_HDFSOURCE_DIR}/dffunc.inc
    ${HDF4_HDFSOURCE_DIR}/hdf.inc
)

SET_SOURCE_FILES_PROPERTIES (${HDF4_HDF_SRC_CSTUB_FSRCS} PROPERTIES LANGUAGE C) 

SET (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_SRC_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_SRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h ${HDF4_BINARY_DIR}/h4config.h)
SET_TARGET_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FCSTUB_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_NAME} ${LIB_TYPE})

SET (HDF4_F_FORTRAN_SRCS 
    ${HDF4_HDFSOURCE_DIR}/df24ff.f  
    ${HDF4_HDFSOURCE_DIR}/dfanff.f 
    ${HDF4_HDFSOURCE_DIR}/dfpff.f 
    ${HDF4_HDFSOURCE_DIR}/dfr8ff.f
    ${HDF4_HDFSOURCE_DIR}/dfsdff.f 
    ${HDF4_HDFSOURCE_DIR}/dfufp2iff.f
    ${HDF4_HDFSOURCE_DIR}/dfff.f 
    ${HDF4_HDFSOURCE_DIR}/hfileff.f 
    ${HDF4_HDFSOURCE_DIR}/mfgrff.f
    ${HDF4_HDFSOURCE_DIR}/vattrff.f 
    ${HDF4_HDFSOURCE_DIR}/vgff.f 
)
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_SRC_FORTRAN_LIB_TARGET} ${LIB_TYPE} ${HDF4_F_FORTRAN_SRCS})
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET}
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE /DLL"
      )
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
ENDIF (WIN32)
SET_TARGET_PROPERTIES (${HDF4_SRC_FORTRAN_LIB_TARGET} PROPERTIES LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FORTRAN_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_NAME} ${LIB_TYPE})
  
IF (BUILD_TESTING)

  SET (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

  #-----------------------------------------------------------------------------
  # Add test fortran stub library
  #-----------------------------------------------------------------------------
  ADD_LIBRARY (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  SET_TARGET_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  IF (WIN32)
    ADD_DEFINITIONS (-DDOS_FS)
  ENDIF (WIN32)
  TARGET_LINK_LIBRARIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET}  ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} ${LIB_TYPE})
  
  #-- Adding test for fortest
  ADD_EXECUTABLE (fortest ${HDF4_HDF_TESTSOURCE_DIR}/fortest.c)
  TARGET_NAMING (fortest)
  TARGET_LINK_LIBRARIES (fortest ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET})
  IF (WIN32)
    IF (MSVC)
      TARGET_LINK_LIBRARIES (fortest "ws2_32.lib")
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
  SET_TARGET_PROPERTIES (fortest PROPERTIES LINKER_LANGUAGE C)

  ADD_TEST (NAME fortest COMMAND $<TARGET_FILE:fortest>)

  #-----------------------------------------------------------------------------
  #-- Adding test for fortestF
  SET (FORTEST_FSRCS 
      ${HDF4_HDF_TESTSOURCE_DIR}/fortestF.f
      ${HDF4_HDF_TESTSOURCE_DIR}/forsupff.f
      ${HDF4_HDF_TESTSOURCE_DIR}/manf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/mgrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/slabwf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/t24f.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tanf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tanfilef.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tpf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tr8f.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdmmsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdnmmsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdnntf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdntf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tsdstrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tstubsf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tvattrf.f
      ${HDF4_HDF_TESTSOURCE_DIR}/tvsetf.f
  )

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
  ENDIF (WIN32)
  TARGET_LINK_LIBRARIES (fortestF ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} )
  SET_TARGET_PROPERTIES (fortestF PROPERTIES LINKER_LANGUAGE Fortran)

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
    #MESSAGE (STATUS " Copying ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} to ${PROJECT_BINARY_DIR}/test_files/")
    ADD_CUSTOM_COMMAND (
        TARGET     fortestF 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file} ${dest}
    )
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

  ADD_TEST (NAME fortestF COMMAND $<TARGET_FILE:fortestF>)
  SET (passRegex "All Fortran Interface Tests Passed")
  SET_PROPERTY (TEST fortestF PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
  
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (HDF4_EXPORTED_TARGETS)
  INSTALL (
      TARGETS 
          ${HDF4_SRC_FCSTUB_LIB_TARGET}
          ${HDF4_SRC_FORTRAN_LIB_TARGET}
      EXPORT
          ${HDF4_EXPORTED_TARGETS}
      LIBRARY DESTINATION lib COMPONENT fortlibraries 
      ARCHIVE DESTINATION lib COMPONENT fortlibraries
      RUNTIME DESTINATION bin COMPONENT fortlibraries
  )
ENDIF (HDF4_EXPORTED_TARGETS)
    