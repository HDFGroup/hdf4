
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
ADD_TEST (
    NAME MFHDF_FORTRAN_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        SDS.hdf
        SDSchunked.hdf
        SDScompressed.hdf
        SDSUNLIMITED.hdf
        SLABS.hdf
)
SET_TESTS_PROPERTIES (MFHDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
SET (last_test "MFHDF_FORTRAN_EXAMPLES-clearall-objects")

FOREACH (example ${examples})
  ADD_TEST (NAME f_exmftest_${example} COMMAND $<TARGET_FILE:f_exmf_${example}>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_exmftest_${example} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_exmftest_${example} PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "f_exmftest_${example}")
ENDFOREACH (example ${examples})

FOREACH (example ${skip_examples})
  ADD_TEST (NAME f_exmftest_${example} COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_exmftest_${example}")
ENDFOREACH (example ${skip_examples})
