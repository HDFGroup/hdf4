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
rem  File Name: testhdp.bat
rem  This batch file is used to test hdp (dumper)
rem  This script has the following options:
rem    testhdp (release / debug) [other options]
rem             Test either the release or debug version of hdp (dumper)
rem             [other options] Options from Unix script, see :usage for details
rem  Created:       Scott Wegner, 6/11/08
rem  Last Updated:  Scott Wegner, 6/11/08

setlocal enabledelayedexpansion
pushd %~dp0

set srcdir=%CD%
set currentdir=%CD%

rem Determine whether the szip library is available
rem On Windows, this is determined by parsing h4config.h
findstr /b /i /c:"#define H4_HAVE_LIBSZ" ..\..\hdf\src\h4config.h > nul
if %errorlevel% equ 0 (
    set use_comp_szip=yes
) else (
    set use_comp_szip=no
)

rem Definitions of commands and variables
rem The tool name
set hdp=hdp
rem The path of the tool binary
set hdp_bin=%CD%\%1\%hdp%

set rm=del /f /q
set diff=fc
rem cmp will need to be emulated through redirection in each use
rem set cmp='cmp -s'
rem number of errors (0)
set nerrors=0
rem quit on error (not)
set quitonerr=0
rem no cleaning temp. files (yes)
set noclean=0
rem dumper sub-command to test only
set only=x
rem dumper sub-command to test not
set except=

shift

goto :main


rem Definitions of functions/shorthands
rem

rem Print Usage of the command
:usage
    echo.Usage: %0 [-help] [-noclean] [-quit] [-except ^<command^>] [-only ^<command^>]
    echo.    -help: display help information
    echo.    -noclean: do not clean away temporary files
    echo.    -quit: quit immediately if any test fails
    echo.    -except: skip one specific command
    echo.    -only: test one specific command
    echo.^<command^> can be one of {list, dumpsds, dumprig, dumpvd, dumpvg, dumpgr}
    exit /b 0
    
rem Print message with formats according to message level ($1)
:mesg
  set level=%1
  
    rem We define %params% here because Windows `shift` command doesn't affect
    rem the %* variable.  --SJW 8/23/07
    set params=%*
    set params=!params:* =!

    if %level% equ 0 (
        echo.=============================
        echo.%params%
        echo.=============================
    ) else if %level% equ 3 (
        echo.-----------------------------
        echo.%params%
        echo.-----------------------------
    ) else if %level% equ 6 (
        echo.*** %params% ***
    ) else (
        echo.MESG^(^): Unknown level ^(%level%^)
        exit /b 1
    )
    exit /b 0

rem Run the test to produce an output file which is then
rem compared with the expected (%1) output.
rem Note that this can be used to produce the expected
rem output files by replace "%output%" with "%expected%"
rem in the run-the-test commands.
:test
    rem parse the arguments
    set output=%CD%\tmp.out
    set expected=%srcdir%\testfiles\%1
    
    rem We define %params% here because Windows `shift` command doesn't affect
    rem the %* variable.  --SJW 8/23/07
    set params=%*
    set params=!params:* =!
    
    rem print a id banner
    call :mesg 6 !params!
    
    rem run the test
    type nul > %output%
    rem The first three lines of the "correct output" file are used to
    rem identify the test.  Copy them to the output to make cmp happy.
    rem Windows doesnt have SED, so we need to use a little bit of a hack
    set lineCount=0
    for /f "tokens=* delims=" %%a in (%expected%) do (
        set /a lineCount=!lineCount!+1
        if !lineCount! gtr 3 (
            goto after
        )
        echo.%%a>>%output%
    )
    :after
    
    pushd %srcdir%\testfiles
    %hdp_bin% %params%>>%output%
    popd
    
    fc %expected% %output% > nul
    
    if %errorlevel% neq 0 (
        %diff% %expected% %output%
        echo.   ^<^<^< FAILED ^>^>^>
        set /a nerrors=%nerrors%+1
        
        if %quitonerr% gtr 0 (
            rem quitonerr not supported on Windows.  Simply quit this test
            rem without cleanup
            exit /b 1
        )
    )
    
    if %noclean% equ 0 (
        %rm% %output%
    )
    
    exit /b 0

    
    
