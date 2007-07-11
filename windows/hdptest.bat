@ ECHO OFF
REM This batch file is used for HDF4 hdp tests.
REM By Xuan Bai
REM Created on 11/22/2004
REM Last Modified on 11/22/2004

cd windows\bin\%1
mkdir temptest
copy ..\..\..\mfhdf\dumper\testfiles\. temptest\. >temp.txt
del temp.txt

echo -----------------------------
echo Test command list
echo -----------------------------

hdp list temptest\tdata.hdf > temptest\list-1.results 2>&1
hdp list -l temptest\tdata.hdf > temptest\list-2.results 2>&1
hdp list -d temptest\tdata.hdf > temptest\list-3.results 2>&1
hdp list -e temptest\tdata.hdf > temptest\list-4.results 2>&1
hdp list -t 720 temptest\tdata.hdf > temptest\list-5.results 2>&1
hdp list -d -t "Numeric Data Group" temptest\tdata.hdf > temptest\list-6.results 2>&1
hdp list -g temptest\tdata.hdf > temptest\list-7.results 2>&1
hdp list -a temptest\tdata.hdf > temptest\list-8.results 2>&1
hdp list -a temptest\Example6.hdf > temptest\list-9.results 2>&1
hdp list -n temptest\Example6.hdf > temptest\list-10.results 2>&1

cd temptest
copy ..\..\..\..\deleteline.bat . >temp.txt
del temp.txt
mkdir fctemp

call deleteline list-1.results 1 
call deleteline list-1.out 4
call deleteline list-2.results 1 
call deleteline list-2.out 4
call deleteline list-3.results 1 
call deleteline list-3.out 4
call deleteline list-4.results 1 
call deleteline list-4.out 4
call deleteline list-5.results 1 
call deleteline list-5.out 4
call deleteline list-6.results 1 
call deleteline list-6.out 4
call deleteline list-7.results 1 
call deleteline list-7.out 4
call deleteline list-8.results 1 
call deleteline list-8.out 4
call deleteline list-9.results 1 
call deleteline list-9.out 4
call deleteline list-10.results 1 
call deleteline list-10.out 4

cd fctemp
fc list-1.results list-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list tdata.hdf                                             PASSED
) else (
   echo Testing hdp list tdata.hdf                                             FAILED
   more temp.txt
)
del temp.txt

fc list-2.results list-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -l tdata.hdf                                          PASSED
) else (
   echo Testing hdp list -l tdata.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc list-3.results list-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -d tdata.hdf                                          PASSED
) else (
   echo Testing hdp list -d tdata.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc list-4.results list-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -e tdata.hdf                                          PASSED
) else (
   echo Testing hdp list -e tdata.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc list-5.results list-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -t 720 tdata.hdf                                      PASSED
) else (
   echo Testing hdp list -t 720 tdata.hdf                                      FAILED
   more temp.txt
)
del temp.txt

fc list-6.results list-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -d -t Numeric Data Group tdata.hdf                    PASSED
) else (
   echo Testing hdp list -d -t Numeric Data Group tdata.hdf                    FAILED
   more temp.txt
)
del temp.txt

fc list-7.results list-7.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -g tdata.hdf                                          PASSED
) else (
   echo Testing hdp list -g tdata.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc list-8.results list-8.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -a tdata.hdf                                          PASSED
) else (
   echo Testing hdp list -a tdata.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc list-9.results list-9.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -a Example6.hdf                                       PASSED
) else (
   echo Testing hdp list -a Example6.hdf                                       FAILED
   more temp.txt
)
del temp.txt

fc list-10.results list-10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp list -n Example6.hdf                                       PASSED
) else (
   echo Testing hdp list -n Example6.hdf                                       FAILED
   more temp.txt
)
del temp.txt

cd ..\..

echo -----------------------------
echo Test command dumpsds
echo -----------------------------

