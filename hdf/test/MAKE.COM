!$!
!$!
!$! File to build the test files in the HDF test suite
!$!
!$!
!$! The following constants should probably be defined
!$!   only once in the main HDF MAKE.COM...
!$!
$ define/nolog hdf$lib [-.lib]df
$ define/nolog hdf$include [-.include]
$ define/nolog sys$clib sys$library:vaxcrtl
!$!
$ type sys$input
  **    Making testhdf **
$ cc/define=VMS/include=hdf$include   testhdf
$ type sys$input
  **    Making H level test programs **
$!
$ cc/define=VMS/include=hdf$include   hfile
$ cc/define=VMS/include=hdf$include   hfile1
$ cc/define=VMS/include=hdf$include   hblocks
$ cc/define=VMS/include=hdf$include   hextelt
$ cc/define=VMS/include=hdf$include   vers
$ type sys$input
  **  Making C interface test programs **
$!
$ cc/define=VMS/include=hdf$include   rig
$ cc/define=VMS/include=hdf$include   sdmms
$ cc/define=VMS/include=hdf$include   sdnmms
$ cc/define=VMS/include=hdf$include   an
$ cc/define=VMS/include=hdf$include   anfile
$ cc/define=VMS/include=hdf$include   slab
$ cc/define=VMS/include=hdf$include   sdstr
$ cc/define=VMS/include=hdf$include   litend
$ cc/define=VMS/include=hdf$include   tvset
$ link testhdf,rig,sdmms,sdnmms,an,anfile,sdstr,slab,litend, -
  hfile, hfile1,hblocks,hextelt,vers,tvset, -
  hdf$lib/lib,sys$clib/lib
$ type sys$input 
  **  Making Fortran interface test programs **
$!
$ cc/define=VMS/include=hdf$include   fortest
$ fortran tr8F.f
$ fortran t24F.f
$ fortran tpF.f
$ fortran tsdmmsF.f
$ fortran tsdnmmsF.f
$ fortran tsdntF.f
$ fortran tsdnntF.f
$ fortran tanF.f
$ fortran tanfileF.f
$ fortran tstubsF.f
$ fortran tsdstrF.f
$ fortran slabwf.f
$ fortran slab1wf.f
$ fortran slab2wf.f
$ fortran slab3wf.f
$ fortran slab4wf.f
$ fortran egfhi.f
$ link fortest, hdf$lib/lib, sys$clib/lib
$ link tr8f, hdf$lib/lib, sys$clib/lib
$ link t24f, hdf$lib/lib, sys$clib/lib
$ link tpf, hdf$lib/lib, sys$clib/lib
$ link tsdmmsf, hdf$lib/lib, sys$clib/lib
$ link tsdnmmsf, hdf$lib/lib, sys$clib/lib
$ link tsdntf, hdf$lib/lib, sys$clib/lib
$ link tsdnntf, hdf$lib/lib, sys$clib/lib
$ link tanf, hdf$lib/lib, sys$clib/lib
$ link tanfilef, hdf$lib/lib, sys$clib/lib
$ link tstubsf, hdf$lib/lib, sys$clib/lib
$ link tsdstrf, hdf$lib/lib, sys$clib/lib
$ link slabwf, hdf$lib/lib, sys$clib/lib
$ link slab1wf, hdf$lib/lib, sys$clib/lib
$ link slab2wf, hdf$lib/lib, sys$clib/lib
$ link slab3wf, hdf$lib/lib, sys$clib/lib
$ link slab4wf, hdf$lib/lib, sys$clib/lib
$ link egfhi, hdf$lib/lib, sys$clib/lib
$ delete *.obj;*
$!


