$! --------------------------------------------------------------------------
$! For making XDRTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ macro :== macro/nolist
$ ccc := cc /opt/nodebug/nolist/include=[]
$
$ ccc XDR.C
$ ccc XDRTEST.C
$ ccc XDRARRAY.C
$ ccc XDRFLOAT.C
$ ccc XDRSTDIO.C
$ macro HTONL.MAR
$ macro NTOHL.MAR
$
$ library/create [-.-.-.LIB]NETCDF.OLB
$ library/replace [-.-.-.LIB]NETCDF.OLB XDR, XDRFLOAT, XDRSTDIO, HTONL, NTOHL
$
$ link/nodebug/notraceback/exec=XDRTEST.exe -
    xdrtest.obj, -
    xdrarray.obj, -
    [-.-.-.lib]netcdf.olb/lib, -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
