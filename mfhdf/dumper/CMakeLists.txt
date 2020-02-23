cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_DUMPER)

set (hdp_SRCS
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_dump.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_gr.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_list.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_rig.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_sds.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_util.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vd.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/hdp_vg.c
    ${HDF4_MFHDF_DUMPER_SOURCE_DIR}/show.c
)

if (NOT ONLY_SHARED_LIBS)
  add_executable (hdp ${hdp_SRCS})
  target_include_directories(hdp PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (hdp STATIC)
  target_link_libraries (hdp PRIVATE ${HDF4_MF_LIB_TARGET})
  set_target_properties (hdp PROPERTIES COMPILE_DEFINITIONS "HDF")
  if (WIN32)
    set_target_properties (hdp PROPERTIES LINK_FLAGS "/STACK:10000000")
  endif ()
  set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdp")

  set (H4_DEP_EXECUTABLES hdp)
endif ()

if (BUILD_SHARED_LIBS)
  add_executable (hdp-shared ${hdp_SRCS})
  target_include_directories(hdp-shared PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
  TARGET_C_PROPERTIES (hdp-shared SHARED)
  target_link_libraries (hdp-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (hdp-shared PROPERTIES COMPILE_DEFINITIONS "HDF")
  if (WIN32)
    set_target_properties (hdp-shared PROPERTIES LINK_FLAGS "/STACK:10000000")
  endif ()
  set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdp-shared")

  set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdp-shared)
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
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------

foreach (exec ${H4_DEP_EXECUTABLES})
  INSTALL_PROGRAM_PDB (${exec} ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)
endforeach ()

install (
    TARGETS
        ${H4_DEP_EXECUTABLES}
    EXPORT
        ${HDF4_EXPORTED_TARGETS}
    RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
)
