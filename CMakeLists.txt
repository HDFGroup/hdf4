cmake_minimum_required (VERSION 3.18)
project (HDF4 LANGUAGES C)

if (POLICY CMP0074)
  # find_package() uses <PackageName>_ROOT variables.
  cmake_policy (SET CMP0074 NEW)
endif ()

if (POLICY CMP0083)
  # To control generation of Position Independent Executable (PIE) or not,
  # some flags are required at link time.
  cmake_policy (SET CMP0083 NEW)
endif ()

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

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

# Whether the most recently called project() command, in the current scope or above,
# was in the top level CMakeLists.txt file.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.21.0")
  if(NOT PROJECT_IS_TOP_LEVEL)
    set (HDF4_EXTERNALLY_CONFIGURED 1)
  endif()
else()
  if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set (HDF4_EXTERNALLY_CONFIGURED 1)
  endif()
endif()
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
set (HDF4_LIB_INFIX "" CACHE STRING "Use infix for custom library naming.")
mark_as_advanced (HDF4_LIB_INFIX)
# HDF4_LIB_INFIX :
#   This infix is added to all library names after 'hdf4'.
#   e.g. the infix '_exin' results in the library name 'libhdf_ex.so'
#   This name is used in packages on debian based systems.
#   (see https://packages.debian.org/jessie/amd64/libhdf-exin-8/filelist)
#option (HDF4_EXTERNAL_LIB_SUFFIX "Use prefix for custom library naming." "")
set (HDF4_EXTERNAL_LIB_SUFFIX "" CACHE STRING "Use suffix for custom library naming.")
mark_as_advanced (HDF4_EXTERNAL_LIB_SUFFIX)
# HDF4_EXTERNAL_LIB_SUFFIX :
#   If the parent project needs to install hdf libraries, but avoid
#   name conflicts with system versions, then a suffix may be added
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
#   # Setup all necessary overrides for szip so that HDF4 uses our
#   # internally compiled szip rather than any other version
#   if (HDF4_ENABLE_SZIP_SUPPORT)
#    # We must tell the main HDF4 library that it depends on our szip
#     set (HDF4_LIB_DEPENDENCIES prjszip)
#     # Override the szip header file
#     if (PRJ_USE_SYSTEM_SZIP)
#       set (H4_SZIP_HEADER "szip.h")
#     else ()
#      set (H4_SZIP_HEADER "prj_szip.h")
#       # Set vars that FindSZIP would have set if used in sub project
#       set (SZIP_INCLUDE_DIRS "${PRJ_SZIP_INCLUDE_DIRS}")
#       set (SZIP_LIBRARIES prjszip)
#     endif ()
#  endif ()
#
#   # Add the sub project
#   add_subdirectory (Utilities/hdf4.3.0)
#   # Add the HDF4 dirs to our include path
#   set (HDF4_INCLUDE_DIR
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.3.0/hdf/src
#       ${PROJECT_BINARY_DIR}/Utilities/hdf4.3.0
#       ${PROJECT_SOURCE_DIR}/Utilities/hdf4.3.0/mfhdf/src
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
# Set the core names of all the libraries CORENAME is the base library name
# for targets, BASE_CORE
# filename are made of PREFIX_BASE_INFIX_CORE_SUFFIX
#-----------------------------------------------------------------------------
set (HDF4_LIB_BASE              "hdf")
set (HDF4_LIB_SYM_BASE          "df")
set (HDF4_MFLIB_BASE            "mfhdf")

set (HDF4_LIB_CORE              "")
set (HDF4_MF_LIB_CORE           "")
set (HDF4_TEST_LIB_CORE         "_test")
set (HDF4_TOOLS_LIB_CORE        "_tools")
set (HDF4_FORTRAN_LIB_CORE      "_fortran")
set (HDF4_FCSTUB_LIB_CORE       "_fcstub")
set (HDF4_JAVA_JNI_LIB_CORE     "_java")

