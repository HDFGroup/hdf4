cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_HDIFF)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

IF (WIN32)
  SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
ENDIF (WIN32)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (hdiff_SRCS
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_array.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_gr.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_list.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_main.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_mattbl.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_gattr.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_misc.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_sds.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_table.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_vs.c
    ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdiff_dim.c
    ${HDF4_SOURCE_DIR}/mfhdf/util/getopt.c
)
  
ADD_EXECUTABLE(hdiff ${hdiff_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES(hdiff ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hdiff ${LIB_TYPE})
TARGET_WIN_PROPERTIES (hdiff ${LIB_TYPE})

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  #-- Adding test for hdifftst
  SET (hdifftst_SRCS
      ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/hdifftst.c
  )
  
  ADD_EXECUTABLE (hdifftst ${hdifftst_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdifftst ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdifftst ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (hdifftst ${LIB_TYPE})
  TARGET_WIN_PROPERTIES (hdifftst ${LIB_TYPE})

  ADD_TEST (NAME hdifftst COMMAND $<TARGET_FILE:hdifftst>)

  MACRO (ADD_H4_TEST resultfile resultcode)
    ADD_TEST (
        NAME HDIFF-${resultfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdiff>"
            -D "TEST_ARGS:STRING=${ARGN}"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}/testfiles"
            -D "TEST_OUTPUT=${resultfile}.out"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_REFERENCE=${resultfile}.txt"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
    )
  ENDMACRO (ADD_H4_TEST file)
 
  #-- Copy all the data files from the test directory into the source directory
  SET (HDF4_REFERENCE_TEST_FILES
      hdifftst1.hdf
      hdifftst2.hdf
      hdifftst3.hdf
      hdifftst4.hdf
      hdifftst5.hdf
      hdifftst6.hdf
      hdifftst7.hdf
  )
  SET (HDF4_REFERENCE_FILES
      hdiff_01.txt
      hdiff_02.txt
      hdiff_03.txt
      hdiff_04.txt
      hdiff_05.txt
#      hdiff_06.txt
      hdiff_07.txt
      hdiff_08.txt
      hdiff_09.txt
      hdiff_10.txt
      hdiff_11.txt
      hdiff_12.txt
      hdiff_13.txt
      hdiff_14.txt
      hdiff_15.txt
  )
 
  FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
    SET (dest "${PROJECT_BINARY_DIR}/testfiles/${h4_file}")
    #MESSAGE(STATUS " Copying ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/testfiles/")
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff 
        POST_BUILD
        COMMAND    ${CMAKE_COMMAND}
        ARGS       -E copy_if_different ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${h4_file} ${dest}
    )
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})

  FOREACH (out_file ${HDF4_REFERENCE_FILES})
    SET (outdest "${PROJECT_BINARY_DIR}/testfiles/${out_file}")
    #MESSAGE (STATUS " Translating ${out_file}")
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${XLATE_UTILITY}
        ARGS       ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/${out_file} ${outdest} -l3
    )
  ENDFOREACH (out_file ${HDF4_REFERENCE_FILES})

  IF (WIN32)
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${XLATE_UTILITY}
        ARGS       ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/hdiff_06w.txt ${PROJECT_BINARY_DIR}/testfiles/hdiff_06.txt -l3
    )
  ELSE (WIN32)
    ADD_CUSTOM_COMMAND (
        TARGET     hdiff
        POST_BUILD
        COMMAND    ${XLATE_UTILITY}
        ARGS       ${HDF4_MFHDF_HDIFF_SOURCE_DIR}/testfiles/hdiff_06.txt ${PROJECT_BINARY_DIR}/testfiles/hdiff_06.txt -l3
    )
  ENDIF (WIN32)

  # help message
  ADD_H4_TEST (hdiff_01 1) 

  # Compare global attributes only
  ADD_H4_TEST (hdiff_02 1 -g hdifftst1.hdf hdifftst2.hdf)

  # Compare SD local attributes only
  ADD_H4_TEST (hdiff_03 1 -s hdifftst1.hdf hdifftst2.hdf)

  # Compare SD data only
  ADD_H4_TEST (hdiff_04 1 -d hdifftst1.hdf hdifftst2.hdf)

  # Compare Vdata data only
  ADD_H4_TEST (hdiff_05 1 -D hdifftst1.hdf hdifftst2.hdf)

  # Print statistics
  ADD_H4_TEST (hdiff_06 1 -d -S hdifftst1.hdf hdifftst2.hdf)

  # Compare SD data on variable(s)
  ADD_H4_TEST (hdiff_07 1 -d -v dset1 hdifftst1.hdf hdifftst2.hdf)

  # Compare vdata on variable(s) 
  ADD_H4_TEST (hdiff_08 1 -D -u vdata1 hdifftst1.hdf hdifftst2.hdf)

  # Print difference up to count number
  ADD_H4_TEST (hdiff_09 1 -d -e 2 hdifftst1.hdf hdifftst2.hdf)

  # Print difference when it is greater than limit
  ADD_H4_TEST (hdiff_10 1 -d -t 2 hdifftst1.hdf hdifftst2.hdf)

  # no options
  ADD_H4_TEST (hdiff_11 1 hdifftst1.hdf hdifftst2.hdf)

  # percent (relative)
  ADD_H4_TEST (hdiff_12 1 -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf)

  # hyperslab reading 
  ADD_H4_TEST (hdiff_13 0 hdifftst3.hdf hdifftst4.hdf)

  # lone dim
  ADD_H4_TEST (hdiff_14 1 hdifftst5.hdf hdifftst6.hdf)

  # group loop
  ADD_H4_TEST (hdiff_15 0 -b hdifftst7.hdf hdifftst7.hdf)

ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        hdiff
    RUNTIME DESTINATION
        bin/tools
    COMPONENT
        toolsapplications
)
