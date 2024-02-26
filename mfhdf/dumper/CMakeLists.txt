cmake_minimum_required (VERSION 3.18)
project (HDF4_MFHDF_DUMPER C)

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

add_executable (hdp ${hdp_SRCS})
target_include_directories(hdp PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_COMP_INCLUDE_DIRECTORIES};${HDF4_BINARY_DIR}")
if (HDF4_BUILD_STATIC_TOOLS)
  TARGET_C_PROPERTIES (hdp STATIC)
  target_link_libraries (hdp PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (hdp SHARED)
  target_link_libraries (hdp PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (hdp PROPERTIES COMPILE_DEFINITIONS "HDF")
set_global_variable (HDF4_UTILS_TO_EXPORT "${HDF4_UTILS_TO_EXPORT};hdp")

set (H4_DEP_EXECUTABLES hdp)

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
