#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFortranFunctionExists.cmake)

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
