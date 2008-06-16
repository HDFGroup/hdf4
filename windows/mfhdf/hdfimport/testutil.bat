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
rem  This batch file is used to test hdfimport utility
rem  This script has the following options:
rem    testutil (release / debug)
rem             Test either the release or debug version of hdfimport utility
rem  Created:       Scott Wegner, 6/11/08
rem  Last Updated:  Scott Wegner, 6/11/08

setlocal enabledelayedexpansion
pushd %~dp0

set srcdir=%CD%

rem Other platforms use $machinetype to determinte whether 32-bit features
rem are available.  This is unneccessary on Windows.  --SJW 6/11/08
rem machinetype="$1"

rem # Check if target machine supports 32 bits datatype.
rem # "true" is TRUE; "" means FALSE.
rem case $machinetype in
rem     *unicos*) has32="";;
rem     *) has32="true";;
rem esac
set has32=true

rem initialize errors variable
set errors=0
set haserr=0

rem setup hdfed command which is used often
set hdfed=%CD%\..\..\hdf\util\%1\hdfed
set hdfls=%CD%\..\..\hdf\util\%1\hdfls
rem use -batch mode for no prompt
set hdfedcmd=%hdfed% -batch
rem filter out the library version
rem Windows doesn't have SED, so we will have to use findstr instead
rem SED="sed -e /library/,/String/d"
set testcmd=%CD%\%1\hdfimport

echo.
echo.==============================
echo.HDFIMPORT tests started
echo.==============================

