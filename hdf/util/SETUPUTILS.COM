#
# $Header$
#
# $Log$
# Revision 1.1  1992/08/24 22:06:19  sxu
# Initial revision
#
#
$ ! This file is only for VAX/VMX systems.
$ !
$ ! set up utilities
$ ! change below to the hdf bin directory
$ ! 
$ define/nolog hdf$bin    _$4$dua1:[folk.hdf32.bin]
$ !
$ hdfls    :== $ hdf$bin:hdfls
$ hdfrseq  :== $ hdf$bin:hdfrseq
$ r8tohdf  :== $ hdf$bin:r8tohdf
$ hdfcomp  :== $ hdf$bin:hdfcomp
$ hdftor8  :== $ hdf$bin:hdftor8
$ hdftopal :== $ hdf$bin:hdftopal
$ paltohdf :== $ hdf$bin:paltohdf
$ hdf24to8 :== $ hdf$bin:hdf24to8
$ hdfpack  :== $ hdf$bin:hdfpack
$ ristosds :== $ hdf$bin:ristosds
$ vshow    :== $ hdf$bin:vshow
$ vmake    :== $ hdf$bin:vmake
$ hdfed    :== $ hdf$bin:hdfed

