@ECHO OFF

REM File name: ctest0_gen.bat
REM This file is used to generate test0.c souce code, which will be used in ctest0 project.
REM By Xuan Bai
REM Last Modified: 8/1/2007

:: Put us in the directory of the batch file.
pushd %~dp0
mfhdf\ncgen\%1\ncgen -c -o ctest0.nc mfhdf\ncgen\test0.cdl > mfhdf\test0.c
popd