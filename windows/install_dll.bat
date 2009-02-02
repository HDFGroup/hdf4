@REM File name: install_dll.bat
@REM This batch file is used to copy HDF4 DLLs into system folder.
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 7/30/2007

@echo OFF
copy hdf\src\Debug\hd424md.dll %systemroot%\system >temp.txt
copy mfhdf\libsrc\Debug\hm424md.dll %systemroot%\system >temp.txt
copy hdf\src\Release\hd424m.dll %systemroot%\system >temp.txt
copy mfhdf\libsrc\release\hm424m.dll %systemroot%\system >temp.txt
del temp.txt
