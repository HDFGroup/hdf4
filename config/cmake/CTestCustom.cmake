set (CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 1500)

set (CTEST_CUSTOM_WARNING_EXCEPTION
    ${CTEST_CUSTOM_WARNING_EXCEPTION}
    "note.*expected.*void.*but argument is of type.*volatile"
    "SZIP.src.*:[ \t]*warning"
    "jpeg.src.*:[ \t]*warning"
    "POSIX name for this item is deprecated"
    "disabling jobserver mode"
    "config.cmake.xlatefile.c"
)
 
 
set (CTEST_CUSTOM_MEMCHECK_IGNORE
    ${CTEST_CUSTOM_MEMCHECK_IGNORE}
    hdfpack-clear-refs
    hdfpalette-clear-refs
    hdfjpeg-clear-refs
    HDP_tst-clearall-objects
    HDP_tst_err-clearall-objects
    HDP-clear-my.dat
    HDP-clear-mybin.dat
    hdfimport-clear-refs
    HDIFF-clearall-objects
    hrepack-clearall-objects
    HDF_EXAMPLES-clearall-objects
    HDF_FORTRAN_EXAMPLES-clearall-objects
    MFHDF_EXAMPLES-clearall-objects
    MFHDF_FORTRAN_EXAMPLES-clearall-objects
    MFHDF_TEST-clearall-objects
    testhdf-clearall-objects
    testhdf_thf0-clearall-objects
    testhdf_thf1-clearall-objects
    testhdf_thf2-clearall-objects
    testhdf_thf3-clearall-objects
    testhdf_thf4-clearall-objects
    testhdf_thf5-clearall-objects
    testhdf_thf6-clearall-objects
    testhdf_thf7-clearall-objects
    testhdf_thf8-clearall-objects
    testhdf_thf9-clearall-objects
    testhdf_thf10-clearall-objects
    testhdf_thf11-clearall-objects
    testhdf_thf12-clearall-objects
    testhdf_thf13-clearall-objects
    testhdf_thf14-clearall-objects
    testhdf_thf15-clearall-objects
    testhdf_thf16-clearall-objects
    testhdf_thf17-clearall-objects
    testhdf_thf18-clearall-objects
    testhdf_thf19-clearall-objects
    testhdf_thf20-clearall-objects
    testhdf_thf21-clearall-objects
    testhdf_thf22-clearall-objects
    testhdf_thf23-clearall-objects
    testhdf_thf24-clearall-objects
    testhdf_thf25-clearall-objects
    testhdf_thf26-clearall-objects
    testhdf_thf27-clearall-objects
    testhdf_thf28-clearall-objects
    testhdf_thf29-clearall-objects
    testhdf_thf30-clearall-objects
    testhdf_thf31-clearall-objects
    testhdf_thf32-clearall-objects
    testhdf_thf33-clearall-objects
    testhdf_thf34-clearall-objects
    testhdf_thf35-clearall-objects
    testhdf_thf36-clearall-objects
    testhdf_thf37-clearall-objects
    testhdf_thf38-clearall-objects
    testhdf_thf39-clearall-objects
    testhdf_thf40-clearall-objects
    testhdf_thf41-clearall-objects
    testhdf_thf42-clearall-objects
    testhdf_thf43-clearall-objects
    testhdf_thf44-clearall-objects
    testhdf_thf45-clearall-objects
    testhdf_thf46-clearall-objects
    testhdf_thf47-clearall-objects
    testhdf_thf48-clearall-objects
    testhdf_thf49-clearall-objects
    testhdf_thf50-clearall-objects
    testhdf_thf51-clearall-objects
    testhdf_thf52-clearall-objects
    testhdf_thf53-clearall-objects
    testhdf_thf54-clearall-objects
    testhdf_thf55-clearall-objects
    testhdf_thf56-clearall-objects
    testhdf_thf57-clearall-objects
    testhdf_thf58-clearall-objects
    testhdf_thf59-clearall-objects
    testhdf_thf60-clearall-objects
    hrepack-clearall-objects
    MFHDF_FORTRAN-clearall-objects
    HDF_FORTRAN-clearall-objects
    hdfgif-clear-refs
    ncdump-clearall-objects
    NC_TEST-clearall-objects                            
    HEDIT-hdfed.input1
    HEDIT-ristosds.input1
)