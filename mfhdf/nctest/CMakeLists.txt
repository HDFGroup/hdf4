cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_NCTEST)

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
target_include_directories(nctest PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};$<$<BOOL:${HDF4_BUILD_XDR_LIB}>:${HDF4_MFHDF_XDR_DIR}>")
if (NOT BUILD_SHARED_LIBS)
  TARGET_C_PROPERTIES (nctest STATIC)
  target_link_libraries (nctest PRIVATE ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (nctest SHARED)
  target_link_libraries (nctest PRIVATE ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (nctest PROPERTIES COMPILE_DEFINITIONS "HDF")

include (CMakeTests.cmake)
