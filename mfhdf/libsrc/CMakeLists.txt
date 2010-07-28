cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_LIBSRC C CXX)

#-----------------------------------------------------------------------------
# Generate the netcdf.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
IF (WIN32)
  SET (H4_WINDOWS_INCLUDE_FILE "#include <hdfi.h>")
  SET (NETCDF_EXTERN HDFLIBAPI)
ELSE (WIN32)
  SET (NETCDF_EXTERN extern)
ENDIF (WIN32)

SET (NC_OLD_FILLVALUES 0)
   
STRING (TOLOWER ${CMAKE_SYSTEM_NAME} netcdf_sys)

MESSAGE (STATUS "System Name ${CMAKE_SYSTEM_NAME}")
IF (HDF4_ENABLE_NETCDF)
  ADD_CUSTOM_COMMAND (
      OUTPUT ${HDF4_BINARY_DIR}/netcdf.h
      COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/config/netcdf-${netcdf_sys}.h ${HDF4_BINARY_DIR}/netcdf.h
      COMMENT " Copying netcdf Name netcdf-${netcdf_sys}.h to netcdf.h"
  )
ELSE (HDF4_ENABLE_NETCDF)
  ADD_CUSTOM_COMMAND (
      OUTPUT ${HDF4_BINARY_DIR}/hdf4_netcdf.h
      COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/config/netcdf-${netcdf_sys}.h ${HDF4_BINARY_DIR}/hdf4_netcdf.h
      COMMENT " Copying netcdf Name netcdf-${netcdf_sys}.h to hdf4_netcdf.h"
  )
ENDIF (HDF4_ENABLE_NETCDF)

SET (HDF4_MFHDF_LIBSRC_CSRCS
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/array.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/attr.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/cdf.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/dim.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/file.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/hdfsds.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/iarray.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/error.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/globdef.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfsd.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/nssdc.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/putget.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/putgetg.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/sharray.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/string.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/var.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/xdrposix.c
)

SET (HDF4_MFHDF_LIBSRC_CHDRS
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/alloc.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/error.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/local_nc.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfhdf.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfhdfi.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/hdf2netcdf.h
)

IF (HDF4_ENABLE_NETCDF)
  SET (HDF4_MFHDF_LIBSRC_CHDRS ${HDF4_MFHDF_LIBSRC_CHDRS} ${HDF4_BINARY_DIR}/netcdf.h)
ELSE(HDF4_ENABLE_NETCDF)
  SET (HDF4_MFHDF_LIBSRC_CHDRS ${HDF4_MFHDF_LIBSRC_CHDRS} ${HDF4_BINARY_DIR}/hdf4_netcdf.h)
ENDIF (HDF4_ENABLE_NETCDF)

IF (NOT HDF4_BUILD_XDR_LIB)
  INCLUDE_DIRECTORIES (/usr/include/rpc)
ENDIF (NOT HDF4_BUILD_XDR_LIB)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR}/lib)
INCLUDE_DIRECTORIES (${HDF4_MFHDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDF_PORT_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS -DSWAP)
ENDIF (WIN32)

ADD_LIBRARY (${HDF4_MF_LIB_TARGET} ${LIB_TYPE} ${HDF4_MFHDF_LIBSRC_CSRCS} ${HDF4_MFHDF_LIBSRC_CHDRS})
IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  TARGET_LINK_LIBRARIES (${HDF4_MF_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET})
ENDIF (HDF4_BUILD_XDR_LIB)
IF (BUILD_SHARED_LIBS)
  IF (WIN32)
    ADD_DEFINITIONS (-DHDFAPDLL)
  ENDIF (WIN32)
ENDIF (BUILD_SHARED_LIBS)
TARGET_LINK_LIBRARIES (${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_LIB_TARGET} ${HDF4_MF_LIB_NAME} ${LIB_TYPE})

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${HDF4_MFHDF_LIBSRC_CHDRS}
    DESTINATION
        include
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS 
        ${HDF4_MF_LIB_TARGET}
    EXPORT 
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION lib COMPONENT libraries 
    ARCHIVE DESTINATION lib COMPONENT libraries
    RUNTIME DESTINATION bin COMPONENT libraries
)
