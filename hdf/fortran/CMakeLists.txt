cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_FORTRAN)

#-----------------------------------------------------------------------------
# Build the Fortran Examples
#-----------------------------------------------------------------------------
IF (HDF4_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY (${HDF4_HDF_FORTRAN_SOURCE_DIR}/examples ${HDF4_HDF_FORTRAN_BINARY_DIR}/examples)
ENDIF (HDF4_BUILD_EXAMPLES)
