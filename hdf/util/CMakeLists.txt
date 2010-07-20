cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_UTIL C CXX)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})
INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR}/lib)

IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)

#-- Adding utility hdfls
ADD_EXECUTABLE (hdfls ${HDF4_HDF_SOURCE_DIR}/util/hdfls.c)
TARGET_NAMING (hdfls)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfls PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdfls ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME testhdfls COMMAND $<TARGET_FILE:hdfls>)

#-- Adding utility hdf2gif

SET (hdf2gif_SRCS
    ${HDF4_HDF_SOURCE_DIR}/util/hdf2gif.c
    ${HDF4_HDF_SOURCE_DIR}/util/hdfgifwr.c
)
  
ADD_EXECUTABLE (hdf2gif ${hdf2gif_SRCS})
TARGET_NAMING (hdf2gif)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdf2gif PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdf2gif ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testhdf2gif COMMAND $<TARGET_FILE:hdf2gif> skull.hdf skull.gif)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_HDF2GIF_TEST_FILES
    skull.hdf
)
FOREACH (h4_file ${HDF4_HDF2GIF_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     hdf2gif
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_HDF2GIF_TEST_FILES})

#-- Adding utility gif2hdf

SET (gif2hdf_SRCS
    ${HDF4_HDF_SOURCE_DIR}/util/gif2hdf.c
    ${HDF4_HDF_SOURCE_DIR}/util/gif2mem.c
    ${HDF4_HDF_SOURCE_DIR}/util/gifread.c
    ${HDF4_HDF_SOURCE_DIR}/util/decompress.c
    ${HDF4_HDF_SOURCE_DIR}/util/writehdf.c
)
  
ADD_EXECUTABLE (gif2hdf ${gif2hdf_SRCS})
TARGET_NAMING (gif2hdf)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (gif2hdf PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (gif2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testgif2hdf-sunwheel COMMAND $<TARGET_FILE:gif2hdf> SunWheel.gif SunWheel.hdf)
ADD_TEST (NAME testgif2hdf-butterfly COMMAND $<TARGET_FILE:gif2hdf> bttrfly.gif bttrfly.hdf)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_GIF2HDF_TEST_FILES
    SunWheel.gif
    bttrfly.gif
)
FOREACH (h4_file ${HDF4_GIF2HDF_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     gif2hdf
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_GIF2HDF_TEST_FILES})

#-- Adding utility hdf24to8
ADD_EXECUTABLE (hdf24to8 ${HDF4_HDF_SOURCE_DIR}/util/hdf24to8.c)
TARGET_NAMING (hdf24to8)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdf24to8 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdf24to8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testhdf24to8 COMMAND $<TARGET_FILE:hdf24to8> head.r24 head8.hdf)

#-- Adding utility hdftor8
ADD_EXECUTABLE (hdftor8 ${HDF4_HDF_SOURCE_DIR}/util/hdftor8.c)
TARGET_NAMING (hdftor8)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdftor8 PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdftor8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testhdftor8 COMMAND $<TARGET_FILE:hdftor8> head8.hdf)
#ADD_TEST (cmp "img001-263.328 head.r8")
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_HDFTOR8_TEST_FILES
    head.r24
    head.r8
)
FOREACH (h4_file ${HDF4_HDFTOR8_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     hdftor8
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_HDFTOR8_TEST_FILES})

#-- Adding utility hdfed

SET (hdfed_SRCS
    ${HDF4_HDF_SOURCE_DIR}/util/he_cntrl.c
    ${HDF4_HDF_SOURCE_DIR}/util/he_disp.c
    ${HDF4_HDF_SOURCE_DIR}/util/he_file.c
    ${HDF4_HDF_SOURCE_DIR}/util/he_main.c
)
  
ADD_EXECUTABLE (hdfed ${hdfed_SRCS})
TARGET_NAMING (hdfed)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfed PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdfed ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME testhdfed COMMAND $<TARGET_FILE:hdfed> "< hdfed.input1 > hdfed.tmp 2>&1")
#ADD_TEST (diff  "hdfed.tmp ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfed.out1")
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_HDFED_TEST_FILES
    storm110.hdf
    ntcheck.hdf
)
FOREACH (h4_file ${HDF4_HDFED_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     hdfed
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_HDFED_TEST_FILES})

#-- Adding utility ristosds
ADD_EXECUTABLE (ristosds ${HDF4_HDF_SOURCE_DIR}/util/ristosds.c)
TARGET_NAMING (ristosds)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (ristosds  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (ristosds ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME testristosds COMMAND $<TARGET_FILE:ristosds> storm*.hdf -o storm.hdf > /dev/null 2>&1)
#ADD_TEST (NAME hdfed COMMAND $<TARGET_FILE:hdfed> "< ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.input1 > hdfed.tmp1 2>&1")
#ADD_TEST (diff  "hdfed.tmp1 ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.out1")
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_RISTOSDS_TEST_FILES
    storm110.hdf
    storm120.hdf
    storm130.hdf
)
FOREACH (h4_file ${HDF4_RISTOSDS_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     ristosds
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_RISTOSDS_TEST_FILES})

