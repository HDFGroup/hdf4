
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

file (MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/testdir)

#-----------------------------------------------------------------------------
# test programs
#-----------------------------------------------------------------------------
if (HDF4_ENABLE_NETCDF)
  #-- Adding test for ftest
  add_executable (ftest ${HDF4_BINARY_DIR}/ftest.f ${HDF4_HDF_TESTSOURCE_DIR}/forsupff.f ${HDF4_BINARY_DIR}/netcdf.inc)
  target_include_directories (ftest
      PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
  )
  target_compile_options(ftest
      PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
  )
  if(MSVC)
    set_property(TARGET ftest PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
  endif()
  set_target_properties (ftest PROPERTIES LINKER_LANGUAGE Fortran)
  if (NOT BUILD_SHARED_LIBS)
    target_link_libraries (ftest PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET})
  else ()
    target_link_libraries (ftest PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET})
  endif ()

  #-- Copy all the dat files from the test directory into the source directory
  set (HDF4_REFERENCE_TEST_FILES
    test_nc.cdl
    test_nc.nc
  )
  foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
    HDFTEST_COPY_FILE("${HDF4_MFHDF_FORTRAN_SOURCE_DIR}/${h4_file}" "${PROJECT_BINARY_DIR}/${h4_file}" "netcdf_files")
  endforeach ()
  add_custom_target(netcdf_files ALL COMMENT "Copying files needed by netcdf tests" DEPENDS ${netcdf_files_list})

  add_test (NAME ftest COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:ftest>)
endif ()

#-- Adding test for f_hdftest
add_executable (f_hdftest hdftest.f)
target_include_directories (f_hdftest
    PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
)
target_compile_options(f_hdftest
    PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
)
if(MSVC)
  set_property(TARGET f_hdftest PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
endif()
set_target_properties (f_hdftest PROPERTIES LINKER_LANGUAGE Fortran)
if (NOT BUILD_SHARED_LIBS)
  target_link_libraries (f_hdftest PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET})
else ()
  target_link_libraries (f_hdftest PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
endif ()

#-- Adding test for f_hdftest1
add_executable (f_hdftest1 hdftest1.f)
target_include_directories (f_hdftest1
    PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
)
target_compile_options(f_hdftest1
    PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
)
if(MSVC)
  set_property(TARGET f_hdftest1 PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
endif()
set_target_properties (f_hdftest1 PROPERTIES LINKER_LANGUAGE Fortran)
if (NOT BUILD_SHARED_LIBS)
  target_link_libraries (f_hdftest1 PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET})
else ()
  target_link_libraries (f_hdftest1 PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET})
endif ()

#-- Adding test for f_tszip
add_executable (f_tszip tszip.f)
target_include_directories (f_tszip
    PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_MFHDF_FORTRAN_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_MFHDFSOURCE_DIR};${HDF4_HDF_TESTSOURCE_DIR}"
)
target_compile_options(f_tszip
    PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
)
if(MSVC)
  set_property(TARGET f_tszip PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
endif()
set_target_properties (f_tszip PROPERTIES LINKER_LANGUAGE Fortran)
if (NOT BUILD_SHARED_LIBS)
  target_link_libraries (f_tszip PRIVATE ${HDF4_MF_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_TARGET})
else ()
  target_link_libraries (f_tszip PRIVATE ${HDF4_MF_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FORTRAN_LIBSH_TARGET})
endif ()

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################
# Remove any output file left over from previous test run
add_test (
    NAME MFHDF_FORTRAN-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
        cchunk_gz.hdf
        cchunk_nb.hdf
        cchunk_no.hdf
        cchunk_rl.hdf
        cchunk_sk.hdf
        chunk_gz.hdf
        chunk_nb.hdf
        chunk_no.hdf
        chunk_rl.hdf
        chunk_sk.hdf
        comp_gz.hdf
        comp_no.hdf
        comp_rl.hdf
        comp_sk.hdf
        comp_szip.hdf
        copy.nc
        test.nc
        test1.hdf
        test2.hdf
        test_empty.hdf
        test_var.hdf
        test_file.hdf
        test_file01.hdf
        test_file02.hdf
        test_file03.hdf
        test_file04.hdf
        test_file05.hdf
        test_file06.hdf
        test_file07.hdf
        test_file08.hdf
        test_file09.hdf
        test_file10.hdf
        test_file11.hdf
        test_file12.hdf
        test_file13.hdf
        test_file14.hdf
        test_file15.hdf
        test_file16.hdf
        test_file17.hdf
        test_file18.hdf
        test_file19.hdf
        test_file20.hdf
        test_file21.hdf
        test_file22.hdf
        test_file23.hdf
        test_file24.hdf
        test_file25.hdf
        test_file26.hdf
        test_file27.hdf
        test_file28.hdf
        test_file29.hdf
        test_file30.hdf
        test_file31.hdf
        test_file32.hdf
        test_file33.hdf
        test_file34.hdf
        test_file35.hdf
)
if (HDF4_ENABLE_NETCDF)
  set_tests_properties (MFHDF_FORTRAN-clearall-objects PROPERTIES DEPENDS ftest LABELS ${PROJECT_NAME})
endif ()

add_test (NAME f_hdftest COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:f_hdftest>)
set (passRegex "Total errors : [ ]+0")
set_property (TEST f_hdftest PROPERTY PASS_REGULAR_EXPRESSION "${passRegex}")
set_tests_properties (f_hdftest PROPERTIES DEPENDS MFHDF_FORTRAN-clearall-objects LABELS ${PROJECT_NAME})

add_test (NAME f_hdftest1 COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:f_hdftest1>)
set (failRegex "failed" "errors")
set_property (TEST f_hdftest1 PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")
set_tests_properties (f_hdftest1 PROPERTIES DEPENDS f_hdftest LABELS ${PROJECT_NAME})

add_test (NAME f_tszip COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:f_tszip>)
set (failRegex "failed" "errors")
set_property (TEST f_tszip PROPERTY FAIL_REGULAR_EXPRESSION "${failRegex}")
set_tests_properties (f_tszip PROPERTIES DEPENDS f_hdftest1 LABELS ${PROJECT_NAME})
