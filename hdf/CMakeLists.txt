cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
IF (EXISTS "${HDF4_HDF_SOURCE_DIR}/util" AND IS_DIRECTORY "${HDF4_HDF_SOURCE_DIR}/util")
  OPTION (HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)
  IF (HDF4_BUILD_UTILS)
    ADD_SUBDIRECTORY (${HDF4_HDF_SOURCE_DIR}/util   ${HDF4_HDF_BINARY_DIR}/util)
  ENDIF (HDF4_BUILD_UTILS)
ENDIF (EXISTS "${HDF4_HDF_SOURCE_DIR}/util" AND IS_DIRECTORY "${HDF4_HDF_SOURCE_DIR}/util")
