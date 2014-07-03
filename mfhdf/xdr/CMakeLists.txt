cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_XDR C CXX)

set (HDF4_MFHDF_XDR_SRCS
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdr.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrfloat.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrstdio.c
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdrarray.c
)

set (HDF4_MFHDF_XDR_HDRS
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/xdr.h
    ${HDF4_MFHDF_XDR_SOURCE_DIR}/types.h
)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})

add_definitions (-DHDF)

if (WIN32)
  add_definitions (-DDOS_FS -DNO_SYS_XDR_INC)
endif (WIN32)

if (MSVC OR MINGW)
  add_library (${HDF4_MF_XDR_LIB_TARGET} ${LIB_TYPE} ${HDF4_MFHDF_XDR_SRCS} ${HDF4_MFHDF_XDR_HDRS})
  target_link_libraries (${HDF4_MF_XDR_LIB_TARGET} "ws2_32.lib")
else (MSVC OR MINGW)
  add_library (${HDF4_MF_XDR_LIB_TARGET} ${LIB_TYPE} ${HDF4_MFHDF_XDR_SRCS} ${HDF4_MFHDF_XDR_HDRS} )
endif (MSVC OR MINGW)
TARGET_C_PROPERTIES (${HDF4_MF_XDR_LIB_TARGET} " " " ")
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_XDR_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_XDR_LIB_TARGET} ${HDF4_MF_XDR_LIB_NAME} ${LIB_TYPE})

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${HDF4_MF_XDR_LIB_TARGET} ${HDF4_INSTALL_LIB_DIR} libraries)
endif (BUILD_SHARED_LIBS)
  
INSTALL (
    TARGETS 
        ${HDF4_MF_XDR_LIB_TARGET}
    EXPORT 
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
    ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
    RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT libraries
)
