$! --------------------------------------------------------------------------
$! For making HDP.EXE on VMS.
$! --------------------------------------------------------------------------
$!
$
$ ccc := cc /opt/nodebug/define=(HDF,NO_SYS_XDR_INC)/nolist -
            /include=([-.libsrc],[-.xdr],[--.hdf.src], -
            [--.hdf.jpeg], [--.hdf.zlib]) 
$
$ ccc hdp.C
$ ccc hdp_dump.C
$ ccc hdp_list.C
$ ccc hdp_rig.C
$ ccc hdp_sds.C
$ ccc hdp_util.C
$ ccc hdp_vd.C
$ ccc hdp_vg.C
$ ccc show.C
$ link/nodebug/notraceback/exe=hdp.exe -
    hdp.obj, -
    hdp_dump.obj, -
    hdp_list.obj, -
    hdp_rig.obj, -
    hdp_sds.obj, -
    hdp_util.obj, -
    hdp_vd.obj, -
    hdp_vg.obj, -
    show.obj, -
    [--.lib]mfhdf/lib,[--.hdf.src]df/lib,[--.hdf.jpeg]libjpeg.olb/lib,  -
    [--.hdf.zlib]libz.olb/lib, -
    sys$input/opt
        sys$library:vaxcrtl/lib

