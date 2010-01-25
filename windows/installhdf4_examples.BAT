@ECHO OFF
@REM File name: installhdf4lib.bat
@REM This batch file is used to install HDF4 debug version library and utilties.
@REM By Xuan Bai
@REM Last Updated: 07/30/2007

mkdir hdf4lib
pushd hdf4lib
rem ./hdf4lib

mkdir debug
pushd debug
rem ./hdf4lib/debug

mkdir examples
pushd examples
rem ./hdf4lib/debug/examples

mkdir hdf
pushd hdf
rem ./hdf4lib/debug/examples/hdf
copy ..\..\..\..\hdf\examples\Debug\*.exe .

mkdir fortran
pushd fortran
rem ./hdf4lib/debug/examples/hdf/fortran
copy ..\..\..\..\..\hdf\fortran\examples\Debug\*.exe .
popd
rem ./hdf4lib/debug/examples/hdf

popd
rem ./hdf4lib/debug/examples

mkdir mfhdf
pushd mfhdf
rem ./hdf4lib/debug/examples/mfhdf
copy ..\..\..\..\mfhdf\examples\Debug\*.exe . 

mkdir fortran
pushd fortran
rem ./hdf4lib/debug/examples/mfhdf/fortran
copy ..\..\..\..\..\mfhdf\fortran\examples\Debug\*.exe .
popd
rem ./hdf4lib/debug/examples/mfhdf

popd
rem ./hdf4lib/debug/examples

popd
rem ./hdf4lib/debug

popd
rem ./hdf4lib

mkdir release
pushd release
rem ./hdf4lib/release

mkdir examples
pushd examples
rem ./hdf4lib/release/examples

mkdir hdf
pushd hdf
rem ./hdf4lib/Release/examples/hdf
copy ..\..\..\..\hdf\examples\Release\*.exe .

mkdir fortran
pushd fortran
rem ./hdf4lib/Release/examples/hdf/fortran
copy ..\..\..\..\..\hdf\fortran\examples\Release\*.exe .
popd
rem ./hdf4lib/Release/examples/hdf

popd
rem ./hdf4lib/ReleaseRelease/examples

mkdir mfhdf
pushd mfhdf
rem ./hdf4lib/Release/examples/mfhdf
copy ..\..\..\..\mfhdf\examples\Release\*.exe . 

mkdir fortran
pushd fortran
rem ./hdf4lib/Release/examples/mfhdf/fortran
copy ..\..\..\..\..\mfhdf\fortran\examples\Release\*.exe .
popd
rem ./hdf4lib/Release/examples/mfhdf

popd
rem ./hdf4lib/Release/examples

popd
rem ./hdf4lib/Release

popd
rem ./hdf4lib
