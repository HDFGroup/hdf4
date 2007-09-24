@ ECHO OFF
REM This batch file is used for HDF4 utilities tests.
REM By Xuan Bai
REM Created on 11/30/2004
REM Last Modified on 7/30/2007

echo *** Testing gif2hdf and hdf2gif  ***
REM gif2hdf and hdf2gif 
pushd hdf\util\%1
rmdir /s/q gif_hdf_temp >temp.txt 2>&1
del temp.txt
mkdir gif_hdf_temp

hdf2gif ..\testfiles\skull.hdf  gif_hdf_temp\skull.gif
gif2hdf ..\testfiles\SunWheel.gif  gif_hdf_temp\SunWheel.hdf
gif2hdf ..\testfiles\bttrfly.gif  gif_hdf_temp\bttrfly.hdf

echo  *********************** NOTE ****************************
echo  Use WEB Browser and JHV to compare
echo  hdf4\hdf\util\testfiles\SunWheel.gif with
echo  hdf4\hdf\util\%1\gif_hdf_temp\SunWheel.hdf
echo  and     
echo  hdf4\hdf\util\testfiles\bttrfly.gif with
echo  hdf4\hdf\util\%1\gif_hdf_temp\bttrfly.hdf
echo.
echo  Use JHV and WEB Browser to compare
echo  hdf4\hdf\util\testfiles\skull.hdf with
echo  hdf4\hdf\util\%1\gif_hdf_temp\skull.gif
echo  ********************* END NOTE **************************"
echo. 

REM hdf24to8
echo *** Testing hdf24to8  ***
hdf24to8 ..\testfiles\head.r24 head8.hdf
hdftor8 head8.hdf
fc /b img001-263.328 ..\testfiles\head.r8 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set error1=0
) else (
   set error1=1
   more temp.txt
)
del temp.txt
del head8.hdf
del img001-263.328
del pal001

REM hdfed
echo *** Testing hdfed  ***
copy ..\testfiles\storm110.hdf . >temp.txt 2>&1
copy ..\testfiles\ntcheck.hdf . >>temp.txt 2>&1
del temp.txt
hdfed -batch < ..\hdfed.input1 > hdfed.tmp 2>&1
REM   The following test is commented out due to a Windows-specific bug.
REM   Windows outputs the exponent of in scientific notation with a minimum
REM   of 3 digits, while most other platforms use 2.  This causes false-positives
REM   in our test results.  Rather than maintaining our over version of the testfiles,
REM   we simply comment out the test.
REM fc hdfed.tmp ..\..\..\hdf\util\hdfed.out1 ^> temp.txt 2^>&1
REM find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
REM    more temp.txt
)
REM del temp.txt
del hdfed.tmp
del storm110.hdf
del ntcheck.hdf

REM ristosds
echo *** Testing ristosds  ***
copy ..\testfiles\storm*.hdf . >temp.txt 2>&1
del temp.txt
ristosds storm110.hdf storm120.hdf storm130.hdf -o storm.hdf > temp.txt 2>&1
del temp.txt
hdfed -batch < ..\ristosds.input1 > hdfed.tmp1 2>&1
fc hdfed.tmp1 ..\ristosds.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del storm*.hdf
del hdfed.tmp1

REM hdfpack
echo *** Testing hdfpack  ***
copy ..\testfiles\test.hdf . >temp.txt 2>&1
del temp.txt
hdfpack test.hdf test.pck
hdfpack -b test.hdf test.blk
hdfls test.hdf > hdfls.tmp1 2>&1
hdfls test.pck >> hdfls.tmp1 2>&1
findstr /V "library String" hdfls.tmp1 > hdfls_temp.tmp1
del hdfls.tmp1
rename hdfls_temp.tmp1 hdfls.tmp1
fc hdfls.tmp1 ..\hdfpack.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del hdfls.tmp1
del test.blk
del test.hdf
del test.pck

REM hdftopal/paltohdf
echo *** Testing hdftopal/paltohdf  ***
copy ..\testfiles\palette.raw . >temp.txt 2>&1
del temp.txt
paltohdf palette.raw palette.hdf
hdftopal palette.hdf palette.raw.new
fc /b palette.raw palette.raw.new > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del palette.*

REM r8tohdf/hdftor8
echo *** Testing r8tohdf/hdftor8  ***
copy ..\testfiles\storm*.raw . >temp.txt 2>&1
del temp.txt
copy ..\testfiles\palette.raw . >temp.txt 2>&1
del temp.txt
r8tohdf 57 57 storm.hdf storm110.raw storm120.raw storm130.raw storm140.raw
r8tohdf 57 57 storm.hdf -p palette.raw -i storm110.raw
hdftor8 storm.hdf
hdfls -l storm.hdf > hdfls.tmp2 2>&1
findstr /V "library String" hdfls.tmp2 > hdfls_temp.tmp2
del hdfls.tmp2
rename hdfls_temp.tmp2 hdfls.tmp2
fc hdfls.tmp2 ..\..\..\hdf\util\hdftor8.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt

fc /b img001-057.057  storm110.raw > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt

fc /b img002-057.057  storm120.raw > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt

fc /b img003-057.057  storm130.raw > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt

fc /b img004-057.057  storm140.raw > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del *.raw
del *.057
del storm.hdf
del hdfls.tmp2
del pal005

REM hdfcomp
echo *** Testing hdfcomp  ***
copy ..\testfiles\storm1*0.hdf . >temp.txt 2>&1
del temp.txt
hdfcomp allstorms.hdf storm110.hdf storm120.hdf storm130.hdf
hdfcomp allcomp.hdf -c storm110.hdf storm120.hdf storm130.hdf
hdfls -l allstorms.hdf > hdfls.tmp3 2>&1
hdfls -l allcomp.hdf >> hdfls.tmp3 2>&1
findstr /V "library String" hdfls.tmp3 > hdfls_temp.tmp3
del hdfls.tmp3
rename hdfls_temp.tmp3 hdfls.tmp3
fc hdfls.tmp3 ..\hdfcomp.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del allstorms.hdf
del allcomp.hdf
del hdfls.tmp3
del storm1*0.hdf

REM jpeg2hdf/hdf2jpeg
echo *** Testing jpeg2hdf/hdf2jpeg  ***
copy ..\testfiles\jpeg_img.jpg . >temp.txt 2>&1
del temp.txt
jpeg2hdf jpeg_img.jpg jpeg.hdf
hdf2jpeg jpeg.hdf jpeg2.jpg
hdfls -l jpeg.hdf > hdfls.tmp4 2>&1
findstr /V "library String" hdfls.tmp4 > hdfls_temp.tmp4
del hdfls.tmp4
rename hdfls_temp.tmp4 hdfls.tmp4
fc hdfls.tmp4 ..\jpeg2hdf.out1 > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt

fc /b jpeg_img.jpg jpeg2.jpg > temp.txt 2>&1
find "FC: no diff" temp.txt > nul
if %ERRORLEVEL%==0 (
   set errors=%error1%
) else (
   set errors=%error1%+1
   set error1=%error%
   more temp.txt
)
del temp.txt
del hdfls.tmp4
del jpeg.hdf
del jpeg2.jpg
del jpeg_img.jpg

if "%errors%"=="0" (
   echo =================================
   echo HDF Utilities tests passed.
   echo =================================
) else (
   echo =================================
   echo HDF Utilities tests failed.
   echo =================================
)

popd
