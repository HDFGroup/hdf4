
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
    NAME NCGEN-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
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

HDFTEST_COPY_FILE("${HDF4_MFHDF_NCGEN_SOURCE_DIR}/test0.cdl" "${PROJECT_BINARY_DIR}/test0.cdl" "ncgen_files")
add_custom_target(ncgen_files ALL COMMENT "Copying files needed by ncgen tests" DEPENDS ${ncgen_files_list})

#-- Adding test for ncgen
add_test (
    NAME NCGEN-test0.nc
    COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:ncgen> -b -o test0.nc test0.cdl
)
set_tests_properties (NCGEN-test0.nc PROPERTIES DEPENDS "NCGEN-clearall-objects" LABELS ${PROJECT_NAME})

add_test (
    NAME NCGEN-test1.cdl
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncdump>"
        -D "TEST_ARGS:STRING=-n;test1;test0.nc"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test1.cdl"
        -D "TEST_EXPECT=0"
        -D "TEST_SKIP_COMPARE=TRUE"
        -P "${HDF_RESOURCES_DIR}/runTest.cmake"
)
set_tests_properties (NCGEN-test1.cdl PROPERTIES DEPENDS "NCGEN-test0.nc" LABELS ${PROJECT_NAME})

add_test (
    NAME NCGEN-test1.nc
    COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:ncgen> -b test1.cdl
)
set_tests_properties (NCGEN-test1.nc PROPERTIES DEPENDS "NCGEN-test1.cdl" LABELS ${PROJECT_NAME})

add_test (
    NAME NCGEN-test2.cdl
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncdump>"
        -D "TEST_ARGS:STRING=test1.nc"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test2.cdl"
        -D "TEST_EXPECT=0"
        -D "TEST_REFERENCE=test1.cdl"
        -P "${HDF_RESOURCES_DIR}/runTest.cmake"
)
set_tests_properties (NCGEN-test2.cdl PROPERTIES DEPENDS "NCGEN-test1.nc" LABELS ${PROJECT_NAME})

#
#ctest0$(EXEEXT):        ncgen$(EXEEXT) $(srcdir)/test0.cdl
#    $(TESTS_ENVIRONMENT) ./ncgen -c -o ctest0.nc $(srcdir)/test0.cdl > test0.c
#    $(COMPILE) -c -o ctest0$(EXEEXT).o test0.c
#    $(LINK) ctest0$(EXEEXT).o $(ctest0_LDADD) $(LDFLAGS) $(SHLIBLOC) $(LIBS)

add_test (
    NAME NCGEN-ctest0
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncgen>"
        -D "TEST_ARGS:STRING=-c;-o;ctest0.nc;test0.cdl"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test0.c"
        -D "TEST_EXPECT=0"
        -D "TEST_SKIP_COMPARE=TRUE"
        -P "${HDF_RESOURCES_DIR}/runTest.cmake"
)
set_tests_properties (NCGEN-ctest0 PROPERTIES DEPENDS "NCGEN-test2.cdl" LABELS ${PROJECT_NAME})
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

if (HDF4_BUILD_FORTRAN)
#
#ftest0$(EXEEXT):        ncgen$(EXEEXT) test0.cdl netcdf.inc
#    $(TESTS_ENVIRONMENT) ./ncgen -f -o ftest0.nc $(srcdir)/test0.cdl > test0.f
#    $(F77) $(FFLAGS) -o $@ test0.f $(LDFLAGS) $(SHLIBLOC) $(LIBS)
add_test (
    NAME NCGEN-ftest0
    COMMAND "${CMAKE_COMMAND}"
        -D "TEST_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}"
        -D "TEST_PROGRAM=$<TARGET_FILE:ncgen>"
        -D "TEST_ARGS:STRING=-f;-o;ftest0.nc;test0.cdl"
        -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
        -D "TEST_OUTPUT=test0.f"
        -D "TEST_EXPECT=0"
        -D "TEST_SKIP_COMPARE=TRUE"
        -P "${HDF_RESOURCES_DIR}/runTest.cmake"
)
set_tests_properties (NCGEN-ftest0 PROPERTIES DEPENDS "NCGEN-ctest0" LABELS ${PROJECT_NAME})
# Test the "-f" option of ncgen
#f-check:    b-check ftest0
#    $(TESTS_ENVIRONMENT) ./ftest0
#    $(TESTS_ENVIRONMENT) $(NCDUMP) -n test1 ftest0.nc > ftest1.cdl
#    @if $(DIFF) test1.cdl ftest1.cdl; then                              \
#      echo "*** ncgen -f test successful ***";                          \
#    else                                                                \
#      echo "*** ncgen -f test failed (but roundoff differences are OK) ***"; \
#    fi
endif ()
