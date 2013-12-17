
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
ADD_TEST (
    NAME NC_TEST-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        test.nc
        test2.nc
)

#-- Adding test for nctest
ADD_TEST (NAME NC_TEST-nctest COMMAND $<TARGET_FILE:nctest>)
SET_TESTS_PROPERTIES (NC_TEST-nctest PROPERTIES DEPENDS NC_TEST-clearall-objects LABELS ${PROJECT_NAME})
