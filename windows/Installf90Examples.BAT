@ECHO OFF
REM This batch file is used to install HDF4 Fortran examples'
REM executable files.
REM By Xuan Bai
REM Created on: 12/23/2004
REM Last Modified: 12/23/2004

cd fortran\examples

mkdir f90examplesREL
mkdir f90examplesDBG

cd createtest
copy debug\createtest.exe ..\f90examplesDBG\
copy release\createtest.exe ..\f90examplesREL\
cd ..

cd gitest
copy debug\gitest.exe ..\f90examplesDBG\
copy release\gitest.exe ..\f90examplesREL\
cd ..

cd rattrtest
copy debug\rattrtest.exe ..\f90examplesDBG\
copy release\rattrtest.exe ..\f90examplesREL\
cd ..

cd readtest
copy debug\readtest.exe ..\f90examplesDBG\
copy release\readtest.exe ..\f90examplesREL\
cd ..

cd sattrtest
copy debug\sattrtest.exe ..\f90examplesDBG\
copy release\sattrtest.exe ..\f90examplesREL\
cd ..

cd sds2grouptest
copy debug\sds2grouptest.exe ..\f90examplesDBG\
copy release\sds2grouptest.exe ..\f90examplesREL\
cd ..

cd sh_dimtest
copy debug\sh_dimtest.exe ..\f90examplesDBG\
copy release\sh_dimtest.exe ..\f90examplesREL\
cd ..

cd unltest
copy debug\unltest.exe ..\f90examplesDBG\
copy release\unltest.exe ..\f90examplesREL\
cd ..

cd ..\..