set (HDF4_SRC_LIB_CORENAME          "${HDF4_LIB_BASE}")
set (HDF4_SRC_LIB_SYM_CORENAME      "${HDF4_LIB_SYM_BASE}")
set (HDF4_SRC_FCSTUB_LIB_CORENAME   "${HDF4_LIB_BASE}${HDF4_FCSTUB_LIB_CORE}")
set (HDF4_SRC_FORTRAN_LIB_CORENAME  "${HDF4_LIB_BASE}${HDF4_FORTRAN_LIB_CORE}")
set (HDF4_MF_LIB_CORENAME           "${HDF4_MFLIB_BASE}")
set (HDF4_HDF_TEST_LIB_CORENAME     "${HDF4_LIB_BASE}${HDF4_TEST_LIB_CORE}${HDF4_FCSTUB_LIB_CORE}")
set (HDF4_HDF_TEST_FCSTUB_LIB_CORENAME     "${HDF4_LIB_BASE}${HDF4_TEST_LIB_CORE}")
set (HDF4_MF_FCSTUB_LIB_CORENAME    "${HDF4_MFLIB_BASE}${HDF4_FCSTUB_LIB_CORE}")
set (HDF4_MF_FORTRAN_LIB_CORENAME   "${HDF4_MFLIB_BASE}${HDF4_FORTRAN_LIB_CORE}")
set (HDF4_MF_TEST_LIB_CORENAME      "${HDF4_MFLIB_BASE}${HDF4_TEST_LIB_CORE}")
set (HDF4_TOOLS_LIB_CORENAME        "${HDF4_LIB_BASE}${HDF4_TOOLS_LIB_CORE}")
set (HDF4_JAVA_JNI_LIB_CORENAME     "${HDF4_LIB_BASE}${HDF4_JAVA_JNI_LIB_CORE}")
set (HDF4_JAVA_HDF_LIB_CORENAME     "jarhdf")
set (HDF4_JAVA_TEST_LIB_CORENAME    "jartest4")

