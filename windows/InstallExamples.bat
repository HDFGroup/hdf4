@ECHO OFF
REM This batch file is used to install HDF4 C examples
REM executable files.
REM By Xuan Bai
REM Created on: 12/23/2004
REM Last Modified: 8/1/2004

pushd windows\examples\cexamples

mkdir examplesREL
mkdir examplesDBG

for %%a in (chgnamtest createtest dimtest gitest rattrtest readtest sattrtest sds2grouptest sh_dimtest unltest) do (
    copy debug\%%a.exe examplesDBG\
    copy release\%%a.exe examplesREL\
)

popd