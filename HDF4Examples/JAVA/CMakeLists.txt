cmake_minimum_required (VERSION 3.12)
project (HDF4Examples_JAVA Java)

set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${HDFJAVA_LIB_DIR};${JAVA_INCLUDE_PATH};${JAVA_INCLUDE_PATH2}"
)

add_subdirectory (exAN)
add_subdirectory (exGR)
add_subdirectory (exVD)
add_subdirectory (exSD)
add_subdirectory (exJ)
