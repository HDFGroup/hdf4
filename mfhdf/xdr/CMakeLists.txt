PROJECT ( HDF4_MFHDF_XDR)

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

SET (HDF4_MFHDF_XDR_SRCS
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdr.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrfloat.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrstdio.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrarray.c
)

SET (HDF4_MFHDF_XDR_HDRS
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdr.h
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/types.h
)

INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${HDF4_MFHDF_XDR_DIR} )

ADD_DEFINITIONS(-DHDF)

IF(WIN32)
  add_definitions(-DNO_SYS_XDR_INC)
ENDIF(WIN32)

IF (BUILD_SHARED_LIBS)
  ADD_LIBRARY(${HDF4_MF_XDR_LIB_NAME} SHARED ${HDF4_MFHDF_XDR_SRCS} ${HDF4_MFHDF_XDR_HDRS} )
  # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
  IF(MSVC)
    SET_TARGET_PROPERTIES(${HDF4_MF_XDR_LIB_NAME} PROPERTIES COMPILER_FLAGS "${SHARED_C_FLAGS_RELEASE}")
    TARGET_LINK_LIBRARIES(${HDF4_MF_XDR_LIB_NAME} "ws2_32.lib")
  ENDIF(MSVC)
  SET_TARGET_PROPERTIES(${HDF4_MF_XDR_LIB_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ENDIF (BUILD_SHARED_LIBS)

ADD_LIBRARY(${HDF4_MF_XDR_LIB_NAME}-static STATIC  ${HDF4_MFHDF_XDR_SRCS} ${HDF4_MFHDF_XDR_HDRS})
SET_TARGET_PROPERTIES(${HDF4_MF_XDR_LIB_NAME}-static PROPERTIES OUTPUT_NAME "${HDF4_MF_XDR_LIB_NAME}")
SET_TARGET_PROPERTIES(${HDF4_MF_XDR_LIB_NAME}-static PROPERTIES PREFIX "lib")

# Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
SET_TARGET_PROPERTIES(${HDF4_MF_XDR_LIB_NAME}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
INSTALL(
  TARGETS 
    ${HDF4_MF_XDR_LIB_NAME}-static
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

IF (BUILD_SHARED_LIBS)
INSTALL(
  TARGETS 
    ${HDF4_MF_XDR_LIB_NAME}
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
ENDIF (BUILD_SHARED_LIBS)
