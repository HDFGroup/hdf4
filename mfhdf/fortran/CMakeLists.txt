cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_FORTRAN C CXX Fortran)

INCLUDE_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_MFHDF_FORTRAN_BINARY_DIR}
    ${HDF4_HDFSOURCE_DIR}
    ${HDF4_MFHDFSOURCE_DIR}
    ${HDF4_HDF_TESTSOURCE_DIR}
)

add_definitions (-DHDF)

if (HDF4_ENABLE_NETCDF)
  configure_file (${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/ftest.f.in ${HDF4_BINARY_DIR}/ftest.f)
  configure_file (${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/jackets.c.in ${HDF4_BINARY_DIR}/jackets.c)
endif (HDF4_ENABLE_NETCDF)
configure_file (${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/netcdf.inc.in ${HDF4_BINARY_DIR}/netcdf.inc)
set (HDF4_MFHDF_FORTRAN_XFHDRS ${HDF4_BINARY_DIR}/netcdf.inc ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mffunc.inc)

if (HDF4_ENABLE_NETCDF)
  set (HDF4_MFHDF_LIBSRC_CSTUB_FSRCS ${HDF4_BINARY_DIR}/jackets.c ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mfsdf.c)
else (HDF4_ENABLE_NETCDF)
  set (HDF4_MFHDF_LIBSRC_CSTUB_FSRCS ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mfsdf.c)
endif (HDF4_ENABLE_NETCDF)

set (HDF4_MFHDF_LIBSRC_CSTUB_FHDRS 
    ${HDF4_MFHDFSOURCE_DIR}/hdf2netcdf.h 
    ${HDF4_MFHDFSOURCE_DIR}/mfhdf.h 
    ${HDF4_MFHDFSOURCE_DIR}/mfhdfi.h
)
if (HDF4_ENABLE_NETCDF)
#  set (HDF4_MFHDF_LIBSRC_CSTUB_FSRCS ${HDF4_MFHDF_LIBSRC_CSTUB_FSRCS} ${HDF4_BINARY_DIR}/jackets.c)
  set (HDF4_MFHDF_LIBSRC_CSTUB_FHDRS ${HDF4_MFHDF_LIBSRC_CSTUB_FHDRS} ${HDF4_BINARY_DIR}/netcdf.h)
else (HDF4_ENABLE_NETCDF)
  set (HDF4_MFHDF_LIBSRC_CSTUB_FHDRS ${HDF4_MFHDF_LIBSRC_CSTUB_FHDRS} ${HDF4_BINARY_DIR}/hdf4_netcdf.h)
endif (HDF4_ENABLE_NETCDF)

set (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
add_library (${HDF4_MF_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_MFHDF_LIBSRC_CSTUB_FSRCS} ${HDF4_MFHDF_LIBSRC_CSTUB_FHDRS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h)
set_target_properties (${HDF4_MF_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
if (WIN32)
  add_definitions (-DDOS_FS)
endif (WIN32)
if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
  target_link_libraries (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET})
endif (HDF4_BUILD_XDR_LIB)
TARGET_C_PROPERTIES (${HDF4_MF_FCSTUB_LIB_TARGET} " " " ")
target_link_libraries (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FCSTUB_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_FCSTUB_LIB_NAME} ${LIB_TYPE})

set (HDF4_MF_FORTRAN_SRCS 
    ${HDF4_MFHDF_FORTRAN_DIR}/mfsdff.f
)
#-----------------------------------------------------------------------------
add_library (${HDF4_MF_FORTRAN_LIB_TARGET} ${LIB_TYPE} ${HDF4_MF_FORTRAN_SRCS})
set (SHARED_LINK_FLAGS " ")
if (WIN32)
  if (${LIB_TYPE} MATCHES "SHARED")
    if (MSVC)
      set (SHARED_LINK_FLAGS "/DLL /DEF:${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mfhdf_fortrandll.def")
    endif (MSVC)
  endif (${LIB_TYPE} MATCHES "SHARED")
endif (WIN32)
TARGET_FORTRAN_PROPERTIES (${HDF4_MF_FORTRAN_LIB_TARGET} " " ${SHARED_LINK_FLAGS})
set_target_properties (${HDF4_MF_FORTRAN_LIB_TARGET} PROPERTIES LINKER_LANGUAGE Fortran)
target_link_libraries (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_FCSTUB_LIB_TARGET} ${LINK_LIBS})
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FORTRAN_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_FORTRAN_LIB_NAME} ${LIB_TYPE})

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
install (
    FILES
        ${HDF4_MFHDF_FORTRAN_XFHDRS}
    DESTINATION
        ${HDF4_INSTALL_INCLUDE_DIR}
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_INSTALL_LIB_DIR} fortlibraries)
  INSTALL_TARGET_PDB (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_INSTALL_LIB_DIR} fortlibraries)
endif (BUILD_SHARED_LIBS)
  
install (
    TARGETS 
        ${HDF4_MF_FCSTUB_LIB_TARGET}
        ${HDF4_MF_FORTRAN_LIB_TARGET}
    EXPORT 
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries 
    ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries
    RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT fortlibraries
)
