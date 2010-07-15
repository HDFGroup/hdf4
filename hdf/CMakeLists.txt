cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF C CXX Fortran)

IF (HDF4_BUILD_FORTRAN)
  #-----------------------------------------------------------------------------
  # Detect name mangling convention used between Fortran and C
  #-----------------------------------------------------------------------------
  INCLUDE (FortranCInterface)
  FortranCInterface_HEADER (
      ${HDF4_HDF_SRC_BINARY_DIR}/FCMangle.h
      MACRO_NAMESPACE "H4_F77_"
      SYMBOL_NAMESPACE "H4_F77_"
      SYMBOLS mysub mymod:my_sub
  )

  FILE (STRINGS ${HDF4_HDF_SRC_BINARY_DIR}/FCMangle.h CONTENTS REGEX "H4_F77_GLOBAL\\(.*,.*\\) +(.*)") 
  STRING (REGEX MATCH "H4_F77_GLOBAL\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
  SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

  FILE (STRINGS ${HDF4_HDF_SRC_BINARY_DIR}/FCMangle.h CONTENTS REGEX "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)") 
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
        SET (CHECK_FUNCTION_EXISTS_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
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

#      MESSAGE ( "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")
#      MESSAGE ( "Test result ${OUTPUT}")
#      MESSAGE ( "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")

      IF (${VARIABLE})
        SET (${VARIABLE} 1 CACHE INTERNAL "Have Fortran function ${FUNCTION}")
        MESSAGE (STATUS "Testing Fortran ${FUNCTION} - OK")
        FILE (APPEND
            ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log 
            "Determining if the Fortran ${FUNCTION} exists passed with the following output:\n"
            "${OUTPUT}\n\n"
        )
      ELSE (${VARIABLE})
        MESSAGE (STATUS "Testing Fortran ${FUNCTION} - Fail")
        SET (${VARIABLE} "" CACHE INTERNAL "Have Fortran function ${FUNCTION}")
        FILE (APPEND
            ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log 
            "Determining if the Fortran ${FUNCTION} exists failed with the following output:\n"
            "${OUTPUT}\n\n"
        )
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
  CHECK_FORTRAN_FEATURE (sizeof 
  "
       PROGRAM main
       i = sizeof(x)       
       END PROGRAM
  " 
    FORTRAN_HAVE_SIZEOF
  )  

  CHECK_FORTRAN_FEATURE (RealIsNotDouble 
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

  #-----------------------------------------------------------------------------
  # Fortran Real Size
  #-----------------------------------------------------------------------------
  IF (FORTRAN_DEFAULT_REAL_NOT_DOUBLE)
    # default real is 4 bytes, so include double signatures
    SET (F_DBLE "Include")
  ELSE (FORTRAN_DEFAULT_REAL_NOT_DOUBLE)
    # default real is 8 bytes, so exclude double signatures
    SET (F_DBLE "Exclude")
  ENDIF (FORTRAN_DEFAULT_REAL_NOT_DOUBLE)

  #-----------------------------------------------------------------------------
  # Make sure generated files and modules are picked up correctly
  #-----------------------------------------------------------------------------
  INCLUDE_DIRECTORIES ( 
      ${CMAKE_Fortran_MODULE_DIRECTORY}
      ${HDF4_HDF_SRC_BINARY_DIR}
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
ENDIF (HDF4_BUILD_FORTRAN)

ADD_SUBDIRECTORY (${HDF4_HDF_SOURCE_DIR}/src      ${HDF4_HDF_BINARY_DIR}/src)

IF (HDF4_BUILD_UTILS)
  ADD_SUBDIRECTORY (${HDF4_HDF_SOURCE_DIR}/util   ${HDF4_HDF_BINARY_DIR}/util)
ENDIF (HDF4_BUILD_UTILS)

IF (HDF4_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY (${HDF4_HDF_SOURCE_DIR}/examples   ${HDF4_HDF_BINARY_DIR}/examples)
ENDIF (HDF4_BUILD_EXAMPLES)

IF (HDF4_BUILD_FORTRAN)
  ADD_SUBDIRECTORY (${HDF4_HDF_SOURCE_DIR}/fortran     ${HDF4_HDF_BINARY_DIR}/fortran)
ENDIF (HDF4_BUILD_FORTRAN)
