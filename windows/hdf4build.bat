@echo off
rem File Name: hdf4build.bat
rem This batch file is used to build HDF4 Libraries and Tools.
rem There batch file takes the following options:
rem      Usage: hdf4build [/?] ^| ^(vs8 ^| vnet ^| vs6^) [enablefortran] [disableszip] [useenv]
rem      Options:
rem         vs8               Build using Visual Studio 2005 
rem         vnet              Build using Visual Studio .NET
rem         vs6               Build using Visual Studio 6.0
rem         enablefortran     Build HDF4 C/Fortran Library and Tools 
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

rem By default, only C libraries are built.

setlocal enabledelayedexpansion
pushd %~dp0

set nerrors=0
if not "%1"=="/?" goto main

rem Print a help message
:help

    echo.Builds HDF4 Libraries and Tools.
    echo.
    echo.Usage: %~nx0 [/?] ^| ^(vs8 ^| vnet ^| vs6 ^) [enablefortran] [disableszip] [useenv]
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
    echo.   enablefortran           Build HDF4 C/Fortran Library and Tools 
    echo.                           [default C only]
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

    rem Check Visual Studio environment
    for %%a in (%*) do (
        if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_use_vs2005=true
            
        ) else if "%%a"=="vnet" (
            rem Use VS.NET as our compiler
            set hdf4_use_vnet=true
            
        ) else if "%%a"=="vs6" (
            rem Use VS6 as our compiler
            set hdf4_use_vs6=true
            
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_enablefortran=true
            
        ) else if "%%a"=="disableszip" (
            rem Completely disable szip library
            set hdf4_disableszip=true
            
        ) else if "%%a"=="useenv" (
            rem Tell the compiler to use variable settings in the environment 
            set hdf4_useenv=true
            
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

    if "%build_results%"=="" set build_results="%CD%\build_results.txt"
    if "%hdf4_tmp%"=="" set hdf4_tmp="%CD%\tmp.txt"
    type nul > %build_results%
    type nul > %hdf4_tmp%
    
    echo.   Setting up environment >> %build_results%

    rem Make sure we chose exactly one compiler above
    if not "%hdf4_use_vs2005%%hdf4_use_vnet%%hdf4_use_vs6%"=="true" (
        echo.Error: Must specify exactly one compiler to build with.
        exit /b 1
    )

    rem Setup our project directory, file extension, and other compiler-
    rem specific switches
    if "%hdf4_use_vs2005%"=="true" (
        set hdf4_projdir="%CD%\windows\proj"
        set hdf4_projext=.sln
        set hdf4_build=/build
    ) else if "%hdf4_use_vnet%"=="true" (
        set hdf4_projdir="%CD%\windows\proj_vnet"
        set hdf4_projext=.sln
        set hdf4_build=/build
    ) else (
        rem Assume vs6
        set hdf4_projdir="%CD%\windows\proj_vs6"
        set hdf4_projext=.dsw
        set hdf4_build=/make
    )

    rem By default, use all.sln, unless hdf4_enablefortran is set
    if "%hdf4_enablefortran%"=="true" (
        echo.Building Fortran projects enabled
        echo.Building Fortran projects enabled >> %build_results%
        rem Use hdf4_projdir:"= to remove any surrounding quotes
        set hdf4_sln="%hdf4_projdir:"=%\all_fortran\all_fortran%hdf4_projext%"
    ) else set hdf4_sln="%hdf4_projdir:"=%\all\all%hdf4_projext%"
    
    rem Setup Intel Fortran environment, if needed.
    if "%hdf4_enablefortran%"=="true" (
        if not defined ifort_compiler91 (
            echo.Error: Cannot setup Intel Fortran 9.1 environment.  Please
            echo.make sure IFORT_COMPILER91 is defined in the environment.
            exit /b 1
         )

         rem Assume 32-bit environment
         call "%ifort_compiler91%\IA32\Bin\ifortvars.bat" 
    )

    rem Setup Visual Studio environment.
    if "%hdf4_use_vs2005%"=="true" (
        echo.Using Visual Studio 2005
        echo.Using Visual Studio 2005 >> %build_results%
        call "%vs80comntools%\vsvars32.bat" >> %build_results%
        set hdf4_cc=devenv

    ) else if "%hdf4_use_vnet%"=="true" (
        echo.Using Visual Studio .NET 2003
        echo.Using Visual Studio .NET 2003 >> %build_results%
        call "%vs71comntools%\vsvars32.bat" >> %build_results%
        set hdf4_cc=devenv

    ) else (
        rem Assume vs6
        echo.Using Visual Studio 6.0
        echo.Using Visual Studio 6.0 >> %build_results%
        call "%msdevdir%\..\..\VC98\Bin\vcvars32.bat" > %build_results%
        set hdf4_cc=msdev
    )

    rem See if "useenv" was specified
    if "%hdf4_useenv%"=="true" (
        echo.Using compiler settings from environment.
        echo.Using compiler settings from environment. >> %build_results%
        set hdf4_cflags=%hdf4_cflags% /useenv
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
    
    if "%hdf4_disableszip%"=="true" (
        rem Make sure we have h4config.h, so we can edit it
        if not exist hdf\src\h4config.h (
            echo.Error: Cannot find %cd%\hdf\src\h4config.h>&2
            exit /b 1
        )
    )

    if "%hdf4_useenv%"=="true" (
        rem Make sure we LIB and INCLUDE are defined
        if not defined include (
            echo.Error: INCLUDE must be defined in the environment if specifying useenv
            exit /b 1
        ) else if not defined lib (
            echo.Error: LIB must be defined in the environment if specifying useenv
            exit /b 1
        )
   )
 
    rem Make sure we're not trying to build Fortran libraries with VS6,
    rem not supported.  --SJW 11/7/07
    if "%hdf4_enablefortran% %hdf4_use_vs6%"=="true true" (
        echo.Error: Build Fortran libraries with Visual Studio 6.0 is
        echo.unsupported.  Please choose another compiler, or build without
        echo.Fortran libraries.
        exit /b 1
    )

    exit /b 0
    


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
    for %%a in (Debug Release) DO (
        rem Set a configuration variable that is specific to the compiler.
        rem Note: We need this because VS 6.0 , VS.NET, and VS 2005 all take
        rem slightly different options. --SJW 11/12/07
        if "%hdf4_use_vs6%"=="true" (
            set hdf4_config=/MAKE "all - Win32 %%a" /REBUILD
        ) else if "%hdf4_use_vnet%"=="true" (
            set hdf4_config=/Rebuild "%%a"
        ) else (
            set hdf4_config=/Rebuild "%%a|Win32"
        )
        echo.       Building %%a libraries...
        echo.       Building %%a libraries... >> %build_results%
        %hdf4_cc% %hdf4_sln% !hdf4_config! %hdf4_cflags% >> %build_results% 2>&1
        if !errorlevel! neq 0 ( 
            exit /b
        )
    )

    exit /b 0


rem Handle errors
:error

    rem For now, our error handling just consists of setting nerrors and calling end
    echo.HDF4 build failed.>&2
    echo.HDF4 build failed. >> %build_results%
    set /a nerrors=%nerrors%+1
    goto end

    rem We'll never really get here, but we keep this line for consistency
    exit /b

 
rem Cleanup our environment
:cleanup

    del /f /q %hdf4_tmp% > nul

    exit /b 0


rem This is where the magic happens
:main

    call :parse_params %*
    if %errorlevel% neq 0 (
        goto error
    )
    
    call :setup
    if %errorlevel% neq 0 (
        goto error
    )

    call :verify_env
    if %errorlevel% neq 0 goto error
    
    rem If szip is disabled, we need to edit h4config.h
    if "%hdf4_disableszip%"=="true" (
        call :disableszip
        if !errorlevel! neq 0 goto error
    )
    
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

    if "%nerrors%"=="0" (
        echo. All HDF4 libraries and tools build successfully!
        echo. All HDF4 libraries and tools build successfully! >> %build_results%
    )

    rem Fall throught to end

:end
    call :cleanup
    popd
    endlocal & exit /b %nerrors%

