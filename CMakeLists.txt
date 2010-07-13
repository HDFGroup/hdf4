cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4 C CXX)

#-----------------------------------------------------------------------------
# Instructions for use : Normal Build
#
# For standard build of HDF4 libraries,tests and tools. 
# Run cmake using the HDF4 source tree to generate a build tree.
# Enable/Disable options according to requirements and
# set CMAKE_INSTALL_PREFIX to the required install path.
# Make install can be used to install all components for system-wide use.
#
#-----------------------------------------------------------------------------
# Instructions for use : Sub-Project Build
#
# To include HDF4 as a sub-project within another project. 
# Set HDF4_EXTERNALLY_CONFIGURED to 1 in the parent project and
# supply values for the following variables...
#
# HDF4_EXPORTED_TARGETS :
#   Set this to the name of the targets variable which controls exports
#   If unset (because parent project does not support/use the 
#   INSTALL (EXPORT target...) syntax), then targets are not configured 
#   for export during install.
#
# HDF4_LIB_DEPENDENCIES :
#   If the build of HDF4 libs is being customized, then rules for the
#   dependencies of the HDF4 libs may be 'incomplete', add additional
#   dependencies to this variable so that external projects pick them up
#
# HDF4_EXTERNAL_LIB_PREFIX :
#   If the parent project needs to install hdf libraries, but avoid 
#   name conflicts with system versions, then a prefix may be added
#   to ensure that the correct versions configured are used.
#
# Consider this example, it builds its own zlib
# library and tells HDF4 to add it as a dependency - this ensures that
# any project making use of this build of HDF4 will use the correct zlib
#
#   # Tell hdf4 that we are manually overriding certain settings
#   SET (HDF4_EXTERNALLY_CONFIGURED 1)
#   # Avoid duplicating names of installed libraries
#   SET (HDF4_EXTERNAL_LIB_PREFIX "prj")
#   # Targets linking to the HDF4 libs need to know their names
#   # if they are changed in the sub project, they should be here too
#   SET (HDF4_LIB_NAME "prjhdf4")
#   SET (HDF4_MF_LIB_NAME "prjhdf4_mf")
#   SET (HDF4_LIBRARY "${HDF4_LIB_NAME};${HDF4_MF_LIB_NAME}")
#   # Export configuration to this export variable
#   SET (HDF4_EXPORTED_TARGETS "project-targets")
#
#   # Setup all necessary overrides for zlib so that HDF4 uses our
#   # internally compiled zlib rather than any other version
#   IF (HDF4_ENABLE_Z_LIB_SUPPORT)
#    # We must tell the main HDF4 library that it depends on our zlib 
#     SET (HDF4_LIB_DEPENDENCIES prjzlib)    
#     # Override the zlib header file
#     IF (PRJ_USE_SYSTEM_ZLIB)
#       SET (H4_ZLIB_HEADER "zlib.h")
#     ELSE (PRJ_USE_SYSTEM_ZLIB)
#      SET (H4_ZLIB_HEADER "prj_zlib.h")
#       # Set vars that FindZlib would have set if used in sub project
#       SET (ZLIB_INCLUDE_DIRS "${PRJ_ZLIB_INCLUDE_DIRS}")
#       SET (ZLIB_LIBRARIES prjzlib)
#     ENDIF (PRJ_USE_SYSTEM_ZLIB)
#  ENDIF (HDF4_ENABLE_Z_LIB_SUPPORT)
#   
#   # Add the sub project
#   ADD_SUBDIRECTORY (Utilities/hdf4.2.5)
#   # Add the HDF4 dirs to our include path
#   SET (HDF4_INCLUDE_DIR 
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.2.5/hdf/src
#       ${PROJECT_BINARY_DIR}/Utilities/hdf4.2.5
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.2.5/mfhdf/libsrc
#   )
#
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Basic HDF4 stuff here
#-----------------------------------------------------------------------------
SET (HDF4_PACKAGE "hdf4")
SET (HDF4_PACKAGE_NAME "HDF")
SET (HDF4_PACKAGE_VERSION "${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}")
SET (HDF4_PACKAGE_VERSION_MAJOR "${H4_VERS_MAJOR}.${H4_VERS_MINOR}")
SET (HDF4_PACKAGE_VERSION_MINOR "${H4_VERS_RELEASE}")
SET (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION}-${H4_VERS_SUBRELEASE}")
SET (HDF4_PACKAGE_TARNAME "hdf")
SET (HDF4_PACKAGE_URL "http://www.hdfgroup.org")
SET (HDF4_PACKAGE_BUGREPORT "help@hdfgroup.org")

