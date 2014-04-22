cmake_minimum_required (VERSION 2.8.10)
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
# Allow Visual Studio solution directories
#-----------------------------------------------------------------------------
# Provide a way for Visual Studio Express users to turn OFF the new FOLDER
# organization feature. Default to ON for non-Express users. Express users must
# explicitly turn off this option to build HDF5 in the Express IDE...
#
OPTION (HDF4_USE_FOLDERS "Enable folder grouping of projects in IDEs." ON)
MARK_AS_ADVANCED (HDF4_USE_FOLDERS)
IF (HDF4_USE_FOLDERS)
  SET_PROPERTY (GLOBAL PROPERTY USE_FOLDERS ON)
ENDIF (HDF4_USE_FOLDERS)
OPTION (HDF4_NO_PACKAGES "CPACK - Disable packaging" OFF)
MARK_AS_ADVANCED (HDF4_NO_PACKAGES)

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
SET (HDF4_HDF_TEST_FCSTUB_LIB_CORENAME     "hdf_test_fcstub")
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
SET (HDF4_HDF_TEST_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_HDF_TEST_LIB_CORENAME}")
SET (HDF4_HDF_TEST_FCSTUB_LIB_NAME     "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_HDF_TEST_FCSTUB_LIB_CORENAME}")
SET (HDF4_MF_FCSTUB_LIB_NAME    "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FCSTUB_LIB_CORENAME}")
SET (HDF4_MF_FORTRAN_LIB_NAME   "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_FORTRAN_LIB_CORENAME}")
SET (HDF4_MF_TEST_LIB_NAME      "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_MF_TEST_LIB_CORENAME}")
SET (HDF4_TOOLS_LIB_NAME        "${HDF4_EXTERNAL_LIB_PREFIX}${HDF4_TOOLS_LIB_CORENAME}")

#-----------------------------------------------------------------------------
# Set the target names of all the libraries
#-----------------------------------------------------------------------------
SET (HDF4_LIB_TARGET              ${HDF4_LIB_CORENAME})
SET (HDF4_SRC_LIB_TARGET          ${HDF4_SRC_LIB_CORENAME})
SET (HDF4_SRC_FCSTUB_LIB_TARGET   ${HDF4_SRC_FCSTUB_LIB_CORENAME})
SET (HDF4_SRC_FORTRAN_LIB_TARGET  ${HDF4_SRC_FORTRAN_LIB_CORENAME})
SET (HDF4_MF_LIB_TARGET           ${HDF4_MF_LIB_CORENAME})
SET (HDF4_MF_XDR_LIB_TARGET       ${HDF4_MF_XDR_LIB_CORENAME})
SET (HDF4_HDF_TEST_LIB_TARGET     ${HDF4_HDF_TEST_LIB_CORENAME})
SET (HDF4_HDF_TEST_FCSTUB_LIB_TARGET     ${HDF4_HDF_TEST_FCSTUB_LIB_CORENAME})
SET (HDF4_MF_FCSTUB_LIB_TARGET    ${HDF4_MF_FCSTUB_LIB_CORENAME})
SET (HDF4_MF_FORTRAN_LIB_TARGET   ${HDF4_MF_FORTRAN_LIB_CORENAME})
SET (HDF4_MF_TEST_LIB_TARGET      ${HDF4_MF_TEST_LIB_CORENAME})
SET (HDF4_TOOLS_LIB_TARGET        ${HDF4_TOOLS_LIB_CORENAME})

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
SET (HDF4_RESOURCES_DIR       ${HDF4_SOURCE_DIR}/config/cmake)
SET (HDF4_HDFSOURCE_DIR       ${HDF4_SOURCE_DIR}/hdf/src)
SET (HDF4_HDF_TESTSOURCE_DIR  ${HDF4_SOURCE_DIR}/hdf/test)
SET (HDF4_MFHDFSOURCE_DIR     ${HDF4_SOURCE_DIR}/mfhdf/libsrc)
SET (HDF4_MFHDF_TEST_DIR      ${HDF4_SOURCE_DIR}/mfhdf/test)
SET (HDF4_MFHDF_FORTRAN_DIR   ${HDF4_SOURCE_DIR}/mfhdf/fortran)
SET (HDF4_MFHDF_XDR_DIR       ${HDF4_SOURCE_DIR}/mfhdf/xdr)

