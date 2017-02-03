cmake_minimum_required (VERSION 3.1)
PROJECT (HDF4_HDF_FORTRAN C CXX Fortran)

#-----------------------------------------------------------------------------
# Make sure generated files and modules are picked up correctly
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
)

set (HDF4_HDF_SRC_CSTUB_FSRCS
    ${HDF4_HDFSOURCE_DIR}/dfanf.c
    ${HDF4_HDFSOURCE_DIR}/dff.c
    ${HDF4_HDFSOURCE_DIR}/dfpf.c
    ${HDF4_HDFSOURCE_DIR}/dfr8f.c
    ${HDF4_HDFSOURCE_DIR}/dfsdf.c
    ${HDF4_HDFSOURCE_DIR}/dfutilf.c
    ${HDF4_HDFSOURCE_DIR}/df24f.c
    ${HDF4_HDFSOURCE_DIR}/dfufp2if.c
    ${HDF4_HDFSOURCE_DIR}/herrf.c
    ${HDF4_HDFSOURCE_DIR}/hfilef.c
    ${HDF4_HDFSOURCE_DIR}/mfanf.c
    ${HDF4_HDFSOURCE_DIR}/mfgrf.c
    ${HDF4_HDFSOURCE_DIR}/vattrf.c
    ${HDF4_HDFSOURCE_DIR}/vgf.c
)
set_source_files_properties (${HDF4_HDF_SRC_CSTUB_FSRCS} PROPERTIES LANGUAGE C)

set (HDF4_HDF_SRC_FHDRS
    ${HDF4_HDFSOURCE_DIR}/dffunc.inc
    ${HDF4_HDFSOURCE_DIR}/hdf.inc
)

set (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
add_library (${HDF4_SRC_FCSTUB_LIB_TARGET} STATIC ${HDF4_HDF_SRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h ${HDF4_BINARY_DIR}/h4config.h)
TARGET_C_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET} STATIC " " " ")
target_link_libraries (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FCSTUB_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_NAME} STATIC)
set_target_properties (${HDF4_SRC_FCSTUB_LIB_TARGET} PROPERTIES
    FOLDER libraries/fortran
    LINKER_LANGUAGE C
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
if (WIN32)
  set_property (TARGET ${HDF4_SRC_FCSTUB_LIB_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
endif ()
set (install_targets ${HDF4_SRC_FCSTUB_LIB_TARGET})

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_SRC_FCSTUB_LIBSH_TARGET} SHARED ${HDF4_HDF_SRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDFSOURCE_DIR}/hproto_fortran.h ${HDF4_BINARY_DIR}/h4config.h)
  TARGET_C_PROPERTIES (${HDF4_SRC_FCSTUB_LIBSH_TARGET} SHARED " " " ")
  target_link_libraries (${HDF4_SRC_FCSTUB_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FCSTUB_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_SRC_FCSTUB_LIBSH_TARGET} ${HDF4_SRC_FCSTUB_LIB_NAME} SHARED)
  set_target_properties (${HDF4_SRC_FCSTUB_LIBSH_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE C
      COMPILE_DEFINITIONS "H4_BUILT_AS_DYNAMIC_LIB"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
  )
  if (WIN32)
    set_property (TARGET ${HDF4_SRC_FCSTUB_LIBSH_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
  endif ()
  set (install_targets ${install_targets} ${HDF4_SRC_FCSTUB_LIBSH_TARGET})
endif ()

set (HDF4_F_FORTRAN_SRCS
    ${HDF4_HDFSOURCE_DIR}/df24ff.f
    ${HDF4_HDFSOURCE_DIR}/dfanff.f
    ${HDF4_HDFSOURCE_DIR}/dfpff.f
    ${HDF4_HDFSOURCE_DIR}/dfr8ff.f
    ${HDF4_HDFSOURCE_DIR}/dfsdff.f
    ${HDF4_HDFSOURCE_DIR}/dfufp2iff.f
    ${HDF4_HDFSOURCE_DIR}/dfff.f
    ${HDF4_HDFSOURCE_DIR}/hfileff.f
    ${HDF4_HDFSOURCE_DIR}/mfgrff.f
    ${HDF4_HDFSOURCE_DIR}/vattrff.f
    ${HDF4_HDFSOURCE_DIR}/vgff.f
)
set_source_files_properties (${HDF4_F_FORTRAN_SRCS} PROPERTIES LANGUAGE Fortran)

#-----------------------------------------------------------------------------
add_library (${HDF4_SRC_FORTRAN_LIB_TARGET} STATIC ${HDF4_F_FORTRAN_SRCS})
set (SHARED_LINK_FLAGS " ")
TARGET_FORTRAN_PROPERTIES (${HDF4_SRC_FORTRAN_LIB_TARGET} STATIC " " " ")
target_link_libraries (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${LINK_LIBS})
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FORTRAN_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_NAME} STATIC)
set_target_properties (${HDF4_SRC_FORTRAN_LIB_TARGET} PROPERTIES
    FOLDER libraries/fortran
    LINKER_LANGUAGE Fortran
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
    Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
)
if (WIN32)
  set_property (TARGET ${HDF4_SRC_FORTRAN_LIB_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
endif ()
set (install_targets ${install_targets} ${HDF4_SRC_FORTRAN_LIB_TARGET})

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_SRC_FORTRAN_LIBSH_TARGET} SHARED ${HDF4_F_FORTRAN_SRCS})
  set (SHARED_LINK_FLAGS " ")
  if (WIN32 AND MSVC)
    set (SHARED_LINK_FLAGS "/DLL /DEF:${HDF4_HDF_FORTRAN_SOURCE_DIR}/hdf_fortrandll.def")
  endif ()
  TARGET_FORTRAN_PROPERTIES (${HDF4_SRC_FORTRAN_LIBSH_TARGET} SHARED " " ${SHARED_LINK_FLAGS})
  target_link_libraries (${HDF4_SRC_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FCSTUB_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FORTRAN_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_SRC_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FORTRAN_LIB_NAME} SHARED)
  set_target_properties (${HDF4_SRC_FORTRAN_LIBSH_TARGET} PROPERTIES
      FOLDER libraries/fortran
      LINKER_LANGUAGE Fortran
      COMPILE_DEFINITIONS "H4_BUILT_AS_DYNAMIC_LIB"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
      Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
  )
  if (WIN32)
    set_property (TARGET ${HDF4_SRC_FORTRAN_LIBSH_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
  endif ()
  set (install_targets ${install_targets} ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
endif ()


if (BUILD_TESTING)
  include (CMakeTests.cmake)
endif ()

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${HDF4_HDF_SRC_FHDRS}
    DESTINATION
        ${HDF4_INSTALL_INCLUDE_DIR}
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (HDF4_EXPORTED_TARGETS)
  if (BUILD_SHARED_LIBS)
    INSTALL_TARGET_PDB (${HDF4_SRC_FCSTUB_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)
    #INSTALL_TARGET_PDB (${HDF4_SRC_FORTRAN_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)
  endif ()
  INSTALL_TARGET_PDB (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_INSTALL_BIN_DIR} fortlibraries)

  INSTALL (
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
endif ()
