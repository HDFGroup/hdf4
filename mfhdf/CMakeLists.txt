cmake_minimum_required (VERSION 3.12)
project (HDF4_MFHDF C)

set(CMAKE_DIRECTORY_LABELS “MFHDF”)

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
if (HDF4_BUILD_TOOLS)
  set (NCGEN_UTILITY 0)
  add_subdirectory (dumper)
  add_subdirectory (hdfimport)
  add_subdirectory (hdiff)
  add_subdirectory (hrepack)
  if (HDF4_BUILD_NETCDF_TOOLS)
    set (NCGEN_UTILITY 1)
    add_subdirectory (ncgen)
    add_subdirectory (ncdump)
  endif ()
  if (BUILD_TESTING)
    add_subdirectory (nctest)
  endif ( )
endif ()
