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
rem  File Name: testhdiff.bat
rem  This batch file is used to test hdiff tool
rem  This script has the following options:
rem    testhdiff (release / debug)
rem             Test either the release or debug version of hdiff tool
rem  Created:       Scott Wegner, 6/11/08
rem  Last Updated:  Scott Wegner, 6/11/08

setlocal enabledelayedexpansion
pushd %~dp0

rem The tool name
set hdiff=hdiff
rem The path of the tool binary
set hdiff_bin=%CD%\%1\%hdiff%

rem We will need to pipe commands below to emulate cmp -s
rem CMP='cmp -s'
set diff=fc

set nerrors=0
set verbose=yes

rem The build (current) directory might be different than the source directory
set srcdir=%CD%

if not exist testfiles (
    mkdir testfiles
)

goto main



rem Print a message with formats according to message level
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
    

rem Report the result and exit
:finish
    if %nerrors% equ 0 (
        call :mesg 0 All hdiff tests passed
    ) else (
        call :mesg 0 hdiff tests failed: %nerrors%
    )
    goto end


rem Print a line-line message left justified in a field of 70 characters
rem beginning with the word "Testing".
rem
:testing
    set test_msg=Testing %dumper%
    for %%a in (%*) do (
        if %%a neq PASSED (
        if %%a neq *FAILED* (
        if %%a neq CREATED (
        if %%a neq -SKIP- (
            set test_msg=!test_msg! %%~nxa
        ) ) ) )
    )
    set test_msg=%test_msg%                                                                
    echo.%test_msg:~0,69% %1
    
    exit /b


rem Run a test and print PASS or *FAIL*.  If a test fails then increment
rem the `nerrors' global variable and (if $verbose is set) display the
rem difference between the actual output and the expected output. The
rem expected output is given as the first argument to this function and
rem the actual output file is calculated by replacing the `.ddl' with
rem `.out'.  The actual output is not removed if $HDF5_NOCLEANUP has a
rem non-zero value.
rem
:tooltest
    set expect=%srcdir%\testfiles\%1
    set actual=%CD%\testfiles\%~n1.out
    set actual_err=%CD%\testfiles\%~n1.err
    
    rem We define %params% here because Windows `shift` command doesn't affect
    rem the %* variable.  --SJW 8/22/07
    set params=
    for /f "tokens=2*" %%a in ("%*") do (
        if "%%b"=="" (
            set params=%%a
        ) else (
            set params=%%a %%b
        )
    )
    rem Make sure that params is defined, so we don't encounter errors later
    if not defined params (
        set params=
    )

    rem Run test.
    rem Tflops interprets $@as when no parameter is given (e.g., the
    rem case of missing file name).  Changed it to use $@ till Tflops fixes it.
    (
        echo.#############################
        rem Remove quotes here, because Linux 'echo' command strips them
        echo.Expected output for 'hdiff %params%'
        echo.#############################
        pushd %srcdir%\testfiles
        %hdiff_bin% %params%
        popd
    ) >%actual% 2>%actual_err%
    type %actual_err% >> %actual%


    rem Used only to create the output file
    if not exist %expect% (
        rem Create the expected file if it doesn't yet exist.
        call :testing CREATED %hdiff% %params%
        copy /y %actual% %expect% > nul
    ) else (
        fc %expect% %actual% > nul
        if !errorlevel! equ 0 (
            call :testing PASSED %hdiff% %params%
        ) else (
            call :testing *FAILED* %hdiff% %params%
            echo.    Expected result ^(%expect%^) differs from actual result ^(%actual%^)
            set /a nerrors=%nerrors%+1
            if "yes"=="%verbose%" fc %expect% %actual%
        )
    )

    rem Clean up output file
    if not defined hdf4_nocleanup (
        del /f %actual% %actual_err%
    )
    
    exit /b 0
    


:run
    rem help message
    call :tooltest hdiff_01.txt 

    rem Compare global attributes only
    call :tooltest hdiff_02.txt -g hdifftst1.hdf hdifftst2.hdf

    rem Compare SD local attributes only
    call :tooltest hdiff_03.txt -s hdifftst1.hdf hdifftst2.hdf

    rem Compare SD data only
    call :tooltest hdiff_04.txt -d hdifftst1.hdf hdifftst2.hdf

    rem Compare Vdata data only
    call :tooltest hdiff_05.txt -D hdifftst1.hdf hdifftst2.hdf

    rem Print statistics
    rem Skip on Windows because scientific format is printed in non-standard
    rem way. --SJW 6/11/08
    rem call :tooltest hdiff_06.txt -d -S hdifftst1.hdf hdifftst2.hdf
    call :testing -SKIP- hdiff_06.txt -d -S hdifftst1.hdf hdifftst2.hdf

    rem Compare SD data on variable(s)
    call :tooltest hdiff_07.txt -d -v dset1 hdifftst1.hdf hdifftst2.hdf

    rem Compare vdata on variable(s) 
    call :tooltest hdiff_08.txt -D -u vdata1 hdifftst1.hdf hdifftst2.hdf

    rem Print difference up to count number
    call :tooltest hdiff_09.txt -d -e 2 hdifftst1.hdf hdifftst2.hdf

    rem Print difference when it is greater than limit
    call :tooltest hdiff_10.txt -d -t 2 hdifftst1.hdf hdifftst2.hdf

    rem no options
    call :tooltest hdiff_11.txt hdifftst1.hdf hdifftst2.hdf

    rem percent (relative)
    call :tooltest hdiff_12.txt -d -p 0.05 -v dset3 hdifftst1.hdf hdifftst2.hdf

    rem hyperslab reading 
    call :tooltest hdiff_13.txt hdifftst3.hdf hdifftst4.hdf

    rem lone dim
    call :tooltest hdiff_14.txt hdifftst5.hdf hdifftst6.hdf

    rem group loop
    call :tooltest hdiff_15.txt -b hdifftst7.hdf hdifftst7.hdf
    
    exit /b 0

    

rem ##############################################################################
rem ###           T H E   T E S T S                                            ###
rem ##############################################################################
:main
    rem Print a beginning banner
    call :mesg 0 Running hdiff tests

    rem compare output
    call :run

    rem End of test, return exit code
    goto finish


:end
popd
endlocal & exit /b %nerrors%