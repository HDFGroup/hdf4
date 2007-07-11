@REM File Name: win32releasetst.bat
@REM This batch file is used to test release version of HDF4 Libraries and Utilities.
@REM There are two options for this batch file:
@REM   1. win32releasetst                -- Test HDF4 tools and c library
@REM   2. win32releasetst enablefortran  -- Test HDF4 tools and c/fortran library
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 07/05/2005

@ECHO OFF

echo ***************************************************************************** 
echo                         HDF4 C Library and Utilities Tests -- Release
echo *****************************************************************************

echo Test Release Version:

echo.
echo ==============================
echo testhdf
echo ==============================
cd testhdf\release
mkdir testdir
copy ..\..\hdf\test\*.dat . >temp.txt
del temp.txt
testhdf
del *.dat
del *.hdf
rmdir /s/q testdir

echo.
echo ==============================
echo xdrtest
echo ==============================
cd ..\..\xdrtest\release
xdrtest
del test.xdr

echo.
echo ==============================
echo HDF Utilities tests started
echo ==============================
cd ..\..
call utiltest release

echo.
echo ============================
echo HDF-SD C interfaces tests
echo ============================
REM generate the netCDF file test1.nc so hdftest.exe will work
cd ncgen\release\
ncgen -b ..\..\mfhdf\ncgen\test0.cdl
cd ..\..\hdftest\release
copy ..\..\ncgen\release\test0.nc test1.nc >temp.txt
del temp.txt
hdftest.exe > hdfout.new
fc hdfout.new ..\..\mfhdf\libsrc\hdfout.sav > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** HDF passes formatted test ***
) else (
   echo *** HDF fails formatted test ***
   more temp.txt
)
del temp.txt
del *.hdf
del test1.nc
del hdfout.new
del ..\..\ncgen\release\test0.nc

echo.
echo =========================
echo netCDF formatted tests
echo =========================
cd ..\..\cdftest\release
cdftest.exe > testout.new
fc testout.new ..\..\mfhdf\libsrc\testout.sav > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** netCDF passes formatted test ***
) else (
   echo *** netCDF fails formatted test ***
   more temp.txt
)
del temp.txt
del testout.new
del test.cdf

echo.
echo =========================
echo ncdump test
echo =========================
cd ..\..\windows\bin\release
mkdir temptest
cd temptest
copy ..\..\..\..\mfhdf\ncdump\test0.cdl . > temp.txt
del temp.txt

..\..\..\..\ncgen\release\ncgen -o test0.nc -n test0.cdl
..\..\..\..\ncdump\release\ncdump test0.nc > test1.cdl
..\..\..\..\ncgen\release\ncgen -o test1.nc -n test1.cdl
..\..\..\..\ncdump\release\ncdump -n test0 test1.nc > test2.cdl
fc test1.cdl test2.cdl > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** ncdump test successful ***
) else (
   echo *** ncdump test failed ***
   more temp.txt
)
del test1.cdl
del test2.cdl
del temp.txt
del test*.nc

echo.
echo =========================
echo ncgen -b test
echo =========================
..\..\..\..\ncgen\release\ncgen -b test0.cdl
..\..\..\..\ncdump\release\ncdump -n test1 test0.nc > test1.cdl
..\..\..\..\ncgen\release\ncgen -b test1.cdl
..\..\..\..\ncdump\release\ncdump test1.nc > test2.cdl
fc test1.cdl test2.cdl >temp.txt
if %ERRORLEVEL%==0 (
   echo *** ncgen -b test successful ***
) else (
   echo *** ncgen -b test failed ***
   more temp.txt
)
cd ..
rmdir /s/q temptest
cd ..\..\..

echo.
echo =============================
echo Running hdp tests
echo =============================
call hdptest release

echo.
echo ==============================
echo HDFIMPORT tests started
echo ==============================
call importtest release

echo.
echo ==============================
echo HDFDIFF tests started
echo ==============================
call difftest release

