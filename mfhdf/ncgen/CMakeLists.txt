cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_MFHDF_NCGEN)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR} ${HDF4_MFHDFSOURCE_DIR} ${HDF4_MFHDF_NCGEN_SOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS(-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

IF (WIN32 AND NOT CYGWIN)
  SET (ncgen_tab_SRCS
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msofttab.h
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msofttab.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/msoftyy.c
  )
  SET (NCGEN_UTILITY 1 PARENT_SCOPE)
ELSE (WIN32 AND NOT CYGWIN)
  FIND_PACKAGE (FLEX)
  IF (FLEX_FOUND)
      FLEX_TARGET (NCGEN_SCANNER ncgen.l ${CMAKE_CURRENT_BINARY_DIR}/ncgenyy.c)
  ENDIF (FLEX_FOUND)

  FIND_PACKAGE (BISON)
  IF (BISON_FOUND)
    BISON_TARGET (NCGEN_PARSER ncgen.y ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c COMPILE_FLAGS -y)
  ENDIF (BISON_FOUND)

  IF (BISON_NCGEN_PARSER_DEFINED AND FLEX_NCGEN_SCANNER_DEFINED)
    SET (NCGEN_UTILITY 1 PARENT_SCOPE)
  ENDIF (BISON_NCGEN_PARSER_DEFINED AND FLEX_NCGEN_SCANNER_DEFINED)
#  ADD_FLEX_BISON_DEPENDENCY (NCGEN_SCANNER NCGEN_PARSER)
  SET (ncgen_tab_SRCS
      ${CMAKE_CURRENT_BINARY_DIR}/ncgenyy.c
      ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c
      ${CMAKE_CURRENT_BINARY_DIR}/ncgentab.h
  )
ENDIF (WIN32 AND NOT CYGWIN)
 
#IF (${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c) 
  SET (ncgen_SRCS
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/close.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/escapes.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/generate.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/genlib.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/getfill.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/init.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/load.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/main.c
      ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/ncgen.h
  )
  
  ADD_EXECUTABLE (ncgen ${ncgen_SRCS} ${ncgen_tab_SRCS})
  TARGET_C_PROPERTIES (ncgen " " " ")
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (ncgen ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

  IF (BUILD_TESTING)
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

  ENDIF (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

  #-----------------------------------------------------------------------------
  # Add file(s) to CMake Install
  #-----------------------------------------------------------------------------

  INSTALL_PROGRAM_PDB (ncgen ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

  INSTALL (
      TARGETS
          ncgen
      RUNTIME DESTINATION
          ${HDF4_INSTALL_TOOLS_BIN_DIR}
      COMPONENT
          toolsapplications
  )
#ENDIF (${CMAKE_CURRENT_BINARY_DIR}/ncgentab.c)
