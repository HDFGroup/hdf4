cmake_minimum_required(VERSION 3.1.0)
PROJECT ( HDF4_JAVA_SRC C Java )

#-----------------------------------------------------------------------------
# Traverse source subdirectory
#-----------------------------------------------------------------------------
add_subdirectory (${HDF4_JAVA_SRC_SOURCE_DIR}/jni ${HDF4_JAVA_SRC_BINARY_DIR}/jni)
add_subdirectory (${HDF4_JAVA_SRC_SOURCE_DIR}/hdf ${HDF4_JAVA_SRC_BINARY_DIR}/hdf)
