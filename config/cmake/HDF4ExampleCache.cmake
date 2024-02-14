# CMake cache file for examples

#########################
# EXTERNAL cache entries
#########################

# set example options to match build options
set (H4EX_BUILD_TESTING ${BUILD_TESTING} CACHE BOOL "Enable examples testing" FORCE)
set (H4EX_BUILD_EXAMPLES ${HDF4_BUILD_EXAMPLES} CACHE BOOL "Build Examples" FORCE)
set (HDF_BUILD_C ON CACHE BOOL "Build examples C support" FORCE)
set (HDF_BUILD_FORTRAN ${HDF4_BUILD_FORTRAN} CACHE BOOL "Build examples FORTRAN support" FORCE)
set (HDF_BUILD_JAVA ${HDF4_BUILD_JAVA} CACHE BOOL "Build examples JAVA support" FORCE)
set (H4EX_USE_GNU_DIRS ${HDF4_USE_GNU_DIRS} CACHE BOOL "ON to use GNU Coding Standard install directory variables, OFF to use historical settings" FORCE)

#preset HDF4 cache vars to this projects libraries instead of searching
set (H4EX_HDF4_HEADER "H4config.h" CACHE STRING "Name of HDF4 header" FORCE)
#set (H4EX_HDF4_INCLUDE_DIRS $<TARGET_PROPERTY:${HDF4_LIBSH_TARGET},INCLUDE_DIRECTORIES> CACHE PATH "HDF4 include dirs" FORCE)
set (H4EX_HDF4_INCLUDE_DIRS "${HDF4_SRC_INCLUDE_DIRS};${HDF4_BINARY_DIR}" CACHE PATH "HDF4 include dirs" FORCE)
set (H4EX_HDF4_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "HDF4 build folder" FORCE)
set (EXAMPLES_EXTERNALLY_CONFIGURED ON CACHE BOOL "Examples build is used in another project" FORCE)

set (EXAMPLE_VARNAME "H4")
set (H4EX_RESOURCES_DIR ${HDF_RESOURCES_DIR})
message (STATUS "HDF4 Example H4EX_RESOURCES_DIR: ${H4EX_RESOURCES_DIR}")

if (NOT BUILD_SHARED_LIBS AND BUILD_STATIC_LIBS)
  set (USE_SHARED_LIBS OFF CACHE BOOL "Use Shared Libraries for Examples" FORCE)
  set (H4EX_HDF4_LINK_LIBS "${HDF4_SRC_LIB_TARGET};${HDF4_MF_LIB_TARGET}" CACHE STRING "HDF4 target" FORCE)
  if (HDF4_BUILD_FORTRAN)
    set (H4EX_HDF4_LINK_LIBS ${H4EX_HDF4_LINK_LIBS} ${HDF4_F90_LIB_TARGET})
    set (HDF_MOD_EXT "/static" CACHE STRING "Use Static Modules for Examples" FORCE)
  endif ()
else ()
  set (USE_SHARED_LIBS ON CACHE BOOL "Use Shared Libraries for Examples" FORCE)
  set (H4EX_HDF4_LINK_LIBS "${HDF4_SRC_LIBSH_TARGET};${HDF4_MF_LIBSH_TARGET}" CACHE STRING "HDF4 target" FORCE)
  if (HDF4_BUILD_FORTRAN)
    set (H4EX_HDF4_LINK_LIBS ${H4EX_HDF4_LINK_LIBS} ${HDF4_F90_LIBSH_TARGET})
    set (HDF_MOD_EXT "/shared" CACHE STRING "Use Shared Modules for Examples" FORCE)
  endif ()
  if (HDF4_BUILD_JAVA)
    set (HDF4_JAVA_INCLUDE_DIRS ${HDF4_JAVA_JARS} ${HDF4_JAVA_LOGGING_JAR})
    set (H4EX_JAVA_LIBRARY ${HDF4_JAVA_JNI_LIB_TARGET})
    set (H4EX_JAVA_LIBRARIES ${HDF4_JAVA_HDF_LIB_TARGET} ${HDF4_JAVA_JNI_LIB_TARGET})
    set (HDF4_LIBRARY_PATH ${CMAKE_TEST_OUTPUT_DIRECTORY})
    message (STATUS "HDF4 Example java lib: ${H4EX_JAVA_LIBRARY} jars: ${HDF4_JAVA_INCLUDE_DIRS}")
  endif ()
endif ()
message (STATUS "HDF4 Example link libs: ${H4EX_HDF4_LINK_LIBS} Includes: ${H4EX_HDF4_INCLUDE_DIRS}")

set (HDF4_TOOLS_DIR ${CMAKE_TEST_OUTPUT_DIRECTORY} CACHE STRING "HDF4 Directory for all Executables" FORCE)
set (H4EX_HDF4_DUMP_EXECUTABLE $<TARGET_FILE:hdp${tgt_file_ext}> CACHE STRING "HDF4 hdp target" FORCE)
