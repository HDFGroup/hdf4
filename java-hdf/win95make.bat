@ECHO OFF

REM Set up the jhv and java home directories. You need java 1.1.1 or above
REM -----------------------------------------------------------------------
SET JHVHOME=c:\jhv
SET JAVAHOME=c:\jdk1.1.3

@REM Do not make changes under this line unless you know what you are doing.
@REM -----------------------------------------------------------------------

@REM Test if jhv and java home directories are valid
IF not exist %JHVHOME%\ncsa\hdf\jhv\JHV.java GOTO :NOJHV
IF not exist %JAVAHOME%\bin\javac.exe GOTO :NOJAVA
GOTO :BUILD

:NOJHV
@ECHO ON
@ECHO %JHVHOME% not found, please check your jhv home directory
@PAUSE
@GOTO :END

:NOJAVA
@ECHO ON
@ECHO %JAVAHOME% not found, please check your java home directory
@PAUSE
@GOTO :END

:BUILD
mkdir %JHVHOME%\classes
SET CLASSPATH=%JAVAHOME%\lib;%JHVHOME%\classes;
@CLS
@ECHO ON
@ECHO compiling, please wait ....
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\hdflib\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\jhv\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\palette\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\java\awt\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\java\awt\image\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\java\util\*.java
@ECHO compiling is done
:END
