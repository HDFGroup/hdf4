#############################################################################################
####  Change default configuration of options in config/cmake/cacheinit.cmake file        ###
####  format: set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DXXX:YY=ZZZZ")                 ###
#############################################################################################

### uncomment/comment and change the following lines for other configuration options

#############################################################################################
####      alternate toolsets       ####
#set(CMAKE_GENERATOR_TOOLSET "Intel C++ Compiler 17.0")

#############################################################################################
####      ext libraries       ####

### ext libs from tgz
set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ -DTGZPATH:PATH=${CTEST_SCRIPT_DIRECTORY}")
### ext libs from git
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_ALLOW_EXTERNAL_SUPPORT:STRING=GIT")
### ext libs on system
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DJPEG_LIBRARY:FILEPATH=some_location/lib/jpeg.lib -DJPEG_INCLUDE_DIR:PATH=some_location/include")
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DZLIB_LIBRARY:FILEPATH=some_location/lib/zlib.lib -DZLIB_INCLUDE_DIR:PATH=some_location/include")
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DSZIP_LIBRARY:FILEPATH=some_location/lib/szlib.lib -DSZIP_INCLUDE_DIR:PATH=some_location/include")

##############################################################################################
### disable test program builds

#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DBUILD_TESTING:BOOL=OFF")

#############################################################################################
### disable packaging

#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_NO_PACKAGES:BOOL=ON")
### Create install package with external libraries (szip, zlib, jpeg)
set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_PACKAGE_EXTLIBS:BOOL=ON")

#############################################################################################
