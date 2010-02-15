PROJECT ( HDF4_MFHDF_LIBSRC)

# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all static libraries."
  )

#-----------------------------------------------------------------------------
# Generate the netcdf.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
IF(WIN32)
  SET(H4_WINDOWS_INCLUDE_FILE "#include <hdfi.h>")
  SET(NETCDF_EXTERN HDFLIBAPI)
ELSE(WIN32)
  SET(NETCDF_EXTERN extern)
ENDIF(WIN32)

SET(NC_OLD_FILLVALUES 0)
   
string(TOLOWER ${CMAKE_SYSTEM_NAME} netcdf_sys)

MESSAGE(STATUS "System Name ${CMAKE_SYSTEM_NAME}")
ADD_CUSTOM_COMMAND(
   OUTPUT ${HDF4_BINARY_DIR}/netcdf.h
   COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/config/netcdf-${netcdf_sys}.h ${HDF4_BINARY_DIR}/netcdf.h
   COMMENT " Copying netcdf Name netcdf-${netcdf_sys}.h to netcdf.h")

SET (HDF4_MFHDF_LIBSRC_CSRCS
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/array.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/attr.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/cdf.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/dim.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/error.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/file.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/globdef.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/hdfsds.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/iarray.c
#    ${HDF4_MFHDF_FORTRAN_DIR}/jackets.c
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
    ${HDF4_BINARY_DIR}/netcdf.h
)


IF (NOT HDF4_BUILD_XDR_LIB)
 INCLUDE_DIRECTORIES( /usr/include/rpc )
ENDIF (NOT HDF4_BUILD_XDR_LIB)

INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR}/lib )
INCLUDE_DIRECTORIES( ${HDF4_MFHDF_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${HDF4_MFHDF_PORT_DIR} )

ADD_DEFINITIONS(-DHDF)

IF (HDF4_BUILD_XDR_LIB)
  IF(WIN32)
    add_definitions(-DNO_SYS_XDR_INC)
  ENDIF(WIN32)
  INCLUDE_DIRECTORIES( ${HDF4_MFHDF_XDR_DIR} )
ENDIF ( HDF4_BUILD_XDR_LIB )

IF (BUILD_SHARED_LIBS)
  IF(WIN32)
    add_definitions(-D_MFHDFLIB_ -D_HDFDLL_ -DHDFAPDLL -D_HDFLIB_)
  ENDIF(WIN32)
  ADD_LIBRARY(${HDF4_MF_LIB_NAME} SHARED ${HDF4_MFHDF_LIBSRC_CSRCS} ${HDF4_MFHDF_LIBSRC_CHDRS} )
  # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
  IF(MSVC)
    SET_TARGET_PROPERTIES(${HDF4_MF_LIB_NAME} PROPERTIES COMPILER_FLAGS "${SHARED_C_FLAGS_RELEASE}")
  ENDIF(MSVC)
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES(${HDF4_MF_LIB_NAME} ${HDF4_MF_XDR_LIB_NAME})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(${HDF4_MF_LIB_NAME} ${HDF4_SRC_LIB_NAME})
  SET_TARGET_PROPERTIES(${HDF4_MF_LIB_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ENDIF (BUILD_SHARED_LIBS)

ADD_LIBRARY(${HDF4_MF_LIB_NAME}-static STATIC  ${HDF4_MFHDF_LIBSRC_CSRCS} ${HDF4_MFHDF_LIBSRC_CHDRS})
SET_TARGET_PROPERTIES(${HDF4_MF_LIB_NAME}-static PROPERTIES OUTPUT_NAME "${HDF4_MF_LIB_NAME}")
SET_TARGET_PROPERTIES(${HDF4_MF_LIB_NAME}-static PROPERTIES PREFIX "lib")

# Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
SET_TARGET_PROPERTIES(${HDF4_MF_LIB_NAME}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
INSTALL(
  TARGETS 
    ${HDF4_MF_LIB_NAME}-static
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

IF (BUILD_SHARED_LIBS)
INSTALL(
  TARGETS 
    ${HDF4_MF_LIB_NAME}
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
ENDIF (BUILD_SHARED_LIBS)
