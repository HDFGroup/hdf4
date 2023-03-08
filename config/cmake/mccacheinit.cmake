# This is the CMakeCache file.

########################
# EXTERNAL cache entries
########################

set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build Shared Libraries" FORCE)

set (BUILD_TESTING ON CACHE BOOL "Build HDF4 Unit Testing" FORCE)

set (HDF_PACKAGE_EXT "" CACHE STRING "Name of HDF package extension" FORCE)

set (HDF_PACKAGE_NAMESPACE "hdf4::" CACHE STRING "Name for HDF package namespace" FORCE)

set (HDF4_BUILD_EXAMPLES ON CACHE BOOL "Build HDF4 Library Examples" FORCE)

set (HDF4_BUILD_UTILS ON CACHE BOOL "Build HDF4 Library Utilities" FORCE)

set (HDF4_BUILD_GENERATORS OFF CACHE BOOL "Build Test Generators" FORCE)

set (HDF4_ENABLE_SZIP_SUPPORT ON CACHE BOOL "Use SZip Filter" FORCE)

set (HDF4_ENABLE_SZIP_ENCODING ON CACHE BOOL "Use SZip Encoding" FORCE)

set (HDF4_ENABLE_COVERAGE OFF CACHE BOOL "Enable code coverage for Libraries and Programs" FORCE)

set (HDF4_ENABLE_USING_MEMCHECKER ON CACHE BOOL "Indicate that a memory checker is used" FORCE)

set (HDF4_DISABLE_COMPILER_WARNINGS OFF CACHE BOOL "Disable compiler warnings" FORCE)

set (HDF4_ENABLE_DEPRECATED_SYMBOLS ON CACHE BOOL "Enable deprecated public API symbols" FORCE)

set (HDF4_PACKAGE_EXTLIBS OFF CACHE BOOL "(WINDOWS)CPACK - include external libraries" FORCE)

set (HDF4_NO_PACKAGES ON CACHE BOOL "CPACK - Disable packaging" FORCE)

set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Debug" FORCE)

set (HDF4_ALLOW_EXTERNAL_SUPPORT "NO" CACHE STRING "Allow External Library Building (NO GIT SVN TGZ)" FORCE)
set_property (CACHE HDF4_ALLOW_EXTERNAL_SUPPORT PROPERTY STRINGS NO GIT SVN TGZ)

set (ZLIB_PACKAGE_NAME "zlib" CACHE STRING "Name of ZLIB package" FORCE)
set (ZLIB_TGZ_NAME "ZLib.tar.gz" CACHE STRING "Use HDF4_ZLib from compressed file" FORCE)
set (ZLIB_TGZ_ORIGPATH "https://github.com/madler/zlib/releases/download/v1.2.13" CACHE STRING "Use ZLIB from original location" FORCE)
set (ZLIB_TGZ_ORIGNAME "zlib-1.2.13.tar.gz" CACHE STRING "Use ZLIB from original compressed file" FORCE)
set (ZLIB_USE_LOCALCONTENT OFF CACHE BOOL "Use local file for ZLIB FetchContent" FORCE)

set (SZIP_PACKAGE_NAME "szip" CACHE STRING "Name of SZIP package" FORCE)
set (LIBAEC_PACKAGE_NAME "libaec" CACHE STRING "Name of AEC SZIP package" FORCE)
set (SZIP_TGZ_NAME "SZip.tar.gz" CACHE STRING "Use SZip from compressed file" FORCE)
set (SZAEC_TGZ_NAME "LIBAEC.tar.gz" CACHE STRING "Use SZip AEC from compressed file" FORCE)
set (USE_LIBAEC ON CACHE BOOL "Use libaec szip replacement" FORCE)
set (LIBAEC_TGZ_ORIGPATH "https://gitlab.dkrz.de/k202009/libaec/-/archive/v1.0.6" CACHE STRING "Use LIBAEC from original location" FORCE)
set (LIBAEC_TGZ_ORIGNAME "libaec-v1.0.6.tar.gz" CACHE STRING "Use LIBAEC from original compressed file" FORCE)
set (LIBAEC_USE_LOCALCONTENT OFF CACHE BOOL "Use local file for LIBAEC FetchContent" FORCE)

set (JPEG_PACKAGE_NAME "jpeg" CACHE STRING "Name of JPEG package" FORCE)
set (JPEG_TGZ_NAME "JPEG9e.tar.gz" CACHE STRING "Use JPEG from compressed file" FORCE)
set (JPEG_TGZ_ORIGPATH "https://www.ijg.org/filesz" CACHE STRING "Use JPEG from original location" FORCE)
set (JPEG_TGZ_ORIGNAME "jpegsrc.v9e.tar.gz" CACHE STRING "Use JPEG from original compressed file" FORCE)
set (JPEG_USE_LOCALCONTENT OFF CACHE BOOL "Use local file for JPEG FetchContent" FORCE)
