set(CMAKE_USER_MAKE_RULES_OVERRIDE
   ${CMAKE_CURRENT_SOURCE_DIR}/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
   ${CMAKE_CURRENT_SOURCE_DIR}/cxx_flag_overrides.cmake)
PROJECT(HDF4 C CXX)
cmake_minimum_required(VERSION 2.6)

#-----------------------------------------------------------------------------
# Instructions for use :
#
# For standard build of HDF4 libraries,tests and tools. 
# Run cmake using the HDF4 source tree to generate a build tree.
# Make install can be used to install all components for system-wide use.
#
# To include HDF4 as a sub-project within another project. 
# Set HDF4_EXTERNALLY_CONFIGURED to 1 in the parent project and
# supply values for the following variables...
# HDF4_EXPORTED_TARGETS :
#   Set this to the name of the targets variable which controls exports
#   If unset (because parent project does not support/use the 
#   INSTALL(EXPORT target...) syntax), then targets are not configured 
#   for export during install.
# HDF4_TARGET_DEPENDENCIES :
#   If the build of HDF4 libs is being customized, then rules for the
#   dependencies of the HDF4 libs may be 'incomplete', add additional
#   dependencies to this variable so that external projects pick them up
#
# Consider this example from the ParaView project, it builds its own zlib
# library and tells HDF4 to add it as a dependency - this ensures that
# any project making use of this build of HDF4 will use the correct zlib
#
#  SET(HDF4_EXTERNALLY_CONFIGURED 1)
#  IF(HDF4_ENABLE_Z_LIB_SUPPORT)
#    SET(HDF4_TARGET_DEPENDENCIES vtkzlib)
#  ENDIF(HDF4_ENABLE_Z_LIB_SUPPORT)
#  SET(HDF4_EXPORTED_TARGETS)
#  ADD_SUBDIRECTORY(Utilities/HDF4-2.4)
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Basic HDF4 stuff here
#-----------------------------------------------------------------------------
SET (HDF4_PACKAGE "HDF4")
SET (HDF4_PACKAGE_NAME "HDF")
SET (HDF4_PACKAGE_VERSION 4.2.5)
SET (HDF4_PACKAGE_VERSION_MAJOR 4.2)
SET (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION}")
SET (HDF4_PACKAGE_TARNAME "hdf")
SET (HDF4_PACKAGE_URL "htt://www.hdfgroup.org")
SET (HDF4_PACKAGE_BUGREPORT "help@hdfgroup.org")

#-----------------------------------------------------------------------------
# Set the names of all the libraries
#-----------------------------------------------------------------------------
SET (HDF4_LIB_NAME              "HDF4")
SET (HDF4_SRC_LIB_NAME          "hdf")
SET (HDF4_SRC_FCSTUB_LIB_NAME   "hdf_fcstub")
SET (HDF4_SRC_FORTRAN_LIB_NAME  "hdf_fortran")
SET (HDF4_MF_LIB_NAME           "mfhdf")
SET (HDF4_MF_XDR_LIB_NAME       "xdr")
SET (HDF4_SRC_TEST_LIB_NAME     "hdf_test")
SET (HDF4_MF_FCSTUB_LIB_NAME    "mfhdf_fcstub")
SET (HDF4_MF_FORTRAN_LIB_NAME   "mfhdf_fortran")
SET (HDF4_MF_TEST_LIB_NAME      "mf_test")
SET (HDF4_TOOLS_LIB_NAME        "h4tools")

