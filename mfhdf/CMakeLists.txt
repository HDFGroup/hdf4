cmake_minimum_required (VERSION 3.1.0)
PROJECT (HDF4_MFHDF)

if (CYGWIN)
  INCLUDE_DIRECTORIES (/usr/include/tirpc)
else (CYGWIN)
  INCLUDE_DIRECTORIES (/usr/include/rpc)
endif (CYGWIN)

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_MFHDF_SOURCE_DIR}/dumper" AND IS_DIRECTORY "${HDF4_MFHDF_SOURCE_DIR}/dumper")
  option (HDF4_BUILD_TOOLS  "Build HDF4 Tools" OFF)
  if (HDF4_BUILD_TOOLS)
    set (NCGEN_UTILITY 0)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/dumper      ${HDF4_MFHDF_BINARY_DIR}/dumper)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/hdfimport   ${HDF4_MFHDF_BINARY_DIR}/hdfimport)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/hdiff       ${HDF4_MFHDF_BINARY_DIR}/hdiff)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/hrepack     ${HDF4_MFHDF_BINARY_DIR}/hrepack)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/ncgen       ${HDF4_MFHDF_BINARY_DIR}/ncgen)
    add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/ncdump      ${HDF4_MFHDF_BINARY_DIR}/ncdump)
    if (BUILD_TESTING)
      add_subdirectory (${HDF4_MFHDF_SOURCE_DIR}/nctest    ${HDF4_MFHDF_BINARY_DIR}/nctest)
    endif (BUILD_TESTING )
  endif (HDF4_BUILD_TOOLS)
endif (EXISTS "${HDF4_MFHDF_SOURCE_DIR}/dumper" AND IS_DIRECTORY "${HDF4_MFHDF_SOURCE_DIR}/dumper")
