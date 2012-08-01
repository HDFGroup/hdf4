cmake_minimum_required (VERSION 2.8.6)
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
  ELSE (NOT WIN32)
    SET (HDF4_INSTALL_DATA_DIR ".")
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
SET (HDF4_PACKAGE_STRING "${HDF4_PACKAGE_NAME} ${HDF4_PACKAGE_VERSION}-${H4_VERS_SUBRELEASE}")
SET (HDF4_PACKAGE_TARNAME "hdf")
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
# Option to use legacy naming for windows libs/programs, default is legacy
#-----------------------------------------------------------------------------
IF (WIN32 AND NOT CYGWIN)
  OPTION (HDF_LEGACY_NAMING "Use Legacy Names for Libraries and Programs" ON)
ENDIF (WIN32 AND NOT CYGWIN)

#-----------------------------------------------------------------------------
# Option to Build Shared/Static libs, default is static
#-----------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  ADD_DEFINITIONS (-DH4_BUILT_AS_DYNAMIC_LIB)
ELSE (BUILD_SHARED_LIBS)
  ADD_DEFINITIONS (-DH4_BUILT_AS_STATIC_LIB)
  IF (NOT WIN32)
    # should this be a user setting : Everyone uses it anyway ?
    ADD_DEFINITIONS (-DPIC)
  ENDIF (NOT WIN32)
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
# Include the main src and config directories
#-----------------------------------------------------------------------------
SET (HDF4_INCLUDE_DIRECTORIES
    ${HDF4_HDFSOURCE_DIR} 
    ${HDF4_MFHDFSOURCE_DIR} 
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
  IF (NOT CYGWIN)
    ADD_DEFINITIONS (-DBIND_TO_CURRENT_VCLIBS_VERSION=1)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS (-D_CONSOLE)
  ENDIF (NOT CYGWIN)
ENDIF (WIN32)

IF (MSVC)
  SET (CMAKE_MFC_FLAG 0)
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
  SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (CMAKE_COMPILER_IS_GNUCXX)
  SET (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS} -fomit-frame-pointer -finline-functions -fno-common")
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
  ENDIF (MPI_C_FOUND)
ENDIF (HDF4_ENABLE_PARALLEL)

# Parallel IO usage requires MPI to be Linked and Included
IF (H4_HAVE_PARALLEL)
  SET (LINK_LIBS ${LINK_LIBS} ${MPI_C_LIBRARIES})
  IF (MPI_C_LINK_FLAGS)
    SET (CMAKE_EXE_LINKER_FLAGS ${MPI_C_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  ENDIF (MPI_C_LINK_FLAGS)
ENDIF (H4_HAVE_PARALLEL)

INCLUDE (ExternalProject)
OPTION (HDF4_ALLOW_EXTERNAL_SUPPORT "Allow External Library Building" "NO")
OPTION (JPEG_USE_EXTERNAL "Use External Library Building for JPEG" 0)
OPTION (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 0)
OPTION (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 0)
IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  SET (JPEG_USE_EXTERNAL 1)
  SET (ZLIB_USE_EXTERNAL 1)
  SET (SZIP_USE_EXTERNAL 1)
  IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN")
    SET (JPEG_URL ${JPEG_SVN_URL})
    SET (ZLIB_URL ${ZLIB_SVN_URL})
    SET (SZIP_URL ${SZIP_SVN_URL})
  ELSEIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    IF (NOT TGZ_PATH)
      SET (TGZ_PATH ${HDF4_SOURCE_DIR})
    ENDIF (NOT TGZ_PATH)
    SET (JPEG_URL ${TGZ_PATH}/${JPEG_TGZ_NAME})
    SET (ZLIB_URL ${TGZ_PATH}/${ZLIB_TGZ_NAME})
    SET (SZIP_URL ${TGZ_PATH}/${SZIP_TGZ_NAME})
  ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN")
ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")

#-----------------------------------------------------------------------------
# Option for LibJpeg support
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_JPEG_LIB_SUPPORT "Enable libjpeg" ON)
IF (HDF4_ENABLE_JPEG_LIB_SUPPORT)
  IF (NOT H4_JPEGLIB_HEADER)
    IF (NOT JPEG_USE_EXTERNAL)
      FIND_PACKAGE (JPEG NAMES ${JPEG_PACKAGE_NAME}${HDF_PACKAGE_EXT})
    ENDIF (NOT JPEG_USE_EXTERNAL)
    IF (JPEG_FOUND)
      SET (JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIR})
      SET (H4_HAVE_JPEGLIB_H 1)
      SET (H4_HAVE_LIBJPEG 1)
      SET (H4_JPEGLIB_HEADER "jpeglib.h")
      SET (JPEG_INCLUDE_DIR_GEN ${JPEG_INCLUDE_DIR})
    ELSE (JPEG_FOUND)
      IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        # May need to build JPEG with PIC on x64 machines with gcc
        # Need to use CMAKE_ANSI_CFLAGS define so that compiler test works
        IF (BUILD_JPEG_WITH_PIC)
          SET (JPEG_CMAKE_C_FLAGS "-fPIC")
        ELSE (BUILD_JPEG_WITH_PIC)
          IF (DEFINED CMAKE_ANSI_CFLAGS)
            SET (JPEG_CMAKE_C_FLAGS ${CMAKE_ANSI_CFLAGS})
          ELSE (DEFINED CMAKE_ANSI_CFLAGS)
            SET (JPEG_CMAKE_C_FLAGS " ")
          ENDIF (DEFINED CMAKE_ANSI_CFLAGS)
        ENDIF (BUILD_JPEG_WITH_PIC)

        EXTERNAL_JPEG_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE} ${JPEG_CMAKE_C_FLAGS})
        SET (H4_HAVE_JPEGLIB_H 1)
        SET (H4_HAVE_LIBJPEG 1)
        SET (H4_JPEGLIB_HEADER "jpeglib.h")
        MESSAGE (STATUS "JPEGLIB is built")
      ELSE (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        MESSAGE (FATAL_ERROR " JPEGLib is Required for JPEGLib support in HDF4")
      ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    ENDIF (JPEG_FOUND)
  ELSE (NOT H4_JPEGLIB_HEADER)
    # This project is being called from within another and JPEGLib is already configured
    SET (H4_HAVE_JPEGLIB_H 1)
  ENDIF (NOT H4_JPEGLIB_HEADER)
  SET (LINK_LIBS ${LINK_LIBS} ${JPEG_LIBRARY})
  INCLUDE_DIRECTORIES (${JPEG_INCLUDE_DIR})
  MESSAGE (STATUS "JPEGLIB is ON")
ENDIF (HDF4_ENABLE_JPEG_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" ON)
IF (HDF4_ENABLE_Z_LIB_SUPPORT)
  IF (NOT H4_ZLIB_HEADER)
    IF (NOT ZLIB_USE_EXTERNAL)
      FIND_PACKAGE (ZLIB NAMES ${ZLIB_PACKAGE_NAME}${HDF_PACKAGE_EXT})
    ENDIF (NOT ZLIB_USE_EXTERNAL)
    IF (ZLIB_FOUND)
      SET (H4_HAVE_FILTER_DEFLATE 1)
      SET (H4_HAVE_ZLIB_H 1)
      SET (H4_HAVE_LIBZ 1)
      SET (H4_ZLIB_HEADER "zlib.h")
      SET (ZLIB_INCLUDE_DIR_GEN ${ZLIB_INCLUDE_DIR})
    ELSE (ZLIB_FOUND)
      IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        EXTERNAL_ZLIB_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE})
        SET (H4_HAVE_FILTER_DEFLATE 1)
        SET (H4_HAVE_ZLIB_H 1)
        SET (H4_HAVE_LIBZ 1)
        MESSAGE (STATUS "Filter ZLIB is built")
      ELSE (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        MESSAGE (FATAL_ERROR " ZLib is Required for ZLib support in HDF4")
      ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    ENDIF (ZLIB_FOUND)
  ELSE (NOT H4_ZLIB_HEADER)
    # This project is being called from within another and ZLib is already configured
    SET (H4_HAVE_FILTER_DEFLATE 1)
    SET (H4_HAVE_ZLIB_H 1)
    SET (H4_HAVE_LIBZ 1)
  ENDIF (NOT H4_ZLIB_HEADER)
  SET (LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES})
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIR})
  MESSAGE (STATUS "Filter ZLIB is ON")
