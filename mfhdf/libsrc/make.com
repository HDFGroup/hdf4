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
$ ccc := cc /opt/nodebug/include=([-.xdr])/nolist/define=stdc_includes
$ librep := library/replace [-.-.LIB]NETCDF.OLB
$
$ define rpc sys$disk:[-.xdr]
$ define sys sys$library
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
$ ccc PUTGETG.C
$ ccc SHARRAY.C
$ ccc STRING.C
$ ccc VAR.C
$ ccc XDRPOSIX.C
$ macro HTONS.MAR
$ macro NTOHS.MAR
$
$ librep ARRAY, ATTR, CDF, DIM, FILE, IARRAY, ERROR, -
    PUTGET, SPUTGETG, HARRAY, STRING, VAR, HTONS, NTOHS
$
$ link/nodebug/exec=CDFTEST.exe -
    cdftest.obj, -
    [-.-.lib]netcdf/library, -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
$
$ create/dir [-.-.include]
$
$ copy netcdf.h [-.-.-.include]
$
