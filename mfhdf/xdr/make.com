$! --------------------------------------------------------------------------
$! For making XDRTEST.EXE on VMS if you don't have MMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ macro :== macro/nolist/migration
$ ccc := cc /opt/nodebug/nolist/define=(HDF,VMS, NO_SYS_XDR_INC)  -
            /include=([],[--.hdf.src], [--.hdf.jpeg],[--.hdf.zlib])
$
$ ccc XDR.C
$ ccc XDRTEST.C
$ ccc XDRARRAY.C
$ ccc XDRFLOAT.C
$ ccc XDRSTDIO.C
$! macro HTONL.MAR
$! macro NTOHL.MAR
$
$ library/create [-.-.LIB]MFHDF.OLB
$! library/replace [-.-.LIB]MFHDF.OLB XDR, XDRFLOAT, XDRSTDIO, HTONL, NTOHL
$ library/replace [-.-.LIB]MFHDF.OLB XDR, XDRFLOAT, XDRSTDIO
$
$ link/nodebug/exec=XDRTEST.exe -
    xdrtest.obj, -
    xdrarray.obj, -
    [-.-.lib]mfhdf.olb/lib, -
    sys$input/opt
       sys$library:vaxcrtl/lib
$ delete *.obj;*

