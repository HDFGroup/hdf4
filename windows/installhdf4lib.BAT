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

mkdir bin
pushd bin
rem ./hdf4lib/debug/bin
copy ..\..\..\hdf\util\Debug\gif2hdf.exe .
copy ..\..\..\hdf\util\Debug\hdf24to8.exe .
copy ..\..\..\hdf\util\Debug\hdf2gif.exe .
copy ..\..\..\hdf\util\Debug\hdf2jpeg.exe .
copy ..\..\..\hdf\util\Debug\hdf8to24.exe .
copy ..\..\..\hdf\util\Debug\hdfcomp.exe .
copy ..\..\..\hdf\util\Debug\hdfed.exe .
copy ..\..\..\mfhdf\hdfimport\Debug\hdfimport.exe .
copy ..\..\..\hdf\util\Debug\hdfls.exe .
copy ..\..\..\hdf\util\Debug\hdfpack.exe .
copy ..\..\..\hdf\util\Debug\hdftopal.exe .
copy ..\..\..\hdf\util\Debug\hdftor8.exe .
copy ..\..\..\hdf\util\Debug\hdfunpac.exe .
copy ..\..\..\mfhdf\hdiff\Debug\hdiff.exe .
copy ..\..\..\mfhdf\dumper\Debug\hdp.exe .
copy ..\..\..\mfhdf\hrepack\Debug\hrepack.exe .
copy ..\..\..\hdf\util\Debug\jpeg2hdf.exe .
copy ..\..\..\mfhdf\ncdump\Debug\ncdump.exe .
copy ..\..\..\mfhdf\ncgen\Debug\ncgen.exe .
copy ..\..\..\mfhdf\nctest\Debug\nctest.exe .
copy ..\..\..\hdf\util\Debug\paltohdf.exe .
copy ..\..\..\hdf\util\Debug\r8tohdf.exe .
copy ..\..\..\hdf\util\Debug\ristosds.exe .
copy ..\..\..\hdf\util\Debug\vmake.exe .
copy ..\..\..\hdf\util\Debug\vshow.exe .
popd

mkdir dll
pushd dll
rem ./hdf4lib/debug/dll
copy ..\..\..\hdf\src\Debug\hd422md.dll .
copy ..\..\..\hdf\src\Debug\hd422md.lib .
copy ..\..\..\hdf\src\Debug\hdf_fcstubdlld.dll .
copy ..\..\..\hdf\src\Debug\hdf_fcstubdlld.lib .
copy ..\..\..\hdf\src\Debug\hdf_fortrandlld.dll .
copy ..\..\..\hdf\src\Debug\hdf_fortrandlld.lib .
copy ..\..\..\mfhdf\libsrc\Debug\hm422md.dll .
copy ..\..\..\mfhdf\libsrc\Debug\hm422md.lib .
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fcstubdlld.dll
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fcstubdlld.lib
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fortrandlld.dll
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fortrandlld.lib
copy ..\..\..\mfhdf\xdr\Debug\xdr_for_dll.lib
popd

mkdir include
pushd include
rem ./hdf4lib/debug/include
copy ..\..\..\hdf\src\*.h . 
copy ..\..\..\hdf\src\*.inc . 
del maldebug.h
del mcache.h
del dfconvrt.h
del dfivms.h
del dir_mac.h
del hqueue.h 
del src.inc
del sys_dir_mac.h
del patchlevel.h
del glist.h
del trace.h
del vgint.h
copy ..\..\..\mfhdf\libsrc\*.h . 
del alloc.h
del error.h
del win32cdf.h
del hdftest.h
copy ..\..\..\mfhdf\fortran\msoft\netcdf.inc .
popd

mkdir lib
pushd lib
rem ./hdf4lib/debug/lib
copy ..\..\..\hdf\src\Debug\hd422d.lib .
copy ..\..\..\hdf\src\Debug\hdf_fcstubd.lib .
copy ..\..\..\hdf\src\Debug\hdf_fortran.lib .
copy ..\..\..\mfhdf\libsrc\Debug\hm422d.lib .
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fcstubd.lib .
copy ..\..\..\mfhdf\fortran\Debug\mfhdf_fortran.lib .
copy ..\..\..\mfhdf\xdr\Debug\xdr.lib .
popd

