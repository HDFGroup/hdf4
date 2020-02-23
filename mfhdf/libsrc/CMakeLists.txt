cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_LIBSRC C)

#-----------------------------------------------------------------------------
# Generate the netcdf.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
if (WIN32)
  set (H4_WINDOWS_INCLUDE_FILE "#include <hdfi.h>")
  set (NETCDF_EXTERN HDFLIBAPI)
else ()
  set (NETCDF_EXTERN extern)
endif ()

set (NC_OLD_FILLVALUES 0)

set (HDF4_MFHDF_LIBSRC_CSRCS
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/array.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/attr.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/cdf.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/dim.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/file.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/hdfsds.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/iarray.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/error.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/globdef.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfdatainfo.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfsd.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/nssdc.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/putget.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/putgetg.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/sharray.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/string.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/var.c
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/xdrposix.c
)

set (HDF4_MFHDF_LIBSRC_CHDRS
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/alloc.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/error.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/local_nc.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfhdf.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfhdfi.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/mfdatainfo.h
    ${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/hdf2netcdf.h
)

if (HDF4_ENABLE_NETCDF)
  configure_file (${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/netcdf.h.in ${HDF4_BINARY_DIR}/netcdf.h)
  set (HDF4_MFHDF_LIBSRC_XCHDRS ${HDF4_BINARY_DIR}/netcdf.h)
else ()
  configure_file (${HDF4_MFHDF_LIBSRC_SOURCE_DIR}/netcdf.h.in ${HDF4_BINARY_DIR}/hdf4_netcdf.h)
  set (HDF4_MFHDF_LIBSRC_XCHDRS ${HDF4_BINARY_DIR}/hdf4_netcdf.h)
endif ()

if (NOT ONLY_SHARED_LIBS)
  add_library (${HDF4_MF_LIB_TARGET} STATIC ${HDF4_MFHDF_LIBSRC_CSRCS} ${HDF4_MFHDF_LIBSRC_CHDRS})
  target_include_directories (${HDF4_MF_LIB_TARGET}
      PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (${HDF4_MF_LIB_TARGET} PUBLIC ${HDF4_MF_XDR_LIB_TARGET})
  endif ()
  TARGET_C_PROPERTIES (${HDF4_MF_LIB_TARGET} STATIC)
  target_link_libraries (${HDF4_MF_LIB_TARGET} PUBLIC ${HDF4_SRC_LIB_TARGET})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_LIB_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_LIB_TARGET} ${HDF4_MF_LIB_NAME} STATIC)
  set_target_properties (${HDF4_MF_LIB_TARGET} PROPERTIES
      FOLDER libraries
      COMPILE_DEFINITIONS "HDF"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  if (WIN32)
    set_property (TARGET ${HDF4_MF_LIB_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
  endif ()
  set (install_targets ${HDF4_MF_LIB_TARGET})
endif ()

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_MF_LIBSH_TARGET} SHARED ${HDF4_MFHDF_LIBSRC_CSRCS} ${HDF4_MFHDF_LIBSRC_CHDRS})
  target_include_directories (${HDF4_MF_LIBSH_TARGET}
      PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (${HDF4_MF_LIBSH_TARGET} PUBLIC ${HDF4_MF_XDR_LIBSH_TARGET})
  endif ()
  TARGET_C_PROPERTIES (${HDF4_MF_LIBSH_TARGET} SHARED)
  target_link_libraries (${HDF4_MF_LIBSH_TARGET} PUBLIC ${HDF4_SRC_LIBSH_TARGET})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_LIBSH_TARGET} ${HDF4_MF_LIB_NAME} SHARED)
  set_target_properties (${HDF4_MF_LIBSH_TARGET} PROPERTIES
      FOLDER libraries
      COMPILE_DEFINITIONS "HDF;H4_BUILT_AS_DYNAMIC_LIB"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
  )
  if (WIN32)
    set_property (TARGET ${HDF4_MF_LIBSH_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
  endif ()
  set (install_targets ${install_targets} ${HDF4_MF_LIBSH_TARGET})
endif ()

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
install (
    FILES
        ${HDF4_MFHDF_LIBSRC_CHDRS} ${HDF4_MFHDF_LIBSRC_XCHDRS}
    DESTINATION
        ${HDF4_INSTALL_INCLUDE_DIR}
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${HDF4_MF_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} libraries)
endif ()
if (NOT ONLY_SHARED_LIBS)
  INSTALL_TARGET_PDB (${HDF4_MF_LIB_TARGET} ${HDF4_INSTALL_LIB_DIR} libraries)
endif ()


install (
    TARGETS
        ${install_targets}
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
    ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
    RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT libraries
    FRAMEWORK DESTINATION ${HDF4_INSTALL_FWRK_DIR} COMPONENT libraries
    INCLUDES DESTINATION include
)

#-----------------------------------------------------------------------------
# Create pkgconfig files
#-----------------------------------------------------------------------------
set (_PKG_CONFIG_PREFIX ${CMAKE_INSTALL_PREFIX})
set (_PKG_CONFIG_EXEC_PREFIX \${prefix})
set (_PKG_CONFIG_LIBDIR \${exec_prefix}/lib)
set (_PKG_CONFIG_INCLUDEDIR \${prefix}/include)
set (_PKG_CONFIG_LIBNAME "${HDF4_SRC_LIB_CORENAME}")
set (_PKG_CONFIG_VERSION "${HDF4_PACKAGE_VERSION}")

foreach (libs ${LINK_LIBS} ${LINK_COMP_LIBS})
  set (_PKG_CONFIG_LIBS_PRIVATE "${_PKG_CONFIG_LIBS_PRIVATE} -l${libs}")
endforeach ()

set (_PKG_CONFIG_LIBS "${_PKG_CONFIG_LIBS} -l${HDF4_SRC_LIB_CORENAME} -l${HDF4_MF_LIB_CORENAME}")
if (BUILD_SHARED_LIBS)
  set (_PKG_CONFIG_SH_LIBS "${_PKG_CONFIG_SH_LIBS} -l${HDF4_SRC_LIB_CORENAME} -l${HDF4_MF_LIB_CORENAME}")
endif ()

set (_PKG_CONFIG_REQUIRES)
set (_PKG_CONFIG_REQUIRES_PRIVATE)

configure_file (
    ${HDF_RESOURCES_DIR}/libhdf4.pc.in
    ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_SRC_LIB_CORENAME}-${HDF4_PACKAGE_VERSION}.pc
    @ONLY
)
install (
    FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_SRC_LIB_CORENAME}-${HDF4_PACKAGE_VERSION}.pc
    DESTINATION ${HDF4_INSTALL_LIB_DIR}/pkgconfig
    COMPONENT libraries
)

if (NOT WIN32)
  set (_PKG_CONFIG_COMPILER ${CMAKE_C_COMPILER})
  configure_file (
      ${HDF_RESOURCES_DIR}/libh4cc.in
      ${HDF4_BINARY_DIR}/CMakeFiles/h4cc
      @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/h4cc
      DESTINATION ${HDF4_INSTALL_BIN_DIR}
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      COMPONENT libraries
  )
endif ()
