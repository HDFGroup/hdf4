@echo off
rem File Name: hdf4bt.bat
rem This batch file is used to build and test HDF4 Libraries and Utilities.
rem There batch file takes the following options:
rem      Usage: hdf4bt [/?] ^| ^(vs8 ^| vnet ^| vs6^) [enablefortran] [disableszip] [useenv]
rem      Options:
rem         vs8               Build using Visual Studio 2005 
rem         vnet              Build using Visual Studio .NET 
rem         vs6               Build using Visual Studio 6.0 
rem         enablefortran     Build and test HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem         disableszip       Disable szip library in HDF4
rem 
rem         useenv            Build using variables set in the environment.
rem
rem By Xuan Bai
rem Created: 11/08/2004
rem Last Updated: 11/11/2007

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

rem By default, only C libraries are built and tested.

setlocal enabledelayedexpansion
pushd %~dp0

set nerrors=0
if not "%1"=="/?" goto main

rem Print a help message
:help

    echo.Builds and tests HDF4 libraries and utilities.
    echo.
    echo.Usage: %~nx0 [/?] ^| ^(vs8 ^| vnet ^| vs6^) [enablefortran] [disableszip] [useenv]
    echo.
    echo.Options:
    echo.
    echo.   /?                      Help information
    echo.
    echo.   vs8                     Build using Visual Studio 2005 
    echo.
    echo.   vnet                    Build using Visual Studio .NET 
    echo.
    echo.   vs6                     Build using Visual Studio 6.0 
    echo.
    echo.   enablefortran           Build and testHDF4 C/Fortran Library and 
    echo.                           Tools [default C only]
    echo.
    echo.   disableszip             Disable szip library in HDF4
    echo.
    echo.   useenv                  Build using variables set in the environment.
    echo.
    echo.
    echo.Specifying useenv requires you to set the INCLUDE and LIB variables to the
    echo.directories of all external libraries.  This overrides any include or library
    echo.directories set in the IDE.

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    for %%a in (%*) do (
        if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_build_params=!hdf4_build_params! vs8
            
        ) else if "%%a"=="vnet" (
            rem Use VS.NET as our compiler
            set hdf4_build_params=!hdf4_build_params! vnet
            
        ) else if "%%a"=="vs6" (
            rem Use VS 6.0 as our compiler
            set hdf4_build_params=!hdf4_build_params! vs6
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_build_params=!hdf4_build_params! enablefortran
            set hdf4_test_params=!hdf4_test_params! enablefortran
            
        ) else if "%%a"=="disableszip" (
            rem Completely disable szip
            set hdf4_build_params=!hdf4_build_params! disableszip
            
        ) else if "%%a"=="useenv" (
            rem Set the compiler to use environment variables for settings
            set hdf4_build_params=!hdf4_build_params! useenv
            
        ) else if "%%a"=="/?" (
            rem Set errorlevel 1 and send to help
            
            call :help
            exit /b 1
            
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

    rem Constants
    if "%bt_results%"=="" set bt_results="%CD%\bt_results.txt"
    if "%build_results%"=="" set build_results="%CD%\build_results.txt"

    rem Create our results file
    type nul > %bt_results%

    exit /b 0


rem Verify our environment is ok before build
:verify_env

    rem Make sure we specified a compiler
    if not defined hdf4_build_params (
        echo.Error: must specify a compiler
        exit /b 1

    ) 
    if "%hdf4_build_params:vs8=%"=="%hdf4_build_params%" (
    if "%hdf4_build_params:vnet=%"=="%hdf4_build_params%" (
    if "%hdf4_build_params:vs6=%"=="%hdf4_build_params%" (
        rem if we get here, that means we didn't find any compiler
        echo.Error: must specify a compiler
        exit /b 1
    ) ) )

    rem find build and test scripts
    for %%a in (hdf4build.bat hdf4check.bat) do (
        if not exist %%a (
            echo.Error: cannot find %%a
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

    rem For now, our error handling just consists of setting nerrors and calling end
    echo.hdf4bt failed.
    echo.hdf4bt failed. >> %bt_results%
    set /a nerrors=%nerrors%+1
    goto end
    
    rem We'll never really get here, but we keep this line for consistency
    exit /b 


    
rem This is where the magic happens
:main

    rem prefix params with " to escape help string
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
        echo. All HDF4 libraries and tools build and tested successfully! >> %bt_results%
    )

    rem Fall through to end

:end
    popd
    endlocal & exit /b %nerrors%

