@REM File name: install_dll.bat
@REM This batch file is used to copy HDF4 DLLs into system folder.
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 01/28/2005

@ ECHO OFF
copy Windows\dll\debug\hd421md.dll %SystemRoot%\system >temp.txt
copy Windows\dll\debug\hm421md.dll %SystemRoot%\system >temp.txt
copy Windows\dll\release\hd421m.dll %SystemRoot%\system >temp.txt
copy Windows\dll\release\hm421m.dll %SystemRoot%\system >temp.txt
del temp.txt