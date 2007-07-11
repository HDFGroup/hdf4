@ECHO OFF
REM This batch file is used to install HDF4 C examples'
REM executable files.
REM By Xuan Bai
REM Created on: 12/23/2004
REM Last Modified: 12/23/2004

cd examples

mkdir examplesREL
mkdir examplesDBG

cd chgnamtest
copy debug\chgnamtest.exe ..\examplesDBG\
copy release\chgnamtest.exe ..\examplesREL\
cd ..

cd createtest
copy debug\createtest.exe ..\examplesDBG\
copy release\createtest.exe ..\examplesREL\
cd ..

cd dimtest
copy debug\dimtest.exe ..\examplesDBG\
copy release\dimtest.exe ..\examplesREL\
cd ..

cd gitest
copy debug\gitest.exe ..\examplesDBG\
copy release\gitest.exe ..\examplesREL\
cd ..

cd rattrtest
copy debug\rattrtest.exe ..\examplesDBG\
copy release\rattrtest.exe ..\examplesREL\
cd ..

cd readtest
copy debug\readtest.exe ..\examplesDBG\
copy release\readtest.exe ..\examplesREL\
cd ..

cd sattrtest
copy debug\sattrtest.exe ..\examplesDBG\
copy release\sattrtest.exe ..\examplesREL\
cd ..

cd sds2grouptest
copy debug\sds2grouptest.exe ..\examplesDBG\
copy release\sds2grouptest.exe ..\examplesREL\
cd ..

cd sh_dimtest
copy debug\sh_dimtest.exe ..\examplesDBG\
copy release\sh_dimtest.exe ..\examplesREL\
cd ..

cd unltest
copy debug\unltest.exe ..\examplesDBG\
copy release\unltest.exe ..\examplesREL\
cd ..

cd ..