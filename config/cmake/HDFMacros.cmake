#-------------------------------------------------------------------------------
MACRO (SET_GLOBAL_VARIABLE name value)
  SET (${name} ${value} CACHE INTERNAL "Used to pass variables between directories" FORCE)
ENDMACRO (SET_GLOBAL_VARIABLE)

#-------------------------------------------------------------------------------
MACRO (IDE_GENERATED_PROPERTIES SOURCE_PATH HEADERS SOURCES)
  #set(source_group_path "Source/AIM/${NAME}")
  STRING (REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH})
  source_group (${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #SET_PROPERTY (SOURCE ${HEADERS}
  #       PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)
ENDMACRO (IDE_GENERATED_PROPERTIES)

#-------------------------------------------------------------------------------
MACRO (IDE_SOURCE_PROPERTIES SOURCE_PATH HEADERS SOURCES)
  #  INSTALL (FILES ${HEADERS}
  #       DESTINATION include/R3D/${NAME}
  #       COMPONENT Headers       
  #  )

  STRING (REPLACE "/" "\\\\" source_group_path ${SOURCE_PATH}  )
  source_group (${source_group_path} FILES ${HEADERS} ${SOURCES})

  #-- The following is needed if we ever start to use OS X Frameworks but only
  #--  works on CMake 2.6 and greater
  #SET_PROPERTY (SOURCE ${HEADERS}
  #       PROPERTY MACOSX_PACKAGE_LOCATION Headers/${NAME}
  #)
ENDMACRO (IDE_SOURCE_PROPERTIES)

#-------------------------------------------------------------------------------
MACRO (TARGET_NAMING target)
  IF (WIN32 AND NOT MINGW)
    IF (BUILD_SHARED_LIBS)
      IF (HDF_LEGACY_NAMING)
        SET_TARGET_PROPERTIES (${target} PROPERTIES OUTPUT_NAME "dll")
        SET_TARGET_PROPERTIES (${target} PROPERTIES PREFIX "${target}")
      ELSE (HDF_LEGACY_NAMING)
        SET_TARGET_PROPERTIES (${target} PROPERTIES OUTPUT_NAME "${target}dll")
      ENDIF (HDF_LEGACY_NAMING)
    ENDIF (BUILD_SHARED_LIBS)
  ENDIF (WIN32 AND NOT MINGW)
ENDMACRO (TARGET_NAMING)

#-------------------------------------------------------------------------------
MACRO (HDF_SET_LIB_OPTIONS libtarget libname libtype)
  # message (STATUS "${libname} libtype: ${libtype}")
  IF (${libtype} MATCHES "SHARED")
    IF (WIN32 AND NOT MINGW)
      IF (HDF_LEGACY_NAMING)
        SET (LIB_RELEASE_NAME "${libname}dll")
        SET (LIB_DEBUG_NAME "${libname}ddll")
      ELSE (HDF_LEGACY_NAMING)
        SET (LIB_RELEASE_NAME "${libname}")
        SET (LIB_DEBUG_NAME "${libname}_D")
      ENDIF (HDF_LEGACY_NAMING)
    ELSE (WIN32 AND NOT MINGW)
      SET (LIB_RELEASE_NAME "${libname}")
      SET (LIB_DEBUG_NAME "${libname}_debug")
    ENDIF (WIN32 AND NOT MINGW)
  ELSE (${libtype} MATCHES "SHARED")
    IF (WIN32 AND NOT MINGW)
      IF (HDF_LEGACY_NAMING)
        SET (LIB_RELEASE_NAME "${libname}")
        SET (LIB_DEBUG_NAME "${libname}d")
      ELSE (HDF_LEGACY_NAMING)
        SET (LIB_RELEASE_NAME "lib${libname}")
        SET (LIB_DEBUG_NAME "lib${libname}_D")
      ENDIF (HDF_LEGACY_NAMING)
    ELSE (WIN32 AND NOT MINGW)
      SET (LIB_RELEASE_NAME "lib${libname}")
      SET (LIB_DEBUG_NAME "lib${libname}_debug")
    ENDIF (WIN32 AND NOT MINGW)
  ENDIF (${libtype} MATCHES "SHARED")
  
  SET_TARGET_PROPERTIES (${libtarget}
      PROPERTIES
      DEBUG_OUTPUT_NAME          ${LIB_DEBUG_NAME}
      RELEASE_OUTPUT_NAME        ${LIB_RELEASE_NAME}
      MINSIZEREL_OUTPUT_NAME     ${LIB_RELEASE_NAME}
      RELWITHDEBINFO_OUTPUT_NAME ${LIB_RELEASE_NAME}
  )
  
  #----- Use MSVC Naming conventions for Shared Libraries
  IF (MINGW AND BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES (${libtarget}
        PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
    )
  ENDIF (MINGW AND BUILD_SHARED_LIBS)

ENDMACRO (HDF_SET_LIB_OPTIONS)

#-------------------------------------------------------------------------------
MACRO (TARGET_WIN_PROPERTIES target)
  IF (WIN32)
    IF (MSVC)
      IF (NOT BUILD_SHARED_LIBS)
        SET_TARGET_PROPERTIES (${target}
            PROPERTIES
                LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
        ) 
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (MSVC)
  ENDIF (WIN32)
ENDMACRO (TARGET_WIN_PROPERTIES)

#-------------------------------------------------------------------------------
MACRO (TARGET_FORTRAN_WIN_PROPERTIES target)
  IF (WIN32)
    IF (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (${target}
            PROPERTIES
                COMPILE_FLAGS "/dll"
                LINK_FLAGS "/SUBSYSTEM:CONSOLE"
        ) 
      ENDIF (MSVC)
    ELSE (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (${target}
            PROPERTIES
                LINK_FLAGS "/NODEFAULTLIB:MSVCRT"
        ) 
      ENDIF (MSVC)
    ENDIF (BUILD_SHARED_LIBS)
  ENDIF (WIN32)
ENDMACRO (TARGET_FORTRAN_WIN_PROPERTIES)
