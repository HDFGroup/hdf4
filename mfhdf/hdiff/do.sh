#! /bin/sh
 make distclean
 cd ../..
 ./configure
 cd mfhdf/hdiff
 make >& make.log
 more make.log

