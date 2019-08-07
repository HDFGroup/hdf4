#-------------------------------------------------------------------------------
macro (H4_SET_LIB_OPTIONS libtarget libname libtype)
  set (LIB_OUT_NAME "${libname}")
  if (${libtype} MATCHES "SHARED")
    set (PACKAGE_SOVERSION ${HDF4_PACKAGE_SOVERSION})
    set (PACKAGE_COMPATIBILITY ${HDF4_SOVERS_INTERFACE}.0.0)
    set (PACKAGE_CURRENT ${HDF4_SOVERS_INTERFACE}.${HDF4_SOVERS_MINOR}.0)
    if (WIN32)
      set (LIBHDF_VERSION ${HDF4_PACKAGE_VERSION_MAJOR})
    else ()
      set (LIBHDF_VERSION ${HDF4_PACKAGE_SOVERSION_MAJOR})
    endif ()
    set_target_properties (${libtarget} PROPERTIES VERSION ${PACKAGE_SOVERSION})
    if (WIN32)
        set (${LIB_OUT_NAME} "${LIB_OUT_NAME}-${LIBHDF_VERSION}")
    else ()
        set_target_properties (${libtarget} PROPERTIES SOVERSION ${LIBHDF_VERSION})
    endif ()
    if (CMAKE_C_OSX_CURRENT_VERSION_FLAG)
      set_property(TARGET ${libtarget} APPEND PROPERTY
          LINK_FLAGS "${CMAKE_C_OSX_CURRENT_VERSION_FLAG}${PACKAGE_CURRENT} ${CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG}${PACKAGE_COMPATIBILITY}"
      )
    endif ()
  endif ()
  HDF_SET_LIB_OPTIONS (${libtarget} ${LIB_OUT_NAME} ${libtype})

  #-- Apple Specific install_name for libraries
  if (APPLE)
    option (HDF4_BUILD_WITH_INSTALL_NAME "Build with library install_name set to the installation path" OFF)
    if (HDF4_BUILD_WITH_INSTALL_NAME)
      set_target_properties (${libtarget} PROPERTIES
          INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
          BUILD_WITH_INSTALL_RPATH ${HDF4_BUILD_WITH_INSTALL_NAME}
      )
    endif ()
    if (HDF4_BUILD_FRAMEWORKS)
      if (${libtype} MATCHES "SHARED")
        # adapt target to build frameworks instead of dylibs
        set_target_properties(${libtarget} PROPERTIES
            XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
            FRAMEWORK TRUE
            FRAMEWORK_VERSION ${HDF4_PACKAGE_VERSION_MAJOR}
            MACOSX_FRAMEWORK_IDENTIFIER org.hdfgroup.${libtarget}
            MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${HDF4_PACKAGE_VERSION_MAJOR}
            MACOSX_FRAMEWORK_BUNDLE_VERSION ${HDF4_PACKAGE_VERSION_MAJOR})
      endif ()
    endif ()
  endif ()

endmacro ()
