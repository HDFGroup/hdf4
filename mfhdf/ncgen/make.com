$! --------------------------------------------------------------------------
$! For making NCGEN.EXE on VMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/include=([--.include],[--.hdf.src], -
     [--.hdf.jpeg],[--.hdf.zlib], sys$library)/nolist    -
     /define=(HDF,VMS,NO_SYS_XDR_INC)
$
$ copy vmstab.c ncgentab.c
$ copy vmstab.h ncgentab.h
$ copy vms_yy.c ncgenyy.c
$
$ ccc MAIN.C
$ ccc GENERATE.C
$ ccc LOAD.C
$ ccc NCGENTAB.C
$ ccc ESCAPES.C
$ ccc GETFILL.C
$ ccc INIT.C
$ ccc CLOSE.C
$ ccc GENLIB.C
$ ccc [-.UTIL]GETOPT.C
$
$ link/nodebug/notraceback/exe=NCGEN.exe -
    getfill.obj, -
    close.obj, -
    genlib.obj, -
    escapes.obj, -
    generate.obj, -
    getopt.obj, -
    init.obj, -
    load.obj, -
    main.obj, -
    ncgentab.obj, -
    [--.lib]mfhdf/lib, [--.hdf.src]df/lib,  -
    [--.hdf.jpeg]libjpeg.olb/lib, -
    [--.hdf.zlib]libz.olb/lib, -
    sys$input/opt
	sys$library:vaxcrtl/lib
