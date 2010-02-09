PROJECT ( HDF4_HDF_SRC)

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

SET (HDF4_HDF_SRC_CSRCS
    ${HDF4_HDF_SRC_SOURCE_DIR}/atom.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/bitvect.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cdeflate.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnbit.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnone.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/crle.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cskphuff.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/cszip.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/df24.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfan.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfconv.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgroup.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfimcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfjpeg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkconv.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkcray.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkfuji.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfknat.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkswap.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfkvms.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfrle.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsd.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfstubs.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfunjpeg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfutil.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dynarray.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/glist.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbitio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hblocks.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbuffer.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hchunks.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcomp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcompri.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdfalloc.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/herr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hextelt.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfile.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfiledd.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hkit.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/linklist.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mcache.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfan.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mstdio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/tbbt.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattr.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vconv.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vg.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgp.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vhi.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vio.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vparse.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vrw.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vsfld.c
)

SET (HDF4_HDF_SRC_FSRCS
    ${HDF4_HDF_SRC_SOURCE_DIR}/df24ff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfanf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfanff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dff.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfpf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfpff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8f.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8ff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsdf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsdff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2i.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2iff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfutilf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/herrf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfilef.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/df24f.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2if.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfileff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfanf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgrf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgrff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattrf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattrff.f
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgf.c
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgff.f
)

SET (HDF4_HDF_SRC_CHDRS
    ${HDF4_HDF_SRC_SOURCE_DIR}/atom.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/bitvect.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cdeflate.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnbit.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cnone.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/crle.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cskphuff.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/cszip.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/df.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfan.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfgr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfrig.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfsd.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfstubs.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2i.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/dynarray.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/glist.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/h4config.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hbitio.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hchunks.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcomp.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hcompi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hconv.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdf.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdfi.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/herr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hfile.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hkit.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hlimits.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hntdefs.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hproto.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/hqueue.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/htags.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/linklist.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mcache.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfan.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mfgr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/mstdio.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/tbbt.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vattr.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vg.h
    ${HDF4_HDF_SRC_SOURCE_DIR}/vgint.h
)

SET (HDF4_HDF_SRC_FHDRS
    ${HDF4_HDF_SRC_SOURCE_DIR}/dffunc.f90
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdf.f90
    ${HDF4_HDF_SRC_SOURCE_DIR}/dffunc.inc
    ${HDF4_HDF_SRC_SOURCE_DIR}/hdf.inc
)

INCLUDE_DIRECTORIES( ${HDF4_HDF_SOURCE_DIR} )

IF (HDF4_BUILD_FORTRAN)
  IF(MSVC)
    ADD_SUBDIRECTORY(${HDF4_HDF_SOURCE_DIR}/src/fort_ps ${PROJECT_BINARY_DIR}/src/fort_ps)
    SET (HDF4_HDF_SRC_SRCS ${HDF4_HDF_SRC_CSRCS})
    SET (HDF4_HDF_SRC_HDRS ${HDF4_HDF_SRC_CHDRS})
  ELSE(MSVC)
    SET (HDF4_HDF_SRC_SRCS ${HDF4_HDF_SRC_CSRCS} ${HDF4_HDF_SRC_FSRCS})
    SET (HDF4_HDF_SRC_HDRS ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDF_SRC_SOURCE_DIR}/hproto_fortran.h ${HDF4_HDF_SRC_FHDRS})
  ENDIF(MSVC)
ELSE (HDF4_BUILD_FORTRAN)
    SET (HDF4_HDF_SRC_SRCS ${HDF4_HDF_SRC_CSRCS})
    SET (HDF4_HDF_SRC_HDRS ${HDF4_HDF_SRC_CHDRS})
ENDIF (HDF4_BUILD_FORTRAN)

IF(WIN32)
  ADD_DEFINITIONS(-D_HDFLIB_)
ENDIF(WIN32)

IF (BUILD_SHARED_LIBS)
  IF(WIN32)
    ADD_DEFINITIONS(-D_HDFDLL_-DHDFLIBDLL)
  ENDIF(WIN32)
  ADD_LIBRARY(${HDF4_SRC_LIB_NAME} SHARED ${HDF4_HDF_SRC_SRCS} ${HDF4_HDF_SRC_HDRS} )
  # Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
  IF(MSVC)
    SET_TARGET_PROPERTIES(${HDF4_SRC_LIB_NAME} PROPERTIES COMPILE_FLAGS "${SHARED_C_FLAGS_RELEASE}")
  ENDIF(MSVC)
  TARGET_LINK_LIBRARIES(${HDF4_SRC_LIB_NAME} ${LINK_LIBS})
  SET_TARGET_PROPERTIES(${HDF4_SRC_LIB_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ENDIF (BUILD_SHARED_LIBS)

ADD_LIBRARY(${HDF4_SRC_LIB_NAME}-static STATIC  ${HDF4_HDF_SRC_SRCS} ${HDF4_HDF_SRC_HDRS})
SET_TARGET_PROPERTIES(${HDF4_SRC_LIB_NAME}-static PROPERTIES OUTPUT_NAME "${HDF4_SRC_LIB_NAME}")
SET_TARGET_PROPERTIES(${HDF4_SRC_LIB_NAME}-static PROPERTIES PREFIX "lib")

# Help CMake 2.6.x and lower (not necessary for 2.8 and above, but doesn't hurt):
SET_TARGET_PROPERTIES(${HDF4_SRC_LIB_NAME}-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#-----------------------------------------------------------------------------
# Add library to CMake Install : Installs lib and cmake config info
#-----------------------------------------------------------------------------
INSTALL(
  TARGETS 
    ${HDF4_SRC_LIB_NAME}-static
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

IF (BUILD_SHARED_LIBS)
INSTALL(
  TARGETS 
    ${HDF4_SRC_LIB_NAME}
  EXPORT 
    hdf4-targets
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
ENDIF (BUILD_SHARED_LIBS)
