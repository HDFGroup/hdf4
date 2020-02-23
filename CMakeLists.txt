cmake_minimum_required (VERSION 3.10)
project (HDF4 C)

if (POLICY CMP0074)
  cmake_policy (SET CMP0074 NEW)
endif ()

#-----------------------------------------------------------------------------
# Instructions for use : Normal Build
#
# For standard build of HDF4 libraries,tests and tools.
# Run cmake using the HDF4 source tree to generate a build tree.
# Enable/Disable options according to requirements and
# set CMAKE_INSTALL_PREFIX to the required install path.
# Make install can be used to install all components for system-wide use.
#
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message (FATAL_ERROR "\nERROR! ${PROJECT_NAME} DOES NOT SUPPORT IN SOURCE BUILDS!\n"
      "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
      " == CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}\n"
      "NEXT STEPS:\n"
      "(1) Delete the CMakeCache.txt file and the CMakeFiles/ directory\n"
      "     under the source directory for ${PROJECT_NAME}, otherwise you\n"
      "     will not be able to configure ${PROJECT_NAME} correctly!\n"
      "      * For example, on linux machines do:\n"
      "        $ rm -r CMakeCache.txt CMakeFiles/\n"
      "(2) Create a different directory and configure ${PROJECT_NAME} in that directory.\n"
      "      * For example, on linux machines do:\n"
      "        $ mkdir MY_BUILD\n"
      "        $ cd MY_BUILD\n"
      "        $ cmake [OPTIONS] ..\n"
      )
endif ()

# CMake version 3.14 added option --ignore-eol to compare files
#     cmake -E compare_files --ignore-eol file1 file2
#if(CMAKE_VERSION VERSION_LESS "3.14.0" AND WIN32)
#    MESSAGE(FATAL_ERROR "Windows builds requires a minimum of CMake 3.14")
#endif()

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
#   install (EXPORT target...) syntax), then targets are not configured
#   for export during install.
#
# HDF4_LIB_DEPENDENCIES :
#   If the build of HDF4 libs is being customized, then rules for the
#   dependencies of the HDF4 libs may be 'incomplete', add additional
#   dependencies to this variable so that external projects pick them up
#
#option (HDF4_EXTERNAL_LIB_PREFIX "Use prefix for custom library naming." "")
set (HDF4_EXTERNAL_LIB_PREFIX "" CACHE STRING "Use prefix for custom library naming.")
mark_as_advanced (HDF4_EXTERNAL_LIB_PREFIX)
# HDF4_EXTERNAL_LIB_PREFIX :
#   If the parent project needs to install hdf libraries, but avoid
#   name conflicts with system versions, then a prefix may be added
#   to ensure that the correct versions configured are used.
#
# HDF4_INSTALL_BIN_DIR, HDF4_INSTALL_LIB_DIR, HDF4_INSTALL_INCLUDE_DIR, HDF4_INSTALL_DATA_DIR :
#   Customize the 'bin', 'lib', 'include', and 'share' installation directories.
#
# HDF4_INSTALL_NO_DEVELOPMENT :
#   Set to true to skip installation of headers and CMake package files.
#
# Consider this example, it builds its own zlib
# library and tells HDF4 to add it as a dependency - this ensures that
# any project making use of this build of HDF4 will use the correct zlib
#
#   # Tell hdf4 that we are manually overriding certain settings
#   set (HDF4_EXTERNALLY_CONFIGURED 1)
#   # Avoid duplicating names of installed libraries
#   set (HDF4_EXTERNAL_LIB_PREFIX "prj")
#   # Targets linking to the HDF4 libs need to know their names
#   # if they are changed in the sub project, they should be here too
#   set (HDF4_LIB_NAME "prjhdf4")
#   set (HDF4_MF_LIB_NAME "prjhdf4_mf")
#   set (HDF4_LIBRARY "${HDF4_LIB_NAME};${HDF4_MF_LIB_NAME}")
#   # Export configuration to this export variable
#   set (HDF4_EXPORTED_TARGETS "project-targets")
#
#   # Setup all necessary overrides for zlib so that HDF4 uses our
#   # internally compiled zlib rather than any other version
#   if (HDF4_ENABLE_Z_LIB_SUPPORT)
#    # We must tell the main HDF4 library that it depends on our zlib
#     set (HDF4_LIB_DEPENDENCIES prjzlib)
#     # Override the zlib header file
#     if (PRJ_USE_SYSTEM_ZLIB)
#       set (H4_ZLIB_HEADER "zlib.h")
#     else ()
#      set (H4_ZLIB_HEADER "prj_zlib.h")
#       # Set vars that FindZlib would have set if used in sub project
#       set (ZLIB_INCLUDE_DIRS "${PRJ_ZLIB_INCLUDE_DIRS}")
#       set (ZLIB_LIBRARIES prjzlib)
#     endif ()
#  endif ()
#
#   # Add the sub project
#   add_subdirectory (Utilities/hdf4.2.5)
#   # Add the HDF4 dirs to our include path
#   set (HDF4_INCLUDE_DIR
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.2.5/hdf/src
#       ${PROJECT_BINARY_DIR}/Utilities/hdf4.2.5
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.2.5/mfhdf/libsrc
#   )
#
#-----------------------------------------------------------------------------
string (TIMESTAMP CONFIG_DATE "%Y-%m-%d")

