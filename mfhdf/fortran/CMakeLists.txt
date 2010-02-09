PROJECT(HDF4_MFHDF_F90)

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

#-----------------------------------------------------------------------------
# f90CStub lib
#-----------------------------------------------------------------------------
SET(f90CStub_C_SRCS
  ${HDF4_MFHDF_FORTRAN_DIR}/?.c 
)

SET(f90CStub_C_HDRS
  ${HDF4_MFHDF_FORTRAN_DIR}/?.c 
)

SET_SOURCE_FILES_PROPERTIES( ${f90CStub_C_SRCS} PROPERTIES LANGUAGE C) 

ADD_LIBRARY(${HDF4_MF_FCSTUB_LIB_NAME} ${f90CStub_C_SRCS} )
TARGET_LINK_LIBRARIES(${HDF4_MF_FCSTUB_LIB_NAME} hdf4)

#-----------------------------------------------------------------------------
ADD_LIBRARY(H4FORTRAN_NETCDF  STATIC ${HDF4_MFHDF_FORTRAN_DIR}/?.f90
ADD_LIBRARY(H4FORTRAN_MFFUNC  STATIC ${HDF4_MFHDF_FORTRAN_DIR}/?.f90 )

#-----------------------------------------------------------------------------
ADD_LIBRARY(${HDF4_MF_FORTRAN_LIB_NAME} STATIC ${HDF4MODULE_SRC} )
SET_GLOBAL_VARIABLE(HDF4_LIBRARIES_TO_EXPORT 
  "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_MF_FCSTUB_LIB_NAME};${HDF4_MF_FORTRAN_LIB_NAME}"
)
add_dependencies(${HDF4_MF_FORTRAN_LIB_NAME}
  ${HDF4_MF_FCTUB_LIB_NAME}
)
