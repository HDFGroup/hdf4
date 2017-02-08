
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
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
set_tests_properties (MFHDF_EXAMPLES-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
set (last_test "MFHDF_EXAMPLES-clearall-objects")

foreach (example ${examples})
  add_test (NAME MFHDF_EXAMPLES-${example} COMMAND $<TARGET_FILE:mf_${example}>)
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (MFHDF_EXAMPLES-${example} PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  else ()
    set_tests_properties (MFHDF_EXAMPLES-${example} PROPERTIES LABELS ${PROJECT_NAME})
  endif ()
  set (last_test "MFHDF_EXAMPLES-${example}")
endforeach ()
