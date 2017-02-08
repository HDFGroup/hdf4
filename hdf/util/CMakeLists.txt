cmake_minimum_required (VERSION 3.2.2)
project (HDF4_HDF_UTIL C CXX)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})

if (WIN32)
  add_definitions (-DDOS_FS)
endif ()

if (HDF4_BUILD_TOOLS)
  #-- Adding tool hdfls
  add_executable (hdfls ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
  TARGET_NAMING (hdfls STATIC)
  TARGET_C_PROPERTIES (hdfls STATIC " " " ")
  target_link_libraries (hdfls ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfed
  set (hdfed_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_cntrl.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_disp.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_file.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_main.c
  )

  add_executable (hdfed ${hdfed_SRCS})
  TARGET_NAMING (hdfed STATIC)
  TARGET_C_PROPERTIES (hdfed STATIC " " " ")
  target_link_libraries (hdfed ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()

if (HDF4_BUILD_UTILS)
  #-- Adding utility hdf2gif
  set (hdf2gif_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2gif.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfgifwr.c
  )

  add_executable (hdf2gif ${hdf2gif_SRCS})
  TARGET_NAMING (hdf2gif STATIC)
  TARGET_C_PROPERTIES (hdf2gif STATIC " " " ")
  target_link_libraries (hdf2gif ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility gif2hdf
  set (gif2hdf_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2hdf.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2mem.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gifread.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/decompress.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/writehdf.c
 )

  add_executable (gif2hdf ${gif2hdf_SRCS})
  TARGET_NAMING (gif2hdf STATIC)
  TARGET_C_PROPERTIES (gif2hdf STATIC " " " ")
  target_link_libraries (gif2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf24to8
  add_executable (hdf24to8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
  TARGET_NAMING (hdf24to8 STATIC)
  TARGET_C_PROPERTIES (hdf24to8 STATIC " " " ")
  target_link_libraries (hdf24to8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdftor8
  add_executable (hdftor8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
  TARGET_NAMING (hdftor8 STATIC)
  TARGET_C_PROPERTIES (hdftor8 STATIC " " " ")
  target_link_libraries (hdftor8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility ristosds
  add_executable (ristosds ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
  TARGET_NAMING (ristosds STATIC)
  TARGET_C_PROPERTIES (ristosds STATIC " " " ")
  target_link_libraries (ristosds ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfpack
  add_executable (hdfpack ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
  TARGET_NAMING (hdfpack STATIC)
  TARGET_C_PROPERTIES (hdfpack STATIC " " " ")
  target_link_libraries (hdfpack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility paltohdf
  add_executable (paltohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
  TARGET_NAMING (paltohdf STATIC)
  TARGET_C_PROPERTIES (paltohdf STATIC " " " ")
  target_link_libraries (paltohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdftopal
  add_executable (hdftopal ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
  TARGET_NAMING (hdftopal STATIC)
  TARGET_C_PROPERTIES (hdftopal STATIC " " " ")
  target_link_libraries (hdftopal ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility r8tohdf
  add_executable (r8tohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
  TARGET_NAMING (r8tohdf STATIC)
  TARGET_C_PROPERTIES (r8tohdf STATIC " " " ")
  target_link_libraries (r8tohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfcomp
  add_executable (hdfcomp ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
  TARGET_NAMING (hdfcomp STATIC)
  TARGET_C_PROPERTIES (hdfcomp STATIC " " " ")
  target_link_libraries (hdfcomp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility jpeg2hdf
  add_executable (jpeg2hdf ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
  TARGET_NAMING (jpeg2hdf STATIC)
  TARGET_C_PROPERTIES (jpeg2hdf STATIC " " " ")
  target_link_libraries (jpeg2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf2jpeg
  add_executable (hdf2jpeg ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
  TARGET_NAMING (hdf2jpeg STATIC)
  TARGET_C_PROPERTIES (hdf2jpeg STATIC " " " ")
  target_link_libraries (hdf2jpeg ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf8to24
  add_executable (hdf8to24 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
  TARGET_NAMING (hdf8to24 STATIC)
  TARGET_C_PROPERTIES (hdf8to24 STATIC " " " ")
  target_link_libraries (hdf8to24 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfunpac
  add_executable (hdfunpac ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
  TARGET_NAMING (hdfunpac STATIC)
  TARGET_C_PROPERTIES (hdfunpac STATIC " " " ")
  target_link_libraries (hdfunpac ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility vmake
  add_executable (vmake ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
  TARGET_NAMING (vmake STATIC)
  TARGET_C_PROPERTIES (vmake STATIC " " " ")
  target_link_libraries (vmake ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility vshow
  add_executable (vshow ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
  TARGET_NAMING (vshow STATIC)
  TARGET_C_PROPERTIES (vshow STATIC " " " ")
  target_link_libraries (vshow ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif ()

if (BUILD_TESTING AND HDF4_BUILD_UTILS)
  include (CMakeTests.cmake)
endif ()

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
if (HDF4_BUILD_TOOLS)
  INSTALL_PROGRAM_PDB (hdfls ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)
  INSTALL_PROGRAM_PDB (hdfed ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)

  INSTALL (
      TARGETS
          hdfls
          hdfed
      RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
  )
endif ()

if (HDF4_BUILD_UTILS)
  INSTALL_PROGRAM_PDB (gif2hdf ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdf2gif ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdf2jpeg ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdf24to8 ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdf8to24 ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdfcomp ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdfpack ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdftopal ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdftor8 ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (hdfunpac ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (jpeg2hdf ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (paltohdf ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (r8tohdf ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (ristosds ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (vmake ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)
  INSTALL_PROGRAM_PDB (vshow ${HDF4_INSTALL_UTILS_BIN_DIR} utilsapplications)

  INSTALL (
      TARGETS
          gif2hdf
          hdf2gif
          hdf2jpeg
          hdf24to8
          hdf8to24
          hdfcomp
          hdfpack
          hdftopal
          hdftor8
          hdfunpac
          jpeg2hdf
          paltohdf
          r8tohdf
          ristosds
          vmake
          vshow
      RUNTIME DESTINATION ${HDF4_INSTALL_UTILS_BIN_DIR} COMPONENT utilsapplications
  )
endif ()
