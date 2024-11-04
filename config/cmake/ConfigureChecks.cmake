#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
include (CheckFunctionExists)
include (CheckIncludeFile)
include (CheckIncludeFiles)
include (CheckLibraryExists)
include (CheckSymbolExists)
include (CheckTypeSize)
include (CheckVariableExists)
include (TestBigEndian)
include (CheckStructHasMember)

set (HDF_PREFIX "H4")

# Check for Darwin (not just Apple - we also want to catch OpenDarwin)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set (${HDF_PREFIX}_HAVE_DARWIN 1)
endif ()

# Check for Solaris
if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    set (${HDF_PREFIX}_HAVE_SOLARIS 1)
endif ()

#-----------------------------------------------------------------------------
# This MACRO checks IF the symbol exists in the library and IF it
# does, it appends library to the list.
#-----------------------------------------------------------------------------
set (LINK_LIBS "")
set (LINK_PUB_LIBS "")
macro (CHECK_LIBRARY_EXISTS_CONCAT LIBRARY SYMBOL VARIABLE)
  CHECK_LIBRARY_EXISTS ("${LIBRARY};${LINK_LIBS}" ${SYMBOL} "" ${VARIABLE})
  if (${VARIABLE})
    set (LINK_LIBS ${LINK_LIBS} ${LIBRARY})
  endif ()
endmacro ()

# ----------------------------------------------------------------------
# WINDOWS Hard code Values
# ----------------------------------------------------------------------
set (WINDOWS)

if (MINGW)
  set (${HDF_PREFIX}_HAVE_MINGW 1)
  set (WINDOWS 1) # MinGW tries to imitate Windows
  set (CMAKE_REQUIRED_FLAGS "-DWIN32_LEAN_AND_MEAN=1 -DNOGDI=1")
  set (${HDF_PREFIX}_HAVE_WINSOCK2_H 1)
  set (__USE_MINGW_ANSI_STDIO 1)
endif ()

if (WIN32 AND NOT MINGW)
  if (NOT UNIX)
    set (WINDOWS 1)
    set (CMAKE_REQUIRED_FLAGS "/DWIN32_LEAN_AND_MEAN=1 /DNOGDI=1")
    if (MSVC)
      set (${HDF_PREFIX}_HAVE_VISUAL_STUDIO 1)
    endif ()
  endif ()
  message (TRACE "MSVC=${MSVC}")
  message (TRACE "HAVE_VISUAL_STUDIO=${${HDF_PREFIX}_HAVE_VISUAL_STUDIO}")
endif ()

if (WINDOWS)
  set (CMAKE_REQUIRED_LIBRARIES "ws2_32.lib;wsock32.lib")
  set (${HDF_PREFIX}_HAVE_WIN32_API 1)
  set (${HDF_PREFIX}_HAVE_SYSTEM 1)
  if (NOT UNIX AND NOT CYGWIN)
    set (${HDF_PREFIX}_HAVE_LIBWS2_32 1)
    set (${HDF_PREFIX}_HAVE_LIBWSOCK32 1)
  endif ()
endif ()

# ----------------------------------------------------------------------
# END of WINDOWS Hard code Values
# ----------------------------------------------------------------------

if (NOT WINDOWS)
  TEST_BIG_ENDIAN (${HDF_PREFIX}_WORDS_BIGENDIAN)
endif ()

