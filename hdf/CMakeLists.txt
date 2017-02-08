cmake_minimum_required (VERSION 3.2.2)
PROJECT (HDF4_HDF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_HDF_SOURCE_DIR}/util" AND IS_DIRECTORY "${HDF4_HDF_SOURCE_DIR}/util")
  option (HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)
  if (HDF4_BUILD_UTILS OR HDF4_BUILD_TOOLS)
    add_subdirectory (${HDF4_HDF_SOURCE_DIR}/util   ${HDF4_HDF_BINARY_DIR}/util)
  endif ()
endif ()
