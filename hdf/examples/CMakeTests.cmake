
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
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
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
else ()
  set_tests_properties (HDF_EXAMPLES-clearall-objects PROPERTIES LABELS EXAMPLES)
endif ()
set (last_test "HDF_EXAMPLES-clearall-objects")

foreach (example ${examples})
  add_test (NAME HDF_EXAMPLES-${example} COMMAND $<TARGET_FILE:hdf_${example}>)
  if (NOT "${last_test}" STREQUAL "")
    set_tests_properties (HDF_EXAMPLES-${example} PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  else ()
    set_tests_properties (HDF_EXAMPLES-${example} PROPERTIES LABELS EXAMPLES)
  endif ()
  set (last_test "HDF_EXAMPLES-${example}")
endforeach ()

add_test (NAME HDF_EXAMPLES-VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:hdf_VG_add_sds_to_vgroup>)
if (NOT "${last_test}" STREQUAL "")
  set_tests_properties (HDF_EXAMPLES-VG_add_sds_to_vgroup PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
else ()
  set_tests_properties (HDF_EXAMPLES-VG_add_sds_to_vgroup PROPERTIES LABELS EXAMPLES)
endif ()
set (last_test "HDF_EXAMPLES-VG_add_sds_to_vgroup")
