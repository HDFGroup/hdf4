
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
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
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
    ${HDF4_BINARY_DIR}/${HDF4_PACKAGE}${HDF_PACKAGE_EXT}-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the FindHDF4.cmake file for the install directory
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  CONFIGURE_FILE (
      ${HDF4_RESOURCES_DIR}/FindHDF4.cmake.in 
      ${HDF4_BINARY_DIR}/CMakeFiles/FindHDF4${HDF_PACKAGE_EXT}.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/FindHDF4${HDF_PACKAGE_EXT}.cmake
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      COMPONENT configinstall
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)


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
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
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
      DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
      COMPONENT configinstall
  )
ENDIF (NOT HDF4_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the libhdf4.settings file for the lib info
#-----------------------------------------------------------------------------
IF (H4_WORDS_BIGENDIAN)
  SET (BYTESEX big-endian)
ELSE (H4_WORDS_BIGENDIAN)
  SET (BYTESEX little-endian)
ENDIF (H4_WORDS_BIGENDIAN)
CONFIGURE_FILE (
    ${HDF4_RESOURCES_DIR}/libhdf4.settings.cmake.in 
    ${HDF4_BINARY_DIR}/libhdf4.settings @ONLY
)
INSTALL (
    FILES ${HDF4_BINARY_DIR}/libhdf4.settings
    DESTINATION ${HDF4_INSTALL_CMAKE_DIR}/${HDF4_PACKAGE}
    COMPONENT libraries
)

#-----------------------------------------------------------------------------
# Configure the HDF4_Examples.cmake file and the examples
#-----------------------------------------------------------------------------
OPTION (HDF4_PACK_EXAMPLES  "Package the HDF4 Library Examples Compressed File" OFF)
IF (HDF4_PACK_EXAMPLES)
  CONFIGURE_FILE (
      ${HDF4_RESOURCES_DIR}/HDF4_Examples.cmake.in 
      ${HDF4_BINARY_DIR}/HDF4_Examples.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF4_BINARY_DIR}/HDF4_Examples.cmake
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  IF (EXISTS "${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}")
    INSTALL (
        FILES
            ${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}
            ${HDF4_SOURCE_DIR}/release_notes/USING_CMake_Examples.txt
        DESTINATION ${HDF4_INSTALL_DATA_DIR}
        COMPONENT hdfdocuments
    )
  ENDIF (EXISTS "${HDF4_EXAMPLES_COMPRESSED_DIR}/${HDF4_EXAMPLES_COMPRESSED}")
ENDIF (HDF4_PACK_EXAMPLES)

#-----------------------------------------------------------------------------
# Configure the README.txt file for the binary package
#-----------------------------------------------------------------------------
SET (BINARY_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
SET (BINARY_PLATFORM "${CMAKE_SYSTEM_NAME}")
IF (WIN32)
  SET (BINARY_EXAMPLE_ENDING "zip")
  SET (BINARY_INSTALL_ENDING "exe")
  IF (CMAKE_CL_64)
    SET (BINARY_SYSTEM_NAME "win64")
  ELSE (CMAKE_CL_64)
    SET (BINARY_SYSTEM_NAME "win32")
  ENDIF (CMAKE_CL_64)
  IF (${CMAKE_SYSTEM_VERSION} MATCHES "6.1")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM} 7")
  ELSEIF (${CMAKE_SYSTEM_VERSION} MATCHES "6.2")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM} 8")
  ENDIF (${CMAKE_SYSTEM_VERSION} MATCHES "6.1")
  SET (BINARY_PLATFORM "${BINARY_PLATFORM} ${MSVC_C_ARCHITECTURE_ID}")
  IF (${CMAKE_C_COMPILER_VERSION} MATCHES "16.*")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using VISUAL STUDIO 2010")
  ELSEIF (${CMAKE_C_COMPILER_VERSION} MATCHES "15.*")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using VISUAL STUDIO 2008")
  ELSEIF (${CMAKE_C_COMPILER_VERSION} MATCHES "17.*")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using VISUAL STUDIO 2012")
  ELSE (${CMAKE_C_COMPILER_VERSION} MATCHES "16.*")
    SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using VISUAL STUDIO ${CMAKE_C_COMPILER_VERSION}")
  ENDIF (${CMAKE_C_COMPILER_VERSION} MATCHES "16.*")
ELSEIF (APPLE)
  SET (BINARY_EXAMPLE_ENDING "tar.gz")
  SET (BINARY_INSTALL_ENDING "dmg")
  SET (BINARY_PLATFORM "${BINARY_PLATFORM} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")
  SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using ${CMAKE_C_COMPILER_ID} C ${CMAKE_C_COMPILER_VERSION}")
