cmake_minimum_required (VERSION 2.8.10)
PROJECT (HDF4_HDF_UTIL C CXX)

INCLUDE_DIRECTORIES (${HDF4_HDFSOURCE_DIR})

if (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
endif (WIN32)

if (HDF4_BUILD_TOOLS)
  #-- Adding tool hdfls
  ADD_EXECUTABLE (hdfls ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
  TARGET_NAMING (hdfls ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdfls " " " ")
  TARGET_LINK_LIBRARIES (hdfls ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfed
  set (hdfed_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_cntrl.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_disp.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_file.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_main.c
  )
  
  ADD_EXECUTABLE (hdfed ${hdfed_SRCS})
  TARGET_NAMING (hdfed ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdfed " " " ")
  TARGET_LINK_LIBRARIES (hdfed ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_TOOLS)

if (HDF4_BUILD_UTILS)
  #-- Adding utility hdf2gif
  set (hdf2gif_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2gif.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfgifwr.c
  )
  
  ADD_EXECUTABLE (hdf2gif ${hdf2gif_SRCS})
  TARGET_NAMING (hdf2gif ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdf2gif " " " ")
  TARGET_LINK_LIBRARIES (hdf2gif ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility gif2hdf
  set (gif2hdf_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2hdf.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2mem.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gifread.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/decompress.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/writehdf.c
 )
  
  ADD_EXECUTABLE (gif2hdf ${gif2hdf_SRCS})
  TARGET_NAMING (gif2hdf ${LIB_TYPE})
  TARGET_C_PROPERTIES (gif2hdf " " " ")
  TARGET_LINK_LIBRARIES (gif2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf24to8
  ADD_EXECUTABLE (hdf24to8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
  TARGET_NAMING (hdf24to8 ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdf24to8 " " " ")
  TARGET_LINK_LIBRARIES (hdf24to8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdftor8
  ADD_EXECUTABLE (hdftor8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
  TARGET_NAMING (hdftor8 ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdftor8 " " " ")
  TARGET_LINK_LIBRARIES (hdftor8 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility ristosds
  ADD_EXECUTABLE (ristosds ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
  TARGET_NAMING (ristosds ${LIB_TYPE})
  TARGET_C_PROPERTIES (ristosds " " " ")
  TARGET_LINK_LIBRARIES (ristosds ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfpack
  ADD_EXECUTABLE (hdfpack ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
  TARGET_NAMING (hdfpack ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdfpack " " " ")
  TARGET_LINK_LIBRARIES (hdfpack ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility paltohdf
  ADD_EXECUTABLE (paltohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
  TARGET_NAMING (paltohdf ${LIB_TYPE})
  TARGET_C_PROPERTIES (paltohdf " " " ")
  TARGET_LINK_LIBRARIES (paltohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdftopal
  ADD_EXECUTABLE (hdftopal ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
  TARGET_NAMING (hdftopal ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdftopal " " " ")
  TARGET_LINK_LIBRARIES (hdftopal ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility r8tohdf
  ADD_EXECUTABLE (r8tohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
  TARGET_NAMING (r8tohdf ${LIB_TYPE})
  TARGET_C_PROPERTIES (r8tohdf " " " ")
  TARGET_LINK_LIBRARIES (r8tohdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfcomp
  ADD_EXECUTABLE (hdfcomp ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
  TARGET_NAMING (hdfcomp ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdfcomp " " " ")
  TARGET_LINK_LIBRARIES (hdfcomp ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility jpeg2hdf
  ADD_EXECUTABLE (jpeg2hdf ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
  TARGET_NAMING (jpeg2hdf ${LIB_TYPE})
  TARGET_C_PROPERTIES (jpeg2hdf " " " ")
  TARGET_LINK_LIBRARIES (jpeg2hdf ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf2jpeg
  ADD_EXECUTABLE (hdf2jpeg ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
  TARGET_NAMING (hdf2jpeg ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdf2jpeg " " " ")
  TARGET_LINK_LIBRARIES (hdf2jpeg ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdf8to24
  ADD_EXECUTABLE (hdf8to24 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
  TARGET_NAMING (hdf8to24 ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdf8to24 " " " ")
  TARGET_LINK_LIBRARIES (hdf8to24 ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility hdfunpac
  ADD_EXECUTABLE (hdfunpac ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
  TARGET_NAMING (hdfunpac ${LIB_TYPE})
  TARGET_C_PROPERTIES (hdfunpac " " " ")
  TARGET_LINK_LIBRARIES (hdfunpac ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility vmake
  ADD_EXECUTABLE (vmake ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
  TARGET_NAMING (vmake ${LIB_TYPE})
  TARGET_C_PROPERTIES (vmake " " " ")
  TARGET_LINK_LIBRARIES (vmake ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})

  #-- Adding utility vshow
  ADD_EXECUTABLE (vshow ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
  TARGET_NAMING (vshow ${LIB_TYPE})
  TARGET_C_PROPERTIES (vshow " " " ")
  TARGET_LINK_LIBRARIES (vshow ${HDF4_MF_LIB_TARGET} ${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
endif (HDF4_BUILD_UTILS)

if (BUILD_TESTING AND HDF4_BUILD_UTILS)
  include (CMakeTests.cmake)
endif (BUILD_TESTING AND HDF4_BUILD_UTILS)

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
      RUNTIME DESTINATION
          ${HDF4_INSTALL_TOOLS_BIN_DIR}
      COMPONENT
          toolsapplications
  )
endif (HDF4_BUILD_TOOLS)

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
      RUNTIME DESTINATION
          ${HDF4_INSTALL_UTILS_BIN_DIR}
      COMPONENT
          utilsapplications
  )
endif (HDF4_BUILD_UTILS)
