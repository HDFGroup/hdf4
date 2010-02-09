#-----------------------------------------------------------------------------
# Include all the necessary files for macros
#-----------------------------------------------------------------------------
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFortranFunctionExists.cmake)

MESSAGE(STATUS "Configure Checks that still need to be implemented")
MESSAGE(STATUS "  GetConsoleScreenBufferInfo function for Windows")

#-----------------------------------------------------------------------------
# Always SET this for now IF we are on an OS X box
#-----------------------------------------------------------------------------
IF (APPLE)
    SET(H4_AC_APPLE_UNIVERSAL_BUILD 1)
ENDIF()

#-----------------------------------------------------------------------------
# Option to Clear File Buffers before write --enable-clear-file-buffers
#-----------------------------------------------------------------------------
OPTION (HDF4_Enable_NetCDF "Build HDF4 versions of NetCDF-3 APIS" ON)
IF (HDF4_Enable_NetCDF)
  SET (H4_HAVE_NETCDF 1)
ENDIF (HDF4_Enable_NetCDF)

SET(WINDOWS)
IF(WIN32)
  IF(NOT UNIX)
    SET(WINDOWS 1)
  ENDIF(NOT UNIX)
ENDIF(WIN32)

#-----------------------------------------------------------------------------
# This MACRO checks IF the symbol exists in the library and IF it
# does, it appends library to the list.
#-----------------------------------------------------------------------------
SET(LINK_LIBS "")
MACRO(CHECK_LIBRARY_EXISTS_CONCAT LIBRARY SYMBOL VARIABLE)
  CHECK_LIBRARY_EXISTS("${LIBRARY};${LINK_LIBS}" ${SYMBOL} "" ${VARIABLE})
  IF(${VARIABLE})
    SET(LINK_LIBS ${LINK_LIBS} ${LIBRARY})
  ENDIF(${VARIABLE})
ENDMACRO(CHECK_LIBRARY_EXISTS_CONCAT)

#-----------------------------------------------------------------------------
#  Check for the math library "m"
#-----------------------------------------------------------------------------
IF(WINDOWS)
  SET(H4_HAVE_LIBM 1)
ELSE(WINDOWS)
  CHECK_LIBRARY_EXISTS_CONCAT("m" printf     H4_HAVE_LIBM)
ENDIF(WINDOWS)
CHECK_LIBRARY_EXISTS_CONCAT("ws2_32" printf  H4_HAVE_LIBWS2_32)
CHECK_LIBRARY_EXISTS_CONCAT("wsock32" printf H4_HAVE_LIBWSOCK32)
#CHECK_LIBRARY_EXISTS_CONCAT("dl"     dlopen       H4_HAVE_LIBDL)
CHECK_LIBRARY_EXISTS_CONCAT("ucb"    gethostname  H4_HAVE_LIBUCB)
CHECK_LIBRARY_EXISTS_CONCAT("socket" connect      H4_HAVE_LIBSOCKET)
CHECK_LIBRARY_EXISTS("c" gethostbyname "" NOT_NEED_LIBNSL)


IF(NOT NOT_NEED_LIBNSL)
  CHECK_LIBRARY_EXISTS_CONCAT("nsl"    gethostbyname  H4_HAVE_LIBNSL)
ENDIF(NOT NOT_NEED_LIBNSL)