IF (NOT HDF4_INSTALL_BIN_DIR)
  SET (HDF4_INSTALL_BIN_DIR bin)
  SET (HDF4_INSTALL_UTILS_BIN_DIR ${HDF4_INSTALL_BIN_DIR}) #${HDF4_INSTALL_BIN_DIR}/utils
  SET (HDF4_INSTALL_TOOLS_BIN_DIR ${HDF4_INSTALL_BIN_DIR}) #${HDF4_INSTALL_BIN_DIR}/tools
ENDIF (NOT HDF4_INSTALL_BIN_DIR)
IF (NOT HDF4_INSTALL_LIB_DIR)
  SET (HDF4_INSTALL_LIB_DIR lib)
ENDIF (NOT HDF4_INSTALL_LIB_DIR)
IF (NOT HDF4_INSTALL_INCLUDE_DIR)
  SET (HDF4_INSTALL_INCLUDE_DIR include)
ENDIF (NOT HDF4_INSTALL_INCLUDE_DIR)
IF (NOT HDF4_INSTALL_DATA_DIR)
  IF (NOT WIN32)
    SET (HDF4_INSTALL_DATA_DIR share)
    SET (HDF4_INSTALL_CMAKE_DIR share/cmake)
  ELSE (NOT WIN32)
    SET (HDF4_INSTALL_DATA_DIR ".")
    SET (HDF4_INSTALL_CMAKE_DIR cmake)
  ENDIF (NOT WIN32)
ENDIF (NOT HDF4_INSTALL_DATA_DIR)

#-----------------------------------------------------------------------------
# parse the full version number from hfile.h and include in H4_VERS_INFO
#-----------------------------------------------------------------------------
FILE (READ ${HDF4_HDFSOURCE_DIR}/hfile.h _hfile_h_contents)
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
# Basic HDF4 stuff here
#-----------------------------------------------------------------------------
SET (HDF4_PACKAGE "hdf4")
SET (HDF4_PACKAGE_NAME "HDF")
SET (HDF4_PACKAGE_VERSION "${H4_VERS_MAJOR}.${H4_VERS_MINOR}.${H4_VERS_RELEASE}")
SET (HDF4_PACKAGE_VERSION_MAJOR "${H4_VERS_MAJOR}.${H4_VERS_MINOR}")
SET (HDF4_PACKAGE_VERSION_MINOR "${H4_VERS_RELEASE}")
SET (HDF4_PACKAGE_VERSION_STRING "${HDF4_PACKAGE_VERSION}")
IF (NOT "${H4_VERS_SUBRELEASE}" STREQUAL "")
  SET (HDF4_PACKAGE_VERSION_STRING "${HDF4_PACKAGE_VERSION_STRING}-${H4_VERS_SUBRELEASE}")
