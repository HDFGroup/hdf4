$! --------------------------------------------------------------------------
$! For making NCDUMP.EXE on VMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/define=(HDF,VMS,NO_SYS_XDR_INC)/nolist -
            /include=([-.libsrc],[-.xdr],[--.hdf.src], -
            [--.hdf.jpeg], [--.hdf.zlib]) 
$
$ ccc [-.UTIL]GETOPT.C
$ ccc DUMPLIB.C
$ ccc NCDUMP.C
$ ccc VARDATA.C
$
$ link/nodebug/exe=NCDUMP.exe -
    getopt.obj, -
    dumplib.obj, -
    ncdump.obj, -
    vardata.obj, -
    [--.lib]mfhdf/lib,[--.hdf.src]df/lib,[--.hdf.jpeg]libjpeg.olb/lib,  -
    [--.hdf.zlib]libz.olb/lib, -
    sys$input/opt
        sys$library:vaxcrtl/lib
