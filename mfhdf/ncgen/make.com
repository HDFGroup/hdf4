$! --------------------------------------------------------------------------
$! For making NCGEN.EXE on VMS.
$! --------------------------------------------------------------------------
$!
$! $Id$
$
$ ccc := cc /opt/nodebug/include=([---.include])/nolist
$
$ copy vmstab.c ncgentab.c
$ copy vmstab.h ncgentab.h
$ copy vms_yy.c ncgenyy.c
$
$ ccc GETFILL.C
$ ccc CLOSE.C
$ ccc DERROR.C
$ ccc EMALLOC.C
$ ccc ESCAPES.C
$ ccc GENERATE.C
$ ccc [-.UTIL]GETOPT.C
$ ccc INIT.C
$ ccc LOAD.C
$ ccc MAIN.C
$ ccc NCGENTAB.C
$
$ link/nodebug/notraceback/exe=NCGEN.exe -
    getfill.obj, -
    close.obj, -
    derror.obj, -
    emalloc.obj, -
    escapes.obj, -
    generate.obj, -
    getopt.obj, -
    init.obj, -
    load.obj, -
    main.obj, -
    ncgentab.obj, -
    [---.lib]netcdf/library, -
    sys$input/opt
	sys$library:vaxcrtl.exe/share