ENDIF (NOT "${H4_VERS_SUBRELEASE}" STREQUAL "")
SET (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION_STRING}")
SET (HDF5_PACKAGE_TARNAME "${HDF4_PACKAGE}${HDF_PACKAGE_EXT}")
SET (HDF4_PACKAGE_URL "http://www.hdfgroup.org")
SET (HDF4_PACKAGE_BUGREPORT "help@hdfgroup.org")

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
INCLUDE (${HDF4_RESOURCES_DIR}/HDFMacros.cmake)
INCLUDE (${HDF4_RESOURCES_DIR}/HDFLibMacros.cmake)
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
SET (EXTERNAL_HEADER_LIST "")
SET (EXTERNAL_LIBRARY_LIST "")
SET (EXTERNAL_LIBRARYDLL_LIST "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
INCLUDE (${HDF4_RESOURCES_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Option to Build Shared/Static libs, default is static
#-----------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
SET (H4_ENABLE_SHARED_LIB NO)
SET (H4_ENABLE_STATIC_LIB NO)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  SET (H4_BUILT_AS_DYNAMIC_LIB 1)
  SET (H4_ENABLE_SHARED_LIB YES)
ELSE (BUILD_SHARED_LIBS)
  SET (H4_BUILT_AS_STATIC_LIB 1)
  SET (H4_ENABLE_STATIC_LIB YES)
  IF (NOT WIN32)
    # should this be a user setting : Everyone uses it anyway ?
    ADD_DEFINITIONS (-DPIC)
    IF (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
      ADD_DEFINITIONS (-KPIC)
    ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  ENDIF (NOT WIN32)
ENDIF (BUILD_SHARED_LIBS)

ADD_DEFINITIONS (-DBIG_LONGS -DSWAP)
ADD_DEFINITIONS (-DHAVE_CONFIG_H)

#-----------------------------------------------------------------------------
# Temporary disable optimization flag
#-----------------------------------------------------------------------------
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR CYGWIN)
  FOREACH (flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    IF (${flag_var} MATCHES "O3")
      STRING (REGEX REPLACE "O3" "O0" ${flag_var} "${${flag_var}}")
    ENDIF (${flag_var} MATCHES "O3")
  ENDFOREACH (flag_var)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR CYGWIN)

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
# Option to use deprecated public API symbols
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_DEPRECATED_SYMBOLS "Enable deprecated public API symbols" ON)
IF (HDF4_ENABLE_DEPRECATED_SYMBOLS)
  SET (H4_NO_DEPRECATED_SYMBOLS 0)   
ELSE (HDF4_ENABLE_DEPRECATED_SYMBOLS)
  SET (H4_NO_DEPRECATED_SYMBOLS 1)   
ENDIF (HDF4_ENABLE_DEPRECATED_SYMBOLS)

#-----------------------------------------------------------------------------
# Include the main src and config directories
#-----------------------------------------------------------------------------
SET (HDF4_INCLUDE_DIRECTORIES
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
SET (EXE_EXT "")
IF (WIN32)
  SET (EXE_EXT ".exe")
  IF (NOT CYGWIN)
    ADD_DEFINITIONS (-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS (-D_CONSOLE)
  ENDIF (NOT CYGWIN)
ENDIF (WIN32)

IF (MSVC)
  SET (CMAKE_MFC_FLAG 0)
  SET (WIN_COMPILE_FLAGS "")
  SET (WIN_LINK_FLAGS "")
ENDIF (MSVC)

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
  #-- NMake Makefiles will overwhelm the console with warnings if -Wall is used.
  IF (NOT WIN32)
    ADD_DEFINITIONS (-Wall)
  ENDIF (NOT WIN32)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
  IF (CMAKE_BUILD_TYPE MATCHES Debug)
    SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -finline-functions -fno-common")
  ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
    SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
  ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (CMAKE_COMPILER_IS_GNUCXX)
  IF (CMAKE_BUILD_TYPE MATCHES Debug)
    SET (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS} -finline-functions -fno-common")
  ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
    SET (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS} -fomit-frame-pointer -finline-functions -fno-common")
  ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

#-----------------------------------------------------------------------------
# Option to allow the user to disable compiler warnings
#-----------------------------------------------------------------------------
OPTION (HDF4_DISABLE_COMPILER_WARNINGS "Disable compiler warnings" OFF)
IF (HDF4_DISABLE_COMPILER_WARNINGS)
  # MSVC uses /w to suppress warnings.  It also complains if another
  # warning level is given, so remove it.
  IF (MSVC)
    SET (HDF4_WARNINGS_BLOCKED 1)
    STRING (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
    STRING (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
  ENDIF (MSVC)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
  ENDIF (WIN32 AND NOT CYGWIN)
  # Borland uses -w- to suppress warnings.
  IF (BORLAND)
    SET (HDF4_WARNINGS_BLOCKED 1)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w-")
  ENDIF (BORLAND)

  # Most compilers use -w to suppress warnings.
  IF (NOT HDF4_WARNINGS_BLOCKED)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
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
# Option to Enable MPI Parallel
#-----------------------------------------------------------------------------
SET (CMAKE_MODULE_PATH ${HDF4_RESOURCES_DIR} ${CMAKE_MODULE_PATH})
OPTION (HDF4_ENABLE_PARALLEL "Enable parallel build (requires MPI)" OFF)
IF (HDF4_ENABLE_PARALLEL)
  INCLUDE (FindMPI)
  INCLUDE_DIRECTORIES (${MPI_C_INCLUDE_PATH})
  IF (MPI_C_FOUND)
    SET (H4_HAVE_PARALLEL 1)
    # MPI checks, only do these if MPI_C_FOUND is true, otherwise they always fail
    # and once set, they are cached as false and not regenerated
    SET (CMAKE_REQUIRED_LIBRARIES "${MPI_C_LIBRARIES}" )
    CHECK_FUNCTION_EXISTS (MPI_File_get_size H4_HAVE_MPI_GET_SIZE) 
    # Used by Fortran + MPI
    CHECK_SYMBOL_EXISTS (MPI_Comm_c2f "${MPI_C_INCLUDE_PATH}/mpi.h"  H4_HAVE_MPI_MULTI_LANG_Comm)
    CHECK_SYMBOL_EXISTS (MPI_Info_c2f "${MPI_C_INCLUDE_PATH}/mpi.h"  H4_HAVE_MPI_MULTI_LANG_Info)
  ELSE (MPI_C_FOUND)
    MESSAGE (STATUS "Parallel libraries not found")
  ENDIF (MPI_C_FOUND)
ENDIF (HDF4_ENABLE_PARALLEL)

# Parallel IO usage requires MPI to be Linked and Included
IF (H4_HAVE_PARALLEL)
  SET (LINK_LIBS ${LINK_LIBS} ${MPI_C_LIBRARIES})
  IF (MPI_C_LINK_FLAGS)
    SET (CMAKE_EXE_LINKER_FLAGS ${MPI_C_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  ENDIF (MPI_C_LINK_FLAGS)
ENDIF (H4_HAVE_PARALLEL)

#-----------------------------------------------------------------------------
# Include user macros
#-----------------------------------------------------------------------------
INCLUDE (UserMacros.cmake)

INCLUDE (CMakeFilters.cmake)

#-----------------------------------------------------------------------------
# Option for external libraries on windows
#-----------------------------------------------------------------------------
OPTION (HDF4_PACKAGE_EXTLIBS "CPACK - include external libraries" OFF)
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  IF (HDF4_PACKAGE_EXTLIBS)
    SET (HDF4_NO_PACKAGES OFF CACHE BOOL "CPACK - Disable packaging" FORCE)
    IF (HDF4_ENABLE_JPEG_LIB_SUPPORT AND JPEG_FOUND)
      PACKAGE_JPEG_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    ENDIF (HDF4_ENABLE_JPEG_LIB_SUPPORT AND JPEG_FOUND)

    IF (HDF4_ENABLE_Z_LIB_SUPPORT AND ZLIB_FOUND)
      PACKAGE_ZLIB_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    ENDIF (HDF4_ENABLE_Z_LIB_SUPPORT AND ZLIB_FOUND)
    
    IF (HDF4_ENABLE_SZIP_SUPPORT AND SZIP_FOUND)
      PACKAGE_SZIP_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
    ENDIF (HDF4_ENABLE_SZIP_SUPPORT AND SZIP_FOUND)
  ENDIF (HDF4_PACKAGE_EXTLIBS)
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Option to build HDF4 xdr Library
#-----------------------------------------------------------------------------
OPTION (HDF4_BUILD_XDR_LIB  "Build HDF4 XDR Library" OFF)
IF (WIN32 AND NOT CYGWIN)
  SET (HDF4_BUILD_XDR_LIB ON CACHE BOOL "Build HDF4 XDR Library" FORCE)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/xdr ${PROJECT_BINARY_DIR}/xdr)
ENDIF (HDF4_BUILD_XDR_LIB)

#-----------------------------------------------------------------------------
# Add the HDF4 Library Target to the build
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf/src    ${PROJECT_BINARY_DIR}/hdf/src )
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/libsrc   ${PROJECT_BINARY_DIR}/mfhdf/libsrc)

IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  IF (JPEG_FOUND AND JPEG_USE_EXTERNAL)
    ADD_DEPENDENCIES (${HDF4_SRC_LIB_TARGET} JPEG)
  ENDIF (JPEG_FOUND AND JPEG_USE_EXTERNAL)
  IF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
    ADD_DEPENDENCIES (${HDF4_SRC_LIB_TARGET} ZLIB)
  ENDIF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
  IF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
    ADD_DEPENDENCIES (${HDF4_SRC_LIB_TARGET} SZIP)
  ENDIF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")

IF (HDF4_BUILD_XDR_LIB)
  ADD_DEPENDENCIES (${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET})
ENDIF (HDF4_BUILD_XDR_LIB)

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
    IF (EXISTS "${HDF4_SOURCE_DIR}/hdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/test")
      ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf/test   ${PROJECT_BINARY_DIR}/hdf/test)
    ENDIF (EXISTS "${HDF4_SOURCE_DIR}/hdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/test")
    IF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/test")
      ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/test ${PROJECT_BINARY_DIR}/mfhdf/test)
    ENDIF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/test" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/test")
  ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)
  INCLUDE (${HDF4_SOURCE_DIR}/CTestConfig.cmake)
  CONFIGURE_FILE (${HDF4_RESOURCES_DIR}/CTestCustom.cmake ${HDF4_BINARY_DIR}/CTestCustom.ctest @ONLY)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Option to build Fortran bindings/tests/examples
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
#-----------------------------------------------------------------------------
# Set default name mangling : overridden by Fortran detection in fortran dir
SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## __")
IF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran")
  OPTION (HDF4_BUILD_FORTRAN "Build FORTRAN support" ON)
  IF (HDF4_BUILD_FORTRAN)
    IF (WIN32 AND NOT CYGWIN)
      SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) NAME")
      SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) NAME")
    ENDIF (WIN32 AND NOT CYGWIN)
    INCLUDE (${HDF4_RESOURCES_DIR}/HDF4UseFortran.cmake)
    ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/fortran ${PROJECT_BINARY_DIR}/mfhdf/fortran)
    ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf/fortran ${PROJECT_BINARY_DIR}/hdf/fortran)
  ENDIF (HDF4_BUILD_FORTRAN)
