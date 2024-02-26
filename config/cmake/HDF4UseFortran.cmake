#
# This file provides functions for additional Fortran support.
#
#-------------------------------------------------------------------------------

include (${HDF_RESOURCES_DIR}/HDFUseFortran.cmake)

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND CMAKE_Fortran_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
  set (CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fallow-argument-mismatch")
endif ()
