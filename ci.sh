#!/bin/bash
echo $CFLAGS
echo $LDFLAGS
echo $EXT2LIB
export CFLAGS+=" -I/usr/include/tirpc/"
export LDFLAGS+=" -ltirpc"
export EXT2LIB+=" -ltirpc"
echo $CFLAGS
echo $LDFLAGS
echo $EXT2LIB
autoreconf -fvi
./configure --enable-shared --disable-fortran --disable-netcdf
cat ./config.log
make
make check

