
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME NCGEN-clearall-objects
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          ctest0.nc
          ctest1.cdl
          ftest0.nc
          ftest1.cdl
          ncgentab.c
          ncgentab.h
          ncgenyy.c
          netcdf.inc
          test0.c
          test0.f
          test0.nc
          test1.cdl
          test1.nc
          test2.cdl
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (NCGEN-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (NCGEN-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "NCGEN-clearall-objects")

  #-- Copy all the data files from the test directory into the source directory
  #MESSAGE (STATUS " Copying ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/test0.cdl to ${PROJECT_BINARY_DIR}/")
  ADD_CUSTOM_COMMAND (
      TARGET     ncgen
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/test0.cdl ${PROJECT_BINARY_DIR}/test0.cdl
  )

#-- Adding test for ncgen
ADD_TEST (
    NAME NCGEN-test0.nc
    COMMAND $<TARGET_FILE:ncgen> -b -o test0.nc test0.cdl
)
SET_TESTS_PROPERTIES (NCGEN-test0.nc PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
SET (last_test "NCGEN-test0.nc")

ADD_TEST (
    NAME NCGEN-test1.cdl
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncdump>"
        -D "TEST_ARGS:STRING=-n;test1;test0.nc"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test1.cdl"
        -D "TEST_EXPECT=0"
        -D "TEST_SKIP_COMPARE=TRUE"
        -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
)
SET_TESTS_PROPERTIES (NCGEN-test1.cdl PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
SET (last_test "NCGEN-test1.cdl")

ADD_TEST (
    NAME NCGEN-test1.nc
    COMMAND $<TARGET_FILE:ncgen> -b test1.cdl
)
SET_TESTS_PROPERTIES (NCGEN-test1.nc PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
SET (last_test "NCGEN-test1.nc")

ADD_TEST (
    NAME NCGEN-test2.cdl
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncdump>"
        -D "TEST_ARGS:STRING=test1.nc"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test2.cdl"
        -D "TEST_EXPECT=0"
        -D "TEST_REFERENCE=test1.cdl"
        -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
)
SET_TESTS_PROPERTIES (NCGEN-test2.cdl PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
SET (last_test "NCGEN-test2.cdl")

#
#ctest0$(EXEEXT):        ncgen$(EXEEXT) $(srcdir)/test0.cdl
#    $(TESTS_ENVIRONMENT) ./ncgen -c -o ctest0.nc $(srcdir)/test0.cdl > test0.c
#    $(COMPILE) -c -o ctest0$(EXEEXT).o test0.c
#    $(LINK) ctest0$(EXEEXT).o $(ctest0_LDADD) $(LDFLAGS) $(SHLIBLOC) $(LIBS)

ADD_TEST (
    NAME NCGEN-ctest0
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncgen>"
        -D "TEST_ARGS:STRING=-c;-o;ctest0.nc;test0.cdl"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test0.c"
        -D "TEST_EXPECT=0"
        -D "TEST_SKIP_COMPARE=TRUE"
        -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
)
SET_TESTS_PROPERTIES (NCGEN-ctest0 PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
SET (last_test "NCGEN-ctest0")
#
## Test the "-c" option of ncgen
#c-check:    b-check ctest0
#    $(TESTS_ENVIRONMENT) ./ctest0        # tests `-c' option, creates ctest0.nc
#    $(TESTS_ENVIRONMENT) $(NCDUMP) -n test1 ctest0.nc > ctest1.cdl
#    @if $(DIFF) test1.cdl ctest1.cdl; then                              \
#      echo "*** ncgen -c test successful ***";                          \
#    else                                                                \
#      echo "*** ncgen -c test failed  ***";                             \
#    fi

IF (HDF4_BUILD_FORTRAN)
#
#ftest0$(EXEEXT):        ncgen$(EXEEXT) test0.cdl netcdf.inc
#    $(TESTS_ENVIRONMENT) ./ncgen -f -o ftest0.nc $(srcdir)/test0.cdl > test0.f
#    $(F77) $(FFLAGS) -o $@ test0.f $(LDFLAGS) $(SHLIBLOC) $(LIBS)
  ADD_TEST (
      NAME NCGEN-ftest0
      COMMAND "${CMAKE_COMMAND}"
          -D "TEST_PROGRAM=$<TARGET_FILE:ncgen>"
          -D "TEST_ARGS:STRING=-f;-o;ftest0.nc;test0.cdl"
          -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
          -D "TEST_OUTPUT=test0.f"
          -D "TEST_EXPECT=0"
          -D "TEST_SKIP_COMPARE=TRUE"
          -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
  )
  SET_TESTS_PROPERTIES (NCGEN-ftest0 PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  SET (last_test "NCGEN-ftest0")
# Test the "-f" option of ncgen
#f-check:    b-check ftest0
#    $(TESTS_ENVIRONMENT) ./ftest0
#    $(TESTS_ENVIRONMENT) $(NCDUMP) -n test1 ftest0.nc > ftest1.cdl
#    @if $(DIFF) test1.cdl ftest1.cdl; then                              \
#      echo "*** ncgen -f test successful ***";                          \
#    else                                                                \
#      echo "*** ncgen -f test failed (but roundoff differences are OK) ***"; \
#    fi
ENDIF (HDF4_BUILD_FORTRAN)
