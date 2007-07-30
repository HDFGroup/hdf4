@REM File name: installfortran_dll.bat
@REM This batch file is used to copy HDF4 fortran DLLs into system folder.
@REM By Fang GUO
@REM Created: 07/05/2005
@REM Last Updated: 7/30/07

@ ECHO OFF

copy hdf\src\Debug\hdf_fcstubdlld.dll %SystemRoot%\system >temp.txt
copy hdf\src\Debug\hdf_fortrandlld.dll %SystemRoot%\system >temp.txt
copy mfhdf\fortran\Debug\mfhdf_fortrandlld.dll %SystemRoot%\system >temp.txt
copy mfhdf\fortran\Debug\mfhdf_fcstubdlld.dll %SystemRoot%\system >temp.txt
copy hdf\test\Debug\dlllibtest_fcstubd.dll %SystemRoot%\system >temp.txt

copy hdf\src\Release\hdf_fcstubdll.dll %SystemRoot%\system >temp.txt
copy hdf\src\Release\hdf_fortrandll.dll %SystemRoot%\system >temp.txt
copy mfhdf\fortran\Release\mfhdf_fortrandll.dll %SystemRoot%\system >temp.txt
copy mfhdf\fortran\Release\mfhdf_fcstubdll.dll %SystemRoot%\system >temp.txt
copy hdf\test\Release\dlllibtest_fcstub.dll %SystemRoot%\system >temp.txt