#-----------------------------------------------------------------------------
# Set the true names of all the libraries if customized by external project
#-----------------------------------------------------------------------------
set (HDF4_SRC_LIB_NAME          "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_SRC_LIB_SYM_NAME      "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_LIB_SYM_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_SRC_FCSTUB_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_FCSTUB_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_SRC_FORTRAN_LIB_NAME  "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_FORTRAN_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_MF_LIB_NAME           "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MFLIB_BASE}${HDF4_LIB_INFIX}${HDF4_MF_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_HDF_TEST_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_TEST_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_HDF_TEST_FCSTUB_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_TEST_LIB_CORE}${HDF4_HDF_FCSTUB_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_MF_FCSTUB_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MFLIB_BASE}${HDF4_LIB_INFIX}${HDF4_FCSTUB_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_MF_FORTRAN_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MFLIB_BASE}${HDF4_LIB_INFIX}${HDF4_FORTRAN_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_MF_TEST_LIB_NAME      "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MFLIB_BASE}${HDF4_LIB_INFIX}${HDF4_TEST_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_TOOLS_LIB_NAME        "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_TOOLS_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
#set (HDF4_JAVA_JNI_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_LIB_BASE}${HDF4_LIB_INFIX}${HDF4_JAVA_JNI_LIB_CORE}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_JAVA_JNI_LIB_NAME     "${HDF4_LIB_BASE}${HDF4_JAVA_JNI_LIB_CORE}")
set (HDF4_JAVA_HDF_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_JAVA_HDF_LIB_CORENAME}${HDF4_LIB_INFIX}${HDF4_EXTERNAL_LIB_SUFFIX}")
set (HDF4_JAVA_TEST_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_JAVA_TEST_LIB_CORENAME}${HDF4_LIB_INFIX}${HDF4_EXTERNAL_LIB_SUFFIX}")

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
set (HDF4_SRC_LIB_TARGET          "${HDF4_SRC_LIB_CORENAME}-static")
set (HDF4_SRC_LIB_SYM_TARGET      "${HDF4_SRC_LIB_SYM_CORENAME}-static")
set (HDF4_SRC_FCSTUB_LIB_TARGET   "${HDF4_SRC_FCSTUB_LIB_CORENAME}-static")
set (HDF4_SRC_FORTRAN_LIB_TARGET  "${HDF4_SRC_FORTRAN_LIB_CORENAME}-static")
set (HDF4_MF_LIB_TARGET           "${HDF4_MF_LIB_CORENAME}-static")
set (HDF4_HDF_TEST_LIB_TARGET     "${HDF4_HDF_TEST_LIB_CORENAME}-static")
set (HDF4_HDF_TEST_FCSTUB_LIB_TARGET     "${HDF4_HDF_TEST_FCSTUB_LIB_CORENAME}-static")
set (HDF4_MF_FCSTUB_LIB_TARGET    "${HDF4_MF_FCSTUB_LIB_CORENAME}-static")
set (HDF4_MF_FORTRAN_LIB_TARGET   "${HDF4_MF_FORTRAN_LIB_CORENAME}-static")
set (HDF4_MF_TEST_LIB_TARGET      "${HDF4_MF_TEST_LIB_CORENAME}-static")
set (HDF4_TOOLS_LIB_TARGET        "${HDF4_TOOLS_LIB_CORENAME}-static")
set (HDF4_JAVA_JNI_LIB_TARGET     "${HDF4_JAVA_JNI_LIB_CORENAME}")
set (HDF4_JAVA_HDF_LIB_TARGET     "${HDF4_JAVA_HDF_LIB_CORENAME}")
set (HDF4_JAVA_TEST_LIB_TARGET    "${HDF4_JAVA_TEST_LIB_CORENAME}")
set (HDF4_SRC_LIBSH_SYM_TARGET      "${HDF4_SRC_LIB_SYM_CORENAME}-shared")
set (HDF4_SRC_LIBSH_TARGET          "${HDF4_SRC_LIB_CORENAME}-shared")
set (HDF4_SRC_FCSTUB_LIBSH_TARGET   "${HDF4_SRC_FCSTUB_LIB_CORENAME}-shared")
set (HDF4_SRC_FORTRAN_LIBSH_TARGET  "${HDF4_SRC_FORTRAN_LIB_CORENAME}-shared")
set (HDF4_MF_LIBSH_TARGET           "${HDF4_MF_LIB_CORENAME}-shared")
set (HDF4_MF_FCSTUB_LIBSH_TARGET    "${HDF4_MF_FCSTUB_LIB_CORENAME}-shared")
set (HDF4_MF_FORTRAN_LIBSH_TARGET   "${HDF4_MF_FORTRAN_LIB_CORENAME}-shared")
set (HDF4_TOOLS_LIBSH_TARGET        "${HDF4_TOOLS_LIB_CORENAME}-shared")

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
set (HDF_CONFIG_DIR           ${HDF4_SOURCE_DIR}/config)
set (HDF_RESOURCES_DIR        ${HDF4_SOURCE_DIR}/config/cmake)
set (HDF4_HDFSOURCE_DIR       ${HDF4_SOURCE_DIR}/hdf/src)
set (HDF4_HDF_TESTSOURCE_DIR  ${HDF4_SOURCE_DIR}/hdf/test)
set (HDF4_MFHDFSOURCE_DIR     ${HDF4_SOURCE_DIR}/mfhdf/src)
set (HDF4_MFHDF_TEST_DIR      ${HDF4_SOURCE_DIR}/mfhdf/test)
set (HDF4_MFHDF_UTIL_DIR      ${HDF4_SOURCE_DIR}/mfhdf/util)
set (HDF4_MFHDF_FORTRAN_DIR   ${HDF4_SOURCE_DIR}/mfhdf/fortran)
set (HDF4_JAVA_JNI_SRC_DIR         ${HDF4_SOURCE_DIR}/java/src/jni)
set (HDF4_JAVA_HDF_SRC_DIR         ${HDF4_SOURCE_DIR}/java/src/hdf)
set (HDF4_JAVA_TEST_SRC_DIR        ${HDF4_SOURCE_DIR}/java/test)
set (HDF4_JAVA_LIB_DIR             ${HDF4_SOURCE_DIR}/java/lib)
set (HDF4_JAVA_LOGGING_JAR         ${HDF4_SOURCE_DIR}/java/lib/slf4j-api-2.0.6.jar)
set (HDF4_JAVA_LOGGING_NOP_JAR     ${HDF4_SOURCE_DIR}/java/lib/ext/slf4j-nop-2.0.6.jar)
set (HDF4_JAVA_LOGGING_SIMPLE_JAR  ${HDF4_SOURCE_DIR}/java/lib/ext/slf4j-simple-2.0.6.jar)
set (HDF4_DOXYGEN_DIR              ${HDF4_SOURCE_DIR}/doxygen)

set (HDF4_SRC_INCLUDE_DIRS ${HDF4_HDFSOURCE_DIR} ${HDF4_MFHDFSOURCE_DIR})

set (CMAKE_MODULE_PATH ${HDF_RESOURCES_DIR} ${CMAKE_MODULE_PATH})

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
string (REGEX REPLACE ".*#define[ \t]+LIBVER_SUBRELEASE[ \t]+\"([0-9A-Za-z._-]*)\".*$"
    "\\1" H4_VERS_SUBRELEASE ${_hfile_h_contents})
message (TRACE "VERSION: ${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}-${H4_VERS_SUBRELEASE}")

#-----------------------------------------------------------------------------
# parse the full soversion number from config/lt_vers.am and include in H4_SOVERS_INFO
#-----------------------------------------------------------------------------
file (READ ${HDF4_SOURCE_DIR}/config/lt_vers.am _lt_vers_am_contents)
string (REGEX REPLACE ".*LT_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$"
    "\\1" H4_DF_SOVERS_INTERFACE ${_lt_vers_am_contents})
string (REGEX REPLACE ".*LT_VERS_REVISION[ \t]+=[ \t]+([0-9]*).*$"
    "\\1" H4_DF_SOVERS_MINOR ${_lt_vers_am_contents})
string (REGEX REPLACE ".*LT_VERS_AGE[ \t]+=[ \t]+([0-9]*).*$"
    "\\1" H4_DF_SOVERS_RELEASE ${_lt_vers_am_contents})
math (EXPR H4_DF_SOVERS_MAJOR ${H4_DF_SOVERS_INTERFACE}-${H4_DF_SOVERS_RELEASE})
message (VERBOSE "HDF SOVERSION: ${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
string (REGEX MATCH ".*LT_MF_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$" H4_MF_SOVERS_EXISTS ${_lt_vers_am_contents})
if (H4_MF_SOVERS_EXISTS)
  string (REGEX REPLACE ".*LT_MF_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_SOVERS_INTERFACE ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_MF_VERS_REVISION[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_SOVERS_MINOR ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_MF_VERS_AGE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_SOVERS_RELEASE ${_lt_vers_am_contents})
  math (EXPR H4_MF_SOVERS_MAJOR ${H4_MF_SOVERS_INTERFACE}-${H4_MF_SOVERS_RELEASE})
  message (VERBOSE "MFHDFSOVERSION: ${H4_MF_SOVERS_MAJOR}.${H4_MF_SOVERS_RELEASE}.${H4_MF_SOVERS_MINOR}")
endif ()
string (REGEX MATCH ".*LT_F_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$" H4_F_SOVERS_EXISTS ${_lt_vers_am_contents})
if (H4_F_SOVERS_EXISTS)
  string (REGEX REPLACE ".*LT_F_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_F_SOVERS_INTERFACE ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_F_VERS_REVISION[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_F_SOVERS_MINOR ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_F_VERS_AGE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_F_SOVERS_RELEASE ${_lt_vers_am_contents})
  math (EXPR H4_F_SOVERS_MAJOR ${H4_F_SOVERS_INTERFACE}-${H4_F_SOVERS_RELEASE})
  message (VERBOSE "SOVERSION_F: ${H4_F_SOVERS_MAJOR}.${H4_F_SOVERS_RELEASE}.${H4_F_SOVERS_MINOR}")
endif ()
string (REGEX MATCH ".*LT_MF_F_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$" H4_MF_F_SOVERS_EXISTS ${_lt_vers_am_contents})
if (H4_MF_F_SOVERS_EXISTS)
  string (REGEX REPLACE ".*LT_MF_F_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_F_SOVERS_INTERFACE ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_MF_F_VERS_REVISION[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_F_SOVERS_MINOR ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_MF_F_VERS_AGE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_MF_F_SOVERS_RELEASE ${_lt_vers_am_contents})
  math (EXPR H4_MF_F_SOVERS_MAJOR ${H4_MF_F_SOVERS_INTERFACE}-${H4_MF_F_SOVERS_RELEASE})
  message (VERBOSE "SOVERSION_MF_F: ${H4_MF_F_SOVERS_MAJOR}.${H4_MF_F_SOVERS_RELEASE}.${H4_MF_F_SOVERS_MINOR}")
endif ()
string (REGEX MATCH ".*LT_JAVA_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$" H4_JAVA_SOVERS_EXISTS ${_lt_vers_am_contents})
if(H4_JAVA_SOVERS_EXISTS)
  string (REGEX REPLACE ".*LT_JAVA_VERS_INTERFACE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_JAVA_SOVERS_INTERFACE ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_JAVA_VERS_REVISION[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_JAVA_SOVERS_MINOR ${_lt_vers_am_contents})
  string (REGEX REPLACE ".*LT_JAVA_VERS_AGE[ \t]+=[ \t]+([0-9]*).*$"
      "\\1" H4_JAVA_SOVERS_RELEASE ${_lt_vers_am_contents})
  math (EXPR H4_JAVA_SOVERS_MAJOR ${H4_JAVA_SOVERS_INTERFACE}-${H4_JAVA_SOVERS_RELEASE})
  message (VERBOSE "SOVERSION_JAVA: ${H4_JAVA_SOVERS_MAJOR}.${H4_JAVA_SOVERS_RELEASE}.${H4_JAVA_SOVERS_MINOR}")
endif ()

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
set (HDF4_DF_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
set (HDF4_DF_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
if (H4_MF_SOVERS_EXISTS)
  set (HDF4_MF_PACKAGE_SOVERSION "${H4_MF_SOVERS_MAJOR}.${H4_MF_SOVERS_RELEASE}.${H4_MF_SOVERS_MINOR}")
  set (HDF4_MF_PACKAGE_SOVERSION_MAJOR "${H4_MF_SOVERS_MAJOR}")
else ()
  set (HDF4_MF_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
  set (HDF4_MF_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
endif ()
if (H4_XDR_SOVERS_EXISTS)
  set (HDF4_XDR_PACKAGE_SOVERSION "${H4_XDR_SOVERS_MAJOR}.${H4_XDR_SOVERS_RELEASE}.${H4_XDR_SOVERS_MINOR}")
  set (HDF4_XDR_PACKAGE_SOVERSION_MAJOR "${H4_XDR_SOVERS_MAJOR}")
else ()
  set (HDF4_XDR_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
  set (HDF4_XDR_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
endif ()
if (H4_F_SOVERS_EXISTS)
  set (HDF4_F_PACKAGE_SOVERSION "${H4_F_SOVERS_MAJOR}.${H4_F_SOVERS_RELEASE}.${H4_F_SOVERS_MINOR}")
  set (HDF4_F_PACKAGE_SOVERSION_MAJOR "${H4_F_SOVERS_MAJOR}")
else ()
  set (HDF4_F_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
  set (HDF4_F_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
endif ()
if (H4_MF_F_SOVERS_EXISTS)
  set (HDF4_MF_F_PACKAGE_SOVERSION "${H4_MF_F_SOVERS_MAJOR}.${H4_MF_F_SOVERS_RELEASE}.${H4_MF_F_SOVERS_MINOR}")
  set (HDF4_MF_F_PACKAGE_SOVERSION_MAJOR "${H4_MF_F_SOVERS_MAJOR}")
else ()
  set (HDF4_MF_F_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
  set (HDF4_MF_F_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
endif ()
if (H4_JAVA_SOVERS_EXISTS)
  set (HDF4_JAVA_PACKAGE_SOVERSION "${H4_JAVA_SOVERS_MAJOR}.${H4_JAVA_SOVERS_RELEASE}.${H4_JAVA_SOVERS_MINOR}")
  set (HDF4_JAVA_PACKAGE_SOVERSION_MAJOR "${H4_JAVA_SOVERS_MAJOR}")
else ()
  set (HDF4_JAVA_PACKAGE_SOVERSION "${H4_DF_SOVERS_MAJOR}.${H4_DF_SOVERS_RELEASE}.${H4_DF_SOVERS_MINOR}")
  set (HDF4_JAVA_PACKAGE_SOVERSION_MAJOR "${H4_DF_SOVERS_MAJOR}")
endif ()
set (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION_STRING}")
set (HDF4_PACKAGE_TARNAME "${HDF4_PACKAGE}${HDF_PACKAGE_EXT}")
set (HDF4_PACKAGE_URL "http://www.hdfgroup.org")
set (HDF4_PACKAGE_BUGREPORT "help@hdfgroup.org")

#-----------------------------------------------------------------------------
# Set variables needed for installation
#-----------------------------------------------------------------------------
set (HDF4_VERSION_STRING ${HDF4_PACKAGE_VERSION})
set (HDF4_VERSION_MAJOR  ${HDF4_PACKAGE_VERSION_MAJOR})
set (HDF4_VERSION_MINOR  ${HDF4_PACKAGE_VERSION_MINOR})

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
include (${HDF_RESOURCES_DIR}/HDFMacros.cmake)

HDF_DIR_PATHS("HDF4")

include (${HDF_RESOURCES_DIR}/HDFLibMacros.cmake)
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
# Include directories in the source or build tree should come before other
# directories to prioritize headers in the sources over installed ones.
#-----------------------------------------------------------------------------
set (CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set (HDF4_COMP_INCLUDE_DIRECTORIES)

#-----------------------------------------------------------------------------
# Mac OS X Options
#-----------------------------------------------------------------------------
if (HDF4_BUILD_FRAMEWORKS AND NOT BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries" FORCE)
endif ()

#-----------------------------------------------------------------------------
# Option to Build Shared and Static libs, default is both
#-----------------------------------------------------------------------------
option (HDF4_ONLY_SHARED_LIBS "Only Build Shared Libraries" OFF)
mark_as_advanced (ONLY_SHARED_LIBS)
option (BUILD_STATIC_LIBS "Build Static Libraries" ON)
set (H4_ENABLE_STATIC_LIB NO)
option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set (H4_ENABLE_SHARED_LIB NO)

option (HDF4_BUILD_STATIC_TOOLS "Build Static Tools NOT Shared Tools" OFF)

# only shared libraries/tools is true if user forces static OFF
if (NOT BUILD_STATIC_LIBS)
  set (ONLY_SHARED_LIBS ON CACHE BOOL "Only Build Shared Libraries" FORCE)
endif ()

# only shared libraries is set ON by user then force settings
if (ONLY_SHARED_LIBS)
  set (H4_ENABLE_STATIC_LIB NO)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries" FORCE)
  set (BUILD_STATIC_LIBS OFF CACHE BOOL "Build Static Libraries" FORCE)
  if (HDF4_BUILD_STATIC_TOOLS)
    message (WARNING "Cannot build static tools without static libraries. Building shared tools.")
  endif ()
  set (HDF4_BUILD_STATIC_TOOLS OFF CACHE BOOL "Build Static Tools NOT Shared Tools" FORCE)
endif ()

if (NOT BUILD_SHARED_LIBS AND NOT HDF4_BUILD_STATIC_TOOLS)
  message (WARNING "Cannot build shared tools without shared libraries. Building static tools.")
  set (HDF4_BUILD_STATIC_TOOLS ON CACHE BOOL "Build Static Tools NOT Shared Tools" FORCE)
endif ()

if (BUILD_STATIC_LIBS)
  set (H4_ENABLE_STATIC_LIB YES)
endif ()
if (BUILD_SHARED_LIBS)
  set (H4_ENABLE_SHARED_LIB YES)
endif ()

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_definitions (HAVE_CONFIG_H)

#-----------------------------------------------------------------------------
# Temporary disable optimization flag
#-----------------------------------------------------------------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR CYGWIN)
  foreach (flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "O3")
      string (REGEX REPLACE "O3" "O2" ${flag_var} "${${flag_var}}")
    endif ()
  endforeach ()
endif ()

option (HDF4_ENABLE_ANALYZER_TOOLS "enable the use of Clang tools" OFF)
mark_as_advanced (HDF4_ENABLE_ANALYZER_TOOLS)
if (HDF4_ENABLE_ANALYZER_TOOLS)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/tools.cmake)
endif ()
option (HDF4_ENABLE_SANITIZERS "execute the Clang sanitizer" OFF)
mark_as_advanced (HDF4_ENABLE_SANITIZERS)
if (HDF4_ENABLE_SANITIZERS)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/sanitizers.cmake)
endif ()
option (HDF4_ENABLE_FORMATTERS "format source files" OFF)
mark_as_advanced (HDF4_ENABLE_FORMATTERS)
if (HDF4_ENABLE_FORMATTERS)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/formatting.cmake)
endif ()

#-----------------------------------------------------------------------------
# Option to use code coverage
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_COVERAGE "Enable code coverage for Libraries and Programs" OFF)
if (HDF4_ENABLE_COVERAGE)
  include (${HDF4_SOURCE_DIR}/config/sanitizer/code-coverage.cmake)
  if(CODE_COVERAGE AND CODE_COVERAGE_ADDED)
    message(VERBOSE "Add instrumentation to all targets")
    add_code_coverage () # Adds instrumentation to all targets
  else ()
    message(VERBOSE "Use --coverage option")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
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
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
if (WIN32)
  add_compile_definitions (_CRT_SECURE_NO_WARNINGS)
  if (MSVC)
    add_compile_definitions (_BIND_TO_CURRENT_VCLIBS_VERSION=1 _CONSOLE)
  endif ()
endif ()

option (HDF4_MINGW_STATIC_GCC_LIBS "Statically link libgcc/libstdc++" OFF)

if (MSVC)
  set (CMAKE_MFC_FLAG 0)
  set (WIN_COMPILE_FLAGS "")
  set (WIN_LINK_FLAGS "")
endif ()

include (${HDF_RESOURCES_DIR}/HDFCompilerFlags.cmake)
set (CMAKE_MODULE_PATH ${HDF_RESOURCES_DIR} ${CMAKE_MODULE_PATH})

#-----------------------------------------------------------------------------
# Include user macros
#-----------------------------------------------------------------------------
include (UserMacros.cmake)

#-----------------------------------------------------------------------------
# Include filter (zlib, szip, etc.) macros
#-----------------------------------------------------------------------------
set (HDF4_ENABLE_JPEG_LIB_SUPPORT ON) #Enable libjpeg required
option (HDF4_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF) #SZip is optional
set (HDF4_ENABLE_Z_LIB_SUPPORT ON) #Enable libzlib required
include (CMakeFilters.cmake)

#-----------------------------------------------------------------------------
# Options to build tools
#-----------------------------------------------------------------------------
option (HDF4_BUILD_TOOLS  "Build HDF4 Tools" ON)

#-----------------------------------------------------------------------------
# Add the HDF4 Library Target to the build
#-----------------------------------------------------------------------------
add_subdirectory (hdf/src)
add_subdirectory (mfhdf/src)

#-----------------------------------------------------------------------------
# Option to build documentation
#-----------------------------------------------------------------------------
option (HDF4_BUILD_DOC "Build documentation" OFF)
if (HDF4_BUILD_DOC AND IS_DIRECTORY "${HDF4_DOXYGEN_DIR}")
# check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    option (HDF4_ENABLE_DOXY_WARNINGS "Enable fail if doxygen parsing has warnings." OFF)
    mark_as_advanced (HDF4_ENABLE_DOXY_WARNINGS)
    if (HDF4_ENABLE_DOXY_WARNINGS)
      set (HDF4_DOXY_WARNINGS "FAIL_ON_WARNINGS")
    else ()
      set (HDF4_DOXY_WARNINGS "NO")
    endif ()
    message(VERBOSE "Doxygen version: ${DOXYGEN_VERSION}")
    add_subdirectory (doxygen)
  else ()
    message(WARNING "Doxygen needs to be installed to generate the doxygen documentation")
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
    if (IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/test")
      add_subdirectory (hdf/test)
    endif ()
    if (IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/test")
      add_subdirectory (mfhdf/test)
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to build Fortran bindings/tests
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
# NOTE:
# This interface is UNSAFE on 64-bit systems as the interface attempts to
# store pointers in 32-bit integers.
#-----------------------------------------------------------------------------
# Set default name mangling : overridden by Fortran detection in fortran dir
set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## __")
if (IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran")
  option (HDF4_BUILD_FORTRAN "Build FORTRAN support" OFF)
  if (HDF4_BUILD_FORTRAN)
    if (WIN32)
      set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) NAME")
      set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) NAME")
    endif ()
    message (VERBOSE "Fortran compiler ID is ${CMAKE_Fortran_COMPILER_ID}")
    #include (${HDF_RESOURCES_DIR}/HDFCompilerFortranFlags.cmake)
    include (${HDF_RESOURCES_DIR}/HDF4UseFortran.cmake)

    if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_Fortran_COMPILER_VERSION VERSION_LESS 10.0)
      set (CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -fallow-argument-mismatch")
      set (CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -fallow-argument-mismatch")
    endif ()

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
# Option to build HDF4 Java Library
#-----------------------------------------------------------------------------
if (IS_DIRECTORY "${HDF4_SOURCE_DIR}/java")
  option (HDF4_BUILD_JAVA "Build Java HDF Library" OFF)
  if (HDF4_BUILD_JAVA)
    if (NOT BUILD_SHARED_LIBS)
      message (FATAL_ERROR "\nJava requires shared libraries!\n")
    else ()
      add_subdirectory (java)
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Generate the H4config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
configure_file (${HDF_RESOURCES_DIR}/h4config.h.in ${PROJECT_BINARY_DIR}/h4config.h @ONLY)

#-----------------------------------------------------------------------------
# Option to build examples
#-----------------------------------------------------------------------------
if (IS_DIRECTORY "${HDF4_SOURCE_DIR}/HDF4Examples")
  option (HDF4_BUILD_EXAMPLES  "Build HDF4 Library Examples" ON)
  if (HDF4_BUILD_EXAMPLES AND NOT HDF4_USE_SANITIZER)
    include (${HDF_RESOURCES_DIR}/HDF4ExampleCache.cmake)
    set (HDF4_VERSION ${HDF4_PACKAGE_VERSION})
    add_subdirectory (HDF4Examples)
  endif ()
endif ()

include (CMakeInstallation.cmake)