ENDIF (HDF4_ENABLE_Z_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
OPTION (HDF4_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)
IF (HDF4_ENABLE_SZIP_SUPPORT)
  OPTION (HDF4_ENABLE_SZIP_ENCODING "Use SZip Encoding" OFF)
  IF (NOT SZIP_USE_EXTERNAL)
    FIND_PACKAGE (SZIP NAMES ${SZIP_PACKAGE_NAME}${HDF_PACKAGE_EXT})
  ENDIF (NOT SZIP_USE_EXTERNAL)
  IF (SZIP_FOUND)
    SET (H4_HAVE_FILTER_SZIP 1)
    SET (H4_HAVE_SZLIB_H 1)
    SET (H4_HAVE_LIBSZ 1)
    SET (SZIP_INCLUDE_DIR_GEN ${SZIP_INCLUDE_DIR})
  ELSE (SZIP_FOUND)
    IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      EXTERNAL_SZIP_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE} ${HDF4_ENABLE_SZIP_ENCODING})
      SET (H4_HAVE_FILTER_SZIP 1)
      SET (H4_HAVE_SZLIB_H 1)
      SET (H4_HAVE_LIBSZ 1)
      MESSAGE (STATUS "Filter SZIP is built")
    ELSE (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      MESSAGE (FATAL_ERROR "SZIP is Required for SZIP support in HDF4")
    ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  ENDIF (SZIP_FOUND)
  SET (LINK_LIBS ${LINK_LIBS} ${SZIP_LIBRARIES})
  INCLUDE_DIRECTORIES (${SZIP_INCLUDE_DIR})
  MESSAGE (STATUS "Filter SZIP is ON")
  IF (HDF4_ENABLE_SZIP_ENCODING)
    SET (H4_HAVE_SZIP_ENCODER 1)
  ENDIF (HDF4_ENABLE_SZIP_ENCODING)
ENDIF (HDF4_ENABLE_SZIP_SUPPORT)

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
# Add the HDF4 Library Target to the build
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

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
IF (NOT HDF4_INSTALL_NO_DEVELOPMENT)
  INSTALL (
      FILES ${PROJECT_BINARY_DIR}/h4config.h
      DESTINATION ${HDF4_INSTALL_INCLUDE_DIR}
      COMPONENT headers
  )
ENDIF (NOT HDF4_INSTALL_NO_DEVELOPMENT)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  INSTALL (
      EXPORT ${HDF4_EXPORTED_TARGETS}
      DESTINATION ${HDF4_INSTALL_DATA_DIR}/cmake/hdf4
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
      COMPONENT configinstall
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  EXPORT (
      TARGETS ${HDF4_LIBRARIES_TO_EXPORT} ${HDF4_LIB_DEPENDENCIES}
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf4-config.cmake file for the build directory
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
    ${HDF4_RESOURCES_DIR}/hdf4-config.cmake.build.in 
    ${HDF4_BINARY_DIR}/hdf4-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the HDF4-config.cmake file for the install directory
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  CONFIGURE_FILE (
      ${HDF4_RESOURCES_DIR}/hdf4-config.cmake.install.in
      ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}/cmake/hdf4
      COMPONENT configinstall
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf4-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  CONFIGURE_FILE (
      ${HDF4_RESOURCES_DIR}/hdf4-config-version.cmake.in
      ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}/cmake/hdf4
      COMPONENT configinstall
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Add Document File(s) to CMake Install
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  INSTALL (
      FILES
          ${HDF4_SOURCE_DIR}/COPYING
          ${HDF4_SOURCE_DIR}/README.txt
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  IF (EXISTS "${HDF4_SOURCE_DIR}/release_notes" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/release_notes")
    SET (release_files
        ${HDF4_SOURCE_DIR}/release_notes/CMake.txt
        ${HDF4_SOURCE_DIR}/release_notes/Using_CMake.txt
        ${HDF4_SOURCE_DIR}/release_notes/HISTORY.txt
        ${HDF4_SOURCE_DIR}/release_notes/INSTALL
        ${HDF4_SOURCE_DIR}/release_notes/RELEASE.txt
    )
    IF (WIN32 AND NOT CYGWIN)
      SET (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL_WINDOWS.txt
      )
    ELSE (WIN32 AND NOT CYGWIN)
      SET (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL_CYGWIN.txt
      )
    ENDIF (WIN32 AND NOT CYGWIN)
    INSTALL (
        FILES ${release_files}
        DESTINATION ${HDF4_INSTALL_DATA_DIR}/release_notes
        COMPONENT hdfdocuments
    )
  ENDIF (EXISTS "${HDF4_SOURCE_DIR}/release_notes" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/release_notes")
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED AND NOT HDF4_NO_PACKAGES)
  SET (CPACK_PACKAGE_VENDOR "HDF_Group")
  SET (CPACK_PACKAGE_NAME "${HDF4_PACKAGE_NAME}")
  SET (CPACK_PACKAGE_INSTALL_DIRECTORY "HDF4")
  SET (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION}")
  SET (CPACK_PACKAGE_VERSION_MAJOR "${HDF4_PACKAGE_VERSION_MAJOR}")
  SET (CPACK_PACKAGE_VERSION_MINOR "${HDF4_PACKAGE_VERSION_MINOR}")
  SET (CPACK_PACKAGE_VERSION_PATCH "")
  SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  SET (CPACK_PACKAGE_RELOCATABLE TRUE)
  SET (CPACK_MONOLITHIC_INSTALL ON)

  IF (WIN32)
    SET (CPACK_MONOLITHIC_INSTALL ON)
    SET (CPACK_NSIS_CONTACT "${HDF4_PACKAGE_BUGREPORT}")
    SET (CPACK_NSIS_MODIFY_PATH ON)
    SET (CPACK_NSIS_PACKAGE_NAME "HDF4 ${HDF4_PACKAGE_VERSION}")
  ELSE (WIN32)
    SET (CPACK_PACKAGING_INSTALL_PREFIX "/usr")
    SET (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    SET (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    SET (CPACK_DEBIAN_PACKAGE_MAINTAINER "${HDF4_PACKAGE_BUGREPORT}")
    
    SET (CPACK_RPM_COMPONENT_INSTALL ON)
    SET (CPACK_RPM_PACKAGE_RELOCATABLE ON)
    SET (CPACK_RPM_PACKAGE_LICENSE "BSD-style")
    SET (CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    SET (CPACK_RPM_PACKAGE_URL "${HDF4_PACKAGE_URL}")
  ENDIF (WIN32)

  INCLUDE(InstallRequiredSystemLibraries)

  SET (CPACK_INSTALL_CMAKE_PROJECTS "${HDF4_BINARY_DIR};HDF4;ALL;/")
  
  IF (HDF4_PACKAGE_EXTLIBS)
    IF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      IF (JPEG_FOUND AND JPEG_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;libraries;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;headers;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;configinstall;/")
      ENDIF (JPEG_FOUND AND JPEG_USE_EXTERNAL)
      IF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;libraries;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;headers;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;configinstall;/")
      ENDIF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
      IF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;libraries;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;headers;/")
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;configinstall;/")
      ENDIF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
    ENDIF (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  ENDIF (HDF4_PACKAGE_EXTLIBS)

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
  CPACK_ADD_COMPONENT (hdfdocuments 
      DISPLAY_NAME "HDF4 Documents"
      GROUP Documents
  )
  CPACK_ADD_COMPONENT (configinstall 
      DISPLAY_NAME "HDF4 CMake files" 
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
        DEPENDS libraries
        GROUP Applications
    )
    CPACK_ADD_COMPONENT (toolsheaders 
        DISPLAY_NAME "HDF4 Tools Headers" 
        DEPENDS libraries
        GROUP Development
    )
  ENDIF (HDF4_BUILD_TOOLS)

  IF (HDF4_BUILD_UTILS)
    CPACK_ADD_COMPONENT (utilsapplications 
        DISPLAY_NAME "HDF4 Utility Applications" 
        DEPENDS libraries
        GROUP Applications
    )
    CPACK_ADD_COMPONENT (utilsheaders 
        DISPLAY_NAME "HDF4 Utility Headers" 
        DEPENDS libraries
        GROUP Development
    )
  ENDIF (HDF4_BUILD_UTILS)
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED AND NOT HDF4_NO_PACKAGES)
  