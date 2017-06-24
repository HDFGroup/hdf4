#! /bin/sh
#
# Copyright by The HDF Group.
# All rights reserved.
#
# This file is part of HDF4.  The full HDF4 copyright notice, including
# terms governing use, modification, and redistribution, is contained in
# the COPYING file, which can be found at the root of the source code
# distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.
# If you do not have access to either file, you may request a copy from
# help@hdfgroup.org.

#
#  This file:  run-all-ex.sh
# Written by:  Larry Knox
#       Date:  January 17, 2014
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                               #
# This script will run the scripts to compile and run the installed hdf4        #
# examples.                                                                     #
#                                                                               #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

echo "Run c examples"
if ((cd c; sh ./run-c-ex.sh) && \
   (if test -d fortran; then   
       echo "Run fortran examples" 
       cd fortran; sh ./run-fortran-ex.sh 
    fi)); then
   echo "Done"
   exit 0
else
   exit 1
fi

