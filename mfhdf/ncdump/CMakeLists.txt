cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_NCDUMP)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32 AND NOT CYGWIN)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32 AND NOT CYGWIN)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32 AND NOT CYGWIN)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (ncdump_SRCS
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/dumplib.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/ncdump.c
    ${HDF4_MFHDF_NCDUMP_SOURCE_DIR}/vardata.c
    ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
)
  
ADD_EXECUTABLE (ncdump ${ncdump_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncdump ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET} )
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (ncdump ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (ncdump ${LIB_TYPE})
TARGET_WIN_PROPERTIES (ncdump ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

IF (BUILD_TESTING)

#-- Adding test for ncdump
#ADD_TEST (NAME testncdump COMMAND $<TARGET_FILE:testncdump>)
# 
##-- Copy all the dat files from the test directory into the source directory
#SET (HDF4_REFERENCE_TEST_FILES
#    sds_szipped.dat
#    smallslice.0000.nc
#    test1.nc
#    testout.sav
#)
# 
#FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
#  SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
#  #MESSAGE (STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
#  ADD_CUSTOM_COMMAND (
#      TARGET     ncdumptest 
#      POST_BUILD
#      COMMAND    ${CMAKE_COMMAND}
#      ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
#  )
#      
#ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
  IF (NCGEN_UTILITY)
#    $(TESTS_ENVIRONMENT) ../ncgen/ncgen -o test0.nc -n $(srcdir)/test0.cdl
#    $(TESTS_ENVIRONMENT) ./ncdump test0.nc > test1.cdl
#    $(TESTS_ENVIRONMENT) ../ncgen/ncgen -o test1.nc -n test1.cdl
#    $(TESTS_ENVIRONMENT) ./ncdump -n test0 test1.nc > test2.cdl
#    @cmp test1.cdl test2.cdl && echo "*** ncdump test successful ***" ;     \
#        rm -f test1.nc test1.cdl test2.cdl
  ENDIF (NCGEN_UTILITY)

ENDIF (BUILD_TESTING)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        ncdump
    RUNTIME DESTINATION
        ${HDF4_INSTALL_BIN_DIR}/tools
    COMPONENT
        toolsapplications
)


