#
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_C_FLAGS "${CMAKE_C99_STANDARD_COMPILE_OPTION} ${CMAKE_C_FLAGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_SANITIZER_FLAGS} ${CMAKE_C_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_SANITIZER_FLAGS} ${CMAKE_CXX_FLAGS}")
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
  message (VERBOSE "Warnings Configuration: C default: ${CMAKE_C_FLAGS}")
  message (VERBOSE "Warnings Configuration: CXX default: ${CMAKE_CXX_FLAGS}")
endif ()
#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
if(WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "Intel")
  set(_INTEL_WINDOWS 1)
endif()

if(WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "Clang"
   AND "x${CMAKE_C_SIMULATE_ID}" STREQUAL "xMSVC")
  set(_CLANG_MSVC_WINDOWS 1)
endif()

# Disable deprecation warnings for standard C functions.
# really only needed for newer versions of VS, but should
# not hurt other versions, and this will work into the
# future
if(MSVC OR _INTEL_WINDOWS OR _CLANG_MSVC_WINDOWS)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
else()
endif()

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stack:10000000")
endif()

# MSVC 14.28 enables C5105, but the Windows SDK 10.0.18362.0 triggers it.
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC" AND NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 19.28)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -wd5105")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd5105")
endif()

if(_CLANG_MSVC_WINDOWS AND "x${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "xGNU")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -stack:20000000")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL SunPro AND
    NOT DEFINED CMAKE_CXX${CMAKE_CXX_STANDARD}_STANDARD_COMPILE_OPTION)
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.13)
    if (NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD EQUAL 98)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
    endif()
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stlport4")
  endif()
endif()

if (CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS}")
  if (${HDF_CFG_NAME} MATCHES "Debug")
    if (NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 5.0)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -ftrapv -fno-common")
    endif ()
  else ()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 5.0)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstdarg-opt")
    endif ()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 10.0)
      #-----------------------------------------------------------------------------
      # Option to allow the user to enable build extended diagnostics
      #
      # This should NOT be on by default as it can cause process issues.
      #-----------------------------------------------------------------------------
      option (HDF5_ENABLE_BUILD_DIAGS "Enable color and URL extended diagnostic messages" OFF)
      if (HDF5_ENABLE_BUILD_DIAGS)
        message (STATUS "... default color and URL extended diagnostic messages enabled")
      else ()
        message (STATUS "... disable color and URL extended diagnostic messages")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-urls=never -fno-diagnostics-color")
      endif ()
    endif ()
  endif ()
