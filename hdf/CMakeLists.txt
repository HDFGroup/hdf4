PROJECT ( HDF4_HDF )

# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/hdf
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/hdf
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/hdf
  CACHE PATH
  "Single Directory for all static libraries."
  )

ADD_SUBDIRECTORY(${HDF4_HDF_SOURCE_DIR}/src      src)

IF (HDF4_BUILD_UTILS)
  ADD_SUBDIRECTORY(${HDF4_HDF_SOURCE_DIR}/util ${PROJECT_BINARY_DIR}/hdf/util)
ENDIF ( HDF4_BUILD_UTILS )

#IF (HDF4_BUILD_FORTRAN)
#  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/hdf/fortran ${PROJECT_BINARY_DIR}/hdf/fortran)
#ENDIF (HDF4_BUILD_FORTRAN)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
#OPTION(BUILD_TESTING "Build HDF4 Unit Testing" OFF)
#IF (BUILD_TESTING)
#  SET (DART_TESTING_TIMEOUT 1200 CACHE INTEGER  
#    "Timeout in seconds for each test (default 1200=20minutes)")
#  ENABLE_TESTING()
#  INCLUDE(CTest)
#  IF(NOT HDF4_EXTERNALLY_CONFIGURED)
#    ADD_SUBDIRECTORY(${HDF4_HDF_SOURCE_DIR}/test     test)
#  ENDIF(NOT HDF4_EXTERNALLY_CONFIGURED)
#  include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
#ENDIF ( BUILD_TESTING )