#-- Adding utility hdfpack
ADD_EXECUTABLE (hdfpack ${HDF4_HDF_SOURCE_DIR}/util/hdfpack.c)
TARGET_NAMING (hdfpack)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfpack PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdfpack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testhdfpack COMMAND $<TARGET_FILE:hdfpack> test.hdf test.pck)
ADD_TEST (NAME testhdfpack-block COMMAND $<TARGET_FILE:hdfpack> -b test.hdf test.blk)
IF (WIN32)
#  ADD_TEST (hdfls test.hdf | findstr /v "library String" ) > hdfls.tmp1 2>&1)
#  ADD_TEST (hdfls test.pck | findstr /v "library String" ) >> hdfls.tmp1 2>&1)
#  ADD_TEST (fc hdfls.tmp1 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.out1 > nul)
ELSE (WIN32)
#  ADD_TEST (hdfls test.hdf | $SED ) > hdfls.tmp1 2>&1)
#  ADD_TEST (hdfls test.pck | $SED ) >> hdfls.tmp1 2>&1)
#  ADD_TEST (diff hdfls.tmp1 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.out1)
ENDIF (WIN32)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_HDFPACK_TEST_FILES
    test.hdf
)
FOREACH (h4_file ${HDF4_HDFPACK_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     hdfpack
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_HDFPACK_TEST_FILES})

#-- Adding utility paltohdf
ADD_EXECUTABLE (paltohdf ${HDF4_HDF_SOURCE_DIR}/util/paltohdf.c)
TARGET_NAMING (paltohdf)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (paltohdf PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (paltohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#-- Adding utility hdftopal
ADD_EXECUTABLE (hdftopal ${HDF4_HDF_SOURCE_DIR}/util/hdftopal.c)
TARGET_NAMING (hdftopal)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdftopal PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdftopal ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testpaltohdf COMMAND $<TARGET_FILE:paltohdf> palette.raw palette.hdf)
ADD_TEST (NAME testhdftopal COMMAND $<TARGET_FILE:hdftopal> palette.hdf palette.raw.new)
#ADD_TEST (cmp "palette.raw palette.raw.new")
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_PALTOHDF_TEST_FILES
    palette.raw
)
FOREACH (h4_file ${HDF4_PALTOHDF_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     paltohdf
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_PALTOHDF_TEST_FILES})

