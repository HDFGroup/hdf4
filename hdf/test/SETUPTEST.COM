!$ ! This file is only for VAX/VMS systems.
!$ !
!$ ! set up testhdf
!$ ! change below to the hdf bin directory
!$ ! 
$ define/nolog hdf$test    _$1$dia0:[folk.hdf33.hdf.test]
$ !
$ testhdf     :== $ hdf$test:testhdf
