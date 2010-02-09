#-------------------------------------------------------------------------------
MACRO (SET_GLOBAL_VARIABLE name value)
  SET(${name} ${value} CACHE INTERNAL "Used to pass variables between directories" FORCE)
ENDMACRO (SET_GLOBAL_VARIABLE name value)

#-------------------------------------------------------------------------------
MACRO (IDE_GENERATED_PROPERTIES SOURCE_PATH HEADERS SOURCES)
  #set(source_group_path "Source/AIM/${NAME}")
  STRING(REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH}  )
  source_group(${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #set_property(SOURCE ${HEADERS}
  #       PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)

ENDMACRO (IDE_GENERATED_PROPERTIES SOURCE_PATH HEADERS SOURCES)

#-------------------------------------------------------------------------------
MACRO (IDE_SOURCE_PROPERTIES SOURCE_PATH HEADERS SOURCES)
  #  INSTALL (FILES ${HEADERS}
  #       DESTINATION include/R3D/${NAME}
  #       COMPONENT Headers       
  #  )

  STRING(REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH}  )
  source_group(${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #set_property(SOURCE ${HEADERS}
  #       PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)

ENDMACRO (IDE_SOURCE_PROPERTIES NAME HEADERS SOURCES)

#-------------------------------------------------------------------------------
MACRO(H4_SET_LIB_OPTIONS libtarget libname libtype)
  # message (STATUS "${libname} libtype: ${libtype}")
  IF ( ${libtype} MATCHES "SHARED")
    IF (WIN32 AND NOT MINGW)
      SET(LIB_RELEASE_NAME "${libname}dll")
      SET(LIB_DEBUG_NAME "${libname}dll_D")
    ELSE (WIN32 AND NOT MINGW)
      SET(LIB_RELEASE_NAME "${libname}")
      SET(LIB_DEBUG_NAME "${libname}_debug")
    ENDIF(WIN32 AND NOT MINGW)
  ELSE ( ${libtype} MATCHES "SHARED")
    IF (WIN32 AND NOT MINGW)
      SET(LIB_RELEASE_NAME "lib${libname}")
      SET(LIB_DEBUG_NAME "lib${libname}_D")
    ELSE (WIN32 AND NOT MINGW)
      SET(LIB_RELEASE_NAME "${libname}")
      SET(LIB_DEBUG_NAME "${libname}_debug")
    ENDIF(WIN32 AND NOT MINGW)
  ENDIF ( ${libtype} MATCHES "SHARED")
  
  SET_TARGET_PROPERTIES( ${libtarget}
    PROPERTIES
    DEBUG_OUTPUT_NAME          ${LIB_DEBUG_NAME}
    RELEASE_OUTPUT_NAME        ${LIB_RELEASE_NAME}
    MINSIZEREL_OUTPUT_NAME     ${LIB_RELEASE_NAME}
    RELWITHDEBINFO_OUTPUT_NAME ${LIB_RELEASE_NAME}
  )
  
  #----- Use MSVC Naming conventions for Shared Libraries
  IF (MINGW AND BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES( ${libtarget}
      PROPERTIES
      IMPORT_SUFFIX ".lib"
      IMPORT_PREFIX ""
      PREFIX ""
    )
  ENDIF (MINGW AND BUILD_SHARED_LIBS)

  IF (BUILD_SHARED_LIBS)
  IF (WIN32)
    SET(LIBHDF_VERSION HDF4_PACKAGE_VERSION_MAJOR)
  ELSE (WIN32)
    SET(LIBHDF_VERSION ${HDF4_PACKAGE_VERSION})
  ENDIF (WIN32)
  SET_TARGET_PROPERTIES(${libtarget} PROPERTIES VERSION ${LIBHDF_VERSION})
  SET_TARGET_PROPERTIES(${libtarget} PROPERTIES SOVERSION ${LIBHDF_VERSION})
  ENDIF (BUILD_SHARED_LIBS)

  #-- Apple Specific install_name for libraries
  IF (APPLE)
  OPTION(HDF4_BUILD_WITH_INSTALL_NAME "Build with library install_name set to the installation path" OFF)
  if (HDF4_BUILD_WITH_INSTALL_NAME)
  SET_TARGET_PROPERTIES(${libtarget} PROPERTIES
     LINK_FLAGS "-current_version ${HDF4_PACKAGE_VERSION} -compatibility_version ${HDF4_PACKAGE_VERSION}"
     INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
     BUILD_WITH_INSTALL_RPATH ${HDF4_BUILD_WITH_INSTALL_NAME}
  )
  endif()
  ENDIF (APPLE)

ENDMACRO(H4_SET_LIB_OPTIONS libname)


#-------------------------------------------------------------------------------
MACRO(H4_SET_BOTH_LIB_TYPES libname libname_sources)
 ADD_LIBRARY(${libname} SHARED ${libname_sources})
 ADD_LIBRARY(${libname}-static STATIC ${libname_sources})
 # The library target "libname" already has a default OUTPUT_NAME of "libname", so we don't need to change it.
 # The library target "libname-static" has a default OUTPUT_NAME of "libname-static", so change it.
 SET_TARGET_PROPERTIES(${libname}-static PROPERTIES OUTPUT_NAME "libname")
 # Now the library target "libname-static" will be named "libname.lib" with MS tools.
 # This conflicts with the "libname.lib" import library corresponding to "libname.dll",
 # so we add a "lib" prefix (which is default on other platforms anyway):
 SET_TARGET_PROPERTIES(${libname}-static PROPERTIES PREFIX "lib")
 # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
 SET_TARGET_PROPERTIES(${libname} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
 SET_TARGET_PROPERTIES(${libname}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

ENDMACRO(H4_SET_BOTH_LIB_TYPES)