#-----------------------------------------------------------------------------
# Set the core names of all the libraries
#-----------------------------------------------------------------------------
SET (HDF4_LIB_CORENAME              "hdf4")
SET (HDF4_SRC_LIB_CORENAME          "hdf")
SET (HDF4_SRC_FCSTUB_LIB_CORENAME   "hdf_fcstub")
SET (HDF4_SRC_FORTRAN_LIB_CORENAME  "hdf_fortran")
SET (HDF4_MF_LIB_CORENAME           "mfhdf")
SET (HDF4_MF_XDR_LIB_CORENAME       "xdr")
SET (HDF4_HDF_TEST_LIB_CORENAME     "hdf_test")
SET (HDF4_MF_FCSTUB_LIB_CORENAME    "mfhdf_fcstub")
SET (HDF4_MF_FORTRAN_LIB_CORENAME   "mfhdf_fortran")
SET (HDF4_MF_TEST_LIB_CORENAME      "mf_test")
SET (HDF4_TOOLS_LIB_CORENAME        "h4tools")

#-----------------------------------------------------------------------------
# Set the true names of all the libraries if customized by external project
#-----------------------------------------------------------------------------
SET (HDF4_LIB_NAME              "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_CORENAME}")
SET (HDF4_SRC_LIB_NAME          "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_LIB_CORENAME}")
SET (HDF4_SRC_FCSTUB_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_FCSTUB_LIB_CORENAME}")
SET (HDF4_SRC_FORTRAN_LIB_NAME  "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_FORTRAN_LIB_CORENAME}")
SET (HDF4_MF_LIB_NAME           "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_LIB_CORENAME}")
SET (HDF4_MF_XDR_LIB_NAME       "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_XDR_LIB_CORENAME}")
SET (HDF4_HDF_TEST_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_HDF_LIB_CORENAME}")
SET (HDF4_MF_FCSTUB_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FCSTUB_LIB_CORENAME}")
SET (HDF4_MF_FORTRAN_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FORTRAN_LIB_CORENAME}")
SET (HDF4_MF_TEST_LIB_NAME      "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_TEST_LIB_CORENAME}")
SET (HDF4_TOOLS_LIB_NAME        "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_TOOLS_LIB_CORENAME}")

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
SET (HDF4_RESOURCES_DIR       ${HDF4_SOURCE_DIR}/config/cmake)
SET (HDF4_HDF_SOURCE_DIR      ${HDF4_SOURCE_DIR}/hdf/src)
SET (HDF4_HDF_TEST_SOURCE_DIR ${HDF4_SOURCE_DIR}/hdf/test)
SET (HDF4_MFHDF_SOURCE_DIR    ${HDF4_SOURCE_DIR}/mfhdf/libsrc)
SET (HDF4_MFHDF_TEST_DIR      ${HDF4_SOURCE_DIR}/mfhdf/test)
SET (HDF4_MFHDF_FORTRAN_DIR   ${HDF4_SOURCE_DIR}/mfhdf/fortran)
SET (HDF4_MFHDF_XDR_DIR       ${HDF4_SOURCE_DIR}/mfhdf/xdr)

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
INCLUDE (${HDF4_RESOURCES_DIR}/HDFMacros.cmake)
INCLUDE (${HDF4_RESOURCES_DIR}/HDF4Macros.cmake)

#-----------------------------------------------------------------------------
# Setup output Directories
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
  SET (CMAKE_Fortran_MODULE_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin/fortran CACHE PATH "Single Directory for all fortran modules."
  )