hdp dumpsds temptest\swf32.hdf > temptest\dumpsds-1.results 2>&1
hdp dumpsds -i 2 temptest\swf32.hdf > temptest\dumpsds-2.results 2>&1
hdp dumpsds -i 1,3 temptest\swf32.hdf > temptest\dumpsds-3.results 2>&1
hdp dumpsds -n Time temptest\swf32.hdf > temptest\dumpsds-4.results 2>&1
hdp dumpsds -n fakeDim0,Data-Set-2 temptest\swf32.hdf > temptest\dumpsds-5.results 2>&1
hdp dumpsds -r 3,2 temptest\swf32.hdf > temptest\dumpsds-6.results 2>&1
hdp dumpsds -r 3,2 -d temptest\swf32.hdf > temptest\dumpsds-7.results 2>&1
hdp dumpsds -h temptest\swf32_fileattr.hdf > temptest\dumpsds-8.results 2>&1
hdp dumpsds -c temptest\swf32_fileattr.hdf > temptest\dumpsds-9.results 2>&1
hdp dumpsds -g temptest\swf32_fileattr.hdf > temptest\dumpsds-10.results 2>&1
hdp dumpsds -l temptest\swf32_fileattr.hdf > temptest\dumpsds-11.results 2>&1

cd temptest

call deleteline dumpsds-1.results 1 
call deleteline dumpsds-1.out 4
call deleteline dumpsds-2.results 1 
call deleteline dumpsds-2.out 4
call deleteline dumpsds-3.results 1 
call deleteline dumpsds-3.out 4
call deleteline dumpsds-4.results 1 
call deleteline dumpsds-4.out 4
call deleteline dumpsds-5.results 1 
call deleteline dumpsds-5.out 4
call deleteline dumpsds-6.results 1 
call deleteline dumpsds-6.out 4
call deleteline dumpsds-7.results 1 
call deleteline dumpsds-7.out 4
call deleteline dumpsds-8.results 1 
call deleteline dumpsds-8.out 4
call deleteline dumpsds-9.results 1 
call deleteline dumpsds-9.out 4
call deleteline dumpsds-10.results 1 
call deleteline dumpsds-10.out 4
call deleteline dumpsds-11.results 1 
call deleteline dumpsds-11.out 4

cd fctemp
fc dumpsds-1.results dumpsds-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds swf32.hdf                                          PASSED
) else (
   echo Testing hdp dumpsds swf32.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-2.results dumpsds-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -i 2 swf32.hdf                                     PASSED
) else (
   echo Testing hdp dumpsds -i 2 swf32.hdf                                     FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-3.results dumpsds-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -i 1,3 swf32.hdf                                   PASSED
) else (
   echo Testing hdp dumpsds -i 1,3 swf32.hdf                                   FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-4.results dumpsds-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -n Time swf32.hdf                                  PASSED
) else (
   echo Testing hdp dumpsds -n Time swf32.hdf                                  FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-5.results dumpsds-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -n fakeDim0,Data-Set-2 swf32.hdf                   PASSED
) else (
   echo Testing hdp dumpsds -n fakeDim0,Data-Set-2 swf32.hdf                   FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-6.results dumpsds-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -r 3,2 swf32.hdf                                   PASSED
) else (
   echo Testing hdp dumpsds -r 3,2 swf32.hdf                                   FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-7.results dumpsds-7.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -r 3,2 -d swf32.hdf                                PASSED
) else (
   echo Testing hdp dumpsds -r 3,2 -d swf32.hdf                                FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-8.results dumpsds-8.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -h swf32_fileattr.hdf                              PASSED
) else (
   echo Testing hdp dumpsds -h swf32_fileattr.hdf                              FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-9.results dumpsds-9.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -c swf32_fileattr.hdf                              PASSED
) else (
   echo Testing hdp dumpsds -c swf32_fileattr.hdf                              FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-10.results dumpsds-10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -g swf32_fileattr.hdf                              PASSED
) else (
   echo Testing hdp dumpsds -g swf32_fileattr.hdf                              FAILED
   more temp.txt
)
del temp.txt

