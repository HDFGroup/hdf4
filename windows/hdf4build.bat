@echo off
rem File Name: hdf4build.bat
rem This batch file is used to build HDF4 Libraries and Tools.
rem There batch file takes the following options:
rem      Usage: hdf4build [/?] | [vs8] [enablefortran] [disableszip]
rem      Options:
rem         vs8               Build using Visual Studio 2005 [default
rem                           uses Visual Studio .NET]
rem         enablefortran     Build HDF4 C/Fortran Library and Tools 
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
rem    - jpeg, zlib, and szip (unless disabled) DLLs are already placed in an 
rem      accessible directory
rem    - hdf4_ext_jpeg, hdf4_ext_zlib, and hdf4_ext_szip (unless disabled) 
rem      should all be defined

rem By default, only C libraries are built.

if "%1"=="/?" (
    goto help
) else (
    goto main
)

rem Print a help message
:help

    echo.Builds HDF4 Libraries and Tools.
    echo.
    echo.Usage: %~nx0 [vs8] [enablefortran] [disableszip]
    echo.
    echo.Options:
    echo.
    echo.   /?                      Help information
    echo.
    echo.   vs8                     Build using Visual Studio 2005 [default
    echo.                           uses Visual Studio .NET]
    echo.
    echo.   enablefortran           Build HDF4 C/Fortran Library and Tools 
    echo.                           [default C only]
    echo.
    echo.   disableszip             Disable szip library in HDF4

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    rem Check Visual Studio environment
    for %%a in (%*) do (
        if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_use_vs2005=true
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_enablefortran=true
            
        ) else if "%%a"=="disableszip" (
            rem Completely disable szip library
            set hdf4_disableszip=true
            
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

    if "x%build_results%"=="x" set build_results="%CD%\build_results.txt"
    if "x%hdf4_tmp%"=="x" set hdf4_tmp="%CD%\tmp.txt"
    type nul > %build_results%
    
    echo.   Setting up environment >> %build_results%

    rem Put us in the directory of the batch file.
    pushd %~dp0
    
    rem By default, use all.sln, unless hdf4_enablefortran is set
    if defined hdf4_enablefortran (
        echo.Building Fortran projects enabled
        echo.Building Fortran projects enabled >> %build_results%
        set hdf4_sln="%CD%\windows\proj\all_fortran\all_fortran.sln"
    ) else set hdf4_sln="%CD%\windows\proj\all\all.sln"
    
    
    rem Setup Visual Studio environment.  By default, use the Visual Studio .NET environment
    if defined hdf4_use_vs2005 (
        echo.Using Visual Studio 2005
        echo.Using Visual Studio 2005 >> %build_results%
        call "%vs80comntools%\vsvars32.bat" >> %build_results%
    ) else (
        echo.Using Visual Studio .NET 2003
        echo.Using Visual Studio .NET 2003 >> %build_results%
        call "%vs71comntools%\vsvars32.bat" >> %build_results%
    )

    exit /b 0


rem Verify our environment is ok before build
:verify_env

    rem Find external libraries
    for %%a in (JPEG ZLIB SZIP) do (
        if not defined HDF4_EXT_%%a (
            echo.Error: HDF4_EXT_%%a not defined>&2
            exit /b 1
        )
    )
    
    if defined hdf4_disableszip (
        rem Make sure we have h4config.h, so we can edit it
        if not exist hdf\src\h4config.h (
            echo.Error: Cannot find %cd%\hdf\src\h4config.h>&2
            exit /b 1
        )
    )
    
    rem Make sure we have compiler
    devenv /? > nul
    if %errorlevel% neq 0 (
        echo.Error: Cannot find devenv in PATH>&2
        exit /b 1
    )
    
    rem find our sln file
    if not exist %hdf4_sln% (
        echo.Error: cannot find %hdf4_sln%>&2
        exit /b 1
    )
    
    exit /b 0


rem Convert project files to Visual Studio 2005 format
:convert

    echo.Converting project files

    echo.***************************************************************************** >> %build_results%
    echo.                        Convert HDF4 Project Files >> %build_results%
    echo.***************************************************************************** >> %build_results%
    echo. >> %build_results%

    devenv %hdf4_sln% /upgrade >> %build_results% 2>&1
    if %errorlevel% neq 0 exit /b

    exit /b
    

rem Make neccessary changes to disable szip
:disableszip

    echo.Szip library disabled
    echo.Szip library disabled >> %build_results%

    findstr /v /c:"#define H4_HAVE_LIBSZ 1" hdf\src\h4config.h > %hdf4_tmp%
    findstr /v /c:"#define H4_HAVE_SZLIB_H 1" %hdf4_tmp% > hdf\src\h4config.h
    if errorlevel 2 (
        echo.   Error updating h4config.h for no szip>&2
        echo.   Error updating h4config.h for no szip >> %build_results%
        exit /b
    )
    
    rem For Fortran, we need the macro defined, but we may leave it empty
    echo.|set /p hdf4_ext_szip=

    exit /b 0
    
    
rem Build the HDF4 libraries.  By default, C libraries are built.
:all

    echo.   Building HDF4

    echo.***************************************************************************** >> %build_results%
    echo.                        Build HDF4 Library and Tools >> %build_results%
    echo.***************************************************************************** >> %build_results%
    echo. >> %build_results%

    rem Build both debug and release versions
    for %%a in (debug release) DO (
        echo.       Building %%a libraries...
        echo.       Building %%a libraries... >> %build_results%
        devenv %hdf4_sln% /rebuild %%a /useenv >> %build_results% 2>&1
        if errorlevel 1 exit /b
    )

    exit /b


rem Handle errors
:error

    rem For now, our error handling just consists of calling cleanup, and setting errorlevel
    echo.HDF4 build failed.>&2
    echo.HDF4 build failed. >> %build_results%
    call :cleanup

    endlocal
    exit /b 1


rem Cleanup our environment
:cleanup

    set hdf4_enablefortran=
    set hdf4_disableszip=
    set hdf4_use_vs2005=
    set hdf4_sln=

    del /f /q %hdf4_tmp%
    
    rem Don't unset build_results if it is being used by hdf4bt.BAT
    if "x%bt_results%"=="x" set build_results=
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
    
    rem If szip is disabled, we need to edit h4config.h
    call :disableszip
    if %errorlevel% neq 0 goto error
    
    rem If we are using VS2005, we need to upgrade project files first
    if defined hdf4_use_vs2005 call :convert
    
    echo.Building HDF4 Libraries and Tools
    echo.

    echo.***************************************************************************** >> %build_results%
    echo.                        Build HDF4 Libraries and Tools >> %build_results%
    echo.***************************************************************************** >> %build_results%
    echo. >> %build_results%

    call :all
    if %errorlevel% neq 0 (
        echo.   Error building HDF4 libraries!>&2
        echo.   Error building HDF4 libraries! >> %build_results%
        goto error
    )

    echo. All HDF4 libraries and tools build successfully!
    echo. All HDF4 libraries and tools build successfully! >> %build_results%

    call :cleanup
    
    endlocal
    exit /b 0
