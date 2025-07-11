cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_FORTRAN C Fortran)

if (WIN32)
  set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) NAME")
  set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) NAME")
endif ()
message (VERBOSE "Fortran compiler ID is ${CMAKE_Fortran_COMPILER_ID}")
#include (${HDF_RESOURCES_DIR}/HDFCompilerFortranFlags.cmake)
include (${HDF_RESOURCES_DIR}/HDF4UseFortran.cmake)

if (CMAKE_Fortran_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_Fortran_COMPILER_VERSION VERSION_LESS 10.0)
  set (CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -fallow-argument-mismatch")
  set (CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -fallow-argument-mismatch")
endif ()

set (HDF4_MFHDF_FORTRAN_XFHDRS ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mffunc.inc)

set (HDF4_MFHDF_SRC_CSTUB_FSRCS ${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mfsdf.c)

set (HDF4_MFHDF_SRC_CSTUB_FHDRS
    ${HDF4_MFHDFSOURCE_DIR}/mfhdf.h
    # nc_priv.h may not be necessary...
    ${HDF4_MFHDFSOURCE_DIR}/nc_priv.h
)
set_source_files_properties (${HDF4_MFHDF_SRC_CSTUB_FSRCS} PROPERTIES LANGUAGE C)

set (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
if (BUILD_STATIC_LIBS)
  add_library (${HDF4_MF_FCSTUB_LIB_TARGET} STATIC ${HDF4_MFHDF_SRC_CSTUB_FSRCS} ${HDF4_MFHDF_SRC_CSTUB_FHDRS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h)
  target_include_directories(${HDF4_MF_FCSTUB_LIB_TARGET}
      PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_BINARY_DIR}"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  target_compile_options(${HDF4_MF_FCSTUB_LIB_TARGET} PRIVATE "${HDF4_CMAKE_C_FLAGS}")
  target_compile_definitions(${HDF4_MF_FCSTUB_LIB_TARGET}
      PUBLIC
          ${HDF_EXTRA_C_FLAGS}
          ${HDF_EXTRA_FLAGS}
  )
  TARGET_C_PROPERTIES (${HDF4_MF_FCSTUB_LIB_TARGET} STATIC)
  target_link_libraries (${HDF4_MF_FCSTUB_LIB_TARGET} PUBLIC ${HDF4_MF_LIB_TARGET})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FCSTUB_LIB_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_MF_FCSTUB_LIB_NAME} STATIC 0)
  set_target_properties (${HDF4_MF_FCSTUB_LIB_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE C
      COMPILE_DEFINITIONS "HDF"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  set (install_targets ${HDF4_MF_FCSTUB_LIB_TARGET})
endif ()

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_MF_FCSTUB_LIBSH_TARGET} SHARED ${HDF4_MFHDF_SRC_CSTUB_FSRCS} ${HDF4_MFHDF_SRC_CSTUB_FHDRS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h)
  target_include_directories(${HDF4_MF_FCSTUB_LIBSH_TARGET}
      PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_BINARY_DIR}"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  target_compile_options(${HDF4_MF_FCSTUB_LIBSH_TARGET} PRIVATE "${HDF4_CMAKE_C_FLAGS}")
  target_compile_definitions(${HDF4_MF_FCSTUB_LIBSH_TARGET}
      PUBLIC
          ${HDF_EXTRA_C_FLAGS}
          ${HDF_EXTRA_FLAGS}
  )
  TARGET_C_PROPERTIES (${HDF4_MF_FCSTUB_LIBSH_TARGET} SHARED)
  target_link_libraries (${HDF4_MF_FCSTUB_LIBSH_TARGET} PUBLIC ${HDF4_MF_LIBSH_TARGET})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FCSTUB_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_FCSTUB_LIBSH_TARGET} ${HDF4_MF_FCSTUB_LIB_NAME} SHARED "MF_F")
  set_target_properties (${HDF4_MF_FCSTUB_LIBSH_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE C
      COMPILE_DEFINITIONS "HDF;H4_BUILT_AS_DYNAMIC_LIB"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
  )
  set (install_targets ${install_targets} ${HDF4_MF_FCSTUB_LIBSH_TARGET})
endif ()

set (HDF4_MF_FORTRAN_SRCS
    ${HDF4_MFHDF_FORTRAN_DIR}/mfsdff.f
)
set_source_files_properties (${HDF4_MF_FORTRAN_SRCS} PROPERTIES LANGUAGE Fortran)
#-----------------------------------------------------------------------------
if (BUILD_STATIC_LIBS)
  add_library (${HDF4_MF_FORTRAN_LIB_TARGET} STATIC ${HDF4_MF_FORTRAN_SRCS})
  target_include_directories (${HDF4_MF_FORTRAN_LIB_TARGET}
      PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  target_compile_definitions (${HDF4_MF_FORTRAN_LIB_TARGET}
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  target_link_libraries (${HDF4_MF_FORTRAN_LIB_TARGET} PUBLIC ${HDF4_MF_FCSTUB_LIB_TARGET})
  set_target_properties (${HDF4_MF_FORTRAN_LIB_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (WIN32)
    set_property (TARGET ${HDF4_MF_FORTRAN_LIB_TARGET} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif ()
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FORTRAN_LIB_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_MF_FORTRAN_LIB_NAME} STATIC 0)
  set (install_targets ${install_targets} ${HDF4_MF_FORTRAN_LIB_TARGET})
endif ()

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_MF_FORTRAN_LIBSH_TARGET} SHARED ${HDF4_MF_FORTRAN_SRCS})
  target_include_directories (${HDF4_MF_FORTRAN_LIBSH_TARGET}
      PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
      INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
  )
  target_compile_definitions (${HDF4_MF_FORTRAN_LIBSH_TARGET}
      PUBLIC "H4_BUILT_AS_DYNAMIC_LIB"
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if (WIN32 AND MSVC)
    set_property (TARGET ${HDF4_MF_FORTRAN_LIBSH_TARGET} PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS} /DLL /DEF:${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/mfhdf_fortrandll.def")
  endif ()
  target_link_libraries (${HDF4_MF_FORTRAN_LIBSH_TARGET} PUBLIC ${HDF4_MF_FCSTUB_LIBSH_TARGET})
  set_target_properties (${HDF4_MF_FORTRAN_LIBSH_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE Fortran
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
  )
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FORTRAN_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_MF_FORTRAN_LIB_NAME} SHARED "MF_F")
  set (install_targets ${install_targets} ${HDF4_MF_FORTRAN_LIBSH_TARGET})
endif ()

if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()

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
  INSTALL_TARGET_PDB (${HDF4_MF_FCSTUB_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)
  #INSTALL_TARGET_PDB (${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)
endif (BUILD_SHARED_LIBS)
INSTALL_TARGET_PDB (${HDF4_MF_FCSTUB_LIB_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)

install (
    TARGETS
        ${install_targets}
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries
    ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT fortlibraries
    RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT fortlibraries
    FRAMEWORK DESTINATION ${HDF4_INSTALL_FWRK_DIR} COMPONENT fortlibraries
    INCLUDES DESTINATION include
)

#-----------------------------------------------------------------------------
# Create pkgconfig files
#-----------------------------------------------------------------------------
set (_PKG_CONFIG_PREFIX ${CMAKE_INSTALL_PREFIX})
set (_PKG_CONFIG_EXEC_PREFIX \${prefix})
set (_PKG_CONFIG_LIBDIR \${exec_prefix}/lib)
set (_PKG_CONFIG_INCLUDEDIR \${prefix}/include)
set (_PKG_CONFIG_LIBNAME "${HDF4_MF_FORTRAN_LIB_CORENAME}")
set (_PKG_CONFIG_VERSION "${HDF4_PACKAGE_VERSION}")
set (PKG_CONFIG_LIBNAME "${HDF4_MF_FORTRAN_LIB_CORENAME}")
if (${HDF_CFG_NAME} MATCHES "Debug")
  set (PKG_CONFIG_LIBNAME "${PKG_CONFIG_LIBNAME}${CMAKE_DEBUG_POSTFIX}")
endif ()

foreach (libs ${LINK_LIBS} ${LINK_COMP_LIBS})
  set (_PKG_CONFIG_LIBS_PRIVATE "${_PKG_CONFIG_LIBS_PRIVATE} -l${libs}")
endforeach ()

set (_PKG_CONFIG_LIBS "${_PKG_CONFIG_LIBS} -l${PKG_CONFIG_LIBNAME}")
if (BUILD_SHARED_LIBS)
  set (_PKG_CONFIG_SH_LIBS "${_PKG_CONFIG_SH_LIBS} -l${PKG_CONFIG_LIBNAME}")
endif ()

set (_PKG_CONFIG_REQUIRES "${HDF4_MF_LIB_CORENAME}-${HDF4_PACKAGE_VERSION}")
set (_PKG_CONFIG_REQUIRES_PRIVATE "${HDF4_MF_LIB_CORENAME}-${HDF4_PACKAGE_VERSION}")

configure_file (
    ${HDF_RESOURCES_DIR}/libhdf4.pc.in
    ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_MF_FORTRAN_LIB_CORENAME}.pc
    @ONLY
)
install (
    FILES ${HDF4_BINARY_DIR}/CMakeFiles/${HDF4_MF_FORTRAN_LIB_CORENAME}.pc
    DESTINATION ${HDF4_INSTALL_LIB_DIR}/pkgconfig
    COMPONENT fortlibraries
)

if (NOT WIN32)
  set (_PKG_CONFIG_COMPILER ${CMAKE_Fortran_COMPILER})
  configure_file (
      ${HDF_RESOURCES_DIR}/libh4cc.in
      ${HDF4_BINARY_DIR}/CMakeFiles/h4fc
      @ONLY
  )
  install (
      FILES ${HDF4_BINARY_DIR}/CMakeFiles/h4fc
      DESTINATION ${HDF4_INSTALL_BIN_DIR}
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      COMPONENT fortlibraries
  )
endif ()