fc dumpsds-11.results dumpsds-11.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpsds -l swf32_fileattr.hdf                              PASSED
) else (
   echo Testing hdp dumpsds -l swf32_fileattr.hdf                              FAILED
   more temp.txt
)
del temp.txt

cd ..\..

echo -----------------------------
echo Test command dumprig
echo -----------------------------

hdp dumprig temptest\tdf24.hdf > temptest\dumprig-1.results 2>&1
hdp dumprig -i 1,2 temptest\tdf24.hdf > temptest\dumprig-2.results 2>&1
hdp dumprig -i 1,3 temptest\tdf24.hdf > temptest\dumprig-3.results 2>&1
hdp dumprig -m 24 temptest\tdf24.hdf > temptest\dumprig-4.results 2>&1
hdp dumprig -r 3,4 temptest\tdf24.hdf > temptest\dumprig-5.results 2>&1
hdp dumprig -r 3,4 -d temptest\tdf24.hdf > temptest\dumprig-6.results 2>&1

cd temptest

call deleteline dumprig-1.results 1 
call deleteline dumprig-1.out 4
call deleteline dumprig-2.results 1 
call deleteline dumprig-2.out 4
call deleteline dumprig-3.results 1 
call deleteline dumprig-3.out 4
call deleteline dumprig-4.results 1 
call deleteline dumprig-4.out 4
call deleteline dumprig-5.results 1 
call deleteline dumprig-5.out 4
call deleteline dumprig-6.results 1 
call deleteline dumprig-6.out 4

cd fctemp
fc dumprig-1.results dumprig-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig tdf24.hdf                                          PASSED
) else (
   echo Testing hdp dumprig tdf24.hdf                                          FAILED
   more temp.txt
)
del temp.txt

fc dumprig-2.results dumprig-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig -i 1,2 tdf24.hdf                                   PASSED
) else (
   echo Testing hdp dumprig -i 1,2 tdf24.hdf                                   FAILED
   more temp.txt
)
del temp.txt

fc dumprig-3.results dumprig-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig -i 1,3 tdf24.hdf                                   PASSED
) else (
   echo Testing hdp dumprig -i 1,3 tdf24.hdf                                   FAILED
   more temp.txt
)
del temp.txt

fc dumprig-4.results dumprig-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig -m 24 tdf24.hdf                                    PASSED
) else (
   echo Testing hdp dumprig -m 24 tdf24.hdf                                    FAILED
   more temp.txt
)
del temp.txt

fc dumprig-5.results dumprig-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig -r 3,4 tdf24.hdf                                   PASSED
) else (
   echo Testing hdp dumprig -r 3,4 tdf24.hdf                                   FAILED
   more temp.txt
)
del temp.txt

fc dumprig-6.results dumprig-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumprig -r 3,4 -d tdf24.hdf                                PASSED
) else (
   echo Testing hdp dumprig -r 3,4 -d tdf24.hdf                                FAILED
   more temp.txt
)
del temp.txt

cd ..\..

echo -----------------------------
echo Test command dumpvd
echo -----------------------------

hdp dumpvd temptest\tvset.hdf > temptest\dumpvd-1.results 2>&1
hdp dumpvd -i 1,3,5 temptest\tvset.hdf > temptest\dumpvd-2.results 2>&1
hdp dumpvd -r 1238,1239,1251,1252 temptest\tvset.hdf > temptest\dumpvd-3.results 2>&1
hdp dumpvd -n "Multi-Order Vdata" temptest\tvset.hdf > temptest\dumpvd-4.results 2>&1
hdp dumpvd -n "Mixed Vdata","Integer Vdata" temptest\tvset.hdf > temptest\dumpvd-5.results 2>&1
hdp dumpvd -c "Test object","No class specified" temptest\tvset.hdf > temptest\dumpvd-6.results 2>&1
hdp dumpvd -f B temptest\tvset.hdf > temptest\dumpvd-7.results 2>&1
hdp dumpvd -f "STATION_NAME","FLOATS" temptest\tvset.hdf > temptest\dumpvd-8.results 2>&1
hdp dumpvd -f "STATION_NAME","FLOATS" -d temptest\tvset.hdf > temptest\dumpvd-9.results 2>&1
hdp dumpvd temptest\tvattr.hdf > temptest\dumpvd-10.results 2>&1

