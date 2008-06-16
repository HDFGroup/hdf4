@echo OFF
rem /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
rem * Copyright by The HDF Group.                                               *
rem * Copyright by the Board of Trustees of the University of Illinois.         *
rem * All rights reserved.                                                      *
rem *                                                                           *
rem * This file is part of HDF.  The full HDF copyright notice, including       *
rem * terms governing use, modification, and redistribution, is contained in    *
rem * the files COPYING and Copyright.html.  COPYING can be found at the root   *
rem * of the source code distribution tree; Copyright.html can be found at      *
rem * http://hdfgroup.org/products/hdf4/doc/Copyright.html.  If you do not have *
rem * access to either file, you may request a copy from help@hdfgroup.org.     *
rem * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
rem
rem  File Name: testutil.bat
rem  This batch file is used to test hdf utilities
rem  This script has the following options:
rem    testutil (release / debug)
rem             Test either the release or debug version of hdf utilities
rem  Created:       Scott Wegner, 6/10/08
rem  Last Updated:  Scott Wegner, 6/16/08

setlocal enabledelayedexpansion
pushd %~dp0

rem Set srcdir to "." rather than %CD%, because gif2hdf has a strict limit on
rem group name length
set srcdir=.

rem Unix script uses machinetype to set values, but is unneccessary on Windows
rem --SJW 6/10/08
rem machinetype="$1"
rem
rem # Check if target machine supports 32 bits datatype.
rem # "true" is TRUE; "" means FALSE.
rem case $machinetype in
rem     *unicos*) has32="";;
rem     *) has32="true";;
rem esac
set has32=true

rem initialize error variable
set errors=0
set haserr=0

rem setup hdfed command which is used often
set hdfed=%CD%\%1\hdfed
rem use -batch mode for no prompt
set hdfedcmd=%hdfed% -batch
rem Windows doesn't have SED, so we will have to manipulate output otherwise
rem SED="sed -e /library/,/String/d"  # filter out the library version

echo.
echo.==============================
echo.HDF Utilities tests started
echo.==============================

rem gif2hdf and hdf2gif
if not exist %CD%\%1\gif2hdf (
    echo.** gif2hdf is not available ***
) else if not exist %CD%\%1\hdf2gif (
    echo.** hdf2gif is not available ***
) else (
    echo.** Testing gif2hdf and hdf2gif  ***
    del /f /q gif_hdf_temp 2> nul
    rmdir /s /q gif_hdf_temp 2> nul
    mkdir gif_hdf_temp
    %CD%\%1\hdf2gif %srcdir%\testfiles\skull.hdf  gif_hdf_temp\skull.gif
    %CD%\%1\gif2hdf %srcdir%\testfiles\SunWheel.gif  gif_hdf_temp\SunWheel.hdf
    %CD%\%1\gif2hdf %srcdir%\testfiles\bttrfly.gif  gif_hdf_temp\bttrfly.hdf
)

echo. *********************** NOTE ****************************
echo. Use WEB Browser and JHV to compare
echo. testfiles\SunWheel.gif and testfiles\bttrfly.gif
echo. with gif_hdf_temp\SunWheel.hdf and gif_hdf_temp\bttrfly.hdf
echo.
echo. Use JHV  and WEB Browser to compare
echo. testfiles\skull.hdf and gif_hdf_temp\skull.gif
echo. ********************* END NOTE **************************
echo.

set errors=0

