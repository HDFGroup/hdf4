cmake_minimum_required (VERSION 3.12)
PROJECT (JPEG C)

#-----------------------------------------------------------------------------
# Basic JPEG stuff here
#-----------------------------------------------------------------------------

set (JPEG_PACKAGE_EXT ${HDF_PACKAGE_EXT})
set (HDF_USE_GNU_DIRS ${HDF4_USE_GNU_DIRS})
set (CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
set (CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE})
set (PACKAGE_NAMESPACE ${HDF_PACKAGE_NAMESPACE})
if (MINGW)
  set (WINDOWS 1) # MinGW tries to imitate Windows
endif ()
if (WINDOWS)
  set (HAVE_STDDEF_H 1)
  set (HAVE_SYS_TYPES_H 1)
endif ()
set (HAVE_PROTOTYPES 1) # jpeg
CHECK_INCLUDE_FILE_CONCAT ("sys/types.h"     HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("stddef.h"        HAVE_STDDEF_H)

#----------------------------------------------------------------
# JPEG special check
#----------------------------------------------------------------
CHECK_TYPE_SIZE ("unsigned char"           UNSIGNED_CHAR)
CHECK_TYPE_SIZE ("unsigned short"          UNSIGNED_SHORT)

CHECK_INCLUDE_FILE_CONCAT ("string.h"        HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT ("strings.h"       HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT ("stdlib.h"        HAVE_STDLIB_H)

# _Bool type support
CHECK_INCLUDE_FILE_CONCAT (stdbool.h    HAVE_STDBOOL_H)

#-----------------------------------------------------------------------------
# Check for some functions that are used
#
CHECK_FUNCTION_EXISTS (void              HAVE_VOID)
CHECK_FUNCTION_EXISTS (const             HAVE_CONST)

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
set (JPEG_RESOURCES_DIR          ${HDF_RESOURCES_DIR}/JPEG)
set (JPEG_SRC_DIR                ${JPEG_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Set the core names of all the libraries
#-----------------------------------------------------------------------------
set (JPEG_LIB_CORENAME           "jpeg")

#-----------------------------------------------------------------------------
# Set the true names of all the libraries if customized by external project
#-----------------------------------------------------------------------------
set (JPEG_LIB_NAME              "${JPEG_EXTERNAL_LIB_PREFIX}${JPEG_LIB_CORENAME}")

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
set (JPEG_LIB_TARGET             "${JPEG_LIB_CORENAME}-static")

#-----------------------------------------------------------------------------
# parse the full version number from jpeglib.h and include in JPEG_FULL_VERSION
#-----------------------------------------------------------------------------
file (READ ${JPEG_SRC_DIR}/jpeglib.h _jpeglib_h_contents)
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_FULL_VERSION ${_jpeglib_h_contents})
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION_MAJOR[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_VERS_MAJOR ${_jpeglib_h_contents})
string (REGEX REPLACE ".*#define[ \t]+JPEG_LIB_VERSION_MINOR[ \t]+([0-9][0-9]*).*$"
    "\\1" JPEG_VERS_MINOR ${_jpeglib_h_contents})
#message (STATUS "VERSION: ${JPEG_FULL_VERSION}")

#set (JPEG_VERS_MAJOR 9)
#set (JPEG_VERS_MINOR 0)
set (JPEG_VERS_RELEASE 0)

#-----------------------------------------------------------------------------
set (JPEG_PACKAGE "jpeg")
set (JPEG_PACKAGE_NAME "JPEG")
set (JPEG_PACKAGE_VERSION "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}")
set (JPEG_PACKAGE_VERSION_MAJOR "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}")
set (JPEG_PACKAGE_VERSION_MINOR "${JPEG_VERS_RELEASE}")
set (JPEG_PACKAGE_STRING "${JPEG_PACKAGE_NAME} ${JPEG_PACKAGE_VERSION}")
set (JPEG_PACKAGE_TARNAME "${JPEG_PACKAGE_NAME}${HDF_PACKAGE_EXT}")
set (JPEG_PACKAGE_URL "http://www.hdfgroup.org")
set (JPEG_PACKAGE_BUGREPORT "help@hdfgroup.org")
set (JPEG_PACKAGE_SOVERSION "${JPEG_VERS_MAJOR}.${JPEG_VERS_MINOR}.${JPEG_VERS_RELEASE}")
set (JPEG_PACKAGE_SOVERSION_MAJOR "${JPEG_VERS_MAJOR}")

HDF_DIR_PATHS(${JPEG_PACKAGE_NAME})

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects
#-----------------------------------------------------------------------------
if (NOT JPEG_EXPORTED_TARGETS)
  set (JPEG_EXPORTED_TARGETS "jpeg-targets")
endif ()

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME,
# add it to this variable. This is NOT used by Make Install, but for projects
# which include JPEG as a sub-project within their build tree
#-----------------------------------------------------------------------------
set_global_variable (JPEG_LIBRARIES_TO_EXPORT "")

#-----------------------------------------------------------------------------
# Mac OS X Options
#-----------------------------------------------------------------------------
if (JPEG_BUILD_FRAMEWORKS AND NOT BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries")
endif ()

set (CMAKE_POSITION_INDEPENDENT_CODE ON)

#-----------------------------------------------------------------------------
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
set (EXE_EXT "")
if (WIN32)
  set (EXE_EXT ".exe")
  add_definitions (-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS)
  add_definitions (-D_CONSOLE)
endif ()

if (MSVC)
  set (CMAKE_MFC_FLAG 0)
endif ()

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -Wno-strict-prototypes")
endif ()
if (CMAKE_C_COMPILER_ID MATCHES "IntelLLVM" OR CMAKE_C_COMPILER_ID MATCHES "[Cc]lang")
   set (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -Wno-deprecated-non-prototype -Wno-strict-prototypes")
endif ()

#-----------------------------------------------------------------------------
# This is in here to help some of the GCC based IDES like Eclipse
# and code blocks parse the compiler errors and warnings better.
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif ()

#-----------------------------------------------------------------------------
set (MEMORYMGR jmemnobs)
option (ENABLE_MEMORYMGR "Select memory manager" OFF)
if (ENABLE_MEMORYMGR)
  set (MEMORYMGR jmemname)
endif ()

#-----------------------------------------------------------------------------
# All libs/tests/examples need the main include directories
#-----------------------------------------------------------------------------
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${JPEG_BINARY_DIR};${JPEG_SOURCE_DIR};${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

set (JPEG_SRCS
    ${JPEG_SRC_DIR}/jaricom.c
    ${JPEG_SRC_DIR}/jcapimin.c
    ${JPEG_SRC_DIR}/jcapistd.c
    ${JPEG_SRC_DIR}/jcarith.c
    ${JPEG_SRC_DIR}/jccoefct.c
    ${JPEG_SRC_DIR}/jccolor.c
    ${JPEG_SRC_DIR}/jcdctmgr.c
    ${JPEG_SRC_DIR}/jchuff.c
    ${JPEG_SRC_DIR}/jcinit.c
    ${JPEG_SRC_DIR}/jcmainct.c
    ${JPEG_SRC_DIR}/jcmarker.c
    ${JPEG_SRC_DIR}/jcmaster.c
    ${JPEG_SRC_DIR}/jcomapi.c
    ${JPEG_SRC_DIR}/jcparam.c
    ${JPEG_SRC_DIR}/jcprepct.c
    ${JPEG_SRC_DIR}/jcsample.c
    ${JPEG_SRC_DIR}/jctrans.c
    ${JPEG_SRC_DIR}/jdapimin.c
    ${JPEG_SRC_DIR}/jdapistd.c
    ${JPEG_SRC_DIR}/jdarith.c
    ${JPEG_SRC_DIR}/jdatadst.c
    ${JPEG_SRC_DIR}/jdatasrc.c
    ${JPEG_SRC_DIR}/jdcoefct.c
    ${JPEG_SRC_DIR}/jdcolor.c
    ${JPEG_SRC_DIR}/jddctmgr.c
    ${JPEG_SRC_DIR}/jdhuff.c
    ${JPEG_SRC_DIR}/jdinput.c
    ${JPEG_SRC_DIR}/jdmainct.c
    ${JPEG_SRC_DIR}/jdmarker.c
    ${JPEG_SRC_DIR}/jdmaster.c
    ${JPEG_SRC_DIR}/jdmerge.c
    ${JPEG_SRC_DIR}/jdpostct.c
    ${JPEG_SRC_DIR}/jdsample.c
    ${JPEG_SRC_DIR}/jdtrans.c
    ${JPEG_SRC_DIR}/jerror.c
    ${JPEG_SRC_DIR}/jfdctflt.c
    ${JPEG_SRC_DIR}/jfdctfst.c
    ${JPEG_SRC_DIR}/jfdctint.c
    ${JPEG_SRC_DIR}/jidctflt.c
    ${JPEG_SRC_DIR}/jidctfst.c
    ${JPEG_SRC_DIR}/jidctint.c
    ${JPEG_SRC_DIR}/jquant1.c
    ${JPEG_SRC_DIR}/jquant2.c
    ${JPEG_SRC_DIR}/jutils.c
    ${JPEG_SRC_DIR}/jmemmgr.c
    ${JPEG_SRC_DIR}/${MEMORYMGR}.c
)

set (JPEG_HDRS
    ${JPEG_SRC_DIR}/jdct.h
    ${JPEG_SRC_DIR}/jinclude.h
    ${JPEG_SRC_DIR}/jmemsys.h
    ${JPEG_SRC_DIR}/jpegint.h
    ${JPEG_SRC_DIR}/jversion.h
)

set (JPEG_PUBLIC_HEADERS
    ${JPEG_SRC_DIR}/jerror.h
    ${JPEG_SRC_DIR}/jmorecfg.h
    ${JPEG_SRC_DIR}/jpeglib.h
)

add_library (${JPEG_LIB_TARGET} STATIC ${JPEG_SRCS} ${JPEG_HDRS} ${JPEG_PUBLIC_HEADERS})
target_include_directories(${JPEG_LIB_TARGET} PRIVATE "${JPEG_SRC_DIR};${CMAKE_BINARY_DIR}")
TARGET_C_PROPERTIES (${JPEG_LIB_TARGET} STATIC)
target_link_libraries (${JPEG_LIB_TARGET} PRIVATE ${LINK_LIBS})
H4_SET_LIB_OPTIONS (${JPEG_LIB_TARGET} ${JPEG_LIB_NAME} STATIC)
set_target_properties(${JPEG_LIB_TARGET} PROPERTIES
    PUBLIC_HEADER ""
    LINKER_LANGUAGE C
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
set_global_variable (JPEG_LIBRARIES_TO_EXPORT ${JPEG_LIB_TARGET})
set (install_targets ${JPEG_LIB_TARGET})

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (JPEG_EXPORTED_TARGETS)
  INSTALL_TARGET_PDB (${JPEG_LIB_TARGET} ${JPEG_INSTALL_BIN_DIR} libraries)

  install (
      TARGETS
          ${install_targets}
      EXPORT
          ${JPEG_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
      ARCHIVE DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
      RUNTIME DESTINATION ${JPEG_INSTALL_BIN_DIR} COMPONENT libraries
      FRAMEWORK DESTINATION ${JPEG_INSTALL_FWRK_DIR} COMPONENT libraries
      PUBLIC_HEADER DESTINATION ${JPEG_INSTALL_INCLUDE_DIR} COMPONENT headers
  )
endif ()

#-----------------------------------------------------------------------------
# Generate the jconfig.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
configure_file (${JPEG_RESOURCES_DIR}/jconfig.h.in ${JPEG_BINARY_DIR}/jconfig.h @ONLY)

include (CMakePackageConfigHelpers)

#-----------------------------------------------------------------------------
# Check for Installation Utilities
#-----------------------------------------------------------------------------
if (WIN32)
  set (PF_ENV_EXT "(x86)")
  find_program (NSIS_EXECUTABLE NSIS.exe PATHS "$ENV{ProgramFiles}\\NSIS" "$ENV{ProgramFiles${PF_ENV_EXT}}\\NSIS")
  if(NOT CPACK_WIX_ROOT)
    file(TO_CMAKE_PATH "$ENV{WIX}" CPACK_WIX_ROOT)
  endif ()
  find_program (WIX_EXECUTABLE candle  PATHS "${CPACK_WIX_ROOT}/bin")
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  install (
      EXPORT ${JPEG_EXPORTED_TARGETS}
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}
      FILE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${PACKAGE_NAMESPACE}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  export (
      TARGETS ${JPEG_LIBRARIES_TO_EXPORT} ${JPEG_LIB_DEPENDENCIES}
      FILE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${PACKAGE_NAMESPACE}
  )
  export (PACKAGE ${JPEG_PACKAGE}${JPEG_PACKAGE_EXT})
endif ()

#-----------------------------------------------------------------------------
# Set includes needed for build
#-----------------------------------------------------------------------------
set (JPEG_INCLUDES_BUILD_TIME
    ${JPEG_SRC_DIR} ${JPEG_BINARY_DIR}
)

#-----------------------------------------------------------------------------
# Set variables needed for installation
#-----------------------------------------------------------------------------
set (JPEG_VERSION_STRING ${JPEG_PACKAGE_VERSION})
set (JPEG_VERSION_MAJOR  ${JPEG_PACKAGE_VERSION_MAJOR})
set (JPEG_VERSION_MINOR  ${JPEG_PACKAGE_VERSION_MINOR})

#-----------------------------------------------------------------------------
# Configure the jpeg-config.cmake file for the build directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${JPEG_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/${JPEG_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
configure_package_config_file (
    ${JPEG_RESOURCES_DIR}/jpeg-config.cmake.in
    "${JPEG_BINARY_DIR}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${JPEG_INSTALL_CMAKE_DIR}"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
    INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

#-----------------------------------------------------------------------------
# Configure the jpeg-config.cmake file for the install directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${JPEG_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${JPEG_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_INSTALL_PREFIX}" )
configure_package_config_file (
    ${JPEG_RESOURCES_DIR}/jpeg-config.cmake.in
    "${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${JPEG_INSTALL_CMAKE_DIR}"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
)
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  install (
      FILES ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config.cmake
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Configure the jpeg-config-version.cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  configure_file (
      ${JPEG_RESOURCES_DIR}/jpeg-config-version.cmake.in
      ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config-version.cmake @ONLY
  )
  install (
      FILES ${JPEG_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${JPEG_PACKAGE}${JPEG_PACKAGE_EXT}-config-version.cmake
      DESTINATION ${JPEG_INSTALL_CMAKE_DIR}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Check for Installation Utilities
#-----------------------------------------------------------------------------
if (WIN32)
  set (PF_ENV_EXT "(x86)")
  find_program (NSIS_EXECUTABLE NSIS.exe PATHS "$ENV{ProgramFiles}\\NSIS" "$ENV{ProgramFiles${PF_ENV_EXT}}\\NSIS")
  if(NOT CPACK_WIX_ROOT)
    file(TO_CMAKE_PATH "$ENV{WIX}" CPACK_WIX_ROOT)
  endif()
  find_program (WIX_EXECUTABLE candle  PATHS "${CPACK_WIX_ROOT}/bin")
endif ()

#-----------------------------------------------------------------------------
# Configure the README file for the binary package
#-----------------------------------------------------------------------------
configure_file (${JPEG_SOURCE_DIR}/README ${JPEG_BINARY_DIR}/JPEG_README @ONLY)

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
if (NOT JPEG_EXTERNALLY_CONFIGURED)
  set (CPACK_PACKAGE_VENDOR "HDF_Group")
  set (CPACK_PACKAGE_NAME "${JPEG_PACKAGE_NAME}")
  if (NOT WIN32 OR JPEG_VERS_SUBRELEASE MATCHES "^[0-9]+$")
    set (CPACK_PACKAGE_VERSION "${JPEG_PACKAGE_VERSION_STRING}")
  else ()
    set (CPACK_PACKAGE_VERSION "${JPEG_PACKAGE_VERSION}")
  endif ()
  set (CPACK_PACKAGE_VERSION_MAJOR "${JPEG_PACKAGE_VERSION_MAJOR}")
  set (CPACK_PACKAGE_VERSION_MINOR "${JPEG_PACKAGE_VERSION_MINOR}")
  set (CPACK_PACKAGE_VERSION_PATCH "")
  set (CPACK_RESOURCE_FILE_LICENSE "${JPEG_BINARY_DIR}/JPEG_README")
  set (CPACK_PACKAGE_DESCRIPTION_FILE "${JPEG_BINARY_DIR}/JPEG_README")
  set (CPACK_RESOURCE_FILE_README "${JPEG_BINARY_DIR}/JPEG_README")
  set (CPACK_PACKAGE_RELOCATABLE TRUE)
  set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "jpeg9e Installation")
  set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${CPACK_PACKAGE_VERSION}")

  set (CPACK_GENERATOR "TGZ")
  if (WIN32)
    set (CPACK_GENERATOR "ZIP")

    if (NSIS_EXECUTABLE)
      list (APPEND CPACK_GENERATOR "NSIS")
    endif ()
    # Installers for 32- vs. 64-bit CMake:
    #  - Root install directory (displayed to end user at installer-run time)
    #  - "NSIS package/display name" (text used in the installer GUI)
    #  - Registry key used to store info about the installation
    set (CPACK_NSIS_PACKAGE_NAME "${JPEG_PACKAGE_STRING}")
    if (CMAKE_CL_64)
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} (Win64)")
    else ()
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    endif ()
    set (CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_VERSION}")
    set (CPACK_NSIS_CONTACT "${JPEG_PACKAGE_BUGREPORT}")
    set (CPACK_NSIS_MODIFY_PATH ON)
    set (CPACK_NSIS_PACKAGE_NAME "JPEG ${JPEG_PACKAGE_VERSION}")
    if (WIX_EXECUTABLE)
      list (APPEND CPACK_GENERATOR "WIX")
    endif ()
#WiX variables
    set (CPACK_WIX_UNINSTALL "1")
    set (CPACK_RESOURCE_FILE_LICENSE "${JPEG_BINARY_DIR}/JPEG_README")
  elseif (APPLE)
    list (APPEND CPACK_GENERATOR "STGZ")
    list (APPEND CPACK_GENERATOR "DragNDrop")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    #set (CPACK_PACKAGE_ICON "${JPEG_RESOURCES_DIR}/hdf.icns")

    option (JPEG_PACK_MACOSX_FRAMEWORK  "Package the JPEG Library in a Framework" OFF)
    if (JPEG_PACK_MACOSX_FRAMEWORK AND JPEG_BUILD_FRAMEWORKS)
      list (APPEND CPACK_GENERATOR "Bundle")
      set (CPACK_BUNDLE_NAME "${JPEG_PACKAGE_STRING}")
      set (CPACK_BUNDLE_LOCATION "/")    # make sure CMAKE_INSTALL_PREFIX ends in /
      set (CMAKE_INSTALL_PREFIX "/${CPACK_BUNDLE_NAME}.framework/Versions/${CPACK_PACKAGE_VERSION}/${CPACK_PACKAGE_NAME}/")
      set (CPACK_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      #-----------------------------------------------------------------------------
      # Configure the Info.plist file for the install bundle
      #-----------------------------------------------------------------------------
      configure_file (
          ${JPEG_RESOURCES_DIR}/CPack.Info.plist.in
          ${JPEG_BINARY_DIR}/CMakeFiles/Info.plist @ONLY
      )
      configure_file (
          ${JPEG_RESOURCES_DIR}/PkgInfo.in
          ${JPEG_BINARY_DIR}/CMakeFiles/PkgInfo @ONLY
      )
      install (
          FILES ${JPEG_BINARY_DIR}/CMakeFiles/PkgInfo
          DESTINATION ..
      )
    endif ()
  else ()
    list (APPEND CPACK_GENERATOR "STGZ")
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    set (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "${JPEG_PACKAGE_BUGREPORT}")

#      list (APPEND CPACK_GENERATOR "RPM")
    set (CPACK_RPM_PACKAGE_RELEASE "1")
    set (CPACK_RPM_COMPONENT_INSTALL ON)
    set (CPACK_RPM_PACKAGE_RELOCATABLE ON)
  endif ()

  # By default, do not warn when built on machines using only VS Express:
  if (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
  endif ()
  include (InstallRequiredSystemLibraries)

  set (CPACK_INSTALL_CMAKE_PROJECTS "${JPEG_BINARY_DIR};JPEG;libraries;/")
  set (CPACK_INSTALL_CMAKE_PROJECTS "${JPEG_BINARY_DIR};JPEG;configinstall;/")

  set (CPACK_ALL_INSTALL_TYPES Full Developer User)
  set (CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Everything")

  set(CPACK_COMPONENTS_ALL libraries headers documents configinstall)

  include (CPack)

  cpack_add_component_group(Runtime)

  cpack_add_component (libraries
      DISPLAY_NAME "JPEG Libraries"
      REQUIRED
      GROUP Runtime
      INSTALL_TYPES Full Developer User
  )
  cpack_add_component (configinstall
      DISPLAY_NAME "JPEG CMake files"
      DEPENDS libraries
      GROUP Development
      INSTALL_TYPES Full Developer User
  )

endif ()
