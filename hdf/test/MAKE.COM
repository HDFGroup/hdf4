! ****************************************************************************
! * NCSA HDF                                                                 *
! * Software Development Group                                               *
! * National Center for Supercomputing Applications                          *
! * University of Illinois at Urbana-Champaign                               *
! * 605 E. Springfield, Champaign IL 61820                                   *
! *                                                                          *
! * For conditions of distribution and use, see the accompanying             *
! * hdf/COPYING file.                                                        *
! *                                                                          *
! ***************************************************************************
!
! $Id$
!$!
!$!
!$! File to build the test files in the HDF test suite
!$!
!$!
!$! The following constants should probably be defined
!$!   only once in the main HDF MAKE.COM...
!$!
$ hdflib= "[-.src]df/lib,[-.jpeg]libjpeg/lib,[-.zlib]libz/lib"
!$ define/nolog hdf$include ([-.src],[-.jpeg],[-.zlib])
$ define/nolog sys$clib sys$library:vaxcrtl
$ ccopt= "/include=([-.src],[-.jpeg],[-.zlib])/define=VMS"

!$!
$ type sys$input
  **    Making testhdf **
$ cc 'ccopt   testhdf
$ type sys$input
  **    Making H level test programs **
!$!
$ cc 'ccopt   file
$ cc 'ccopt   file1
$ cc 'ccopt   blocks
$ cc 'ccopt   extelt
$ cc 'ccopt   vers
$ type sys$input
  **  Making C interface test programs **
!$!
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

$ link testhdf,rig,sdmms,sdnmms,an,anfile,sdstr,slab,litend, -
  file,file1,blocks,extelt,vers,tvset,conv,tree,bitio,comp,man,mgr.obj, -
  nbit, 'hdflib
$ type sys$input 
  **  Making Fortran interface test programs **
!$!
$ cc 'ccopt   fortest.c
$ cc 'ccopt   forsupf.c
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
$ fortran forsupff.f
$ link fortest, forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tr8f, forsupff, forsupf, 'hdflib, sys$clib/lib
$ link t24f,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tpf, forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tsdmmsf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tsdnmmsf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tsdntf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tsdnntf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tanf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tanfilef,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tstubsf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link tsdstrf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link slabwf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link slab1wf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link slab2wf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link slab3wf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link slab4wf,forsupff, forsupf, 'hdflib, sys$clib/lib
$ link egfhi,forsupff, forsupf, 'hdflib, sys$clib/lib
!$ delete *.obj;*
!$!


