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
$ macro :== macro/migration/nolist

$ librep := library/replace [--.LIB]mfhdf.OLB

$ ccc := cc /noopt/debug/include=([-.xdr],[--.hdf.src],[--.hdf.jpeg], -
 [--.hdf.zlib])/define=(NO_SYS_XDR_INC, swap, HDF, VMS)
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
$ ccc globdef.c
$ ccc IARRAY.C
$ ccc nssdc.c
$ ccc PUTGET.C
$ ccc PUTGETG.C
$ ccc SHARRAY.C
$ ccc STRING.C
$ ccc VAR.C
$ ccc XDRPOSIX.C
! $ ccc XDRSTDIO.C
$ ccc HDFSDS.C
$ ccc MFSD.C
$ ccc hdftest.c
!$ macro HTONS.MAR
!$ macro NTOHS.MAR
$
$ librep ARRAY, ATTR, CDF, DIM, FILE, globdef, IARRAY, ERROR, -
    nssdc, PUTGET, PUTGETG, SHARRAY, STRING, VAR, -
    HDFSDS, MFSD, XDRPOSIX
$ library/list=netcdf.list/name [--.lib]mfhdf.olb
$ link/nodebug/exec=CDFTEST.exe/syslib -
    cdftest.obj, -
    [--.lib]mfhdf/library, -
    [--.hdf.src]df/library, -
    [--.hdf.jpeg]libjpeg/library, -
    [--.hdf.zlib]libz/library,  -
    sys$input/opt 
 	sys$library:vaxcrtl/lib
$
$ create/dir [-.-.include]
$
$ copy netcdf.h [-.-.include]
$
$ link/nodebug/exec=hdftest.exe/syslib -
    hdftest.obj, -
    [--.lib]mfhdf/library, -
    [--.hdf.src]df/library, -
    [--.hdf.jpeg]libjpeg/library, -
    [--.hdf.zlib]libz/library, -
    sys$input/opt 
        sys$library:vaxcrtl/lib
$
