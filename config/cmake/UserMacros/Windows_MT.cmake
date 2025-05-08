########################################################
#  Include file for user options
########################################################

# To use this option, copy both the macro and option code
# into the root UserMacros.cmake file. Then enable the option,
# using the command line add "-DBUILD_STATIC_CRT_LIBS:BOOL=ON"
# OR add an include to the root UserMacros.cmake file:
# INCLUDE(path_to_file/WINDOWS_MT.cmake)

#-----------------------------------------------------------------------------
# Option to Build with Static CRT libraries on Windows (USE WITH CAUTION!!!)
#-----------------------------------------------------------------------------
option (HDF4_BUILD_STATIC_CRT_LIBS "Build With Static Windows CRT Libraries" OFF)
mark_as_advanced (HDF4_BUILD_STATIC_CRT_LIBS)
if (HDF4_BUILD_STATIC_CRT_LIBS)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()
