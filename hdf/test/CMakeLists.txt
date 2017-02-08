cmake_minimum_required (VERSION 3.2.2)
project (HDF4_HDF_TEST C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
    ${HDF4_HDFSOURCE_DIR}
)
LINK_DIRECTORIES (
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

set (testhdf_SRCS
    ${HDF4_HDF_TEST_SOURCE_DIR}/testhdf.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/an.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/anfile.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/bitio.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/blocks.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/chunks.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/comp.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/conv.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/extelt.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/file.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/file1.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/litend.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/macros.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/man.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/mgr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/nbit.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/rig.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdmms.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdnmms.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/sdstr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/slab.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tattdatainfo.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tbv.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdatainfo.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdfr8.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tdupimgs.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrattr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tmgrcomp.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tree.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tszip.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tusejpegfuncs.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tutils.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvattr.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvnameclass.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvset.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/tvsfpack.c
    ${HDF4_HDF_TEST_SOURCE_DIR}/vers.c
  )

if (WIN32)
  add_definitions (-DDOS_FS)
endif ()

#-- Adding test for testhdf
add_executable (testhdf ${testhdf_SRCS})
TARGET_NAMING (testhdf STATIC)
TARGET_C_PROPERTIES (testhdf STATIC " " " ")
target_link_libraries (testhdf ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
set_target_properties (testhdf PROPERTIES FOLDER test)
if (BUILD_SHARED_LIBS)
  add_executable (testhdf-shared ${testhdf_SRCS})
  TARGET_NAMING (testhdf-shared SHARED)
  TARGET_C_PROPERTIES (testhdf-shared SHARED " " " ")
  target_link_libraries (testhdf-shared ${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  set_target_properties (testhdf-shared PROPERTIES FOLDER test)
endif ()

#-- Adding test for buffer
if (NOT WIN32)
  add_executable (buffer ${HDF4_HDF_TEST_SOURCE_DIR}/buffer.c)
  TARGET_NAMING (buffer STATIC)
  TARGET_C_PROPERTIES (buffer STATIC " " " ")
  target_link_libraries (buffer ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  set_target_properties (buffer PROPERTIES FOLDER test)
  if (BUILD_SHARED_LIBS)
    add_executable (buffer-shared ${HDF4_HDF_TEST_SOURCE_DIR}/buffer.c)
    TARGET_NAMING (buffer-shared SHARED)
    TARGET_C_PROPERTIES (buffer-shared SHARED " " " ")
    target_link_libraries (buffer-shared ${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
    set_target_properties (buffer-shared PROPERTIES FOLDER test)
  endif ()
endif ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

include (CMakeTests.cmake)

