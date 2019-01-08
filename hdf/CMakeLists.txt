cmake_minimum_required (VERSION 3.10)
project (HDF4_HDF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_HDF_SOURCE_DIR}/util" AND IS_DIRECTORY "${HDF4_HDF_SOURCE_DIR}/util")
  option (HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)
  if (HDF4_BUILD_UTILS OR HDF4_BUILD_TOOLS)
    add_subdirectory (util)
  endif ()
endif ()
