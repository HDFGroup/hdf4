$! --------------------------------------------------------------------------
$! For making NCTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/include=([-.-.hdf.include],[-.libsrc])/nolist  -
            /define=stdc_includes/define=swap/define=VMS/define=HDF

$
$ ccc ADD.C
$ ccc ATTTESTS.C
$ ccc CDFTESTS.C
$ ccc DIMTESTS.C
$ ccc DRIVER.C
$ ccc EMALLOC.C
$ ccc ERROR.C
$ ccc MISCTEST.C
$ ccc REC.C
$ ccc SLABS.C
$ ccc VAL.C
$ ccc VARDEF.C
$ ccc VARGET.C
$ ccc VARPUT.C
$ ccc VPUTGET.C
$ ccc VARGETG.C
$ ccc VARPUTG.C
$ ccc VPUTGETG.C
$ ccc VARTESTS.C
$
$ link/nodebug/notraceback/exe=NCTEST.exe -
    add.obj, -
    atttests.obj, -
    cdftests.obj, -
    dimtests.obj, -
    driver.obj, -
    emalloc.obj, -
    error.obj, -
    misctest.obj, -
    rec.obj, -
    slabs.obj, -
    val.obj, -
    vardef.obj, -
    varget.obj, -
    varput.obj, -
    vputget.obj, -
    vargetg.obj, -
    varputg.obj, -
    vputgetg.obj, -
    vartests.obj, -
    [-.-.-.lib]netcdf/library, [-.-.hdf.lib]df/library, -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
$
$ run nctest
$
