cmake_minimum_required (VERSION 3.18)
project (HDF4_HDF C)

set(CMAKE_DIRECTORY_LABELS “HDF”)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
if (IS_DIRECTORY "${HDF4_HDF_SOURCE_DIR}/util")
  option (HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)
  if (HDF4_BUILD_UTILS OR HDF4_BUILD_TOOLS)
    add_subdirectory (util)
  endif ()
endif ()