echo.
echo ==============================
echo HDFREAPCK tests started
echo ==============================
cd windows\bin\release
copy ..\..\..\mfhdf\hrepack\image*.txt . > temp.txt
del temp.txt
hrepacktst
del image*.txt
del *.hdf
cd ..\..\..

echo.
echo ==============================
echo nctest
echo ==============================
cd nctest\Release
copy ..\..\mfhdf\nctest\test_unlim.nc test_unlim.nc > temp.txt
nctest
del temp.txt
del *.nc

cd ..\..

echo.
echo.
echo Test Release DLL Version:

echo.
echo ==============================
echo dlltesthdf
echo ==============================
cd dlltesthdf\release
mkdir testdir
copy ..\..\hdf\test\*.dat . >temp.txt
del temp.txt
dlltesthdf
del *.dat
del *.hdf
rmdir /s/q testdir
cd ..\..\

echo.
echo ==============================
echo dllxdrtest
echo ==============================
cd dllxdrtest\release
dllxdrtest
del test.xdr

echo.
echo ============================
echo HDF-SD C interfaces DLL tests
echo ============================
REM generate the netCDF file test1.nc so hdftest.exe will work
cd ..\..\ncgen\release\
ncgen -b ..\..\mfhdf\ncgen\test0.cdl
cd ..\..\hdftest\release
copy ..\..\ncgen\release\test0.nc test1.nc >temp.txt
del temp.txt
hdftest.exe > hdfout.new
fc hdfout.new ..\..\mfhdf\libsrc\hdfout.sav > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** HDF passes formatted test ***
) else (
   echo *** HDF fails formatted test ***
   more temp.txt
)
del temp.txt
del *.hdf
del test1.nc
del hdfout.new
del ..\..\ncgen\release\test0.nc

echo.
echo ==========================
echo netCDF DLL formatted tests
echo ==========================
cd ..\..\dllcdftest\release
dllcdftest > testout.new
fc testout.new ..\..\mfhdf\libsrc\testout.sav > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** netCDF passes formatted test ***
) else (
   echo *** netCDF fails formatted test ***
   more temp.txt
)
del temp.txt
del testout.new
del test.cdf

echo.
echo ==============================
echo dllnctest
echo ==============================
cd ..\..\dllnctest\release
copy ..\..\mfhdf\nctest\test_unlim.nc .\test_unlim.nc  > temp.txt
del temp.txt
dllnctest
del *.nc
cd ..\..


if "%1"=="enablefortran" (

echo *****************************************************************************
echo                         HDF4 Fortran Library and Utilities Tests -- Release
echo *****************************************************************************

echo.
echo.
echo Test Release Fortran Version:
echo.
echo ===========================================
echo HDF Library Fortran Interface Tests Setup
echo ===========================================
cd fortestf\Release
copy ..\..\hdf\test\*.dat . >temp.txt
del temp.txt
copy ..\..\hdf\test\fort_ps\fortest.arg . >temp.txt
del temp.txt
fortestf.exe
del *.dat
del fortest.arg
cd ..\..\

echo.
echo ===================================
echo HDF-SD Fortran interfaces tests
echo ===================================
cd hdftestf\release
mkdir testdir
hdftestf.exe
del *.hdf
rmdir /s/q testdir
cd ..\..\

echo.
echo.
echo Test Release Fortran DLL Version:

echo.
echo =============================================
echo HDF Library Fortran Interface DLL Tests Setup
echo =============================================
cd dllfortestf\release
copy ..\..\hdf\test\*.dat .  > temp.txt
del temp.txt
copy ..\..\hdf\test\fort_ps\fortest.arg .  > temp.txt
del temp.txt
dllfortestf
del *.dat
del fortest.arg
cd ..\..\

echo.
echo ===================================
echo HDF-SD Fortran interfaces DLL tests
echo ===================================
cd dllhdftestf\release
mkdir testdir
dllhdftestf
del *.hdf
rmdir /s/q testdir
cd ..\..\
)
