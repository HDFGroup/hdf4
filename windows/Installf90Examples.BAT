@ECHO OFF
REM This batch file is used to install HDF4 Fortran examples'
REM executable files.
REM By Xuan Bai
REM Created on: 12/23/2004
REM Last Modified: 8/2/2007

pushd windows\examples\f90examples

mkdir f90examplesREL
mkdir f90examplesDBG

for %%a in (createtest gitest rattrtest readtest sattrtest sds2grouptest sh_dimtest unltest) do (
    copy debug\%%a.exe f90examplesDBG\
    copy release\%%a.exe f90examplesREL\
)

popd