ENDIF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran")

IF (MSVC)
  SET (H4_HDSETVBUF "HDsetvbuf(F,S,M,Z) (((Z)>1)?setvbuf(F,S,M,Z):setvbuf(F,S,M,2))")
ELSE (MSVC)
  SET (H4_HDSETVBUF "NOP")
ENDIF (MSVC)

#-----------------------------------------------------------------------------
# Library utilities
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf   ${PROJECT_BINARY_DIR}/hdf)
ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf ${PROJECT_BINARY_DIR}/mfhdf)

#-----------------------------------------------------------------------------
# Option to build HDF4 Examples
#-----------------------------------------------------------------------------
IF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/examples")
  OPTION (HDF4_BUILD_EXAMPLES  "Build HDF4 Examples" OFF)
  IF (HDF4_BUILD_EXAMPLES)
    IF (HDF4_BUILD_FORTRAN)
      IF (EXISTS "${HDF4_SOURCE_DIR}/hdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/fortran/examples")
        ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf/fortran/examples     ${PROJECT_BINARY_DIR}/hdf/fortran/examples)
      ENDIF (EXISTS "${HDF4_SOURCE_DIR}/hdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/hdf/fortran/examples")
      IF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples")
        ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/fortran/examples     ${PROJECT_BINARY_DIR}/mfhdf/fortran/examples)
      ENDIF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/fortran/examples")
    ENDIF (HDF4_BUILD_FORTRAN)

    ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/hdf/examples    ${PROJECT_BINARY_DIR}/hdf/examples)
    ADD_SUBDIRECTORY (${HDF4_SOURCE_DIR}/mfhdf/examples    ${PROJECT_BINARY_DIR}/mfhdf/examples)
  ENDIF (HDF4_BUILD_EXAMPLES)
ENDIF (EXISTS "${HDF4_SOURCE_DIR}/mfhdf/examples" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/mfhdf/examples")

#-----------------------------------------------------------------------------
# Generate the H4config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
CONFIGURE_FILE (${HDF4_RESOURCES_DIR}/h4config.h.in      ${PROJECT_BINARY_DIR}/h4config.h @ONLY)

INCLUDE (CMakeInstallation.cmake)
