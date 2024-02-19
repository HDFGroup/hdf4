cmake_minimum_required (VERSION 3.12)
project (HDF4_HDF_UTIL C)

if (HDF4_BUILD_TOOLS)
  #-- Adding tool hdfls
  if (BUILD_STATIC_LIBS)
    add_executable (hdfls${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
    target_include_directories(hdfls${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfls${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdfls${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_EXECUTABLES hdfls${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfls${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfls.c)
    target_include_directories(hdfls${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfls${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdfls${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfls${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdfed
  set (hdfed_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_cntrl.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_disp.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_file.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/he_main.c
  )

  if (BUILD_STATIC_LIBS)
    add_executable (hdfed${H4_NAME_STATIC_TOOLS} ${hdfed_SRCS})
    target_include_directories(hdfed${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfed${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdfed${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfed${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfed${H4_NAME_SHARED_TOOLS} ${hdfed_SRCS})
    target_include_directories(hdfed${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfed${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdfed${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_EXECUTABLES ${H4_DEP_EXECUTABLES} hdfed${H4_NAME_SHARED_TOOLS})
  endif ()
endif ()

if (HDF4_BUILD_UTILS)
  #-- Adding utility hdf2gif
  set (hdf2gif_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2gif.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfgifwr.c
  )

  if (BUILD_STATIC_LIBS)
    add_executable (hdf2gif${H4_NAME_STATIC_TOOLS} ${hdf2gif_SRCS})
    target_include_directories(hdf2gif${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf2gif${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdf2gif${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES hdf2gif${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf2gif${H4_NAME_SHARED_TOOLS} ${hdf2gif_SRCS})
    target_include_directories(hdf2gif${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf2gif${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdf2gif${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2gif${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility gif2hdf
  set (gif2hdf_SRCS
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2hdf.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gif2mem.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/gifread.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/decompress.c
      ${HDF4_HDF_UTIL_SOURCE_DIR}/writehdf.c
  )

  if (BUILD_STATIC_LIBS)
    add_executable (gif2hdf${H4_NAME_STATIC_TOOLS} ${gif2hdf_SRCS})
    target_include_directories(gif2hdf${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (gif2hdf${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (gif2hdf${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} gif2hdf${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (gif2hdf${H4_NAME_SHARED_TOOLS} ${gif2hdf_SRCS})
    target_include_directories(gif2hdf${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (gif2hdf${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (gif2hdf${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} gif2hdf${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdf24to8
  if (BUILD_STATIC_LIBS)
    add_executable (hdf24to8${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
    target_include_directories(hdf24to8${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf24to8${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdf24to8${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf24to8${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf24to8${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf24to8.c)
    target_include_directories(hdf24to8${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf24to8${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdf24to8${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf24to8${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdftor8
  if (BUILD_STATIC_LIBS)
    add_executable (hdftor8${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
    target_include_directories(hdftor8${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdftor8${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdftor8${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftor8${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdftor8${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftor8.c)
    target_include_directories(hdftor8${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdftor8${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdftor8${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftor8${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility ristosds
  if (BUILD_STATIC_LIBS)
    add_executable (ristosds${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
    target_include_directories(ristosds${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (ristosds${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (ristosds${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} ristosds${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (ristosds${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/ristosds.c)
    target_include_directories(ristosds${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (ristosds${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (ristosds${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} ristosds${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdfpack
  if (BUILD_STATIC_LIBS)
    add_executable (hdfpack${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
    target_include_directories(hdfpack${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfpack${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdfpack${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfpack${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfpack${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfpack.c)
    target_include_directories(hdfpack${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfpack${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdfpack${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfpack${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility paltohdf
  if (BUILD_STATIC_LIBS)
    add_executable (paltohdf${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
    target_include_directories(paltohdf${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (paltohdf${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (paltohdf${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} paltohdf${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (paltohdf${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/paltohdf.c)
    target_include_directories(paltohdf${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (paltohdf${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (paltohdf${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} paltohdf${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdftopal
  if (BUILD_STATIC_LIBS)
    add_executable (hdftopal${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
    target_include_directories(hdftopal${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdftopal${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdftopal${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftopal${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdftopal${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdftopal.c)
    target_include_directories(hdftopal${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdftopal${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdftopal${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdftopal${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility r8tohdf
  if (BUILD_STATIC_LIBS)
    add_executable (r8tohdf${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
    target_include_directories(r8tohdf${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (r8tohdf${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (r8tohdf${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} r8tohdf${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (r8tohdf${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/r8tohdf.c)
    target_include_directories(r8tohdf${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (r8tohdf${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (r8tohdf${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} r8tohdf${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdfcomp
  if (BUILD_STATIC_LIBS)
    add_executable (hdfcomp${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
    target_include_directories(hdfcomp${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfcomp${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdfcomp${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfcomp${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfcomp${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfcomp.c)
    target_include_directories(hdfcomp${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfcomp${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdfcomp${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfcomp${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility jpeg2hdf
  if (BUILD_STATIC_LIBS)
    add_executable (jpeg2hdf${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
    target_include_directories(jpeg2hdf${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (jpeg2hdf${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (jpeg2hdf${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} jpeg2hdf${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (jpeg2hdf${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/jpeg2hdf.c)
    target_include_directories(jpeg2hdf${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (jpeg2hdf${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (jpeg2hdf${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} jpeg2hdf${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdf2jpeg
  if (BUILD_STATIC_LIBS)
    add_executable (hdf2jpeg${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
    target_include_directories(hdf2jpeg${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf2jpeg${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdf2jpeg${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2jpeg${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf2jpeg${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf2jpeg.c)
    target_include_directories(hdf2jpeg${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf2jpeg${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdf2jpeg${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf2jpeg${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdf8to24
  if (BUILD_STATIC_LIBS)
    add_executable (hdf8to24${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
    target_include_directories(hdf8to24${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf8to24${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdf8to24${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf8to24${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdf8to24${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdf8to24.c)
    target_include_directories(hdf8to24${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdf8to24${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdf8to24${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdf8to24${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility hdfunpac
  if (BUILD_STATIC_LIBS)
    add_executable (hdfunpac${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
    target_include_directories(hdfunpac${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfunpac${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (hdfunpac${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfunpac${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (hdfunpac${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/hdfunpac.c)
    target_include_directories(hdfunpac${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (hdfunpac${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (hdfunpac${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} hdfunpac${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility vmake
  if (BUILD_STATIC_LIBS)
    add_executable (vmake${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
    target_include_directories(vmake${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (vmake${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (vmake${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vmake${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (vmake${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/vmake.c)
    target_include_directories(vmake${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (vmake${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (vmake${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vmake${H4_NAME_SHARED_TOOLS})
  endif ()

  #-- Adding utility vshow
  if (BUILD_STATIC_LIBS)
    add_executable (vshow${H4_NAME_STATIC_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
    target_include_directories(vshow${H4_NAME_STATIC_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (vshow${H4_NAME_STATIC_TOOLS} STATIC)
    target_link_libraries (vshow${H4_NAME_STATIC_TOOLS} PRIVATE ${HDF4_MF_LIB_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vshow${H4_NAME_STATIC_TOOLS})
  endif ()

  if (BUILD_SHARED_LIBS)
    add_executable (vshow${H4_NAME_SHARED_TOOLS} ${HDF4_HDF_UTIL_SOURCE_DIR}/vshow.c)
    target_include_directories(vshow${H4_NAME_SHARED_TOOLS} PRIVATE "${HDF4_HDF_BINARY_DIR};${HDF4_HDFSOURCE_DIR};${HDF4_BINARY_DIR}")
    TARGET_C_PROPERTIES (vshow${H4_NAME_SHARED_TOOLS} SHARED)
    target_link_libraries (vshow${H4_NAME_SHARED_TOOLS} PRIVATE ${HDF4_MF_LIBSH_TARGET})
    set (H4_DEP_UTILITIES ${H4_DEP_UTILITIES} vshow${H4_NAME_SHARED_TOOLS})
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
