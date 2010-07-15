cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_FORTRAN C CXX Fortran)

#-----------------------------------------------------------------------------
# Detect name mangling convention used between Fortran and C
#-----------------------------------------------------------------------------
INCLUDE (FortranCInterface)
FortranCInterface_HEADER (
    ${HDF4_MFHDF_FORTRAN_BINARY_DIR}/FCMangle.h
    MACRO_NAMESPACE "H4_F77_"
    SYMBOL_NAMESPACE "H4_F77_"
    SYMBOLS mysub mymod:my_sub
)

FILE (STRINGS ${HDF4_MFHDF_FORTRAN_BINARY_DIR}/FCMangle.h CONTENTS REGEX "H4_F77_GLOBAL\\(.*,.*\\) +(.*)")
STRING (REGEX MATCH "H4_F77_GLOBAL\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

FILE (STRINGS ${HDF4_MFHDF_FORTRAN_BINARY_DIR}/FCMangle.h CONTENTS REGEX "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)")
STRING (REGEX MATCH "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

#-----------------------------------------------------------------------------
# The provided CMake Fortran macros don't provide a general check function
# so this one is used for a sizeof test.
#-----------------------------------------------------------------------------
MACRO (CHECK_FORTRAN_FEATURE FUNCTION CODE VARIABLE)
  IF (NOT DEFINED ${VARIABLE})
    MESSAGE (STATUS "Testing Fortran ${FUNCTION}")
    IF (CMAKE_REQUIRED_LIBRARIES)
      SET (CHECK_FUNCTION_EXISTS_ADD_LIBRARIES
          "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    ELSE (CMAKE_REQUIRED_LIBRARIES)
      SET (CHECK_FUNCTION_EXISTS_ADD_LIBRARIES)
    ENDIF (CMAKE_REQUIRED_LIBRARIES)
    FILE (WRITE
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testFortranCompiler.f
        "${CODE}"
    )
    TRY_COMPILE (${VARIABLE}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testFortranCompiler.f
        CMAKE_FLAGS "${CHECK_FUNCTION_EXISTS_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )

#    MESSAGE ( "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")
#    MESSAGE ( "Test result ${OUTPUT}")
#    MESSAGE ( "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")

    IF (${VARIABLE})
      SET (${VARIABLE} 1 CACHE INTERNAL "Have Fortran function ${FUNCTION}")
      MESSAGE (STATUS "Testing Fortran ${FUNCTION} - OK")
      FILE (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
          "Determining if the Fortran ${FUNCTION} exists passed with the following output:\n"
          "${OUTPUT}\n\n"
      )
    ELSE (${VARIABLE})
      MESSAGE (STATUS "Testing Fortran ${FUNCTION} - Fail")
      SET (${VARIABLE} "" CACHE INTERNAL "Have Fortran function ${FUNCTION}")
      FILE (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
          "Determining if the Fortran ${FUNCTION} exists failed with the following output:\n"
          "${OUTPUT}\n\n")
    ENDIF (${VARIABLE})
  ENDIF (NOT DEFINED ${VARIABLE})
ENDMACRO (CHECK_FORTRAN_FEATURE)

#-----------------------------------------------------------------------------
# Configure Checks which require Fortran compilation must go in here
# not in the main ConfigureChecks.cmake files, because if the user has
# no Fortran compiler, problems arise.
#
# Be careful with leading spaces here, do not remove them.
#-----------------------------------------------------------------------------
CHECK_FORTRAN_FEATURE(sizeof
  "
       PROGRAM main
       i = sizeof(x)
       END PROGRAM
  "
  FORTRAN_HAVE_SIZEOF
)

CHECK_FORTRAN_FEATURE(RealIsNotDouble
  "
       MODULE type_mod
         INTERFACE h4t
           MODULE PROCEDURE h4t_real
           MODULE PROCEDURE h4t_dble
         END INTERFACE
       CONTAINS
         SUBROUTINE h4t_real(r)
           REAL :: r
         END SUBROUTINE h4t_real
         SUBROUTINE h4t_dble(d)
           DOUBLE PRECISION :: d
         END SUBROUTINE h4t_dble
       END MODULE type_mod
       PROGRAM main
         USE type_mod
         REAL :: r
         DOUBLE PRECISION :: d
         CALL h4t(r)
         CALL h4t(d)
       END PROGRAM main
  "
  FORTRAN_DEFAULT_REAL_NOT_DOUBLE
)

INCLUDE_DIRECTORIES (${CMAKE_Fortran_MODULE_DIRECTORY} ${HDF4_HDF_BINARY_DIR} ${HDF4_HDF_SOURCE_DIR}/src  ${HDF4_HDF_TEST_SOURCE_DIR})
LINK_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_MFHDF_BINARY_DIR}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
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
   
STRING (TOLOWER ${CMAKE_SYSTEM_NAME} mf_f90_sys)

IF (HDF4_ENABLE_NETCDF)
  ADD_CUSTOM_COMMAND (
      OUTPUT ${HDF4_BINARY_DIR}/ftest.f
      COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/config/ftest-${mf_f90_sys}.f ${HDF4_BINARY_DIR}/ftest.f
      COMMENT " Copying ftest Name ftest-${mf_f90_sys}.f to ftest.f"
  )

  ADD_CUSTOM_COMMAND (
      OUTPUT ${HDF4_BINARY_DIR}/jackets.c
      COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/config/jackets-${mf_f90_sys}.c ${HDF4_BINARY_DIR}/jackets.c
      COMMENT " Copying jackets Name jackets-${mf_f90_sys}.c to jackets.c"
  )
ENDIF (HDF4_ENABLE_NETCDF)

ADD_CUSTOM_COMMAND (
   OUTPUT ${HDF4_BINARY_DIR}/netcdf.inc
   COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/config/netcdf-${mf_f90_sys}.inc ${HDF4_BINARY_DIR}/netcdf.inc
   COMMENT " Copying netcdf Name netcdf-${mf_f90_sys}.inc to netcdf.inc")

ADD_DEFINITIONS (-DHDF)

SET (HDF4_MFHDF_LIBSRC_CSTUB_FSRCS ${HDF4_MFHDF_FORTRAN_DIR}/mfsdf.c)
IF (HDF4_ENABLE_NETCDF)
  SET (HDF4_MFHDF_LIBSRC_CSTUB_FSRCS ${HDF4_MFHDF_LIBSRC_CSTUB_FSRCS} ${HDF4_BINARY_DIR}/jackets.c)
ENDIF (HDF4_ENABLE_NETCDF)

SET (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_MF_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_MFHDF_LIBSRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDF_SRC_SOURCE_DIR}/hproto_fortran.h)
SET_TARGET_PROPERTIES (${HDF4_MF_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)
IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  TARGET_LINK_LIBRARIES (${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_LINK_LIBRARIES (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FCSTUB_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_FCSTUB_LIB_NAME} ${LIB_TYPE})

IF (WIN32)
  SET (HDF4_MF_FORTRAN_SRCS 
      ${HDF4_MFHDF_FORTRAN_DIR}/fort_ps/mfsdpff.f
  )
ELSE (WIN32)
  SET (HDF4_MF_FORTRAN_SRCS 
      ${HDF4_MFHDF_FORTRAN_DIR}/mfsdff.f
  )
ENDIF (WIN32)
#-----------------------------------------------------------------------------
ADD_LIBRARY (${HDF4_MF_FORTRAN_LIB_TARGET} ${LIB_TYPE} ${HDF4_MF_FORTRAN_SRCS})
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (${HDF4_MF_FCSTUB_LIB_TARGET}
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE /DLL"
      )
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
ENDIF (WIN32)
SET_TARGET_PROPERTIES (${HDF4_MF_FORTRAN_LIB_TARGET} PROPERTIES LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_FCSTUB_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FORTRAN_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_FORTRAN_LIB_NAME} ${LIB_TYPE})

#-----------------------------------------------------------------------------
# test programs
#-----------------------------------------------------------------------------
IF (HDF4_ENABLE_NETCDF)
  #-- Adding test for ftest
  IF (WIN32)
    ADD_EXECUTABLE (ftest ${HDF4_BINARY_DIR}/ftest.f ${HDF4_HDF_TEST_SOURCE_DIR}/fort_ps/forsupffp.f ${HDF4_BINARY_DIR}/netcdf.inc)
  ELSE (WIN32)
    ADD_EXECUTABLE (ftest ${HDF4_BINARY_DIR}/ftest.f ${HDF4_HDF_TEST_SOURCE_DIR}/forsupff.f ${HDF4_HDF_TEST_SOURCE_DIR}/forsupf.c ${HDF4_BINARY_DIR}/netcdf.inc)
  ENDIF (WIN32)
  TARGET_NAMING (ftest)
  IF (WIN32)
    IF (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (ftest
            PROPERTIES
                COMPILE_FLAGS "/dll"
                LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        ) 
      ENDIF (MSVC)
    ELSE (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (ftest PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
      ENDIF (MSVC)
    ENDIF (BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES (ftest PROPERTIES LINKER_LANGUAGE Fortran)
  ENDIF (WIN32)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)

  ADD_TEST (NAME ftest COMMAND $<TARGET_FILE:ftest>)
ENDIF (HDF4_ENABLE_NETCDF)

#-- Adding test for f_hdftest
ADD_EXECUTABLE (f_hdftest hdftest.f)
TARGET_NAMING (f_hdftest)
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_hdftest
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE"
      ) 
    ENDIF (MSVC)
  ELSE (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_hdftest PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES (f_hdftest PROPERTIES LINKER_LANGUAGE Fortran)
ENDIF (WIN32)
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_hdftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_hdftest ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST (NAME f_hdftest COMMAND $<TARGET_FILE:f_hdftest>)

#-- Adding test for f_hdftest1
ADD_EXECUTABLE (f_hdftest1 hdftest1.f)
TARGET_NAMING (f_hdftest1)
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_hdftest1
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE"
      ) 
    ENDIF (MSVC)
  ELSE (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_hdftest1 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES (f_hdftest1 PROPERTIES LINKER_LANGUAGE Fortran)
ENDIF (WIN32)
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_hdftest1 ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_hdftest1 ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST (NAME f_hdftest1 COMMAND $<TARGET_FILE:f_hdftest1>)

#-- Adding test for f_tszip
ADD_EXECUTABLE (f_tszip tszip.f)
TARGET_NAMING (f_tszip)
IF (WIN32)
  IF (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_tszip
          PROPERTIES
              COMPILE_FLAGS "/dll"
              LINK_FLAGS "/SUBSYSTEM:CONSOLE"
      ) 
    ENDIF (MSVC)
  ELSE (BUILD_SHARED_LIBS)
    IF (MSVC)
      SET_TARGET_PROPERTIES (f_tszip PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (MSVC)
  ENDIF (BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES (f_tszip PROPERTIES LINKER_LANGUAGE Fortran)
ENDIF (WIN32)
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_tszip ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (f_tszip ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)

ADD_TEST (NAME f_tszip COMMAND $<TARGET_FILE:f_tszip>)

IF (HDF4_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/examples ${PROJECT_BINARY_DIR}/examples)
ENDIF (HDF4_BUILD_EXAMPLES)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS 
        ${HDF4_MF_FCSTUB_LIB_TARGET}
        ${HDF4_MF_FORTRAN_LIB_TARGET}
    EXPORT 
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION lib COMPONENT fortlibraries 
    ARCHIVE DESTINATION lib COMPONENT fortlibraries
    RUNTIME DESTINATION bin COMPONENT fortlibraries
)