ELSE (NOT HDF4_EXTERNALLY_CONFIGURED)
  # if we are externally configured, but the project uses old cmake scripts
  # this may not be set and utilities like Hdetect will fail
  IF (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
  ENDIF (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects using FindHDF4.
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXPORTED_TARGETS)
  SET (HDF4_EXPORTED_TARGETS "hdf4-targets")
ENDIF (NOT HDF4_EXPORTED_TARGETS)

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME,
# add it to this variable. This is NOT used by Make Install, but for projects
# which include hdf4 as a sub-project within their build tree
#-----------------------------------------------------------------------------
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
INCLUDE (${HDF4_RESOURCES_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Option to use legacy naming for windows libs/programs, default is legacy
#-----------------------------------------------------------------------------
IF (WIN32)
  OPTION (HDF_LEGACY_NAMING "Use Legacy Names for Libraries and Programs" ON)
ENDIF (WIN32)

#-----------------------------------------------------------------------------
# Option to Build Shared/Static libs, default is static
#-----------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  SET (H4_BUILT_AS_DYNAMIC_LIB 1)
  IF (MSVC)
    SET (CMAKE_MFC_FLAG 0)
    FOREACH (flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      IF (${flag_var} MATCHES "/MT")
        STRING (REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
      ENDIF (${flag_var} MATCHES "/MT")
    ENDFOREACH (flag_var)
  ENDIF (MSVC)
ELSE (BUILD_SHARED_LIBS)
  IF (NOT WIN32)
    # should this be a user setting : Everyone uses it anyway ?
    ADD_DEFINITIONS (-DPIC)
  ENDIF (NOT WIN32)
  IF (MSVC)
    SET (CMAKE_MFC_FLAG 0)
    FOREACH (flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      IF (${flag_var} MATCHES "/MD")
        STRING (REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      ENDIF (${flag_var} MATCHES "/MD")
    ENDFOREACH (flag_var)
  ENDIF (MSVC)
ENDIF (BUILD_SHARED_LIBS)

ADD_DEFINITIONS (-DBIG_LONGS -DSWAP)
ADD_DEFINITIONS (-DHAVE_CONFIG_H)

#-----------------------------------------------------------------------------
# Option to use code coverage
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_COVERAGE "Enable code coverage for Libraries and Programs" OFF)
IF (HDF4_ENABLE_COVERAGE)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    SET (LDFLAGS "${LDFLAGS} -fprofile-arcs -ftest-coverage")   
ENDIF (HDF4_ENABLE_COVERAGE)

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
SET (HDF4_LIB_TARGET              ${HDF4_LIB_CORENAME})
SET (HDF4_SRC_LIB_TARGET          ${HDF4_SRC_LIB_CORENAME})
SET (HDF4_SRC_FCSTUB_LIB_TARGET   ${HDF4_SRC_FCSTUB_LIB_CORENAME})
SET (HDF4_SRC_FORTRAN_LIB_TARGET  ${HDF4_SRC_FORTRAN_LIB_CORENAME})
SET (HDF4_MF_LIB_TARGET           ${HDF4_MF_LIB_CORENAME})
SET (HDF4_MF_XDR_LIB_TARGET       ${HDF4_MF_XDR_LIB_CORENAME})
SET (HDF4_HDF_TEST_LIB_TARGET     ${HDF4_HDF_LIB_CORENAME})
SET (HDF4_MF_FCSTUB_LIB_TARGET    ${HDF4_MF_FCSTUB_LIB_CORENAME})
SET (HDF4_MF_FORTRAN_LIB_TARGET   ${HDF4_MF_FORTRAN_LIB_CORENAME})
SET (HDF4_MF_TEST_LIB_TARGET      ${HDF4_MF_TEST_LIB_CORENAME})
SET (HDF4_TOOLS_LIB_TARGET        ${HDF4_TOOLS_LIB_CORENAME})

#-----------------------------------------------------------------------------
# Include the main src and config directories
#-----------------------------------------------------------------------------
SET (HDF4_INCLUDE_DIRECTORIES
    ${HDF4_HDF_SOURCE_DIR} 
    ${HDF4_MFHDF_SOURCE_DIR} 
    ${HDF4_SOURCE_DIR} 
    ${HDF4_BINARY_DIR}
)
INCLUDE_DIRECTORIES (${HDF4_INCLUDE_DIRECTORIES} )

#-----------------------------------------------------------------------------
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
SET (EXE_EXT "")
IF (WIN32)
  SET (EXE_EXT ".exe")
  ADD_DEFINITIONS (-DBIND_TO_CURRENT_VCLIBS_VERSION=1)
  ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
ENDIF (WIN32)

SET (MAKE_SYSTEM)
IF (CMAKE_BUILD_TOOL MATCHES "make")
  SET (MAKE_SYSTEM 1)
ENDIF (CMAKE_BUILD_TOOL MATCHES "make")

SET (CFG_INIT "/${CMAKE_CFG_INTDIR}")
IF (MAKE_SYSTEM)
  SET (CFG_INIT "")
ENDIF (MAKE_SYSTEM)

#-----------------------------------------------------------------------------
# Add some definitions for Debug Builds
#-----------------------------------------------------------------------------
IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DDEBUG)
  #-- NMake Makefiles will overwhelm the console with warnings if -Wall is used.
  IF (NOT WIN32)
    ADD_DEFINITIONS (-Wall)
  ENDIF (NOT WIN32)
ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS (-UH4_DEBUG_API -DNDEBUG)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

#-----------------------------------------------------------------------------
# Option to allow the user to disable compiler warnings
#-----------------------------------------------------------------------------
OPTION (HDF4_DISABLE_COMPILER_WARNINGS "Disable compiler warnings" OFF)
IF (HDF4_DISABLE_COMPILER_WARNINGS)
  # MSVC uses /w to suppress warnings.  It also complains if another
  # warning level is given, so remove it.
  IF (MSVC)
    SET (HDF4_WARNINGS_BLOCKED 1)
    STRING (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " "
        CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
  ENDIF (MSVC)
  IF (WIN32)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
  ENDIF (WIN32)
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
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (CMAKE_COMPILER_IS_GNUCXX)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

#-----------------------------------------------------------------------------
# parse the full version number from hfile.h and include in H4_VERS_INFO
#-----------------------------------------------------------------------------
FILE (READ ${HDF4_HDF_SOURCE_DIR}/hfile.h _hfile_h_contents)
STRING (REGEX REPLACE ".*#define[ \t]+LIBVER_MAJOR[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_MAJOR ${_hfile_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+LIBVER_MINOR[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_MINOR ${_hfile_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+LIBVER_RELEASE[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_RELEASE ${_hfile_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+LIBVER_SUBRELEASE[ \t]+\"([0-9A-Za-z.]*)\".*$"
    "\\1" H4_VERS_SUBRELEASE ${_hfile_h_contents})
#MESSAGE (STATUS "VERSION: ${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}-${H4_VERS_SUBRELEASE}")

#-----------------------------------------------------------------------------
# Option for LibJpeg support
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_JPEG_LIB_SUPPORT "Enable libjpeg" OFF)
IF (HDF4_ENABLE_JPEG_LIB_SUPPORT)
  IF (NOT H4_JPEGLIB_HEADER)
    FIND_PACKAGE (JPEG REQUIRED)
    IF (JPEG_FOUND)
      SET (H4_HAVE_JPEGLIB_H 1)
      SET (H4_HAVE_LIBJPEG 1)
      SET (H4_JPEGLIB_HEADER "jpeglib.h")
    ELSE (JPEG_FOUND)
      MESSAGE (FATAL " JPEGLib is Required for JPEGLib support in HDF4")
    ENDIF (JPEG_FOUND)
  ELSE (NOT H4_JPEGLIB_HEADER)
    # This project is being called from within another and JPEGLib is already configured
    SET (H4_HAVE_JPEGLIB_H 1)
  ENDIF (NOT H4_JPEGLIB_HEADER)
  SET (LINK_LIBS ${LINK_LIBS} ${JPEG_LIBRARIES})
  INCLUDE_DIRECTORIES (${JPEG_INCLUDE_DIRS})
  MESSAGE (STATUS "JPEGLIB is ON")
ENDIF (HDF4_ENABLE_JPEG_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" OFF)
IF (HDF4_ENABLE_Z_LIB_SUPPORT)
  IF (NOT H4_ZLIB_HEADER)
    FIND_PACKAGE (ZLIB REQUIRED)
    IF (ZLIB_FOUND)
      SET (H4_HAVE_FILTER_DEFLATE 1)
      SET (H4_HAVE_ZLIB_H 1)
      SET (H4_HAVE_LIBZ 1)
      SET (H4_ZLIB_HEADER "zlib.h")
    ELSE (ZLIB_FOUND)
      MESSAGE (FATAL " ZLib is Required for ZLib support in HDF4")
    ENDIF (ZLIB_FOUND)
  ELSE (NOT H4_ZLIB_HEADER)
    # This project is being called from within another and ZLib is already configured
    SET (H4_HAVE_FILTER_DEFLATE 1)
    SET (H4_HAVE_ZLIB_H 1)
    SET (H4_HAVE_LIBZ 1)
  ENDIF (NOT H4_ZLIB_HEADER)
  CONFIGURE_FILE (${HDF4_RESOURCES_DIR}/hdf4_zlib.h.in ${HDF4_BINARY_DIR}/hdf4_zlib.h @ONLY IMMEDIATE)
  SET (LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES})
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  MESSAGE (STATUS "Filter ZLIB is ON")
ENDIF (HDF4_ENABLE_Z_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
SET (CMAKE_MODULE_PATH ${HDF4_RESOURCES_DIR} ${CMAKE_MODULE_PATH})
OPTION (HDF4_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)
IF (HDF4_ENABLE_SZIP_SUPPORT)
  FIND_PACKAGE (SZIP REQUIRED)
  IF (SZIP_FOUND)
    SET (H4_HAVE_FILTER_SZIP 1)
    SET (H4_HAVE_SZLIB_H 1)
    SET (H4_HAVE_LIBSZ 1)
    SET (LINK_LIBS ${LINK_LIBS} ${SZIP_LIBRARIES})
    INCLUDE_DIRECTORIES (${SZIP_INCLUDE_DIRS})
#      MESSAGE (STATUS "SZIP_INCLUDE_DIRS: ${SZIP_INCLUDE_DIRS}")
#      MESSAGE (STATUS "SZIP_INCLUDE_DIR: ${SZIP_INCLUDE_DIR}")
    MESSAGE (STATUS "Filter SZIP is ON")
    OPTION (HDF4_ENABLE_SZIP_ENCODING "Use SZip Encoding" OFF)
    IF (HDF4_ENABLE_SZIP_ENCODING)
      SET (H4_HAVE_SZIP_ENCODER 1)
    ENDIF (HDF4_ENABLE_SZIP_ENCODING)
  ELSE (SZIP_FOUND)
      MESSAGE (FATAL_ERROR "SZIP is Required for SZIP support in HDF4")
  ENDIF (SZIP_FOUND)
ENDIF (HDF4_ENABLE_SZIP_SUPPORT)

#-----------------------------------------------------------------------------
# Build utility to copy and strip X lines of file
#-----------------------------------------------------------------------------
IF (HDF4_BUILD_TOOLS AND BUILD_TESTING)
  SET (XLATE_UTILITY "xlatefile")
  ADD_EXECUTABLE(${XLATE_UTILITY} ${HDF4_RESOURCES_DIR}/xlatefile.c)
ENDIF (HDF4_BUILD_TOOLS AND BUILD_TESTING)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
OPTION (BUILD_TESTING "Build HDF4 Unit Testing" OFF)
IF (BUILD_TESTING)
  SET (DART_TESTING_TIMEOUT 1200
      CACHE INTEGER
      "Timeout in seconds for each test (default 1200=20minutes)"
  )
  ENABLE_TESTING ()
  INCLUDE (CTest)
  IF (NOT HDF4_EXTERNALLY_CONFIGURED)
    ADD_SUBDIRECTORY(${HDF4_SOURCE_DIR}/hdf/test ${PROJECT_BINARY_DIR}/test)
  ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)
  INCLUDE (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Option to build Fortran bindings/tests/examples
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_FORTRAN "Build FORTRAN support" OFF)
IF (HDF4_BUILD_FORTRAN)
  # Set default name mangling : overridden by Fortran detection in fortran dir
  SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
  SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## _")
ELSE (HDF4_BUILD_FORTRAN )
  SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
  SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## _")
ENDIF (HDF4_BUILD_FORTRAN)

IF (MSVC)
  SET (H4_HDSETVBUF "HDsetvbuf(F,S,M,Z) (((Z)>1)?setvbuf(F,S,M,Z):setvbuf(F,S,M,2))")
ELSE (MSVC)
  SET (H4_HDSETVBUF "NOP")
ENDIF (MSVC)

#-----------------------------------------------------------------------------
# Generate the H4config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
CONFIGURE_FILE (${HDF4_RESOURCES_DIR}/h4config.h.in ${HDF4_BINARY_DIR}/h4config.h)

#-----------------------------------------------------------------------------
# Add the HDF4 Library Target to the build
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf ${PROJECT_BINARY_DIR}/hdf)
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf ${PROJECT_BINARY_DIR}/mfhdf)

#-----------------------------------------------------------------------------
# Option to build HDF4 xdr Library
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_XDR_LIB  "Build HDF4 XDR Library" ON)
IF (HDF4_BUILD_XDR_LIB)
  ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/xdr ${PROJECT_BINARY_DIR}/xdr)
ENDIF (HDF4_BUILD_XDR_LIB)

#-----------------------------------------------------------------------------
# Option to build HDF4 Tools
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_TOOLS  "Build HDF4 Tools" OFF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Utilities
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_UTILS  "Build HDF4 Utilities" OFF)