rem hdf24to8
rem "uncompress" utiltiy became obsolete; replaced with gunzip EIP 2004/12/01
rem gunzip not available on Windows; simply untarring files SJW 2007/7/31
if not exist %CD%\%1\hdf24to8 (
    echo.** hdf24to8 is not available ***
) else if not exist %CD%\%1\hdftor8 (
    echo.** hdftor8 is not available ***
) else (
    echo.** Testing hdf24to8  ***
    del /f /q head.r24 head.r8 head8.hdf img001-263.328 pal001 2> nul
    rem cp $srcdir/testfiles/head.r24.Z $srcdir/testfiles/head.r8.Z .
    copy /y %srcdir%\testfiles\head.r24 . > nul
    copy /y %srcdir%\testfiles\head.r8 . > nul
    %CD%\%1\hdf24to8 head.r24 head8.hdf
    %CD%\%1\hdftor8 head8.hdf
    fc /b img001-263.328 head.r8 > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q head.r24 head.r8 head8.hdf img001-263.328 pal001
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdf24to8 might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q head.r24 head.r8 head8.hdf img001-263.328 pal001
    echo. copy /y testfiles\head.r24 .
    echo. copy /y testfiles\head.r8 .
    echo. %1\hdf24to8 head.r24 head8.hdf
    echo. %1\hdftor8 head8.hdf
    echo. fc /b img001-263.328 head.r8
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem hdfed
if not exist %hdfed% (
    echo.** hdfed not available ***
) else (
    echo.** Testing hdfed  ***
    del /f /q storm110.hdf ntcheck.hdf hdfed.tmp 2> nul
    copy /y %srcdir%\testfiles\storm110.hdf . > nul
    copy /y %srcdir%\testfiles\ntcheck.hdf . > nul
    %hdfedcmd% < %srcdir%\hdfed.input1 > hdfed.tmp 2>&1
    rem On Windows, don't actually compare results.  This is due to a formatting
    rem issue related to floating point numbers and scientific notation.  Windows
    rem uses 3 exponent digits, while other platforms use 2.  This creates spurious
    rem errors.  --SJW 6/12/08
    rem fc hdfed.tmp %srcdir%\hdfed.out1 ^> nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q storm110.hdf ntcheck.hdf hdfed.tmp
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdfed might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo. 
    echo. del /f /q storm110.hdf ntcheck.hdf hdfed.tmp
    echo. copy /y testfiles\storm110.hdf .
    echo. copy /y testfiles\ntcheck.hdf .
    echo. %hdfedcmd% ^< hdfed.input1 ^> hdfed.tmp 2^>^&1
    rem echo. fc hdfed.tmp %srcdir%\hdfed.out1
    echo. ******* END NOTE *************
    echo.
    errors=0
)


rem hdfpack
if not exist %CD%\%1\hdfpack (
    echo.** hdfpack not available ***
) else (
    echo.** Testing hdfpack  ***
    del /f /q test.hdf test.blk test.pck hdfls.tmp1 2>nul
    copy /y %srcdir%\testfiles\test.hdf . > nul
    %CD%\%1\hdfpack test.hdf test.pck
    %CD%\%1\hdfpack -b test.hdf test.blk
    rem Use findstr instead of SED
    (%CD%\%1\hdfls test.hdf | findstr /v "library String" ) > hdfls.tmp1 2>&1
    (%CD%\%1\hdfls test.pck | findstr /v "library String" ) >> hdfls.tmp1 2>&1
    fc hdfls.tmp1 %srcdir%\hdfpack.out1 > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q test.hdf test.blk test.pck hdfls.tmp1
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdfpack might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q test.hdf test.blk test.pck hdfls.tmp1
    echo. copy /y testfiles\test.hdf .
    echo. %1\hdfpack test.hdf test.pck
    echo. %1\hdfpack -b test.hdf test.blk
    echo. ^(%1\hdfls test.hdf ^| findstr /v "library String" ^) ^> hdfls.tmp1 2^>^&1
    echo. ^(%1\hdfls test.pck ^| findstr /v "library String" ^) ^>^> hdfls.tmp1 2^>^&1
    echo. fc hdfls.tmp1 hdfpack.out1
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem hdftopal/paltohdf
if not exist %CD%\%1\hdftopal (
    echo.** hdftopal not available ***
) else if not exist %CD%\%1\paltohdf (
    echo.** paltohdf not available ***
) else (
    echo.** Testing hdftopal\paltohdf  ***
    del /f /q palette.* 2>nul
    copy /y testfiles\palette.raw . > nul
    %CD%\%1\paltohdf palette.raw palette.hdf
    %CD%\%1\hdftopal palette.hdf palette.raw.new
    fc palette.raw palette.raw.new > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q palette.* 
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdftopal or paltohdf might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q palette.* 
    echo. copy testfiles\palette.raw .
    echo. %1\paltohdf palette.raw palette.hdf
    echo. %1\hdftopal palette.hdf palette.raw.new
    echo. fc palette.raw palette.raw.new
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem r8tohdf/hdftor8
if not exist %CD%\%1\r8tohdf (
    echo.** r8tohdf not available ***
) else if not exist %CD%\%1\hdftor8 (
    echo.** hdftor8 not available ***
) else if not exist %CD%\%1\hdfls (
    echo.** hdfls not available ***
) else (
    echo.** Testing r8tohdf\hdftor8  ***
    del /f /q storm* img* palette.raw hdfls.tmp2 pal005 2>nul
    copy /y %srcdir%\testfiles\storm*.raw . > nul
    copy /y %srcdir%\testfiles\palette.raw . > nul
    rem On Windows, hardcode the names of each testfile, rather than using storm*.raw
    %CD%\%1\r8tohdf 57 57 storm.hdf storm110.raw storm120.raw storm130.raw storm140.raw
    %CD%\%1\r8tohdf 57 57 storm.hdf -p palette.raw -i storm110.raw
    %CD%\%1\hdftor8 storm.hdf
    rem Use findstr instead of SED
    (%CD%\%1\hdfls -l storm.hdf | findstr /v "library String") > hdfls.tmp2 2>&1
    fc  hdfls.tmp2 %srcdir%\hdftor8.out1 > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    fc img001-057.057  storm110.raw > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    fc img002-057.057  storm120.raw > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    fc img003-057.057  storm130.raw > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    fc img004-057.057  storm140.raw > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q -f storm* img* palette.raw hdfls.tmp2 pal005
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. r8tohdf or hdftor8 might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q storm* img* palette.raw hdfls.tmp2 pal005
    echo. copy /y testfiles\storm*.raw .
    echo. copy /y testfiles\palette.raw .
    echo. %1\r8tohdf 57 57 storm.hdf storm110.raw storm120.raw storm130.raw storm140.raw
    echo. %1\r8tohdf 57 57 storm.hdf -p palette.raw -i storm110.raw
    echo. %1\hdftor8 storm.hdf
    echo. ^(%1\hdfls -l storm.hdf ^| findstr /v "library String"^) ^> hdfls.tmp2 2^>^&1
    echo. fc  hdfls.tmp2 hdftor8.out1
    echo. fc img001-057.057  storm110.raw 
    echo. fc img002-057.057  storm120.raw 
    echo. fc img003-057.057  storm130.raw 
    echo. fc img004-057.057  storm140.raw 
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem hdfcomp
if not exist %CD%\%1\hdfcomp (
    echo.** hdfcomp not available ***
) else if not exist %CD%\%1\hdfls (
    echo.** hdfls not available ***
) else (
    echo.** Testing hdfcomp  ***
    del /f /q storm*.hdf all*.hdf hdfls.tmp3 2> nul
    copy /y %srcdir%\testfiles\storm*.hdf . > nul
    rem On Windows, hardcode the names of each testfile, rather than using storm*.raw
    %CD%\%1\hdfcomp allstorms.hdf storm110.hdf storm120.hdf storm130.hdf
    %CD%\%1\hdfcomp allcomp.hdf -c storm110.hdf storm120.hdf storm130.hdf
    rem Use findstr instead of SED
    ( %CD%\%1\hdfls -l allstorms.hdf | findstr /v "library String" ) > hdfls.tmp3 2>&1
    ( %CD%\%1\hdfls -l allcomp.hdf | findstr /v "library String" ) >> hdfls.tmp3 2>&1
    fc hdfls.tmp3 %srcdir%\hdfcomp.out1 > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q storm*.hdf all*.hdf hdfls.tmp3
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdfcomp might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q storm*.hdf all*.hdf hdfls.tmp3
    echo. copy /y testfiles\storm*.hdf .
    echo. %1\hdfcomp allstorms.hdf storm110.hdf storm120.hdf storm130.hdf
    echo. %1\hdfcomp allcomp.hdf -c storm110.hdf storm120.hdf storm130.hdf
    echo. ^( %1\hdfls -l allstorms.hdf ^| findstr /v "library String" ^) ^> hdfls.tmp3 2^>^&1
    echo. ^( %1\hdfls -l allcomp.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp3 2^>^&1
    echo. fc hdfls.tmp3 hdfcomp.out1 
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem jpeg2hdf/hdf2jpeg
if not exist %CD%\%1\jpeg2hdf (
    echo.** jpeg2hdf available ***
) else if not exist %CD%\%1\hdf2jpeg (
    echo.** hdf2jpeg available ***
) else if not exist %CD%\%1\hdfls (
    echo.** hdfls not available ***
) else (
    echo.** Testing jpeg2hdf\hdf2jpeg  ***
    del /f /q jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4 2> nul
    copy /y %srcdir%\testfiles\jpeg_img.jpg . > nul
    %CD%\%1\jpeg2hdf jpeg_img.jpg jpeg.hdf
    %CD%\%1\hdf2jpeg jpeg.hdf jpeg2.jpg
    rem Use findstr instead of SED
    (%CD%\%1\hdfls -l jpeg.hdf | findstr /v "library String" ) > hdfls.tmp4 2>&1
    fc hdfls.tmp4 %srcdir%\jpeg2hdf.out1 > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    fc jpeg_img.jpg jpeg2.jpg > nul
    if !errorlevel! neq 0 (
        set errors=1
    )
    del /f /q jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. jpeg2hdf or hdf2jpeg might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q jpeg.hdf jpeg_img.jpg jpeg2.jpg hdfls.tmp4
    echo. copy /y testfiles\jpeg_img.jpg .
    echo. %1\jpeg2hdf jpeg_img.jpg jpeg.hdf
    echo. %1\hdf2jpeg jpeg.hdf jpeg2.jpg
    echo. ^(%1\hdfls -l jpeg.hdf ^| findstr /v "library String" ^) ^> hdfls.tmp4 2^>^&1
    echo. fc hdfls.tmp4 jpeg2hdf.out1
    echo. fc jpeg_img.jpg jpeg2.jpg
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem
rem fp2hdf is now called hdfimport, and is in mfhdf/hdfimport
rem

rem
rem Check errors result
if %haserr% equ 0 (
    echo.==============================
    echo.HDF Utilities tests passed.
    echo.==============================
) else (
    echo.**************************************
    echo.HDF Utilities tests encountered errors
    echo.**************************************
)

echo.
popd
endlocal & exit /b %haserr%
