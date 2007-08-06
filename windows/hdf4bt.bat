@echo off
rem File Name: hdf4bt.bat
rem This batch file is used to build and test HDF4 Libraries and Utilities.
rem There batch file takes the following options:
rem      Usage: hdf4bt [/?] | [vs8] [enablefortran] [disableszip]
rem      Options:
rem         vs8               Build using Visual Studio 2005 [default
rem                           uses Visual Studio .NET]
rem         enablefortran     Build and test HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem         disableszip       Disable szip library in HDF4
rem 
rem By Xuan Bai
rem Created: 11/08/2004
rem Last Updated: 8/6/2007

rem This batch file makes the following assumptions:
rem    - The appropriate version of Visual Studio is installed and setup
rem    - The directory structure is setup from a fresh source copy
rem    - copy_hdf.bat has already been run from the ./windows directory
rem    - Environment variables INCLUDE and LIB have been setup with jpeg, zlib
rem      and szip (unless disabled) directories
rem      and zlib directories
rem    - jpeg, zlib, and szip (unless disabled) DLLs are already placed in an 
rem      accessible directory
rem    - hdf4_ext_jpeg, hdf4_ext_zlib, and hdf4_ext_szip (unless disabled) 
rem      should all be defined

rem By default, only C libraries are built and tested.

if "%1"=="/?" (
    goto help
) else (
    goto main
)

rem Print a help message
:help

    echo.Builds and tests HDF4 libraries and utilities.
    echo.
    echo.Usage: %~nx0 [/?] ^| [vs8] [enablefortran] [disableszip]
    echo.
    echo.Options:
    echo.
    echo.   /?                      Help information
    echo.
    echo.   vs8                     Build using Visual Studio 2005 [default
    echo.                           uses Visual Studio .NET]
    echo.
    echo.   enablefortran           Build and testHDF4 C/Fortran Library and 
    echo.                           Tools [default C only]
    echo.
    echo.   disableszip             Disable szip library in HDF4

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    for %%a in (%*) do (
        if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_build_params=!hdf4_build_params! vs8
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_build_params=!hdf4_build_params! enablefortran
            set hdf4_test_params=!hdf4_test_params! enablefortran
            
        ) else if "%%a"=="disableszip" (
            rem Completely disable szip
            set hdf4_build_params=!hdf4_build_params! disableszip
            
        ) else if "%%a"=="/?" (
            rem Set errorlevel 1 and send to help
            
            call :help
            exit /b 1
            
        ) else (
            rem Set errorlevel if we receive a bad parameter 
            echo.Error parsing parameters!>&2
            echo.>&2
            call :help
            exit /b 1
        )
    )
    
    exit /b 0


rem Setup our environment
:setup

    rem Put us in the directory of the batch file.
    pushd %~dp0

    rem Constants
    if "x%bt_results%"=="x" set bt_results="%CD%\bt_results.txt"
    if "x%build_results%"=="x" set build_results="%CD%\build_results.txt"

    rem Create our results file
    type nul > %bt_results%

    exit /b 0


rem Verify our environment is ok before build
:verify_env

    rem find build and test scripts
    for %%a in (hdf4build.bat hdf4check.bat) do (
        if not exist %%a (
            echo.Error: cannot find %%a>&2
            exit /b 1
        )
    )
    
    exit /b 0


rem Build HDF4 libraries and tools
:build

    call hdf4build %hdf4_build_params%
    if %errorlevel% neq 0 (
        type %build_results% >> %bt_results%
        exit /b 1
    ) else (
        type %build_results% >> %bt_results%
        exit /b 0
    )


rem Test our libraries and tools
:test

    call hdf4check %hdf4_test_params%
    if %errorlevel% neq 0 (
        type check_results.txt >> %bt_results%
        exit /b 1
    ) else (
        type check_results.txt >> %bt_results%
        
        rem Check for FAILED marks
        find "FAILED" check_results.txt > nul
        if !errorlevel! equ 0 (
            exit /b 1
        ) else (
            exit /b 0
        )
    )


rem Handle errors
:error

    rem For now, our error handling just consists of calling cleanup, and exiting
    echo.hdf4bt failed.>&2
    echo.hdf4bt failed. >> %bt_results%
    call :cleanup
    
    rem these need to be on the same line to preserve errorlevel
    endlocal
    exit /b 1


rem Cleanup our environment
:cleanup

    set build_results=
    set hdf4_build_params=
    set hdf4_test_params=

    rem Don't unset bt_results if it is being used by autotest_hdf4.bat
    if "x%hdf4_auto_results%"=="x" set bt_results=

    popd

    exit /b 0

    
rem This is where the magic happens
:main

    rem We need delayed expansion because we are setting and then re-using variables
    rem within for loops.
    setlocal enabledelayedexpansion
    
    call :parse_params %*
    if %errorlevel% neq 0 (
        exit /b 1
    )

    call :setup

    call :verify_env
    if %errorlevel% neq 0 goto error
    
    echo.Build and testing all HDF4 libraries and tools
    echo.

    echo.***************************************************************************** >> %bt_results%
    echo.                        Build and Test HDF4 Library and Tools >> %bt_results%
    echo.***************************************************************************** >> %bt_results%
    echo. >> %bt_results%

    call :build
    if %errorlevel% neq 0 (
        echo.Error building HDF4 libraries!>&2
        goto error
    )

    call :test
    if %errorlevel% neq 0 (
        echo.Error testing HDF4 libraries!>&2
        goto error
    )

    echo. All HDF4 libraries and tools build and tested successfully!
    echo. All HDF4 libraries and tools build and tested successfully! >> %bt_results%

    call :cleanup

    endlocal
    exit /b 0
