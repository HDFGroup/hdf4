
#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
if (NOT HDF4_INSTALL_NO_DEVELOPMENT)
  install (
      FILES ${PROJECT_BINARY_DIR}/h4config.h
      DESTINATION ${HDF4_INSTALL_INCLUDE_DIR}
      COMPONENT headers
  )
endif (NOT HDF4_INSTALL_NO_DEVELOPMENT)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  install (
      EXPORT ${HDF4_EXPORTED_TARGETS}
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
      COMPONENT configinstall
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  export (
      TARGETS ${HDF4_LIBRARIES_TO_EXPORT} ${HDF4_LIB_DEPENDENCIES}
      FILE ${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-targets.cmake
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf4-config.cmake file for the build directory
#-----------------------------------------------------------------------------
set (HDF4_INCLUDES_BUILD_TIME
    ${HDF4_HDF_SOURCE_DIR}
    ${HDF4_MFHDF_SOURCE_DIR}
    ${HDF4_MFHDF_XDR_DIR}
    ${HDF4_BINARY_DIR}
)
set (HDF4_VERSION_STRING ${HDF4_PACKAGE_VERSION})
set (HDF4_VERSION_MAJOR  ${HDF4_PACKAGE_VERSION_MAJOR})
set (HDF4_VERSION_MINOR  ${HDF4_PACKAGE_VERSION_MINOR})

configure_file (
    ${HDF_RESOURCES_DIR}/hdf4-config.cmake.build.in 
    ${HDF4_BINARY_DIR}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the FindHDF4.cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  configure_file (
      ${HDF_RESOURCES_DIR}/FindHDF4.cmake.in 
      ${HDF4_BINARY_DIR}/CMakeFiles/FindHDF4${HDF_PACKAGE_EXT}.cmake @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/FindHDF4${HDF_PACKAGE_EXT}.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      COMPONENT configinstall
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)


#-----------------------------------------------------------------------------
# Configure the hdf4-config.cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  configure_file (
      ${HDF_RESOURCES_DIR}/hdf4-config.cmake.install.in
      ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      COMPONENT configinstall
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf4-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED)
  configure_file (
      ${HDF_RESOURCES_DIR}/hdf4-config-version.cmake.in
      ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config-version.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      COMPONENT configinstall
  )
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the libhdf4.settings file for the lib info
#-----------------------------------------------------------------------------
if (H4_WORDS_BIGENDIAN)
  set (BYTESEX big-endian)
else (H4_WORDS_BIGENDIAN)
  set (BYTESEX little-endian)
endif (H4_WORDS_BIGENDIAN)
configure_file (
    ${HDF_RESOURCES_DIR}/libhdf4.settings.cmake.in 
    ${HDF4_BINARY_DIR}/libhdf4.settings @ONLY
)
install (
    FILES ${HDF4_BINARY_DIR}/libhdf4.settings
    DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
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
    install (
        FILES
            ${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}
            ${HDF4_SOURCE_DIR}/release_notes/USING_CMake_Examples.txt
        DESTINATION ${HDF4_INSTALL_DATA_DIR}
        COMPONENT hdfdocuments
    )
  endif (EXISTS "${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}")
endif (HDF4_PACK_EXAMPLES)

#-----------------------------------------------------------------------------
# Configure the README.txt file for the binary package
#-----------------------------------------------------------------------------
HDF_README_PROPERTIES(HDF4_BUILD_FORTRAN)

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
    endif (WIN32)
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
      endif (WIN32)
      if (CYGWIN)
        set (release_files
            ${release_files}
            ${HDF4_SOURCE_DIR}/release_notes/INSTALL_Cygwin.txt
        )
      endif (CYGWIN)
    endif (HDF4_PACK_INSTALL_DOCS)
    install (
        FILES ${release_files}
        DESTINATION ${HDF4_INSTALL_DATA_DIR}
        COMPONENT hdfdocuments
    )
  endif (EXISTS "${HDF4_SOURCE_DIR}/release_notes" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/release_notes")
endif (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
if (NOT HDF4_EXTERNALLY_CONFIGURED AND NOT HDF4_NO_PACKAGES)
  set (CPACK_PACKAGE_VENDOR "HDF_Group")
  set (CPACK_PACKAGE_NAME "${HDF4_PACKAGE_NAME}")
  if (CDASH_LOCAL)
    set (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION}")
  else (CDASH_LOCAL)
    set (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION_STRING}")
  endif (CDASH_LOCAL)
  set (CPACK_PACKAGE_VERSION_MAJOR "${HDF4_PACKAGE_VERSION_MAJOR}")
  set (CPACK_PACKAGE_VERSION_MINOR "${HDF4_PACKAGE_VERSION_MINOR}")
  set (CPACK_PACKAGE_VERSION_PATCH "")
  set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  if (EXISTS "${HDF4_SOURCE_DIR}/release_notes")
    set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
    set (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  endif (EXISTS "${HDF4_SOURCE_DIR}/release_notes")
  set (CPACK_PACKAGE_RELOCATABLE TRUE)
  set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${CPACK_PACKAGE_VERSION}")
  set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.bmp")

  set (CPACK_GENERATOR "TGZ") 
  if (WIN32)
    list (APPEND CPACK_GENERATOR "NSIS") 
    # Installers for 32- vs. 64-bit CMake:
    #  - Root install directory (displayed to end user at installer-run time)
    #  - "NSIS package/display name" (text used in the installer GUI)
    #  - Registry key used to store info about the installation
    set (CPACK_NSIS_PACKAGE_NAME "${HDF4_PACKAGE_STRING}")
    if (CMAKE_CL_64)
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} (Win64)")
    else (CMAKE_CL_64)
      set (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
      set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    endif (CMAKE_CL_64)
    # set the install/unistall icon used for the installer itself
    # There is a bug in NSI that does not handle full unix paths properly.
    set (CPACK_NSIS_MUI_ICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.ico")
    set (CPACK_NSIS_MUI_UNIICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.ico")
    # set the package header icon for MUI
    set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}\\\\hdf.bmp")
    set (CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}")
    set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_VERSION}")
    set (CPACK_MONOLITHIC_INSTALL ON)
    set (CPACK_NSIS_CONTACT "${HDF4_PACKAGE_BUGREPORT}")
    set (CPACK_NSIS_MODIFY_PATH ON)
  elseif (APPLE)
    list (APPEND CPACK_GENERATOR "DragNDrop") 
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_PACKAGE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.icns")

    if (HDF4_PACK_MACOSX_BUNDLE)
      list (APPEND CPACK_GENERATOR "Bundle")
      set (CPACK_BUNDLE_NAME "${HDF4_PACKAGE_STRING}")
      set (CPACK_BUNDLE_LOCATION "/")    # make sure CMAKE_INSTALL_PREFIX ends in /
      set (CMAKE_INSTALL_PREFIX "/${CPACK_BUNDLE_NAME}.framework/Versions/${CPACK_PACKAGE_VERSION}/${CPACK_PACKAGE_NAME}/")
      set (CPACK_BUNDLE_ICON "${HDF_RESOURCES_EXT_DIR}/hdf.icns")
      set (CPACK_BUNDLE_PLIST "${HDF4_BINARY_DIR}/CMakeFiles/Info.plist")
      set (CPACK_APPLE_GUI_INFO_STRING "Hierarchical Data Format (HDF) Software Library and Utilities")
      set (CPACK_APPLE_GUI_COPYRIGHT "Copyright © 2006-2014 by The HDF Group. All rights reserved.")
      set (CPACK_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      set (CPACK_APPLE_GUI_BUNDLE_NAME "${HDF4_PACKAGE_STRING}")
      set (CPACK_APPLE_GUI_VERSION_STRING "${CPACK_PACKAGE_VERSION_STRING}")
      set (CPACK_APPLE_GUI_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      #-----------------------------------------------------------------------------
      # Configure the Info.plist file for the install bundle
      #-----------------------------------------------------------------------------
      configure_file (
          ${HDF_RESOURCES_EXT_DIR}/CPack.Info.plist.in
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
                ${HDF4_BINARY_DIR}/CMakeFiles/version.plist
          DESTINATION ..
      )
    endif (HDF4_PACK_MACOSX_BUNDLE)
  else (WIN32)
    list (APPEND CPACK_GENERATOR "STGZ") 
    set (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    set (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "${HDF4_PACKAGE_BUGREPORT}")
    
    set (CPACK_RPM_COMPONENT_INSTALL ON)
    set (CPACK_RPM_PACKAGE_RELOCATABLE ON)
    set (CPACK_RPM_PACKAGE_LICENSE "BSD-style")
    set (CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    set (CPACK_RPM_PACKAGE_URL "${HDF4_PACKAGE_URL}")
  endif (WIN32)
  
  # By default, do not warn when built on machines using only VS Express:
  if (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
  endif (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
  include (InstallRequiredSystemLibraries)

  set (CPACK_INSTALL_CMAKE_PROJECTS "${HDF4_BINARY_DIR};HDF4;ALL;/")
  
  if (HDF4_PACKAGE_EXTLIBS)
    if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      if (JPEG_FOUND AND JPEG_USE_EXTERNAL)
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;libraries;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;headers;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${JPEG_INCLUDE_DIR_GEN};JPEG;configinstall;/")
      endif (JPEG_FOUND AND JPEG_USE_EXTERNAL)
      if (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;libraries;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;headers;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;configinstall;/")
      endif (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
      if (SZIP_FOUND AND SZIP_USE_EXTERNAL)
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;libraries;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;headers;/")
        set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;configinstall;/")
      endif (SZIP_FOUND AND SZIP_USE_EXTERNAL)
    endif (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  endif (HDF4_PACKAGE_EXTLIBS)

  include (CPack)

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

  if (HDF4_BUILD_FORTRAN)
    CPACK_ADD_COMPONENT (fortlibraries 
        DISPLAY_NAME "HDF4 Fortran Libraries" 
        DEPENDS libraries
        GROUP Runtime
    )
  endif (HDF4_BUILD_FORTRAN)

  if (HDF4_BUILD_TOOLS)
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
  endif (HDF4_BUILD_TOOLS)

  if (HDF4_BUILD_UTILS)
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
  endif (HDF4_BUILD_UTILS)
endif (NOT HDF4_EXTERNALLY_CONFIGURED AND NOT HDF4_NO_PACKAGES)
  