#-----------------------------------------------------------------------------
# Option to build HDF4 Examples
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_EXAMPLES  "Build HDF4 Examples" OFF)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${PROJECT_BINARY_DIR}/H4config.h
    DESTINATION
        include
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
INSTALL (
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    DESTINATION
        lib
    FILE
        HDF4-targets.cmake
)

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
EXPORT (
    TARGETS
        ${HDF4_LIBRARIES_TO_EXPORT} ${HDF4_LIB_DEPENDENCIES}
    FILE
        HDF4-targets.cmake
)

#-----------------------------------------------------------------------------
# Configure the HDF4-config.cmake file for the build directory
#-----------------------------------------------------------------------------
SET (HDF4_INCLUDES_BUILD_TIME
    ${HDF4_HDF_SOURCE_DIR}
    ${HDF4_MFHDF_SOURCE_DIR}
    ${HDF4_MFHDF_XDR_DIR}
    ${HDF4_BINARY_DIR}
)
SET (HDF4_VERSION_STRING @HDF4_PACKAGE_VERSION@)
SET (HDF4_VERSION_MAJOR  @HDF4_PACKAGE_VERSION_MAJOR@)
SET (HDF4_VERSION_MINOR  @HDF4_PACKAGE_VERSION_MINOR@)

CONFIGURE_FILE (
    ${HDF4_RESOURCES_DIR}/HDF4-config.cmake.build.in 
    ${HDF4_BINARY_DIR}/HDF4-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the HDF5-config.cmake file for the install directory
#-----------------------------------------------------------------------------
INSTALL (CODE
    "SET (HDF4_INSTALL_SKIP_TARGETS 1)
     INCLUDE (${HDF4_BINARY_DIR}/HDF4-config.cmake) 
     CONFIGURE_FILE (
         \"${HDF4_RESOURCES_DIR}/HDF4-config.cmake.install.in\" 
         \"${CMAKE_INSTALL_PREFIX}/lib/HDF4-config.cmake\" @ONLY
     )
    "
)

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
SET (CPACK_PACKAGE_VENDOR "The HDF Group")
SET (CPACK_PACKAGE_NAME "${HDF4_PACKAGE_NAME}")
SET (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION}")
SET (CPACK_PACKAGE_VERSION_MAJOR "${HDF4_PACKAGE_VERSION_MAJOR}")
SET (CPACK_PACKAGE_VERSION_MINOR "${HDF4_PACKAGE_VERSION_MINOR}")
SET (CPACK_PACKAGE_VERSION_PATCH "")
SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

IF (WIN32)
  SET (CPACK_NSIS_PACKAGE_NAME "HDF4 ${HDF4_PACKAGE_VERSION}")
ENDIF (WIN32)

INCLUDE (CPack)

#-----------------------------------------------------------------------------
# Now list the cpack commands
#-----------------------------------------------------------------------------
CPACK_ADD_COMPONENT (applications 
    DISPLAY_NAME "HDF4 Applications" 
    DEPENDS libraries
    GROUP Applications
)
CPACK_ADD_COMPONENT (libraries 
    DISPLAY_NAME "HDF4 Libraries"
    GROUP Runtime
)
CPACK_ADD_COMPONENT (headers 
    DISPLAY_NAME "HDF4 Headers" 
    DEPENDS libraries
    GROUP Development
)

IF (HDF4_BUILD_FORTRAN)
  CPACK_ADD_COMPONENT (fortlibraries 
      DISPLAY_NAME "HDF4 Fortran Libraries" 
      DEPENDS libraries
      GROUP Runtime
  )
ENDIF (HDF4_BUILD_FORTRAN)

IF (HDF4_BUILD_TOOLS)
  CPACK_ADD_COMPONENT (toolsapplications 
      DISPLAY_NAME "HDF4 Tools Applications" 
      DEPENDS toolslibraries
      GROUP Applications
  )
  CPACK_ADD_COMPONENT (toolslibraries 
      DISPLAY_NAME "HDF4 Tools Libraries" 
      DEPENDS libraries
      GROUP Runtime
  )
  CPACK_ADD_COMPONENT (toolsheaders 
      DISPLAY_NAME "HDF4 Tools Headers" 
      DEPENDS toolslibraries
      GROUP Development
  )
ENDIF (HDF4_BUILD_TOOLS)
