
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

set (HDF4_NC_TEST_FILES
    test_unlim.cdl
    test_unlim.nc
)

foreach (h4_file ${HDF4_NC_TEST_FILES})
  HDFTEST_COPY_FILE("${HDF4_MFHDF_NCTEST_SOURCE_DIR}/${h4_file}" "${PROJECT_BINARY_DIR}/${h4_file}" "nc_test_files")
endforeach ()
add_custom_target(nc_test_files ALL COMMENT "Copying files needed by nc tests" DEPENDS ${nc_test_files_list})

# Remove any output file left over from previous test run
add_test (
    NAME NC_TEST-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
        test.nc
        test2.nc
)

#-- Adding test for nctest
add_test (NAME NC_TEST-nctest COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:nctest>)
set_tests_properties (NC_TEST-nctest PROPERTIES DEPENDS NC_TEST-clearall-objects LABELS ${PROJECT_NAME})