#-----------------------------------------------------------------------------
# Check IF header file exists and add it to the list.
#-----------------------------------------------------------------------------
macro (CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES ("${USE_INCLUDES};${FILE}" ${VARIABLE})
  if (${VARIABLE})
    set (USE_INCLUDES ${USE_INCLUDES} ${FILE})
  endif ()
endmacro ()

#-----------------------------------------------------------------------------
#  Check for the existence of certain header files
#-----------------------------------------------------------------------------
CHECK_INCLUDE_FILE_CONCAT ("fcntl.h"         ${HDF_PREFIX}_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE_CONCAT ("unistd.h"        ${HDF_PREFIX}_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT ("arpa/inet.h"     ${HDF_PREFIX}_HAVE_INET_H)
CHECK_INCLUDE_FILE_CONCAT ("netinet/in.h"    ${HDF_PREFIX}_HAVE_NETINET_IN_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/file.h"      ${HDF_PREFIX}_HAVE_SYS_FILE_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/resource.h"  ${HDF_PREFIX}_HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/stat.h"      ${HDF_PREFIX}_HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/time.h"      ${HDF_PREFIX}_HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/types.h"     ${HDF_PREFIX}_HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/wait.h"      ${HDF_PREFIX}_HAVE_SYS_WAIT_H)

# Windows
if (NOT CYGWIN)
  CHECK_INCLUDE_FILE_CONCAT ("winsock2.h"      ${HDF_PREFIX}_HAVE_WINSOCK2_H)
endif ()

#-----------------------------------------------------------------------------
#  Library checks
#-----------------------------------------------------------------------------
if (MINGW OR NOT WINDOWS)
  CHECK_LIBRARY_EXISTS_CONCAT ("m" ceil     ${HDF_PREFIX}_HAVE_LIBM)
  CHECK_LIBRARY_EXISTS_CONCAT ("ws2_32" WSAStartup  ${HDF_PREFIX}_HAVE_LIBWS2_32)
  CHECK_LIBRARY_EXISTS_CONCAT ("wsock32" gethostbyname ${HDF_PREFIX}_HAVE_LIBWSOCK32)
endif ()

# For other tests to use the same libraries
set (HDF4_REQUIRED_LIBRARIES ${HDF4_REQUIRED_LIBRARIES} ${LINK_LIBS})

set (USE_INCLUDES "")
if (WINDOWS)
  set (USE_INCLUDES ${USE_INCLUDES} "windows.h")
endif ()

#-----------------------------------------------------------------------------
#  Platform-specific flags
#-----------------------------------------------------------------------------

set (HDF_EXTRA_C_FLAGS)
set (HDF_EXTRA_FLAGS)
if (MINGW OR NOT WINDOWS)
  # Might want to check explicitly for Linux and possibly Cygwin
  # instead of checking for not Solaris or Darwin.
  if (NOT ${HDF_PREFIX}_HAVE_SOLARIS AND NOT ${HDF_PREFIX}_HAVE_DARWIN)
    # Linux Specific flags
    #
    # This is needed so strdup, etc. are exposed properly
    #
    # POSIX feature information can be found in the gcc manual at:
    # http://www.gnu.org/s/libc/manual/html_node/Feature-Test-Macros.html
    set (HDF_EXTRA_C_FLAGS -D_POSIX_C_SOURCE=200809L)

    set (CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} ${HDF_EXTRA_FLAGS})
  endif ()
endif ()

if (CYGWIN)
  set (CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} -D_GNU_SOURCE")
  add_definitions ("-D_GNU_SOURCE")
endif ()

add_definitions (${HDF_EXTRA_FLAGS})

#-----------------------------------------------------------------------------
#  Check the size in bytes of all the int and float types
#-----------------------------------------------------------------------------
macro (HDF_CHECK_TYPE_SIZE type var)
  set (aType ${type})
  set (aVar  ${var})
  message (TRACE "Checking size of ${aType} and storing into ${aVar}")
  CHECK_TYPE_SIZE (${aType}   ${aVar})
  if (NOT ${aVar})
    set (${aVar} 0 CACHE INTERNAL "SizeOf for ${aType}")
    message (TRACE "Size of ${aType} was NOT Found")
  endif ()
endmacro ()

HDF_CHECK_TYPE_SIZE (long ${HDF_PREFIX}_SIZEOF_LONG)

#-----------------------------------------------------------------------------
# Check for some functions that are used
#
CHECK_FUNCTION_EXISTS (fcntl             ${HDF_PREFIX}_HAVE_FCNTL)
CHECK_FUNCTION_EXISTS (fork              ${HDF_PREFIX}_HAVE_FORK)
CHECK_FUNCTION_EXISTS (getrusage         ${HDF_PREFIX}_HAVE_GETRUSAGE)
CHECK_FUNCTION_EXISTS (system            ${HDF_PREFIX}_HAVE_SYSTEM)
CHECK_FUNCTION_EXISTS (wait              ${HDF_PREFIX}_HAVE_WAIT)
