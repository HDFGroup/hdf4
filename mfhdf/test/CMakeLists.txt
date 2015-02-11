cmake_minimum_required (VERSION 3.1.0)
project (HDF4_MFHDF_TEST)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

add_definitions (-DHDF)

if (WIN32)
  add_definitions (-DDOS_FS)
endif (WIN32)

if (HDF4_BUILD_XDR_LIB)
  if (WIN32)
    add_definitions (-DNO_SYS_XDR_INC)
  endif (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
endif (HDF4_BUILD_XDR_LIB)

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
    ${HDF4_MFHDF_TEST_SOURCE_DIR}/tutils.c
)

#-- Adding test for hdftest
add_executable (hdftest ${hdftest_SRCS})
TARGET_NAMING (hdftest STATIC)
TARGET_C_PROPERTIES (hdftest STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_XDR_LIB_TARGET} ${LINK_LIBS})
else (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE_TEST_FILES
    sds_szipped.dat
    smallslice.0000.nc
    test1.nc
)

foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
   set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
   add_custom_command (
     TARGET     hdftest
     POST_BUILD
     COMMAND    ${CMAKE_COMMAND}
     ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
   )
endforeach (h4_file ${HDF4_REFERENCE_TEST_FILES})

#-- Adding test for cdftest
add_executable (cdftest ${HDF4_MFHDF_TEST_SOURCE_DIR}/cdftest.c)
TARGET_NAMING (cdftest STATIC)
TARGET_C_PROPERTIES (cdftest STATIC " " " ")
if (HDF4_BUILD_XDR_LIB)
  target_link_libraries (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else (HDF4_BUILD_XDR_LIB)
  target_link_libraries (cdftest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE2_TEST_FILES
    testout.sav
)

foreach (h4_file ${HDF4_REFERENCE2_TEST_FILES})
   set (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
   add_custom_command (
     TARGET     cdftest
     POST_BUILD
     COMMAND    ${CMAKE_COMMAND}
     ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
   )
endforeach (h4_file ${HDF4_REFERENCE2_TEST_FILES})

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
  target_link_libraries (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
else (HDF4_BUILD_XDR_LIB)
  target_link_libraries (hdfnctest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_XDR_LIB)

include (CMakeTests.cmake)
