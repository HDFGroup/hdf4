@REM File Name: win32debugtst.bat
@REM This batch file is used to test debug version of HDF4 Libraries and Utilities.
@REM There are two options for this batch file:
@REM   1. win32debugtst                -- Test HDF4 tools and c library
@REM   2. win32debugtst enablefortran  -- Test HDF4 tools and c/fortran library
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 07/30/2007

@ECHO OFF

echo ***************************************************************************** 
echo                         HDF4 C Library and Utilities Tests -- Debug 
echo ***************************************************************************** 

echo Test Debug Version:

echo.
echo ==============================
echo testhdf
echo ==============================
pushd .\hdf\test\Debug
mkdir testdir
copy ..\*.dat . >temp.txt
del temp.txt
testhdf
del *.dat
del *.hdf
rmdir /s/q testdir
popd

echo.
echo ==============================
echo xdrtest
echo ==============================
pushd .\mfhdf\xdr\Debug
xdrtest
del test.xdr
popd

echo.
echo ==============================
echo HDF Utilities tests started
echo ==============================
call utiltest Debug

echo.
echo ============================
echo HDF-SD C interfaces tests
echo ============================
REM generate the netCDF file test1.nc so hdftest.exe will work
pushd .\mfhdf\ncgen\Debug
ncgen -b ..\test0.cdl
popd
pushd .\mfhdf\libsrc\Debug
copy ..\..\ncgen\Debug\test0.nc test1.nc >temp.txt
del temp.txt
hdftest.exe > hdfout.new
fc hdfout.new ..\hdfout.sav | find "FC: no diff" > temp.txt 2>&1
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
del ..\..\ncgen\Debug\test0.nc
popd

echo.
echo =========================
echo netCDF formatted tests
echo =========================
pushd .\mfhdf\libsrc\Debug
cdftest.exe > testout.new
fc testout.new ..\testout.sav | find "FC: no diff" > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** netCDF passes formatted test ***
) else (
   echo *** netCDF fails formatted test ***
   more temp.txt
)
del temp.txt
del testout.new
del test.cdf
popd

echo.
echo =========================
echo ncdump test
echo =========================
pushd .\mfhdf
mkdir temptest
pushd temptest
copy ..\ncdump\test0.cdl . > temp.txt
del temp.txt

..\ncgen\Debug\ncgen -o test0.nc -n test0.cdl
..\ncdump\Debug\ncdump test0.nc > test1.cdl
..\ncgen\Debug\ncgen -o test1.nc -n test1.cdl
..\ncdump\Debug\ncdump -n test0 test1.nc > test2.cdl
fc test1.cdl test2.cdl | find "FC: no diff" > temp.txt 2>&1
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
..\ncgen\Debug\ncgen -b test0.cdl
..\ncdump\Debug\ncdump -n test1 test0.nc > test1.cdl
..\ncgen\Debug\ncgen -b test1.cdl
..\ncdump\Debug\ncdump test1.nc > test2.cdl
fc test1.cdl test2.cdl | find "FC: no diff" >temp.txt
if %ERRORLEVEL%==0 (
   echo *** ncgen -b test successful ***
) else (
   echo *** ncgen -b test failed ***
   more temp.txt
)
popd
rmdir /s/q temptest
popd

echo.
echo =============================
echo Running hdp tests
echo =============================
REM call hdptest Debug

echo.
echo ==============================
echo HDFIMPORT tests started
echo ==============================
call importtest Debug

echo.
echo ==============================
echo HDFDIFF tests started
echo ==============================
call difftest Debug

echo.
echo ==============================
echo HDFREPACK tests started
echo ==============================
pushd .\mfhdf\hrepack\Debug
copy ..\image*.txt . > temp.txt
del temp.txt
hrepacktst
del image*.txt
del *.hdf
popd

echo.
echo ==============================
echo nctest
echo ==============================
pushd .\mfhdf\nctest\Debug
copy ..\test_unlim.nc . > temp.txt
nctest
del temp.txt
del *.nc
popd

echo.
echo.
echo Test Debug DLL Version:

echo.
echo ==============================
echo dlltesthdf
echo ==============================
pushd .\hdf\test\Debug
mkdir testdir
copy ..\*.dat . >temp.txt
del temp.txt
dlltesthdf
del *.dat
del *.hdf
rmdir /s/q testdir
popd

echo.
echo ==============================
echo dllxdrtest
echo ==============================
pushd .\mfhdf\xdr\Debug
dllxdrtest
del test.xdr
popd

echo.
echo ============================
echo HDF-SD C interfaces DLL tests
echo ============================
REM generate the netCDF file test1.nc so hdftest.exe will work
pushd .\mfhdf\ncgen\Debug
ncgen -b ..\test0.cdl
popd
pushd .\mfhdf\libsrc\Debug
REM put the file test1.nc in this directory
copy ..\..\ncgen\Debug\test0.nc test1.nc > temp.txt
del temp.txt
dllhdftest> hdfout.new
fc hdfout.new ..\hdfout.sav | find "FC: no diff" > temp.txt 2>&1
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
del ..\..\ncgen\debug\test0.nc
popd

echo.
echo ==========================
echo netCDF DLL formatted tests
echo ==========================
pushd .\mfhdf\libsrc\Debug
dllcdftest > testout.new
fc testout.new ..\testout.sav | find "FC: no diff" > temp.txt 2>&1
if %ERRORLEVEL%==0 (
   echo *** netCDF passes formatted test ***
) else (
   echo *** netCDF fails formatted test ***
   more temp.txt
)
del temp.txt
del testout.new
del test.cdf
popd

echo.
echo ==============================
echo dllnctest
echo ==============================
pushd .\mfhdf\nctest\Debug
copy ..\test_unlim.nc .\test_unlim.nc  > temp.txt
del temp.txt
dllnctest
del *.nc
popd

if "%1"=="enablefortran" (

echo *****************************************************************************
echo                         HDF4 Fortran Library and Utilities Tests -- Debug
echo *****************************************************************************

echo.
echo.
echo Test Debug Fortran Version:

echo.
echo ===========================================
echo HDF Library Fortran Interface Tests Setup
echo ===========================================
pushd hdf\test\Debug
copy ..\*.dat . >temp.txt
del temp.txt
copy ..\fort_ps\fortest.arg . >temp.txt
del temp.txt
fortestf.exe
del *.dat
del fortest.arg
popd

echo.
echo ===================================
echo HDF-SD Fortran interfaces tests
echo ===================================
pushd mfhdf\fortran\Debug
mkdir testdir
hdftestf.exe
del *.hdf
rmdir /s/q testdir
popd

echo.
echo.
echo Test Debug Fortran DLL Version:

echo.
echo =============================================
echo HDF Library Fortran Interface DLL Tests Setup
echo =============================================
pushd hdf\test\Debug
copy ..\*.dat .  > temp.txt
del temp.txt
copy ..\fort_ps\fortest.arg .  > temp.txt
del temp.txt
dllfortestf
del *.dat
del fortest.arg
popd

echo.
echo ===================================
echo HDF-SD Fortran interfaces DLL tests
echo ===================================
echo.
pushd mfhdf\fortran\Debug
mkdir testdir
dllhdftestf
del *.hdf
rmdir /s/q testdir
popd
)
