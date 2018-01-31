cmake_minimum_required (VERSION 3.2.2)
project (HDF4_MFHDF_NCTEST)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

add_definitions (-DHDF)

if (WIN32)
  add_definitions (-DDOS_FS)
endif ()

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif ()
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif ()

set (nctest_SRCS
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/add.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/atttests.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/cdftests.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/dimtests.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/driver.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/emalloc.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/error.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/misctest.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/rec.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/slabs.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/val.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/varget_unlim.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/varget.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/vargetg.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/varput.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/varputg.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/vardef.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/vartests.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/vputget.c
    ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/vputgetg.c
)

add_executable (nctest ${nctest_SRCS})
TARGET_C_PROPERTIES (nctest STATIC " " " ")
target_link_libraries (nctest ${HDF4_MF_LIB_TARGET})

include (CMakeTests.cmake)