#-----------------------------------------------------------------------------
# Allow Visual Studio solution directories
#-----------------------------------------------------------------------------
# Provide a way for Visual Studio Express users to turn OFF the new FOLDER
# organization feature. Default to ON for non-Express users. Express users must
# explicitly turn off this option to build HDF4 in the Express IDE...
#
option (HDF4_USE_FOLDERS "Enable folder grouping of projects in IDEs." ON)
mark_as_advanced (HDF4_USE_FOLDERS)
if (HDF4_USE_FOLDERS)
  set_property (GLOBAL PROPERTY USE_FOLDERS ON)
endif ()
option (HDF4_NO_PACKAGES "CPACK - Disable packaging" OFF)
mark_as_advanced (HDF4_NO_PACKAGES)

#-----------------------------------------------------------------------------
# Set the core names of all the libraries
#-----------------------------------------------------------------------------
set (HDF4_SRC_LIB_CORENAME          "hdf")
set (HDF4_SRC_FCSTUB_LIB_CORENAME   "hdf_fcstub")
set (HDF4_SRC_FORTRAN_LIB_CORENAME  "hdf_fortran")
set (HDF4_MF_LIB_CORENAME           "mfhdf")
set (HDF4_MF_XDR_LIB_CORENAME       "xdr")
set (HDF4_HDF_TEST_LIB_CORENAME     "hdf_test")
set (HDF4_HDF_TEST_FCSTUB_LIB_CORENAME     "hdf_test_fcstub")
set (HDF4_MF_FCSTUB_LIB_CORENAME    "mfhdf_fcstub")
set (HDF4_MF_FORTRAN_LIB_CORENAME   "mfhdf_fortran")
set (HDF4_MF_TEST_LIB_CORENAME      "mf_test")
set (HDF4_TOOLS_LIB_CORENAME        "h4tools")
set (HDF4_JAVA_JNI_LIB_CORENAME     "hdf_java")
set (HDF4_JAVA_HDF_LIB_CORENAME     "jarhdf")
set (HDF4_JAVA_TEST_LIB_CORENAME    "jartest4")

