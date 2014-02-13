
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
ADD_TEST (
    NAME HDF_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        General_HDFobjects.hdf
        General_Rimages.hdf
        General_Vdatas.hdf
        General_Vgroups.hdf
        Image_with_Palette.hdf
        Packed_Vdata.hdf
        Two_Vdatas.hdf
        Two_Vgroups.hdf
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (HDF_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
ELSE (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (HDF_EXAMPLES-clearall-objects PROPERTIES LABELS EXAMPLES)
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "HDF_EXAMPLES-clearall-objects")

FOREACH (example ${examples})
  ADD_TEST (NAME hdftest_${example} COMMAND $<TARGET_FILE:hdf_${example}>)
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (hdftest_${example} PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (hdftest_${example} PROPERTIES LABELS EXAMPLES)
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "hdftest_${example}")
ENDFOREACH (example ${examples})

ADD_TEST (NAME hdftest_VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:hdf_VG_add_sds_to_vgroup>)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (hdftest_VG_add_sds_to_vgroup PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
ELSE (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (hdftest_VG_add_sds_to_vgroup PROPERTIES LABELS EXAMPLES)
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "hdftest_VG_add_sds_to_vgroup")
