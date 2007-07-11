@ECHO OFF
@REM File Name: hdf4bt.bat
@REM This batch file is used to build and test HDF4 Libraries and Utilities.
@REM There are two options for this batch file:
@REM   1. hdf5bt                -- Build and test HDF4 tools and c library
@REM   3. hdf5bt enablefortran  -- Build and test HDF4 tools and c/fortran library
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 07/05/2005

type nul > hdf4_results.txt
echo ***************************************************************************** >> hdf4_results.txt
echo                         Build and Test HDF4 Library and Utilities >> hdf4_results.txt
echo ***************************************************************************** >> hdf4_results.txt

call hdf4build %1
more build_results.txt >> hdf4_results.txt
del build_results.txt
call hdf4check %1
more check_results.txt >> hdf4_results.txt
del check_results.txt