#-- Adding utility r8tohdf
ADD_EXECUTABLE (r8tohdf ${HDF4_HDF_SOURCE_DIR}/util/r8tohdf.c)
TARGET_NAMING (r8tohdf)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (r8tohdf  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (r8tohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testr8tohdf-storm COMMAND $<TARGET_FILE:r8tohdf> 57 57 storm.hdf storm110.raw storm120.raw storm130.raw storm140.raw)
ADD_TEST (NAME testr8tohdf-palette COMMAND $<TARGET_FILE:r8tohdf> 57 57 storm.hdf -p palette.raw -i storm110.raw)
ADD_TEST (NAME testhdftor8-storm COMMAND $<TARGET_FILE:hdftor8> storm.hdf)
IF (WIN32)
#  ADD_TEST (hdfls -l storm.hdf | findstr /v "library String") > hdfls.tmp2 2>&1)
#  ADD_TEST (fc hdfls.tmp2 ${HDF4_HDF_SOURCE_DIR}/hdftor8.out1 > nul)
ELSE (WIN32)
#  ADD_TEST (hdfls -l storm.hdf | $SED) > hdfls.tmp2 2>&1)
#  ADD_TEST (diff hdfls.tmp2 ${HDF4_HDF_SOURCE_DIR}/hdftor8.out1)
ENDIF (WIN32)
#ADD_TEST (cmp img001-057.057  storm110.raw)
#ADD_TEST (cmp img002-057.057  storm120.raw)
#ADD_TEST (cmp img003-057.057  storm130.raw)
#ADD_TEST (cmp img004-057.057  storm140.raw)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_R8TOHDF_TEST_FILES
    storm110.raw
    storm120.raw
    storm130.raw
    storm140.raw
)
FOREACH (h4_file ${HDF4_R8TOHDF_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     r8tohdf
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )
ENDFOREACH (h4_file ${HDF4_R8TOHDF_TEST_FILES})

#-- Adding utility hdfcomp
ADD_EXECUTABLE (hdfcomp ${HDF4_HDF_SOURCE_DIR}/util/hdfcomp.c)
TARGET_NAMING (hdfcomp)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfcomp  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdfcomp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testhdfcomp-storms COMMAND $<TARGET_FILE:hdfcomp> allstorms.hdf storm110.hdf storm120.hdf storm130.hdf)
ADD_TEST (NAME testhdfcomp COMMAND $<TARGET_FILE:hdfcomp> allcomp.hdf storm110.hdf storm120.hdf storm130.hdf)
IF (WIN32)
#  ADD_TEST (hdfls -l allstorms.hdf | findstr /v "library String" ) > hdfls.tmp3 2>&1)
#  ADD_TEST (hdfls -l allcomp.hdf | findstr /v "library String" ) >> hdfls.tmp3 2>&1)
#  ADD_TEST (fc hdfls.tmp3 ${HDF4_HDF_SOURCE_DIR}/hdfcomp.out1 > nul)
ELSE (WIN32)
#  ADD_TEST (hdfls -l allstorms.hdf | $SED) > hdfls.tmp3 2>&1)
#  ADD_TEST (hdfls -l allcomp.hdf | $SED) >> hdfls.tmp3 2>&1)
#  ADD_TEST (diff hdfls.tmp3 ${HDF4_HDF_SOURCE_DIR}/hdfcomp.out1)
ENDIF (WIN32)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_HDFCOMP_TEST_FILES
    storm110.hdf
    storm120.hdf
    storm130.hdf
)
FOREACH (h4_file ${HDF4_HDFCOMP_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     hdfcomp
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )

ENDFOREACH (h4_file ${HDF4_HDFCOMP_TEST_FILES})

#-- Adding utility jpeg2hdf
ADD_EXECUTABLE (jpeg2hdf ${HDF4_HDF_SOURCE_DIR}/util/jpeg2hdf.c)
TARGET_NAMING (jpeg2hdf)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (jpeg2hdf  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (jpeg2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#-- Adding utility hdf2jpeg
ADD_EXECUTABLE (hdf2jpeg ${HDF4_HDF_SOURCE_DIR}/util/hdf2jpeg.c)
TARGET_NAMING (hdf2jpeg)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdf2jpeg PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdf2jpeg ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

ADD_TEST (NAME testjpeg2hdf COMMAND $<TARGET_FILE:jpeg2hdf> jpeg_img.jpg jpeg.hdf)
ADD_TEST (NAME testhdf2jpeg COMMAND $<TARGET_FILE:hdf2jpeg> jpeg.hdf jpeg2.jpg)
IF (WIN32)
#  ADD_TEST (hdfls -l jpeg.hdf | findstr /v "library String" ) > hdfls.tmp4 2>&1)
#  ADD_TEST (fc hdfls.tmp4 ${HDF4_HDF_SOURCE_DIR}/jpeg2hdf.out1)
#  ADD_TEST (fc jpeg_img.jpg jpeg2.jpg)
ELSE (WIN32)
#  ADD_TEST (hdfls -l jpeg.hdf | $SED) > hdfls.tmp4 2>&1)
#  ADD_TEST (diff hdfls.tmp4 ${HDF4_HDF_SOURCE_DIR}/jpeg2hdf.out1)
#  ADD_TEST (cmp jpeg_img.jpg jpeg2.jpg)
ENDIF (WIN32)
#-- Copy all the data files from the test directory into the source directory
SET (HDF4_JPEG2HDF_TEST_FILES
    jpeg_img.jpg
)
FOREACH (h4_file ${HDF4_JPEG2HDF_TEST_FILES})
   SET (dest "${PROJECT_BINARY_DIR}/${h4_file}")
   #MESSAGE (STATUS " Copying ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} to ${PROJECT_BINARY_DIR}/")
   ADD_CUSTOM_COMMAND (
       TARGET     jpeg2hdf
       POST_BUILD
       COMMAND    ${CMAKE_COMMAND}
       ARGS       -E copy_if_different ${HDF4_HDF_UTIL_SOURCE_DIR}/testfiles/${h4_file} ${dest}
   )
ENDFOREACH (h4_file ${HDF4_JPEG2HDF_TEST_FILES})

#-- Adding utility hdf8to24
ADD_EXECUTABLE (hdf8to24 ${HDF4_HDF_SOURCE_DIR}/util/hdf8to24.c)
TARGET_NAMING (hdf8to24)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdf8to24  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdf8to24 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME hdf8to24 COMMAND $<TARGET_FILE:hdf8to24>)

#-- Adding utility hdfunpac
ADD_EXECUTABLE (hdfunpac ${HDF4_HDF_SOURCE_DIR}/util/hdfunpac.c)
TARGET_NAMING (hdfunpac)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (hdfunpac  PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (hdfunpac ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME hdfunpac COMMAND $<TARGET_FILE:hdfunpac>)

#-- Adding utility vmake
ADD_EXECUTABLE (vmake ${HDF4_HDF_SOURCE_DIR}/util/vmake.c)
TARGET_NAMING (vmake)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (vmake PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (vmake ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME vmake COMMAND $<TARGET_FILE:vmake>)

#-- Adding utility vshow
ADD_EXECUTABLE (vshow ${HDF4_HDF_SOURCE_DIR}/util/vshow.c)
TARGET_NAMING (vshow)
IF (WIN32)
  IF (MSVC)
    IF (NOT BUILD_SHARED_LIBS)
      SET_TARGET_PROPERTIES (vshow PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT") 
    ENDIF (NOT BUILD_SHARED_LIBS)
  ENDIF (MSVC)
ENDIF (WIN32)
TARGET_LINK_LIBRARIES (vshow ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

#ADD_TEST (NAME vshow COMMAND $<TARGET_FILE:vshow>)
