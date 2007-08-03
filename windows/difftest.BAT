@ ECHO OFF
REM This batch file is used for HDF4 diff tests.
REM By Xuan Bai
REM Created on 11/29/2004
REM Last Modified on 7/30/2007

echo *** Testing hdfdiff  ***
pushd mfhdf\hdiff\%1
mkdir temptest
cd temptest

rem Instead of generating comparision files all the time, use the files in the repository
rem ..\hdifftst

copy ..\..\testfiles\* . > temp.txt 2>&1
del temp.txt

REM help message
..\hdiff > hdiff_01.out 2>&1 

REM Compare global attributes only
..\hdiff -g hdifftst1.hdf hdifftst2.hdf > hdiff_02.out 2>&1 

REM Compare SD local attributes only
..\hdiff -s hdifftst1.hdf hdifftst2.hdf > hdiff_03.out 2>&1 

REM Compare SD data only
..\hdiff -d hdifftst1.hdf hdifftst2.hdf > hdiff_04.out 2>&1 

REM Compare Vdata daqlsta only
..\hdiff -D hdifftst1.hdf hdifftst2.hdf > hdiff_05.out 2>&1

REM Print statistics
REM   The following test is commented out due to a Windows-specific bug.
REM   Windows outputs the exponent of in scientific notation with a minimum
REM   of 3 digits, while most other platforms use 2.  This causes false-positives
REM   in our test results.  Rather than maintaining our over version of the testfiles,
REM   we simply comment out the test.
REM ..\hdiff -d -S hdifftst1.hdf hdifftst2.hdf > hdiff_06.out 2>&1 

REM Compare SD data on variable(s)
..\hdiff -d -v dset1 hdifftst1.hdf hdifftst2.hdf > hdiff_07.out 2>&1 

REM Compare vdata on variable(s) 
..\hdiff -D -u vdata1 hdifftst1.hdf hdifftst2.hdf > hdiff_08.out 2>&1 

REM Print difference up to count number
..\hdiff -d -e 2 hdifftst1.hdf hdifftst2.hdf > hdiff_09.out 2>&1

REM Print difference when it is greater than limit
..\hdiff -d -t 2 hdifftst1.hdf hdifftst2.hdf > hdiff_10.out 2>&1 

REM no options
..\hdiff hdifftst1.hdf hdifftst2.hdf > hdiff_11.out 2>&1

REM percent(relative)
..\hdiff -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf > hdiff_12.out 2>&1

REM hyperslab reading
..\hdiff hdifftst3.hdf hdifftst4.hdf > hdiff_13.out 2>&1

REM lone dim
..\hdiff hdifftst5.hdf hdifftst6.hdf > hdiff_14.out 2>&1

copy ..\..\..\..\deleteline.bat . >temp.txt
del temp.txt

mkdir fctemp 

call deleteline hdiff_01.txt 3 
call deleteline hdiff_01.out 0
call deleteline hdiff_02.txt 3 
call deleteline hdiff_02.out 0
call deleteline hdiff_03.txt 3 
call deleteline hdiff_03.out 0
call deleteline hdiff_04.txt 3 
call deleteline hdiff_04.out 0
call deleteline hdiff_05.txt 3 
call deleteline hdiff_05.out 0
REM call deleteline hdiff_06.txt 3 
REM call deleteline hdiff_06.out 0
call deleteline hdiff_07.txt 3 
call deleteline hdiff_07.out 0
call deleteline hdiff_08.txt 3 
call deleteline hdiff_08.out 0
call deleteline hdiff_09.txt 3 
call deleteline hdiff_09.out 0
call deleteline hdiff_10.txt 3 
call deleteline hdiff_10.out 0
call deleteline hdiff_11.txt 3 
call deleteline hdiff_11.out 0
call deleteline hdiff_12.txt 3
call deleteline hdiff_12.out 0
call deleteline hdiff_13.txt 3
call deleteline hdiff_13.out 0
call deleteline hdiff_14.txt 3
call deleteline hdiff_14.out 0

cd fctemp

