@ECHO OFF

@REM set up your java home directory. You need java 1.1.1 or above.
@SET JAVAHOME=c:\jdk1.1.5

@REM set up your JHV home directory.
@SET JHVHOME=.

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
SET CLASSPATH=%JAVAHOME%\lib;%JHVHOME%\classes
@CLS
@ECHO ON
@ECHO compiling, please wait ....

%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\hdflib\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\util\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\util\decoder\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\util\encoder\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\event\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\image\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\palette\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\animation\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\plots\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\layer\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\message\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\jhv\*.java

REM @ECHO need JSDK classes to compile the ncsa.hdf.server package
REM SET CLASSPATH=%JAVAHOME%\lib;%JHVHOME%\classes;C:\Jsdk\lib\classes.zip
REM %JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\server\*.java

@ECHO compiling is done
:END
