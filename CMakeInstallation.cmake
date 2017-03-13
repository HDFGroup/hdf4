include (CMakePackageConfigHelpers)

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
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT HDF4_INSTALL_NO_DEVELOPMENT)
  install (
      FILES ${PROJECT_BINARY_DIR}/h4config.h
      DESTINATION ${HDF4_INSTALL_INCLUDE_DIR}
      COMPONENT headers
  )
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  install (
      EXPORT ${HDF4_EXPORTED_TARGETS}
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${HDF4_PACKAGE}::
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  export (
      TARGETS ${HDF4_LIBRARIES_TO_EXPORT} ${HDF4_LIB_DEPENDENCIES} ${HDF4_UTILS_TO_EXPORT}
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
      NAMESPACE ${HDF4_PACKAGE}::
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Set includes needed for build
#-----------------------------------------------------------------------------
set (HDF4_INCLUDES_BUILD_TIME
    ${HDF4_HDF_SOURCE_DIR}
    ${HDF4_MFHDF_SOURCE_DIR}
    ${HDF4_MFHDF_XDR_DIR}
    ${HDF4_BINARY_DIR}
)

#-----------------------------------------------------------------------------
# Set variables needed for installation
#-----------------------------------------------------------------------------
set (HDF4_VERSION_STRING ${HDF4_PACKAGE_VERSION})
set (HDF4_VERSION_MAJOR  ${HDF4_PACKAGE_VERSION_MAJOR})
set (HDF4_VERSION_MINOR  ${HDF4_PACKAGE_VERSION_MINOR})

#-----------------------------------------------------------------------------
# Configure the hdf4-config.cmake file for the build directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${HDF4_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/${HDF4_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}" )
configure_package_config_file (
    ${HDF_RESOURCES_DIR}/hdf4-config.cmake.in
    "${HDF4_BINARY_DIR}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${HDF4_INSTALL_CMAKE_DIR}"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
    INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

#-----------------------------------------------------------------------------
# Configure the hdf4-config.cmake file for the install directory
#-----------------------------------------------------------------------------
set (INCLUDE_INSTALL_DIR ${HDF4_INSTALL_INCLUDE_DIR})
set (SHARE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${HDF4_INSTALL_CMAKE_DIR}" )
set (CURRENT_BUILD_DIR "${CMAKE_INSTALL_PREFIX}" )
configure_package_config_file (
    ${HDF_RESOURCES_DIR}/hdf4-config.cmake.in
    "${HDF4_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake"
    INSTALL_DESTINATION "${HDF4_INSTALL_CMAKE_DIR}"
    PATH_VARS INCLUDE_INSTALL_DIR SHARE_INSTALL_DIR CURRENT_BUILD_DIR
)
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  install (
      FILES ${HDF4_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Configure the hdf4-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  configure_file (
      ${HDF_RESOURCES_DIR}/hdf4-config-version.cmake.in
      ${HDF4_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}
      COMPONENT configinstall
  )
endif ()

#-----------------------------------------------------------------------------
# Configure the libhdf4.settings file for the lib info
#-----------------------------------------------------------------------------
if (H4_WORDS_BIGENDIAN)
  set (BYTESEX big-endian)
else ()
  set (BYTESEX little-endian)
endif ()
configure_file (
    ${HDF_RESOURCES_DIR}/libhdf4.settings.cmake.in
    ${HDF4_BINARY_DIR}/libhdf4.settings @ONLY
)
install (
    FILES ${HDF4_BINARY_DIR}/libhdf4.settings
    DESTINATION ${HDF4_INSTALL_LIB_DIR}
    COMPONENT libraries
)

#-----------------------------------------------------------------------------
# Configure the HDF4_Examples.cmake file and the examples
#-----------------------------------------------------------------------------
option (HDF4_PACK_EXAMPLES  "Package the HDF4 Library Examples Compressed File" OFF)
if (HDF4_PACK_EXAMPLES)
  configure_file (
      ${HDF_RESOURCES_DIR}/HDF4_Examples.cmake.in
      ${HDF4_BINARY_DIR}/HDF4_Examples.cmake @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/HDF4_Examples.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  if (EXISTS "${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}
    )
    install (
      DIRECTORY ${HDF4_BINARY_DIR}/HDF4Examples
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      USE_SOURCE_PERMISSIONS
      COMPONENT hdfdocuments
    )
  endif ()
  install (
      FILES
          ${HDF4_SOURCE_DIR}/release_notes/USING_CMake_Examples.txt
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  install (
      FILES
          ${HDF_RESOURCES_DIR}/CTestScript.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  install (
      FILES
          ${HDF_RESOURCES_DIR}/HDF4_Examples_options.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
endif ()

#-----------------------------------------------------------------------------
# Configure the README.txt file for the binary package
#-----------------------------------------------------------------------------
HDF_README_PROPERTIES(HDF4_BUILD_FORTRAN)

#-----------------------------------------------------------------------------
# Configure the COPYING.txt file for the windows binary package
#-----------------------------------------------------------------------------
if (WIN32)
  configure_file (${HDF4_SOURCE_DIR}/COPYING ${HDF4_BINARY_DIR}/COPYING.txt @ONLY)
endif ()

#-----------------------------------------------------------------------------
# Add Document File(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  install (
      FILES
          ${HDF4_SOURCE_DIR}/COPYING
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  if (EXISTS "${HDF4_SOURCE_DIR}/release_notes" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/release_notes")
    set (release_files
        ${HDF4_SOURCE_DIR}/release_notes/USING_HDF4_CMake.txt
        ${HDF4_SOURCE_DIR}/release_notes/RELEASE.txt
    )
    if (WIN32)
      set (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/USING_HDF4_VS.txt
      )
    endif ()
    if (HDF4_PACK_INSTALL_DOCS)
      set (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL_CMake.txt
          ${HDF4_SOURCE_DIR}/release_notes/HISTORY.txt
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL
      )
      if (WIN32)
        set (release_files
            ${release_files}
            ${HDF4_SOURCE_DIR}/release_notes/INSTALL_Windows.txt
        )
      endif ()
      if (CYGWIN)
        set (release_files
            ${release_files}
            ${HDF4_SOURCE_DIR}/release_notes/INSTALL_Cygwin.txt
        )
      endif ()
    endif ()
    install (
        FILES ${release_files}
        DESTINATION ${HDF4_INSTALL_DATA_DIR}
        COMPONENT hdfdocuments
    )
  endif ()
endif ()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (CMAKE_HOST_UNIX)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/HDF_Group/${HDF4_PACKAGE_NAME}/${HDF4_PACKAGE_VERSION}"
      CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
  else ()
    GetDefaultWindowsPrefixBase(CMAKE_GENERIC_PROGRAM_FILES)
    set (CMAKE_INSTALL_PREFIX
      "${CMAKE_GENERIC_PROGRAM_FILES}/HDF_Group/${HDF4_PACKAGE_NAME}/${HDF4_PACKAGE_VERSION}"
      CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
    set (CMAKE_GENERIC_PROGRAM_FILES)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED AND NOT HDF4_NO_PACKAGES)
  set (CPACK_PACKAGE_VENDOR "HDF_Group")
  set (CPACK_PACKAGE_NAME "${HDF4_PACKAGE_NAME}")
  if (CDASH_LOCAL)
    set (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION}")
  else ()
    set (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION_STRING}")
  endif ()
  set (CPACK_PACKAGE_VERSION_MAJOR "${HDF4_PACKAGE_VERSION_MAJOR}")
  set (CPACK_PACKAGE_VERSION_MINOR "${HDF4_PACKAGE_VERSION_MINOR}")
  set (CPACK_PACKAGE_VERSION_PATCH "")
  set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  if (EXISTS "${HDF4_SOURCE_DIR}/release_notes")
    set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
    set (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  endif ()
  set (CPACK_PACKAGE_RELOCATABLE TRUE)
  if (OVERRIDE_INSTALL_VERSION)
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${OVERRIDE_INSTALL_VERSION}")
  else ()
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${CPACK_PACKAGE_VERSION}")
  endif ()
  set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.bmp")

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
    set (CPACK_NSIS_PACKAGE_NAME "${HDF4_PACKAGE_STRING}")
    if (CMAKE_CL_64)
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} (Win64)")
    else ()
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    endif ()
    # set the install/unistall icon used for the installer itself
    # There is a bug in NSI that does not handle full unix paths properly.
    set (CPACK_NSIS_MUI_ICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.ico")
    set (CPACK_NSIS_MUI_UNIICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.ico")
    # set the package header icon for MUI
    set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.bmp")
    set (CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}")
    if (OVERRIDE_INSTALL_VERSION)
      set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${OVERRIDE_INSTALL_VERSION}")
    else ()
      set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_VERSION}")
    endif ()
    set (CPACK_NSIS_CONTACT "${HDF4_PACKAGE_BUGREPORT}")
    set (CPACK_NSIS_MODIFY_PATH ON)

    if (WIX_EXECUTABLE)
      list (APPEND CPACK_GENERATOR "WIX")
    endif ()
#WiX variables
    set (CPACK_WIX_UNINSTALL "1")
# .. variable:: CPACK_WIX_LICENSE_RTF
#  RTF License File
#
#  If CPACK_RESOURCE_FILE_LICENSE has an .rtf extension it is used as-is.
#
#  If CPACK_RESOURCE_FILE_LICENSE has an .txt extension it is implicitly
#  converted to RTF by the WiX Generator.
#  The expected encoding of the .txt file is UTF-8.
#
#  With CPACK_WIX_LICENSE_RTF you can override the license file used by the
#  WiX Generator in case CPACK_RESOURCE_FILE_LICENSE is in an unsupported
#  format or the .txt -> .rtf conversion does not work as expected.
    set (CPACK_RESOURCE_FILE_LICENSE "${HDF4_BINARY_DIR}/COPYING.txt")
# .. variable:: CPACK_WIX_PRODUCT_ICON
#  The Icon shown next to the program name in Add/Remove programs.
    set(CPACK_WIX_PRODUCT_ICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.ico")
#
# .. variable:: CPACK_WIX_UI_BANNER
#
#  The bitmap will appear at the top of all installer pages other than the
#  welcome and completion dialogs.
#
#  If set, this image will replace the default banner image.
#
#  This image must be 493 by 58 pixels.
#
# .. variable:: CPACK_WIX_UI_DIALOG
#
#  Background bitmap used on the welcome and completion dialogs.
#
#  If this variable is set, the installer will replace the default dialog
#  image.
#
#  This image must be 493 by 312 pixels.
#
    set(CPACK_WIX_PROPERTY_ARPCOMMENTS "Hierarchical Data Format (HDF) Software Library and Utilities")
    set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${HDF4_PACKAGE_URL}")
    set(CPACK_WIX_PROPERTY_ARPHELPLINK "${HDF4_PACKAGE_BUGREPORT}")
    if (BUILD_SHARED_LIBS)
      set(CPACK_WIX_PATCH_FILE "${HDF_RESOURCES_DIR}/patch.xml")
    endif ()
  elseif (APPLE)
    list (APPEND CPACK_GENERATOR "STGZ")
    list (APPEND CPACK_GENERATOR "DragNDrop")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.icns")

    option (HDF4_PACK_MACOSX_FRAMEWORK  "Package the HDF Library in a Framework" OFF)
    if (HDF4_PACK_MACOSX_FRAMEWORK AND HDF4_BUILD_FRAMEWORKS)
      set (CPACK_BUNDLE_NAME "${HDF4_PACKAGE_STRING}")
      set (CPACK_BUNDLE_LOCATION "/")    # make sure CMAKE_INSTALL_PREFIX ends in /
      set (CMAKE_INSTALL_PREFIX "/${CPACK_BUNDLE_NAME}.framework/Versions/${CPACK_PACKAGE_VERSION}/${CPACK_PACKAGE_NAME}/")
      set (CPACK_BUNDLE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.icns")
      set (CPACK_BUNDLE_PLIST "${HDF4_BINARY_DIR}/CMakeFiles/Info.plist")
      set (CPACK_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      #-----------------------------------------------------------------------------
      # Configure the Info.plist file for the install bundle
      #-----------------------------------------------------------------------------
      configure_file (
          ${HDF_RESOURCES_DIR}/CPack.Info.plist.in
          ${HDF4_BINARY_DIR}/CMakeFiles/Info.plist @ONLY
      )
      configure_file (
          ${HDF_RESOURCES_DIR}/PkgInfo.in
          ${HDF4_BINARY_DIR}/CMakeFiles/PkgInfo @ONLY
      )
      configure_file (
          ${HDF_RESOURCES_EXT_DIR}/version.plist.in
          ${HDF4_BINARY_DIR}/CMakeFiles/version.plist @ONLY
      )
      install (
          FILES ${HDF4_BINARY_DIR}/CMakeFiles/PkgInfo
          DESTINATION ..
      )
    endif ()
  else ()
    list (APPEND CPACK_GENERATOR "STGZ")
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    set (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "${HDF4_PACKAGE_BUGREPORT}")

#    list (APPEND CPACK_GENERATOR "RPM")
    set (CPACK_RPM_PACKAGE_RELEASE "1")
    set (CPACK_RPM_COMPONENT_INSTALL ON)
    set (CPACK_RPM_PACKAGE_RELOCATABLE ON)
    set (CPACK_RPM_PACKAGE_LICENSE "BSD-style")
    set (CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    set (CPACK_RPM_PACKAGE_URL "${HDF4_PACKAGE_URL}")
    set (CPACK_RPM_PACKAGE_SUMMARY "HDF is a unique technology suite that makes possible the management of extremely large and complex data collections.")
    set (CPACK_RPM_PACKAGE_DESCRIPTION
        "The HDF technology suite includes:

    * A versatile data model that can represent very complex data objects and a wide variety of metadata.

    * A completely portable file format with no limit on the number or size of data objects in the collection.

    * A software library that runs on a range of computational platforms, from laptops to massively parallel systems, and implements a high-level API with C, C++, Fortran 90, and Java interfaces.

    * A rich set of integrated performance features that allow for access time and storage space optimizations.

    * Tools and applications for managing, manipulating, viewing, and analyzing the data in the collection.

The HDF data model, file format, API, library, and tools are open and distributed without charge.
"
    )

    #-----------------------------------------------------------------------------
    # Configure the spec file for the install RPM
    #-----------------------------------------------------------------------------
#    configure_file ("${HDF4_RESOURCES_DIR}/hdf4.spec.in" "${CMAKE_CURRENT_BINARY_DIR}/${HDF4_PACKAGE_NAME}.spec" @ONLY IMMEDIATE)
#    set (CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_BINARY_DIR}/${HDF4_PACKAGE_NAME}.spec")
  endif ()

  # By default, do not warn when built on machines using only VS Express:
  if (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
  endif ()
  include (InstallRequiredSystemLibraries)

  set (CPACK_INSTALL_CMAKE_PROJECTS "${HDF4_BINARY_DIR};HDF4;ALL;/")

  if (HDF4_PACKAGE_EXTLIBS)
    if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      if (JPEG_FOUND AND JPEG_USE_EXTERNAL)
        if (WIN32)
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;ALL;/")
        else ()
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;libraries;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;headers;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;configinstall;/")
        endif ()
      endif ()
      if (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
        if (WIN32)
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;ALL;/")
        else ()
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;libraries;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;headers;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;configinstall;/")
        endif ()
      endif ()
      if (SZIP_FOUND AND SZIP_USE_EXTERNAL)
        if (WIN32)
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;ALL;/")
        else ()
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;libraries;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;headers;/")
          set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;configinstall;/")
        endif ()
      endif ()
    endif ()
  endif ()

  include (CPack)

  cpack_add_install_type(Full DISPLAY_NAME "Everything")
  cpack_add_install_type(Developer)

  cpack_add_component_group(Runtime)

  cpack_add_component_group(Documents
      EXPANDED
      DESCRIPTION "Release notes for developing HDF applications"
  )

  cpack_add_component_group(Development
      EXPANDED
      DESCRIPTION "All of the tools you'll need to develop HDF applications"
  )

  cpack_add_component_group(Applications
      EXPANDED
      DESCRIPTION "Tools for HDF4 files"
  )

  #-----------------------------------------------------------------------------
  # Now list the cpack commands
  #-----------------------------------------------------------------------------
  cpack_add_component (libraries
      DISPLAY_NAME "HDF4 Libraries"
      REQUIRED
      GROUP Runtime
      INSTALL_TYPES Full Developer User
  )
  cpack_add_component (headers
      DISPLAY_NAME "HDF4 Headers"
      DEPENDS libraries
      GROUP Development
      INSTALL_TYPES Full Developer
  )
  cpack_add_component (hdfdocuments
      DISPLAY_NAME "HDF4 Documents"
      GROUP Documents
      INSTALL_TYPES Full Developer
  )
  cpack_add_component (configinstall
      DISPLAY_NAME "HDF4 CMake files"
      HIDDEN
      DEPENDS libraries
      GROUP Development
      INSTALL_TYPES Full Developer User
  )

  if (HDF4_BUILD_FORTRAN)
    cpack_add_component (fortlibraries
        DISPLAY_NAME "HDF4 Fortran Libraries"
        DEPENDS libraries
        GROUP Runtime
        INSTALL_TYPES Full Developer User
    )
  endif ()

  if (HDF4_BUILD_TOOLS)
    cpack_add_component (toolsapplications
        DISPLAY_NAME "HDF4 Tools Applications"
        DEPENDS libraries
        GROUP Applications
        INSTALL_TYPES Full Developer User
    )
  endif ()

  if (HDF4_BUILD_UTILS)
    cpack_add_component (utilsapplications
        DISPLAY_NAME "HDF4 Utility Applications"
        DEPENDS libraries
        GROUP Applications
        INSTALL_TYPES Full Developer User
    )
  endif ()
endif ()
