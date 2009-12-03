@echo off
rem File Name: hdf4build.bat
rem This batch file is used to build HDF4 Libraries and Tools.
rem There batch file takes the following options:
rem         vs9               Build using Visual Studio 2008
rem         vs8               Build using Visual Studio 2005
rem         enablefortran     Build HDF4 C/Fortran Library and Tools 
rem                           [default C only]
rem         ivf101            Build HDF4 Fortran using Intel Visual Fortran 10.1
rem         ivf91             Build HDF4 Fortran using Intel Visual Fortran 9.1
rem                           [default Intel Visual Fortran 9.1]
rem         nodebug           Note: Default is to build debug and release versions
rem         useenv            Build using variables set in the environment.
rem
rem By Xuan Bai
rem Created: 11/08/2004
rem Last Updated: Scott Wegner, 7/7/08

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
rem    - If building Fortran libraries, the default version of Visual Studio
rem      associated with Intel Visual Fortran is also specified (this is
rem      chosen when Intel Visual Fortran is installed)

rem By default, only C libraries are built.

setlocal enabledelayedexpansion
pushd %~dp0

set nerrors=0
if "%1"=="/?" goto help
set blddebug=debug
set bldrelease=release
goto main

rem Print a help message
:help

    echo.Builds HDF4 Libraries and Tools.
    echo.
    echo.Usage: %~nx0 [OPTIONS]
    echo.
    echo.   /?                      Help information
    echo.   vs9                     Build using Visual Studio 2008
    echo.   vs8                     Build using Visual Studio 2005
    echo.   enablefortran           Build HDF4 C/Fortran Library and Tools 
    echo.                           [default C only]
    echo.   ivf101                  Build HDF4 Fortran using Intel Visual Fortran 10.1
    echo.   ivf91                   Build HDF4 Fortran using Intel Visual Fortran 9.1
    echo.                           [default Intel Visual Fortran 9.1]
    echo.   nodebug                 Note: Default is to build debug and release versions
    echo.   useenv                  Build using variables set in the environment.
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
            set hdf4_use_vs2008=true
            
        ) else if "%%a"=="vs8" (
            rem Use VS2005 as our compiler
            set hdf4_use_vs2005=true
                        
        ) else if "%%a"=="enablefortran" (
            rem Enable Fortran
            set hdf4_enablefortran=true
            
        ) else if "%%a"=="ivf91" (
            rem Enable Fortran
            set hdf4_use_ivf91=true
            
        ) else if "%%a"=="ivf101" (
            rem Enable Fortran
            set hdf4_use_ivf101=true
            
         ) else if "%%a"=="nodebug" (
            rem Enable Fortran
            set blddebug=
            
       ) else if "%%a"=="useenv" (
            rem Tell the compiler to use variable settings in the environment 
            set hdf4_useenv=true
            
        ) else (
            rem Set errorlevel if we receive a bad parameter 
            echo.Unknown option: %%a
            call :help
            exit /b 1
        )
    )
    
    exit /b 0


