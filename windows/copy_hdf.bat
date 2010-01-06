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
rem This batch file takes the following options:
rem         vs8               Build using Visual Studio 2005
rem         vs9               Build using Visual Studio 2008
rem                           [default Visual Studio 2008]
rem         enablefortran     Build HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem         ivf91             Build HDF4 Fortran using Intel Visual Fortran 9.1
rem         ivf101            Build HDF4 Fortran using Intel Visual Fortran 10.1
rem         ivf111            Build HDF4 Fortran using Intel Visual Fortran 11.1
rem                           [default Intel Visual Fortran 11.1]

pushd %~dp0

set nerrors=0
if "%1"=="/?" goto help
set VS_VERSION=vs9
set IVF_VERSION=ivf111
set hdf4_enablefortran=
goto main

rem Print a help message
:help

    echo.Builds HDF4 Libraries and Tools.
    echo.
    echo.Usage: %~nx0 [OPTIONS]
    echo.
    echo.   /?                      Help information
    echo.   vs8                     Build using Visual Studio 2005
    echo.   vs9                     Build using Visual Studio 2008
    echo.                           [default Visual Studio 2008]
    echo.   ivf91                   Build HDF4 Fortran using Intel Visual Fortran 9.1
    echo.   ivf101                  Build HDF4 Fortran using Intel Visual Fortran 10.1
    echo.   ivf111                  Build HDF4 Fortran using Intel Visual Fortran 11.1
    echo.                           [default Intel Visual Fortran 11.1]

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    for %%a in (%*) do (
        if "%%a"=="vs9" (
            rem Use VS2008 as our compiler
            set VS_VERSION=vs9
            
        ) else if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set VS_VERSION=vs8
                        
        ) else if "%%a"=="ivf91" (
            rem Enable Fortran
            set IVF_VERSION=ivf91
            set hdf4_enablefortran=true
            
        ) else if "%%a"=="ivf101" (
            rem Enable Fortran
            set IVF_VERSION=ivf101
            set hdf4_enablefortran=true
           
        ) else if "%%a"=="ivf111" (
            rem Enable Fortran
            set IVF_VERSION=ivf111
            set hdf4_enablefortran=true
            
        ) else (
            rem Set errorlevel if we receive a bad parameter 
            echo.Unknown option: %%a
            call :help
            exit /b 1
        )
    )
    
    exit /b 0


rem This is where the magic happens
:main

    call :parse_params %*
    if %errorlevel% neq 0 (
        echo.Error parsing parameters!
        goto end
    )

	rem Create an "exclude file", which contains this file, itself, and the proj directory
	echo.\exclude.txt > exclude.txt
	echo.\%~nx0 >> exclude.txt
	echo.\proj\ >> exclude.txt
	echo.\vs8\ >> exclude.txt
	echo.\vs9\ >> exclude.txt
	echo.\ivf91\ >> exclude.txt
	echo.\ivf101\ >> exclude.txt
	echo.\vs8ivf101\ >> exclude.txt
	echo.\vs9ivf101\ >> exclude.txt
	echo.\ivf111\ >> exclude.txt
	echo.\examples\ >> exclude.txt

	rem Copy our files
	xcopy . .. /e /i /y /EXCLUDE:exclude.txt

	rem Remove our temporary exclude file
	del %CD%\exclude.txt

	pushd %VS_VERSION%
	xcopy . ..\proj /e /i /y
	popd

    if "f%hdf4_enablefortran%"=="ftrue" (
		echo.enabling fortran
		pushd %IVF_VERSION%
		xcopy . ..\proj /e /i /y
		popd
		
		if "%IVF_VERSION%"=="ivf101" (
			pushd %VS_VERSION%%IVF_VERSION%
			xcopy . ..\proj /e /i /y
			popd
		)
	)

    rem Fall throught to end

:end
    popd
    endlocal & exit /b

