$! --------------------------------------------------------------------------
$! For making NCDUMP.EXE on VMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/include=([-.libsrc],[--.hdf.include]) -
            /define=VMS/define=HDF/nolist
$
$ ccc [-.UTIL]GETOPT.C
$ ccc DUMPLIB.C
$ ccc NCDUMP.C
$ ccc VARDATA.C
$
$ link/nodebug/notraceback/exe=NCDUMP.exe -
    getopt.obj, -
    dumplib.obj, -
    ncdump.obj, -
    vardata.obj, -
    [---.lib]netcdf/library, [--.hdf.lib]df/library,  -
    sys$input/opt
        sys$share:vaxcrtl.exe/share
