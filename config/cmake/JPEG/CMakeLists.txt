cmake_minimum_required (VERSION 3.2.2)
PROJECT (JPEG C)

#-----------------------------------------------------------------------------
# Basic JPEG stuff here
#-----------------------------------------------------------------------------

set (BUILD_SHARED_LIBS OFF)
set (JPEG_PACKAGE_EXT ${HDF_PACKAGE_EXT})
#set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
#set (CMAKE_DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
#set (CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
#set (CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_PDB_OUTPUT_DIRECTORY})
set (CMAKE_ANSI_CFLAGS ${jpeg_pic})
set (HDF_USE_GNU_DIRS ${HDF4_USE_GNU_DIRS})
#set (CMAKE_OSX_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
#set (CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE})

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
# Mac OS X Options
#-----------------------------------------------------------------------------
if (JPEG_BUILD_FRAMEWORKS AND NOT BUILD_SHARED_LIBS)
  set (BUILD_SHARED_LIBS ON CACHE BOOL "Build Shared Libraries")
endif ()

#-----------------------------------------------------------------------------
# Option to Build Shared and Static libs, default is shared
#-----------------------------------------------------------------------------
option (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
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

set (MAKE_SYSTEM)
if (CMAKE_BUILD_TOOL MATCHES "make")
  set (MAKE_SYSTEM 1)
endif ()

set (CFG_INIT "/${CMAKE_CFG_INTDIR}")
if (MAKE_SYSTEM)
  set (CFG_INIT "")
endif ()

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
endif ()
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_LOADED)
  set (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS} -fomit-frame-pointer -finline-functions -fno-common")
endif ()

#-----------------------------------------------------------------------------
# This is in here to help some of the GCC based IDES like Eclipse
# and code blocks parse the compiler errors and warnings better.
#-----------------------------------------------------------------------------
if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif ()
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_LOADED)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif ()

#-----------------------------------------------------------------------------
set (MEMORYMGR jmemnobs)
option (ENABLE_MEMORYMGR "Select memory manager" OFF)
if (ENABLE_MEMORYMGR)
  set (MEMORYMGR jmemname)
endif ()

#-----------------------------------------------------------------------------
# Include user macros
#-----------------------------------------------------------------------------
#include (UserMacros.cmake)

#-----------------------------------------------------------------------------
# All libs/tests/examples need the main include directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${JPEG_BINARY_DIR} ${JPEG_SOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

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
    PUBLIC_HEADER "${JPEG_PUBLIC_HEADERS}"
    LINKER_LANGUAGE C
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
set_global_variable (JPEG_LIBRARIES_TO_EXPORT ${JPEG_LIB_TARGET})
set (install_targets ${JPEG_LIB_TARGET})

if (BUILD_SHARED_LIBS)
  add_library (${JPEG_LIBSH_TARGET} SHARED ${JPEG_SRCS} ${JPEG_HDRS} ${JPEG_PUBLIC_HEADERS})
  target_include_directories(${JPEG_LIBSH_TARGET} PRIVATE "${JPEG_SRC_DIR};${CMAKE_BINARY_DIR}")
  TARGET_C_PROPERTIES (${JPEG_LIBSH_TARGET} SHARED)
  target_link_libraries (${JPEG_LIBSH_TARGET} PRIVATE ${LINK_LIBS})
  H4_SET_LIB_OPTIONS (${JPEG_LIBSH_TARGET} ${JPEG_LIB_NAME} SHARED)
  set_target_properties(${JPEG_LIBSH_TARGET} PROPERTIES
      COMPILE_DEFINITIONS "JPEG_BUILT_AS_DYNAMIC_LIB"
      PUBLIC_HEADER "${JPEG_PUBLIC_HEADERS}"
      LINKER_LANGUAGE C
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS JPEG_BUILT_AS_DYNAMIC_LIB=1
  )
  set_global_variable (JPEG_LIBRARIES_TO_EXPORT "${JPEG_LIBRARIES_TO_EXPORT};${JPEG_LIBSH_TARGET}")
  set (install_targets ${install_targets} ${JPEG_LIBSH_TARGET})
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${JPEG_LIBSH_TARGET} ${JPEG_INSTALL_BIN_DIR} libraries)
endif (BUILD_SHARED_LIBS)
INSTALL_TARGET_PDB (${JPEG_LIB_TARGET} ${JPEG_INSTALL_BIN_DIR} libraries)

install (
    TARGETS
        ${install_targets}
    LIBRARY DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
    ARCHIVE DESTINATION ${JPEG_INSTALL_LIB_DIR} COMPONENT libraries
    RUNTIME DESTINATION ${JPEG_INSTALL_BIN_DIR} COMPONENT libraries
    FRAMEWORK DESTINATION ${JPEG_INSTALL_FWRK_DIR} COMPONENT libraries
    PUBLIC_HEADER DESTINATION ${JPEG_INSTALL_INCLUDE_DIR} COMPONENT headers
)

#-----------------------------------------------------------------------------
# Generate the jconfig.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
configure_file (${JPEG_RESOURCES_DIR}/jconfig.h.in ${JPEG_BINARY_DIR}/jconfig.h @ONLY)

#-----------------------------------------------------------------------------
# Set includes needed for build
#-----------------------------------------------------------------------------
set (JPEG_INCLUDES_BUILD_TIME
    ${JPEG_SRC_DIR} ${JPEG_BINARY_DIR}
)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
install (
    FILES ${PROJECT_BINARY_DIR}/jconfig.h
    DESTINATION ${JPEG_INSTALL_INCLUDE_DIR}
    COMPONENT headers
)
