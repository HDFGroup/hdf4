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
!
! $ !
! $ ! Build the entire HDF library
! $ !
! $ !
! $ ! The following defines should be used to tell VMS where to
! $ !   place the resulting HDF library, include files, and utilites
! $ !   These directories will not be created, you must create them
! $ !   by hand.
! $ !
$ create/dir [-.lib]
$ create/dir [-.include]
$ create/dir [-.bin]
$ define/nolog hdf$src     mrlxp4$dka200:[folk.dev.hdf.src]   ! HDF src
$ define/nolog hdf$util    mrlxp4$dka200:[folk.dev.hdf.util]  ! HDF utilities
$ define/nolog hdf$test    mrlxp4$dka200:[folk.dev.hdf.test]  ! HDF test programs
$ define/nolog jpeg$src    mrlxp4$dka200:[folk.dev.hdf.jpeg]  !
$ define/nolog zlib$src    mrlxp4$dka200:[folk.dev.hdf.zlib]  !
$ define/nolog hdf$lib     mrlxp4$dka200:[folk.dev.lib]df ! Location and name for lib
$ define/nolog jpeg$lib     mrlxp4$dka200:[folk.dev.lib]libjpeg.olb
$ define/nolog zlib$lib     mrlxp4$dka200:[folk.dev.lib]libz.olb
$ define/nolog hdf$include mrlxp4$dka200:[folk.dev.include] ! Where to put includes
$ define/nolog hdf$bin     mrlxp4$dka200:[folk.dev.bin]   ! Where to put executables
$ !
$ define/nolog sys$clib sys$library:vaxcrtl
$ !
$ ! Makefile for VAX/VMX systems.
$ !
$ ! Make jpeg library
$  type sys$input
  ======== Building jpeg library =======
$ set def jpeg$src
$ copy jconfig.vms jconfig.h
$ copy makefile.vms make.com
$ @make
$ copy libjpeg.olb jpeg$lib
$ copy jconfig.h hdf$include
$ copy jerror.h hdf$include
$ copy jmorecfg.h  hdf$include
$ copy jpeglib.h  hdf$include
$ delete *.obj;*
$ delete *.exe;*
$!
$ ! Make gzip library
$  type sys$input
  ======== Building gzip library =======
$ set def zlib$src
$ copy make_vms.com make.com
$ @make
$ copy libz.olb zlib$lib
$ copy zconf.h hdf$include
$ copy zlib.h  hdf$include
$ delete *.obj;*
$ delete *.exe;*
$ !
$ ! Make full df lib (with fortran stubs)
$ type sys$input
  ======== Building HDF 4.0 ========
$ !              
$ ! Build the base HDF library
$ !
$ set def hdf$src
$ @MAKENOF
$ !
$ ! If you do not want to include the HDF Fortran interfaces, comment
$ !   out the following command.
$ !
$ @MAKEFS
$ !
$ ! Move the files to their correct destinations
$ !
$ copy df.olb hdf$lib
$ copy *.h    hdf$include
$ delete *.obj;*
$ !
$ ! Build the HDF utility programs
$ !
$ set default hdf$util
$ @MAKEUTIL
$ !
$ ! Move the HDF utilities to where they belong
$ !
$ copy *.exe hdf$bin
$ delete *.obj;*
$ !
$ @setuputils
$ !
$ ! Build the test programs
$ !
$ set default hdf$test
$ @MAKE
$ !
$ delete *.obj;*
$ type sys$input
  ======== HDF 4.0 has now been built ========
