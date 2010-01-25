@echo OFF
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
rem access to either file, you may request a copy from helphdfgroup.org.


rem File Name: hdf4build_examples.bat
rem This batch file is used to build HDF4 C/Fortran examples.
rem This batch file takes the following options:
rem .  vs9               Build examples using Visual Studio 2008
rem .  vs8               Build examples using Visual Studio 2005
rem .  ivf111            Build HDF4 Fortran examples using Intel Visual Fortran 11.1
rem .  ivf101            Build HDF4 Fortran examples using Intel Visual Fortran 10.1
rem .  ivf91             Build HDF4 Fortran examples using Intel Visual Fortran 9.1
rem                           [default Intel Visual Fortran 10.1]
rem .  enablefortran     Build HDF4 examples, including Fortran
rem                           [default C only]
rem .  useenv            Build HDF4 examples using compiler settings defined
rem .  nodebug           Note: Default is to build debug and release versions
rem .                    in the environment, rather than the IDE.
rem .  /?                Help information

rem This batch file makes the following assumptions:
rem    - The appropriate version of Visual Studio is installed and setup
rem    - The directory structure is setup from a fresh source copy
rem    - copy_hdf.bat has already been run from the ./windows directory
rem    - HDF4 has already been built using standard settings
rem    - Visual Studio already contains the required paths for external libraries
rem    - szip and zlib DLLs are already placed in an accessible directory
rem    - hdf4_ext_szip or hdf4_ext_zlib have been set accordingly
rem    - if building with the /useenv option, szip and zlib paths have been added
rem      to %include% and %libpath% as necessary.

rem By default, only C examples are built.

setlocal enabledelayedexpansion
pushd %~dp0

set nerrors=0
if "%1"=="/?" goto help
set blddebug=debug
set bldrelease=release
goto main

