@ECHO OFF
@REM File Name: hdf4build.bat
@REM This batch file is used to build HDF4 Libraries and Tools.
@REM There are two options for this batch file.
@REM   1. hdf5build                -- Build HDF5 tools and c library
@REM   3. hdf5build enablefortran  -- Build HDF5 tools and c/fortran library
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 07/05/2005

if "%1"=="" GOTO BUILDC
if "%1"=="/?" GOTO HELP
if "%1"=="enablefortran" GOTO BUILDALL
GOTO WRONG

:BUILDC
echo.
echo Building HDF4 C Library and TOOLS
echo.
type nul > build_results.txt

echo ***************************************************************************** >> build_results.txt
echo                         Build HDF4 C Library and Tools >> build_results.txt
echo ***************************************************************************** >> build_results.txt

msdev all.dsw /make "all - ALL" /Rebuild /out all.log
more all.log >> build_results.txt
del all.log
GOTO END

:BUILDALL
echo.
echo Building HDF4 C/Fortran Libraries and TOOLS
echo.
type nul > build_results.txt

echo ***************************************************************************** >> build_results.txt
echo                         Build HDF4 C/Fortran Library and Tools >> build_results.txt
echo ***************************************************************************** >> build_results.txt
msdev all.dsw /make "all - ALL" /Rebuild /out all.log
msdev all.dsw /make "fortestf - ALL" "hdftestf - ALL" "dllfortestf - ALL" "dllhdftestf - ALL" /rebuild /out all_fortran.log
more all_fortran.log >>all.log
more all.log >> build_results.txt
del all_fortran.log
del all.log
GOTO END

:WRONG
echo.
echo The syntax of the command is incorrect.
echo.

:HELP
echo There are 3 options for batch file hdf4build.bat
echo.
echo    hdf5build                   Build HDF5 C Library and Tools
echo    hdf5build enablefortran     Build HDF5 C/Fortran Libraries and Tools
echo    hdf5build /?                Help information
echo.
:END