$! --------------------------------------------------------------------------
$! For making FTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/nolist/define=HDF    -
         /include=([--.libsrc],[---.hdf.include])
$
$ ccc JACKETS.C
$ ccc MFSDF.C
$ fort MFSDFF.FOR
$ fort HDFTEST.FOR
$ fort FTEST.FOR
$
$ link/nodebug/traceback/exec=HDFTEST.exe -
    hdftest.obj, mfsdf.obj, mfsdff.obj, -
    [----.lib]netcdf.olb/lib, [---.hdf.lib]df.olb/lib, -
    sys$input/opt
        sys$library:vaxcrtl.exe/share

$ link/nodebug/traceback/exec=FTEST.exe -
    ftest.obj, -
    jackets.obj, -
    [----.lib]netcdf.olb/lib, [---.hdf.lib]df.olb/lib,  -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
$
$ library/replace [----.lib]NETCDF.OLB JACKETS
$
$ copy netcdf.inc [----.include]
$
$ run hdftest
$ run ftest
$