rem Report the result and exit
:finish
    if %nerrors% equ 0 (
        call :mesg 0 All hdp tests passed
    ) else (
        call :mesg 0 hdp tests failed: %nerrors%
    )
    goto end



rem ===============
rem  Main Body
rem ===============
:main

rem parse arguments
:parse
    if not "%1"=="" (
        if "%1"=="-quit" (
            set quitonerr=1
        ) else if "%1"=="-noclean" (
            set noclean=1
        ) else if "%1"=="-help" (
            call :usage
            exit /b 0
        ) else if "%1"=="-only" (
            if "%2"=="list" (
                set only=%2
            ) else if "%2"=="dumpsds" (
                set only=%2
            ) else if "%2"=="dumprig" (
                set only=%2
            ) else if "%2"=="dumpvd" (
                set only=%2
            ) else if "%2"=="dumpvg" (
                set only=%2
            ) else if "%2"=="dumpgr" (
                set only=%2
            ) else (
                echo.Unknown command: %2
                call :usage
                exit /b 1
            )
            shift
        ) else if "%1"=="-except" (
            if "%2"=="list" (
                set except=%2
            ) else if "%2"=="dumpsds" (
                set except=%2
            ) else if "%2"=="dumprig" (
                set except=%2
            ) else if "%2"=="dumpvd" (
                set except=%2
            ) else if "%2"=="dumpvg" (
                set except=%2
            ) else if "%2"=="dumpgr" (
                set except=%2
            ) else (
                echo.Unknown command: %2
                call :usage
                exit /b 1
            )
            shift
        ) else (
            echo.Unkown option:%1
            call :usage
            exit /b 1
        )
        shift
    )
    if not "%1"=="" (
        goto parse
    )
    
    rem Print a beginning banner
    call :mesg 0 Running hdp tests
    
    rem Test command list
    set testCmd=list
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        call :test list-1.out list tdata.hdf
        call :test list-2.out list -l tdata.hdf
        call :test list-3.out list -d tdata.hdf
        call :test list-4.out list -e tdata.hdf
        call :test list-5.out list -t 720 tdata.hdf
        call :test list-6.out list -d -t "Numeric Data Group" tdata.hdf
        call :test list-7.out list -g tdata.hdf
        call :test list-8.out list -a tdata.hdf
        call :test list-9.out list -a Example6.hdf
        call :test list-10.out list -n Example6.hdf
    )
    
    rem Test command dumpsds
    set testCmd=dumpsds
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        rem Test 1 prints all datasets
        call :test dumpsds-1.out dumpsds swf32.hdf

        rem Tests 2 and 3 print datasets given their indices
        call :test dumpsds-2.out dumpsds -i 2 swf32.hdf
        call :test dumpsds-3.out dumpsds -i 1,3 swf32.hdf

        rem Test 4 should fail with error message: "SD with name Time: not found"
        call :test dumpsds-4.out dumpsds -n Time swf32.hdf

        rem Test 5 prints datasets given their names 
        call :test dumpsds-5.out dumpsds -n fakeDim0,Data-Set-2 swf32.hdf

        rem Test 6 prints datasets given their ref numbers
        call :test dumpsds-6.out dumpsds -r 3,2 swf32.hdf

        rem Test 7 prints only data of the datasets selected by their ref numbers
        call :test dumpsds-7.out dumpsds -r 3,2 -d swf32.hdf

        rem Test 8 prints only header information
        call :test dumpsds-8.out dumpsds -h swf32_fileattr.hdf

        rem Test 9 prints data in clean format, no \digit's
        call :test dumpsds-9.out dumpsds -c swf32_fileattr.hdf

        rem Test 10 prints contents of file without file attribute's data
        call :test dumpsds-10.out dumpsds -g swf32_fileattr.hdf

        rem Test 11 prints contents of file without local attribute's data
        call :test dumpsds-11.out dumpsds -l swf32_fileattr.hdf

        rem Test 12 prints a dataset by name and the name is very long
        call :test dumpsds-12.out dumpsds -h -n "The name of this dataset is long and it is used to test the new variable length name feature." SDSlongname.hdf

        rem Test 13 prints contents of file when a dimension has the same name as its SDS
        call :test dumpsds-13.out dumpsds sds1_dim1_samename.hdf

        rem Test 14 prints contents of file when a dimension has the same name as 
        rem that of another SDS
        call :test dumpsds-14.out dumpsds sds2_dim1_samename.hdf
        
        rem Test 15 prints headers of all data sets with various compression method to
        rem test displaying compression information

        if not "%use_comp_szip%"=="yes" (
            call :test dumpsds-15.out dumpsds sds_compressed.hdf
        ) else (
            call :test dumpsds-15szip.out dumpsds sds_compressed.hdf
        )
    )
    
    rem Test command dumprig
    set testCmd=dumprig
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        call :test dumprig-1.out dumprig tdf24.hdf
        call :test dumprig-2.out dumprig -i 1,2 tdf24.hdf
        rem '-i 3' is invalid
        call :test dumprig-3.out dumprig -i 1,3 tdf24.hdf
        call :test dumprig-4.out dumprig -m 24 tdf24.hdf
        call :test dumprig-5.out dumprig -r 3,4 tdf24.hdf
        call :test dumprig-6.out dumprig -r 3,4 -d tdf24.hdf
    )
    
    rem Test command dumpvd
    set testCmd=dumpvd
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        call :test dumpvd-1.out dumpvd tvset.hdf
        call :test dumpvd-2.out dumpvd -i 1,3,5 tvset.hdf
        call :test dumpvd-3.out dumpvd -r 1238,1239,1251,1252 tvset.hdf
        call :test dumpvd-4.out dumpvd -n "Multi-Order Vdata" tvset.hdf
        call :test dumpvd-5.out dumpvd -n "Mixed Vdata","Integer Vdata" tvset.hdf
        call :test dumpvd-6.out dumpvd -c "Test object","No class specified" tvset.hdf
        call :test dumpvd-7.out dumpvd -f B tvset.hdf
        call :test dumpvd-8.out dumpvd -f "STATION_NAME","FLOATS" tvset.hdf
        call :test dumpvd-9.out dumpvd -f "STATION_NAME","FLOATS" -d tvset.hdf
        call :test dumpvd-10.out dumpvd tvattr.hdf

        rem Tests 11 and 12 print out the vdatas of classes "SDSVar" and "CoordVar"
        rem to test the fix of bugzilla 624 (these are new classes used to distinguish
        rem between SDS and coordinate variables)
        call :test dumpvd-11.out dumpvd -c "SDSVar" sds1_dim1_samename.hdf
        call :test dumpvd-12.out dumpvd -c "CoordVar" sds1_dim1_samename.hdf
    )
    
    rem Test command dumpvg
    set testCmd=dumpvg
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        call :test dumpvg-1.out dumpvg tvset.hdf
        call :test dumpvg-2.out dumpvg -i 0,1 tvset.hdf
        call :test dumpvg-3.out dumpvg -r 3 tvset.hdf
        call :test dumpvg-4.out dumpvg -n "Simple Vgroup" tvset.hdf
        call :test dumpvg-5.out dumpvg -c "Test object" tvset.hdf
        call :test dumpvg-6.out dumpvg -i 1,3,5 tdata.hdf
        call :test dumpvg-7.out dumpvg -r 32,39 tdata.hdf
        call :test dumpvg-8.out dumpvg -n nsamp,tdata.hdf tdata.hdf
        call :test dumpvg-9.out dumpvg -c CDF0.0 tdata.hdf

        rem Added option -h to the following test; this option has always 
        rem failed; just fixed it - BMR 8/1/00
        call :test dumpvg-10.out dumpvg -h -c Dim0.0,Var0.0 tdata.hdf

        rem this following test is removed since option -d is removed
        rem call :test dumpvg-11.out dumpvg -c Dim0.0,Var0.0 -d tdata.hdf

        rem moved test rem12 up to rem11, consequently - BMR 7/25/00
        call :test dumpvg-11.out dumpvg tvattr.hdf

        rem Added these two tests for the new feature: vgroup has variable length 
        rem name - BMR 10/27/06
        rem Note that the dumpvg-13 test searches for an SDS also
        call :test dumpvg-12.out dumpvg VGlongname.hdf
        call :test dumpvg-13.out dumpvg -n "SD Vgroup - this vgroup has an sds as a member and it is actually meant to test long vgroup name" VGlongname.hdf

        rem Prints contents of file when a dimension has the same name as its SDS 
        call :test dumpvg-14.out dumpvg sds1_dim1_samename.hdf

        rem Prints contents of file when a dimension has the same name as that 
        rem of another SDS
        call :test dumpvg-15.out dumpvg sds2_dim1_samename.hdf
    )

    rem Test command dumpgr
    set testCmd=dumpgr
    set testName=Test command %testCmd%
    if "%except%"=="%testCmd%" (
        call :mesg 3 %testName% SKIPPED
    ) else if not "!only:%testCmd%=x!"=="x" (
        call :mesg 3 %testName% SKIPPED
    ) else (
        call :mesg 3 %testName%
        call :test dumpgr-1.out dumpgr grtdfui82.hdf
        call :test dumpgr-2.out dumpgr -i 0,1,3 grtdfui82.hdf
        call :test dumpgr-3.out dumpgr -i 0 grtdfui82.hdf
        call :test dumpgr-4.out dumpgr -n Image_array_5 grtdfui82.hdf
        call :test dumpgr-5.out dumpgr -r 6,2,3 grtdfui82.hdf
        call :test dumpgr-6.out dumpgr -r 6 -d  grtdfui82.hdf
        call :test dumpgr-7.out dumpgr -o %currentdir%\my.dat grtdfui82.hdf
        %rm% my.dat
        call :test dumpgr-8.out dumpgr -o %currentdir%\mybin.dat  -b grtdfui82.hdf
        %rm% mybin.dat
        call :test dumpgr-9.out dumpgr grtdfui83.hdf
        call :test dumpgr-10.out dumpgr grtdfui84.hdf
        call :test dumpgr-11.out dumpgr grtdfui162.hdf

        rem Test 12 shows that file attribute is printed
        call :test dumpgr-12.out dumpgr grtdfi322.hdf

        rem Tests 13, 14, and 15 test option -h, which was not included in any
        rem of the previous tests, and the new options -p and -pd, printing palette
        rem with or without palette information 
        call :test dumpgr-13.out dumpgr -p Image_with_Palette.hdf
        call :test dumpgr-14.out dumpgr -h Image_with_Palette.hdf
        call :test dumpgr-15.out dumpgr -r 2,4 -pd Image_with_Palette.hdf

        rem Test 16: to test new option -s, printing data as stream
        call :test dumpgr-16.out dumpgr -r 6 -d -s grtdfui82.hdf

        rem Test 17: to test new option -m ^(interlace mode = LINE^)
        call :test dumpgr-17.out dumpgr -r 6 -m 1 grtdfui82.hdf

        rem Test 18: to test new option -c ^(printing attribute data in clean format^)
        call :test dumpgr-18.out dumpgr -c grtdfi322.hdf

        rem Test 19: to test new options -g and -l ^(suppress all attribute data^)
        call :test dumpgr-19.out dumpgr -g -l grtdfi322.hdf
    )
    
    rem End of test
    goto finish



:end
    popd
    endlocal & exit /b %nerrors%
