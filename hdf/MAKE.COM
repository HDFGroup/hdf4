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
$ create/dir [.lib]
$ create/dir [.include]
$ create/dir [.bin]
$ define/nolog hdf$src     mrlxp4$dka300:[folk.HDF33r3.hdf.src]   ! HDF src
$ define/nolog hdf$util    mrlxp4$dka300:[folk.HDF33r3.hdf.util]  ! HDF utilities
$ define/nolog hdf$test    mrlxp4$dka300:[folk.HDF33r3.hdf.test]  ! HDF test programs
$ define/nolog hdf$lib     mrlxp4$dka300:[folk.HDF33r3.hdf.lib]df ! Location and name for lib
$ define/nolog hdf$include mrlxp4$dka300:[folk.HDF33r3.hdf.include] ! Where to put includes
$ define/nolog hdf$bin     mrlxp4$dka300:[folk.HDF33r3.hdf.bin]   ! Where to put executables
$ !
$ define/nolog sys$clib sys$library:vaxcrtl
$ !
! $ ! Makefile for VAX/VMX systems.
! $ !
! $ ! Make full df lib (with fortran stubs)
$ type sys$input
  ======== Building HDF 4.0 ========
! $ !              
! $ ! Build the base HDF library
! $ !
$ set def hdf$src
$ @MAKENOF
! $ !
! $ ! If you do not want to include the HDF Fortran interfaces, comment
! $ !   out the following command.
! $ !
$ @MAKEFS
! $ !
! $ ! Move the files to their correct destinations
! $ !
$ copy df.olb hdf$lib
$ copy *.h    hdf$include
! $ !
! $ ! Build the HDF utility programs
! $ !
$ set default hdf$util
$ @MAKEUTIL
! $ !
! $ ! Move the HDF utilities to where they belong
! $ !
$ copy *.exe hdf$bin
! $ !
$ @setuputils
! $ !
! $ ! Build the test programs
! $ !
$ set default hdf$test
$ @MAKE
$ set default hdf$src
$ !
$ type sys$input
  ======== HDF 4.0 has now been built ========
