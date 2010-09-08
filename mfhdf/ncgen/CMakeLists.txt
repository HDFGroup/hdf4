cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_NCGEN)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

ADD_DEFINITIONS (-DHDF)

IF (WIN32)
  ADD_DEFINITIONS(-DDOS_FS)
ENDIF (WIN32)

SET (ncgen_SRCS
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/close.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/escapes.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/generate.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/genlib.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/getfill.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/init.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/load.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/main.c
    ${HDF4_MFHDF_NCGEN_SOURCE_DIR}/ncgentab.c
)
  
ADD_EXECUTABLE (ncgen ${ncgen_SRCS})
TARGET_LINK_LIBRARIES (ncgen ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
TARGET_NAMING (ncgen)
TARGET_WIN_PROPERTIES (ncgen)

#-- Adding test for ncgen

#ADD_TEST (NAME testncgen COMMAND $<TARGET_FILE:testncgen>)
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
#  #MESSAGE(STATUS " Copying ${HDF4_MFHDF_TEST_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
#  ADD_CUSTOM_COMMAND (
#      TARGET     ncgentest 
#      POST_BUILD
#      COMMAND    ${CMAKE_COMMAND}
#      ARGS       -E copy_if_different ${HDF4_MFHDF_TEST_DIR}/${h4_file} ${dest}
#  )
#      
#ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        ncgen
    RUNTIME DESTINATION
        bin/tools
    COMPONENT
        toolsapplications
)
