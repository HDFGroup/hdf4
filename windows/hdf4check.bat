@ECHO OFF
@REM File Name: hdf4check.bat
@REM This batch file is used to test HDF4 Libraries and Utilities.
@REM There are 3 options for this batch file:
@REM   1. hdf4check                -- HDF4 tools and c library tests
@REM   2. hdf4check enablefortran  -- HDF4 tools and c/fortran library tests
@REM   3. hdf4check /?             -- Help information
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 07/05/2005

if "%1"=="" GOTO TESTC
if "%1"=="enablefortran" GOTO TESTALL
if "%1" == "/?" GOTO HELP
GOTO WRONG

:TESTC
echo.
echo. Testing HDF4 C Library and TOOLS
echo.

echo install_dll
call install_dll
type nul > check_results.txt
echo Win32releasetst
call Win32releasetst >> check_results.txt 2>>&1

echo. >> check_results.txt
echo Win32debugtst
call Win32debugtst >> check_results.txt 2>>&1

GOTO END

:TESTALL
echo.
echo.Testing HDF4 C/Fortran Libaries and TOOLS
echo.
type nul > check_results.txt
echo install_dll
call install_dll
echo install_fortrandll
call install_fortrandll
echo Win32releasetst enablefortran
call Win32releasetst enablefortran>> check_results.txt 2>>&1

echo. >> check_results.txt
echo Win32debugtst enablefortran
call Win32debugtst enablefortran>> check_results.txt 2>>&1

GOTO END

:WRONG
echo.
echo The syntax of the command is wrong!
echo.

:HELP 
echo There are 3 options for batch file hdf4check.bat
echo.
echo   1. hdf5check                -- HDF5 tools and c library tests
echo   2. hdf5check enablefortran  -- HDF5 tools and c/fortran library tests
echo   3. hdf4check /?             -- Help information
echo.

:END