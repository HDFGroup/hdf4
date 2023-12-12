cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_HDF)

#-----------------------------------------------------------------------------
# Build the hdf examples
#-----------------------------------------------------------------------------
add_subdirectory (examples)
if (HDF_BUILD_FORTRAN)
  add_subdirectory (fortran)
endif ()
