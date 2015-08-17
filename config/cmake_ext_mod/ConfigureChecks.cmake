#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
include (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
include (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
include (${CMAKE_ROOT}/Modules/CheckVariableExists.cmake)
include (${CMAKE_ROOT}/Modules/CheckFortranFunctionExists.cmake)
include (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
if(CMAKE_CXX_COMPILER)
  include (${CMAKE_ROOT}/Modules/TestForSTDNamespace.cmake)
endif(CMAKE_CXX_COMPILER)

#-----------------------------------------------------------------------------
# APPLE/Darwin setup
#-----------------------------------------------------------------------------
if (APPLE)
  list (LENGTH CMAKE_OSX_ARCHITECTURES ARCH_LENGTH)
  if (ARCH_LENGTH GREATER 1)
    set (CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)
    message(FATAL_ERROR "Building Universal Binaries on OS X is NOT supported by the HDF5 project. This is"
    "due to technical reasons. The best approach would be build each architecture in separate directories"
    "and use the 'lipo' tool to combine them into a single executable or library. The 'CMAKE_OSX_ARCHITECTURES'"
    "variable has been set to a blank value which will build the default architecture for this system.")
  endif ()
  set (${HDF_PREFIX}_AC_APPLE_UNIVERSAL_BUILD 0)
endif (APPLE)

# Check for Darwin (not just Apple - we also want to catch OpenDarwin)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
    set (${HDF_PREFIX}_HAVE_DARWIN 1) 
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Check for Solaris
if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS") 
    set (${HDF_PREFIX}_HAVE_SOLARIS 1) 
endif (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")

#-----------------------------------------------------------------------------
# This MACRO checks IF the symbol exists in the library and IF it
# does, it appends library to the list.
#-----------------------------------------------------------------------------
set (LINK_LIBS "")
MACRO (CHECK_LIBRARY_EXISTS_CONCAT LIBRARY SYMBOL VARIABLE)
  CHECK_LIBRARY_EXISTS ("${LIBRARY};${LINK_LIBS}" ${SYMBOL} "" ${VARIABLE})
  if (${VARIABLE})
    set (LINK_LIBS ${LINK_LIBS} ${LIBRARY})
  endif (${VARIABLE})
ENDMACRO (CHECK_LIBRARY_EXISTS_CONCAT)

# ----------------------------------------------------------------------
# WINDOWS Hard code Values
# ----------------------------------------------------------------------

set (WINDOWS)
if (WIN32)
  if (MINGW)
    set (${HDF_PREFIX}_HAVE_MINGW 1)
    set (WINDOWS 1) # MinGW tries to imitate Windows
    set (CMAKE_REQUIRED_FLAGS "-DWIN32_LEAN_AND_MEAN=1 -DNOGDI=1")
  endif (MINGW)
  set (${HDF_PREFIX}_HAVE_WIN32_API 1)
  set (CMAKE_REQUIRED_LIBRARIES "ws2_32.lib;wsock32.lib")
  if (NOT UNIX AND NOT MINGW)
    set (WINDOWS 1)
    set (CMAKE_REQUIRED_FLAGS "/DWIN32_LEAN_AND_MEAN=1 /DNOGDI=1")
    if (MSVC)
      set (${HDF_PREFIX}_HAVE_VISUAL_STUDIO 1)
    endif (MSVC)
  endif (NOT UNIX AND NOT MINGW)
endif (WIN32)

if (WINDOWS)
  set (${HDF_PREFIX}_HAVE_STDDEF_H 1)
  set (${HDF_PREFIX}_HAVE_SYS_STAT_H 1)
  set (${HDF_PREFIX}_HAVE_SYS_TYPES_H 1)
  set (${HDF_PREFIX}_HAVE_LIBM 1)
  set (${HDF_PREFIX}_HAVE_STRDUP 1)
  set (${HDF_PREFIX}_HAVE_SYSTEM 1)
  set (${HDF_PREFIX}_HAVE_LONGJMP 1)
  if (NOT MINGW)
    set (${HDF_PREFIX}_HAVE_GETHOSTNAME 1)
  endif (NOT MINGW)
  if (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
    set (${HDF_PREFIX}_HAVE_GETCONSOLESCREENBUFFERINFO 1)
  endif (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
  set (${HDF_PREFIX}_HAVE_FUNCTION 1)
  set (${HDF_PREFIX}_GETTIMEOFDAY_GIVES_TZ 1)
  set (${HDF_PREFIX}_HAVE_TIMEZONE 1)
  set (${HDF_PREFIX}_HAVE_GETTIMEOFDAY 1)
  if (MINGW)
    set (${HDF_PREFIX}_HAVE_WINSOCK2_H 1)
  endif (MINGW)
  set (${HDF_PREFIX}_HAVE_LIBWS2_32 1)
  set (${HDF_PREFIX}_HAVE_LIBWSOCK32 1)
endif (WINDOWS)

# ----------------------------------------------------------------------
# END of WINDOWS Hard code Values
# ----------------------------------------------------------------------

if (CYGWIN)
  set (${HDF_PREFIX}_HAVE_LSEEK64 0)
endif (CYGWIN)

#-----------------------------------------------------------------------------
#  Check for the math library "m"
#-----------------------------------------------------------------------------
if (NOT WINDOWS)
  CHECK_LIBRARY_EXISTS_CONCAT ("m" ceil     ${HDF_PREFIX}_HAVE_LIBM)
  CHECK_LIBRARY_EXISTS_CONCAT ("dl" dlopen     ${HDF_PREFIX}_HAVE_LIBDL)
  CHECK_LIBRARY_EXISTS_CONCAT ("ws2_32" WSAStartup  ${HDF_PREFIX}_HAVE_LIBWS2_32)
  CHECK_LIBRARY_EXISTS_CONCAT ("wsock32" gethostbyname ${HDF_PREFIX}_HAVE_LIBWSOCK32)
endif (NOT WINDOWS)

CHECK_LIBRARY_EXISTS_CONCAT ("ucb"    gethostname  ${HDF_PREFIX}_HAVE_LIBUCB)
CHECK_LIBRARY_EXISTS_CONCAT ("socket" connect      ${HDF_PREFIX}_HAVE_LIBSOCKET)
CHECK_LIBRARY_EXISTS ("c" gethostbyname "" NOT_NEED_LIBNSL)

if (NOT NOT_NEED_LIBNSL)
  CHECK_LIBRARY_EXISTS_CONCAT ("nsl"    gethostbyname  ${HDF_PREFIX}_HAVE_LIBNSL)
endif (NOT NOT_NEED_LIBNSL)

# For other tests to use the same libraries
set (CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${LINK_LIBS})

set (USE_INCLUDES "")
if (WINDOWS)
  set (USE_INCLUDES ${USE_INCLUDES} "windows.h")
endif (WINDOWS)

if (NOT WINDOWS)
  TEST_BIG_ENDIAN (${HDF_PREFIX}_WORDS_BIGENDIAN)
endif (NOT WINDOWS)

# For other specific tests, use this MACRO.
MACRO (HDF_FUNCTION_TEST OTHER_TEST)
  if ("${HDF_PREFIX}_${OTHER_TEST}" MATCHES "^${HDF_PREFIX}_${OTHER_TEST}$")
    set (MACRO_CHECK_FUNCTION_DEFINITIONS "-D${OTHER_TEST} ${CMAKE_REQUIRED_FLAGS}")
    set (OTHER_TEST_ADD_LIBRARIES)
    if (CMAKE_REQUIRED_LIBRARIES)
      set (OTHER_TEST_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif (CMAKE_REQUIRED_LIBRARIES)

    foreach (def ${HDF_EXTRA_TEST_DEFINITIONS})
      set (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}=${${def}}")
    endforeach (def)

    foreach (def
        HAVE_SYS_TIME_H
        HAVE_UNISTD_H
        HAVE_SYS_TYPES_H
        HAVE_SYS_SOCKET_H
    )
      if ("${${HDF_PREFIX}_${def}}")
        set (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}")
      endif ("${${HDF_PREFIX}_${def}}")
    endforeach (def)

    if (LARGEFILE)
      set (MACRO_CHECK_FUNCTION_DEFINITIONS
          "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE"
      )
    endif (LARGEFILE)

    #message (STATUS "Performing ${OTHER_TEST}")
    TRY_COMPILE (${OTHER_TEST}
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_EXT_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
        "${OTHER_TEST_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )
    if (${OTHER_TEST})
      set (${HDF_PREFIX}_${OTHER_TEST} 1 CACHE INTERNAL "Other test ${FUNCTION}")
      message (STATUS "Performing Other Test ${OTHER_TEST} - Success")
    else (${OTHER_TEST})
      message (STATUS "Performing Other Test ${OTHER_TEST} - Failed")
      set (${HDF_PREFIX}_${OTHER_TEST} "" CACHE INTERNAL "Other test ${FUNCTION}")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
          "Performing Other Test ${OTHER_TEST} failed with the following output:\n"
          "${OUTPUT}\n"
      )
    endif (${OTHER_TEST})
  endif ("${HDF_PREFIX}_${OTHER_TEST}" MATCHES "^${HDF_PREFIX}_${OTHER_TEST}$")
ENDMACRO (HDF_FUNCTION_TEST)

#-----------------------------------------------------------------------------
# Check for these functions before the time headers are checked
#-----------------------------------------------------------------------------
HDF_FUNCTION_TEST (STDC_HEADERS)

#-----------------------------------------------------------------------------
# Check IF header file exists and add it to the list.
#-----------------------------------------------------------------------------
MACRO (CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES ("${USE_INCLUDES};${FILE}" ${VARIABLE})
  if (${VARIABLE})
    set (USE_INCLUDES ${USE_INCLUDES} ${FILE})
  endif (${VARIABLE})
ENDMACRO (CHECK_INCLUDE_FILE_CONCAT)

#-----------------------------------------------------------------------------
#  Check for the existence of certain header files
#-----------------------------------------------------------------------------
CHECK_INCLUDE_FILE_CONCAT ("sys/resource.h"  ${HDF_PREFIX}_HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/time.h"      ${HDF_PREFIX}_HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CONCAT ("unistd.h"        ${HDF_PREFIX}_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/ioctl.h"     ${HDF_PREFIX}_HAVE_SYS_IOCTL_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/stat.h"      ${HDF_PREFIX}_HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/socket.h"    ${HDF_PREFIX}_HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/types.h"     ${HDF_PREFIX}_HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("stddef.h"        ${HDF_PREFIX}_HAVE_STDDEF_H)
CHECK_INCLUDE_FILE_CONCAT ("setjmp.h"        ${HDF_PREFIX}_HAVE_SETJMP_H)
CHECK_INCLUDE_FILE_CONCAT ("features.h"      ${HDF_PREFIX}_HAVE_FEATURES_H)
CHECK_INCLUDE_FILE_CONCAT ("dirent.h"        ${HDF_PREFIX}_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE_CONCAT ("stdint.h"        ${HDF_PREFIX}_HAVE_STDINT_H)

# IF the c compiler found stdint, check the C++ as well. On some systems this
# file will be found by C but not C++, only do this test IF the C++ compiler
# has been initialized (e.g. the project also includes some c++)
if (${HDF_PREFIX}_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)
  CHECK_INCLUDE_FILE_CXX ("stdint.h" ${HDF_PREFIX}_HAVE_STDINT_H_CXX)
  if (NOT ${HDF_PREFIX}_HAVE_STDINT_H_CXX)
    set (${HDF_PREFIX}_HAVE_STDINT_H "" CACHE INTERNAL "Have includes HAVE_STDINT_H")
    set (USE_INCLUDES ${USE_INCLUDES} "stdint.h")
  endif (NOT ${HDF_PREFIX}_HAVE_STDINT_H_CXX)
endif (${HDF_PREFIX}_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)

# Darwin
CHECK_INCLUDE_FILE_CONCAT ("mach/mach_time.h" ${HDF_PREFIX}_HAVE_MACH_MACH_TIME_H)

# Windows
CHECK_INCLUDE_FILE_CONCAT ("io.h"            ${HDF_PREFIX}_HAVE_IO_H)
if (NOT CYGWIN)
  CHECK_INCLUDE_FILE_CONCAT ("winsock2.h"      ${HDF_PREFIX}_HAVE_WINSOCK2_H)
endif (NOT CYGWIN)
CHECK_INCLUDE_FILE_CONCAT ("sys/timeb.h"     ${HDF_PREFIX}_HAVE_SYS_TIMEB_H)

if (CMAKE_SYSTEM_NAME MATCHES "OSF")
  CHECK_INCLUDE_FILE_CONCAT ("sys/sysinfo.h" ${HDF_PREFIX}_HAVE_SYS_SYSINFO_H)
  CHECK_INCLUDE_FILE_CONCAT ("sys/proc.h"    ${HDF_PREFIX}_HAVE_SYS_PROC_H)
else (CMAKE_SYSTEM_NAME MATCHES "OSF")
  set (${HDF_PREFIX}_HAVE_SYS_SYSINFO_H "" CACHE INTERNAL "" FORCE)
  set (${HDF_PREFIX}_HAVE_SYS_PROC_H    "" CACHE INTERNAL "" FORCE)
endif (CMAKE_SYSTEM_NAME MATCHES "OSF")

CHECK_INCLUDE_FILE_CONCAT ("globus/common.h" ${HDF_PREFIX}_HAVE_GLOBUS_COMMON_H)
CHECK_INCLUDE_FILE_CONCAT ("pdb.h"           ${HDF_PREFIX}_HAVE_PDB_H)
CHECK_INCLUDE_FILE_CONCAT ("pthread.h"       ${HDF_PREFIX}_HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE_CONCAT ("srbclient.h"     ${HDF_PREFIX}_HAVE_SRBCLIENT_H)
CHECK_INCLUDE_FILE_CONCAT ("string.h"        ${HDF_PREFIX}_HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT ("strings.h"       ${HDF_PREFIX}_HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT ("stdlib.h"        ${HDF_PREFIX}_HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CONCAT ("memory.h"        ${HDF_PREFIX}_HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CONCAT ("dlfcn.h"         ${HDF_PREFIX}_HAVE_DLFCN_H)
CHECK_INCLUDE_FILE_CONCAT ("inttypes.h"      ${HDF_PREFIX}_HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("netinet/in.h"    ${HDF_PREFIX}_HAVE_NETINET_IN_H)

#-----------------------------------------------------------------------------
#  Check for large file support
#-----------------------------------------------------------------------------

# The linux-lfs option is deprecated.
set (LINUX_LFS 0)

set (HDF_EXTRA_C_FLAGS)
set (HDF_EXTRA_FLAGS)
if (NOT WINDOWS)
  if (NOT ${HDF_PREFIX}_HAVE_SOLARIS)
  # Linux Specific flags
  # This was originally defined as _POSIX_SOURCE which was updated to
  # _POSIX_C_SOURCE=199506L to expose a greater amount of POSIX
  # functionality so clock_gettime and CLOCK_MONOTONIC are defined
  # correctly.
  # POSIX feature information can be found in the gcc manual at:
  # http://www.gnu.org/s/libc/manual/html_node/Feature-Test-Macros.html
  set (HDF_EXTRA_C_FLAGS -D_POSIX_C_SOURCE=199506L)
  # _BSD_SOURCE deprecated in GLIBC >= 2.20
  TRY_RUN (HAVE_DEFAULT_SOURCE_RUN HAVE_DEFAULT_SOURCE_COMPILE
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_EXT_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=-DHAVE_DEFAULT_SOURCE
        OUTPUT_VARIABLE OUTPUT
    )
  if (HAVE_DEFAULT_SOURCE_COMPILE AND HAVE_DEFAULT_SOURCE_RUN)
    set (HDF_EXTRA_FLAGS -D_DEFAULT_SOURCE)
  else (HAVE_DEFAULT_SOURCE_COMPILE AND HAVE_DEFAULT_SOURCE_RUN)
    set (HDF_EXTRA_FLAGS -D_BSD_SOURCE)
  endif (HAVE_DEFAULT_SOURCE_COMPILE AND HAVE_DEFAULT_SOURCE_RUN)

  option (HDF_ENABLE_LARGE_FILE "Enable support for large (64-bit) files on Linux." ON)
  if (HDF_ENABLE_LARGE_FILE)
    set (msg "Performing TEST_LFS_WORKS")
    TRY_RUN (TEST_LFS_WORKS_RUN   TEST_LFS_WORKS_COMPILE
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_EXT_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=-DTEST_LFS_WORKS
        OUTPUT_VARIABLE OUTPUT
    )
    if (TEST_LFS_WORKS_COMPILE)
      if (TEST_LFS_WORKS_RUN  MATCHES 0)
        set (TEST_LFS_WORKS 1 CACHE INTERNAL ${msg})
        set (LARGEFILE 1)
        set (HDF_EXTRA_FLAGS ${HDF_EXTRA_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE)
        message (STATUS "${msg}... yes")
      else (TEST_LFS_WORKS_RUN  MATCHES 0)
        set (TEST_LFS_WORKS "" CACHE INTERNAL ${msg})
        message (STATUS "${msg}... no")
        file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
              "Test TEST_LFS_WORKS Run failed with the following output and exit code:\n ${OUTPUT}\n"
        )
      endif (TEST_LFS_WORKS_RUN  MATCHES 0)
    else (TEST_LFS_WORKS_COMPILE )
      set (TEST_LFS_WORKS "" CACHE INTERNAL ${msg})
      message (STATUS "${msg}... no")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
          "Test TEST_LFS_WORKS Compile failed with the following output:\n ${OUTPUT}\n"
      )
    endif (TEST_LFS_WORKS_COMPILE)
  endif (HDF_ENABLE_LARGE_FILE)
  set (CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} ${HDF_EXTRA_FLAGS})
  endif (NOT ${HDF_PREFIX}_HAVE_SOLARIS)
endif (NOT WINDOWS)

add_definitions (${HDF_EXTRA_FLAGS})

#-----------------------------------------------------------------------------
# Check for HAVE_OFF64_T functionality
#-----------------------------------------------------------------------------
if (NOT WINDOWS OR MINGW)
  HDF_FUNCTION_TEST (HAVE_OFF64_T)
  if (${HDF_PREFIX}_HAVE_OFF64_T)
    CHECK_FUNCTION_EXISTS (lseek64            ${HDF_PREFIX}_HAVE_LSEEK64)
    CHECK_FUNCTION_EXISTS (fseeko64           ${HDF_PREFIX}_HAVE_FSEEKO64)
    CHECK_FUNCTION_EXISTS (ftello64           ${HDF_PREFIX}_HAVE_FTELLO64)
    CHECK_FUNCTION_EXISTS (ftruncate64        ${HDF_PREFIX}_HAVE_FTRUNCATE64)
  endif (${HDF_PREFIX}_HAVE_OFF64_T)

  CHECK_FUNCTION_EXISTS (fseeko               ${HDF_PREFIX}_HAVE_FSEEKO)
  CHECK_FUNCTION_EXISTS (ftello               ${HDF_PREFIX}_HAVE_FTELLO)

  HDF_FUNCTION_TEST (HAVE_STAT64_STRUCT)
  if (HAVE_STAT64_STRUCT)
    CHECK_FUNCTION_EXISTS (fstat64            ${HDF_PREFIX}_HAVE_FSTAT64)
    CHECK_FUNCTION_EXISTS (stat64             ${HDF_PREFIX}_HAVE_STAT64)
  endif (HAVE_STAT64_STRUCT)
endif (NOT WINDOWS OR MINGW)

#-----------------------------------------------------------------------------
#  Check the size in bytes of all the int and float types
#-----------------------------------------------------------------------------
MACRO (HDF_CHECK_TYPE_SIZE type var)
  set (aType ${type})
  set (aVar  ${var})
#  message (STATUS "Checking size of ${aType} and storing into ${aVar}")
  CHECK_TYPE_SIZE (${aType}   ${aVar})
  if (NOT ${aVar})
    set (${aVar} 0 CACHE INTERNAL "SizeOf for ${aType}")
#    message (STATUS "Size of ${aType} was NOT Found")
  endif (NOT ${aVar})
ENDMACRO (HDF_CHECK_TYPE_SIZE)

HDF_CHECK_TYPE_SIZE (char           ${HDF_PREFIX}_SIZEOF_CHAR)
HDF_CHECK_TYPE_SIZE (short          ${HDF_PREFIX}_SIZEOF_SHORT)
HDF_CHECK_TYPE_SIZE (int            ${HDF_PREFIX}_SIZEOF_INT)
HDF_CHECK_TYPE_SIZE (unsigned       ${HDF_PREFIX}_SIZEOF_UNSIGNED)
if (NOT APPLE)
  HDF_CHECK_TYPE_SIZE (long         ${HDF_PREFIX}_SIZEOF_LONG)
endif (NOT APPLE)
HDF_CHECK_TYPE_SIZE ("long long"    ${HDF_PREFIX}_SIZEOF_LONG_LONG)
HDF_CHECK_TYPE_SIZE (__int64        ${HDF_PREFIX}_SIZEOF___INT64)
if (NOT ${HDF_PREFIX}_SIZEOF___INT64)
  set (${HDF_PREFIX}_SIZEOF___INT64 0)
endif (NOT ${HDF_PREFIX}_SIZEOF___INT64)

HDF_CHECK_TYPE_SIZE (float          ${HDF_PREFIX}_SIZEOF_FLOAT)
HDF_CHECK_TYPE_SIZE (double         ${HDF_PREFIX}_SIZEOF_DOUBLE)
HDF_CHECK_TYPE_SIZE ("long double"  ${HDF_PREFIX}_SIZEOF_LONG_DOUBLE)

HDF_CHECK_TYPE_SIZE (int8_t         ${HDF_PREFIX}_SIZEOF_INT8_T)
HDF_CHECK_TYPE_SIZE (uint8_t        ${HDF_PREFIX}_SIZEOF_UINT8_T)
HDF_CHECK_TYPE_SIZE (int_least8_t   ${HDF_PREFIX}_SIZEOF_INT_LEAST8_T)
HDF_CHECK_TYPE_SIZE (uint_least8_t  ${HDF_PREFIX}_SIZEOF_UINT_LEAST8_T)
HDF_CHECK_TYPE_SIZE (int_fast8_t    ${HDF_PREFIX}_SIZEOF_INT_FAST8_T)
HDF_CHECK_TYPE_SIZE (uint_fast8_t   ${HDF_PREFIX}_SIZEOF_UINT_FAST8_T)

HDF_CHECK_TYPE_SIZE (int16_t        ${HDF_PREFIX}_SIZEOF_INT16_T)
HDF_CHECK_TYPE_SIZE (uint16_t       ${HDF_PREFIX}_SIZEOF_UINT16_T)
HDF_CHECK_TYPE_SIZE (int_least16_t  ${HDF_PREFIX}_SIZEOF_INT_LEAST16_T)
HDF_CHECK_TYPE_SIZE (uint_least16_t ${HDF_PREFIX}_SIZEOF_UINT_LEAST16_T)
HDF_CHECK_TYPE_SIZE (int_fast16_t   ${HDF_PREFIX}_SIZEOF_INT_FAST16_T)
HDF_CHECK_TYPE_SIZE (uint_fast16_t  ${HDF_PREFIX}_SIZEOF_UINT_FAST16_T)

HDF_CHECK_TYPE_SIZE (int32_t        ${HDF_PREFIX}_SIZEOF_INT32_T)
HDF_CHECK_TYPE_SIZE (uint32_t       ${HDF_PREFIX}_SIZEOF_UINT32_T)
HDF_CHECK_TYPE_SIZE (int_least32_t  ${HDF_PREFIX}_SIZEOF_INT_LEAST32_T)
HDF_CHECK_TYPE_SIZE (uint_least32_t ${HDF_PREFIX}_SIZEOF_UINT_LEAST32_T)
HDF_CHECK_TYPE_SIZE (int_fast32_t   ${HDF_PREFIX}_SIZEOF_INT_FAST32_T)
HDF_CHECK_TYPE_SIZE (uint_fast32_t  ${HDF_PREFIX}_SIZEOF_UINT_FAST32_T)

HDF_CHECK_TYPE_SIZE (int64_t        ${HDF_PREFIX}_SIZEOF_INT64_T)
HDF_CHECK_TYPE_SIZE (uint64_t       ${HDF_PREFIX}_SIZEOF_UINT64_T)
HDF_CHECK_TYPE_SIZE (int_least64_t  ${HDF_PREFIX}_SIZEOF_INT_LEAST64_T)
HDF_CHECK_TYPE_SIZE (uint_least64_t ${HDF_PREFIX}_SIZEOF_UINT_LEAST64_T)
HDF_CHECK_TYPE_SIZE (int_fast64_t   ${HDF_PREFIX}_SIZEOF_INT_FAST64_T)
HDF_CHECK_TYPE_SIZE (uint_fast64_t  ${HDF_PREFIX}_SIZEOF_UINT_FAST64_T)

if (NOT APPLE)
  HDF_CHECK_TYPE_SIZE (size_t       ${HDF_PREFIX}_SIZEOF_SIZE_T)
  HDF_CHECK_TYPE_SIZE (ssize_t      ${HDF_PREFIX}_SIZEOF_SSIZE_T)
  if (NOT ${HDF_PREFIX}_SIZEOF_SSIZE_T)
    set (${HDF_PREFIX}_SIZEOF_SSIZE_T 0)
  endif (NOT ${HDF_PREFIX}_SIZEOF_SSIZE_T)
  if (NOT WINDOWS)
    HDF_CHECK_TYPE_SIZE (ptrdiff_t    ${HDF_PREFIX}_SIZEOF_PTRDIFF_T)
  endif (NOT WINDOWS)
endif (NOT APPLE)

HDF_CHECK_TYPE_SIZE (off_t          ${HDF_PREFIX}_SIZEOF_OFF_T)
HDF_CHECK_TYPE_SIZE (off64_t        ${HDF_PREFIX}_SIZEOF_OFF64_T)
if (NOT ${HDF_PREFIX}_SIZEOF_OFF64_T)
  set (${HDF_PREFIX}_SIZEOF_OFF64_T 0)
endif (NOT ${HDF_PREFIX}_SIZEOF_OFF64_T)

if (NOT WINDOWS)
  #-----------------------------------------------------------------------------
  # Check if the dev_t type is a scalar type
  #-----------------------------------------------------------------------------
  HDF_FUNCTION_TEST (DEV_T_IS_SCALAR)

  # ----------------------------------------------------------------------
  # Check for MONOTONIC_TIMER support (used in clock_gettime).  This has
  # to be done after any POSIX/BSD defines to ensure that the test gets
  # the correct POSIX level on linux.
  CHECK_VARIABLE_EXISTS (CLOCK_MONOTONIC HAVE_CLOCK_MONOTONIC)

  #-----------------------------------------------------------------------------
  # Check a bunch of time functions
  #-----------------------------------------------------------------------------
  foreach (test
      HAVE_TM_GMTOFF
      HAVE___TM_GMTOFF
#      HAVE_TIMEZONE
      HAVE_STRUCT_TIMEZONE
      GETTIMEOFDAY_GIVES_TZ
      TIME_WITH_SYS_TIME
      HAVE_TM_ZONE
      HAVE_STRUCT_TM_TM_ZONE
  )
    HDF_FUNCTION_TEST (${test})
  endforeach (test)
  if (NOT CYGWIN AND NOT MINGW)
      HDF_FUNCTION_TEST (HAVE_TIMEZONE)
#      HDF_FUNCTION_TEST (HAVE_STAT_ST_BLOCKS)
  endif (NOT CYGWIN AND NOT MINGW)

  # ----------------------------------------------------------------------
  # Does the struct stat have the st_blocks field?  This field is not Posix.
  #
  HDF_FUNCTION_TEST (HAVE_STAT_ST_BLOCKS)

  # ----------------------------------------------------------------------
  # How do we figure out the width of a tty in characters?
  #
  CHECK_FUNCTION_EXISTS (ioctl             ${HDF_PREFIX}_HAVE_IOCTL)
  HDF_FUNCTION_TEST (HAVE_STRUCT_VIDEOCONFIG)
  HDF_FUNCTION_TEST (HAVE_STRUCT_TEXT_INFO)
  CHECK_FUNCTION_EXISTS (_getvideoconfig   ${HDF_PREFIX}_HAVE__GETVIDEOCONFIG)
  CHECK_FUNCTION_EXISTS (gettextinfo       ${HDF_PREFIX}_HAVE_GETTEXTINFO)
  CHECK_FUNCTION_EXISTS (_scrsize          ${HDF_PREFIX}_HAVE__SCRSIZE)
  if (NOT CYGWIN AND NOT MINGW)
    CHECK_FUNCTION_EXISTS (GetConsoleScreenBufferInfo    ${HDF_PREFIX}_HAVE_GETCONSOLESCREENBUFFERINFO)
  endif (NOT CYGWIN AND NOT MINGW)
  CHECK_SYMBOL_EXISTS (TIOCGWINSZ "sys/ioctl.h" ${HDF_PREFIX}_HAVE_TIOCGWINSZ)
  CHECK_SYMBOL_EXISTS (TIOCGETD   "sys/ioctl.h" ${HDF_PREFIX}_HAVE_TIOCGETD)
endif (NOT WINDOWS)

#-----------------------------------------------------------------------------
# Check for some functions that are used
#
CHECK_FUNCTION_EXISTS (alarm             ${HDF_PREFIX}_HAVE_ALARM)
CHECK_FUNCTION_EXISTS (fork              ${HDF_PREFIX}_HAVE_FORK)
CHECK_FUNCTION_EXISTS (frexpf            ${HDF_PREFIX}_HAVE_FREXPF)
CHECK_FUNCTION_EXISTS (frexpl            ${HDF_PREFIX}_HAVE_FREXPL)

CHECK_FUNCTION_EXISTS (gethostname       ${HDF_PREFIX}_HAVE_GETHOSTNAME)
CHECK_FUNCTION_EXISTS (getpwuid          ${HDF_PREFIX}_HAVE_GETPWUID)
CHECK_FUNCTION_EXISTS (getrusage         ${HDF_PREFIX}_HAVE_GETRUSAGE)
CHECK_FUNCTION_EXISTS (lstat             ${HDF_PREFIX}_HAVE_LSTAT)

CHECK_FUNCTION_EXISTS (rand_r            ${HDF_PREFIX}_HAVE_RAND_R)
CHECK_FUNCTION_EXISTS (random            ${HDF_PREFIX}_HAVE_RANDOM)
CHECK_FUNCTION_EXISTS (setsysinfo        ${HDF_PREFIX}_HAVE_SETSYSINFO)

CHECK_FUNCTION_EXISTS (signal            ${HDF_PREFIX}_HAVE_SIGNAL)
CHECK_FUNCTION_EXISTS (longjmp           ${HDF_PREFIX}_HAVE_LONGJMP)
CHECK_FUNCTION_EXISTS (setjmp            ${HDF_PREFIX}_HAVE_SETJMP)
CHECK_FUNCTION_EXISTS (siglongjmp        ${HDF_PREFIX}_HAVE_SIGLONGJMP)
CHECK_FUNCTION_EXISTS (sigsetjmp         ${HDF_PREFIX}_HAVE_SIGSETJMP)
CHECK_FUNCTION_EXISTS (sigprocmask       ${HDF_PREFIX}_HAVE_SIGPROCMASK)

CHECK_FUNCTION_EXISTS (snprintf          ${HDF_PREFIX}_HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS (srandom           ${HDF_PREFIX}_HAVE_SRANDOM)
CHECK_FUNCTION_EXISTS (strdup            ${HDF_PREFIX}_HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (symlink           ${HDF_PREFIX}_HAVE_SYMLINK)
CHECK_FUNCTION_EXISTS (system            ${HDF_PREFIX}_HAVE_SYSTEM)

CHECK_FUNCTION_EXISTS (tmpfile           ${HDF_PREFIX}_HAVE_TMPFILE)
CHECK_FUNCTION_EXISTS (asprintf          ${HDF_PREFIX}_HAVE_ASPRINTF)
CHECK_FUNCTION_EXISTS (vasprintf         ${HDF_PREFIX}_HAVE_VASPRINTF)
CHECK_FUNCTION_EXISTS (waitpid           ${HDF_PREFIX}_HAVE_WAITPID)

CHECK_FUNCTION_EXISTS (vsnprintf         ${HDF_PREFIX}_HAVE_VSNPRINTF)
if (NOT WINDOWS)
  if (${HDF_PREFIX}_HAVE_VSNPRINTF)
    HDF_FUNCTION_TEST (VSNPRINTF_WORKS)
  endif (${HDF_PREFIX}_HAVE_VSNPRINTF)
endif (NOT WINDOWS)

#-----------------------------------------------------------------------------
# sigsetjmp is special; may actually be a macro
#-----------------------------------------------------------------------------
if (NOT ${HDF_PREFIX}_HAVE_SIGSETJMP)
  if (${HDF_PREFIX}_HAVE_SETJMP_H)
    CHECK_SYMBOL_EXISTS (sigsetjmp "setjmp.h" ${HDF_PREFIX}_HAVE_MACRO_SIGSETJMP)
    if (${HDF_PREFIX}_HAVE_MACRO_SIGSETJMP)
      set (${HDF_PREFIX}_HAVE_SIGSETJMP 1)
    endif (${HDF_PREFIX}_HAVE_MACRO_SIGSETJMP)
  endif (${HDF_PREFIX}_HAVE_SETJMP_H)
endif (NOT ${HDF_PREFIX}_HAVE_SIGSETJMP)

#-----------------------------------------------------------------------------
# Check a bunch of other functions
#-----------------------------------------------------------------------------
if (NOT WINDOWS)
  foreach (test
      HAVE_ATTRIBUTE
      HAVE_C99_FUNC
#      STDC_HEADERS
      HAVE_FUNCTION
      HAVE_C99_DESIGNATED_INITIALIZER
      SYSTEM_SCOPE_THREADS
      HAVE_SOCKLEN_T
      CXX_HAVE_OFFSETOF
  )
    HDF_FUNCTION_TEST (${test})
  endforeach (test)
endif (NOT WINDOWS)

# For other CXX specific tests, use this MACRO.
MACRO (HDF_CXX_FUNCTION_TEST OTHER_TEST)
  if ("${OTHER_TEST}" MATCHES "^${OTHER_TEST}$")
    set (MACRO_CHECK_FUNCTION_DEFINITIONS "-D${OTHER_TEST} ${CMAKE_REQUIRED_FLAGS}")
    set (OTHER_TEST_ADD_LIBRARIES)
    if (CMAKE_REQUIRED_LIBRARIES)
      set (OTHER_TEST_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif (CMAKE_REQUIRED_LIBRARIES)

    foreach (def ${HDF_EXTRA_TEST_DEFINITIONS})
      set (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}=${${def}}")
    endforeach (def)

    foreach (def
        HAVE_SYS_TIME_H
        HAVE_UNISTD_H
        HAVE_SYS_TYPES_H
        HAVE_SYS_SOCKET_H
    )
      if ("${${HDF_PREFIX}_${def}}")
        set (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}")
      endif ("${${HDF_PREFIX}_${def}}")
    endforeach (def)

    if (LARGEFILE)
      set (MACRO_CHECK_FUNCTION_DEFINITIONS
          "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE"
      )
    endif (LARGEFILE)

    #message (STATUS "Performing ${OTHER_TEST}")
    TRY_COMPILE (${OTHER_TEST}
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_EXT_DIR}/HDFCXXTests.cpp
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
        "${OTHER_TEST_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )
    if (${OTHER_TEST} EQUAL 0)
      set (${OTHER_TEST} 1 CACHE INTERNAL "CXX test ${FUNCTION}")
      message (STATUS "Performing CXX Test ${OTHER_TEST} - Success")
    else (${OTHER_TEST} EQUAL 0)
      message (STATUS "Performing CXX Test ${OTHER_TEST} - Failed")
      set (${OTHER_TEST} "" CACHE INTERNAL "CXX test ${FUNCTION}")
      file (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Performing CXX Test ${OTHER_TEST} failed with the following output:\n"
          "${OUTPUT}\n"
      )
    endif (${OTHER_TEST} EQUAL 0)
  endif ("${OTHER_TEST}" MATCHES "^${OTHER_TEST}$")
ENDMACRO (HDF_CXX_FUNCTION_TEST)

#-----------------------------------------------------------------------------
# Check a bunch of cxx functions
#-----------------------------------------------------------------------------
if (CMAKE_CXX_COMPILER_LOADED)
  foreach (test
      OLD_HEADER_FILENAME
      ${HDF_PREFIX}_NO_NAMESPACE
      ${HDF_PREFIX}_NO_STD
      BOOL_NOTDEFINED
      NO_STATIC_CAST
  )
    HDF_CXX_FUNCTION_TEST (${test})
  endforeach (test)
endif (CMAKE_CXX_COMPILER_LOADED)

#-----------------------------------------------------------------------------
# Check if InitOnceExecuteOnce is available
#-----------------------------------------------------------------------------
if (WINDOWS)
  if (NOT HDF_NO_IOEO_TEST)
  message (STATUS "Checking for InitOnceExecuteOnce:")
  if ("${${HDF_PREFIX}_HAVE_IOEO}" MATCHES "^${${HDF_PREFIX}_HAVE_IOEO}$")
    if (LARGEFILE)
      set (CMAKE_REQUIRED_DEFINITIONS
          "${CURRENT_TEST_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE"
      )
    endif (LARGEFILE)
    set (MACRO_CHECK_FUNCTION_DEFINITIONS 
      "-DHAVE_IOEO ${CMAKE_REQUIRED_FLAGS}")
    if (CMAKE_REQUIRED_LIBRARIES)
      set (CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    else (CMAKE_REQUIRED_LIBRARIES)
      set (CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES)
    endif (CMAKE_REQUIRED_LIBRARIES)
    if (CMAKE_REQUIRED_INCLUDES)
      set (CHECK_C_SOURCE_COMPILES_ADD_INCLUDES
        "-DINCLUDE_DIRECTORIES:STRING=${CMAKE_REQUIRED_INCLUDES}")
    else (CMAKE_REQUIRED_INCLUDES)
      set (CHECK_C_SOURCE_COMPILES_ADD_INCLUDES)
    endif (CMAKE_REQUIRED_INCLUDES)

    TRY_RUN(HAVE_IOEO_EXITCODE HAVE_IOEO_COMPILED
      ${CMAKE_BINARY_DIR}
      ${HDF_RESOURCES_EXT_DIR}/HDFTests.c
      COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      -DCMAKE_SKIP_RPATH:BOOL=${CMAKE_SKIP_RPATH}
      "${CHECK_C_SOURCE_COMPILES_ADD_LIBRARIES}"
      "${CHECK_C_SOURCE_COMPILES_ADD_INCLUDES}"
      COMPILE_OUTPUT_VARIABLE OUTPUT)
    # if it did not compile make the return value fail code of 1
    if (NOT HAVE_IOEO_COMPILED)
      set (HAVE_IOEO_EXITCODE 1)
    endif (NOT HAVE_IOEO_COMPILED)
    # if the return value was 0 then it worked
    if ("${HAVE_IOEO_EXITCODE}" EQUAL 0)
      set (${HDF_PREFIX}_HAVE_IOEO 1 CACHE INTERNAL "Test InitOnceExecuteOnce")
      message (STATUS "Performing Test InitOnceExecuteOnce - Success")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log 
        "Performing C SOURCE FILE Test InitOnceExecuteOnce succeded with the following output:\n"
        "${OUTPUT}\n"
        "Return value: ${HAVE_IOEO}\n")
    else ("${HAVE_IOEO_EXITCODE}" EQUAL 0)
      if (CMAKE_CROSSCOMPILING AND "${HAVE_IOEO_EXITCODE}" MATCHES  "FAILED_TO_RUN")
        set (${HDF_PREFIX}_HAVE_IOEO "${HAVE_IOEO_EXITCODE}")
      else (CMAKE_CROSSCOMPILING AND "${HAVE_IOEO_EXITCODE}" MATCHES  "FAILED_TO_RUN")
        set (${HDF_PREFIX}_HAVE_IOEO "" CACHE INTERNAL "Test InitOnceExecuteOnce")
      endif (CMAKE_CROSSCOMPILING AND "${HAVE_IOEO_EXITCODE}" MATCHES  "FAILED_TO_RUN")

      message (STATUS "Performing Test InitOnceExecuteOnce - Failed")
      file (APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log 
        "Performing InitOnceExecuteOnce Test  failed with the following output:\n"
        "${OUTPUT}\n"
        "Return value: ${HAVE_IOEO_EXITCODE}\n")
    endif ("${HAVE_IOEO_EXITCODE}" EQUAL 0)
  endif ("${${HDF_PREFIX}_HAVE_IOEO}" MATCHES "^${${HDF_PREFIX}_HAVE_IOEO}$")
  endif (NOT HDF_NO_IOEO_TEST)
endif (WINDOWS)

#-----------------------------------------------------------------------------
# Determine how 'inline' is used
#-----------------------------------------------------------------------------
set (HDF_EXTRA_TEST_DEFINITIONS INLINE_TEST_INLINE)
foreach (inline_test inline __inline__ __inline)
  set (INLINE_TEST_INLINE ${inline_test})
  HDF_FUNCTION_TEST (INLINE_TEST_${inline_test})
endforeach (inline_test)

set (HDF_EXTRA_TEST_DEFINITIONS)
if (INLINE_TEST___inline__)
  set (${HDF_PREFIX}_inline __inline__)
else (INLINE_TEST___inline__)
  if (INLINE_TEST___inline)
    set (${HDF_PREFIX}_inline __inline)
  else (INLINE_TEST___inline)
    if (INLINE_TEST_inline)
      set (${HDF_PREFIX}_inline inline)
    endif (INLINE_TEST_inline)
  endif (INLINE_TEST___inline)
endif (INLINE_TEST___inline__)

#-----------------------------------------------------------------------------
# Check how to print a Long Long integer
#-----------------------------------------------------------------------------
if (NOT ${HDF_PREFIX}_PRINTF_LL_WIDTH OR ${HDF_PREFIX}_PRINTF_LL_WIDTH MATCHES "unknown")
  set (PRINT_LL_FOUND 0)
  message (STATUS "Checking for appropriate format for 64 bit long:")
  foreach (HDF5_PRINTF_LL l64 l L q I64 ll)
    set (CURRENT_TEST_DEFINITIONS "-DPRINTF_LL_WIDTH=${HDF5_PRINTF_LL}")
    if (${HDF_PREFIX}_SIZEOF_LONG_LONG)
      set (CURRENT_TEST_DEFINITIONS "${CURRENT_TEST_DEFINITIONS} -DHAVE_LONG_LONG")
    endif (${HDF_PREFIX}_SIZEOF_LONG_LONG)
    TRY_RUN (HDF5_PRINTF_LL_TEST_RUN   HDF5_PRINTF_LL_TEST_COMPILE
        ${CMAKE_BINARY_DIR}
        ${HDF_RESOURCES_EXT_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${CURRENT_TEST_DEFINITIONS}
        OUTPUT_VARIABLE OUTPUT
    )
    if (HDF5_PRINTF_LL_TEST_COMPILE)
      if (HDF5_PRINTF_LL_TEST_RUN MATCHES 0)
        set (${HDF_PREFIX}_PRINTF_LL_WIDTH "\"${HDF5_PRINTF_LL}\"" CACHE INTERNAL "Width for printf for type `long long' or `__int64', us. `ll")
        set (PRINT_LL_FOUND 1)
      else (HDF5_PRINTF_LL_TEST_RUN MATCHES 0)
        message ("Width with ${HDF5_PRINTF_LL} failed with result: ${HDF5_PRINTF_LL_TEST_RUN}")
      endif (HDF5_PRINTF_LL_TEST_RUN MATCHES 0)
    else (HDF5_PRINTF_LL_TEST_COMPILE)
      file (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Test ${HDF_PREFIX}_PRINTF_LL_WIDTH for ${HDF5_PRINTF_LL} failed with the following output:\n ${OUTPUT}\n"
      )
    endif (HDF5_PRINTF_LL_TEST_COMPILE)
  endforeach (HDF5_PRINTF_LL)

  if (PRINT_LL_FOUND)
    message (STATUS "Checking for apropriate format for 64 bit long: found ${${HDF_PREFIX}_PRINTF_LL_WIDTH}")
  else (PRINT_LL_FOUND)
    message (STATUS "Checking for apropriate format for 64 bit long: not found")
    set (${HDF_PREFIX}_PRINTF_LL_WIDTH "\"unknown\"" CACHE INTERNAL
        "Width for printf for type `long long' or `__int64', us. `ll"
    )
  endif (PRINT_LL_FOUND)
endif (NOT ${HDF_PREFIX}_PRINTF_LL_WIDTH OR ${HDF_PREFIX}_PRINTF_LL_WIDTH MATCHES "unknown")

# ----------------------------------------------------------------------
# Set the flag to indicate that the machine can handle converting
# denormalized floating-point values.
# (This flag should be set for all machines, except for the Crays, where
# the cache value is set in it's config file)
#
set (${HDF_PREFIX}_CONVERT_DENORMAL_FLOAT 1)