cd temptest

call deleteline dumpvd-1.results 1 
call deleteline dumpvd-1.out 4
call deleteline dumpvd-2.results 1 
call deleteline dumpvd-2.out 4
call deleteline dumpvd-3.results 1 
call deleteline dumpvd-3.out 4
call deleteline dumpvd-4.results 1 
call deleteline dumpvd-4.out 4
call deleteline dumpvd-5.results 1 
call deleteline dumpvd-5.out 4
call deleteline dumpvd-6.results 1 
call deleteline dumpvd-6.out 4
call deleteline dumpvd-7.results 1 
call deleteline dumpvd-7.out 4
call deleteline dumpvd-8.results 1 
call deleteline dumpvd-8.out 4
call deleteline dumpvd-9.results 1 
call deleteline dumpvd-9.out 4
call deleteline dumpvd-10.results 1 
call deleteline dumpvd-10.out 4

cd fctemp
fc dumpvd-1.results dumpvd-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd tvset.hdf                                           PASSED
) else (
   echo Testing hdp dumpvd tvset.hdf                                           FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-2.results dumpvd-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -i 1,3,5 tvset.hdf                                  PASSED
) else (
   echo Testing hdp dumpvd -i 1,3,5 tvset.hdf                                  FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-3.results dumpvd-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -r 1238,1239,1251,1252 tvset.hdf                    PASSED
) else (
   echo Testing hdp dumpvd -r 1238,1239,1251,1252 tvset.hdf                    FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-4.results dumpvd-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -n Multi-Order Vdata tvset.hdf                      PASSED
) else (
   echo Testing hdp dumpvd -n Multi-Order Vdata tvset.hdf                      FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-5.results dumpvd-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -n Mixed Vdata,Integer Vdata tvset.hdf              PASSED
) else (
   echo Testing hdp dumpvd -n Mixed Vdata,Integer Vdata tvset.hdf              FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-6.results dumpvd-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -c Test object,No class specified tvset.hdf         PASSED
) else (
   echo Testing hdp dumpvd -c Test object,No class specified tvset.hdf         FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-7.results dumpvd-7.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -f B tvset.hdf                                      PASSED
) else (
   echo Testing hdp dumpvd -f B tvset.hdf                                      FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-8.results dumpvd-8.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -f STATION_NAME,FLOATS tvset.hdf                    PASSED
) else (
   echo Testing hdp dumpvd -f STATION_NAME,FLOATS tvset.hdf                    FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-9.results dumpvd-9.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd -f STATION_NAME,FLOATS -d tvset.hdf                 PASSED
) else (
   echo Testing hdp dumpvd -f STATION_NAME,FLOATS -d tvset.hdf                 FAILED
   more temp.txt
)
del temp.txt

fc dumpvd-10.results dumpvd-10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvd tvattr.hdf                                          PASSED
) else (
   echo Testing hdp dumpvd tvattr.hdf                                          FAILED
   more temp.txt
)
del temp.txt

cd ..\..

echo -----------------------------
echo Test command dumpvg
echo -----------------------------

