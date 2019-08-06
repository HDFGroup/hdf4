
##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

set (FORTRAN_SRC_DIR ${HDF4_HDF_TEST_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add test fortran stub library
#-----------------------------------------------------------------------------
if (NOT BUILD_SHARED_LIBS)
  add_library (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} STATIC ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  set_target_properties (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  TARGET_C_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} STATIC)
  target_link_libraries (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PUBLIC ${HDF4_SRC_LIB_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} STATIC)
else ()
  add_library (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} SHARED ${HDF4_HDF_TESTSOURCE_DIR}/forsupf.c)
  set_target_properties (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  TARGET_C_PROPERTIES (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} SHARED)
  target_link_libraries (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} PUBLIC ${HDF4_SRC_LIBSH_TARGET})
  H4_SET_LIB_OPTIONS (${HDF4_HDF_TEST_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_NAME} SHARED)
endif ()

#-- Adding test for fortest
add_executable (fortest ${HDF4_HDF_TESTSOURCE_DIR}/fortest.c)
if (NOT BUILD_SHARED_LIBS)
  TARGET_C_PROPERTIES (fortest STATIC)
  target_link_libraries (fortest PRIVATE ${HDF4_SRC_LIB_TARGET} ${HDF4_MF_LIB_TARGET})
else ()
  TARGET_C_PROPERTIES (fortest SHARED)
  target_link_libraries (fortest PRIVATE ${HDF4_SRC_LIBSH_TARGET} ${HDF4_MF_LIBSH_TARGET})
endif ()
set_target_properties (fortest PROPERTIES
    LINKER_LANGUAGE C
    COMPILE_DEFINITIONS "CMAKE_INTDIR"
)

#-----------------------------------------------------------------------------
#-- Adding test for fortestF
set (FORTEST_FSRCS
    ${HDF4_HDF_TESTSOURCE_DIR}/fortestF.f
    ${HDF4_HDF_TESTSOURCE_DIR}/forsupff.f
    ${HDF4_HDF_TESTSOURCE_DIR}/manf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/mgrf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/slabwf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/t24f.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tanf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tanfilef.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tpf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tr8f.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tsdmmsf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tsdnmmsf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tsdnntf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tsdntf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tsdstrf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tstubsf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tvattrf.f
    ${HDF4_HDF_TESTSOURCE_DIR}/tvsetf.f
)

add_executable (fortestF ${FORTEST_FSRCS} )
target_include_directories (fortestF
    PRIVATE "${CMAKE_Fortran_MODULE_DIRECTORY};${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}"
)
target_compile_options(fortestF
    PRIVATE $<$<STREQUAL:"x${CMAKE_Fortran_SIMULATE_ID}","xMSVC">:${WIN_COMPILE_FLAGS}>
)
if(MSVC)
  set_property(TARGET fortestF PROPERTY LINK_FLAGS "/SUBSYSTEM:CONSOLE ${WIN_LINK_FLAGS}")
endif()
set_target_properties (fortestF PROPERTIES
    LINKER_LANGUAGE Fortran
    Fortran_MODULE_DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}
)
if (NOT BUILD_SHARED_LIBS)
  target_link_libraries (fortestF PRIVATE ${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET})
else ()
  target_link_libraries (fortestF PRIVATE ${HDF4_SRC_FORTRAN_LIBSH_TARGET} ${HDF4_SRC_FCSTUB_LIBSH_TARGET} ${HDF4_HDF_TEST_FCSTUB_LIB_TARGET})
endif ()

#-- Copy all the dat files from the test directory into the source directory
set (HDF4_REFERENCE_TEST_FILES
    8bit.dat
    bitio.dat
    gr_r24.dat
    greyjpeg.dat
    jpeg.dat
    litend.dat
    nbit.dat
    tmgr.dat
    tvattr.dat
)
foreach (h4_file ${HDF4_REFERENCE_TEST_FILES})
  HDFTEST_COPY_FILE("${HDF4_HDF_TESTSOURCE_DIR}/test_files/${h4_file}" "${PROJECT_BINARY_DIR}/test_files/${h4_file}" "hdf_fortran_files")
endforeach ()
add_custom_target(hdf_fortran_files ALL COMMENT "Copying files needed by hdf fortran tests" DEPENDS ${hdf_fortran_files_list})

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

# Remove any output file left over from previous test run
add_test (
    NAME HDF_FORTRAN-clearall-objects
    COMMAND ${CMAKE_COMMAND} -E remove
        fortest.arg
        Fortran_err.dat
        GRcompressed.hdf
        GRchunked1.hdf
        GRchunked2.hdf
        manf.hdf
        slab1wf.hdf
        slab4wf.hdf
        slabwf.hdf
        tdf24f.hdf
        tdfanF.hdf
        tdfanflF.hdf
        tmgrf.hdf
)

add_test (NAME fortest COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:fortest>)
set_tests_properties (fortest PROPERTIES DEPENDS HDF_FORTRAN-clearall-objects)

add_test (NAME fortestF COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:fortestF>)
set (passRegex "All Fortran Interface Tests Passed")
set_tests_properties (fortestF PROPERTIES DEPENDS fortest PASS_REGULAR_EXPRESSION "${passRegex}")
