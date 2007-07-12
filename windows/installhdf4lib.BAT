@ECHO OFF
@REM File name: installhdf4lib.bat
@REM This batch file is used to install HDF4 debug version library and utilties.
@REM By Xuan Bai
@REM Last Updated: 01/28/2004

mkdir hdf4lib
cd hdf4lib
mkdir debug
cd debug
mkdir bin
mkdir dll
mkdir lib
mkdir include

cd lib
copy ..\..\..\proj\lib\debug\singlethreaded\*.lib .


cd ..\dll
copy ..\..\..\proj\dll\debug\*.DLL .
copy ..\..\..\proj\lib\debug\multithreaded\*.lib .


cd ..\include
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

cd ..\bin

copy ..\..\..\ncgen\debug\*.exe .
copy ..\..\..\ncdump\debug\*.exe .
copy ..\..\..\nctest\debug\*.exe .
copy ..\..\..\Windows\bin\debug\*.exe .
del hdfimporttest.exe
del hdifftst.exe
del hrepacktst.exe


cd ..\..

mkdir release
cd release
mkdir bin
mkdir dll
mkdir lib
mkdir include


cd lib
copy ..\..\..\proj\lib\release\singlethreaded\*.lib .

cd ..\dll
copy ..\..\..\proj\dll\release\*.DLL .
copy ..\..\..\proj\lib\release\multithreaded\*.lib .


cd ..\include
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

cd ..\bin

copy ..\..\..\ncgen\release\*.exe .
copy ..\..\..\ncdump\release\*.exe .
copy ..\..\..\nctest\release\*.exe .
copy ..\..\..\Windows\bin\release\*.exe .
del hdfimporttest.exe
del hdifftst.exe
del hrepacktst.exe

cd ..\..\..