hdp dumpvg temptest\tvset.hdf > temptest\dumpvg-1.results 2>&1
hdp dumpvg -i 0,1 temptest\tvset.hdf > temptest\dumpvg-2.results 2>&1
hdp dumpvg -r 3 temptest\tvset.hdf > temptest\dumpvg-3.results 2>&1
hdp dumpvg -n "Simple Vgroup" temptest\tvset.hdf > temptest\dumpvg-4.results 2>&1
hdp dumpvg -c "Test object" temptest\tvset.hdf > temptest\dumpvg-5.results 2>&1
hdp dumpvg -i 1,3,5 temptest\tdata.hdf > temptest\dumpvg-6.results 2>&1
hdp dumpvg -r 32,39 temptest\tdata.hdf > temptest\dumpvg-7.results 2>&1
hdp dumpvg -n nsamp,tdata.hdf temptest\tdata.hdf > temptest\dumpvg-8.results 2>&1
hdp dumpvg -c CDF0.0 temptest\tdata.hdf > temptest\dumpvg-9.results 2>&1
hdp dumpvg -h -c Dim0.0,Var0.0 temptest\tdata.hdf > temptest\dumpvg-10.results 2>&1
hdp dumpvg temptest\tvattr.hdf > temptest\dumpvg-11.results 2>&1

cd temptest

call deleteline dumpvg-1.results 1 
call deleteline dumpvg-1.out 4
call deleteline dumpvg-2.results 1 
call deleteline dumpvg-2.out 4
call deleteline dumpvg-3.results 1 
call deleteline dumpvg-3.out 4
call deleteline dumpvg-4.results 1 
call deleteline dumpvg-4.out 4
call deleteline dumpvg-5.results 1 
call deleteline dumpvg-5.out 4
call deleteline dumpvg-6.results 1 
call deleteline dumpvg-6.out 4
call deleteline dumpvg-7.results 1 
call deleteline dumpvg-7.out 4
call deleteline dumpvg-8.results 1 
call deleteline dumpvg-8.out 4
call deleteline dumpvg-9.results 1 
call deleteline dumpvg-9.out 4
call deleteline dumpvg-10.results 1 
call deleteline dumpvg-10.out 4
call deleteline dumpvg-11.results 1 
call deleteline dumpvg-11.out 4

cd fctemp
fc dumpvg-1.results dumpvg-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg tvset.hdf                                           PASSED
) else (
   echo Testing hdp dumpvg tvset.hdf                                           FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-2.results dumpvg-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -i 0,1 tvset.hdf                                    PASSED
) else (
   echo Testing hdp dumpvg -i 0,1 tvset.hdf                                    FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-3.results dumpvg-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -r 3 tvset.hdf                                      PASSED
) else (
   echo Testing hdp dumpvg -r 3 tvset.hdf                                      FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-4.results dumpvg-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -n Simple Vgroup tvset.hdf                          PASSED
) else (
   echo Testing hdp dumpvg -n Simple Vgroup tvset.hdf                          FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-5.results dumpvg-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -c Test object tvset.hdf                            PASSED
) else (
   echo Testing hdp dumpvg -c Test object tvset.hdf                            FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-6.results dumpvg-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -i 1,3,5 tdata.hdf                                  PASSED
) else (
   echo Testing hdp dumpvg -i 1,3,5 tdata.hdf                                  FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-7.results dumpvg-7.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -r 32,39 tdata.hdf                                  PASSED
) else (
   echo Testing hdp dumpvg -r 32,39 tdata.hdf                                  FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-8.results dumpvg-8.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -n nsamp,tdata.hdf tdata.hdf                        PASSED
) else (
   echo Testing hdp dumpvg -n nsamp,tdata.hdf tdata.hdf                        FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-9.results dumpvg-9.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -c CDF0.0 tdata.hdf                                 PASSED
) else (
   echo Testing hdp dumpvg -c CDF0.0 tdata.hdf                                 FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-10.results dumpvg-10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg -h -c Dim0.0,Var0.0 tdata.hdf                       PASSED
) else (
   echo Testing hdp dumpvg -h -c Dim0.0,Var0.0 tdata.hdf                       FAILED
   more temp.txt
)
del temp.txt

fc dumpvg-11.results dumpvg-11.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpvg tvattr.hdf                                          PASSED
) else (
   echo Testing hdp dumpvg tvattr.hdf                                          FAILED
   more temp.txt
)
del temp.txt

