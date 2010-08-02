cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF)

ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/libsrc   ${HDF4_MFHDF_BINARY_DIR}/libsrc)

################################################################################
# Windows correctly uses the ADD_DIRECTORY command in the root CMakeLists.txt
# the following is only needed to get the mfhdf fortran lib to build before the fortran examples
IF (NOT WIN32)
  IF (HDF4_BUILD_FORTRAN)
    ADD_SUBDIRECTORY (${HDF4_MFHDF_SOURCE_DIR}/fortran ${HDF4_MFHDF_BINARY_DIR}/fortran)
  ENDIF (HDF4_BUILD_FORTRAN)
ENDIF (NOT WIN32)
##################################################################################

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
