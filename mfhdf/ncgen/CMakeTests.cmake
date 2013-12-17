
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME ncgen-clearall-objects
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
    SET_TESTS_PROPERTIES (ncgen-clearall-objects PROPERTIES DEPENDS ${last_test} LABELS ${PROJECT_NAME})
  ELSE (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (ncgen-clearall-objects PROPERTIES LABELS ${PROJECT_NAME})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "ncgen-clearall-objects")

  #-- Copy all the data files from the test directory into the source directory
  #MESSAGE (STATUS " Copying ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/test0.cdl to ${PROJECT_BINARY_DIR}/")
  ADD_CUSTOM_COMMAND (
      TARGET     ncgen
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/test0.cdl ${PROJECT_BINARY_DIR}/test0.cdl
  )

#-- Adding test for ncgen
#
#if HDF_BUILD_NETCDF
#
#if HDF_BUILD_FORTRAN
#check: ncgen$(EXEEXT) $(srcdir)/test0.cdl b-check c-check f-check
#else
#check: ncgen$(EXEEXT) $(srcdir)/test0.cdl b-check c-check
#endif
#
#else
#check: ncgen$(EXEEXT) $(srcdir)/test0.cdl b-check c-check
#endif
#
## Test the "-b" option of ncgen
#b-check:    ncgen$(EXEEXT) $(srcdir)/test0.cdl test1.cdl
#    $(TESTS_ENVIRONMENT) ./ncgen -b test1.cdl
#    $(TESTS_ENVIRONMENT) $(NCDUMP) test1.nc > test2.cdl
#    @if $(DIFF) test1.cdl test2.cdl; then                               \
#      echo "*** ncgen -b test successful ***";                          \
#    else                                                                \
#      echo "*** ncgen -b test failed ***";                              \
#    fi
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
#if HDF_BUILD_NETCDF
#
#if HDF_BUILD_FORTRAN
## Test the "-f" option of ncgen
#f-check:    b-check ftest0
#    $(TESTS_ENVIRONMENT) ./ftest0
#    $(TESTS_ENVIRONMENT) $(NCDUMP) -n test1 ftest0.nc > ftest1.cdl
#    @if $(DIFF) test1.cdl ftest1.cdl; then                              \
#      echo "*** ncgen -f test successful ***";                          \
#    else                                                                \
#      echo "*** ncgen -f test failed (but roundoff differences are OK) ***"; \
#    fi
#
#ftest0$(EXEEXT):        ncgen$(EXEEXT) test0.cdl netcdf.inc
#    $(TESTS_ENVIRONMENT) ./ncgen -f -o ftest0.nc $(srcdir)/test0.cdl > test0.f
#    $(F77) $(FFLAGS) -o $@ test0.f $(LDFLAGS) $(SHLIBLOC) $(LIBS)
#endif
#
#endif
#
#netcdf.inc:
#    ln -s ../fortran/$@ .
#
#test1.cdl:  test0.nc
#    $(TESTS_ENVIRONMENT) $(NCDUMP) -n test1 test0.nc > $@
#
#test0.nc:   ncgen$(EXEEXT) $(srcdir)/test0.cdl
#    $(TESTS_ENVIRONMENT) ./ncgen -b -o test0.nc $(srcdir)/test0.cdl
#
#ctest0$(EXEEXT):        ncgen$(EXEEXT) $(srcdir)/test0.cdl
#    $(TESTS_ENVIRONMENT) ./ncgen -c -o ctest0.nc $(srcdir)/test0.cdl > test0.c
#    $(COMPILE) -c -o ctest0$(EXEEXT).o test0.c
#    $(LINK) ctest0$(EXEEXT).o $(ctest0_LDADD) $(LDFLAGS) $(SHLIBLOC) $(LIBS)
