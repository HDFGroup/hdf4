$! --------------------------------------------------------------------------
$! For making FTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/nolist/define=(HDF, VMS, NO_SYS_XDR_INC)    -
         /include=([--.libsrc],[---.include],[---.hdf.src],[---.hdf.jpeg], -
           [---.hdf.zlib],[--.xdr])
$
$ ccc JACKETS.C
$ ccc MFSDF.C
$ fort MFSDFF.FOR
$ fort HDFTEST.FOR
$ fort FTEST.FOR
$
$ link/nodebug/notraceback/exec=HDFTEST.exe -
    hdftest.obj, mfsdf.obj, mfsdff.obj, -
    [---.lib]mfhdf.olb/lib, [---.hdf.src]df.olb/lib, -
    [---.hdf.jpeg]libjpeg/lib,  [---.hdf.zlib]libz.olb/lib, -
    sys$input/opt
        sys$library:vaxcrtl/lib

$ link/nodebug/notraceback/exec=FTEST.exe -
    ftest.obj, -
    jackets.obj, -
    [---.lib]mfhdf.olb/lib, [---.hdf.src]df.olb/lib,  -
    [---.hdf.jpeg]libjpeg/lib,  [---.hdf.zlib]libz.olb/lib, -
    sys$input/opt
	sys$library:vaxcrtl/lib
$
$ library/replace [---.lib]mfhDF.OLB JACKETS
$ library/replace [---.lib]mfhDF.OLB mfsdf
$ library/replace [---.lib]mfhDF.OLB mfsdff
$
$ copy netcdf.inc [---.include]
$ type sys$input
     Run ftest
$ run ftest
$ type sys$input
     Run hdftest
$ create/dir [.testdir]
$ run hdftest
$
$ type sys$input
     Clean up ...
$ delete *.obj;*
$ delete *.exe;*

