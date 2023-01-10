
#-----------------------------------------------------------------------------
# Options for HDF4 Filters
#-----------------------------------------------------------------------------
option (USE_LIBAEC "Use AEC library as SZip Filter" OFF)
option (USE_LIBAEC_STATIC "Use static AEC library " OFF)
option (JPEG_USE_EXTERNAL "Use External Library Building for JPEG" 0)
option (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 0)
option (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 0)

option (BUILD_JPEG_WITH_FETCHCONTENT "Use FetchContent to use original source files" OFF)
if (BUILD_JPEG_WITH_FETCHCONTENT)
  set (JPEG_USE_EXTERNAL "Use External Library Building for JPEG" 1)
  if (NOT JPEG_USE_LOCALCONTENT)
    set (JPEG_URL ${JPEG_TGZ_ORIGPATH}/${JPEG_TGZ_ORIGNAME})
  else ()
    set (JPEG_URL ${TGZPATH}/${JPEG_TGZ_ORIGNAME})
  endif ()
  if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
    message (VERBOSE "Filter JPEG file is ${JPEG_URL}")
  endif ()
  if (NOT EXISTS "${JPEG_URL}")
    set (HDF4_ENABLE_JPEG_SUPPORT OFF CACHE BOOL "" FORCE)
    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
      message (VERBOSE "Filter JPEG file ${JPEG_URL} not found")
    endif ()
  endif ()
endif ()