#-----------------------------------------------------------------------------
# Setup output Directories 
#-----------------------------------------------------------------------------
IF(NOT HDF4_EXTERNALLY_CONFIGURED)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
ENDIF(NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
SET (HDF4_RESOURCES_DIR       ${HDF4_SOURCE_DIR}/Resources)
SET (HDF4_HDF_SOURCE_DIR      ${HDF4_SOURCE_DIR}/hdf/src)
SET (HDF4_HDF_TEST_SOURCE_DIR ${HDF4_SOURCE_DIR}/hdf/test)
SET (HDF4_MFHDF_SOURCE_DIR    ${HDF4_SOURCE_DIR}/mfhdf/libsrc)
SET (HDF4_MFHDF_TEST_DIR      ${HDF4_SOURCE_DIR}/mfhdf/test)
SET (HDF4_MFHDF_FORTRAN_DIR   ${HDF4_SOURCE_DIR}/mfhdf/fortran)
SET (HDF4_MFHDF_XDR_DIR       ${HDF4_SOURCE_DIR}/mfhdf/xdr)

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
INCLUDE(${HDF4_RESOURCES_DIR}/HDF4Macros.cmake)

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects using FindHDF4. If HDF4 is configured as a sub-project
# then we do not override the setting provided by the parent project
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  SET(HDF4_EXPORTED_TARGETS "HDF4-targets")
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)  

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME, 
# add it to this variable. This is NOT used by Make Install, but for projects
# which include HDF4 as a sub-project within their build tree
#-----------------------------------------------------------------------------
SET_GLOBAL_VARIABLE(HDF4_LIBRARIES_TO_EXPORT "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
INCLUDE(${HDF4_SOURCE_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Option to Build Shared/Static libs, default is static
#-----------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  SET (H4_BUILT_AS_DYNAMIC_LIB 1)       
ELSE(BUILD_SHARED_LIBS)
  IF (NOT WIN32)
    # should this be a user setting : Everyone uses it anyway ?
    ADD_DEFINITIONS(-fPIC)
  ENDIF (NOT WIN32)
ENDIF (BUILD_SHARED_LIBS)

    ADD_DEFINITIONS(-DBIG_LONGS -DSWAP)
    ADD_DEFINITIONS(-DHAVE_CONFIG_H)

#-----------------------------------------------------------------------------
# Include the main src and config directories
#-----------------------------------------------------------------------------
SET (HDF4_INCLUDE_DIRECTORIES
  ${HDF4_HDF_SOURCE_DIR} 
  ${HDF4_MFHDF_SOURCE_DIR} 
  ${HDF4_SOURCE_DIR} 
  ${HDF4_BINARY_DIR}
)
INCLUDE_DIRECTORIES( ${HDF4_INCLUDE_DIRECTORIES} )

#-----------------------------------------------------------------------------
# Add some definitions for Debug Builds
#-----------------------------------------------------------------------------
IF ( CMAKE_BUILD_TYPE MATCHES Debug )
  ADD_DEFINITIONS(-DDEBUG)
  #-- NMake Makefiles will overwhelm the console with warnings if -Wall is used.
  if (NOT WIN32)
    ADD_DEFINITIONS(-Wall)
  endif()
ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-UH4_DEBUG_API -DNDEBUG)
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
IF (NOT WIN32)
 SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -fomit-frame-pointer -finline-functions -fno-common")
ENDIF (NOT WIN32)

if(MSVC)
    set(CMAKE_MFC_FLAG 0)
    set(SHARED_C_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
    set(SHARED_C_FLAGS_MINSIZEREL     "/MD /O1 /Ob1 /D NDEBUG")
    set(SHARED_C_FLAGS_RELEASE        "/MD /O2 /Ob2 /D NDEBUG")
    set(SHARED_C_FLAGS_RELWITHDEBINFO "/MD /Zi /O2 /Ob1 /D NDEBUG")
endif(MSVC)

#-----------------------------------------------------------------------------
# Option to allow the user to disable compiler warnings
#-----------------------------------------------------------------------------
OPTION (HDF4_DISABLE_COMPILER_WARNINGS "Disable compiler warnings" OFF)
IF (HDF4_DISABLE_COMPILER_WARNINGS)
  # MSVC uses /w to suppress warnings.  It also complains if another
  # warning level is given, so remove it.
  IF (MSVC)
    SET (HDF4_WARNINGS_BLOCKED 1)
    STRING(REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " "
      CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
  ENDIF (MSVC)
  IF(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  ENDIF()
  # Borland uses -w- to suppress warnings.
  IF (BORLAND)
    SET (HDF4_WARNINGS_BLOCKED 1)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w-")
  ENDIF (BORLAND)

  # Most compilers use -w to suppress warnings.
  IF (NOT HDF4_WARNINGS_BLOCKED)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
  ENDIF (NOT HDF4_WARNINGS_BLOCKED)
ENDIF (HDF4_DISABLE_COMPILER_WARNINGS)

#-----------------------------------------------------------------------------
# This is in here to help some of the GCC based IDES like Eclipse 
# and code blocks parse the compiler errors and warnings better. 
#-----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fomit-frame-pointer")
  include_directories(/usr/include/rpc)
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (CMAKE_COMPILER_IS_GNUCXX)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  include_directories(/usr/include/rpc)
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

#-----------------------------------------------------------------------------
# Option for LibJpeg support
#-----------------------------------------------------------------------------
OPTION(HDF4_ENABLE_JPEG_LIB_SUPPORT "Enable libjpeg" OFF)  
IF (HDF4_ENABLE_JPEG_LIB_SUPPORT)
  IF (NOT H4_JPEGLIB_HEADER)
    FIND_PACKAGE(JPEG REQUIRED)
    IF (JPEG_FOUND)
      SET (H4_HAVE_JPEGLIB_H 1)
      SET (H4_HAVE_LIBJPEG 1)
      SET (H4_JPEGLIB_HEADER "jpeglib.h")
    ELSE (JPEG_FOUND)
      MESSAGE(FATAL " JPEGLib is Required for JPEGLib support in HDF4")
    ENDIF (JPEG_FOUND)
  ELSE (NOT H4_JPEGLIB_HEADER)
    # This project is being called from within another and JPEGLib is already configured
    SET (H4_HAVE_JPEGLIB_H 1)
  ENDIF (NOT H4_JPEGLIB_HEADER)
  SET (LINK_LIBS ${LINK_LIBS} ${JPEG_LIBRARIES} )
  include_directories(${JPEG_INCLUDE_DIRS})
  message(STATUS "JPEGLIB is ON")
ENDIF (HDF4_ENABLE_JPEG_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
OPTION(HDF4_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" OFF)  
IF (HDF4_ENABLE_Z_LIB_SUPPORT)
  IF (NOT H4_ZLIB_HEADER)
    FIND_PACKAGE(ZLIB REQUIRED)
    IF (ZLIB_FOUND)
      SET (H4_HAVE_FILTER_DEFLATE 1)
      SET (H4_HAVE_ZLIB_H 1)
      SET (H4_HAVE_LIBZ 1)
      SET (H4_ZLIB_HEADER "zlib.h")
    ELSE (ZLIB_FOUND)
      MESSAGE(FATAL " ZLib is Required for ZLib support in HDF4")
    ENDIF (ZLIB_FOUND)
  ELSE (NOT H4_ZLIB_HEADER)
    # This project is being called from within another and ZLib is already configured
    SET (H4_HAVE_FILTER_DEFLATE 1)
    SET (H4_HAVE_ZLIB_H 1)
    SET (H4_HAVE_LIBZ 1)
  ENDIF (NOT H4_ZLIB_HEADER)
  CONFIGURE_FILE(${HDF4_SOURCE_DIR}/Resources/hdf4_zlib.h.in 
                 ${HDF4_BINARY_DIR}/hdf4_zlib.h @ONLY IMMEDIATE)
  SET (LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES} )
  include_directories(${ZLIB_INCLUDE_DIRS})
  message(STATUS "Filter ZLIB is ON")
ENDIF (HDF4_ENABLE_Z_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
SET (CMAKE_MODULE_PATH ${HDF4_SOURCE_DIR}/Resources/CMake ${CMAKE_MODULE_PATH})
OPTION(HDF4_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)  
IF (HDF4_ENABLE_SZIP_SUPPORT)
  find_package(SZIP)
  if (SZIP_FOUND)
    SET (H4_HAVE_FILTER_SZIP 1)
    SET (H4_HAVE_SZLIB_H 1)
    SET (H4_HAVE_LIBSZ 1)
    SET (LINK_LIBS ${LINK_LIBS} ${SZIP_LIBRARIES} )
    include_directories(${SZIP_INCLUDE_DIRS})
    message(STATUS "Filter SZIP is ON")
    OPTION(HDF4_ENABLE_SZIP_ENCODING "Use SZip Encoding" OFF)  
    IF (HDF4_ENABLE_SZIP_ENCODING)
      SET (H4_HAVE_SZIP_ENCODER 1)
    ENDIF ()
  ENDIF ()
ENDIF ()

#-----------------------------------------------------------------------------
# Add the HDF4 Library Target to the build
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/hdf      hdf)
ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf    mfhdf)

#-----------------------------------------------------------------------------
# Option to build HDF4 xdr Library
#-----------------------------------------------------------------------------
OPTION(HDF4_BUILD_XDR_LIB  "Build HDF4 XDR Library" ON)
IF (HDF4_BUILD_XDR_LIB)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/xdr ${PROJECT_BINARY_DIR}/mfhdf/xdr)
ENDIF ( HDF4_BUILD_XDR_LIB )

#-----------------------------------------------------------------------------
# Option to build HDF4 C++ Library
#-----------------------------------------------------------------------------
OPTION(HDF4_BUILD_CPP_LIB  "Build HDF4 C++ Library" OFF)
IF (HDF4_BUILD_CPP_LIB)
  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/c++ ${PROJECT_BINARY_DIR}/mfhdf/c++)
ENDIF ( HDF4_BUILD_CPP_LIB )

#-----------------------------------------------------------------------------
# Option to build Fortran bindings/tests/examples
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
#-----------------------------------------------------------------------------
OPTION(HDF4_BUILD_FORTRAN "Build FORTRAN support" OFF)
IF (HDF4_BUILD_FORTRAN)
  # Set default name mangling : overridden by Fortran detection in fortran dir
  SET(H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
  SET(H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## _")
#  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/hdf/fortran ${PROJECT_BINARY_DIR}/hdf/fortran)
#  ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/mfhdf/fortran ${PROJECT_BINARY_DIR}/mfhdf/fortran)
ELSE ( HDF4_BUILD_FORTRAN )
  SET(H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
  SET(H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## _")
ENDIF ( HDF4_BUILD_FORTRAN )

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
OPTION(HDF4_BUILD_TOOLS  "Build HDF4 Tools" OFF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
OPTION(HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)

#-----------------------------------------------------------------------------
# Generate the H4config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
#CONFIGURE_FILE(${HDF4_SOURCE_DIR}/Resources/h4config.h.in
#  ${HDF4_BINARY_DIR}/h4config.h     @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Generate the netcdf.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
IF(WIN32)
  SET(H4_WINDOWS_INCLUDE_FILE "#include <hdfi.h>")
  SET(NETCDF_EXTERN HDFLIBAPI)
ELSE(WIN32)
  SET(NETCDF_EXTERN extern)
ENDIF()
SET(NC_OLD_FILLVALUES 0)
#CONFIGURE_FILE(${HDF4_SOURCE_DIR}/Resources/netcdf.h.in
#  ${HDF4_BINARY_DIR}/netcdf.h     @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
OPTION(BUILD_TESTING "Build HDF4 Unit Testing" OFF)
IF (BUILD_TESTING)
  SET (DART_TESTING_TIMEOUT 1200 CACHE INTEGER  
    "Timeout in seconds for each test (default 1200=20minutes)")
  ENABLE_TESTING()
  INCLUDE(CTest)
  IF(NOT HDF4_EXTERNALLY_CONFIGURED)
    ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/hdf/test      ${PROJECT_BINARY_DIR}/hdf/test)
  ENDIF(NOT HDF4_EXTERNALLY_CONFIGURED)
  include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
ENDIF ( BUILD_TESTING )

#-----------------------------------------------------------------------------
# Rules for Installation using make Install target
#-----------------------------------------------------------------------------
INSTALL (
  FILES 
    ${PROJECT_BINARY_DIR}/H4config.h    
#    ${H4_PUBLIC_HEADERS}
#    ${H4_PRIVATE_HEADERS}
  DESTINATION 
    include 
)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
IF (HDF4_EXPORTED_TARGETS)
#    INSTALL(
#      TARGETS 
#        ${HDF4_LIB_NAME}
#      EXPORT 
#        ${HDF4_EXPORTED_TARGETS}
#      LIBRARY DESTINATION lib 
#      ARCHIVE DESTINATION lib
#      RUNTIME DESTINATION bin
#    )
ENDIF (HDF4_EXPORTED_TARGETS)

#-----------------------------------------------------------------------------
# Export Compilation Targets via Make Install to final location
#-----------------------------------------------------------------------------
IF (HDF4_EXPORTED_TARGETS)
#  INSTALL(
#    EXPORT 
#      ${HDF4_EXPORTED_TARGETS}
#    DESTINATION 
#      lib
#    FILE 
#      HDF4-config.cmake 
#  )
ENDIF (HDF4_EXPORTED_TARGETS)
  
#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
EXPORT(
  TARGETS    
    ${HDF4_LIBRARIES_TO_EXPORT} ${HDF4_TARGET_DEPENDENCIES}
  FILE 
    HDF4-config.cmake 
)
