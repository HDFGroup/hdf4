#
# This file provides functions for Fortran support.
#
#-------------------------------------------------------------------------------
enable_language (Fortran)
  
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

include (${HDF_RESOURCES_EXT_DIR}/HDFUseFortran.cmake)
