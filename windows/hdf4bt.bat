@echo off
rem File Name: hdf4bt.bat
rem This batch file is used to build and test HDF4 Libraries and Utilities.
rem There batch file takes the following options:
rem      Options:
rem         vs9               Build using Visual Studio 2008
rem         enablefortran     Build and test HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem         ivf111            Build HDF4 Fortran using Intel Visual Fortran 11.1
rem         ivf101            Build HDF4 Fortran using Intel Visual Fortran 10.1
rem         useenv            Build using variables set in the environment.
rem         log               Log the build and test results in files defined by
rem                           environment variables HDF4BUILD_LOG and
rem                           HDF4CHECK_LOG, or hdf4build.log and hdf4check.log
rem                           if undefined
rem
rem By Xuan Bai
rem Created: 11/08/2004
rem Last Updated: Scott Weegner, 7/7/08

rem This batch file makes the following assumptions:
rem    - The appropriate version of Visual Studio is installed and setup
rem    - The directory structure is setup from a fresh source copy
rem    - copy_hdf.bat has already been run from the ./windows directory
rem    - The directory structure for external libraries has been setup in the
rem      IDE, or..
rem    - If using useenv switch, environment variables INCLUDE and LIB have 
rem      been setup with jpeg, zlib and szip (unless disabled) directories
rem    - jpeg, zlib, and szip (unless disabled) DLLs are already placed in an 
rem      accessible directory
rem    - hdf4_ext_jpeg, hdf4_ext_zlib, and hdf4_ext_szip (unless disabled) 
rem      should all be defined
rem    - The target platform architecture is specified in the environment
rem      variable PROCESSOR_ARCHITECTURE


rem By default, only C libraries are built and tested.

setlocal enabledelayedexpansion
pushd %~dp0

set nerrors=0
if "%1"=="/?" goto help 
goto main

rem Print a help message
:help

    echo.Builds and tests HDF4 libraries and utilities.
    echo.
    echo.Usage: %~nx0 [OPTIONS]
    echo.
    echo.Options:
    echo.
    echo.   /?                      Help information
    echo.   vs9                     Build using Visual Studio 2008
    echo.   enablefortran           Build and testHDF4 C/Fortran Library and 
    echo.                           Tools [default C only]
    echo.   ivf111                  Build HDF4 Fortran using Intel Visual Fortran 11.1
    echo.   ivf101                  Build HDF4 Fortran using Intel Visual Fortran 10.1
    echo.   useenv                  Build using variables set in the environment.
    echo.   log                     Log the build and test results in files defined by
    echo.                           environment variables HDF4BUILD_LOG and
    echo.                           HDF4CHECK_LOG, or hdf4build.log and hdf4check.log
    echo.                           if undefined
    echo.
    echo.
    echo.Specifying useenv requires you to set the INCLUDE and LIB variables to the
    echo.directories of all external libraries.  This overrides any include or library
    echo.directories set in the IDE.

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    for %%a in (%*) do (
        if "%%a"=="vs9" (
            rem Use VS2008 as our compiler
            set hdf4build_params=!hdf4build_params! vs9
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4build_params=!hdf4build_params! enablefortran
            set hdf4check_params=!hdf4check_params! enablefortran
            
        ) else if "%%a"=="ivf111" (
            rem Enable Fortran
            set hdf4build_params=!hdf4build_params! ivf111
            
        ) else if "%%a"=="ivf101" (
            rem Enable Fortran
            set hdf4build_params=!hdf4build_params! ivf101
            
        ) else if "%%a"=="log" (
            rem Log our results to files defined in environment
            set hdf4_logresults=true
                        
        ) else if "%%a"=="useenv" (
            rem Set the compiler to use environment variables for settings
            set hdf4build_params=!hdf4build_params! useenv
                        
        ) else (
            rem Set errorlevel if we receive a bad parameter 
            echo.Error parsing parameters!
            echo.
            call :help
            exit /b 1
        )
    )
    
    exit /b 0


rem Setup our environment
:setup
    
    if defined hdf4_logresults (
        if not defined hdf4build_log (
            set hdf4build_log=%CD%\hdf4build.log
        )
        type nul > !hdf4build_log!
        if not defined hdf4check_log (
            set hdf4check_log=%CD%\hdf4check.log
        )
        type nul > !hdf4check_log!
    )

    exit /b 0

    

rem Build HDF4 libraries and tools
:build

    if defined hdf4_logresults (
        echo.Calling hdf4build.bat %hdf4build_params%
        echo.Results logged in %hdf4build_log%
        call hdf4build.bat %hdf4build_params% > !hdf4build_log! 2>&1
    ) else (
        call hdf4build.bat %hdf4build_params%
    )
    
    exit /b


    
rem Test our libraries and tools
:test

    if defined hdf4_logresults (
        echo.Calling hdf4check.bat %hdf4check_params%
        echo.Results logged in %hdf4check_log%
        call hdf4check.bat %hdf4check_params% > !hdf4check_log! 2>&1
    ) else (
        call hdf4check.bat %hdf4check_params%
    )
    
    exit /b


rem Handle errors
:error

    rem For now, our error handling just consists of setting nerrors and calling end
    echo.HDF4 build-test failed.
    set /a nerrors=%nerrors%+1
    goto end
    
    rem We'll never really get here, but we keep this line for consistency
    exit /b 


    
rem This is where the magic happens
:main

    call :parse_params %*
    if %errorlevel% neq 0 (
        echo.Error parsing parameters!
        goto error
    )

    call :setup
    if %errorlevel% neq 0 (
        echo.Error setting up hdf4bt environment!
        goto error
    )

    echo.*****************************************************************************
    echo.                        Build and Test HDF4 Library and Tools
    echo.*****************************************************************************
    echo.

    call :build
    if %errorlevel% neq 0 (
        echo.Error building HDF4 libraries!
        goto error
    )

    call :test
    if %errorlevel% neq 0 (
        echo.Error testing HDF4 libraries!
        goto error
    )

    if "%nerrors%"=="0" (
        echo. All HDF4 libraries and tools build and tested successfully!
    )

    rem Fall through to end

:end
    popd
    endlocal & exit /b %nerrors%

