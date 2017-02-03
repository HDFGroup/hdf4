cmake_minimum_required (VERSION 3.1)
PROJECT (HDF4_HDF_SRC C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_HDF_SOURCE_DIR}
)
LINK_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

set (HDF4_HDF_SRC_CSRCS
    ${HDF4_HDF_SRC_SOURCE_DIR}/atom.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/bitvect.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cdeflate.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnbit.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnone.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/crle.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cskphuff.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cszip.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/df24.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfan.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfconv.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgroup.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfimcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfjpeg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfknat.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkswap.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfrle.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsd.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfstubs.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2i.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfunjpeg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfutil.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dynarray.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/glist.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbitio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hblocks.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbuffer.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hchunks.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcompri.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdatainfo.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdfalloc.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/herr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hextelt.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfile.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfiledd.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hkit.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/linklist.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mcache.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfan.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mstdio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/tbbt.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vconv.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vhi.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vparse.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vrw.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vsfld.c
)

set (HDF4_HDF_SRC_CHDRS
    ${HDF4_HDF_SRC_SOURCE_DIR}/atom.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/bitvect.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cdeflate.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnbit.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnone.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/crle.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cskphuff.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cszip.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/df.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfan.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfrig.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsd.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfstubs.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2i.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dynarray.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/glist.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/H4api_adpt.h
    #${HDF4_BINARY_DIR}/h4config.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbitio.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hchunks.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcomp.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcompi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hconv.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdatainfo.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdf.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdfi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/herr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfile.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hkit.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hlimits.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hntdefs.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hproto.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hqueue.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/htags.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/linklist.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mcache.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfan.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mstdio.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/tbbt.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vg.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgint.h
)

add_library (${HDF4_SRC_LIB_TARGET} STATIC ${HDF4_HDF_SRC_CSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_BINARY_DIR}/h4config.h)
TARGET_C_PROPERTIES (${HDF4_SRC_LIB_TARGET} STATIC " " " ")
target_link_libraries (${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_LIB_TARGET} ${HDF4_SRC_LIB_NAME} STATIC)
set_target_properties (${HDF4_SRC_LIB_TARGET} PROPERTIES
    FOLDER libraries
    INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
if (WIN32)
  set_property (TARGET ${HDF4_SRC_LIB_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
endif (WIN32)
set (install_targets ${HDF4_SRC_LIB_TARGET})

if (BUILD_SHARED_LIBS)
  add_library (${HDF4_SRC_LIBSH_TARGET} SHARED ${HDF4_HDF_SRC_CSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_BINARY_DIR}/h4config.h)
  TARGET_C_PROPERTIES (${HDF4_SRC_LIBSH_TARGET} SHARED " " " ")
  target_link_libraries (${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  set_global_variable (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_LIBSH_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_SRC_LIBSH_TARGET} ${HDF4_SRC_LIB_NAME} SHARED)
  set_target_properties (${HDF4_SRC_LIBSH_TARGET} PROPERTIES
      FOLDER libraries
      COMPILE_DEFINITIONS "H4_BUILT_AS_DYNAMIC_LIB"
      INTERFACE_INCLUDE_DIRECTORIES "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
      INTERFACE_COMPILE_DEFINITIONS H4_BUILT_AS_DYNAMIC_LIB=1
  )
  if (WIN32)
    set_property (TARGET ${HDF4_SRC_LIBSH_TARGET} APPEND PROPERTY COMPILE_DEFINITIONS "DOS_FS")
  endif (WIN32)
  set (install_targets ${install_targets} ${HDF4_SRC_LIBSH_TARGET})
endif (BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${HDF4_HDF_SRC_CHDRS}
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
    INSTALL_TARGET_PDB (${HDF4_SRC_LIBSH_TARGET} ${HDF4_INSTALL_BIN_DIR} libraries)
  endif (BUILD_SHARED_LIBS)
  INSTALL_TARGET_PDB (${HDF4_SRC_LIB_TARGET} ${HDF4_INSTALL_BIN_DIR} libraries)

  INSTALL (
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
endif (HDF4_EXPORTED_TARGETS)

