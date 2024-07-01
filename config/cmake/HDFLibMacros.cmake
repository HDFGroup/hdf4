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
  set (H4_ZLIB_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}zlib-static")
  set (H4_ZLIB_LIBRARIES ${H4_ZLIB_STATIC_LIBRARY})

  set (H4_ZLIB_INCLUDE_DIR_GEN "${hdf4_zlib_BINARY_DIR}")
  set (H4_ZLIB_INCLUDE_DIR "${hdf4_zlib_SOURCE_DIR}")
  set (H4_ZLIB_FOUND 1)
  set (H4_ZLIB_INCLUDE_DIRS ${H4_ZLIB_INCLUDE_DIR_GEN} ${H4_ZLIB_INCLUDE_DIR})
  message (STATUS "ZLIB link libs: ${H4_ZLIB_LIBRARIES}")
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
  set (H4_JPEG_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}jpeg-static")
  set (H4_JPEG_LIBRARIES ${H4_JPEG_STATIC_LIBRARY})

  set (H4_JPEG_INCLUDE_DIR_GEN "${jpeg_BINARY_DIR}")
  set (H4_JPEG_INCLUDE_DIR "${jpeg_SOURCE_DIR}")
  set (H4_JPEG_FOUND 1)
  set (H4_JPEG_INCLUDE_DIRS ${H4_JPEG_INCLUDE_DIR_GEN} ${H4_JPEG_INCLUDE_DIR})
  message (STATUS "JPEG link libs: ${H4_JPEG_LIBRARIES}")
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
  set (H4_SZIP_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}szaec-static;${HDF_PACKAGE_NAMESPACE}aec-static")
  set (H4_SZIP_LIBRARIES ${H4_SZIP_STATIC_LIBRARY})

  set (H4_SZIP_INCLUDE_DIR_GEN "${szip_BINARY_DIR}")
  set (H4_SZIP_INCLUDE_DIR "${szip_SOURCE_DIR}/include")
  set (H4_SZIP_FOUND 1)
  set (H4_SZIP_INCLUDE_DIRS ${H4_SZIP_INCLUDE_DIR_GEN} ${H4_SZIP_INCLUDE_DIR})
  message (STATUS "SZIP link libs: ${H4_SZIP_LIBRARIES}")
endmacro ()
