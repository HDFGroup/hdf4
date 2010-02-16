PROJECT ( H4_HDF_UTIL)

# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Libraries"
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all Executables."
  )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/Bin
  CACHE PATH
  "Single Directory for all static libraries."
  )

INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR}/lib )

IF(WIN32)
  ADD_DEFINITIONS(-D_HDFLIB_ -DDOS_FS)
ENDIF(WIN32)

#-- Adding utility gif2hdf

SET (gif2hdf_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/gif2hdf.c
  ${HDF4_HDF_SOURCE_DIR}/util/gif2mem.c
  ${HDF4_HDF_SOURCE_DIR}/util/gifread.c
  ${HDF4_HDF_SOURCE_DIR}/util/decompress.c
  ${HDF4_HDF_SOURCE_DIR}/util/writehdf.c
  )
  
ADD_EXECUTABLE(gif2hdf ${gif2hdf_SRCS})
TARGET_LINK_LIBRARIES(gif2hdf ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(gif2hdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gif2hdf)

#-- Adding utility hdf2gif

SET (hdf2gif_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdf2gif.c
  ${HDF4_HDF_SOURCE_DIR}/util/hdfgifwr.c
  )
  
ADD_EXECUTABLE(hdf2gif ${hdf2gif_SRCS})
TARGET_LINK_LIBRARIES(hdf2gif ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdf2gif ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdf2gif)

#-- Adding utility hdf2jpeg

SET (hdf2jpeg_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdf2jpeg.c
  )
  
ADD_EXECUTABLE(hdf2jpeg ${hdf2jpeg_SRCS})
TARGET_LINK_LIBRARIES(hdf2jpeg ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdf2jpeg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdf2jpeg)

#-- Adding utility hdf24to8

SET (hdf24to8_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdf24to8.c
  )
  
ADD_EXECUTABLE(hdf24to8 ${hdf24to8_SRCS})
TARGET_LINK_LIBRARIES(hdf24to8 ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdf24to8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdf24to8)

#-- Adding utility hdf8to24

SET (hdf8to24_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdf8to24.c
  )
  
ADD_EXECUTABLE(hdf8to24 ${hdf8to24_SRCS})
TARGET_LINK_LIBRARIES(hdf8to24 ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdf8to24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdf8to24)

#-- Adding utility hdfcomp

SET (hdfcomp_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdfcomp.c
  )
  
ADD_EXECUTABLE(hdfcomp ${hdfcomp_SRCS})
TARGET_LINK_LIBRARIES(hdfcomp ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdfcomp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfcomp)

#-- Adding utility hdfed

SET (hdfed_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/he_cntrl.c
  ${HDF4_HDF_SOURCE_DIR}/util/he_disp.c
  ${HDF4_HDF_SOURCE_DIR}/util/he_file.c
  ${HDF4_HDF_SOURCE_DIR}/util/he_main.c
  )
  
ADD_EXECUTABLE(hdfed ${hdfed_SRCS})
TARGET_LINK_LIBRARIES(hdfed ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdfed ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfed)

#-- Adding utility hdfls

SET (hdfls_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdfls.c
  )
  
ADD_EXECUTABLE(hdfls ${hdfls_SRCS})
TARGET_LINK_LIBRARIES(hdfls ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdfls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfls)

#-- Adding utility hdfpack

SET (hdfpack_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdfpack.c
  )
  
ADD_EXECUTABLE(hdfpack ${hdfpack_SRCS})
TARGET_LINK_LIBRARIES(hdfpack ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdfpack ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfpack)

#-- Adding utility hdftopal

SET (hdftopal_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdftopal.c
  )
  
ADD_EXECUTABLE(hdftopal ${hdftopal_SRCS})
TARGET_LINK_LIBRARIES(hdftopal ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdftopal ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdftopal)

#-- Adding utility hdftor8

SET (hdftor8_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdftor8.c
  )
  
ADD_EXECUTABLE(hdftor8 ${hdftor8_SRCS})
TARGET_LINK_LIBRARIES(hdftor8 ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdftor8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdftor8)

#-- Adding utility hdfunpac

SET (hdfunpac_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/hdfunpac.c
  )
  
ADD_EXECUTABLE(hdfunpac ${hdfunpac_SRCS})
TARGET_LINK_LIBRARIES(hdfunpac ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(hdfunpac ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/hdfunpac)

#-- Adding utility jpeg2hdf

SET (jpeg2hdf_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/jpeg2hdf.c
  )
  
ADD_EXECUTABLE(jpeg2hdf ${jpeg2hdf_SRCS})
TARGET_LINK_LIBRARIES(jpeg2hdf ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(jpeg2hdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/jpeg2hdf)

#-- Adding utility paltohdf

SET (paltohdf_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/paltohdf.c
  )
  
ADD_EXECUTABLE(paltohdf ${paltohdf_SRCS})
TARGET_LINK_LIBRARIES(paltohdf ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(paltohdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/paltohdf)

#-- Adding utility r8tohdf

SET (r8tohdf_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/r8tohdf.c
  )
  
ADD_EXECUTABLE(r8tohdf ${r8tohdf_SRCS})
TARGET_LINK_LIBRARIES(r8tohdf ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(r8tohdf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/r8tohdf)

#-- Adding utility ristosds

SET (ristosds_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/ristosds.c
  )
  
ADD_EXECUTABLE(ristosds ${ristosds_SRCS})
TARGET_LINK_LIBRARIES(ristosds ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(ristosds ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ristosds)

#-- Adding utility vmake

SET (vmake_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/vmake.c
  )
  
ADD_EXECUTABLE(vmake ${vmake_SRCS})
TARGET_LINK_LIBRARIES(vmake ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(vmake ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vmake)

#-- Adding utility vshow

SET (vshow_SRCS
  ${HDF4_HDF_SOURCE_DIR}/util/vshow.c
  )
  
ADD_EXECUTABLE(vshow ${vshow_SRCS})
TARGET_LINK_LIBRARIES(vshow ${HDF4_MF_LIB_NAME}-static ${HDF4_SRC_LIB_NAME}-static ${LINK_LIBS} )

#ADD_TEST(vshow ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vshow)

