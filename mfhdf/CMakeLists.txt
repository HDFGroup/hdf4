cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF)

ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/libsrc   ${HDF4_MFHDF_BINARY_DIR}/libsrc)

IF (HDF4_BUILD_TOOLS)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/dumper   ${HDF4_MFHDF_BINARY_DIR}/dumper)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/hdfimport   ${HDF4_MFHDF_BINARY_DIR}/hdfimport)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/hdiff   ${HDF4_MFHDF_BINARY_DIR}/hdiff)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/hrepack   ${HDF4_MFHDF_BINARY_DIR}/hrepack)
  ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/ncdump   ${HDF4_MFHDF_BINARY_DIR}/ncdump)
ENDIF (HDF4_BUILD_TOOLS)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  IF (HDF4_BUILD_TOOLS)
#    ADD_SUBDIRECTORY(${HDF4_MFHDF_SOURCE_DIR}/ncgen   ${HDF4_MFHDF_BINARY_DIR}/ncgen)
    ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/nctest   ${HDF4_MFHDF_BINARY_DIR}/nctest)
  ENDIF (HDF4_BUILD_TOOLS)
ENDIF (BUILD_TESTING )