include (ExternalProject)
#option (HDF4_ALLOW_EXTERNAL_SUPPORT "Allow External Library Building (NO GIT TGZ)" "NO")
set (HDF4_ALLOW_EXTERNAL_SUPPORT "NO" CACHE STRING "Allow External Library Building (NO GIT TGZ)")
set_property (CACHE HDF4_ALLOW_EXTERNAL_SUPPORT PROPERTY STRINGS NO GIT TGZ)
if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  set (JPEG_USE_EXTERNAL "Use External Library Building for JPEG" 1)
  set (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 1)
  set (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 1)
  if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT")
    set (JPEG_URL ${JPEG_GIT_URL} CACHE STRING "Path to jpeg git repository")
    set (JPEG_BRANCH ${JPEG_GIT_BRANCH})
    set (ZLIB_URL ${ZLIB_GIT_URL} CACHE STRING "Path to zlib git repository")
    set (ZLIB_BRANCH ${ZLIB_GIT_BRANCH})
    set (SZIP_URL ${SZIP_GIT_URL} CACHE STRING "Path to szip git repository")
    set (SZIP_BRANCH ${SZIP_GIT_BRANCH})
  elseif (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    if (NOT TGZPATH)
      set (TGZPATH ${HDF4_SOURCE_DIR})
    endif ()
    if (NOT BUILD_JPEG_WITH_FETCHCONTENT)
      set (JPEG_URL ${TGZPATH}/${JPEG_TGZ_NAME})
    endif ()
    if (NOT EXISTS "${JPEG_URL}")
      set (HDF4_ENABLE_JPEG_SUPPORT OFF CACHE BOOL "" FORCE)
      if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
        message (VERBOSE "Filter JPEG file ${JPEG_URL} not found")
      endif ()
    endif ()
    set (ZLIB_URL ${TGZPATH}/${ZLIB_TGZ_NAME})
    if (NOT EXISTS "${ZLIB_URL}")
      set (HDF4_ENABLE_Z_LIB_SUPPORT OFF CACHE BOOL "" FORCE)
      if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
        message (VERBOSE "Filter ZLIB file ${ZLIB_URL} not found")
      endif ()
    endif ()
    set (SZIP_URL ${TGZPATH}/${SZIP_TGZ_NAME})
    if (USE_LIBAEC)
      set (SZIP_URL ${TGZPATH}/${SZAEC_TGZ_NAME})
    endif ()
    if (NOT EXISTS "${SZIP_URL}")
      set (HDF4_ENABLE_SZIP_SUPPORT OFF CACHE BOOL "" FORCE)
      if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
        message (VERBOSE "Filter SZIP file ${SZIP_URL} not found")
      endif ()
    endif ()
  else ()
    set (JPEG_USE_EXTERNAL 0)
    set (ZLIB_USE_EXTERNAL 0)
    set (SZIP_USE_EXTERNAL 0)
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option for LibJpeg support
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_JPEG_LIB_SUPPORT "Enable libjpeg" ON)
if (HDF4_ENABLE_JPEG_LIB_SUPPORT)
  if (NOT H4_JPEGLIB_HEADER)
    if (NOT JPEG_USE_EXTERNAL)
      find_package (JPEG NAMES ${JPEG_PACKAGE_NAME}${HDF_PACKAGE_EXT} COMPONENTS static shared)
      if (NOT JPEG_FOUND)
        find_package (JPEG) # Legacy find
        if (JPEG_FOUND)
          set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${JPEG_LIBRARIES})
        endif ()
      endif ()
    endif ()
    if (JPEG_FOUND)
      set (H4_HAVE_JPEGLIB_H 1)
      set (H4_HAVE_LIBJPEG 1)
      set (H4_JPEGLIB_HEADER "jpeglib.h")
      set (JPEG_INCLUDE_DIR_GEN ${JPEG_INCLUDE_DIR})
      set (JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
    else ()
      # May need to build JPEG with PIC on x64 machines with gcc
      # Need to use CMAKE_ANSI_CFLAGS define so that compiler test works
      if (BUILD_JPEG_WITH_PIC)
        set (JPEG_CMAKE_C_FLAGS "-fPIC")
      else ()
        if (DEFINED CMAKE_ANSI_CFLAGS)
          set (JPEG_CMAKE_C_FLAGS ${CMAKE_ANSI_CFLAGS})
        else ()
          set (JPEG_CMAKE_C_FLAGS " ")
        endif ()
      endif ()
      if (BUILD_JPEG_WITH_FETCHCONTENT)
        # Only tgz files available
        ORIGINAL_JPEG_LIBRARY ("TGZ" ${JPEG_CMAKE_C_FLAGS})
        set (H4_HAVE_JPEGLIB_H 1)
        set (H4_HAVE_LIBJPEG 1)
        set (H4_JPEGLIB_HEADER "jpeglib.h")
        message (STATUS "JPEGLIB is built from fetch content")
      elseif (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        EXTERNAL_JPEG_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT} ${JPEG_CMAKE_C_FLAGS})
        set (H4_HAVE_JPEGLIB_H 1)
        set (H4_HAVE_LIBJPEG 1)
        set (H4_JPEGLIB_HEADER "jpeglib.h")
        message (STATUS "JPEGLIB is built")
      else ()
        message (FATAL_ERROR " JPEGLib is Required for JPEGLib support in HDF4")
      endif ()
    endif ()
  else ()
    # This project is being called from within another and JPEGLib is already configured
    set (H4_HAVE_JPEGLIB_H 1)
  endif ()
  set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${JPEG_STATIC_LIBRARY})
  INCLUDE_DIRECTORIES (${JPEG_INCLUDE_DIRS})
  message (STATUS "JPEGLIB is ON")
