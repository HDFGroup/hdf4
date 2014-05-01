
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
ADD_TEST (
    NAME HDF_FORTRAN_EXAMPLES-clearall-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
        General_HDFobjects.hdf
        General_RImages.hdf
        General_Vdatas.hdf
        General_Vgroups.hdf
        Image_with_Palette.hdf
        Packed_Vdata.hdf
        Two_Vdatas.hdf
        Two_Vgroups.hdf
)
if (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
else (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (HDF_FORTRAN_EXAMPLES-clearall-objects PROPERTIES LABELS EXAMPLES)
endif (NOT "${last_test}" STREQUAL "")
set (last_test "HDF_FORTRAN_EXAMPLES-clearall-objects")

foreach (example ${examples})
  ADD_TEST (NAME f_extest_${example} COMMAND $<TARGET_FILE:f_ex_${example}>)
  if (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_extest_${example} PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
  else (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (f_extest_${example} PROPERTIES LABELS EXAMPLES)
  endif (NOT "${last_test}" STREQUAL "")
  set (last_test "f_extest_${example}")
endforeach (example ${examples})

ADD_TEST (NAME f_extest_VG_add_sds_to_vgroup COMMAND $<TARGET_FILE:f_ex_VG_add_sds_to_vgroup>)
if (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (f_extest_VG_add_sds_to_vgroup PROPERTIES DEPENDS ${last_test} LABELS EXAMPLES)
else (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (f_extest_VG_add_sds_to_vgroup PROPERTIES LABELS EXAMPLES)
endif (NOT "${last_test}" STREQUAL "")
set (last_test "f_extest_VG_add_sds_to_vgroup")

foreach (example ${skip_examples})
  ADD_TEST (NAME f_extest_${example} COMMAND ${CMAKE_COMMAND} -E echo "SKIP f_extest_${example}")
endforeach (example ${skip_examples})
