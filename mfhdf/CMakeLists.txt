cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_MFHDF_SOURCE_DIR}/dumper" AND IS_DIRECTORY "${HDF4_MFHDF_SOURCE_DIR}/dumper")
  option (HDF4_BUILD_TOOLS  "Build HDF4 Tools" OFF)
  if (HDF4_BUILD_TOOLS)
    set (NCGEN_UTILITY 0)
    add_subdirectory (dumper)
    add_subdirectory (hdfimport)
    add_subdirectory (hdiff)
    add_subdirectory (hrepack)
    add_subdirectory (ncgen)
    add_subdirectory (ncdump)
    if (BUILD_TESTING)
      add_subdirectory (nctest)
    endif ( )
  endif ()
endif ()