#-----------------------------------------------------------------------------
# Set the true names of all the libraries if customized by external project
#-----------------------------------------------------------------------------
set (HDF4_SRC_LIB_NAME          "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_LIB_CORENAME}")
set (HDF4_SRC_FCSTUB_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_FCSTUB_LIB_CORENAME}")
set (HDF4_SRC_FORTRAN_LIB_NAME  "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_SRC_FORTRAN_LIB_CORENAME}")
set (HDF4_MF_LIB_NAME           "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_LIB_CORENAME}")
set (HDF4_MF_XDR_LIB_NAME       "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_XDR_LIB_CORENAME}")
set (HDF4_HDF_TEST_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_HDF_TEST_LIB_CORENAME}")
set (HDF4_HDF_TEST_FCSTUB_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_HDF_TEST_FCSTUB_LIB_CORENAME}")
set (HDF4_MF_FCSTUB_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FCSTUB_LIB_CORENAME}")
set (HDF4_MF_FORTRAN_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FORTRAN_LIB_CORENAME}")
set (HDF4_MF_TEST_LIB_NAME      "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_TEST_LIB_CORENAME}")
set (HDF4_TOOLS_LIB_NAME        "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_TOOLS_LIB_CORENAME}")
set (HDF4_JAVA_JNI_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_JAVA_JNI_LIB_CORENAME}")
set (HDF4_JAVA_HDF_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_JAVA_HDF_LIB_CORENAME}")
set (HDF4_JAVA_TEST_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_JAVA_TEST_LIB_CORENAME}")

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
set (HDF4_SRC_LIB_TARGET          "${HDF4_SRC_LIB_CORENAME}-static")
set (HDF4_SRC_FCSTUB_LIB_TARGET   "${HDF4_SRC_FCSTUB_LIB_CORENAME}-static")
set (HDF4_SRC_FORTRAN_LIB_TARGET  "${HDF4_SRC_FORTRAN_LIB_CORENAME}-static")
set (HDF4_MF_LIB_TARGET           "${HDF4_MF_LIB_CORENAME}-static")
set (HDF4_MF_XDR_LIB_TARGET       "${HDF4_MF_XDR_LIB_CORENAME}-static")
set (HDF4_HDF_TEST_LIB_TARGET     "${HDF4_HDF_TEST_LIB_CORENAME}-static")
set (HDF4_HDF_TEST_FCSTUB_LIB_TARGET     "${HDF4_HDF_TEST_FCSTUB_LIB_CORENAME}-static")
set (HDF4_MF_FCSTUB_LIB_TARGET    "${HDF4_MF_FCSTUB_LIB_CORENAME}-static")
set (HDF4_MF_FORTRAN_LIB_TARGET   "${HDF4_MF_FORTRAN_LIB_CORENAME}-static")
set (HDF4_MF_TEST_LIB_TARGET      "${HDF4_MF_TEST_LIB_CORENAME}-static")
set (HDF4_TOOLS_LIB_TARGET        "${HDF4_TOOLS_LIB_CORENAME}-static")
set (HDF4_JAVA_JNI_LIB_TARGET     "${HDF4_JAVA_JNI_LIB_CORENAME}")
set (HDF4_JAVA_HDF_LIB_TARGET     "${HDF4_JAVA_HDF_LIB_CORENAME}")
set (HDF4_JAVA_TEST_LIB_TARGET    "${HDF4_JAVA_TEST_LIB_CORENAME}")
set (HDF4_SRC_LIBSH_TARGET          "${HDF4_SRC_LIB_CORENAME}-shared")
set (HDF4_SRC_FCSTUB_LIBSH_TARGET   "${HDF4_SRC_FCSTUB_LIB_CORENAME}-shared")
set (HDF4_SRC_FORTRAN_LIBSH_TARGET  "${HDF4_SRC_FORTRAN_LIB_CORENAME}-shared")
set (HDF4_MF_LIBSH_TARGET           "${HDF4_MF_LIB_CORENAME}-shared")
set (HDF4_MF_XDR_LIBSH_TARGET       "${HDF4_MF_XDR_LIB_CORENAME}-shared")
set (HDF4_MF_FCSTUB_LIBSH_TARGET    "${HDF4_MF_FCSTUB_LIB_CORENAME}-shared")
set (HDF4_MF_FORTRAN_LIBSH_TARGET   "${HDF4_MF_FORTRAN_LIB_CORENAME}-shared")
set (HDF4_TOOLS_LIBSH_TARGET        "${HDF4_TOOLS_LIB_CORENAME}-shared")

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
set (HDF_RESOURCES_DIR        ${HDF4_SOURCE_DIR}/config/cmake)
set (HDF_RESOURCES_EXT_DIR    ${HDF4_SOURCE_DIR}/config/cmake_ext_mod)
set (HDF4_HDFSOURCE_DIR       ${HDF4_SOURCE_DIR}/hdf/src)
set (HDF4_HDF_TESTSOURCE_DIR  ${HDF4_SOURCE_DIR}/hdf/test)
set (HDF4_MFHDFSOURCE_DIR     ${HDF4_SOURCE_DIR}/mfhdf/libsrc)
set (HDF4_MFHDF_TEST_DIR      ${HDF4_SOURCE_DIR}/mfhdf/test)
set (HDF4_MFHDF_FORTRAN_DIR   ${HDF4_SOURCE_DIR}/mfhdf/fortran)
set (HDF4_MFHDF_XDR_DIR       ${HDF4_SOURCE_DIR}/mfhdf/xdr)
set (HDF4_JAVA_JNI_SRC_DIR         ${HDF4_SOURCE_DIR}/java/src/jni)
set (HDF4_JAVA_HDF_SRC_DIR         ${HDF4_SOURCE_DIR}/java/src/hdf)
set (HDF4_JAVA_TEST_SRC_DIR        ${HDF4_SOURCE_DIR}/java/test)
set (HDF4_JAVA_LIB_DIR             ${HDF4_SOURCE_DIR}/java/lib)
set (HDF4_JAVA_LOGGING_JAR         ${HDF4_SOURCE_DIR}/java/lib/slf4j-api-1.7.25.jar)
set (HDF4_JAVA_LOGGING_NOP_JAR     ${HDF4_SOURCE_DIR}/java/lib/ext/slf4j-nop-1.7.25.jar)
set (HDF4_JAVA_LOGGING_SIMPLE_JAR  ${HDF4_SOURCE_DIR}/java/lib/ext/slf4j-simple-1.7.25.jar)

