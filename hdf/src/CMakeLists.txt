cmake_minimum_required (VERSION 2.8)
PROJECT (HDF4_HDF_SRC C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${CMAKE_Fortran_MODULE_DIRECTORY} ${HDF4_HDF_BINARY_DIR} ${HDF4_HDF_SOURCE_DIR}/src)
LINK_DIRECTORIES (
    ${CMAKE_Fortran_MODULE_DIRECTORY}
    ${HDF4_HDF_BINARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

INCLUDE_DIRECTORIES (${HDF4_HDF_SOURCE_DIR})

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
    #${HDF4_BINARY_DIR}/h4config.h
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

ADD_LIBRARY (${HDF4_SRC_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_SRC_CSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_BINARY_DIR}/h4config.h)
IF (WIN32)
  ADD_DEFINITIONS (-DDOS_FS)
ENDIF (WIN32)
IF (BUILD_SHARED_LIBS)
  IF(WIN32)
    ADD_DEFINITIONS(-DHDFLIBDLL)
  ENDIF(WIN32)
ENDIF (BUILD_SHARED_LIBS)
TARGET_LINK_LIBRARIES (${HDF4_SRC_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_SRC_LIB_TARGET} ${HDF4_SRC_LIB_NAME} ${LIB_TYPE})

IF (HDF4_BUILD_FORTRAN)
  ENABLE_LANGUAGE (Fortran)
  #-----------------------------------------------------------------------------
  # Detect name mangling convention used between Fortran and C
  #-----------------------------------------------------------------------------
  INCLUDE (FortranCInterface)
  FortranCInterface_HEADER (
      ${HDF4_HDF_SRC_BINARY_DIR}/F77Mangle.h
      MACRO_NAMESPACE "H4_F77_"
      SYMBOL_NAMESPACE "H4_F77_"
      SYMBOLS mysub mymod:my_sub
  )

  FILE (STRINGS ${HDF4_HDF_SRC_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL\\(.*,.*\\) +(.*)")
  STRING (REGEX MATCH "H4_F77_GLOBAL\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
  SET (H4_F77_FUNC "H4_F77_FUNC(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

  FILE (STRINGS ${HDF4_HDF_SRC_BINARY_DIR}/F77Mangle.h CONTENTS REGEX "H4_F77_GLOBAL_\\(.*,.*\\) +(.*)")
  STRING (REGEX MATCH "H4_FC_GLOBAL_\\(.*,.*\\) +(.*)" RESULT ${CONTENTS})
  SET (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) ${CMAKE_MATCH_1}" PARENT_SCOPE)

  #-----------------------------------------------------------------------------
  # Make sure generated files and modules are picked up correctly
  #-----------------------------------------------------------------------------
  INCLUDE_DIRECTORIES ( 
      ${CMAKE_Fortran_MODULE_DIRECTORY}
      ${HDF4_HDF_BINARY_DIR}
  )

  #-----------------------------------------------------------------------------
  # Add debug information (intel Fortran : JB)
  #-----------------------------------------------------------------------------
  IF (CMAKE_Fortran_COMPILER MATCHES ifort)
    IF (WIN32)
      SET (CMAKE_Fortran_FLAGS_DEBUG "/debug:full /dbglibs " CACHE "flags" STRING FORCE)
      SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE "flags" STRING FORCE)
    ENDIF (WIN32)
  ENDIF (CMAKE_Fortran_COMPILER MATCHES ifort)
  
  IF (WIN32)
    SET (HDF4_HDF_SRC_CSTUB_FSRCS 
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/herrpf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/hfilepf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/mfanpf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/mfgrpf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/vattrpf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/vgpf.c
    )
  ELSE (WIN32)
    SET (HDF4_HDF_SRC_CSTUB_FSRCS
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfanf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dff.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfpf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8f.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfsdf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2i.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfutilf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/herrf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/hfilef.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/df24f.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2if.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/mfanf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/mfgrf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/vattrf.c
        ${HDF4_HDF_SRC_SOURCE_DIR}/vgf.c
    )
  ENDIF (WIN32)

  SET (HDF4_HDF_SRC_FHDRS
      ${HDF4_HDF_SRC_SOURCE_DIR}/dffunc.inc
      ${HDF4_HDF_SRC_SOURCE_DIR}/hdf.inc
  )

  SET_SOURCE_FILES_PROPERTIES (${HDF4_HDF_SRC_CSTUB_FSRCS} PROPERTIES LANGUAGE C) 

  SET (FORTRAN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Add Main fortran library
#-----------------------------------------------------------------------------
  ADD_LIBRARY (${HDF4_SRC_FCSTUB_LIB_TARGET} ${LIB_TYPE} ${HDF4_HDF_SRC_CSTUB_FSRCS} ${HDF4_HDF_SRC_CHDRS} ${HDF4_HDF_SRC_SOURCE_DIR}/hproto_fortran.h ${HDF4_BINARY_DIR}/h4config.h)
  SET_TARGET_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET} PROPERTIES LINKER_LANGUAGE C)
  IF (WIN32)
    ADD_DEFINITIONS (-DDOS_FS)
  ENDIF (WIN32)
  TARGET_LINK_LIBRARIES (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_LIB_TARGET})
  SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FCSTUB_LIB_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_SRC_FCSTUB_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_NAME} ${LIB_TYPE})

  IF (WIN32)
    SET (HDF4_F_FORTRAN_SRCS 
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/herrpff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/hfilepff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/mfanpff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/mfgrpff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/vattrpff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/fort_ps/vgpff.f
    )
  ELSE (WIN32)
    SET (HDF4_F_FORTRAN_SRCS 
        ${HDF4_HDF_SRC_SOURCE_DIR}/df24ff.f  
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfanff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfpff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfr8ff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfsdff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfufp2iff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/hfileff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/mfgrff.f
        ${HDF4_HDF_SRC_SOURCE_DIR}/vattrff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/vgff.f 
        ${HDF4_HDF_SRC_SOURCE_DIR}/dfff.f 
    )
  ENDIF (WIN32)
  #-----------------------------------------------------------------------------
  ADD_LIBRARY (${HDF4_SRC_FORTRAN_LIB_TARGET} ${LIB_TYPE} ${HDF4_F_FORTRAN_SRCS})
  IF (WIN32)
    IF (BUILD_SHARED_LIBS)
      IF (MSVC)
        SET_TARGET_PROPERTIES (${HDF4_SRC_FCSTUB_LIB_TARGET}
            PROPERTIES
                COMPILE_FLAGS "/dll"
                LINK_FLAGS "/SUBSYSTEM:CONSOLE /DLL"
        )
      ENDIF (MSVC)
    ENDIF (BUILD_SHARED_LIBS)
  ENDIF (WIN32)
  SET_TARGET_PROPERTIES (${HDF4_SRC_FORTRAN_LIB_TARGET} PROPERTIES LINKER_LANGUAGE Fortran)
  TARGET_LINK_LIBRARIES (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FCSTUB_LIB_TARGET} ${LINK_LIBS})
  SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_SRC_FORTRAN_LIB_TARGET}")
  H4_SET_LIB_OPTIONS (${HDF4_SRC_FORTRAN_LIB_TARGET} ${HDF4_SRC_FORTRAN_LIB_NAME} ${LIB_TYPE})
ENDIF (HDF4_BUILD_FORTRAN)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${HDF4_HDF_SRC_CHDRS}
    DESTINATION
        include
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (HDF4_EXPORTED_TARGETS)
  INSTALL (
      TARGETS 
          ${HDF4_SRC_LIB_TARGET}
      EXPORT
          ${HDF4_EXPORTED_TARGETS}
      LIBRARY DESTINATION lib COMPONENT libraries 
      ARCHIVE DESTINATION lib COMPONENT libraries
      RUNTIME DESTINATION bin COMPONENT libraries
  )
  
  IF (HDF4_BUILD_FORTRAN)
    INSTALL (
        TARGETS 
            ${HDF4_SRC_FCSTUB_LIB_TARGET}
            ${HDF4_SRC_FORTRAN_LIB_TARGET}
        EXPORT
            ${HDF4_EXPORTED_TARGETS}
        LIBRARY DESTINATION lib COMPONENT fortlibraries 
        ARCHIVE DESTINATION lib COMPONENT fortlibraries
        RUNTIME DESTINATION bin COMPONENT fortlibraries
    )
  ENDIF (HDF4_BUILD_FORTRAN)
ENDIF (HDF4_EXPORTED_TARGETS)

