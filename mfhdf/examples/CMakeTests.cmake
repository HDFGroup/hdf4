
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
set (last_test "MFHDF_EXAMPLES-clearall-objects")

foreach (example ${examples})
  ADD_TEST (NAME mftest_${example} COMMAND $<TARGET_FILE:mf_${example}>)
  if (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (mftest_${example} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (mftest_${example} PROPERTIES LABELS ${PROJECT_NAME})
  endif (NOT "${last_test}" STREQUAL "")
  set (last_test "mftest_${example}")
endforeach (example ${examples})