endif ()
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_LOADED)
  set (CMAKE_CXX_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_CXX_FLAGS}")
  if (${HDF_CFG_NAME} MATCHES "Debug")
    if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -ftrapv -fno-common")
    endif ()
  else ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstdarg-opt")
    endif ()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
      if (HDF5_ENABLE_BUILD_DIAGS)
        message (STATUS "... default color and URL extended diagnostic messages enabled")
      else ()
        message (STATUS "... disable color and URL extended diagnostic messages")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-urls=never -fno-diagnostics-color")
      endif ()
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option to allow the user to disable compiler warnings
#-----------------------------------------------------------------------------
option (HDF4_DISABLE_COMPILER_WARNINGS "Disable compiler warnings" OFF)
if (HDF4_DISABLE_COMPILER_WARNINGS)
  message (STATUS "....Compiler warnings are suppressed")
  # MSVC uses /w to suppress warnings.  It also complains if another
  # warning level is given, so remove it.
  if (MSVC)
    set (HDF4_WARNINGS_BLOCKED 1)
    string (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
    if (CMAKE_CXX_COMPILER_LOADED)
      string (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
    endif ()
  endif ()
  if (WIN32)
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
  endif ()

  # Most compilers use -w to suppress warnings.
  if (NOT HDF4_WARNINGS_BLOCKED)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    if (CMAKE_CXX_COMPILER_LOADED)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------
if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  list (APPEND HDF4_CMAKE_C_FLAGS "-erroff=%none -DBSD_COMP")
  list (APPEND HDF5_CMAKE_CXX_FLAGS "-erroff=%none -DBSD_COMP")
else ()
  # General flags
  #
  # Note that some of the flags listed here really should be developer
  # flags (listed in a separate variable, below) but we put them here
  # because they are not raised by the current code and we'd like to
  # know if they do start showing up.
  #
  # NOTE: Don't add -Wpadded here since we can't/won't fix the (many)
  # warnings that are emitted. If you need it, add it at configure time.
  if (CMAKE_C_COMPILER_ID STREQUAL "Intel")
    if (_INTEL_WINDOWS)
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wcheck /Wall")
    else ()
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcheck -Wall")
    endif ()
  elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra")
    # C99 disable compile warning-error
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=implicit-function-declaration")
  endif ()
  if (CMAKE_CXX_COMPILER_LOADED)
    if (CMAKE_C_COMPILER_ID STREQUAL "Intel")
      if (_INTEL_WINDOWS)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wcheck /Wall")
      else ()
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcheck -Wall")
      endif ()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=implicit-function-declaration")
    endif ()
  endif ()
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
# Option for --enable-asserts
# By default, CMake adds NDEBUG to CMAKE_${lang}_FLAGS for Release build types
# This option will force/override the default setting for all configurations
#-----------------------------------------------------------------------------
#option (HDF4_ENABLE_ASSERTS "Determines whether NDEBUG is defined to control assertions." OFF)
set (HDF4_ENABLE_ASSERTS "OFF" CACHE STRING "Determines whether NDEBUG is defined to control assertions (OFF NO YES)")
set_property (CACHE HDF4_ENABLE_ASSERTS PROPERTY STRINGS OFF NO YES)
if (HDF4_ENABLE_ASSERTS MATCHES "YES")
  add_compile_options ("-UNDEBUG")
elseif (HDF4_ENABLE_ASSERTS MATCHES "NO")
  add_compile_options ("-DNDEBUG")
endif ()
MARK_AS_ADVANCED (HDF4_ENABLE_ASSERTS)

#-----------------------------------------------------------------------------
# Option for --enable-symbols
# This option will force/override the default setting for all configurations
#-----------------------------------------------------------------------------
#option (HDF4_ENABLE_SYMBOLS "Add debug symbols to the library independent of the build mode and optimization level." OFF)
set (HDF4_ENABLE_SYMBOLS "OFF" CACHE STRING "Add debug symbols to the library independent of the build mode and optimization level (OFF NO YES)")
set_property (CACHE HDF4_ENABLE_SYMBOLS PROPERTY STRINGS OFF NO YES)
if (HDF4_ENABLE_SYMBOLS MATCHES "YES")
  if (CMAKE_C_COMPILER_ID STREQUAL "Intel" AND NOT _INTEL_WINDOWS)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
  elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fno-omit-frame-pointer")
  endif ()
  if(CMAKE_CXX_COMPILER_LOADED)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" AND NOT _INTEL_WINDOWS)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif ()
  endif ()
elseif (HDF4_ENABLE_SYMBOLS MATCHES "NO")
  if (CMAKE_C_COMPILER_ID STREQUAL "Intel" AND NOT _INTEL_WINDOWS)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-s")
  elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
  endif ()
  if(CMAKE_CXX_COMPILER_LOADED)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" AND NOT _INTEL_WINDOWS)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-s")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
    endif ()
  endif ()
endif ()
MARK_AS_ADVANCED (HDF4_ENABLE_SYMBOLS)

#-----------------------------------------------------------------------------
# Option for --enable-profiling
# This option will force/override the default setting for all configurations
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_PROFILING "Enable profiling flags independently from the build mode." OFF)
if (HDF4_ENABLE_PROFILING)
  list (APPEND HDF4_CMAKE_C_FLAGS "${PROFILE_CFLAGS}")
  if(CMAKE_CXX_COMPILER_LOADED)
    list (APPEND HDF4_CMAKE_CXX_FLAGS "${PROFILE_CXXFLAGS}")
  endif ()
endif ()
MARK_AS_ADVANCED (HDF4_ENABLE_PROFILING)

#-----------------------------------------------------------------------------
# Option for --enable-optimization
# This option will force/override the default setting for all configurations
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_OPTIMIZATION "Enable optimization flags/settings independently from the build mode" OFF)
if (HDF4_ENABLE_OPTIMIZATION)
  list (APPEND HDF4_CMAKE_C_FLAGS "${OPTIMIZE_CFLAGS}")
  if(CMAKE_CXX_COMPILER_LOADED)
    list (APPEND HDF4_CMAKE_CXX_FLAGS "${OPTIMIZE_CXXFLAGS}")
  endif ()
endif ()
MARK_AS_ADVANCED (HDF4_ENABLE_OPTIMIZATION)
