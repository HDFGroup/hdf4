# This file provides functions for Fortran support.
#
#-------------------------------------------------------------------------------
enable_language (Fortran)
set (HDF_PREFIX "H4")

#-------------------------------------------------------------------------------
#  Fix Fortran flags if we are compiling staticly on Windows using
#  Windows_MT.cmake from config/cmake/UserMacros
#-------------------------------------------------------------------------------
if (BUILD_STATIC_CRT_LIBS)
  TARGET_STATIC_CRT_FLAGS ()
endif ()

#-----------------------------------------------------------------------------
# Detect name mangling convention used between Fortran and C
#-----------------------------------------------------------------------------
include (FortranCInterface)
FortranCInterface_HEADER (
    ${CMAKE_BINARY_DIR}/F77Mangle.h
    MACRO_NAMESPACE "H4_F77_"
    SYMBOL_NAMESPACE "H4_F77_"
    SYMBOLS mysub mymod:my_sub
)

file (STRINGS ${CMAKE_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL\\(.*,.*\\) +(.*)")
string (REGEX MATCH "H4_F77_GLOBAL\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) ${CMAKE_MATCH_1}")

file (STRINGS ${CMAKE_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)")
string (REGEX MATCH "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) ${CMAKE_MATCH_1}")

#-----------------------------------------------------------------------------
# The provided CMake Fortran macros don't provide a general check function
# so this one is used for a sizeof test.
#-----------------------------------------------------------------------------
macro (CHECK_FORTRAN_FEATURE FUNCTION CODE VARIABLE)
    message (STATUS "Testing Fortran ${FUNCTION}")
    if (HDF4_REQUIRED_LIBRARIES)
      set (CHECK_FUNCTION_EXISTS_ADD_LIBRARIES
          "-DLINK_LIBRARIES:STRING=${HDF4_REQUIRED_LIBRARIES}")
    else ()
      set (CHECK_FUNCTION_EXISTS_ADD_LIBRARIES)
    endif ()
    file (WRITE
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testFortranCompiler.f
        "${CODE}"
    )
    TRY_COMPILE (RESULT_VAR
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testFortranCompiler.f
        CMAKE_FLAGS "${CHECK_FUNCTION_EXISTS_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )

#    message (STATUS "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")
#    message (STATUS "Test result ${OUTPUT}")
#    message (STATUS "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ")

    if (${RESULT_VAR})
      set (${VARIABLE} 1 CACHE INTERNAL "Have Fortran function ${FUNCTION}")
      message (STATUS "Testing Fortran ${FUNCTION} - OK")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
          "Determining if the Fortran ${FUNCTION} exists passed with the following output:\n"
          "${OUTPUT}\n\n"
      )
    else ()
      message (STATUS "Testing Fortran ${FUNCTION} - Fail")
      set (${VARIABLE} 0 CACHE INTERNAL "Have Fortran function ${FUNCTION}")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
          "Determining if the Fortran ${FUNCTION} exists failed with the following output:\n"
          "${OUTPUT}\n\n")
    endif ()
endmacro ()

#-----------------------------------------------------------------------------
# Configure Checks which require Fortran compilation must go in here
# not in the main ConfigureChecks.cmake files, because if the user has
# no Fortran compiler, problems arise.
#
# Be careful with leading spaces here, do not remove them.
#-----------------------------------------------------------------------------

# Check for Non-standard extension intrinsic function SIZEOF
set (${HDF_PREFIX}_FORTRAN_HAVE_SIZEOF FALSE)
CHECK_FORTRAN_FEATURE(sizeof
  "
       PROGRAM main
       i = sizeof(x)
       END PROGRAM
  "
  ${HDF_PREFIX}_FORTRAN_HAVE_SIZEOF
)

# Check for F2008 standard intrinsic function C_SIZEOF
set (${HDF_PREFIX}_FORTRAN_HAVE_C_SIZEOF FALSE)
CHECK_FORTRAN_FEATURE(c_sizeof
  "
       PROGRAM main
         USE ISO_C_BINDING
         INTEGER(C_INT) :: a
         INTEGER(C_SIZE_T) :: result
         result = c_sizeof(a)
       END PROGRAM
  "
  ${HDF_PREFIX}_FORTRAN_HAVE_C_SIZEOF
)

# Check for F2008 standard intrinsic function STORAGE_SIZE
CHECK_FORTRAN_FEATURE(storage_size
  "
       PROGRAM main
         INTEGER :: a
         INTEGER :: result
         result = storage_size(a)
       END PROGRAM
  "
  ${HDF_PREFIX}_FORTRAN_HAVE_STORAGE_SIZE
)

# Check for F2008 standard intrinsic module "ISO_FORTRAN_ENV"
set (${HDF_PREFIX}_HAVE_ISO_FORTRAN_ENV FALSE)
CHECK_FORTRAN_FEATURE(ISO_FORTRAN_ENV
  "
       PROGRAM main
         USE, INTRINSIC :: ISO_FORTRAN_ENV
       END PROGRAM
  "
  ${HDF_PREFIX}_HAVE_ISO_FORTRAN_ENV
)

set (${HDF_PREFIX}_FORTRAN_DEFAULT_REAL_NOT_DOUBLE FALSE)
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
  ${HDF_PREFIX}_FORTRAN_DEFAULT_REAL_NOT_DOUBLE
)

#-----------------------------------------------------------------------------
# Checks if the ISO_C_BINDING module meets all the requirements
#-----------------------------------------------------------------------------
set (${HDF_PREFIX}_FORTRAN_HAVE_ISO_C_BINDING FALSE)
CHECK_FORTRAN_FEATURE(iso_c_binding
  "
       PROGRAM main
            USE iso_c_binding
            IMPLICIT NONE
            TYPE(C_PTR) :: ptr
            TYPE(C_FUNPTR) :: funptr
            INTEGER(C_INT64_T) :: c_int64_type
            CHARACTER(LEN=80, KIND=c_char), TARGET :: ichr
            ptr = C_LOC(ichr(1:1))
       END PROGRAM
  "
  ${HDF_PREFIX}_FORTRAN_HAVE_ISO_C_BINDING
)

#-----------------------------------------------------------------------------
# Add debug information (intel Fortran : JB)
#-----------------------------------------------------------------------------
if (CMAKE_Fortran_COMPILER MATCHES ifort)
    if (WIN32 AND NOT MINGW)
        set (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE STRING "flags" FORCE)
        set (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "flags" FORCE)
    endif ()
endif ()
