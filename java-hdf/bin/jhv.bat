@ECHO OFF

@REM =======================================================================
@REM                    JHV run batch file for Windows 95/NT
@REM =======================================================================


@REM To run this Java-based HDF Viewer (JHV), You need jdk1.1.x. 

@REM set up your java (jdk1.1.x) home directory.
@SET JAVAHOME=C:\jdk

@REM set up your jhv home directory, i.e. where you installed the JHV
@SET JHVHOME=.

@REM Do not make changes under this line unless you know what you are doing.
@REM =======================================================================

@REM you do not need to set yourr icon directory unless you moved the 
@REM hdficons directory or you want to use your own icons.
@SET ICONPATH=%JHVHOME%\lib\hdficons

@REM open this file when JHV starts
SET HDFFILE=%JHVHOME%\samples

@REM Test if jhv and java home directories are valid
IF not exist "%JHVHOME%\lib\classes.zip" SET JHVHOME=..
IF not exist "%JHVHOME%\lib\classes.zip" GOTO :NOJHV
IF not exist "%JAVAHOME%\bin\java.exe" GOTO :NOJAVA
IF not exist "%JHVHOME%\lib\win95\hdf.dll" GOTO :NOHDFDLL 
GOTO :RUN

:NOJHV
@ECHO ON
@ECHO %JHVHOME%\lib\classes.zip not found,
@ECHO please check your jhv home directory.
@PAUSE
@GOTO :END

:NOJAVA
@ECHO ON
@ECHO %JAVAHOME%\bin\java.exe not found,
@ECHO please check your java home directory.
@PAUSE
@GOTO :END

:NOHDFDLL
@ECHO ON
@ECHO %JHVHOME%\lib\win95\hdf.dll not found,
@PAUSE
@GOTO :END

:RUN

SET PATH="";
SET PATH=%JHVHOME%\lib\win95;%JAVAHOME%\bin
SET PATH=%PATH%;"%JHVHOME%\lib\win95;%JAVAHOME%\bin"
SET CLASSPATH=%JAVAHOME%\lib\classes.zip;%JHVHOME%\lib\classes.zip

java ncsa.hdf.jhv.JHV -jhvpath "%JHVHOME\bin%" -jhviconspath "%ICONPATH%" "%HDFFILE%"

:END