cd ..\..

echo -----------------------------
echo Test command dumpgr
echo -----------------------------

hdp dumpgr temptest\grtdfui82.hdf > temptest\dumpgr-1.results 2>&1
hdp dumpgr -i 0,1,3 temptest\grtdfui82.hdf > temptest\dumpgr-2.results 2>&1
hdp dumpgr -i 0 temptest\grtdfui82.hdf > temptest\dumpgr-3.results 2>&1
hdp dumpgr -n Image_array_5 temptest\grtdfui82.hdf > temptest\dumpgr-4.results 2>&1
hdp dumpgr -r 6,2,3 temptest\grtdfui82.hdf > temptest\dumpgr-5.results 2>&1
hdp dumpgr -r 6 -d temptest\grtdfui82.hdf > temptest\dumpgr-6.results 2>&1
hdp dumpgr -o my.dat temptest\grtdfui82.hdf > temptest\dumpgr-7.results 2>&1
hdp dumpgr -o mybin.dat -b temptest\grtdfui82.hdf > temptest\dumpgr-8.results 2>&1
hdp dumpgr temptest\grtdfui83.hdf > temptest\dumpgr-9.results 2>&1
hdp dumpgr temptest\grtdfui84.hdf > temptest\dumpgr-10.results 2>&1
hdp dumpgr temptest\grtdfui162.hdf > temptest\dumpgr-11.results 2>&1
hdp dumpgr temptest\grtdfi322.hdf > temptest\dumpgr-12.results 2>&1
hdp dumpgr -p temptest\Image_with_Palette.hdf > temptest\dumpgr-13.results 2>&1
hdp dumpgr -h temptest\Image_with_Palette.hdf > temptest\dumpgr-14.results 2>&1
hdp dumpgr -r 2,4 -pd temptest\Image_with_Palette.hdf > temptest\dumpgr-15.results 2>&1
hdp dumpgr -r 6 -d -s temptest\grtdfui82.hdf > temptest\dumpgr-16.results 2>&1
hdp dumpgr -r 6 -m 1 temptest\grtdfui82.hdf > temptest\dumpgr-17.results 2>&1
hdp dumpgr -c temptest\grtdfi322.hdf > temptest\dumpgr-18.results 2>&1
hdp dumpgr -g -l temptest\grtdfi322.hdf > temptest\dumpgr-19.results 2>&1

cd temptest

call deleteline dumpgr-1.results 1 
call deleteline dumpgr-1.out 4
call deleteline dumpgr-2.results 1 
call deleteline dumpgr-2.out 4
call deleteline dumpgr-3.results 1 
call deleteline dumpgr-3.out 4
call deleteline dumpgr-4.results 1 
call deleteline dumpgr-4.out 4
call deleteline dumpgr-5.results 1 
call deleteline dumpgr-5.out 4
call deleteline dumpgr-6.results 1 
call deleteline dumpgr-6.out 4
call deleteline dumpgr-7.results 1 
call deleteline dumpgr-7.out 4
call deleteline dumpgr-8.results 1 
call deleteline dumpgr-8.out 4
call deleteline dumpgr-9.results 1 
call deleteline dumpgr-9.out 4
call deleteline dumpgr-10.results 1 
call deleteline dumpgr-10.out 4
call deleteline dumpgr-11.results 1 
call deleteline dumpgr-11.out 4
call deleteline dumpgr-12.results 1 
call deleteline dumpgr-12.out 4
call deleteline dumpgr-13.results 1 
call deleteline dumpgr-13.out 4
call deleteline dumpgr-14.results 1 
call deleteline dumpgr-14.out 4
call deleteline dumpgr-15.results 1 
call deleteline dumpgr-15.out 4
call deleteline dumpgr-16.results 1 
call deleteline dumpgr-16.out 4
call deleteline dumpgr-17.results 1 
call deleteline dumpgr-17.out 4
call deleteline dumpgr-18.results 1 
call deleteline dumpgr-18.out 4
call deleteline dumpgr-19.results 1 
call deleteline dumpgr-19.out 4

