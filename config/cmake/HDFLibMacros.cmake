#-------------------------------------------------------------------------------
macro (EXTERNAL_ZLIB_LIBRARY compress_type)
  if (HDF4_USE_ZLIB_NG)
    set (zlib_folder "ZLIBNG")
  else ()
    set (zlib_folder "ZLIB")
  endif ()
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (HDF4_ZLIB
        GIT_REPOSITORY ${ZLIB_URL}
        GIT_TAG ${ZLIB_BRANCH}
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/${zlib_folder}/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter ZLIB file ${ZLIB_URL}")
    FetchContent_Declare (HDF4_ZLIB
        URL ${ZLIB_URL}
        URL_HASH ""
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/${zlib_folder}/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  endif ()

  FetchContent_MakeAvailable(HDF4_ZLIB)

  add_library(${HDF_PACKAGE_NAMESPACE}zlib-static ALIAS zlib-static)
  set (H4_ZLIB_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}zlib-static")
  set (H4_ZLIB_LIBRARIES ${H4_ZLIB_STATIC_LIBRARY})
  if (HDF4_USE_ZLIB_NG)
    set (H4_ZLIB_HEADER "zlib-ng.h")
  else ()
    set (H4_ZLIB_HEADER "zlib.h")
  endif ()

  set (H4_ZLIB_INCLUDE_DIR_GEN "${hdf4_zlib_BINARY_DIR}")
  set (H4_ZLIB_INCLUDE_DIR "${hdf4_zlib_SOURCE_DIR}")
  set (H4_ZLIB_FOUND 1)
  set (H4_ZLIB_INCLUDE_DIRS ${H4_ZLIB_INCLUDE_DIR_GEN} ${H4_ZLIB_INCLUDE_DIR})
  message (VERBOSE "ZLIB link libs: ${H4_ZLIB_LIBRARIES}")
endmacro ()

#-------------------------------------------------------------------------------
macro (EXTERNAL_JPEG_LIBRARY compress_type)
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (JPEG
        GIT_REPOSITORY ${JPEG_URL}
        GIT_TAG ${JPEG_BRANCH}
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/JPEG/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter JPEG file ${JPEG_URL}")
    FetchContent_Declare (JPEG
        URL ${JPEG_URL}
        URL_HASH ""
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/JPEG/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  endif ()
  FetchContent_MakeAvailable(JPEG)

  add_library(${HDF_PACKAGE_NAMESPACE}jpeg-static ALIAS jpeg-static)
  set (H4_JPEG_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}jpeg-static")
  set (H4_JPEG_LIBRARIES ${H4_JPEG_STATIC_LIBRARY})

  set (H4_JPEG_INCLUDE_DIR_GEN "${jpeg_BINARY_DIR}")
  set (H4_JPEG_INCLUDE_DIR "${jpeg_SOURCE_DIR}")
  set (H4_JPEG_FOUND 1)
  set (H4_JPEG_INCLUDE_DIRS ${H4_JPEG_INCLUDE_DIR_GEN} ${H4_JPEG_INCLUDE_DIR})
  message (VERBOSE "JPEG link libs: ${H4_JPEG_LIBRARIES}")
endmacro ()

#-------------------------------------------------------------------------------
macro (EXTERNAL_SZIP_LIBRARY compress_type encoding)
  # Only libaec library is usable
  if (${compress_type} MATCHES "GIT")
    FetchContent_Declare (SZIP
        GIT_REPOSITORY ${SZIP_URL}
        GIT_TAG ${SZIP_BRANCH}
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/LIBAEC/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  elseif (${compress_type} MATCHES "TGZ")
    message (VERBOSE "Filter SZIP file ${SZIP_URL}")
    FetchContent_Declare (SZIP
        URL ${SZIP_URL}
        URL_HASH ""
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
            ${HDF_RESOURCES_DIR}/LIBAEC/CMakeLists.txt
            <SOURCE_DIR>/CMakeLists.txt
    )
  endif ()
  FetchContent_MakeAvailable(SZIP)

  add_library (${HDF_PACKAGE_NAMESPACE}szaec-static ALIAS szaec-static)
  add_library (${HDF_PACKAGE_NAMESPACE}aec-static ALIAS aec-static)
  set (H4_SZIP_STATIC_LIBRARY "${HDF_PACKAGE_NAMESPACE}szaec-static;${HDF_PACKAGE_NAMESPACE}aec-static")
  set (H4_SZIP_LIBRARIES ${H4_SZIP_STATIC_LIBRARY})

  set (H4_SZIP_INCLUDE_DIR_GEN "${szip_BINARY_DIR}")
  set (H4_SZIP_INCLUDE_DIR "${szip_SOURCE_DIR}/include")
  set (H4_SZIP_FOUND 1)
  set (H4_SZIP_INCLUDE_DIRS ${H4_SZIP_INCLUDE_DIR_GEN} ${H4_SZIP_INCLUDE_DIR})
  message (VERBOSE "SZIP link libs: ${H4_SZIP_LIBRARIES}")
endmacro ()
