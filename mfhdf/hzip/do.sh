#! /bin/sh
make distclean
cd ../..
./configure
cd mfhdf/hzip
make >& make.log
more make.log

