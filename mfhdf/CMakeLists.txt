PROJECT ( HDF4_MFHDF )

# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/mfhdf
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/mfhdf
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin/mfhdf
  CACHE PATH
  "Single Directory for all static libraries."
  )

ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/libsrc   libsrc)
IF(HDF4_BUILD_TOOLS)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/dumper   dumper)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/hdfimport   hdfimport)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/hdiff   hdiff)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/hrepack   hrepack)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/ncdump   ncdump)
ENDIF(HDF4_BUILD_TOOLS)

#IF (HDF4_BUILD_FORTRAN)
#  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/fortran ${PROJECT_BINARY_DIR}/mfhdf/fortran)
#ENDIF (HDF4_BUILD_FORTRAN)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  SET (DART_TESTING_TIMEOUT 1200 CACHE INTEGER  
    "Timeout in seconds for each test (default 1200=20minutes)")
  ENABLE_TESTING()
  INCLUDE(CTest)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/test     test)
    
  IF(HDF4_BUILD_TOOLS)
#    ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/ncgen   ncgen)
    ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/nctest   nctest)
  ENDIF(HDF4_BUILD_TOOLS)
  
  include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
ENDIF ( BUILD_TESTING )
