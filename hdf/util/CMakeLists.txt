cmake_minimum_required (VERSION 3.18)
project (HDF4_HDF_UTIL C)

if (HDF4_BUILD_TOOLS)
  #-- Adding tool hdfls
  add_executable (hdfls ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
  target_include_directories(hdfls PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdfls STATIC)
    target_link_libraries (hdfls PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdfls SHARED)
    target_link_libraries (hdfls PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_EXECUTABLES hdfls)

  #-- Adding utility hdfed
  set (hdfed_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_cntrl.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_disp.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_file.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_main.c
  )

  add_executable (hdfed ${hdfed_SRCS})
  target_include_directories(hdfed PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdfed STATIC)
    target_link_libraries (hdfed PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdfed SHARED)
    target_link_libraries (hdfed PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfed)
endif ()

if (HDF4_BUILD_UTILS)
  #-- Adding utility hdf2gif
  set (hdf2gif_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2gif.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfgifwr.c
  )

  add_executable (hdf2gif ${hdf2gif_SRCS})
  target_include_directories(hdf2gif PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdf2gif STATIC)
    target_link_libraries (hdf2gif PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdf2gif SHARED)
    target_link_libraries (hdf2gif PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES hdf2gif)

  #-- Adding utility gif2hdf
  set (gif2hdf_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2hdf.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2mem.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gifread.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/decompress.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/writehdf.c
  )

  add_executable (gif2hdf ${gif2hdf_SRCS})
  target_include_directories(gif2hdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (gif2hdf STATIC)
    target_link_libraries (gif2hdf PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (gif2hdf SHARED)
    target_link_libraries (gif2hdf PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} gif2hdf)

  #-- Adding utility hdf24to8
  add_executable (hdf24to8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
  target_include_directories(hdf24to8 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdf24to8 STATIC)
    target_link_libraries (hdf24to8 PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdf24to8 SHARED)
    target_link_libraries (hdf24to8 PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf24to8)

  #-- Adding utility hdftor8
  add_executable (hdftor8 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
  target_include_directories(hdftor8 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdftor8 STATIC)
    target_link_libraries (hdftor8 PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdftor8 SHARED)
    target_link_libraries (hdftor8 PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftor8)

  #-- Adding utility ristosds
  add_executable (ristosds ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
  target_include_directories(ristosds PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (ristosds STATIC)
    target_link_libraries (ristosds PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (ristosds SHARED)
    target_link_libraries (ristosds PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} ristosds)

  #-- Adding utility hdfpack
  add_executable (hdfpack ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
  target_include_directories(hdfpack PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdfpack STATIC)
    target_link_libraries (hdfpack PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdfpack SHARED)
    target_link_libraries (hdfpack PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfpack)

  #-- Adding utility paltohdf
  add_executable (paltohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
  target_include_directories(paltohdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (paltohdf STATIC)
    target_link_libraries (paltohdf PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (paltohdf SHARED)
    target_link_libraries (paltohdf PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} paltohdf)

  #-- Adding utility hdftopal
  add_executable (hdftopal ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
  target_include_directories(hdftopal PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdftopal STATIC)
    target_link_libraries (hdftopal PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdftopal SHARED)
    target_link_libraries (hdftopal PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftopal)

  #-- Adding utility r8tohdf
  add_executable (r8tohdf ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
  target_include_directories(r8tohdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (r8tohdf STATIC)
    target_link_libraries (r8tohdf PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (r8tohdf SHARED)
    target_link_libraries (r8tohdf PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} r8tohdf)

  #-- Adding utility hdfcomp
  add_executable (hdfcomp ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
  target_include_directories(hdfcomp PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdfcomp STATIC)
    target_link_libraries (hdfcomp PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdfcomp SHARED)
    target_link_libraries (hdfcomp PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfcomp)

  #-- Adding utility jpeg2hdf
  add_executable (jpeg2hdf ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
  target_include_directories(jpeg2hdf PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (jpeg2hdf STATIC)
    target_link_libraries (jpeg2hdf PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (jpeg2hdf SHARED)
    target_link_libraries (jpeg2hdf PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} jpeg2hdf)

  #-- Adding utility hdf2jpeg
  add_executable (hdf2jpeg ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
  target_include_directories(hdf2jpeg PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdf2jpeg STATIC)
    target_link_libraries (hdf2jpeg PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdf2jpeg SHARED)
    target_link_libraries (hdf2jpeg PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2jpeg)

  #-- Adding utility hdf8to24
  add_executable (hdf8to24 ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
  target_include_directories(hdf8to24 PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdf8to24 STATIC)
    target_link_libraries (hdf8to24 PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdf8to24 SHARED)
    target_link_libraries (hdf8to24 PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf8to24)

  #-- Adding utility hdfunpac
  add_executable (hdfunpac ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
  target_include_directories(hdfunpac PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (hdfunpac STATIC)
    target_link_libraries (hdfunpac PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (hdfunpac SHARED)
    target_link_libraries (hdfunpac PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfunpac)

  #-- Adding utility vmake
  add_executable (vmake ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
  target_include_directories(vmake PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (vmake STATIC)
    target_link_libraries (vmake PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (vmake SHARED)
    target_link_libraries (vmake PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vmake)

  #-- Adding utility vshow
  add_executable (vshow ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
  target_include_directories(vshow PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
  if (HDF4_BUILD_STATIC_TOOLS)
    TARGET_C_PROPERTIES (vshow STATIC)
    target_link_libraries (vshow PRIVATE ${HDF4_MF_LIB_TARGET})
  else ()
    TARGET_C_PROPERTIES (vshow SHARED)
    target_link_libraries (vshow PRIVATE ${HDF4_MF_LIBSH_TARGET})
  endif ()
  set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vshow)
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
