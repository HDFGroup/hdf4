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
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (nctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
else ()
  target_link_libraries (nctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
TARGET_NAMING (nctest STATIC)

set (HDF4_NC_TEST_FILES
    test_unlim.cdl
    test_unlim.nc
)

foreach (h4_file ${HDF4_NC_TEST_FILES})
  set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
  #message (STATUS " Copying ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
  add_custom_command (
      TARGET     nctest
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_NCTEST_SOURCE_DIR}/${h4_file} ${dest}
  )

endforeach ()

include (CMakeTests.cmake)
