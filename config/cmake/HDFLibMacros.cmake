#-------------------------------------------------------------------------------
macro (EXTERNAL_ZLIB_LIBRARY compress_type)
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (HDF4_ZLIB
        GIT_REPOSITORY ${ZLIB_URL}
        GIT_TAG ${ZLIB_BRANCH}
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter ZLIB file ${ZLIB_URL}")
    FetchContent_Declare (HDF4_ZLIB
        URL ${ZLIB_URL}
        URL_HASH ""
    )
  endif ()
  FetchContent_GetProperties(HDF4_ZLIB)
  if(NOT hdf4_zlib_POPULATED)
    FetchContent_Populate(HDF4_ZLIB)

    # Copy an additional/replacement files into the populated source
    file(COPY ${HDF_RESOURCES_DIR}/ZLIB/CMakeLists.txt DESTINATION ${hdf4_zlib_SOURCE_DIR})

    add_subdirectory(${hdf4_zlib_SOURCE_DIR} ${hdf4_zlib_BINARY_DIR})
  endif()

  add_library(${HDF_PACKAGE_NAMESPACE}zlib-static ALIAS zlib-static)
  set (ZLIB_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}zlib-static")
  set (ZLIB_LIBRARIES ${ZLIB_STATIC_LIBRARY})

  set (ZLIB_INCLUDE_DIR_GEN "${hdf4_zlib_BINARY_DIR}")
  set (ZLIB_INCLUDE_DIR "${hdf4_zlib_SOURCE_DIR}")
  set (ZLIB_FOUND 1)
  set (ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR_GEN} ${ZLIB_INCLUDE_DIR})
  message (STATUS "ZLIB link libs: ${ZLIB_LIBRARIES}")
endmacro ()

#-------------------------------------------------------------------------------
macro (EXTERNAL_JPEG_LIBRARY compress_type)
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (JPEG
        GIT_REPOSITORY ${JPEG_URL}
        GIT_TAG ${JPEG_BRANCH}
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter JPEG file ${JPEG_URL}")
    FetchContent_Declare (JPEG
        URL ${JPEG_URL}
        URL_HASH ""
    )
  endif ()
  FetchContent_GetProperties(JPEG)
  if(NOT jpeg_POPULATED)
    FetchContent_Populate(JPEG)

    # Copy an additional/replacement files into the populated source
    file(COPY ${HDF_RESOURCES_DIR}/JPEG/CMakeLists.txt DESTINATION ${jpeg_SOURCE_DIR})

    add_subdirectory(${jpeg_SOURCE_DIR} ${jpeg_BINARY_DIR})
  endif()

  add_library(${HDF_PACKAGE_NAMESPACE}jpeg-static ALIAS jpeg-static)
  set (JPEG_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}jpeg-static")
  set (JPEG_LIBRARIES ${JPEG_STATIC_LIBRARY})

  set (JPEG_INCLUDE_DIR_GEN "${jpeg_BINARY_DIR}")
  set (JPEG_INCLUDE_DIR "${jpeg_SOURCE_DIR}")
  set (JPEG_FOUND 1)
  set (JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIR_GEN} ${JPEG_INCLUDE_DIR})
  message (STATUS "JPEG link libs: ${JPEG_LIBRARIES}")
endmacro ()

#-------------------------------------------------------------------------------
macro (EXTERNAL_SZIP_LIBRARY compress_type encoding)
  # Only libaec library is usable
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (SZIP
        GIT_REPOSITORY ${SZIP_URL}
        GIT_TAG ${SZIP_BRANCH}
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter SZIP file ${SZIP_URL}")
    FetchContent_Declare (SZIP
        URL ${SZIP_URL}
        URL_HASH ""
    )
  endif ()
  FetchContent_GetProperties(SZIP)
  if(NOT szip_POPULATED)
    FetchContent_Populate(SZIP)

    # Copy an additional/replacement files into the populated source
    file(COPY ${HDF_RESOURCES_DIR}/LIBAEC/CMakeLists.txt DESTINATION ${szip_SOURCE_DIR})

    add_subdirectory(${szip_SOURCE_DIR} ${szip_BINARY_DIR})
  endif()

  add_library (${HDF_PACKAGE_NAMESPACE}szaec-static ALIAS szaec-static)
  add_library (${HDF_PACKAGE_NAMESPACE}aec-static ALIAS aec-static)
  set (SZIP_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}szaec-static;${HDF_PACKAGE_NAMESPACE}aec-static")
  set (SZIP_LIBRARIES ${SZIP_STATIC_LIBRARY})

  set (SZIP_INCLUDE_DIR_GEN "${szip_BINARY_DIR}")
  set (SZIP_INCLUDE_DIR "${szip_SOURCE_DIR}/include")
  set (SZIP_FOUND 1)
  set (SZIP_INCLUDE_DIRS ${SZIP_INCLUDE_DIR_GEN} ${SZIP_INCLUDE_DIR})
  message (STATUS "SZIP link libs: ${SZIP_LIBRARIES}")
endmacro ()
