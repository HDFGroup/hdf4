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
!$ ! This file is only for VAX/VMS systems.
!$ !
!$ ! set up testhdf
!$ ! change below to the hdf bin directory
!$ ! 
$ define/nolog hdf$test    _$1$dia0:[folk.hdf33.hdf.test]
$ !
$ testhdf     :== $ hdf$test:testhdf
