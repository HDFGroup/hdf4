@echo OFF
rem /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
rem * Copyright by The HDF Group.                                               *
rem * Copyright by the Board of Trustees of the University of Illinois.         *
rem * All rights reserved.                                                      *
rem *                                                                           *
rem * This file is part of HDF.  The full HDF copyright notice, including       *
rem * terms governing use, modification, and redistribution, is contained in    *
rem * the files COPYING and Copyright.html.  COPYING can be found at the root   *
rem * of the source code distribution tree; Copyright.html can be found at      *
rem * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
rem * access to either file, you may request a copy from help@hdfgroup.org.     *
rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
rem
rem  File Name: testncdump.bat
rem  This batch file is used to test ncdump program
rem  This script has the following options:
rem    testinterface (release / debug)
rem             Test either the release or debug version of ncdump program
rem  Created:       Scott Wegner, 6/11/08
rem  Last Updated:  Scott Wegner, 6/11/08

setlocal enabledelayedexpansion
pushd %~dp0

set srcdir=%CD%

%CD%\..\ncgen\%1\ncgen -o test0.nc -n %srcdir%\test0.cdl
%CD%\%1\ncdump test0.nc > test1.cdl
%CD%\..\ncgen\%1\ncgen -o test1.nc -n test1.cdl
%CD%\%1\ncdump -n test0 test1.nc > test2.cdl
fc test1.cdl test2.cdl > nul
if %errorlevel% equ 0 (
    echo.*** ncdump test successful ***
)
del /f /q test0.nc test1.nc test1.cdl test2.cdl

popd
endlocal & exit /b 0