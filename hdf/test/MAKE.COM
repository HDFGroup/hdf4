$! ****************************************************************************
$! * NCSA HDF                                                                 *
$! * Software Development Group                                               *
$! * National Center for Supercomputing Applications                          *
$! * University of Illinois at Urbana-Champaign                               *
$! * 605 E. Springfield, Champaign IL 61820                                   *
$! *                                                                          *
$! * For conditions of distribution and use, see the accompanying             *
$! * hdf/COPYING file.                                                        *
$! *                                                                          *
$! ***************************************************************************
$!
$! Id: MAKE.COM,v 1.9 1996/01/18 05:09:44 sxu Exp $
$!
$!
$! File to build the test files in the HDF test suite
$!
$!
$! The following constants should probably be defined
$!   only once in the main HDF MAKE.COM...
$!
$ hdflib= "[-.src]df/lib,[-.jpeg]libjpeg/lib,[-.zlib]libz/lib"
$! define/nolog hdf$include ([-.src],[-.jpeg],[-.zlib])
$ define/nolog sys$clib sys$library:vaxcrtl
$ ccopt= "/include=([-.src],[-.jpeg],[-.zlib])/opt/nodebug/define=VMS"
$! ccopt= "/debug/noopt/include=([-.src],[-.jpeg],[-.zlib])/define=VMS"
$! foropt= "/debug/noopt"
$ foropt= "/opt/nodebug"
$!
$ type sys$input
  **    Making testhdf **
$ cc 'ccopt   testhdf
$ type sys$input
  **    Making H level test programs **
$!
$ cc 'ccopt   file
$ cc 'ccopt   file1
$ cc 'ccopt   blocks
$ cc 'ccopt   extelt
$ cc 'ccopt   vers
$ type sys$input
  **  Making C interface test programs **
$!
$ cc 'ccopt   rig
$ cc 'ccopt   sdmms
$ cc 'ccopt   sdnmms
$ cc 'ccopt   an
$ cc 'ccopt   anfile
$ cc 'ccopt   slab
$ cc 'ccopt   sdstr
$ cc 'ccopt   litend
$ cc 'ccopt   tvset
$ cc 'ccopt   conv
$ cc 'ccopt   tree
$ cc 'ccopt   bitio
$ cc 'ccopt   comp
$ cc 'ccopt   man
$ cc 'ccopt   mgr.c
$ cc 'ccopt   nbit
$ cc 'ccopt   tbv
$ cc 'ccopt   gentest
$ cc 'ccopt   tvsfpack
$ link/nodebug testhdf,rig,sdmms,sdnmms,an,anfile,sdstr,slab,litend, -
  file,file1,blocks,extelt,vers,tvset,conv,tree,bitio,comp,man,mgr.obj, -
  nbit, tbv, tvsfpack 'hdflib
$ link gentest, 'hdflib, sys$library:vaxcrtl/lib
$ type sys$input 
  **  Making Fortran interface test programs **
$!
$ cc 'ccopt   fortest.c
$ cc 'ccopt   forsupf.c
$ fortran 'foropt fortestF.f
$ fortran 'foropt tr8F.f
$ fortran 'foropt t24F.f
$ fortran 'foropt tpF.f
$ fortran 'foropt tsdmmsF.f
$ fortran 'foropt tsdnmmsF.f
$ fortran 'foropt tsdntF.f
$ fortran 'foropt tsdnntF.f
$ fortran 'foropt tanF.f
$ fortran 'foropt tanfileF.f
$ fortran 'foropt tstubsF.f
$ fortran 'foropt tsdstrF.f
$ fortran 'foropt slabwf.f
$! fortran slab1wf.f
$! fortran slab2wf.f
$! fortran slab3wf.f
$! fortran slab4wf.f
$ fortran egfhi.f
$ fortran 'foropt forsupff.f
$ fortran 'foropt manf.f
$ fortran 'foropt mgrf.f
$ fortran 'foropt tvsetf.f
$!
$ link/nodebug fortest, 'hdflib, sys$clib/lib
$ link tr8f, forsupff, 'hdflib, sys$clib/lib
$ link t24f,forsupff,  'hdflib, sys$clib/lib
$ link tpf, forsupff, 'hdflib, sys$clib/lib
$ link tsdmmsf,forsupff, 'hdflib, sys$clib/lib
$ link tsdnmmsf,forsupff, 'hdflib, sys$clib/lib
$ link tsdntf,forsupff, 'hdflib, sys$clib/lib
$ link tsdnntf,forsupff, 'hdflib, sys$clib/lib
$ link tanf,forsupff, 'hdflib, sys$clib/lib
$ link tanfilef,forsupff, 'hdflib, sys$clib/lib
$ link tstubsf,forsupff, 'hdflib, sys$clib/lib
$ link tsdstrf,forsupff, 'hdflib, sys$clib/lib
$ link slabwf,forsupff, 'hdflib, sys$clib/lib
$! link slab1wf,forsupff, 'hdflib, sys$clib/lib
$! link slab2wf,forsupff, 'hdflib, sys$clib/lib
$! link slab3wf,forsupff, 'hdflib, sys$clib/lib
$! link slab4wf,forsupff, 'hdflib, sys$clib/lib
$ link manf,forsupff, 'hdflib, sys$clib/lib
$ link mgrf,forsupff, 'hdflib, sys$clib/lib
$ link egfhi,forsupff, 'hdflib, sys$clib/lib
$ link tvsetf,forsupff, 'hdflib, sys$clib/lib
$ link fortestF, forsupff,tr8f,t24f,tpf,tsdmmsf, -
  tsdnmmsf,tsdntf,tsdnntf,tanf,tanfilef,tstubsf,tsdstrf, -
  slabwf,manf,mgrf,tvsetf,-
  'hdflib, sys$clib/lib
$! delete *.obj;*
$!