SET(USE_INCLUDES "")
#-----------------------------------------------------------------------------
# Check IF header file exists and add it to the list.
#-----------------------------------------------------------------------------
MACRO(CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES("${USE_INCLUDES};${FILE}" ${VARIABLE})
  IF(${VARIABLE})
    SET(USE_INCLUDES ${USE_INCLUDES} ${FILE})
  ENDIF(${VARIABLE})
ENDMACRO(CHECK_INCLUDE_FILE_CONCAT)

#-----------------------------------------------------------------------------
# If we are on Windows we know some of the answers to these tests already
#-----------------------------------------------------------------------------
IF(WINDOWS)
  SET(H4_HAVE_IO_H 1)
  SET(H4_HAVE_SETJMP_H 1)
  SET(H4_HAVE_STDDEF_H 1)
  SET(H4_HAVE_SYS_STAT_H 1)
  SET(H4_HAVE_SYS_TIMEB_H 1)
  SET(H4_HAVE_SYS_TYPES_H 1)
  SET(H4_HAVE_WINSOCK_H 1)
ENDIF(WINDOWS)

#-----------------------------------------------------------------------------
#  Check for the existence of certain header files
#-----------------------------------------------------------------------------
CHECK_INCLUDE_FILE_CONCAT("globus/common.h" H4_HAVE_GLOBUS_COMMON_H)
CHECK_INCLUDE_FILE_CONCAT("io.h"            H4_HAVE_IO_H)
CHECK_INCLUDE_FILE_CONCAT("mfhdf.h"         H4_HAVE_MFHDF_H)
CHECK_INCLUDE_FILE_CONCAT("pdb.h"           H4_HAVE_PDB_H)
CHECK_INCLUDE_FILE_CONCAT("pthread.h"       H4_HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE_CONCAT("setjmp.h"        H4_HAVE_SETJMP_H)
CHECK_INCLUDE_FILE_CONCAT("srbclient.h"     H4_HAVE_SRBCLIENT_H)
CHECK_INCLUDE_FILE_CONCAT("stddef.h"        H4_HAVE_STDDEF_H)
CHECK_INCLUDE_FILE_CONCAT("stdint.h"        H4_HAVE_STDINT_H)
CHECK_INCLUDE_FILE_CONCAT("string.h"        H4_HAVE_STRING_H)
CHECK_INCLUDE_FILE_CONCAT("strings.h"       H4_HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT("sys/ioctl.h"     H4_HAVE_SYS_IOCTL_H)
CHECK_INCLUDE_FILE_CONCAT("sys/proc.h"      H4_HAVE_SYS_PROC_H)
CHECK_INCLUDE_FILE_CONCAT("sys/resource.h"  H4_HAVE_SYS_RESOURCE_H)
CHECK_INCLUDE_FILE_CONCAT("sys/socket.h"    H4_HAVE_SYS_SOCKET_H)
CHECK_INCLUDE_FILE_CONCAT("sys/stat.h"      H4_HAVE_SYS_STAT_H)
IF(CMAKE_SYSTEM_NAME MATCHES "OSF")
  CHECK_INCLUDE_FILE_CONCAT("sys/sysinfo.h" H4_HAVE_SYS_SYSINFO_H)
ELSE(CMAKE_SYSTEM_NAME MATCHES "OSF")
  SET(H4_HAVE_SYS_SYSINFO_H "" CACHE INTERNAL "" FORCE)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "OSF")
CHECK_INCLUDE_FILE_CONCAT("sys/time.h"      H4_HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CONCAT("time.h"          H4_HAVE_TIME_H)
CHECK_INCLUDE_FILE_CONCAT("sys/timeb.h"     H4_HAVE_SYS_TIMEB_H)
CHECK_INCLUDE_FILE_CONCAT("sys/types.h"     H4_HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CONCAT("unistd.h"        H4_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE_CONCAT("stdlib.h"        H4_HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CONCAT("memory.h"        H4_HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CONCAT("dlfcn.h"         H4_HAVE_DLFCN_H)
CHECK_INCLUDE_FILE_CONCAT("features.h"      H4_HAVE_FEATURES_H)
CHECK_INCLUDE_FILE_CONCAT("inttypes.h"      H4_HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE_CONCAT("winsock.h"       H4_HAVE_WINSOCK_H)
CHECK_INCLUDE_FILE_CONCAT("netinet/in.h"    H4_HAVE_NETINET_IN_H)


# IF the c compiler found stdint, check the C++ as well. On some systems this
# file will be found by C but not C++, only do this test IF the C++ compiler
# has been initialized (e.g. the project also includes some c++) 
IF (H4_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)
  CHECK_INCLUDE_FILE_CXX("stdint.h" H4_HAVE_STDINT_H_CXX)
  IF (NOT H4_HAVE_STDINT_H_CXX)
    SET (H4_HAVE_STDINT_H "" CACHE INTERNAL "Have includes HAVE_STDINT_H")
  ENDIF (NOT H4_HAVE_STDINT_H_CXX)
ENDIF (H4_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)

#-----------------------------------------------------------------------------
#  Check the size in bytes of all the int and float types
#-----------------------------------------------------------------------------
MACRO ( H4_CHECK_TYPE_SIZE type var)
SET (aType ${type})
SET (aVar  ${var})
#MESSAGE(STATUS "Checking size of ${aType} and storing into ${aVar}") 
    CHECK_TYPE_SIZE(${aType}   ${aVar})
    IF (NOT ${aVar})
        SET (${aVar} 0 CACHE INTERNAL "SizeOf for ${aType}")
 #       MESSAGE(STATUS "Size of ${aType} was NOT Found") 
    ENDIF()
ENDMACRO()



H4_CHECK_TYPE_SIZE(char           H4_SIZEOF_CHAR)
H4_CHECK_TYPE_SIZE(short          H4_SIZEOF_SHORT)
H4_CHECK_TYPE_SIZE(int            H4_SIZEOF_INT)
H4_CHECK_TYPE_SIZE(unsigned        H4_SIZEOF_UNSIGNED)
IF (NOT APPLE)
    H4_CHECK_TYPE_SIZE(long           H4_SIZEOF_LONG)
ENDIF()
H4_CHECK_TYPE_SIZE("long long"    H4_SIZEOF_LONG_LONG)
H4_CHECK_TYPE_SIZE(__int64        H4_SIZEOF___INT64)
IF (NOT H4_SIZEOF___INT64)
    SET (H4_SIZEOF___INT64 0)
ENDIF()

H4_CHECK_TYPE_SIZE(float          H4_SIZEOF_FLOAT)
H4_CHECK_TYPE_SIZE(double         H4_SIZEOF_DOUBLE)
H4_CHECK_TYPE_SIZE("long double"  H4_SIZEOF_LONG_DOUBLE)
H4_CHECK_TYPE_SIZE(int8_t         H4_SIZEOF_INT8_T)
H4_CHECK_TYPE_SIZE(uint8_t        H4_SIZEOF_UINT8_T)
H4_CHECK_TYPE_SIZE(int_least8_t   H4_SIZEOF_INT_LEAST8_T)
H4_CHECK_TYPE_SIZE(uint_least8_t  H4_SIZEOF_UINT_LEAST8_T)
H4_CHECK_TYPE_SIZE(int_fast8_t    H4_SIZEOF_INT_FAST8_T)
H4_CHECK_TYPE_SIZE(uint_fast8_t   H4_SIZEOF_UINT_FAST8_T)
H4_CHECK_TYPE_SIZE(int16_t        H4_SIZEOF_INT16_T)
H4_CHECK_TYPE_SIZE(uint16_t       H4_SIZEOF_UINT16_T)
H4_CHECK_TYPE_SIZE(int_least16_t  H4_SIZEOF_INT_LEAST16_T)
H4_CHECK_TYPE_SIZE(uint_least16_t H4_SIZEOF_UINT_LEAST16_T)
H4_CHECK_TYPE_SIZE(int_fast16_t   H4_SIZEOF_INT_FAST16_T)
H4_CHECK_TYPE_SIZE(uint_fast16_t  H4_SIZEOF_UINT_FAST16_T)
H4_CHECK_TYPE_SIZE(int32_t        H4_SIZEOF_INT32_T)
H4_CHECK_TYPE_SIZE(uint32_t       H4_SIZEOF_UINT32_T)
H4_CHECK_TYPE_SIZE(int_least32_t  H4_SIZEOF_INT_LEAST32_T)
H4_CHECK_TYPE_SIZE(uint_least32_t H4_SIZEOF_UINT_LEAST32_T)
H4_CHECK_TYPE_SIZE(int_fast32_t   H4_SIZEOF_INT_FAST32_T)
H4_CHECK_TYPE_SIZE(uint_fast32_t  H4_SIZEOF_UINT_FAST32_T)
H4_CHECK_TYPE_SIZE(int64_t        H4_SIZEOF_INT64_T)
H4_CHECK_TYPE_SIZE(uint64_t       H4_SIZEOF_UINT64_T)
H4_CHECK_TYPE_SIZE(int_least64_t  H4_SIZEOF_INT_LEAST64_T)
H4_CHECK_TYPE_SIZE(uint_least64_t H4_SIZEOF_UINT_LEAST64_T)
H4_CHECK_TYPE_SIZE(int_fast64_t   H4_SIZEOF_INT_FAST64_T)
H4_CHECK_TYPE_SIZE(uint_fast64_t  H4_SIZEOF_UINT_FAST64_T)
IF (NOT APPLE)
    H4_CHECK_TYPE_SIZE(size_t         H4_SIZEOF_SIZE_T)
    H4_CHECK_TYPE_SIZE(ssize_t        H4_SIZEOF_SSIZE_T)
    IF (NOT H4_SIZEOF_SSIZE_T)
        SET (H4_SIZEOF_SSIZE_T 0)
    ENDIF()
ENDIF()
H4_CHECK_TYPE_SIZE(off_t          H4_SIZEOF_OFF_T)
H4_CHECK_TYPE_SIZE(off64_t        H4_SIZEOF_OFF64_T)
IF (NOT H4_SIZEOF_OFF64_T)
    SET (H4_SIZEOF_OFF64_T 0)
ENDIF()


# For other tests to use the same libraries
SET(CMAKE_REQUIRED_LIBRARIES ${LINK_LIBS})

#-----------------------------------------------------------------------------
# Check for some functions that are used
IF(WINDOWS)
  SET(H4_HAVE_STRDUP 1)
  SET(H4_HAVE_SYSTEM 1)
  SET(H4_HAVE_DIFFTIME 1)
  SET(H4_HAVE_LONGJMP 1)
  SET(H4_STDC_HEADERS 1)
  SET(H4_HAVE_GETHOSTNAME 1)
ENDIF(WINDOWS)

CHECK_FUNCTION_EXISTS(strdup            H4_HAVE_STRDUP)
CHECK_FUNCTION_EXISTS(snprintf          H4_HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(system            H4_HAVE_SYSTEM)
CHECK_FUNCTION_EXISTS(vsnprintf         H4_HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS(waitpid           H4_HAVE_WAITPID)
CHECK_FUNCTION_EXISTS(signal            H4_HAVE_SIGNAL)
CHECK_FUNCTION_EXISTS(ioctl             H4_HAVE_IOCTL)
CHECK_FUNCTION_EXISTS(sigaction         H4_HAVE_SIGACTION)
CHECK_FUNCTION_EXISTS(getpwuid          H4_HAVE_GETPWUID)
CHECK_FUNCTION_EXISTS(getrusage         H4_HAVE_GETRUSAGE)
#CHECK_FUNCTION_EXISTS(gettimeofday      H4_HAVE_GETTIMEOFDAY)
CHECK_FUNCTION_EXISTS(longjmp           H4_HAVE_LONGJMP)
CHECK_FUNCTION_EXISTS(fork              H4_HAVE_FORK)
CHECK_FUNCTION_EXISTS(difftime          H4_HAVE_DIFFTIME)
CHECK_FUNCTION_EXISTS(gethostname       H4_HAVE_GETHOSTNAME)
CHECK_FUNCTION_EXISTS(sigaction         H4_HAVE_SIGACTION)
CHECK_FUNCTION_EXISTS(frexpf            H4_HAVE_FREXPF)
CHECK_FUNCTION_EXISTS(frexpl            H4_HAVE_FREXPL)
CHECK_FUNCTION_EXISTS(alarm             H4_HAVE_ALARM)
CHECK_FUNCTION_EXISTS(fseeko            H4_HAVE_FSEEKO)
CHECK_FUNCTION_EXISTS(ftello            H4_HAVE_FTELLO)
CHECK_FUNCTION_EXISTS(fstat64           H4_HAVE_FSTAT64)
CHECK_FUNCTION_EXISTS(random            H4_HAVE_RANDOM)
CHECK_FUNCTION_EXISTS(rand_r            H4_HAVE_RAND_R)
CHECK_FUNCTION_EXISTS(siglongjmp        H4_HAVE_SIGLONGJMP)
CHECK_FUNCTION_EXISTS(srandom           H4_HAVE_SRANDOM)
CHECK_FUNCTION_EXISTS(stat64            H4_HAVE_STAT64)
CHECK_FUNCTION_EXISTS(vasprintf         H4_HAVE_VASPRINTF)

# MPI checks
SET(CMAKE_REQUIRED_INCLUDES "${MPI_INCLUDE_PATH}/mpi.h" )
SET(CMAKE_REQUIRED_LIBRARIES "${MPI_LIBRARY}" )
CHECK_FUNCTION_EXISTS(MPI_File_get_size H4_HAVE_MPI_GET_SIZE)

# For Fortran+MPI
CHECK_SYMBOL_EXISTS(MPI_Comm_c2f "${MPI_INCLUDE_PATH}/mpi.h"  H4_HAVE_MPI_MULTI_LANG_Comm)
CHECK_SYMBOL_EXISTS(MPI_Info_c2f "${MPI_INCLUDE_PATH}/mpi.h"  H4_HAVE_MPI_MULTI_LANG_Info)

#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
#  Since gettimeofday is not defined any where standard, lets look in all the
#  usual places. On MSVC we are just going to use ::clock()
#-----------------------------------------------------------------------------
IF (NOT MSVC)
  IF("H4_HAVE_TIME_GETTIMEOFDAY" MATCHES "^H4_HAVE_TIME_GETTIMEOFDAY$")
    TRY_COMPILE(HAVE_TIME_GETTIMEOFDAY
          ${CMAKE_BINARY_DIR}
           ${HDF4_SOURCE_DIR}/Resources/GetTimeOfDayTest.cpp
          COMPILE_DEFINITIONS -DTRY_TIME_H
          OUTPUT_VARIABLE OUTPUT)
    IF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
        SET (H4_HAVE_TIME_GETTIMEOFDAY "1" CACHE INTERNAL "H4_HAVE_TIME_GETTIMEOFDAY")
    ENDIF (HAVE_TIME_GETTIMEOFDAY STREQUAL "TRUE")
  ENDIF()

  IF("H4_HAVE_SYS_TIME_GETTIMEOFDAY" MATCHES "^H4_HAVE_SYS_TIME_GETTIMEOFDAY$")
    TRY_COMPILE(HAVE_SYS_TIME_GETTIMEOFDAY
          ${CMAKE_BINARY_DIR}
          ${HDF4_SOURCE_DIR}/Resources/GetTimeOfDayTest.cpp
          COMPILE_DEFINITIONS -DTRY_SYS_TIME_H
          OUTPUT_VARIABLE OUTPUT)
    IF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
        SET (H4_HAVE_SYS_TIME_GETTIMEOFDAY "1" CACHE INTERNAL "H4_HAVE_SYS_TIME_GETTIMEOFDAY")
    ENDIF (HAVE_SYS_TIME_GETTIMEOFDAY STREQUAL "TRUE")
  ENDIF()
ENDIF()

IF (NOT HAVE_SYS_TIME_GETTIMEOFDAY AND NOT H4_HAVE_GETTIMEOFDAY AND NOT MSVC)
    MESSAGE(STATUS "---------------------------------------------------------------")
    MESSAGE(STATUS "Function 'gettimeofday()' was not found. HDF4 will use its")
    MESSAGE(STATUS "  own implementation.. This can happen on older versions of")
    MESSAGE(STATUS "  MinGW on Windows. Consider upgrading your MinGW installation")
    MESSAGE(STATUS "  to a newer version such as MinGW 3.12")
    MESSAGE(STATUS "---------------------------------------------------------------")
ENDIF()


# Check for Symbols
CHECK_SYMBOL_EXISTS ( tzname "time.h" H4_HAVE_DECL_TZNAME)

#-----------------------------------------------------------------------------
#
#-----------------------------------------------------------------------------
IF(NOT WINDOWS)
  CHECK_SYMBOL_EXISTS(TIOCGWINSZ "sys/ioctl.h" H4_HAVE_TIOCGWINSZ)
  CHECK_SYMBOL_EXISTS(TIOCGETD   "sys/ioctl.h" H4_HAVE_TIOCGETD)
ENDIF(NOT WINDOWS)

#-----------------------------------------------------------------------------
#  Check for the Stream VFD driver
#-----------------------------------------------------------------------------
IF(HDF4_STREAM_VFD)
  CHECK_INCLUDE_FILE_CONCAT("netdb.h"       H4_HAVE_NETDB_H)
  CHECK_INCLUDE_FILE_CONCAT("netinet/tcp.h" H4_HAVE_NETINET_TCP_H)
  CHECK_INCLUDE_FILE_CONCAT("sys/filio.h"   H4_HAVE_SYS_FILIO_H)
  SET(H4_HAVE_STREAM 1)
ENDIF(HDF4_STREAM_VFD)


# For other other specific tests, use this MACRO.
MACRO(HDF4_FUNCTION_TEST OTHER_TEST)
  IF("H4_${OTHER_TEST}" MATCHES "^H4_${OTHER_TEST}$")
    SET(MACRO_CHECK_FUNCTION_DEFINITIONS "-D${OTHER_TEST} ${CMAKE_REQUIRED_FLAGS}")
    SET(OTHER_TEST_ADD_LIBRARIES)
    IF(CMAKE_REQUIRED_LIBRARIES)
      SET(OTHER_TEST_ADD_LIBRARIES "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    ENDIF(CMAKE_REQUIRED_LIBRARIES)
    FOREACH(def ${HDF4_EXTRA_TEST_DEFINITIONS})
      SET(MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}=${${def}}")
    ENDFOREACH(def)

    FOREACH(def
        H4_HAVE_SYS_TIME_H
        H4_HAVE_UNISTD_H
        H4_HAVE_SYS_TYPES_H
        H4_HAVE_SYS_SOCKET_H)
      IF("${def}")
        SET(MACRO_CHECK_FUNCTION_DEFINITIONS "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D${def}")
      ENDIF("${def}")
    ENDFOREACH(def)
    IF(LINUX_LFS)
      SET(MACRO_CHECK_FUNCTION_DEFINITIONS
        "${MACRO_CHECK_FUNCTION_DEFINITIONS} -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE")
    ENDIF(LINUX_LFS)

    # (STATUS "Performing ${OTHER_TEST}")
    TRY_COMPILE(${OTHER_TEST}
      ${CMAKE_BINARY_DIR}
      ${HDF4_SOURCE_DIR}/Resources/HDF4Tests.c
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      "${OTHER_TEST_ADD_LIBRARIES}"
      OUTPUT_VARIABLE OUTPUT)
    IF(${OTHER_TEST})
      SET(H4_${OTHER_TEST} 1 CACHE INTERNAL "Other test ${FUNCTION}")
      MESSAGE(STATUS "Performing Other Test ${OTHER_TEST} - Success")
    ELSE(${OTHER_TEST})
      MESSAGE(STATUS "Performing Other Test ${OTHER_TEST} - Failed")
      SET(H4_${OTHER_TEST} "" CACHE INTERNAL "Other test ${FUNCTION}")
      WRITE_FILE(${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log 
        "Performing Other Test ${OTHER_TEST} failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    ENDIF(${OTHER_TEST})
  ENDIF("H4_${OTHER_TEST}" MATCHES "^H4_${OTHER_TEST}$")
ENDMACRO(HDF4_FUNCTION_TEST) 

#-----------------------------------------------------------------------------
# Check a bunch of other functions
#-----------------------------------------------------------------------------
IF(WINDOWS)
  SET(H4_HAVE_TIMEZONE 1)
  SET(H4_HAVE_FUNCTION 1)
ELSE(WINDOWS)
  FOREACH(test 
      TIME_WITH_SYS_TIME
      STDC_HEADERS
      HAVE_TM_ZONE
      HAVE_STRUCT_TM_TM_ZONE
      HAVE_ATTRIBUTE
      HAVE_FUNCTION
      HAVE_TM_GMTOFF
      HAVE_TIMEZONE
      HAVE_STRUCT_TIMEZONE
      HAVE_STAT_ST_BLOCKS
      HAVE_FUNCTION
      SYSTEM_SCOPE_THREADS
      HAVE_SOCKLEN_T
      DEV_T_IS_SCALAR
      HAVE_OFF64_T
      GETTIMEOFDAY_GIVES_TZ
      VSNPRINTF_WORKS
      HAVE_C99_FUNC
      HAVE_C99_DESIGNATED_INITIALIZER
      CXX_HAVE_OFFSETOF
      )
    HDF4_FUNCTION_TEST(${test})
  ENDFOREACH(test)
ENDIF(WINDOWS)

#-----------------------------------------------------------------------------
# Look for 64 bit file stream capability
#-----------------------------------------------------------------------------
IF(HAVE_OFF64_T)
  CHECK_FUNCTION_EXISTS(lseek64           H4_HAVE_LSEEK64)
  CHECK_FUNCTION_EXISTS(fseek64           H4_HAVE_FSEEK64)
ENDIF(HAVE_OFF64_T)

#-----------------------------------------------------------------------------
# Determine how 'inline' is used
#-----------------------------------------------------------------------------
SET(HDF4_EXTRA_TEST_DEFINITIONS INLINE_TEST_INLINE)
FOREACH(inline_test inline __inline__ __inline)
  SET(INLINE_TEST_INLINE ${inline_test})
  HDF4_FUNCTION_TEST(INLINE_TEST_${inline_test})
ENDFOREACH(inline_test)
SET(HDF4_EXTRA_TEST_DEFINITIONS)
IF(INLINE_TEST___inline__)
  SET(H4_inline __inline__)
ELSE(INLINE_TEST___inline__)
  IF(INLINE_TEST___inline)
    SET(H4_inline __inline)
  ELSE(INLINE_TEST___inline)
    IF(INLINE_TEST_inline)
      SET(H4_inline inline)
    ENDIF(INLINE_TEST_inline)
  ENDIF(INLINE_TEST___inline)
ENDIF(INLINE_TEST___inline__)

#-----------------------------------------------------------------------------
# Check how to print a Long Long integer
#-----------------------------------------------------------------------------
SET(H4_H4_PRINTF_LL_WIDTH "H4_PRINTF_LL_WIDTH")
IF (H4_PRINTF_LL_WIDTH MATCHES "^H4_PRINTF_LL_WIDTH$")
  SET(PRINT_LL_FOUND 0)
  MESSAGE(STATUS "Checking for appropriate format for 64 bit long:")
  FOREACH(HDF4_PRINTF_LL l64 l L q I64 ll)
    SET(CURRENT_TEST_DEFINITIONS "-DPRINTF_LL_WIDTH=${HDF4_PRINTF_LL}")
    IF (H4_SIZEOF_LONG_LONG)
      SET(CURRENT_TEST_DEFINITIONS "${CURRENT_TEST_DEFINITIONS} -DHAVE_LONG_LONG")
    ENDIF (H4_SIZEOF_LONG_LONG)
    TRY_RUN(HDF4_PRINTF_LL_TEST_RUN   HDF4_PRINTF_LL_TEST_COMPILE
      ${HDF4_BINARY_DIR}/CMake
      ${HDF4_SOURCE_DIR}/Resources/HDF4Tests.c
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${CURRENT_TEST_DEFINITIONS}
      OUTPUT_VARIABLE OUTPUT)
    IF (HDF4_PRINTF_LL_TEST_COMPILE)
      IF (HDF4_PRINTF_LL_TEST_RUN MATCHES 0)
        SET(H4_PRINTF_LL_WIDTH "\"${HDF4_PRINTF_LL}\"" CACHE INTERNAL "Width for printf for type `long long' or `__int64', us. `ll")
        SET(PRINT_LL_FOUND 1)
      ELSE (HDF4_PRINTF_LL_TEST_RUN MATCHES 0)
        MESSAGE("Width with ${HDF4_PRINTF_LL} failed with result: ${HDF4_PRINTF_LL_TEST_RUN}")
      ENDIF (HDF4_PRINTF_LL_TEST_RUN MATCHES 0)
    ELSE (HDF4_PRINTF_LL_TEST_COMPILE)
      FILE( APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
        "Test H4_PRINTF_LL_WIDTH for ${HDF4_PRINTF_LL} failed with the following output:\n ${OUTPUT}\n")
    ENDIF (HDF4_PRINTF_LL_TEST_COMPILE)
  ENDFOREACH(HDF4_PRINTF_LL)
  IF (PRINT_LL_FOUND)
    MESSAGE(STATUS "Checking for apropriate format for 64 bit long: found ${H4_PRINTF_LL_WIDTH}")
  ELSE (PRINT_LL_FOUND)
    MESSAGE(STATUS "Checking for apropriate format for 64 bit long: not found")
    SET(H4_PRINTF_LL_WIDTH "\"unknown\"" CACHE INTERNAL
      "Width for printf for type `long long' or `__int64', us. `ll")
  ENDIF (PRINT_LL_FOUND)
ENDIF (H4_PRINTF_LL_WIDTH MATCHES "^H4_PRINTF_LL_WIDTH$")

#-----------------------------------------------------------------------------
# Some things that are always true
#-----------------------------------------------------------------------------
SET(H4_CONVERT_DENORMAL_FLOAT 1)

#-----------------------------------------------------------------------------
#  Are we going to use HSIZE_T
#-----------------------------------------------------------------------------
IF (HDF4_ENABLE_HSIZET)
  SET(H4_HAVE_LARGE_HSIZET 1)
ENDIF (HDF4_ENABLE_HSIZET)
IF (CYGWIN)
  SET(H4_HAVE_LSEEK64 0)
ENDIF (CYGWIN)

#-----------------------------------------------------------------------------
# Macro to determine the various conversion capabilities
#-----------------------------------------------------------------------------
MACRO ( H5ConversionTests TEST msg)
  IF("${TEST}" MATCHES "^${TEST}$")
   # MESSAGE(STATUS "===> ${TEST}")
    TRY_RUN(${TEST}_RUN   ${TEST}_COMPILE
      ${HDF4_BINARY_DIR}/CMake
      ${HDF4_SOURCE_DIR}/Resources/ConversionTests.c
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=-D${TEST}_TEST
      OUTPUT_VARIABLE OUTPUT)
    IF ( ${TEST}_COMPILE )
      IF ( ${TEST}_RUN  MATCHES 0)
        SET( ${TEST} 1 CACHE INTERNAL ${msg})
        MESSAGE(STATUS "${msg}... yes")
      ELSE ()
        SET( ${TEST} "" CACHE INTERNAL ${msg})
        MESSAGE(STATUS "${msg}... no")
         FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
            "Test ${TEST} Run failed with the following output and exit code:\n ${OUTPUT}\n")
      ENDIF ()
    ELSE ( ${TEST}_COMPILE )
        SET( ${TEST} "" CACHE INTERNAL ${msg})
        MESSAGE(STATUS "${msg}... no")
        FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
            "Test ${TEST} Compile failed with the following output:\n ${OUTPUT}\n" )
    ENDIF ( ${TEST}_COMPILE)
    
  ENDIF("${TEST}" MATCHES "^${TEST}$")
ENDMACRO ( H5ConversionTests TEST msg)

#-----------------------------------------------------------------------------
# Macro to make some of the conversion tests easier to write/read
#-----------------------------------------------------------------------------
MACRO(H5MiscConversionTest  VAR TEST msg)
	IF ( "${TEST}" MATCHES "^${TEST}$")
		IF ( ${VAR})
		  SET(${TEST} 1 CACHE INTERNAL ${msg} )
		  MESSAGE(STATUS "${msg}... yes")
		else()
			SET(${TEST} "" CACHE INTERNAL ${msg} )
			MESSAGE(STATUS "${msg}... no")
		ENDIF()
	ENDIF()
ENDMACRO(H5MiscConversionTest  VAR TEST msg)

#-----------------------------------------------------------------------------
# Check various conversion capabilities
#-----------------------------------------------------------------------------
H5MiscConversionTest( H4_SIZEOF_LONG_DOUBLE H4_LDOUBLE_TO_INTEGER_ACCURATE "checking IF converting from long double to integers is accurate")
H5ConversionTests( H4_LDOUBLE_TO_INTEGER_WORKS "Checking IF converting from long double to integers works")
H5MiscConversionTest( H4_SIZEOF_LONG_DOUBLE H4_INTEGER_TO_LDOUBLE_ACCURATE "checking IF accurately converting from integers to long double")
H5ConversionTests( H4_ULONG_TO_FLOAT_ACCURATE "Checking IF accurately converting unsigned long to float values")
H5ConversionTests( H4_ULONG_TO_FP_BOTTOM_BIT_ACCURATE "Checking IF accurately converting unsigned long long to floating-point values")
H5ConversionTests( H4_FP_TO_ULLONG_ACCURATE "Checking IF accurately roundup converting floating-point to unsigned long long values" )
H5ConversionTests( H4_FP_TO_ULLONG_RIGHT_MAXIMUM "Checking IF right maximum converting floating-point to unsigned long long values" )
H5ConversionTests( H4_LDOUBLE_TO_UINT_ACCURATE "Checking IF correctly converting long double to unsigned int values")

IF ( H4_ULLONG_TO_FP_CAST_WORKS MATCHES ^H4_ULLONG_TO_FP_CAST_WORKS$)
	SET (H4_ULLONG_TO_FP_CAST_WORKS 1 CACHE INTERNAL "Checking IF compiling unsigned long long to floating-point typecasts work")
	MESSAGE(STATUS "Checking IF compiling unsigned long long to floating-point typecasts work... yes")
ENDIF()

IF ( H4_INTEGER_TO_LDOUBLE_ACCURATE MATCHES ^H4_INTEGER_TO_LDOUBLE_ACCURATE$)
	SET (H4_LLONG_TO_FP_CAST_WORKS 1 CACHE INTERNAL "Checking IF compiling long long to floating-point typecasts work")
	MESSAGE(STATUS "Checking IF compiling long long to floating-point typecasts work... yes")
ENDIF()

H5ConversionTests( H4_ULLONG_TO_LDOUBLE_PRECISION "Checking IF converting unsigned long long to long double with precision")
H5ConversionTests( H4_FP_TO_INTEGER_OVERFLOW_WORKS  "Checking IF overflows normally converting floating-point to integer values")
H5ConversionTests( H4_LDOUBLE_TO_LLONG_ACCURATE "Checking IF correctly converting long double to (unsigned) long long values")
H5ConversionTests( H4_LLONG_TO_LDOUBLE_CORRECT "Checking IF correctly converting (unsigned) long long to long double values")
H5ConversionTests( H4_NO_ALIGNMENT_RESTRICTIONS "Checking IF alignment restrictions are strictly enforced")

#-----------------------------------------------------------------------------
# These tests need to be manually SET for windows since there is currently
# something not quite correct with the actual test implementation. This affects
# the 'dt_arith' test and most likely lots of other code
# ----------------------------------------------------------------------------
IF (WINDOWS)
  SET(H4_FP_TO_ULLONG_RIGHT_MAXIMUM "" CACHE INTERNAL "")
ENDIF()
