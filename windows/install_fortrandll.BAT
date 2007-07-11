@REM File name: installfortran_dll.bat
@REM This batch file is used to copy HDF4 fortran DLLs into system folder.
@REM By Fang GUO
@REM Created: 07/05/2005
@REM Last Updated: 

@ ECHO OFF

copy Windows\dll\debug\hdf_fcstubdlld.dll %SystemRoot%\system >temp.txt
copy Windows\dll\debug\hdf_fortrandlld.dll %SystemRoot%\system >temp.txt
copy Windows\dll\debug\mfhdf_fortrandlld.dll %SystemRoot%\system >temp.txt
copy Windows\dll\debug\mfhdf_fcstubdlld.dll %SystemRoot%\system >temp.txt
copy dlllibtest_fcstub\debug\dlllibtest_fcstubd.dll %SystemRoot%\system >temp.txt

copy Windows\dll\release\hdf_fcstubdll.dll %SystemRoot%\system >temp.txt
copy Windows\dll\release\hdf_fortrandll.dll %SystemRoot%\system >temp.txt
copy Windows\dll\release\mfhdf_fortrandll.dll %SystemRoot%\system >temp.txt
copy Windows\dll\release\mfhdf_fcstubdll.dll %SystemRoot%\system >temp.txt
copy dlllibtest_fcstub\release\dlllibtest_fcstub.dll %SystemRoot%\system >temp.txt