rem Print a help message
:help

    echo.Builds HDF4 example projects.
    echo.
    echo.Usage: %~nx0 [OPTION]
    echo.
    echo.   vs9              Build using Visual Studio 2008
    echo.   vs8              Build using Visual Studio 2005
    echo.   ivf111           Build HDF4 Fortran using Intel Visual Fortran 11.1
    echo.   ivf101           Build HDF4 Fortran using Intel Visual Fortran 10.1
    echo.   ivf91            Build HDF4 Fortran using Intel Visual Fortran 9.1
    echo.                           [default Intel Visual Fortran 10.1]
    echo.   enablefortran    Build HDF4 examples, including Fortran
    echo.                           [default C only]
    echo.   nodebug          Note: Default is to build debug and release versions
    echo.   useenv           Build HDF4 examples using compiler settings defined
    echo.                     in the environment, rather than the IDE.
    echo.   /?                Help information

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
            
        ) else if "%%a"=="ivf111" (
            rem Enable Fortran
            set hdf4_use_ivf111=true
            
        ) else if "%%a"=="nodebug" (
            rem Enable Fortran
            set blddebug=
            
        ) else if "%%a"=="useenv" (
            rem Pass /useenv flag to devenv
            set hdf4_useenv=true
            
        ) else (
            rem Set errorlevel 2 to send to help if we receive a bad parameter
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
    rem Only Intel Fortran 10.1 and 11.1 is supported on VS2008
    if defined hdf4_use_vs2008 (
        if defined hdf4_enablefortran (
            if defined hdf4_use_ivf91 (
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
        if defined hdf4_use_ivf111 (
            if not defined ifort_compiler11 (
                echo.Error: Cannot setup Intel Visual Fortran 11.1 environment.  Please
                echo.make sure IFORT_COMPILER11 is defined in the environment.
                exit /b 1
            )
        ) else if defined hdf4_use_ivf91 (
            if not defined ifort_compiler91 (
                echo.Error: Cannot setup Intel Visual Fortran 9.1 environment.  Please
                echo.make sure IFORT_COMPILER91 is defined in the environment.
                exit /b 1
            )
        ) else (
            if not defined ifort_compiler10 (
                echo.Error: Cannot setup Intel Visual Fortran 10.1 environment.  Please
                echo.make sure IFORT_COMPILER10 is defined in the environment.
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
            ) else if defined hdf4_use_ivf91 (
                if %hdf4_platform%==Win32 (
                    call "%ifort_compiler91%\IA32\Bin\ifortvars.bat"
                ) else (
                    call "%ifort_compiler91%\em64t\Bin\ifortvars.bat"
                )
            )
        ) else (
            if %hdf4_platform%==Win32 (
                call "%vs80comntools%\..\..\VC\vcvarsall.bat" x86
            ) else (
                call "%vs80comntools%\..\..\VC\vcvarsall.bat" x86_amd64
            )
        )
        
    ) else (
        rem Assume VS2008
        echo.Using Visual Studio 2008
        rem Visual Studio 2008 is more complicated, because we can have either
        rem Fortran or not, and 32- or 64-bit.
        if defined hdf4_enablefortran (
			if defined hdf4_use_ivf111 (
				if %hdf4_platform%==Win32 (
					call "%ifort_compiler11%Bin\IA32\ifortvars_ia32.bat"
				) else (
					call "%ifort_compiler11%Bin\intel64\ifortvars_intel64.bat"
				)
            ) else if defined hdf4_use_ivf101 (
				if %hdf4_platform%==Win32 (
					call "%ifort_compiler10%\IA32\Bin\ifortvars.bat"
				) else (
					call "%ifort_compiler10%\em64t\Bin\ifortvars.bat"
				)
            )
        ) else (
            if %hdf4_platform%==Win32 (
                call "%vs90comntools%\..\..\VC\vcvarsall.bat" x86
            ) else (
                call "%vs90comntools%\..\..\VC\vcvarsall.bat" x86_amd64
            )
        )
    )
    
    rem Setup variables for our SLN files
    set C_SLN=%CD%\windows\proj\hdf\examples\allcexamples.sln
    set C_MF_SLN=%CD%\windows\proj\mfhdf\examples\allexamples.sln
    if defined hdf4_enablefortran (
        set FORT_SLN=%CD%\windows\proj\hdf\fortran\examples\allf90examples.sln
        set FORT_MF_SLN=%CD%\windows\proj\mfhdf\fortran\examples\allf90examples.sln
    ) else (
        set FORT_SLN=
        set FORT_MF_SLN=
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
        if defined hdf4_use_ivf101 (
            findstr /c:"Microsoft Visual Studio 9.0" "%ifort_compiler10%IA32\Bin\ifortvars.bat" > nul
        ) else (
            findstr /c:"Microsoft Visual Studio 9.0" "%ifort_compiler11%Bin\IA32\ifortvars_ia32.bat" > nul
        )
    )
    exit /b %errorlevel%

 
rem Build the HDF4 examples.  By default, C examples are built.
:all

    echo.*****************************************************************************
    echo.                        Build HDF4 Examples
    echo.*****************************************************************************
    echo.
    

    rem Build both debug and release versions
     for %%a in (C C_MF FORT FORT_MF) do (
        if defined %%a_SLN (
            echo.**************************
            echo. Building %%a Examples
            echo.**************************
		    for %%b in (%blddebug% %bldrelease%) do (
				echo.Building %hdf4_platform% %%a %%b examples...
				rem Modify our command-line syntax slightly for each compiler, because they
				rem each expect different parameters
				rem Assume Visual Studio 2005/2008
				devenv !%%a_SLN! %hdf4_cflags% /rebuild "%%b|%hdf4_platform%"
				if !errorlevel! neq 0 ( 
					echo.HDF4 %%b %%a examples build failed
					exit /b %errorlevel%
				)
             )
        )
   )

    exit /b 0


rem Handle errors
:error

    rem For now, our error handling just consists of setting nerrors and calling end
    echo.HDF4 examples build failed.
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
    
    call :all
    if %errorlevel% neq 0 (
        echo.Error building HDF4 examples!
        goto error
    )

    if "%nerrors%"=="0" (
        echo. All HDF4 examples built successfully!
    )

    rem Fall throught to end

:end
    popd
    endlocal & exit /b %nerrors%

