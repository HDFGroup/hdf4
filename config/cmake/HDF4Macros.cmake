#-------------------------------------------------------------------------------
MACRO (H4_SET_LIB_OPTIONS libtarget libname libtype)
  set (LIB_OUT_NAME "${libname}")
  if (${libtype} MATCHES "SHARED")
    if (WIN32)
      set (LIBHDF_VERSION ${HDF4_PACKAGE_VERSION_MAJOR})
    else (WIN32)
      set (LIBHDF_VERSION ${HDF4_PACKAGE_VERSION})
    endif (WIN32)
    set_target_properties (${libtarget} PROPERTIES VERSION ${LIBHDF_VERSION})
    set_target_properties (${libtarget} PROPERTIES SOVERSION ${LIBHDF_VERSION})
  endif (${libtype} MATCHES "SHARED")
  HDF_SET_LIB_OPTIONS (${libtarget} ${LIB_OUT_NAME} ${libtype})

  #-- Apple Specific install_name for libraries
  if (APPLE)
    option (HDF4_BUILD_WITH_INSTALL_NAME "Build with library install_name set to the installation path" OFF)
    if (HDF4_BUILD_WITH_INSTALL_NAME)
      set_target_properties (${libtarget} PROPERTIES
          LINK_FLAGS "-current_version ${HDF4_PACKAGE_VERSION} -compatibility_version ${HDF4_PACKAGE_VERSION}"
          INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
          BUILD_WITH_INSTALL_RPATH ${HDF4_BUILD_WITH_INSTALL_NAME}
      )
    endif (HDF4_BUILD_WITH_INSTALL_NAME)
  endif (APPLE)

ENDMACRO (H4_SET_LIB_OPTIONS)
