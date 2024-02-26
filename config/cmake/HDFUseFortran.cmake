# This file provides functions for Fortran support.
#
#-------------------------------------------------------------------------------
enable_language (Fortran)
set (HDF_PREFIX "H4")

include (CheckFortranSourceRuns)
include (CheckFortranSourceCompiles)

#-------------------------------------------------------------------------------
#  Fix Fortran flags if we are compiling statically on Windows using
#  Windows_MT.cmake from config/cmake/UserMacros
#-------------------------------------------------------------------------------
if (BUILD_STATIC_CRT_LIBS)
  TARGET_STATIC_CRT_FLAGS ()
endif ()

#-----------------------------------------------------------------------------
# Detect name mangling convention used between Fortran and C
#-----------------------------------------------------------------------------
include (FortranCInterface)

#-----------------------------------------------------------------------------
# Verify that the Fortran and C/C++ compilers work together
#-----------------------------------------------------------------------------
FortranCInterface_VERIFY()

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

#test code source
set (SIZEOF_CODE
  "
       PROGRAM main
       i = sizeof(x)
       END PROGRAM
  "
)
set (C_SIZEOF_CODE
  "
       PROGRAM main
         USE ISO_C_BINDING
         INTEGER(C_INT) :: a
         INTEGER(C_SIZE_T) :: result
         result = c_sizeof(a)
       END PROGRAM
  "
)
set (STORAGE_SIZE_CODE
  "
       PROGRAM main
         INTEGER :: a
         INTEGER :: result
         result = storage_size(a)
       END PROGRAM
  "
)
set (ISO_FORTRAN_ENV_CODE
  "
       PROGRAM main
         USE, INTRINSIC :: ISO_FORTRAN_ENV
       END PROGRAM
  "
)
set (REALISNOTDOUBLE_CODE
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
)
set (ISO_C_BINDING_CODE
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
)

if (HDF4_REQUIRED_LIBRARIES)
  set (CMAKE_REQUIRED_LIBRARIES "${HDF4_REQUIRED_LIBRARIES}")
endif ()
check_fortran_source_compiles (${SIZEOF_CODE} ${HDF_PREFIX}_FORTRAN_HAVE_SIZEOF SRC_EXT f90)
check_fortran_source_compiles (${C_SIZEOF_CODE} ${HDF_PREFIX}_FORTRAN_HAVE_C_SIZEOF SRC_EXT f90)
check_fortran_source_compiles (${STORAGE_SIZE_CODE} ${HDF_PREFIX}_FORTRAN_HAVE_STORAGE_SIZE SRC_EXT f90)
check_fortran_source_compiles (${ISO_FORTRAN_ENV_CODE} ${HDF_PREFIX}_HAVE_ISO_FORTRAN_ENV SRC_EXT f90)
check_fortran_source_compiles (${REALISNOTDOUBLE_CODE} ${HDF_PREFIX}_FORTRAN_DEFAULT_REAL_NOT_DOUBLE SRC_EXT f90)
check_fortran_source_compiles (${ISO_C_BINDING_CODE} ${HDF_PREFIX}_FORTRAN_HAVE_ISO_C_BINDING SRC_EXT f90)

#-----------------------------------------------------------------------------
# Add debug information (intel Fortran : JB)
#-----------------------------------------------------------------------------
if (CMAKE_Fortran_COMPILER MATCHES ifort)
    if (WIN32 AND NOT MINGW)
        set (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE STRING "flags" FORCE)
        set (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "flags" FORCE)
    endif ()
endif ()
