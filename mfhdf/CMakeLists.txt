cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF C)

set(CMAKE_DIRECTORY_LABELS “MFHDF”)

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
if (HDF4_BUILD_TOOLS)
  add_subdirectory (hdp)
  add_subdirectory (hdfimport)
  add_subdirectory (hdiff)
  add_subdirectory (hrepack)
endif ()