rem Setup our environment
:setup

    echo.Setting up environment
    
    rem Sanity checks
    rem Only Intel Fortran 10.1 is supported on VS2008
    if defined hdf4_use_vs2008 (
        if defined hdf4_enablefortran (
            if not defined hdf4_use_ivf101 (
                echo.Error: Intel Visual Fortran 9.1 is not supported under Visual Studio 2008.
                exit /b 1
            )
        )
    )
    rem Only VS2005 and VS 2008 are supported on x64
    if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
        if not "%hdf4_use_vs2005%%hdf4_use_vs2008%"=="true" (
            echo.Error: Only Visual Studio 2005 and 2008 are supported on 64-bit Windows.
            exit /b 1
        )
    )
    rem Make sure we chose exactly one compiler above
    if not "%hdf4_use_vs2008%%hdf4_use_vs2005%"=="true" (
        echo.Error: Must specify exactly one C++ compiler to build with.
        exit /b 1
    )
    rem Make sure PROCESSOR_ARCHITECURE is set to either x86 or AMD64
    if "%PROCESSOR_ARCHITECTURE%"=="x86" (
        set hdf4_platform=Win32
    ) else if "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
        set hdf4_platform=x64
    ) else (
        echo.Error: Environment variable PROCESSOR_ARCHITECTURE should contain
        echo.either x86 or AMD64
        exit /b 1
    )
    rem Make sure we have environment variables specified for our compilers
    if defined hdf4_enablefortran (
        if defined hdf4_use_ivf101 (
            if not defined ifort_compiler10 (
                echo.Error: Cannot setup Intel Visual Fortran 10.1 environment.  Please
                echo.make sure IFORT_COMPILER10 is defined in the environment.
                exit /b 1
            )
        ) else (
            if not defined ifort_compiler91 (
                echo.Error: Cannot setup Intel Visual Fortran 9.1 environment.  Please
                echo.make sure IFORT_COMPILER91 is defined in the environment.
                exit /b 1
            )
        )
    ) else if defined hdf4_use_vs2005 (
        if not defined vs80comntools (
            echo.Error: Cannot setup Visual Studio 2005 environment.  Please
            echo.make sure VS80COMNTOOLS is defined in the environment.
            exit /b 1
        )
    ) else (
        rem Assume Visual Studio 2008
        if not defined vs90comntools (
            echo.Error: Cannot setup Visual Studio 2008 environment.  Please
            echo.make sure VS90COMNTOOLS is defined in the environment.
            exit /b 1
        )
    )
    rem Make sure we only build Fortran with the default Visual Studio
    if defined hdf4_enablefortran (
        call :check-ifort-vs
        if !errorlevel! neq 0 (
            echo.Error: The version of Visual Studio specified is not compatible with
            echo.the installed Intel Visual Fortran.  You may change the Visual Studio
            echo.associated with Intel Visual Fortran by selecting it in the Windows
            echo.Add/Remove dialog and clicking "Change".
            exit /b 1
        )
    )

    rem Figure out which solution file to use based on configuration, and
    rem setup Visual Studio environment
    if defined hdf4_use_vs2005 (
        echo.Using Visual Studio 2005
        rem Visual Studio 2005 is more complicated, because we can have either
        rem Fortran or not, and 32- or 64-bit.  Check for 4 possible situations
        if defined hdf4_enablefortran (
            if defined hdf4_use_ivf101 (
                if %hdf4_platform%==Win32 (
                    call "%ifort_compiler10%\IA32\Bin\ifortvars.bat"
                ) else (
                    call "%ifort_compiler10%\em64t\Bin\ifortvars.bat"
                )
            ) else (
                if %hdf4_platform%==Win32 (
                    call "%ifort_compiler91%\IA32\Bin\ifortvars.bat"
                ) else (
                    call "%ifort_compiler91%\em64t\Bin\ifortvars.bat"
                )
            )
            set hdf4_sln="%CD%\windows\proj\all_fortran\all_fortran.sln"
        ) else (
            if %hdf4_platform%==Win32 (
                call "%vs80comntools%\..\..\VC\vcvarsall.bat" x86
            ) else (
                call "%vs80comntools%\..\..\VC\vcvarsall.bat" x86_amd64
            )
            set hdf4_sln="%CD%\windows\proj\all\all.sln"
        )
        
    ) else (
        rem Assume VS2008
        echo.Using Visual Studio 2008
        rem Visual Studio 2008 is more complicated, because we can have either
        rem Fortran or not, and 32- or 64-bit.
        if defined hdf4_enablefortran (
            if %hdf4_platform%==Win32 (
                call "%ifort_compiler10%\IA32\Bin\ifortvars.bat"
            ) else (
                call "%ifort_compiler10%\em64t\Bin\ifortvars.bat"
            )
            set hdf4_sln="%CD%\windows\proj\all_fortran\all_fortran.sln"
        ) else (
            if %hdf4_platform%==Win32 (
                call "%vs90comntools%\..\..\VC\vcvarsall.bat" x86
            ) else (
                call "%vs90comntools%\..\..\VC\vcvarsall.bat" x86_amd64
            )
            set hdf4_sln="%CD%\windows\proj\all\all.sln"
        )
    )
            
    rem See if "useenv" was specified
    if defined hdf4_useenv (
        rem This will tell Visual Studio to use include, library, etc. paths
        rem defined by %INCLUDE% %LIB%, etc.  Assume the user has already
        rem added external library paths to these variables.
        echo.Using compiler settings from environment.
        set hdf4_cflags=/useenv
    )

    exit /b 0


rem Make sure that the version of Visual Studio we're using is the same as
rem the version that has been set up with the Intel Visual Fortran.
rem This function returns 0 if everything is OK, and 1 otherwise.
:check-ifort-vs

    if defined hdf4_use_vs2005 (
        if defined hdf4_use_ivf101 (
            findstr /c:"Microsoft Visual Studio 8" "%ifort_compiler10%\IA32\Bin\ifortvars.bat" > nul
        ) else (
            findstr /c:"Microsoft Visual Studio 8" "%ifort_compiler91%\IA32\Bin\ifortvars.bat" > nul
        )
        
    ) else (
        rem Assume VS2008
        findstr /c:"Microsoft Visual Studio 9.0" "%ifort_compiler10%\IA32\Bin\ifortvars.bat" > nul
    )
    exit /b %errorlevel%
        
    

rem Upgrade the project files to the latest format for Visual Studio
:upgrade
    
    echo.Upgrading project files
    devenv %hdf4_sln% /Upgrade /NoLogo
    
    exit /b


rem Build the HDF4 libraries.  By default, C libraries are built.
:all

    echo.*****************************************************************************
    echo.                        Build HDF4 Libraries and Tools
    echo.*****************************************************************************
    echo.
    

    rem Build both debug and release versions
    for %%a in (%blddebug% %bldrelease%) do (
        echo.Building %hdf4_platform% %%a libraries...
        rem Modify our command-line syntax slightly for each compiler, because they
        rem each expect different parameters
        rem Assume Visual Studio 2005/2008
        devenv %hdf4_sln% %hdf4_cflags% /Build "%%a|%hdf4_platform%"
        if !errorlevel! neq 0 ( 
            exit /b
        )
    )

    exit /b 0


rem Handle errors
:error

    rem For now, our error handling just consists of setting nerrors and calling end
    echo.HDF4 build failed.
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
        echo.Error setting up build environment.
        goto error
    )
    
    rem Upgrade project files if needed
    if defined hdf4_use_vs2008 (
        call :upgrade
        if !errorlevel! neq 0 (
            echo.Error upgrading project files!
            goto error
        )
    )

    call :all
    if %errorlevel% neq 0 (
        echo.Error building HDF4 libraries!
        goto error
    )

    if "%nerrors%"=="0" (
        echo. All HDF4 libraries and tools build successfully!
    )

    rem Fall throught to end

:end
    popd
    endlocal & exit /b %nerrors%