ELSE (WIN32)
  SET (BINARY_EXAMPLE_ENDING "tar.gz")
  SET (BINARY_INSTALL_ENDING "sh")
  SET (BINARY_PLATFORM "${BINARY_PLATFORM} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")
  SET (BINARY_PLATFORM "${BINARY_PLATFORM}, using ${CMAKE_C_COMPILER_ID} C ${CMAKE_C_COMPILER_VERSION}")
ENDIF (WIN32)
IF (HDF4_BUILD_FORTRAN)
  SET (BINARY_PLATFORM "${BINARY_PLATFORM} / ${CMAKE_Fortran_COMPILER_ID} Fortran")
ENDIF (HDF4_BUILD_FORTRAN)

CONFIGURE_FILE (
    ${HDF4_RESOURCES_DIR}/README.txt.cmake.in 
    ${HDF4_BINARY_DIR}/README.txt @ONLY
)

#-----------------------------------------------------------------------------
# Add Document File(s) to CMake Install
#-----------------------------------------------------------------------------
IF (NOT HDF4_EXTERNALLY_CONFIGURED)
  INSTALL (
      FILES
          ${HDF4_SOURCE_DIR}/COPYING
      DESTINATION ${HDF4_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  IF (EXISTS "${HDF4_SOURCE_DIR}/release_notes" AND IS_DIRECTORY "${HDF4_SOURCE_DIR}/release_notes")
    SET (release_files
        ${HDF4_SOURCE_DIR}/release_notes/USING_HDF4_CMake.txt
        ${HDF4_SOURCE_DIR}/release_notes/RELEASE.txt
    )
    IF (WIN32 AND NOT CYGWIN)
      SET (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/USING_HDF4_VS.txt
      )
    ENDIF (WIN32 AND NOT CYGWIN)
    IF (HDF5_PACK_INSTALL_DOCS)
      SET (release_files
          ${release_files}
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL_CMake.txt
          ${HDF4_SOURCE_DIR}/release_notes/HISTORY.txt
          ${HDF4_SOURCE_DIR}/release_notes/INSTALL
      )
      IF (WIN32)
        IF (NOT CYGWIN)
          SET (release_files
              ${release_files}
              ${HDF5_SOURCE_DIR}/release_notes/INSTALL_Windows.txt
          )
        ELSE (NOT CYGWIN)
          SET (release_files
              ${release_files}
              ${HDF5_SOURCE_DIR}/release_notes/INSTALL_Cygwin.txt
          )
        ENDIF (NOT CYGWIN)
      ENDIF (WIN32)
    ENDIF (HDF5_PACK_INSTALL_DOCS)
    INSTALL (
        FILES ${release_files}
        DESTINATION ${HDF4_INSTALL_DATA_DIR}
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
  IF (CDASH_LOCAL)
    SET (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION}")
  ELSE (CDASH_LOCAL)
    SET (CPACK_PACKAGE_VERSION "${HDF4_PACKAGE_VERSION_STRING}")
  ENDIF (CDASH_LOCAL)
  SET (CPACK_PACKAGE_VERSION_MAJOR "${HDF4_PACKAGE_VERSION_MAJOR}")
  SET (CPACK_PACKAGE_VERSION_MINOR "${HDF4_PACKAGE_VERSION_MINOR}")
  SET (CPACK_PACKAGE_VERSION_PATCH "")
  SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/release_notes/RELEASE.txt")
  SET (CPACK_PACKAGE_RELOCATABLE TRUE)
  SET (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}/${CPACK_PACKAGE_NAME}/${CPACK_PACKAGE_VERSION}")
  SET (CPACK_PACKAGE_ICON "${HDF4_RESOURCES_DIR}/hdf.bmp")

  SET (CPACK_GENERATOR "TGZ") 
  IF (WIN32)
    LIST (APPEND CPACK_GENERATOR "NSIS") 
    # Installers for 32- vs. 64-bit CMake:
    #  - Root install directory (displayed to end user at installer-run time)
    #  - "NSIS package/display name" (text used in the installer GUI)
    #  - Registry key used to store info about the installation
    SET (CPACK_NSIS_PACKAGE_NAME "${HDF4_PACKAGE_STRING}-${CPACK_PACKAGE_VERSION}")
    IF (CMAKE_CL_64)
      SET (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
      SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} (Win64)")
    ELSE (CMAKE_CL_64)
      SET (CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
      SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    ENDIF (CMAKE_CL_64)
    # set the install/unistall icon used for the installer itself
    # There is a bug in NSI that does not handle full unix paths properly.
    SET (CPACK_NSIS_MUI_ICON "${HDF4_RESOURCES_DIR}\\\\hdf.ico")
    SET (CPACK_NSIS_MUI_UNIICON "${HDF4_RESOURCES_DIR}\\\\hdf.ico")
    # set the package header icon for MUI
    SET (CPACK_PACKAGE_ICON "${HDF4_RESOURCES_DIR}\\\\hdf.bmp")
    SET (CPACK_NSIS_DISPLAY_NAME "@CPACK_NSIS_PACKAGE_NAME@, is a library and multi-object file format for storing and managing data between machines")
    SET (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_VERSION}")
    SET (CPACK_MONOLITHIC_INSTALL ON)
    SET (CPACK_NSIS_CONTACT "${HDF4_PACKAGE_BUGREPORT}")
    SET (CPACK_NSIS_MODIFY_PATH ON)
  ELSEIF (APPLE)
    LIST (APPEND CPACK_GENERATOR "DragNDrop") 
    SET (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)
    SET (CPACK_PACKAGE_DEFAULT_LOCATION "/opt/${CPACK_PACKAGE_NAME}")
    SET (CPACK_PACKAGING_INSTALL_PREFIX "/")
    SET (CPACK_PACKAGE_ICON "${HDF4_RESOURCES_DIR}/hdf.icns")
    SET (CPACK_SET_DESTDIR TRUE) # Required when packaging, and set CMAKE_INSTALL_PREFIX to "/".

    IF (HDF4_PACK_MACOSX_BUNDLE)
      LIST (APPEND CPACK_GENERATOR "Bundle")
      SET (CPACK_BUNDLE_NAME "${HDF4_PACKAGE_STRING}")
      SET (CPACK_BUNDLE_LOCATION "/")    # make sure CMAKE_INSTALL_PREFIX ends in /
      SET (CMAKE_INSTALL_PREFIX "/${CPACK_BUNDLE_NAME}.framework/Versions/${CPACK_PACKAGE_VERSION}/${CPACK_PACKAGE_NAME}/")
      SET (CPACK_BUNDLE_ICON "${HDF4_RESOURCES_DIR}/hdf.icns")
      SET (CPACK_BUNDLE_PLIST "${HDF4_BINARY_DIR}/CMakeFiles/Info.plist")
      SET (CPACK_APPLE_GUI_INFO_STRING "Hierarchical Data Format (HDF) Software Library and Utilities")
      SET (CPACK_APPLE_GUI_COPYRIGHT "Copyright © 2006-2014 by The HDF Group. All rights reserved.")
      SET (CPACK_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      SET (CPACK_APPLE_GUI_BUNDLE_NAME "${HDF4_PACKAGE_STRING}")
      SET (CPACK_APPLE_GUI_VERSION_STRING "${CPACK_PACKAGE_VERSION_STRING}")
      SET (CPACK_APPLE_GUI_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}")
      #-----------------------------------------------------------------------------
      # Configure the Info.plist file for the install bundle
      #-----------------------------------------------------------------------------
      CONFIGURE_FILE (
          ${HDF4_RESOURCES_DIR}/CPack.Info.plist.in
          ${HDF4_BINARY_DIR}/CMakeFiles/Info.plist @ONLY
      )
      CONFIGURE_FILE (
          ${HDF4_RESOURCES_DIR}/PkgInfo.in
          ${HDF4_BINARY_DIR}/CMakeFiles/PkgInfo @ONLY
      )
      CONFIGURE_FILE (
          ${HDF4_RESOURCES_DIR}/version.plist.in
          ${HDF4_BINARY_DIR}/CMakeFiles/version.plist @ONLY
      )
      INSTALL (
          FILES ${HDF4_BINARY_DIR}/CMakeFiles/PkgInfo
                ${HDF4_BINARY_DIR}/CMakeFiles/version.plist
          DESTINATION ..
      )
    ENDIF (HDF4_PACK_MACOSX_BUNDLE)
  ELSE (WIN32)
    LIST (APPEND CPACK_GENERATOR "STGZ") 
    SET (CPACK_PACKAGING_INSTALL_PREFIX "/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    SET (CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

    SET (CPACK_DEBIAN_PACKAGE_SECTION "Libraries")
    SET (CPACK_DEBIAN_PACKAGE_MAINTAINER "${HDF4_PACKAGE_BUGREPORT}")
    
    SET (CPACK_RPM_COMPONENT_INSTALL ON)
    SET (CPACK_RPM_PACKAGE_RELOCATABLE ON)
    SET (CPACK_RPM_PACKAGE_LICENSE "BSD-style")
    SET (CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    SET (CPACK_RPM_PACKAGE_URL "${HDF4_PACKAGE_URL}")
  ENDIF (WIN32)
  
  # By default, do not warn when built on machines using only VS Express:
  IF (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    SET (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
  ENDIF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
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
  