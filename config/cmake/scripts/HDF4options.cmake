#############################################################################################
####  Change default configuration of options in config/cmake/cacheinit.cmake file        ###
####  format: set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DXXX:YY=ZZZZ")                 ###
#############################################################################################

### uncomment/comment and change the following lines for other configuration options

#############################################################################################
####      maximum parallel processor count for build and test       ####
#set (MAX_PROC_COUNT 8)

#############################################################################################
####      alternate toolsets       ####
#set (CMAKE_GENERATOR_TOOLSET "Intel C++ Compiler 17.0")

#############################################################################################
####      Only build static libraries       ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DBUILD_SHARED_LIBS:BOOL=OFF")
####      Only build shared libraries       ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DONLY_SHARED_LIBS:BOOL=OFF")
####      Add PIC option on linux/mac       ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DCMAKE_ANSI_CFLAGS:STRING=-fPIC")
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DBUILD_JPEG_WITH_PIC:BOOL=ON

#############################################################################################
####      fortran enabled      ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_BUILD_FORTRAN:BOOL=ON")
####      fortran disabled      ####
set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_BUILD_FORTRAN:BOOL=OFF")

#############################################################################################
####      java enabled      ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF_BUILD_JAVA:BOOL=ON")
####      java disabled      ####
set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF_BUILD_JAVA:BOOL=OFF")

#############################################################################################
####      hdf4 xdr enabled      ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_BUILD_XDR_LIB:BOOL=ON")

#############################################################################################
### change install prefix (default use INSTALLDIR value)
set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLDIR}")

#############################################################################################
####      ext libraries       ####

### ext libs from tgz
set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_ALLOW_EXTERNAL_SUPPORT:STRING=TGZ -DTGZPATH:PATH=${CTEST_SCRIPT_DIRECTORY}")
### ext libs from git
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_ALLOW_EXTERNAL_SUPPORT:STRING=GIT")
### ext libs on system
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_ALLOW_EXTERNAL_SUPPORT:STRING=NO")
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DJPEG_LIBRARY:FILEPATH=some_location/lib/jpeg.lib -DJPEG_INCLUDE_DIR:PATH=some_location/include")
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DZLIB_LIBRARY:FILEPATH=some_location/lib/zlib.lib -DZLIB_INCLUDE_DIR:PATH=some_location/include")
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DSZIP_LIBRARY:FILEPATH=some_location/lib/szlib.lib -DSZIP_INCLUDE_DIR:PATH=some_location/include")

####      package examples       ####
#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_PACK_EXAMPLES:BOOL=ON -DHDF4_EXAMPLES_COMPRESSED:STRING=HDF5Examples-0.3.9-Source.tar.gz -DHDF4_EXAMPLES_COMPRESSED_DIR:PATH=${CTEST_SCRIPT_DIRECTORY}")

##############################################################################################
### disable test program builds

#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DBUILD_TESTING:BOOL=OFF")

#############################################################################################
### disable packaging

#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_NO_PACKAGES:BOOL=ON")
### Create install package with external libraries (szip, zlib, jpeg)
set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_PACKAGE_EXTLIBS:BOOL=ON")

#############################################################################################
### use a toolchain file

#set (ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DCMAKE_TOOLCHAIN_FILE:STRING=config/toolchain/intel.cmake")

#############################################################################################
