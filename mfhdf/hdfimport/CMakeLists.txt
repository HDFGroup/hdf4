cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_MFHDF_HDFIMPORT)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_MFHDFSOURCE_DIR})

IF (WIN32)
  SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
ENDIF (WIN32)

ADD_DEFINITIONS (-DHDF)

IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)

IF (HDF4_BUILD_XDR_LIB)
  IF (WIN32)
    ADD_DEFINITIONS (-DNO_SYS_XDR_INC)
  ENDIF (WIN32)
  INCLUDE_DIRECTORIES (${HDF4_MFHDF_XDR_DIR})
ENDIF (HDF4_BUILD_XDR_LIB)

SET (hdfimport_SRCS
    ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimport.c
)
  
ADD_EXECUTABLE (hdfimport ${hdfimport_SRCS})
IF (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
ELSE (HDF4_BUILD_XDR_LIB)
  TARGET_LINK_LIBRARIES (hdfimport ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
ENDIF (HDF4_BUILD_XDR_LIB)
TARGET_NAMING (hdfimport ${LIB_TYPE})
TARGET_WIN_PROPERTIES (hdfimport ${LIB_TYPE})

##############################################################################
##############################################################################
###           T E S T I N G                                                ###
##############################################################################
##############################################################################

IF (BUILD_TESTING)
 
  #-- Copy all the dat files from the test directory into the source directory
  SET (HDF4_REFERENCE_TEST_FILES
      hdfimport.input1
      hdfimport.out1
      hdfimport.out2
      hdfimport.out3
      SDSfloat2.hdf
      SDSfloat3.hdf
  )
   
  FOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
      SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
      #MESSAGE (STATUS " Copying ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${h4_file} to ${PROJECT_BINARY_DIR}/")
      ADD_CUSTOM_COMMAND (
          TARGET     hdfimport 
          POST_BUILD
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${h4_file} ${dest}
     )      
  ENDFOREACH (h4_file ${HDF4_REFERENCE_TEST_FILES})
 
  #-- Copy all the hdfls tst files from the test directory into the source directory
  SET (HDF4_LS_TEST_FILES
      ctxtr2.tst 
      ctxtr3.tst 
      cb32i2.tst 
      cb32i3.tst 
      cb16i2.tst 
      cb16i3.tst 
      cb32r2.tst 
      cb32r3.tst 
      cb64r2.tst 
      cb64r3.tst 
      cb64r2-n.tst 
      cb64r3-n.tst 
      ctxtr2_ris.tst
      cb64r2_ris.tst
      SDSfloat2.tst
      SDSfloat3.tst
  )
   
  FOREACH (ls_file ${HDF4_LS_TEST_FILES})
      SET (dest "${PROJECT_BINARY_DIR}/${ls_file}")
      #MESSAGE (STATUS " Copying ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/${ls_file} to ${PROJECT_BINARY_DIR}/")
      ADD_CUSTOM_COMMAND (
          TARGET     hdfimport 
          POST_BUILD
          COMMAND    ${CMAKE_COMMAND}
          ARGS       -E copy_if_different ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/testfiles/${ls_file} ${dest}
     )      
  ENDFOREACH (ls_file ${HDF4_LS_TEST_FILES})

  #-- hdfimporttest
  SET (hdfimporttest_SRCS
      ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/hdfimporttest.c
  )
  
  ADD_EXECUTABLE (hdfimporttest ${hdfimporttest_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (hdfimporttest ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (hdfimporttest ${LIB_TYPE})
  TARGET_WIN_PROPERTIES (hdfimporttest ${LIB_TYPE})

  #-- gen_sds_floats
  SET (gen_sds_floats_SRCS
      ${HDF4_MFHDF_HDFIMPORT_SOURCE_DIR}/gen_sds_floats.c
  )
  
  ADD_EXECUTABLE (gen_sds_floats ${gen_sds_floats_SRCS})
  IF (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS} ${HDF4_MF_XDR_LIB_TARGET})
  ELSE (HDF4_BUILD_XDR_LIB)
    TARGET_LINK_LIBRARIES (gen_sds_floats ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
  ENDIF (HDF4_BUILD_XDR_LIB)
  TARGET_NAMING (gen_sds_floats ${LIB_TYPE})
  TARGET_WIN_PROPERTIES (gen_sds_floats ${LIB_TYPE})
  
##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

  MACRO (ADD_H4_TEST resultfile resultcode testtfile testtype)
    IF ( NOT ${testtype} STREQUAL "")
      IF (${testtype} STREQUAL "N")
        ADD_TEST (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -n -o ${testtfile}.hdf)
      ENDIF (${testtype} STREQUAL "N")
      IF (${testtype} STREQUAL "R")
        ADD_TEST (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -o ${testtfile}.hdf -raster ${ARGN})
      ENDIF (${testtype} STREQUAL "R")
    ELSE ( NOT ${testtype} STREQUAL "")
      ADD_TEST (NAME HIMPORT-${testtfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile} -o ${testtfile}.hdf)
    ENDIF ( NOT ${testtype} STREQUAL "")

    ADD_TEST (
        NAME HIMPORTLS-${testtfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
            -D "TEST_ARGS:STRING=-l;${testtfile}.hdf"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=${testtfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
            -D "TEST_REFERENCE=${testtfile}.tst"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
    )
  ENDMACRO (ADD_H4_TEST)

  MACRO (ADD_H4_TEST_OUT resultfile resultcode)
    ADD_TEST (NAME HIMPORT-OUT-${resultfile} COMMAND $<TARGET_FILE:hdfimport> ${resultfile}.hdf -o ${resultfile}.out)

    ADD_TEST (
        NAME HIMPORTLS-OUT-${resultfile}
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfls>"
            -D "TEST_ARGS:STRING=${resultfile}.out"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_OUTPUT=${resultfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_FILTER:STRING=(File library|String)[^\n]+\n"
            -D "TEST_REFERENCE=${resultfile}.tst"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
    )
  ENDMACRO (ADD_H4_TEST_OUT)

  MACRO (ADD_H4_TEST_ED testfile resultfile resultcode)
    ADD_TEST (
        NAME HIMPORT-EDIT
        COMMAND "${CMAKE_COMMAND}"
            -D "TEST_PROGRAM=$<TARGET_FILE:hdfed>"
            -D "TEST_ARGS:STRING=-batch"
            -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
            -D "TEST_INPUT=${testfile}"
            -D "TEST_OUTPUT=${testfile}.tmp"
            -D "TEST_EXPECT=${resultcode}"
            -D "TEST_REFERENCE=${resultfile}"
            -P "${HDF4_RESOURCES_DIR}/runTest.cmake"
    )
  ENDMACRO (ADD_H4_TEST_ED)

##############################################################################
##############################################################################
###           T H E   T E S T S                                            ###
##############################################################################
##############################################################################

  # Remove any output file left over from previous test run
  ADD_TEST (
      NAME hdfimport-clear-refs
      COMMAND    ${CMAKE_COMMAND}
          -E remove 
          ./*.tmp
          ./*.err
  )

  ADD_TEST (NAME HIMPORTtest COMMAND $<TARGET_FILE:hdfimporttest>)

  # "Testing for 32-bit floating point ASCII (2D data)" 
  ADD_H4_TEST (ctxtr2 0 ctxtr2 "")
  # "Testing for 32-bit floating point ASCII (3D data)" 
  ADD_H4_TEST (ctxtr3 0 ctxtr3 "")
  # "Testing for 32-bit integer binary (2D data)" 
  ADD_H4_TEST (cb32i2 0 cb32i2 "")
  # "Testing for 32-bit integer  binary (3D data)" 
  ADD_H4_TEST (cb32i3 0 cb32i3 "")
  # "Testing for 16-bit integer binary (2D data)" 
  ADD_H4_TEST (cb16i2 0 cb16i2 "")
  # "Testing for 16-bit integer (3D data)" 
  ADD_H4_TEST (cb16i3 0 cb16i3 "")
  # "Testing for 32-bit floating point binary (2D data)" 
  ADD_H4_TEST (cb32r2 0 cb32r2 "")
  # "Testing for 32-bit floating point binary (3D data)" 
  ADD_H4_TEST (cb32r3 0 cb32r3 "")
  # "Testing for 64-bit floating point binary (2D data) - Default Behaviour (Conversion to 32 bit FP SDS)" 
  ADD_H4_TEST (cb64r2 0 cb64r2 "")
  # "Testing for 64-bit floating point binary (3D data) - Default Behaviour (Conversion to 32-bit FP SDS)" 
  ADD_H4_TEST (cb64r3 0 cb64r3 "")
  # "Testing for 64-bit floating point binary (2D data) - Conversion to 64-bit FP SDS" 
  ADD_H4_TEST (cb64r2 0 cb64r2-n "N")
  # "Testing for 64-bit floating point binary (3D data) - Conversion to 64-bit FP SDS" 
  ADD_H4_TEST (cb64r3 0 cb64r3-n "N")
  # "Testing for raster options" 
  ADD_H4_TEST (ctxtr2 0 ctxtr2_ris "R" -e 50 50)
  ADD_H4_TEST (cb64r2 0 cb64r2_ris "R" -i 50 50 -f)
  #
  # test with hdf files
  # "Testing for reading from hdf files" 
  ADD_H4_TEST_OUT (SDSfloat2 0)
  ADD_H4_TEST_OUT (SDSfloat3 0)

  ADD_H4_TEST_ED (hdfimport.input1 hdfimport.out2 0)

ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Rules for Installation of tools using make Install target
#-----------------------------------------------------------------------------
INSTALL (
    TARGETS
        hdfimport
    RUNTIME DESTINATION
        bin/tools
    COMPONENT
        toolsapplications
)
