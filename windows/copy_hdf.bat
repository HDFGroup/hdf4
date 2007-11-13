@echo off
rem Copyright by The HDF Group.
rem Copyright by the Board of Trustees of the University of Illinois.
rem All rights reserved.
rem
rem This file is part of HDF5.  The full HDF5 copyright notice, including
rem terms governing use, modification, and redistribution, is contained in
rem the files COPYING and Copyright.html.  COPYING can be found at the root
rem of the source code distribution tree; Copyright.html can be found at the
rem root level of an installed copy of the electronic HDF5 document set and
rem is linked from the top-level documents page.  It can also be found at
rem http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have
rem access to either file, you may request a copy from help@hdfgroup.org.
rem
rem File Name   : copy_hdf.bat
rem Purpose     : Copy all Files in the current directory to its parent directory
rem
rem Written By  : Scott Wegner, July 11, 2007
rem Last Update : August 8, 2007

pushd %~dp0

rem Create an "exclude file", which contains this file, itself, and the proj directory
echo.\exclude.txt > exclude.txt
echo.\%~nx0 >> exclude.txt
echo.\proj\ >> exclude.txt
echo.\proj_vs6\ >> exclude.txt
echo.\examples\ >> exclude.txt

rem Copy our files
xcopy . .. /e /i /y /EXCLUDE:exclude.txt

rem Remove our temporary exclude file
del %CD%\exclude.txt

popd
