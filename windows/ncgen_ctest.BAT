@ECHO OFF

REM File name: ncgen_ctest.bat
REM This file is used to run ncgen -c test.
REM By Xuan Bai
REM Last Modified: 8/1/2007

echo =========================
echo ncgen -c test
echo =========================
pushd mfhdf\ncgen\%1

REM generate test1.cdl
copy ..\test0.cdl . >temp.txt
del temp.txt
ncgen -b test0.cdl
..\..\ncdump\%1\ncdump -n test1 test0.nc > test1.cdl

REM generate ctest1.cdl
ctest0
..\..\ncdump\%1\ncdump -n test1 ctest0.nc > ctest1.cdl

REM compare test1.cdl and ctest1.cdl
fc test1.cdl ctest1.cdl | find "FC: no diff" >temp.txt
if %ERRORLEVEL%==0 (
   echo *** ncgen -c test successful ***
) else (
   echo *** ncgen -c test failed ***
   more temp.txt
)
del temp.txt
del *.nc
del *.cdl
popd
