@ ECHO OFF
REM This batch file is used for HDF4 import tests.
REM By Xuan Bai
REM Created on 11/22/2004
REM Last Modified on 7/30/2007

echo *** Testing hdfimport  ***
pushd mfhdf\hdfimport\%1
mkdir temptest
cd temptest
..\hdfimporttest

echo Testing for 32-bit floating point ASCII (2D data)
..\hdfimport ctxtr2 -o ctxtr2.hdf
echo Testing for 32-bit floating point ASCII (3D data) 
..\hdfimport ctxtr3 -o ctxtr3.hdf
echo Testing for 32-bit integer binary (2D data)
..\hdfimport cb32i2 -o cb32i2.hdf
echo Testing for 32-bit integer  binary (3D data) 
..\hdfimport cb32i3 -o cb32i3.hdf
echo Testing for 16-bit integer binary (2D data) 
..\hdfimport cb16i2 -o cb16i2.hdf
echo Testing for 16-bit integer (3D data)
..\hdfimport cb16i3 -o cb16i3.hdf
echo Testing for 32-bit floating point binary (2D data) 
..\hdfimport cb32r2 -o cb32r2.hdf
echo Testing for 32-bit floating point binary (3D data) 
..\hdfimport cb32r3 -o cb32r3.hdf
echo Testing for 64-bit floating point binary (2D data) - Default Behaviour (Conversion to 32 bit FP SDS)
..\hdfimport cb64r2 -o cb64r2.hdf
echo Testing for 64-bit floating point binary (3D data) - Default Behaviour (Conversion to 32-bit FP SDS) 
..\hdfimport cb64r3 -o cb64r3.hdf
echo Testing for 64-bit floating point binary (2D data) - Conversion to 64-bit FP SDS
..\hdfimport cb64r2 -n -o cb64r2-n.hdf
echo Testing for 64-bit floating point binary (3D data) - Conversion to 64-bit FP SDS
..\hdfimport cb64r3 -n -o cb64r3-n.hdf
echo Testing for raster options 
..\hdfimport ctxtr2 -o ctxtr2_ris.hdf -raster -e 50 50
..\hdfimport cb64r2 -o cb64r2_ris.hdf -raster -i 50 50 -f

..\..\..\..\hdf\util\%1\hdfls -l ctxtr2.hdf > hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l ctxtr3.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb32i2.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb32i3.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb16i2.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb16i3.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb32r2.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb32r3.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb64r2.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb64r3.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb64r2-n.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb64r3-n.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l ctxtr2_ris.hdf >> hdfls.tmp5 2>&1
..\..\..\..\hdf\util\%1\hdfls -l cb64r2_ris.hdf >> hdfls.tmp5 2>&1

findstr /V "library String" hdfls.tmp5 > hdfls_temp.tmp5
del hdfls.tmp5
rename hdfls_temp.tmp5 hdfls.tmp5

copy ..\..\hdfimport.*ut* . > temp.txt 2>&1
del temp.txt

fc hdfls.tmp5 hdfimport.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set error1=0
) else (
   set error1=1
   more temp.txt
)
del temp.txt

..\..\..\..\hdf\util\%1\hdfed -batch < hdfimport.input1 > hdfed.tmp6 2>&1
REM   The following test is commented out due to a Windows-specific bug.
REM   Windows outputs the exponent of in scientific notation with a minimum
REM   of 3 digits, while most other platforms use 2.  This causes false-positives
REM   in our test results.  Rather than maintaining our over version of the testfiles,
REM   we simply comment out the test.
REM fc hdfed.tmp6 hdfimport.out2 ^> temp.txt 2^>&1
REM find "FC: no diff" temp.txt ^> nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
REM   more temp.txt
)
REM del temp.txt

if %errors%==0 (
   echo =================================
   echo HDFIMPORT Utilities tests passed.
   echo =================================
) else (
   echo =================================
   echo HDFIMPORT Utilities tests failed.
   echo =================================
)
    
cd ..

rmdir /s/q temptest

popd
