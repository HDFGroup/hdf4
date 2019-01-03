cmake_minimum_required (VERSION 3.10)
project (HDF4_MFHDF_TEST)

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
target_include_directories(hdftest
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
)
TARGET_C_PROPERTIES (hdftest STATIC )
target_link_libraries (hdftest PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (hdftest PROPERTIES
    FOLDER test
    COMPILE_DEFINITIONS "HDF"
)
if (BUILD_SHARED_LIBS)
  add_executable (hdftest-shared ${hdftest_SRCS})
  target_include_directories(hdftest-shared
      PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
  )
  TARGET_C_PROPERTIES (hdftest-shared SHARED )
  target_link_libraries (hdftest-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (hdftest-shared PROPERTIES
      FOLDER test
      COMPILE_DEFINITIONS "HDF"
  )
endif ()

#-- Adding test for cdftest
add_executable (cdftest ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
target_include_directories(cdftest
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
)
TARGET_C_PROPERTIES (cdftest STATIC )
target_link_libraries (cdftest PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (cdftest PROPERTIES
    FOLDER test
    COMPILE_DEFINITIONS "HDF"
)
if (BUILD_SHARED_LIBS)
  add_executable (cdftest-shared ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
  target_include_directories(cdftest-shared
      PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
  )
  TARGET_C_PROPERTIES (cdftest-shared SHARED )
  target_link_libraries (cdftest-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (cdftest-shared PROPERTIES
      FOLDER test
      COMPILE_DEFINITIONS "HDF"
  )
endif ()

#-- Adding test for hdfnctest
set (hdfnctest_SRCS
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/hdfnctest.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tunlim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncunlim.c
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tncvargetfill.c
)

add_executable (hdfnctest ${hdfnctest_SRCS})
target_include_directories(hdfnctest
    PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
)
TARGET_C_PROPERTIES (hdfnctest STATIC )
target_link_libraries (hdfnctest PRIVATE ${HDF4_MF_LIB_TARGET})
set_target_properties (hdfnctest PROPERTIES
    FOLDER test
    COMPILE_DEFINITIONS "HDF"
)
if (BUILD_SHARED_LIBS)
  add_executable (hdfnctest-shared ${hdfnctest_SRCS})
  target_include_directories(hdfnctest-shared
      PRIVATE "${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR}"
  )
  TARGET_C_PROPERTIES (hdfnctest-shared SHARED )
  target_link_libraries (hdfnctest-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
  set_target_properties (hdfnctest-shared PROPERTIES
      FOLDER test
      COMPILE_DEFINITIONS "HDF"
  )
endif ()

include (CMakeTests.cmake)
