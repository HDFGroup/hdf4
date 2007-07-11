@ECHO OFF

REM File name: ctest0_gen.bat
REM This file is used to generate test0.c souce code, which will be used in ctest0 project.
REM By Xuan Bai
REM Last Modified: 12/16/2004

ncgen\%1\ncgen -c -o ctest0.nc mfhdf\ncgen\test0.cdl > mfhdf\test0.c