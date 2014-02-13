
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
ADD_TEST (
    NAME MFHDF_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        SDS.hdf
        SDSchunked.hdf
        SDScompressed.hdf
        SDSUNLIMITED.hdf
        SLABS.hdf
        ExternalSDS
)
SET_TESTS_PROPERTIES (MFHDF_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
SET (last_test "MFHDF_EXAMPLES-clearall-objects")

FOREACH (example ${examples})
  ADD_TEST (NAME mftest_${example} COMMAND $<TARGET_FILE:mf_${example}>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (mftest_${example} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (mftest_${example} PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "mftest_${example}")
ENDFOREACH (example ${examples})
