# This is the CMakeCache file.

########################
# EXTERNAL cache entries
########################

set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build Shared Libraries" FORCE)

set (BUILD_TESTING ON CACHE BOOL "Build HDF4 Unit Testing" FORCE)

set (HDF_PACKAGE_EXT "" CACHE STRING "Name of HDF package extension" FORCE)

set (HDF4_BUILD_EXAMPLES ON CACHE BOOL "Build HDF4 Library Examples" FORCE)

set (HDF4_BUILD_FORTRAN ON CACHE BOOL "Build FORTRAN support" FORCE)

set (HDF4_BUILD_TOOLS ON CACHE BOOL "Build HDF4 Tools" FORCE)

set (HDF4_BUILD_UTILS ON CACHE BOOL "Build HDF4 Library Utilities" FORCE)

set (HDF4_BUILD_GENERATORS OFF CACHE BOOL "Build Test Generators" FORCE)

set (HDF4_ENABLE_NETCDF ON CACHE BOOL "Build HDF4 versions of NetCDF-3 APIS" FORCE)

set (HDF4_BUILD_XDR_LIB OFF CACHE BOOL "Build HDF4 XDR Library" FORCE)

set (HDF4_ENABLE_PARALLEL OFF CACHE BOOL "Enable parallel build (requires MPI)" FORCE)

set (MPIEXEC_MAX_NUMPROCS "3" CACHE STRING "Minimum number of processes for HDF parallel tests" FORCE)

set (HDF4_ENABLE_JPEG_LIB_SUPPORT ON CACHE BOOL "Enable Jpeg library" FORCE)

set (HDF4_ENABLE_Z_LIB_SUPPORT ON CACHE BOOL "Enable Zlib Filters" FORCE)

set (HDF4_ENABLE_SZIP_SUPPORT ON CACHE BOOL "Use SZip Filter" FORCE)

set (HDF4_ENABLE_SZIP_ENCODING ON CACHE BOOL "Use SZip Encoding" FORCE)

set (HDF4_ENABLE_COVERAGE OFF CACHE BOOL "Enable code coverage for Libraries and Programs" FORCE)

set (HDF4_ENABLE_USING_MEMCHECKER ON CACHE BOOL "Indicate that a memory checker is used" FORCE)

set (HDF4_DISABLE_COMPILER_WARNINGS OFF CACHE BOOL "Disable compiler warnings" FORCE)

set (HDF4_ENABLE_DEPRECATED_SYMBOLS ON CACHE BOOL "Enable deprecated public API symbols" FORCE)

set (HDF4_PACKAGE_EXTLIBS OFF CACHE BOOL "(WINDOWS)CPACK - include external libraries" FORCE)

set (HDF4_NO_PACKAGES ON CACHE BOOL "CPACK - Disable packaging" FORCE)

set (HDF4_ALLOW_EXTERNAL_SUPPORT "NO" CACHE STRING "Allow External Library Building (NO GIT SVN TGZ)" FORCE)
set_property (CACHE HDF4_ALLOW_EXTERNAL_SUPPORT PROPERTY STRINGS NO GIT SVN TGZ)

set (ZLIB_TGZ_NAME "ZLib.tar.gz" CACHE STRING "Use ZLib from compressed file" FORCE)

set (SZIP_TGZ_NAME "SZip.tar.gz" CACHE STRING "Use SZip from compressed file" FORCE)

set (JPEG_TGZ_NAME "JPEG8d.tar.gz" CACHE STRING "Use JPEG from compressed file" FORCE)

set (ZLIB_PACKAGE_NAME "zlib" CACHE STRING "Name of ZLIB package" FORCE)

set (SZIP_PACKAGE_NAME "szip" CACHE STRING "Name of SZIP package" FORCE)

set (JPEG_PACKAGE_NAME "jpeg" CACHE STRING "Name of JPEG package" FORCE)
