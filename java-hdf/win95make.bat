@ECHO OFF

@REM set up your java home directory. You need java 1.1.1 or above.
@SET JAVAHOME=c:\jdk

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
del %JHVHOME%\classes
mkdir %JHVHOME%\classes
SET CLASSPATH=%JAVAHOME%\lib;%JHVHOME%\classes
@CLS
@ECHO ON
@ECHO compiling, please wait ....

%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\hdflib\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\util\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\event\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\image\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\palette\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\animation\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\plots\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\awt\layer\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\message\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\io\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\jhv\*.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\server\HDFServerConnection.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\server\HDFServer_njsdk.java

@ECHO need JSDK classes to compile the servlet classses package
SET CLASSPATH=%JAVAHOME%\lib;%JHVHOME%\classes;C:\Jsdk\lib\classes.zip
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\server\FileServlet.java
%JAVAHOME%\bin\javac -d %JHVHOME%\classes %JHVHOME%\ncsa\hdf\server\HDFServer.java

@ECHO compiling is done

@ECHO packing, please wait ....
cd %JHVHOME%\classes
delete %JHVHOME%\lib\classes.zip
%JAVAHOME%\bin\jar -cvf %JHVHOME%\lib\classes.zip ncsa

:END
