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
rem  File Name: testinterface.bat
rem  This batch file is used to test mfhdf and netCDF interface
rem  This script has the following options:
rem    testinterface (release / debug)
rem             Test either the release or debug version of mfhdf and netCDF interface

setlocal enabledelayedexpansion
pushd %~dp0

set srcdir=%CD%
set diff=fc /w

echo.============================
echo.HDF-SD C interface tests
echo.============================
%CD%\%1\%2hdftest
echo.
echo.============================
echo.HDF-nc C interface tests
echo.============================
%CD%\%1\%2hdfnctest
echo.
echo.=========================
echo.netCDF formatted tests
echo.=========================
%CD%\%1\%2cdftest > cdfout.new
set cmd=%diff% cdfout.new %srcdir%\testout.sav
echo.%cmd%
%cmd% > nul
if %errorlevel% equ 0 (
    echo.*** netCDF passes formatted test ***
) else (
    echo.*** netCDF fails formatted test ***
    %cmd%
    echo.The above differences are OK if small
)

del /f /q *.new *.hdf *.cdf


popd
endlocal & exit /b 0
