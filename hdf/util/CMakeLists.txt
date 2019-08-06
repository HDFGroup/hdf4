cmake_minimum_required (VERSION 3.10)
project (HDF4_HDF_UTIL C)

if (HDF4_BUILD_TOOLS)
  #-- Adding tool hdfls
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdfls ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
    target_include_directories(hdfls PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfls STATIC)
    target_link_libraries (hdfls PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_EXECUTABLES hdfls)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfls-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
    target_include_directories(hdfls-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfls-shared SHARED)
    target_link_libraries (hdfls-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfls-shared)
  endif ()

  #-- Adding utility hdfed
  set (hdfed_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_cntrl.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_disp.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_file.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_main.c
  )

  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdfed ${hdfed_SRCS})
    target_include_directories(hdfed PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfed STATIC)
    target_link_libraries (hdfed PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfed)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfed-shared ${hdfed_SRCS})
    target_include_directories(hdfed-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfed-shared SHARED)
    target_link_libraries (hdfed-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfed-shared)
  endif ()
endif ()

if (HDF4_BUILD_UTILS)
  #-- Adding utility hdf2gif
  set (hdf2gif_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2gif.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfgifwr.c
  )

  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdf2gif ${hdf2gif_SRCS})
    target_include_directories(hdf2gif PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf2gif STATIC)
    target_link_libraries (hdf2gif PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES hdf2gif)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf2gif-shared ${hdf2gif_SRCS})
    target_include_directories(hdf2gif-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf2gif-shared SHARED)
    target_link_libraries (hdf2gif-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2gif-shared)
  endif ()

  #-- Adding utility gif2hdf
  set (gif2hdf_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2hdf.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2mem.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gifread.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/decompress.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/writehdf.c
  )

  if (NOT ONLY_SHARED_LIBS)
    add_executable (gif2hdf ${gif2hdf_SRCS})
    target_include_directories(gif2hdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (gif2hdf STATIC)
    target_link_libraries (gif2hdf PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} gif2hdf)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (gif2hdf-shared ${gif2hdf_SRCS})
    target_include_directories(gif2hdf-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (gif2hdf-shared SHARED)
    target_link_libraries (gif2hdf-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} gif2hdf-shared)
  endif ()

  #-- Adding utility hdf24to8
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdf24to8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
    target_include_directories(hdf24to8 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf24to8 STATIC)
    target_link_libraries (hdf24to8 PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf24to8)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf24to8-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
    target_include_directories(hdf24to8-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf24to8-shared SHARED)
    target_link_libraries (hdf24to8-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf24to8-shared)
  endif ()

  #-- Adding utility hdftor8
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdftor8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
    target_include_directories(hdftor8 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdftor8 STATIC)
    target_link_libraries (hdftor8 PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftor8)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdftor8-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
    target_include_directories(hdftor8-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdftor8-shared SHARED)
    target_link_libraries (hdftor8-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftor8-shared)
  endif ()

  #-- Adding utility ristosds
  if (NOT ONLY_SHARED_LIBS)
    add_executable (ristosds ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
    target_include_directories(ristosds PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (ristosds STATIC)
    target_link_libraries (ristosds PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} ristosds)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (ristosds-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
    target_include_directories(ristosds-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (ristosds-shared SHARED)
    target_link_libraries (ristosds-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} ristosds-shared)
  endif ()

  #-- Adding utility hdfpack
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdfpack ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
    target_include_directories(hdfpack PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfpack STATIC)
    target_link_libraries (hdfpack PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfpack)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfpack-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
    target_include_directories(hdfpack-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfpack-shared SHARED)
    target_link_libraries (hdfpack-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfpack-shared)
  endif ()

  #-- Adding utility paltohdf
  if (NOT ONLY_SHARED_LIBS)
    add_executable (paltohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
    target_include_directories(paltohdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (paltohdf STATIC)
    target_link_libraries (paltohdf PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} paltohdf)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (paltohdf-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
    target_include_directories(paltohdf-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (paltohdf-shared SHARED)
    target_link_libraries (paltohdf-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} paltohdf-shared)
  endif ()

  #-- Adding utility hdftopal
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdftopal ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
    target_include_directories(hdftopal PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdftopal STATIC)
    target_link_libraries (hdftopal PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftopal)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdftopal-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
    target_include_directories(hdftopal-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdftopal-shared SHARED)
    target_link_libraries (hdftopal-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftopal-shared)
  endif ()

  #-- Adding utility r8tohdf
  if (NOT ONLY_SHARED_LIBS)
    add_executable (r8tohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
    target_include_directories(r8tohdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (r8tohdf STATIC)
    target_link_libraries (r8tohdf PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} r8tohdf)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (r8tohdf-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
    target_include_directories(r8tohdf-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (r8tohdf-shared SHARED)
    target_link_libraries (r8tohdf-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} r8tohdf-shared)
  endif ()

  #-- Adding utility hdfcomp
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdfcomp ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
    target_include_directories(hdfcomp PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfcomp STATIC)
    target_link_libraries (hdfcomp PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfcomp)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfcomp-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
    target_include_directories(hdfcomp-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfcomp-shared SHARED)
    target_link_libraries (hdfcomp-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfcomp-shared)
  endif ()

  #-- Adding utility jpeg2hdf
  if (NOT ONLY_SHARED_LIBS)
    add_executable (jpeg2hdf ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
    target_include_directories(jpeg2hdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (jpeg2hdf STATIC)
    target_link_libraries (jpeg2hdf PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} jpeg2hdf)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (jpeg2hdf-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
    target_include_directories(jpeg2hdf-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (jpeg2hdf-shared SHARED)
    target_link_libraries (jpeg2hdf-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} jpeg2hdf-shared)
  endif ()

  #-- Adding utility hdf2jpeg
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdf2jpeg ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
    target_include_directories(hdf2jpeg PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf2jpeg STATIC)
    target_link_libraries (hdf2jpeg PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2jpeg)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf2jpeg-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
    target_include_directories(hdf2jpeg-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf2jpeg-shared SHARED)
    target_link_libraries (hdf2jpeg-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2jpeg-shared)
  endif ()

  #-- Adding utility hdf8to24
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdf8to24 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
    target_include_directories(hdf8to24 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf8to24 STATIC)
    target_link_libraries (hdf8to24 PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf8to24)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf8to24-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
    target_include_directories(hdf8to24-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdf8to24-shared SHARED)
    target_link_libraries (hdf8to24-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf8to24-shared)
  endif ()

  #-- Adding utility hdfunpac
  if (NOT ONLY_SHARED_LIBS)
    add_executable (hdfunpac ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
    target_include_directories(hdfunpac PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfunpac STATIC)
    target_link_libraries (hdfunpac PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfunpac)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfunpac-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
    target_include_directories(hdfunpac-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (hdfunpac-shared SHARED)
    target_link_libraries (hdfunpac-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfunpac-shared)
  endif ()

  #-- Adding utility vmake
  if (NOT ONLY_SHARED_LIBS)
    add_executable (vmake ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
    target_include_directories(vmake PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (vmake STATIC)
    target_link_libraries (vmake PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vmake)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (vmake-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
    target_include_directories(vmake-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (vmake-shared SHARED)
    target_link_libraries (vmake-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vmake-shared)
  endif ()

  #-- Adding utility vshow
  if (NOT ONLY_SHARED_LIBS)
    add_executable (vshow ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
    target_include_directories(vshow PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (vshow STATIC)
    target_link_libraries (vshow PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vshow)
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (vshow-shared ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
    target_include_directories(vshow-shared PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR}")
    TARGET_C_PROPERTIES (vshow-shared SHARED)
    target_link_libraries (vshow-shared PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vshow-shared)
  endif ()
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
  foreach (exec ${H4_DEP_EXECUTABLES})
    INSTALL_PROGRAM_PDB (${exec} ${HDF4_INSTALL_TOOLS_BIN_DIR} toolsapplications)
  endforeach ()

  INSTALL (
      TARGETS ${H4_DEP_EXECUTABLES}
      RUNTIME DESTINATION ${HDF4_INSTALL_TOOLS_BIN_DIR} COMPONENT toolsapplications
  )
endif ()

if (HDF4_BUILD_UTILS)
  foreach (exec ${H4_DEP_UTILITIES})
    INSTALL_PROGRAM_PDB (${exec} ${HDF4_INSTALL_UTILS_BIN_DIR} toolsapplications)
  endforeach ()

  INSTALL (
      TARGETS ${H4_DEP_UTILITIES}
      RUNTIME DESTINATION ${HDF4_INSTALL_UTILS_BIN_DIR} COMPONENT utilsapplications
  )
endif ()