set (CMAKE_MODULE_PATH ${HDF_RESOURCES_DIR} ${HDF_RESOURCES_EXT_DIR} ${CMAKE_MODULE_PATH})

#-----------------------------------------------------------------------------
# parse the full version number from hfile.h and include in H4_VERS_INFO
#-----------------------------------------------------------------------------
file (READ ${HDF4_HDFSOURCE_DIR}/hfile.h _hfile_h_contents)
string (REGEX REPLACE ".*#define[ \t]+LIBVER_MAJOR[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_MAJOR ${_hfile_h_contents})
string (REGEX REPLACE ".*#define[ \t]+LIBVER_MINOR[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_MINOR ${_hfile_h_contents})
string (REGEX REPLACE ".*#define[ \t]+LIBVER_RELEASE[ \t]+([0-9]*).*$"
    "\\1" H4_VERS_RELEASE ${_hfile_h_contents})
string (REGEX REPLACE ".*#define[ \t]+LIBVER_SUBRELEASE[ \t]+\"([0-9A-Za-z.]*)\".*$"
    "\\1" H4_VERS_SUBRELEASE ${_hfile_h_contents})
#message (STATUS "VERSION: ${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}-${H4_VERS_SUBRELEASE}")

#-----------------------------------------------------------------------------
# Basic HDF4 stuff here
#-----------------------------------------------------------------------------
set (HDF4_PACKAGE "hdf4")
set (HDF4_PACKAGE_NAME "HDF")
set (HDF4_PACKAGE_VERSION "${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}")
set (HDF4_PACKAGE_VERSION_MAJOR "${H4_VERS_MAJOR}.${H4_VERS_MINOR}")
set (HDF4_PACKAGE_VERSION_MINOR "${H4_VERS_RELEASE}")
set (HDF4_PACKAGE_VERSION_STRING "${HDF4_PACKAGE_VERSION}")
if (NOT "${H4_VERS_SUBRELEASE}" STREQUAL "")
  set (HDF4_PACKAGE_VERSION_STRING "${HDF4_PACKAGE_VERSION_STRING}-${H4_VERS_SUBRELEASE}")
endif ()
set (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION_STRING}")
set (HDF4_PACKAGE_TARNAME "${HDF4_PACKAGE}${HDF_PACKAGE_EXT}")
set (HDF4_PACKAGE_URL "http://www.hdfgroup.org")
set (HDF4_PACKAGE_BUGREPORT "help@hdfgroup.org")
set (HDF4_PACKAGE_SOVERSION "${H4_VERS_MAJOR}.${H4_VERS_RELEASE}.${H4_VERS_MINOR}")
set (HDF4_PACKAGE_SOVERSION_MAJOR "${H4_VERS_MAJOR}")

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
include (${HDF_RESOURCES_EXT_DIR}/HDFMacros.cmake)

HDF_DIR_PATHS("HDF4")

include (${HDF_RESOURCES_EXT_DIR}/HDFLibMacros.cmake)
include (${HDF_RESOURCES_DIR}/HDF4Macros.cmake)

set (HDF4_INSTALL_UTILS_BIN_DIR ${HDF4_INSTALL_BIN_DIR})
set (HDF4_INSTALL_TOOLS_BIN_DIR ${HDF4_INSTALL_BIN_DIR})

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects.
#-----------------------------------------------------------------------------
if (NOT HDF4_EXPORTED_TARGETS)
  set (HDF4_EXPORTED_TARGETS "hdf4-targets")
endif ()

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME,
# add it to this variable. This is NOT used by Make Install, but for projects
# which include hdf4 as a sub-project within their build tree
#-----------------------------------------------------------------------------
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "")
set_global_variable (HDF4_UTILS_TO_EXPORT "")

set (EXTERNAL_HEADER_LIST "")
set (EXTERNAL_LIBRARY_LIST "")
set (EXTERNAL_LIBRARYDLL_LIST "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
include (${HDF_RESOURCES_DIR}/ConfigureChecks.cmake)

set (CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

#-----------------------------------------------------------------------------
# Mac OS X Options
#-----------------------------------------------------------------------------
if (HDF4_BUILD_FRAMEWORKS AND NOT BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries" FORCE)
endif ()

#-----------------------------------------------------------------------------
# Option to Build Shared and Static libs, default is both
#-----------------------------------------------------------------------------
option (BUILD_STATIC_LIBS "Build Static Libraries" ON)
set (H4_ENABLE_STATIC_LIB NO)
option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set (H4_ENABLE_SHARED_LIB NO)
option (ONLY_SHARED_LIBS "Only Build Shared Libraries" OFF)
mark_as_advanced (ONLY_SHARED_LIBS)

if (BUILD_STATIC_LIBS)
  set (H4_ENABLE_STATIC_LIB YES)
endif ()
if (BUILD_SHARED_LIBS)
  set (H4_ENABLE_SHARED_LIB YES)
endif ()

# Force only shared libraries if all OFF
if (NOT BUILD_STATIC_LIBS AND NOT BUILD_SHARED_LIBS)
  set (ONLY_SHARED_LIBS ON CACHE BOOL "Only Build Shared Libraries" FORCE)
endif ()

if (ONLY_SHARED_LIBS)
  set (H4_ENABLE_STATIC_LIB NO)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries")
endif ()

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions (-DHAVE_CONFIG_H)

#-----------------------------------------------------------------------------
# Temporary disable optimization flag
#-----------------------------------------------------------------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR CYGWIN)
  foreach (flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "O3")
      string (REGEX REPLACE "O3" "O2" ${flag_var} "${${flag_var}}")
    endif ()
  endforeach ()
endif ()

if (HDF4_ENABLE_ANALYZER_TOOLS)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/tools.cmake)
endif ()
if (HDF4_ENABLE_SANITIZERS)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/sanitizers.cmake)
endif ()

#-----------------------------------------------------------------------------
# Option to use code coverage
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_COVERAGE "Enable code coverage for Libraries and Programs" OFF)
if (HDF4_ENABLE_COVERAGE)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/code-coverage.cmake)
  if(CODE_COVERAGE AND CODE_COVERAGE_ADDED)
    add_code_coverage () # Adds instrumentation to all targets
  else ()
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage -O0 -fprofile-arcs -ftest-coverage")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (LDFLAGS "${LDFLAGS} -fprofile-arcs -ftest-coverage")
      link_libraries (gcov)
    else ()
      set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to use deprecated public API symbols
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_DEPRECATED_SYMBOLS "Enable deprecated public API symbols" ON)
if (HDF4_ENABLE_DEPRECATED_SYMBOLS)
  set (H4_NO_DEPRECATED_SYMBOLS 0)
