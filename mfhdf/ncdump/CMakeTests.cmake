
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME ncdump-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          test0.nc
          test1.nc
          test1.cdl
          test2.cdl
  )

  #-- Copy all the data files from the test directory into the source directory
  #MESSAGE (STATUS " Copying ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/test0.cdl to ${PROJECT_BINARY_DIR}/")
  ADD_CUSTOM_COMMAND (
      TARGET     ncdump
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/test0.cdl ${PROJECT_BINARY_DIR}/test0.cdl
  )

  IF (NCGEN_UTILITY)
#    ncdump test0.cdl
#    SET (CMD $<TARGET_FILE:ncgen>)
#    ADD_CUSTOM_COMMAND (
#        OUTPUT ${PROJECT_BINARY_DIR}/test0.nc
#        COMMAND ${CMD}
#        ARGS > -o test0.nc -n test0.cdl
#        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
#        DEPENDS test0.cdl
#    )
#
#    $<TARGET_FILE:ncgen> -o test0.nc -n test0.cdl
#    $<TARGET_FILE:ncdump> test0.nc > test1.cdl
#    $<TARGET_FILE:ncgen> -o test1.nc -n test1.cdl
#    $<TARGET_FILE:ncdump> -n test0 test1.nc > test2.cdl
#    ADD_TEST (
#        NAME  ncdump-comp
#        COMMAND  ${CMAKE_COMMAND}
#            -E compare test1.cdl test2.cdl
#    )
#    SET_TESTS_PROPERTIES (ncdump-comp PROPERTIES DEPENDS ncdump-clearall-objects LABELS ${PROJECT_NAME})
  ENDIF (NCGEN_UTILITY)
