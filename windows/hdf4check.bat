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
rem  File Name: hdf4check.bat
rem  This batch file is used to test HDF4 Libraries and Tools.
rem  This script has the following options:
rem    hdf4check [enablefortan or nodebug]
rem             enablefortran   Also test HDF4 Fortran libraries [default C only]
rem             nodebug         can be added to not test debug versions

setlocal enabledelayedexpansion
pushd %~dp0
set chkdebug=debug
set chkrelease=release

set nerrors=0

if "%1"=="/?" (
    goto help
) else if "%1"=="enablefortran" (
    set build_fortran_conditional=true
) else if /i "%1" equ "nodebug" (
    set chkdebug=
) else if not "%1"=="" (
    call :help
    exit /b 1
)
if /i "%2" equ "nodebug" (
    set chkdebug=
)
    
goto main


rem Print a help message, then exist
:help
    echo.hdf4check [enablefortan or nodebug]
    echo.         enablefortran   Also test HDF4 Fortran libraries [default C only]
	echo.         nodebug         can be added to not test debug versions
    exit /b 0


    
rem Run the specified test from the directory it needs to run in.  Print a
rem simple header, and a failure message and incrememnt nerrorsif the test fails.
rem Expects the following parameters:
rem     %1 - Path to run test from (use quotes if path has spaces)
rem     %2 - Test to run, with relative path from above
rem     %3 to %5 - Extra parameters
:exec_test

    rem Write an informative header
    echo.
    echo.************************************
    echo.  Testing %2 %3 %4 %5
    echo.************************************
    
    pushd %1
    rem Sanity check-- make sure the test exists
    if not exist %2 (
        echo.Error: Cannot find test script: %CD%\%2
        set /a nerrors=%nerrors%+1
        popd
        exit /b 1
    )
    
    rem Run the test and check for errors
    call %2 %3 %4 %5
    
    if !errorlevel! neq 0 (
        echo. %~n2 test FAILED.
        echo.------------------------------------------------
        echo.    To, run the test by hand,
        echo.    Open a command prompt to: %CD%
        echo.    Then run the command: %2 %3 %4 %5
        echo.
        echo.    Other setup steps may be required,
        echo.    Find the test within hdf4check.bat :run_tests function for details.
        echo.------------------------------------------------
        set /a nerrors=%nerrors%+1
        popd
        exit /b 1
    )
    
    rem If we get here, the test passed
    popd
    exit /b 0
    