else ()
  set (H4_NO_DEPRECATED_SYMBOLS 1)
endif ()

#-----------------------------------------------------------------------------
# Include the main src and config directories
#-----------------------------------------------------------------------------
set (HDF4_INCLUDE_DIRECTORIES
    ${HDF4_HDFSOURCE_DIR}
    ${HDF4_MFHDFSOURCE_DIR}
    ${HDF4_SOURCE_DIR}
    ${HDF4_BINARY_DIR}
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
INCLUDE_DIRECTORIES (${HDF4_INCLUDE_DIRECTORIES} )

#-----------------------------------------------------------------------------
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
set (EXE_EXT "")
if (WIN32 OR MINGW)
  set (EXE_EXT ".exe")
  add_definitions (-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS)
  add_definitions (-D_CONSOLE)
endif ()

if (MSVC)
  set (CMAKE_MFC_FLAG 0)
  set (WIN_COMPILE_FLAGS "")
  set (WIN_LINK_FLAGS "")
endif ()

set (MAKE_SYSTEM)
if (CMAKE_BUILD_TOOL MATCHES "make")
  set (MAKE_SYSTEM 1)
endif ()

set (CFG_INIT "/${CMAKE_CFG_INTDIR}")
if (MAKE_SYSTEM)
  set (CFG_INIT "")
endif ()

include (${HDF_RESOURCES_DIR}/HDFCompilerFlags.cmake)
set (CMAKE_MODULE_PATH ${HDF_RESOURCES_DIR} ${HDF_RESOURCES_EXT_DIR} ${CMAKE_MODULE_PATH})

#-----------------------------------------------------------------------------
# Include user macros
#-----------------------------------------------------------------------------
include (UserMacros.cmake)

#-----------------------------------------------------------------------------
# Include filter (zlib, szip, etc.) macros
#-----------------------------------------------------------------------------
include (CMakeFilters.cmake)

#-----------------------------------------------------------------------------
# Option for external libraries on windows
#-----------------------------------------------------------------------------
option (HDF4_PACKAGE_EXTLIBS "CPACK - include external libraries" OFF)
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  if (HDF4_PACKAGE_EXTLIBS)
    set (HDF4_NO_PACKAGES OFF CACHE BOOL "CPACK - Disable packaging" FORCE)
    if (HDF4_ENABLE_JPEG_LIB_SUPPORT AND JPEG_FOUND)
      PACKAGE_JPEG_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    endif ()

    if (HDF4_ENABLE_Z_LIB_SUPPORT AND ZLIB_FOUND)
      PACKAGE_ZLIB_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    endif ()

    if (HDF4_ENABLE_SZIP_SUPPORT AND SZIP_FOUND)
      PACKAGE_SZIP_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to build HDF4 xdr Library
#-----------------------------------------------------------------------------
option (HDF4_BUILD_XDR_LIB  "Build HDF4 XDR Library" OFF)
if (WIN32 OR MINGW OR APPLE)
  set (HDF4_BUILD_XDR_LIB ON CACHE BOOL "Build HDF4 XDR Library" FORCE)
else ()
  if (NOT HDF4_BUILD_XDR_LIB)
    include (FindXDR)
    if (XDR_FOUND)
      set (CMAKE_REQUIRED_LIBRARIES "${XDR_LIBRARIES}" )
      message (STATUS "HDF4 XDR Library found: ${XDR_LIBRARIES}")
      set (LINK_LIBS ${LINK_LIBS} ${XDR_LIBRARIES})
      set (LINK_SHARED_LIBS ${LINK_SHARED_LIBS} ${XDR_LIBRARIES})
      include_directories(${XDR_INCLUDE_DIRS})
    else ()
      set (HDF4_BUILD_XDR_LIB ON CACHE BOOL "Build HDF4 XDR Library" FORCE)
      message (STATUS "HDF4 XDR Library will be built")
    endif ()
  endif ()
endif ()

if (HDF4_BUILD_XDR_LIB)
  set (H4_NO_SYS_XDR_INC 1)
  add_subdirectory (mfhdf/xdr)
endif ()

#-----------------------------------------------------------------------------
# Add the HDF4 Library Target to the build
#-----------------------------------------------------------------------------
add_subdirectory (hdf/src )
add_subdirectory (mfhdf/libsrc)

if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  if (JPEG_FOUND AND JPEG_USE_EXTERNAL)
    if (NOT ONLY_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIB_TARGET} JPEG)
    endif ()
    if (BUILD_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIBSH_TARGET} JPEG)
    endif ()
  endif ()
  if (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
    if (NOT ONLY_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIB_TARGET} ZLIB)
    endif ()
    if (BUILD_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIBSH_TARGET} ZLIB)
    endif ()
  endif ()
  if (SZIP_FOUND AND SZIP_USE_EXTERNAL)
    if (NOT ONLY_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIB_TARGET} SZIP)
    endif ()
    if (BUILD_SHARED_LIBS)
      add_dependencies (${HDF4_SRC_LIBSH_TARGET} SZIP)
    endif ()
  endif ()