fc hdiff_01.txt hdiff_01.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff                                                          PASSED
) else (
   echo Testing hdiff                                                          FAILED
   more temp.txt
)
del temp.txt

fc hdiff_02.txt hdiff_02.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -g hdifftst1.hdf hdifftst2.hdf                           PASSED
) else (
   echo Testing hdiff -g hdifftst1.hdf hdifftst2.hdf                           FAILED
   more temp.txt
)
del temp.txt

fc hdiff_03.txt hdiff_03.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -s hdifftst1.hdf hdifftst2.hdf                           PASSED
) else (
   echo Testing hdiff -s hdifftst1.hdf hdifftst2.hdf                           FAILED
   more temp.txt
)
del temp.txt

fc hdiff_04.txt hdiff_04.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -d hdifftst1.hdf hdifftst2.hdf                           PASSED
) else (
   echo Testing hdiff -d hdifftst1.hdf hdifftst2.hdf                           FAILED
   more temp.txt
)
del temp.txt

fc hdiff_05.txt hdiff_05.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -D hdifftst1.hdf hdifftst2.hdf                           PASSED
) else (
   echo Testing hdiff -D hdifftst1.hdf hdifftst2.hdf                           FAILED
   more temp.txt
)
del temp.txt

REM   The following test is commented out due to a Windows-specific bug.
REM   Windows outputs the exponent of in scientific notation with a minimum
REM   of 3 digits, while most other platforms use 2.  This causes false-positives
REM   in our test results.  Rather than maintaining our over version of the testfiles,
REM   we simply comment out the test.
REM fc hdiff_06.txt hdiff_06.out >temp.txt
REM if %ERRORLEVEL%==0 (
REM   echo Testing hdiff -d -S hdifftst1.hdf hdifftst2.hdf                        PASSED
REM ) else (
REM   echo Testing hdiff -d -S hdifftst1.hdf hdifftst2.hdf                        FAILED
REM   more temp.txt
REM )
REM del temp.txt

fc hdiff_07.txt hdiff_07.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -d -v dset1 hdifftst1.hdf hdifftst2.hdf                  PASSED
) else (
   echo Testing hdiff -d -v dset1 hdifftst1.hdf hdifftst2.hdf                  FAILED
   more temp.txt
)
del temp.txt

fc hdiff_08.txt hdiff_08.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -D -u vdata1 hdifftst1.hdf hdifftst2.hdf                 PASSED
) else (
   echo Testing hdiff -D -u vdata1 hdifftst1.hdf hdifftst2.hdf                 FAILED
   more temp.txt
)
del temp.txt

fc hdiff_09.txt hdiff_09.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -d -e 2 hdifftst1.hdf hdifftst2.hdf                      PASSED
) else (
   echo Testing hdiff -d -e 2 hdifftst1.hdf hdifftst2.hdf                      FAILED
   more temp.txt
)
del temp.txt

fc hdiff_10.txt hdiff_10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -d -t 2 hdifftst1.hdf hdifftst2.hdf                      PASSED
) else (
   echo Testing hdiff -d -t 2 hdifftst1.hdf hdifftst2.hdf                      FAILED
   more temp.txt
)
del temp.txt

fc hdiff_11.txt hdiff_11.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff hdifftst1.hdf hdifftst2.hdf                              PASSED
) else (
   echo Testing hdiff hdifftst1.hdf hdifftst2.hdf                              FAILED
   more temp.txt
)

fc hdiff_12.txt hdiff_12.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf          PASSED
) else (
   echo Testing hdiff -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf          FAILED
   more temp.txt
)

fc hdiff_13.txt hdiff_13.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff hdifftst3.hdf hdifftst4.hdf                              PASSED
) else (
   echo Testing hdiff hdifftst3.hdf hdifftst4.hdf                              FAILED
   more temp.txt
)

fc hdiff_14.txt hdiff_14.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdiff hdifftst5.hdf hdifftst6.hdf                              PASSED
) else (
   echo Testing hdiff hdifftst5.hdf hdifftst6.hdf                              FAILED
   more temp.txt
)


del temp.txt

cd ..\..

rmdir /s/q temptest

popd