popd

mkdir release
pushd release
rem ./hdf4lib/release

mkdir bin
pushd bin
rem ./hdf4lib/release/bin
copy ..\..\..\hdf\util\Release\gif2hdf.exe .
copy ..\..\..\hdf\util\Release\hdf24to8.exe .
copy ..\..\..\hdf\util\Release\hdf2gif.exe .
copy ..\..\..\hdf\util\Release\hdf2jpeg.exe .
copy ..\..\..\hdf\util\Release\hdf8to24.exe .
copy ..\..\..\hdf\util\Release\hdfcomp.exe .
copy ..\..\..\hdf\util\Release\hdfed.exe .
copy ..\..\..\mfhdf\hdfimport\Release\hdfimport.exe .
copy ..\..\..\hdf\util\Release\hdfls.exe .
copy ..\..\..\hdf\util\Release\hdfpack.exe .
copy ..\..\..\hdf\util\Release\hdftopal.exe .
copy ..\..\..\hdf\util\Release\hdftor8.exe .
copy ..\..\..\hdf\util\Release\hdfunpac.exe .
copy ..\..\..\mfhdf\hdiff\Release\hdiff.exe .
copy ..\..\..\mfhdf\dumper\Release\hdp.exe .
copy ..\..\..\mfhdf\hrepack\Release\hrepack.exe .
copy ..\..\..\hdf\util\Release\jpeg2hdf.exe .
copy ..\..\..\mfhdf\ncdump\Release\ncdump.exe .
copy ..\..\..\mfhdf\ncgen\Release\ncgen.exe .
copy ..\..\..\mfhdf\nctest\Release\nctest.exe .
copy ..\..\..\hdf\util\Release\paltohdf.exe .
copy ..\..\..\hdf\util\Release\r8tohdf.exe .
copy ..\..\..\hdf\util\Release\ristosds.exe .
copy ..\..\..\hdf\util\Release\vmake.exe .
copy ..\..\..\hdf\util\Release\vshow.exe .
popd

mkdir dll
pushd dll
rem ./hdf4lib/release/dll
copy ..\..\..\hdf\src\Release\hd422m.dll .
copy ..\..\..\hdf\src\Release\hd422m.lib .
copy ..\..\..\hdf\src\Release\hdf_fcstubdll.dll .
copy ..\..\..\hdf\src\Release\hdf_fcstubdll.lib .
copy ..\..\..\hdf\src\Release\hdf_fortrandll.dll .
copy ..\..\..\hdf\src\Release\hdf_fortrandll.lib .
copy ..\..\..\mfhdf\libsrc\Release\hm422m.dll .
copy ..\..\..\mfhdf\libsrc\Release\hm422m.lib .
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fcstubdll.dll
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fcstubdll.lib
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fortrandll.dll
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fortrandll.lib
copy ..\..\..\mfhdf\xdr\Release\xdr_for_dll.lib
popd

mkdir include
pushd include
rem ./hdf4lib/release/include
copy ..\..\..\hdf\src\*.h . 
copy ..\..\..\hdf\src\*.inc . 
del maldebug.h
del mcache.h
del dfconvrt.h
del dfivms.h
del dir_mac.h
del hqueue.h 
del src.inc
del sys_dir_mac.h
del patchlevel.h
del glist.h
del trace.h
del vgint.h
copy ..\..\..\mfhdf\libsrc\*.h . 
del alloc.h
del error.h
del win32cdf.h
del hdftest.h
copy ..\..\..\mfhdf\fortran\msoft\netcdf.inc .
popd

mkdir lib
pushd lib
rem ./hdf4lib/release/lib
copy ..\..\..\hdf\src\Release\hd422.lib .
copy ..\..\..\hdf\src\Release\hdf_fcstub.lib .
copy ..\..\..\hdf\src\Release\hdf_fortran.lib .
copy ..\..\..\mfhdf\libsrc\Release\hm422.lib .
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fcstub.lib .
copy ..\..\..\mfhdf\fortran\Release\mfhdf_fortran.lib .
copy ..\..\..\mfhdf\xdr\Release\xdr.lib .
popd

popd
popd
