cmake_minimum_required (VERSION 3.18)
project (H4EXAMPLES C)

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
set (H4EX_C_DIR              ${H4EXAMPLES_SOURCE_DIR}/C)
set (H4EX_HDF_FORTRAN_DIR    ${H4EXAMPLES_SOURCE_DIR}/hdf/FORTRAN)
set (H4EX_MFHDF_FORTRAN_DIR  ${H4EXAMPLES_SOURCE_DIR}/mfhdf/FORTRAN)
set (H4EX_JAVA_DIR           ${H4EXAMPLES_SOURCE_DIR}/JAVA)

#-----------------------------------------------------------------------------
# Basic HDF4Examples stuff here
#-----------------------------------------------------------------------------
if (NOT EXAMPLES_EXTERNALLY_CONFIGURED)
  set (H4EX_RESOURCES_DIR       ${H4EXAMPLES_SOURCE_DIR}/config/cmake)

  include (${H4EX_RESOURCES_DIR}/HDFMacros.cmake)
  include (${H4EX_RESOURCES_DIR}/HDFExampleMacros.cmake)
  set (CMAKE_JAVA_INCLUDE_PATH "")

  SET_HDF_BUILD_TYPE()

  BASIC_SETTINGS (EX)

  #-----------------------------------------------------------------------------
  # HDF4 support
  #-----------------------------------------------------------------------------
  HDF4_SUPPORT (TRUE)
endif ()
message (STATUS "HDF4 link libs: ${H4EX_HDF4_LINK_LIBS}")

#-----------------------------------------------------------------------------
# Option to build JAVA examples
#-----------------------------------------------------------------------------
option (HDF_BUILD_JAVA "Build JAVA support" OFF)
if (HDF_BUILD_JAVA)
  find_package (Java)

  include (${H4EX_RESOURCES_DIR}/UseJava.cmake)
endif ()

if (WIN32)
  set(CMAKE_TEST_LIB_DIRECTORY "${HDF4_TOOLS_DIR}")
else ()
  set(CMAKE_TEST_LIB_DIRECTORY "${HDF4_LIBRARY_PATH}")
endif ()

#-----------------------------------------------------------------------------
# All examples need the main include directories
#-----------------------------------------------------------------------------
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
    "${H4EX_HDF4_INCLUDE_DIRS}"
)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
option (H4EX_BUILD_TESTING "Build HDF4 Example Testing" OFF)
if (H4EX_BUILD_TESTING)
  set (DART_TESTING_TIMEOUT 1200 CACHE STRING
      "Timeout in seconds for each test (default 1200=20minutes)"
  )
  enable_testing ()
  include (CTest)
  include (${PROJECT_SOURCE_DIR}/CTestConfig.cmake)
  configure_file (${H4EX_RESOURCES_DIR}/CTestCustom.cmake ${PROJECT_BINARY_DIR}/CTestCustom.ctest @ONLY)
endif ()

#-----------------------------------------------------------------------------
# Option to build Fortran examples
# Make sure this appears before the CONFIGURE_FILE step
# so that fortran name mangling is detected before writing H4config.h
#-----------------------------------------------------------------------------
# Set default name mangling : overridden by Fortran detection in fortran dir
set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) name ## _")
set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) name ## __")
if (IS_DIRECTORY "${H4EXAMPLES_SOURCE_DIR}/mfhdf/FORTRAN")
  option (HDF_BUILD_FORTRAN "Build FORTRAN support" OFF)
  if (HDF_BUILD_FORTRAN AND HDF4_BUILD_FORTRAN)
    set (H4EX_LINK_Fortran_LIBS ${H4EX_HDF4_LINK_LIBS})

    if (WIN32)
      set (H4_F77_FUNC "H4_F77_FUNC(name,NAME) NAME")
      set (H4_F77_FUNC_ "H4_F77_FUNC_(name,NAME) NAME")
    endif ()
  else ()
    set (HDF_BUILD_FORTRAN OFF CACHE BOOL "Build examples FORTRAN support" FORCE)
  endif ()
else ()
  set (HDF_BUILD_FORTRAN OFF CACHE BOOL "Build examples FORTRAN support" FORCE)
endif ()

#-----------------------------------------------------------------------------
# Option to build JAVA examples
#-----------------------------------------------------------------------------
if (IS_DIRECTORY "${H4EXAMPLES_SOURCE_DIR}/JAVA")
  option (HDF_BUILD_JAVA "Build JAVA support" OFF)
else ()
  set (HDF_BUILD_JAVA OFF CACHE BOOL "Build examples JAVA support" FORCE)
endif ()

#-----------------------------------------------------------------------------
# Build examples
#-----------------------------------------------------------------------------
if (HDF_BUILD_C)
  add_subdirectory (C)
endif ()
if (HDF_BUILD_FORTRAN AND HDF4_BUILD_FORTRAN)
    add_subdirectory (FORTRAN)
endif ()
if (HDF_BUILD_JAVA AND HDF4_BUILD_JAVA)
  add_subdirectory (JAVA)
endif ()
