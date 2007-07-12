@REM File name: install_dll.bat
@REM This batch file is used to copy HDF4 DLLs into system folder.
@REM By Xuan Bai
@REM Created: 11/08/2004
@REM Last Updated: 7/12/2007

@ ECHO OFF
copy proj\dll\debug\hd421md.dll %SystemRoot%\system >temp.txt
copy proj\dll\debug\hm421md.dll %SystemRoot%\system >temp.txt
copy proj\dll\release\hd421m.dll %SystemRoot%\system >temp.txt
copy proj\dll\release\hm421m.dll %SystemRoot%\system >temp.txt
del temp.txt