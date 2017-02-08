cmake_minimum_required (VERSION 3.2.2)
project (HDF4_MFHDF_TEST)

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

set (hdftest_SRCS
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdftest.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tchunk.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcomp.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tcoordvar.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/temptySDSs.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tattributes.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tfile.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tmixed_apis.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tnetcdf.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/trank0.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsd.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tsdsprops.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tszip.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tattdatainfo.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdatainfo.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tdatasizes.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/texternal.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tutils.c
)

#-- Adding test for hdftest
add_executable (hdftest ${hdftest_SRCS})
TARGET_NAMING (hdftest STATIC)
TARGET_C_PROPERTIES (hdftest STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
else ()
  target_link_libraries (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
set_target_properties (hdftest PROPERTIES FOLDER test)
if (BUILD_SHARED_LIBS)
  add_executable (hdftest-shared ${hdftest_SRCS})
  TARGET_NAMING (hdftest-shared SHARED)
  TARGET_C_PROPERTIES (hdftest-shared SHARED " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (hdftest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${HDF4_MF_XDR_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  else ()
    target_link_libraries (hdftest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  endif ()
  set_target_properties (hdftest-shared PROPERTIES FOLDER test)
endif ()

#-- Adding test for cdftest
add_executable (cdftest ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
TARGET_NAMING (cdftest STATIC)
TARGET_C_PROPERTIES (cdftest STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
else ()
  target_link_libraries (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
set_target_properties (cdftest PROPERTIES FOLDER test)
if (BUILD_SHARED_LIBS)
  add_executable (cdftest-shared ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
  TARGET_NAMING (cdftest-shared SHARED)
  TARGET_C_PROPERTIES (cdftest-shared SHARED " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (cdftest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${HDF4_MF_XDR_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  else ()
    target_link_libraries (cdftest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  endif ()
  set_target_properties (cdftest-shared PROPERTIES FOLDER test)
endif ()

#-- Adding test for hdfnctest
set (hdfnctest_SRCS
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdfnctest.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tunlim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncunlim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncvargetfill.c
)

add_executable (hdfnctest ${hdfnctest_SRCS})
TARGET_NAMING (hdfnctest STATIC)
TARGET_C_PROPERTIES (hdfnctest STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
else ()
  target_link_libraries (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()
set_target_properties (hdfnctest PROPERTIES FOLDER test)
if (BUILD_SHARED_LIBS)
  add_executable (hdfnctest-shared ${hdfnctest_SRCS})
  TARGET_NAMING (hdfnctest-shared SHARED)
  TARGET_C_PROPERTIES (hdfnctest-shared SHARED " " " ")
  if (HDF4_BUILD_XDR_LIB)
    target_link_libraries (hdfnctest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${HDF4_MF_XDR_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  else ()
    target_link_libraries (hdfnctest-shared ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET} ${LINK_SHARED_LIBS})
  endif ()
  set_target_properties (hdfnctest-shared PROPERTIES FOLDER test)
endif ()

include (CMakeTests.cmake)
