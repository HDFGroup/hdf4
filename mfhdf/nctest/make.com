$! --------------------------------------------------------------------------
$! For making NCTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /noopt/debug/include=([--.hdf.src],[--.hdf.jpeg], -
          [--.hdf.zlib],[-.libsrc])/nolist  -
            /define=(NO_SYS_XDR_INC,swap,HDF.VMS)

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
$ link/debug/traceback/exe=NCTEST.exe -
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
    [--.lib]mfhdf/lib, [--.hdf.src]df/lib,[--.hdf.jpeg]libjpeg.olb/lib, -
    [--.hdf.zlib]libz.olb/lib, -
    sys$input/opt
	sys$library:vaxcrtl/lib
$
$ run nctest
$