endif ()

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" ON)
if (HDF4_ENABLE_Z_LIB_SUPPORT)
  if (NOT H4_ZLIB_HEADER)
    if (NOT ZLIB_USE_EXTERNAL)
      find_package (ZLIB NAMES ${ZLIB_PACKAGE_NAME}${HDF_PACKAGE_EXT} COMPONENTS static shared)
      if (NOT ZLIB_FOUND)
        find_package (ZLIB) # Legacy find
        if (ZLIB_FOUND)
          set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${ZLIB_LIBRARIES})
        endif ()
      endif ()
    endif ()
    if (ZLIB_FOUND)
      set (H4_HAVE_FILTER_DEFLATE 1)
      set (H4_HAVE_ZLIB_H 1)
      set (H4_HAVE_LIBZ 1)
      set (H4_ZLIB_HEADER "zlib.h")
      set (ZLIB_INCLUDE_DIR_GEN ${ZLIB_INCLUDE_DIR})
      set (ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIR})
    else ()
      if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        EXTERNAL_ZLIB_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT})
        set (H4_HAVE_FILTER_DEFLATE 1)
        set (H4_HAVE_ZLIB_H 1)
        set (H4_HAVE_LIBZ 1)
        if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
          message (VERBOSE "Filter HDF4_ZLIB is built")
        endif ()
      else ()
        message (FATAL_ERROR " ZLib is Required for ZLib support in HDF4")
      endif ()
    endif ()
  else ()
    # This project is being called from within another and ZLib is already configured
    set (H4_HAVE_FILTER_DEFLATE 1)
    set (H4_HAVE_ZLIB_H 1)
    set (H4_HAVE_LIBZ 1)
  endif ()
  set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${ZLIB_STATIC_LIBRARY})
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
    message (VERBOSE "Filter HDF4_ZLIB is ON")
  endif ()
endif ()

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
option (HDF4_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)
set (SZIP_INFO "disabled")
if (HDF4_ENABLE_SZIP_SUPPORT)
  option (HDF4_ENABLE_SZIP_ENCODING "Use SZip Encoding" OFF)
  if (NOT SZIP_USE_EXTERNAL)
    set(SZIP_FOUND FALSE)
    if (USE_LIBAEC)
      set(libaec_USE_STATIC_LIBS ${USE_LIBAEC_STATIC})
      find_package (libaec 1.0.5 CONFIG)
      if (SZIP_FOUND)
        set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_LIBRARIES})
      endif ()
    endif ()

    if (NOT SZIP_FOUND)
      find_package (SZIP NAMES ${SZIP_PACKAGE_NAME}${HDF_PACKAGE_EXT} COMPONENTS static shared)
      if (NOT SZIP_FOUND)
        find_package (SZIP) # Legacy find
        if (SZIP_FOUND)
          set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_LIBRARIES})
        endif ()
      endif ()
    endif ()
  endif ()
  if (SZIP_FOUND)
    set (H4_HAVE_FILTER_SZIP 1)
    set (H4_HAVE_SZLIB_H 1)
    set (H4_HAVE_LIBSZ 1)
    set (SZIP_INCLUDE_DIR_GEN ${SZIP_INCLUDE_DIR})
    set (SZIP_INCLUDE_DIRS ${SZIP_INCLUDE_DIRS} ${SZIP_INCLUDE_DIR})
  else ()
    if (HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "GIT" OR HDF4_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      EXTERNAL_SZIP_LIBRARY (${HDF4_ALLOW_EXTERNAL_SUPPORT} ${HDF4_ENABLE_SZIP_ENCODING})
      set (H4_HAVE_FILTER_SZIP 1)
      set (H4_HAVE_SZLIB_H 1)
      set (H4_HAVE_LIBSZ 1)
      if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
        message (VERBOSE "Filter SZIP is built")
      endif ()
      if (USE_LIBAEC)
        if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
          message (VERBOSE "... with library AEC")
        endif ()
        set (SZIP_PACKAGE_NAME ${LIBAEC_PACKAGE_NAME})
      else ()
        set (SZIP_PACKAGE_NAME ${SZIP_PACKAGE_NAME})
      endif ()
    else ()
      message (FATAL_ERROR "SZIP is Required for SZIP support in HDF4")
    endif ()
  endif ()
  set (LINK_COMP_LIBS ${LINK_COMP_LIBS} ${SZIP_STATIC_LIBRARY})
  INCLUDE_DIRECTORIES (${SZIP_INCLUDE_DIRS})
  if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.15.0")
    message (VERBOSE "Filter SZIP is ON")
  endif ()
  if (HDF4_ENABLE_SZIP_ENCODING)
    set (H4_HAVE_SZIP_ENCODER 1)
    set (SZIP_INFO "enabled with encoder")
  else ()
    set (SZIP_INFO "enabled with decoder only")
  endif ()
endif ()