if not exist %CD%\%1\hdfimport (
    echo.** hdfimport not available ***
    set haserr=1
) else if not exist %CD%\%1\hdfimporttest (
    echo.** hdfimporttest not available ***
    set haserr=1
) else (

    rem use [!S] to prevent SDSfloat2.hdf and SDSfloat3.hdf from being deleted;
    rem please replace with better solution, one is if available. - BMR (2006/9/23)
    del /f /q ctxt* cb* SDSfloat2.out SDSfloat3.out hdfls.tmp5 hdfed.tmp6 hdffiles.tmp 2> nul
    %CD%\%1\hdfimporttest
    echo.Testing for 32-bit floating point ASCII ^(2D data^)
    %testcmd% ctxtr2 -o ctxtr2.hdf
    echo.Testing for 32-bit floating point ASCII ^(3D data^)
    %testcmd% ctxtr3 -o ctxtr3.hdf
    echo.Testing for 32-bit integer binary ^(2D data^)
    %testcmd% cb32i2 -o cb32i2.hdf
    echo.Testing for 32-bit integer  binary ^(3D data^)
    %testcmd% cb32i3 -o cb32i3.hdf
    echo.Testing for 16-bit integer binary ^(2D data^)
    %testcmd% cb16i2 -o cb16i2.hdf
    echo.Testing for 16-bit integer ^(3D data^)
    %testcmd% cb16i3 -o cb16i3.hdf
    echo.Testing for 32-bit floating point binary ^(2D data^)
    %testcmd% cb32r2 -o cb32r2.hdf
    echo.Testing for 32-bit floating point binary ^(3D data^)
    %testcmd% cb32r3 -o cb32r3.hdf
    echo.Testing for 64-bit floating point binary ^(2D data^) - Default Behaviour ^(Conversion to 32 bit FP SDS^)
    %testcmd% cb64r2 -o cb64r2.hdf
    echo.Testing for 64-bit floating point binary ^(3D data^) - Default Behaviour ^(Conversion to 32-bit FP SDS^)
    %testcmd% cb64r3 -o cb64r3.hdf
    echo.Testing for 64-bit floating point binary ^(2D data^) - Conversion to 64-bit FP SDS
    %testcmd% cb64r2 -n -o cb64r2-n.hdf
    echo.Testing for 64-bit floating point binary ^(3D data^) - Conversion to 64-bit FP SDS
    %testcmd% cb64r3 -n -o cb64r3-n.hdf
    echo.Testing for raster options
    %testcmd% ctxtr2 -o ctxtr2_ris.hdf -raster -e 50 50
    %testcmd% cb64r2 -o cb64r2_ris.hdf -raster -i 50 50 -f
    
    rem test with hdf files
    echo.Testing for reading from hdf files
    %testcmd% %srcdir%\SDSfloat2.hdf -o SDSfloat2.out
    %testcmd% %srcdir%\SDSfloat3.hdf -o SDSfloat3.out

    (%hdfls% -l ctxtr2.hdf | findstr /v "library String" ) > hdfls.tmp5 2>&1
    (%hdfls% -l ctxtr3.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb32i2.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb32i3.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb16i2.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb16i3.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb32r2.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb32r3.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb64r2.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb64r3.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb64r2-n.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb64r3-n.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l ctxtr2_ris.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1
    (%hdfls% -l cb64r2_ris.hdf | findstr /v "library String" ) >> hdfls.tmp5 2>&1

    rem run hdfls on SDSfloat*.out, and remove the library version from the
    rem output for later checking agains original output - BMR (2006/9/7)
    (%hdfls% SDSfloat2.out | findstr /v "library String" ) >> hdffiles.tmp 2>&1
    (%hdfls% SDSfloat3.out | findstr /v "library String" ) >> hdffiles.tmp 2>&1

    fc  hdfls.tmp5 %srcdir%\hdfimport.out1 > nul
    if !errorlevel! neq 0 (
        fc  hdfls.tmp5 %srcdir%\hdfimport.out1
        set errors=1
    )
    %hdfedcmd% < %srcdir%\hdfimport.input1 > hdfed.tmp6 2>&1
    rem On Windows, don't actually do comparison, because Windows prints
    rem numbers in scientific notation differently.  There are 3 exponent digits
    rem instead of two. --SJW 6/11/08
    rem fc  hdfed.tmp6 %srcdir%\hdfimport.out2
    if !errorlevel! neq 0 (
        set errors=1
    )

    rem hdfimport.out3 contains output that hdfls previously generated
    rem from SDSfloat*.out with the library version removed - BMR ^(2006/9/7^)
    fc  hdffiles.tmp %srcdir%\hdfimport.out3 > nul
    if !errorlevel! neq 0 (
        fc  hdffiles.tmp %srcdir%\hdfimport.out3
        set errors=1
    )
    
    rem use [!S] to prevent SDSfloat2.hdf and SDSfloat3.hdf from being deleted;
    rem please replace with a better solution, if one is available. - BMR (2006/9/23)
    del /f /q ctxt* cb* SDSfloat2.out SDSfloat3.out hdfls.tmp5 hdfed.tmp6 hdffiles.tmp
    
rem endif,  exist hdfimport and hdfimport test
)

if %errors% equ 1 (
    set haserr=1
    echo. ********* NOTE ***************
    echo. hdfimport might have failed ***
    echo. The above errors could be formatting
    echo. problems which can be ignored
    echo. please run the following by hand to verify
    echo.
    echo. del /f /q ctxt* cb* hdfls.tmp5 hdfed.tmp6 hdffiles.tmp
    echo. %CD%\%1\hdfimporttest
    echo. %testcmd% ctxtr2 -o ctxtr2.hdf
    echo. %testcmd% ctxtr3 -o ctxtr3.hdf
    echo. %testcmd% cb32i2 -o cb32i2.hdf
    echo. %testcmd% cb32i3 -o cb32i3.hdf
    echo. %testcmd% cb16i2 -o cb16i2.hdf
    echo. %testcmd% cb16i3 -o cb16i3.hdf
    echo. %testcmd% cb32r2 -o cb32r2.hdf
    echo. %testcmd% cb32r3 -o cb32r3.hdf
    echo. %testcmd% cb64r2 -o cb64r2.hdf
    echo. %testcmd% cb64r3 -o cb64r3.hdf
    echo. %testcmd% cb64r2 -n -o cb64r2-n.hdf
    echo. %testcmd% cb64r3 -n -o cb64r3-n.hdf
    echo. %testcmd% ctxtr2 -o ctxtr2_ris.hdf -raster -e 50 50
    echo. %testcmd% cb64r2 -o cb64r2_ris.hdf -raster -i 50 50 -f
    echo. %testcmd% %srcdir%\SDSfloat2.hdf -o SDSfloat2.out
    echo. %testcmd% %srcdir%\SDSfloat3.hdf -o SDSfloat3.out
    echo. ^(%hdfls% -l ctxtr2.hdf ^| findstr /v "library String" ^) ^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l ctxtr3.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb32i2.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb32i3.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb16i2.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb16i3.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb32r2.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb32r3.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb64r2.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb64r3.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb64r2-n.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb64r3-n.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l ctxtr2_ris.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% -l cb64r2_ris.hdf ^| findstr /v "library String" ^) ^>^> hdfls.tmp5 2^>^&1
    echo. ^(%hdfls% SDSfloat2.out ^| findstr /v "library String" ^) ^>^> hdffiles.tmp 2^>^&1
    echo. ^(%hdfls% SDSfloat3.out ^| findstr /v "library String" ^) ^>^> hdffiles.tmp 2^>^&1
    echo. fc  hdfls.tmp5 %srcdir%\hdfimport.out1
    echo. %hdfedcmd% < %srcdir%\hdfimport.input1 ^> hdfed.tmp6 2^>^&1
    echo. fc  hdffiles.tmp %srcdir%\hdfimport.out3
    echo. ******* END NOTE *************
    echo.
    set errors=0
)

rem del /f /q 

rem
rem Check errors result
if %haserr% equ 0 (
    echo.=================================
    echo.HDFIMPORT Utilities tests passed.
    echo.=================================
) else (
    echo.*********************************************
    echo.HDFIMPORT Utilities tests encountered errors
    echo.*********************************************
)

echo.
popd
endlocal & exit /b %haserr%