
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
    NAME MFHDF_FORTRAN_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove
        SDS.hdf
        SDSchunked.hdf
        SDScompressed.hdf
        SDSUNLIMITED.hdf
        SLABS.hdf
)
set_tests_properties (MFHDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
set (last_test "MFHDF_FORTRAN_EXAMPLES-clearall-objects")

foreach (example ${examples})
  add_test (NAME f_exmftest_${example} COMMAND $<TARGET_FILE:f_exmf_${example}>)
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (f_exmftest_${example} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (f_exmftest_${example} PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "f_exmftest_${example}")
endforeach ()

foreach (example ${skip_examples})
  add_test (NAME f_exmftest_${example} COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_exmftest_${example}")
endforeach ()
