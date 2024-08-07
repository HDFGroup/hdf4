cmake_minimum_required (VERSION 3.18)
project (HDF4_JAVA_JNI C)

set (HDF4_JAVA_JNI_CSRCS
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfanImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfdfpalImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfdfuImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfexceptionImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfgrImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfheImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfhxImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfnativeImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfr24Imp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfr8Imp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfsdsImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfstructsutil.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvdataImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvfImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvgroupImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvhImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvqImp.c
    ${HDF4_JAVA_JNI_SOURCE_DIR}/hdfvsqImp.c
)

set (CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

########### JNI libraries always must be built shared  ###############
add_library (${HDF4_JAVA_JNI_LIB_TARGET} SHARED ${HDF4_JAVA_JNI_CSRCS})
target_include_directories(${HDF4_JAVA_JNI_LIB_TARGET}
    PRIVATE "${HDF4_BINARY_DIR};${HDF4_JAVA_JNI_SOURCE_DIR}"
)
target_compile_options(${HDF4_JAVA_JNI_LIB_TARGET} PRIVATE "${HDF4_CMAKE_C_FLAGS}")
TARGET_C_PROPERTIES (${HDF4_JAVA_JNI_LIB_TARGET} SHARED)
target_link_libraries (${HDF4_JAVA_JNI_LIB_TARGET} PUBLIC ${HDF4_MF_LIBSH_TARGET} ${HDF4_SRC_LIBSH_TARGET})
set_target_properties (${HDF4_JAVA_JNI_LIB_TARGET} PROPERTIES FOLDER libraries/jni)
SET_GLOBAL_VARIABLE (HDF4_LIBRARIES_TO_EXPORT "${HDF4_LIBRARIES_TO_EXPORT};${HDF4_JAVA_JNI_LIB_TARGET}")
H4_SET_LIB_OPTIONS (${HDF4_JAVA_JNI_LIB_TARGET} ${HDF4_JAVA_JNI_LIB_NAME} SHARED "JAVA")

#-----------------------------------------------------------------------------
# Add Target to clang-format
#-----------------------------------------------------------------------------
if (HDF4_ENABLE_FORMATTERS)
  clang_format (HDF4_JNI_SRC_FORMAT ${HDF4_JAVA_JNI_LIB_TARGET})
endif ()

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
if (HDF4_EXPORTED_TARGETS)
  INSTALL_TARGET_PDB (${HDF4_JAVA_JNI_LIB_TARGET} ${HDF4_INSTALL_BIN_DIR} libraries)
  install (
      TARGETS
          ${HDF4_JAVA_JNI_LIB_TARGET}
      EXPORT
          ${HDF4_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
      ARCHIVE DESTINATION ${HDF4_INSTALL_LIB_DIR} COMPONENT libraries
      RUNTIME DESTINATION ${HDF4_INSTALL_BIN_DIR} COMPONENT libraries
      FRAMEWORK DESTINATION ${HDF4_INSTALL_FWRK_DIR} COMPONENT libraries
      INCLUDES DESTINATION include
  )
endif ()