endif ()

if (HDF4_BUILD_XDR_LIB)
  if (NOT ONLY_SHARED_LIBS)
    add_dependencies (${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET})
  endif ()
  if (BUILD_SHARED_LIBS)
    add_dependencies (${HDF4_MF_LIBSH_TARGET} ${HDF4_MF_XDR_LIBSH_TARGET})
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
option (BUILD_TESTING "Build HDF4 Unit Testing" ON)
if (BUILD_TESTING)
  set (DART_TESTING_TIMEOUT 1200
      CACHE STRING
      "Timeout in seconds for each test (default 1200=20minutes)"
  )
  enable_testing ()
  include (CTest)

  if (NOT EXISTS "${HDF4_BINARY_DIR}/CTestCustom.ctest")
    include (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
    configure_file (${HDF_RESOURCES_DIR}/CTestCustom.cmake ${HDF4_BINARY_DIR}/CTestCustom.ctest @ONLY)
  endif ()

  if (NOT HDF4_EXTERNALLY_CONFIGURED)
    if (EXISTS "${HDF4_SOURCE_DIR}/hdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/test")
      add_subdirectory (hdf/test)
    endif ()
    if (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/test")
      add_subdirectory (mfhdf/test)
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to build Fortran bindings/tests/examples
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
#-----------------------------------------------------------------------------
# Set default name mangling : overridden by Fortran detection in fortran dir
set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## __")
if (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran")
  option (HDF4_BUILD_FORTRAN "Build FORTRAN support" ON)
  if (HDF4_BUILD_FORTRAN)
    if (WIN32)
      set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) NAME")
      set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) NAME")
    endif ()
    include (${HDF_RESOURCES_DIR}/HDF4UseFortran.cmake)
    add_subdirectory (mfhdf/fortran)
    add_subdirectory (hdf/fortran)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Library utilities
#-----------------------------------------------------------------------------
add_subdirectory (hdf)
add_subdirectory (mfhdf)

#-----------------------------------------------------------------------------
# Option to build HDF4 Examples
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/examples")
  option (HDF4_BUILD_EXAMPLES  "Build HDF4 Examples" OFF)
  if (HDF4_BUILD_EXAMPLES)
    if (HDF4_BUILD_FORTRAN)
      if (EXISTS "${HDF4_SOURCE_DIR}/hdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/fortran/examples")
        add_subdirectory (hdf/fortran/examples)
      endif ()
      if (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples")
        add_subdirectory (mfhdf/fortran/examples)
      endif ()
    endif ()

    add_subdirectory (hdf/examples)
    add_subdirectory (mfhdf/examples)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to build HDF4 Java Library
#-----------------------------------------------------------------------------
if (EXISTS "${HDF4_SOURCE_DIR}/java" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/java")
  option (HDF4_BUILD_JAVA  "Build Java HDF Library" OFF)
  if (HDF4_BUILD_JAVA)
    add_subdirectory (java)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Generate the H4config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
configure_file (${HDF_RESOURCES_DIR}/h4config.h.in      ${PROJECT_BINARY_DIR}/h4config.h @ONLY)

include (CMakeInstallation.cmake)
