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
include (${CMAKE_ROOT}/Modules/TestForSTDNamespace.cmake)

#-----------------------------------------------------------------------------
# Always SET this for now IF we are on an OS X box
#-----------------------------------------------------------------------------
if (APPLE)
  LIST (LENGTH CMAKE_OSX_ARCHITECTURES ARCH_LENGTH)
  if (ARCH_LENGTH GREATER 1)
    set (CMAKE_OSX_ARCHITECTURES "" CACHE STRING "" FORCE)
    message(FATAL_ERROR "Building Universal Binaries on OS X is NOT supported by the HDF5 project. This is"
    "due to technical reasons. The best approach would be build each architecture in separate directories"
    "and use the 'lipo' tool to combine them into a single executable or library. The 'CMAKE_OSX_ARCHITECTURES'"
    "variable has been set to a blank value which will build the default architecture for this system.")
  endif ()
  set (H4_AC_APPLE_UNIVERSAL_BUILD 0)
endif (APPLE)

#-----------------------------------------------------------------------------
# Option to Build HDF4 versions of NetCDF-3 APIS
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_NETCDF "Build HDF4 versions of NetCDF-3 APIS" ON)
if (HDF4_ENABLE_NETCDF)
  set (H4_HAVE_NETCDF 1)
endif (HDF4_ENABLE_NETCDF)

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
    set (WINDOWS 1) # MinGW tries to imitate Windows
    set (CMAKE_REQUIRED_FLAGS "-DWIN32_LEAN_AND_MEAN=1 -DNOGDI=1")
  endif (MINGW)
  set (CMAKE_REQUIRED_LIBRARIES "ws2_32.lib;wsock32.lib")
  if (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
    set (WINDOWS 1)
    set (CMAKE_REQUIRED_FLAGS "/DWIN32_LEAN_AND_MEAN=1 /DNOGDI=1")
  endif (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
endif (WIN32)

if (WINDOWS)
  set (H4_HAVE_STDDEF_H 1)
  set (H4_HAVE_SYS_STAT_H 1)
  set (H4_HAVE_SYS_TYPES_H 1)
  set (H4_HAVE_LIBM 1)
  set (H4_HAVE_STRDUP 1)
  set (H4_HAVE_SYSTEM 1)
  set (H4_HAVE_LONGJMP 1)
  if (NOT MINGW)
    set (H4_HAVE_GETHOSTNAME 1)
  endif (NOT MINGW)
  if (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
    set (H4_HAVE_GETCONSOLESCREENBUFFERINFO 1)
  endif (NOT UNIX AND NOT CYGWIN AND NOT MINGW)
  set (H4_HAVE_FUNCTION 1)
  set (H4_HAVE_TIMEZONE 1)
  if (MINGW)
    set (H4_HAVE_WINSOCK2_H 1)
  endif (MINGW)
  set (H4_HAVE_LIBWS2_32 1)
  set (H4_HAVE_LIBWSOCK32 1)
endif (WINDOWS)

# ----------------------------------------------------------------------
# END of WINDOWS Hard code Values
# ----------------------------------------------------------------------

if (CYGWIN)
  CHECK_LIBRARY_EXISTS_CONCAT ("rpc" xdr_opaque     H4_HAVE_RPC)
endif (CYGWIN)

#-----------------------------------------------------------------------------
#  Check for the math library "m"
#-----------------------------------------------------------------------------
if (NOT WINDOWS)
  CHECK_LIBRARY_EXISTS_CONCAT ("m" ceil     H4_HAVE_LIBM)
  CHECK_LIBRARY_EXISTS_CONCAT ("ws2_32" WSAStartup  H4_HAVE_LIBWS2_32)
  CHECK_LIBRARY_EXISTS_CONCAT ("wsock32" gethostbyname H4_HAVE_LIBWSOCK32)
endif (NOT WINDOWS)

CHECK_LIBRARY_EXISTS_CONCAT ("ucb"    gethostname  H4_HAVE_LIBUCB)
CHECK_LIBRARY_EXISTS_CONCAT ("socket" connect      H4_HAVE_LIBSOCKET)
CHECK_LIBRARY_EXISTS ("c" gethostbyname "" NOT_NEED_LIBNSL)

if (NOT NOT_NEED_LIBNSL)
  CHECK_LIBRARY_EXISTS_CONCAT ("nsl"    gethostbyname  H4_HAVE_LIBNSL)
endif (NOT NOT_NEED_LIBNSL)

# For other tests to use the same libraries
set (CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${LINK_LIBS})

set (USE_INCLUDES "")
if (WINDOWS)
  set (USE_INCLUDES ${USE_INCLUDES} "windows.h")
endif (WINDOWS)

if (NOT WINDOWS)
  TEST_BIG_ENDIAN(H4_WORDS_BIGENDIAN)
endif (NOT WINDOWS)

# For other specific tests, use this MACRO.
MACRO (HDF_FUNCTION_TEST OTHER_TEST)
  if ("H4_${OTHER_TEST}" MATCHES "^H4_${OTHER_TEST}$")
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
      if ("${H4_${def}}")
        set (MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}")
      endif ("${H4_${def}}")
    endforeach (def)

    if (LARGEFILE)
      set (MACRO_CHECK_FUNCTION_DEFINITIONS
          "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE"
      )
    endif (LARGEFILE)

    #message (STATUS "Performing ${OTHER_TEST}")
    TRY_COMPILE (${OTHER_TEST}
        ${CMAKE_BINARY_DIR}
        ${HDF4_RESOURCES_DIR}/HDFTests.c
        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
        "${OTHER_TEST_ADD_LIBRARIES}"
        OUTPUT_VARIABLE OUTPUT
    )
    if (${OTHER_TEST})
      set (H4_${OTHER_TEST} 1 CACHE INTERNAL "Other test ${FUNCTION}")
      message (STATUS "Performing Other Test ${OTHER_TEST} - Success")
    else (${OTHER_TEST})
      message (STATUS "Performing Other Test ${OTHER_TEST} - Failed")
      set (H4_${OTHER_TEST} "" CACHE INTERNAL "Other test ${FUNCTION}")
      FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Performing Other Test ${OTHER_TEST} failed with the following output:\n"
          "${OUTPUT}\n"
      )
    endif (${OTHER_TEST})
  endif ("H4_${OTHER_TEST}" MATCHES "^H4_${OTHER_TEST}$")
ENDMACRO (HDF_FUNCTION_TEST)

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
CHECK_INCLUDE_FILE_CONCAT ("unistd.h"        H4_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/stat.h"      H4_HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE_CONCAT ("sys/types.h"     H4_HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT ("stddef.h"        H4_HAVE_STDDEF_H)
CHECK_INCLUDE_FILE_CONCAT ("stdint.h"        H4_HAVE_STDINT_H)

# IF the c compiler found stdint, check the C++ as well. On some systems this
# file will be found by C but not C++, only do this test IF the C++ compiler
# has been initialized (e.g. the project also includes some c++)
if (H4_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)
  CHECK_INCLUDE_FILE_CXX ("stdint.h" H4_HAVE_STDINT_H_CXX)
  if (NOT H4_HAVE_STDINT_H_CXX)
    set (H4_HAVE_STDINT_H "" CACHE INTERNAL "Have includes HAVE_STDINT_H")
    set (USE_INCLUDES ${USE_INCLUDES} "stdint.h")
  endif (NOT H4_HAVE_STDINT_H_CXX)
endif (H4_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)

# Windows
if (NOT CYGWIN)
  CHECK_INCLUDE_FILE_CONCAT ("winsock2.h"      H4_HAVE_WINSOCK2_H)
endif (NOT CYGWIN)
CHECK_INCLUDE_FILE_CONCAT ("pthread.h"       H4_HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE_CONCAT ("string.h"        H4_HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT ("strings.h"       H4_HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT ("stdlib.h"        H4_HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CONCAT ("memory.h"        H4_HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CONCAT ("dlfcn.h"         H4_HAVE_DLFCN_H)
CHECK_INCLUDE_FILE_CONCAT ("inttypes.h"      H4_HAVE_INTTYPES_H)

#-----------------------------------------------------------------------------
#  Check for large file support
#-----------------------------------------------------------------------------

# The linux-lfs option is deprecated.
set (LINUX_LFS 0)

set (HDF_EXTRA_FLAGS)
if (NOT WINDOWS)
  # Linux Specific flags
#  Removed _POSIX_SOURCE due to OS X build errors
    set (HDF_EXTRA_FLAGS -D_BSD_SOURCE)
  option (HDF_ENABLE_LARGE_FILE "Enable support for large (64-bit) files on Linux." ON)
  if (HDF_ENABLE_LARGE_FILE)
    set (msg "Performing TEST_LFS_WORKS")
    TRY_RUN (TEST_LFS_WORKS_RUN   TEST_LFS_WORKS_COMPILE
        ${HDF4_BINARY_DIR}/CMake
        ${HDF4_RESOURCES_DIR}/HDFTests.c
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
        FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
              "Test TEST_LFS_WORKS Run failed with the following output and exit code:\n ${OUTPUT}\n"
        )
      endif (TEST_LFS_WORKS_RUN  MATCHES 0)
    else (TEST_LFS_WORKS_COMPILE )
      set (TEST_LFS_WORKS "" CACHE INTERNAL ${msg})
      message (STATUS "${msg}... no")
      FILE (APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
          "Test TEST_LFS_WORKS Compile failed with the following output:\n ${OUTPUT}\n"
      )
    endif (TEST_LFS_WORKS_COMPILE)
  endif (HDF_ENABLE_LARGE_FILE)
  set (CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} ${HDF_EXTRA_FLAGS})
endif (NOT WINDOWS)

ADD_DEFINITIONS (${HDF_EXTRA_FLAGS})

#-----------------------------------------------------------------------------
#  Check the size in bytes of all the int and float types
#-----------------------------------------------------------------------------
MACRO (H4_CHECK_TYPE_SIZE type var)
  set (aType ${type})
  set (aVar  ${var})
#  message (STATUS "Checking size of ${aType} and storing into ${aVar}")
  CHECK_TYPE_SIZE (${aType}   ${aVar})
  if (NOT ${aVar})
    set (${aVar} 0 CACHE INTERNAL "SizeOf for ${aType}")
#    message (STATUS "Size of ${aType} was NOT Found")
  endif (NOT ${aVar})
ENDMACRO (H4_CHECK_TYPE_SIZE)


H4_CHECK_TYPE_SIZE (char           H4_SIZEOF_CHAR)
H4_CHECK_TYPE_SIZE (short          H4_SIZEOF_SHORT)
H4_CHECK_TYPE_SIZE (int            H4_SIZEOF_INT)
H4_CHECK_TYPE_SIZE (unsigned       H4_SIZEOF_UNSIGNED)
if (NOT APPLE)
  H4_CHECK_TYPE_SIZE (long         H4_SIZEOF_LONG)
endif (NOT APPLE)
H4_CHECK_TYPE_SIZE ("long long"    H4_SIZEOF_LONG_LONG)
H4_CHECK_TYPE_SIZE (__int64        H4_SIZEOF___INT64)
if (NOT H4_SIZEOF___INT64)
  set (H4_SIZEOF___INT64 0)
endif (NOT H4_SIZEOF___INT64)

H4_CHECK_TYPE_SIZE (float          H4_SIZEOF_FLOAT)
H4_CHECK_TYPE_SIZE (double         H4_SIZEOF_DOUBLE)
H4_CHECK_TYPE_SIZE ("long double"  H4_SIZEOF_LONG_DOUBLE)

H4_CHECK_TYPE_SIZE (int8_t         H4_SIZEOF_INT8_T)
H4_CHECK_TYPE_SIZE (uint8_t        H4_SIZEOF_UINT8_T)
H4_CHECK_TYPE_SIZE (int_least8_t   H4_SIZEOF_INT_LEAST8_T)
H4_CHECK_TYPE_SIZE (uint_least8_t  H4_SIZEOF_UINT_LEAST8_T)
H4_CHECK_TYPE_SIZE (int_fast8_t    H4_SIZEOF_INT_FAST8_T)
H4_CHECK_TYPE_SIZE (uint_fast8_t   H4_SIZEOF_UINT_FAST8_T)

H4_CHECK_TYPE_SIZE (int16_t        H4_SIZEOF_INT16_T)
H4_CHECK_TYPE_SIZE (uint16_t       H4_SIZEOF_UINT16_T)
H4_CHECK_TYPE_SIZE (int_least16_t  H4_SIZEOF_INT_LEAST16_T)
H4_CHECK_TYPE_SIZE (uint_least16_t H4_SIZEOF_UINT_LEAST16_T)
H4_CHECK_TYPE_SIZE (int_fast16_t   H4_SIZEOF_INT_FAST16_T)
H4_CHECK_TYPE_SIZE (uint_fast16_t  H4_SIZEOF_UINT_FAST16_T)

H4_CHECK_TYPE_SIZE (int32_t        H4_SIZEOF_INT32_T)
H4_CHECK_TYPE_SIZE (uint32_t       H4_SIZEOF_UINT32_T)
H4_CHECK_TYPE_SIZE (int_least32_t  H4_SIZEOF_INT_LEAST32_T)
H4_CHECK_TYPE_SIZE (uint_least32_t H4_SIZEOF_UINT_LEAST32_T)
H4_CHECK_TYPE_SIZE (int_fast32_t   H4_SIZEOF_INT_FAST32_T)
H4_CHECK_TYPE_SIZE (uint_fast32_t  H4_SIZEOF_UINT_FAST32_T)

H4_CHECK_TYPE_SIZE (int64_t        H4_SIZEOF_INT64_T)
H4_CHECK_TYPE_SIZE (uint64_t       H4_SIZEOF_UINT64_T)
H4_CHECK_TYPE_SIZE (int_least64_t  H4_SIZEOF_INT_LEAST64_T)
H4_CHECK_TYPE_SIZE (uint_least64_t H4_SIZEOF_UINT_LEAST64_T)
H4_CHECK_TYPE_SIZE (int_fast64_t   H4_SIZEOF_INT_FAST64_T)
H4_CHECK_TYPE_SIZE (uint_fast64_t  H4_SIZEOF_UINT_FAST64_T)
if (NOT APPLE)
  H4_CHECK_TYPE_SIZE (size_t       H4_SIZEOF_SIZE_T)
  H4_CHECK_TYPE_SIZE (ssize_t      H4_SIZEOF_SSIZE_T)
  if (NOT H4_SIZEOF_SSIZE_T)
    set (H4_SIZEOF_SSIZE_T 0)
  endif (NOT H4_SIZEOF_SSIZE_T)
endif (NOT APPLE)
H4_CHECK_TYPE_SIZE (off_t          H4_SIZEOF_OFF_T)
H4_CHECK_TYPE_SIZE (off64_t        H4_SIZEOF_OFF64_T)
if (NOT H4_SIZEOF_OFF64_T)
  set (H4_SIZEOF_OFF64_T 0)
endif (NOT H4_SIZEOF_OFF64_T)


#-----------------------------------------------------------------------------
# Check for some functions that are used
#
CHECK_FUNCTION_EXISTS (fork              H4_HAVE_FORK)
CHECK_FUNCTION_EXISTS (strdup            H4_HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (system            H4_HAVE_SYSTEM)
CHECK_FUNCTION_EXISTS (wait              H4_HAVE_WAIT)
CHECK_FUNCTION_EXISTS (vfork             H4_HAVE_VFORK)
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Check a bunch of other functions
#-----------------------------------------------------------------------------
if (NOT WINDOWS)
  foreach (test
      STDC_HEADERS
      HAVE_FUNCTION
  )
    HDF_FUNCTION_TEST (${test})
  endforeach (test)
endif (NOT WINDOWS)