cd fctemp
fc dumpgr-1.results dumpgr-1.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr grtdfui82.hdf                                       PASSED
) else (
   echo Testing hdp dumpgr grtdfui82.hdf                                       FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-2.results dumpgr-2.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -i 0,1,3 grtdfui82.hdf                              PASSED
) else (
   echo Testing hdp dumpgr -i 0,1,3 grtdfui82.hdf                              FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-3.results dumpgr-3.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -i 0 grtdfui82.hdf                                  PASSED
) else (
   echo Testing hdp dumpgr -i 0 grtdfui82.hdf                                  FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-4.results dumpgr-4.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -n Image_array_5 grtdfui82.hdf                      PASSED
) else (
   echo Testing hdp dumpgr -n Image_array_5 grtdfui82.hdf                      FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-5.results dumpgr-5.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -r 6,2,3 grtdfui82.hdf                              PASSED
) else (
   echo Testing hdp dumpgr -r 6,2,3 grtdfui82.hdf                              FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-6.results dumpgr-6.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -r 6 -d grtdfui82.hdf                               PASSED
) else (
   echo Testing hdp dumpgr -r 6 -d grtdfui82.hdf                               FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-7.results dumpgr-7.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -o my.dat grtdfui82.hdf                             PASSED
) else (
   echo Testing hdp dumpgr -o my.dat grtdfui82.hdf                             FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-8.results dumpgr-8.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -o mybin.dat -b grtdfui82.hdf                       PASSED
) else (
   echo Testing hdp dumpgr -o mybin.dat -b grtdfui82.hdf                       FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-9.results dumpgr-9.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr grtdfui83.hdf                                       PASSED
) else (
   echo Testing hdp dumpgr grtdfui83.hdf                                       FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-10.results dumpgr-10.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr grtdfui84.hdf                                       PASSED
) else (
   echo Testing hdp dumpgr grtdfui84.hdf                                       FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-11.results dumpgr-11.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr grtdfui162.hdf                                      PASSED
) else (
   echo Testing hdp dumpgr grtdfui162.hdf                                      FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-12.results dumpgr-12.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr grtdfi322.hdf                                       PASSED
) else (
   echo Testing hdp dumpgr grtdfi322.hdf                                       FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-13.results dumpgr-13.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -p Image_with_Palette.hdf                           PASSED
) else (
   echo Testing hdp dumpgr -p Image_with_Palette.hdf                           FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-14.results dumpgr-14.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -h Image_with_Palette.hdf                           PASSED
) else (
   echo Testing hdp dumpgr -h Image_with_Palette.hdf                           FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-15.results dumpgr-15.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -r 2,4 -pd Image_with_Palette.hdf                   PASSED
) else (
   echo Testing hdp dumpgr -r 2,4 -pd Image_with_Palette.hdf                   FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-16.results dumpgr-16.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -r 6 -d -s grtdfui82.hdf                            PASSED
) else (
   echo Testing hdp dumpgr -r 6 -d -s grtdfui82.hdf                            FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-17.results dumpgr-17.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -r 6 -m 1 grtdfui82.hdf                             PASSED
) else (
   echo Testing hdp dumpgr -r 6 -m 1 grtdfui82.hdf                             FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-18.results dumpgr-18.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -c grtdfi322.hdf                                    PASSED
) else (
   echo Testing hdp dumpgr -c grtdfi322.hdf                                    FAILED
   more temp.txt
)
del temp.txt

fc dumpgr-19.results dumpgr-19.out >temp.txt
if %ERRORLEVEL%==0 (
   echo Testing hdp dumpgr -g -l grtdfi322.hdf                                 PASSED
) else (
   echo Testing hdp dumpgr -g -l grtdfi322.hdf                                 FAILED
   more temp.txt
)
del temp.txt

cd ..\..

del my*.dat
rmdir /s/q temptest

cd ..\..\..
