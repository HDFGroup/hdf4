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
   
string(TOLOWER ${CMAKE_SYSTEM_NAME} mf_f90_sys)

MESSAGE(STATUS "System Name ${CMAKE_SYSTEM_NAME}")
ADD_CUSTOM_COMMAND(
   OUTPUT ${HDF4_BINARY_DIR}/jackets.c
   COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_DIR}/config/jackets-${mf_f90_sys}.c ${HDF4_BINARY_DIR}/jackets.c
   COMMENT " Copying jackets Name jackets-${mf_f90_sys}.c to jackets.c")

ADD_CUSTOM_COMMAND(
   OUTPUT ${HDF4_BINARY_DIR}/ftest.f
   COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_DIR}/config/ftest-${mf_f90_sys}.f ${HDF4_BINARY_DIR}/ftest.f
   COMMENT " Copying ftest Name ftest-${mf_f90_sys}.f to ftest.f")

ADD_CUSTOM_COMMAND(
   OUTPUT ${HDF4_BINARY_DIR}/netcdf.inc
   COMMAND ${CMAKE_COMMAND} -E copy ${HDF4_MFHDF_FORTRAN_DIR}/config/netcdf-${mf_f90_sys}.inc ${HDF4_BINARY_DIR}/netcdf.inc
   COMMENT " Copying netcdf Name netcdf-${mf_f90_sys}.inc to netcdf.inc")

#-----------------------------------------------------------------------------
# f90CStub lib
#-----------------------------------------------------------------------------
SET(f90CStub_C_SRCS
  ${HDF4_MFHDF_FORTRAN_DIR}/?.c 
)

SET(f90CStub_C_HDRS
  ${HDF4_MFHDF_FORTRAN_DIR}/?.c 
)

SET (HDF4_MFHDF_FORTRAN_FSRCS
    ${HDF4_BINARY_DIR}/jackets.c
    ${HDF4_MFHDF_FORTRAN_DIR}/mfsdf.c
    ${HDF4_MFHDF_FORTRAN_DIR}/mfsdff.f
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
