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
!$ ! set up utilities
!$ ! change below to the hdf bin directory
!$ ! 
$ define/nolog hdf$bin    mrlxp4$dka300:[folk.ovms.hdf.util]
$ !
$ hdfls     :== $ hdf$bin:hdfls
!$ hdfrseq   :== $ hdf$bin:hdfrseq
$ r8tohdf   :== $ hdf$bin:r8tohdf
$ hdfcomp   :== $ hdf$bin:hdfcomp
$ hdftor8   :== $ hdf$bin:hdftor8
$ hdftopal  :== $ hdf$bin:hdftopal
$ paltohdf  :== $ hdf$bin:paltohdf
$ hdf24to8  :== $ hdf$bin:hdf24to8
$ hdfpack   :== $ hdf$bin:hdfpack
$ ristosds  :== $ hdf$bin:ristosds
$ vshow     :== $ hdf$bin:vshow
$ vmake     :== $ hdf$bin:vmake
$ hdfed     :== $ hdf$bin:hdfed
$ hdf2jpeg  :== $ hdf$bin:hdf2jpeg
$ jpeg2hdf  :== $ hdf$bin:jpeg2hdf
$ fp2hdf    :== $ hdf$bin:fp2hdf

