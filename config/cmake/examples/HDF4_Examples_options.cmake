#############################################################################################
####  Change default configuration of options in config/cmake/cacheinit.cmake file        ###
####  format: set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DXXX:YY=ZZZZ")                 ###
####  DEFAULT:                                                                            ###
####         BUILD_SHARED_LIBS:BOOL=OFF                                                   ###
####         H4EX_BUILD_C:BOOL=ON                                                         ###
####         H4EX_BUILD_FORTRAN:BOOL=ON                                                   ###
####         H4EX_BUILD_JAVA:BOOL=ON                                                      ###
####         H4EX_BUILD_TESTING:BOOL=ON                                                   ###
#############################################################################################

### uncomment/comment and change the following lines for other configuration options
### build with shared libraries
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DBUILD_SHARED_LIBS:BOOL=ON")

#############################################################################################
####      languages       ####
### disable C builds
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DH4EX_BUILD_C:BOOL=OFF")

### disable Fortran builds
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DH4EX_BUILD_FORTRAN:BOOL=OFF")

### disable JAVA builds
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DH4EX_BUILD_JAVA:BOOL=OFF")

#############################################################################################
### match the hdf45 library namespace
set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DHDF4_NAMESPACE:STRING=hdf4::")

#############################################################################################
### disable test program builds, requires reference files in testfiles subdirectory
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DH4EX_BUILD_TESTING:BOOL=OFF")
#set(ADD_BUILD_OPTIONS "${ADD_BUILD_OPTIONS} -DH4EX_COMPARE_TESTING:BOOL=OFF")

#############################################################################################
