$! --------------------------------------------------------------------------
$! For making CDFTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$!
$! You must create the netcdf library, NETCDF.OLB, from the XDR directory,
$! [-.xdr], before executing this procedure.
$
$
$ macro :== macro/nolist
$ ccc := cc /opt/nodebug/include=([-.xdr])/nolist
$ librep := library/replace [-.-.-.LIB]NETCDF.OLB
$
$ ccc ARRAY.C
$ ccc ATTR.C
$ ccc CDF.C
$ ccc CDFTEST.C
$ ccc DIM.C
$ ccc ERROR.C
$ ccc FILE.C
$ ccc IARRAY.C
$ ccc PUTGET.C
$ ccc SHARRAY.C
$ ccc STRING.C
$ ccc VAR.C
$ macro HTONS.MAR
$ macro NTOHS.MAR
$
$ librep ARRAY, ATTR, CDF, DIM, FILE, IARRAY, ERROR, -
    PUTGET, SHARRAY, STRING, VAR, HTONS, NTOHS
$
$ link/nodebug/exec=CDFTEST.exe -
    cdftest.obj, -
    [-.-.-.lib]netcdf/library, -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
$
$ copy netcdf.h [-.-.-.include]
$
