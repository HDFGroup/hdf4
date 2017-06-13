#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
set (HDF_PREFIX "H4")
include (${HDF_RESOURCES_EXT_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Option to Build HDF4 versions of NetCDF-3 APIS
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_NETCDF "Build HDF4 versions of NetCDF-3 APIS" ON)
if (HDF4_ENABLE_NETCDF)
  set (${HDF_PREFIX}_HAVE_NETCDF 1)
endif ()
