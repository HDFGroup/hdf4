@echo off
rem File Name: hdf4build.bat
rem This batch file is used to build HDF4 Libraries and Tools.
rem There batch file takes the following options:
rem      Usage: hdf4build [[/?] | [vs8] [enablefortran]]
rem      Options:
rem         vs8               Build using Visual Studio 2005 [default
rem                           uses Visual Studio .NET]
rem         enablefortran     Build HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem By Xuan Bai
rem Created: 11/08/2004
rem Last Updated: 8/3/2007

rem This batch file makes the following assumptions:
rem    - The appropriate version of Visual Studio is installed and setup
rem    - The directory structure is setup from a fresh source copy
rem    - copy_hdf.bat has already been run from the ./windows directory
rem    - Visual Studio already contains the required paths for external libraries
rem    - szip, jpeg, and zlib DLLs are already placed in an accessible directory
rem    - If hdf4_ext_szip, hdf4_ext_jpeg, or hdf4_ext_zlib are not set, then %szip_name%,
rem      %jpeg_name%, and %zlib_name% will be used, respectively.

rem By default, only C libraries are built.

goto main

rem Print a help message
:help

    echo.Builds HDF4 Libraries and Tools.
    echo.
    echo.Usage: %~nx0 [[/?] | [vs8] [enablefortran]]
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

    exit /b 0


rem Parse through the parameters sent to file, and set appropriate variables
:parse_params

    for %%a in (%*) do (
        if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_use_vs2005=true
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_enablefortran=true
            
        ) else if "%%a"=="/?" (
            rem Set errorlevel 1 and send to help
            call :help
            exit /b 1
            
        ) else (
            rem Set errorlevel 2 to send to help if we receive a bad parameter (or /?)
            call :help
            exit /b 2
        )
    )
    
    exit /b 0


rem Setup our environment
:setup

    if "x%build_results%"=="x" set build_results="%CD%\build_results.txt"
    type nul > %build_results%
    
    echo.   Setting up environment >> %build_results%

    rem Put us in the directory of the batch file.
    pushd %~dp0

    rem Constants
    if "x%zlib_name%"=="x" set zlib_name=zdll.lib
    if "x%szip_name%"=="x" set szip_name=szlibdll.lib
    if "x%jpeg_name%"=="x" set jpeg_name=libjpeg.lib
    
    rem By default, use all.sln, unless hdf4_enablefortran is set
    if defined hdf4_enablefortran (
        echo.Building Fortran projects enabled
        echo.Building Fortran projects enabled >> %build_results%
        set hdf4_sln="%CD%\windows\proj\all_fortran\all_fortran.sln"
    ) else set hdf4_sln="%CD%\windows\proj\all\all.sln"
    
    rem Set external libraries if they aren't already set
    if "x%hdf4_ext_zlib%"=="x" set hdf4_ext_zlib=%zlib_name%
    if "x%hdf4_ext_jpeg%"=="x" set hdf4_ext_jpeg=%jpeg_name%
    if "x%hdf4_ext_szip%"=="x" set hdf4_ext_szip=%szip_name%

    
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
        devenv %hdf4_sln% /rebuild %%a >> %build_results% 2>&1
        if %errorlevel% neq 0 exit /b
    )

    exit /b


rem Handle errors
:error

    rem For now, our error handling just consists of calling cleanup, and setting errorlevel
    echo.HDF4 build failed.
    echo.HDF4 build failed. >> %build_results%
    call :cleanup

    exit /b 1


rem Cleanup our environment
:cleanup

    set zlib_name=
    set szip_name=
    set jpeg_name=
    set hdf4_enablefortran=
    set hdf4_use_vs2005=
    set hdf4_sln=

    rem Don't unset build_results if it is being used by hdf4bt.BAT
    if "x%bt_results%"=="x" set build_results=
    popd

    exit /b 0


rem This is where the magic happens
:main

    call :parse_params %*
    if %errorlevel% neq 0 (
        if %errorlevel% equ 2 echo.Error parsing parameters!
        exit /b
    )
    
    call :setup

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
        echo.   Error building HDF4 libraries!
        echo.   Error building HDF4 libraries! >> %build_results%
        goto error
    )

    echo. All HDF4 libraries and tools build successfully!
    echo. All HDF4 libraries and tools build successfully! >> %build_results%

    call :cleanup

    exit /b 0