rem Do any required setup for each test, call exec_test to run it, and then
rem cleanup.  Any modifications to tests should happen here
rem Expects the following parameters:
rem     %1 - release or debug, the configuration to test
:run_tests

    echo.*****************************************************************************
    echo.                    HDF4 Library and Utilities Tests -- %1
    echo.*****************************************************************************
    
    rem For each test added here, first do some setup, then add it using the
    rem exec_test syntax: the directory it should run from, the test name itself,
    rem and then any parameters.  Afterwards, do any cleanup necessary, but
    rem cleanup carefully, because some tests may fail.
    
    rem *** STATIC TESTS ***
    
    rem testhdf
    mkdir hdf\test\%1\testdir 2> nul
    mkdir hdf\test\%1\test_files 2> nul
    copy /y hdf\test\test_files\*.dat hdf\test\%1\test_files > nul 2>&1
    call :exec_test hdf\test\%1 testhdf
    del /f /q hdf\test\%1\test_files\*.dat hdf\test\%1\test_files\*.hdf 2> nul
	del /f /q hdf\test\%1\*.hdf 2> nul
    rmdir /s /q hdf\test\%1\test_files 2> nul
    rmdir /s /q hdf\test\%1\testdir 2> nul
    
    rem Currently, we don't build buffer test on Windows
    rem call :exec_test hdf\test buffer
    
    if defined build_fortran_conditional (
        rem Currently, we don't build fortest test on Windows
        rem call :exec_test hdf\test fortest
        
        rem fortestf
        mkdir hdf\test\%1\testdir 2> nul
        mkdir hdf\test\%1\test_files 2> nul
        copy /y hdf\test\test_files\*.dat hdf\test\%1\test_files > nul 2>&1
        copy /y hdf\test\fort_ps\fortest.arg hdf\test\%1 >nul 2>&1
        call :exec_test hdf\test\%1 fortestf
        del /f /q hdf\test\%1\test_files\*.dat hdf\test\%1\fortest.arg 2> nul
		del /f /q hdf\test\%1\*.hdf 2> nul
        rmdir /s /q hdf\test\%1\test_files 2> nul
        rmdir /s /q hdf\test\%1\testdir 2> nul
    )
    
    rem testutil.bat
    call :exec_test hdf\util testutil.bat %1
    
    if defined build_fortran_conditional (
        rem ftest
        copy mfhdf\fortran\test_nc.nc mfhdf\fortran\%1 > nul 2>&1
        call :exec_test mfhdf\fortran\%1 ftest
        del /f /q mfhdf\fortran\%1\test.nc mfhdf\fortran\%1\copy.nc mfhdf\fortran\%1\test_nc.nc 2>nul
        
        rem tszip
        mkdir mfhdf\fortran\%1\testdir 2> nul
        call :exec_test mfhdf\fortran\%1 tszip
        del /f /q mfhdf\fortran\%1\comp_szip.hdf 2>nul
        rmdir /s /q mfhdf\fortran\%1\testdir 2> nul
    
        rem hdftestf
        mkdir mfhdf\fortran\%1\testdir 2> nul
        call :exec_test mfhdf\fortran\%1 hdftestf
        del /f /q mfhdf\fortran\%1\*.hdf 2> nul
        rmdir /s /q mfhdf\fortran\%1\testdir 2> nul

        rem Currently, we don't build tszip test on Windows
        rem call :exec_test mfhdf\fortran tszip
        
        rem Currently, we don't build ftest test on Windows
        rem call :exec_test mfhdf\fortran ftest
    )
    
    rem testinterface.bat
    call :exec_test mfhdf\test testinterface.bat %1
    
    rem testncdump.bat
    call :exec_test mfhdf\ncdump testncdump.bat %1
    
    rem testncgen.bat
    call :exec_test mfhdf\ncgen testncgen.bat %1
    
    rem testhdp.bat
    call :exec_test mfhdf\dumper testhdp.bat %1
    
    rem testutil.bat
    call :exec_test mfhdf\hdfimport testutil.bat %1
    
    rem testhdiff.bat
    call :exec_test mfhdf\hdiff testhdiff.bat %1
    
    rem Currently, we don't build hrepack_check on Windows
    rem call :exec_test mfhdf\hrepack\%1 hrepack_check
    
    rem hrepacktst
    copy /y mfhdf\hrepack\image*.txt mfhdf\hrepack\%1 > nul 2>&1
    call :exec_test mfhdf\hrepack\%1 hrepacktst
    del /f /q mfhdf\hrepack\%1\image*.txt mfhdf\hrepack\%1\*.hdf 2>&1
    
    rem nctest
    copy /y mfhdf\nctest\test_unlim.nc mfhdf\nctest\%1 > nul 2>&1
    call :exec_test mfhdf\nctest\%1 nctest
    del /f /q mfhdf\nctest\%1\*.nc 2>nul
        
    rem xdrtest
    call :exec_test mfhdf\xdr\%1 xdrtest
    del /f /q mfhdf\xdr\%1\test.xdr 2>nul
    
    
    rem *** DLL TESTS ***
    
    
    rem dlltesthdf
    mkdir %CD%\hdf\test\%1\testdir 2> nul
    mkdir %CD%\hdf\test\%1\test_files 2> nul
    copy /y %CD%\hdf\test\test_files\*.dat %CD%\hdf\test\%1\test_files > nul 2>&1
    call :exec_test %CD%\hdf\test\%1 dlltesthdf
    del /f /q %CD%\hdf\test\%1\test_files\*.dat %CD%\hdf\test\%1\test_files\*.hdf 2> nul
	del /f /q %CD%\hdf\test\%1\*.hdf 2> nul
    rmdir /s /q %CD%\hdf\test\%1\test_files 2> nul
    rmdir /s /q %CD%\hdf\test\%1\testdir 2> nul
    
    if defined build_fortran_conditional (
        rem dllfortestf
        mkdir hdf\test\%1\testdir 2> nul
        mkdir hdf\test\%1\test_files 2> nul
        copy /y hdf\test\test_files\*.dat hdf\test\%1\test_files > nul 2>&1
        copy /y hdf\test\fort_ps\fortest.arg hdf\test\%1 >nul 2>&1
        call :exec_test hdf\test\%1 dllfortestf
        del /f /q hdf\test\%1\test_files\*.dat hdf\test\%1\fortest.arg 2> nul
		del /f /q hdf\test\%1\*.hdf 2> nul
        rmdir /s /q hdf\test\%1\test_files 2> nul
        rmdir /s /q hdf\test\%1\testdir 2> nul
    )
    
    if defined build_fortran_conditional (
        rem dllhdftestf
        mkdir mfhdf\fortran\%1\testdir 2> nul
        call :exec_test mfhdf\fortran\%1 dllhdftestf
        del /f /q mfhdf\fortran\%1\*.hdf 2> nul
        rmdir /s /q mfhdf\fortran\%1\testdir 2> nul
    )
    
    rem testinterface.bat
    call :exec_test mfhdf\test testinterface.bat %1 dll
    
    rem dllnctest
    copy /y mfhdf\nctest\test_unlim.nc mfhdf\nctest\%1 > nul 2>&1
    call :exec_test mfhdf\nctest\%1 dllnctest
    del /f /q mfhdf\nctest\%1\*.nc 2> nul
    
    rem dllxdrtest
    call :exec_test mfhdf\xdr\%1 dllxdrtest
    del /f /q mfhdf\xdr\%1\test.xdr 2>nul
    
    exit /b 0


    
rem This is where we we call each of our functions for setup and testing.  Tests
rem are setup in the run_tests function, and actually run in the exec_test
rem function
:main
    
    rem Put built DLLs in the system folder for testing
    call install_dll.BAT
    if defined build_fortran_conditional (
        call install_fortrandll.bat
    )
    
    
    rem Call run_tests to actually spawn all of our tests
    for %%a in (%chkdebug% %chkrelease%) do (
        call :run_tests %%a
    )
    
    if %nerrors% equ 0 (
        echo.HDF4 Tests passed for all configurations!
    )
    
    popd
    endlocal & exit /b %nerrors%
    
