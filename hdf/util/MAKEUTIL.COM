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
!$ !
!$ ! Make HDF utilities
!$ !
!$ ! This assumes that you have run top level MAKE.COM which came
!$ !   with the HDF distribution.  Otherwise, you will need to 
!$ !   manually define hdf$lib and sys$clib
$ !
$ type sys$input
	Making utilities
$ utils="hdfls,hdfrseq,r8tohdf,hdfcomp,hdftor8,hdftopal,"+-
"paltohdf,hdf24to8,hdfpack,ristosds,vshow,vmake,hdf2jpeg,"+-
"jpeg2hdf,he_main,he_cntrl,he_disp,he_file"
!$ define/nolog hdf$lib MRLXP4$DKA300:[folk.hdf33.hdf.lib]df.olb
!$ define/nolog  sys$clib sys$library:vaxcrtl
$ cc/define=VMS/include=[-.include] 'utils
$ link/exe=hdfed he_main,he_cntrl,he_disp,he_file,hdf$lib/lib, -
   sys$clib/lib, MRLXP4$DKA300:[FOLK.HDF33R3.HDF.UTIL]helink.opt/option
$ link hdfls,     hdf$lib/lib, sys$clib/lib
$ link hdfrseq,   hdf$lib/lib, sys$clib/lib
$ link r8tohdf,   hdf$lib/lib, sys$clib/lib
$ link hdfcomp,   hdf$lib/lib, sys$clib/lib
$ link hdftor8,   hdf$lib/lib, sys$clib/lib
$ link hdftopal,  hdf$lib/lib, sys$clib/lib
$ link paltohdf,  hdf$lib/lib, sys$clib/lib
$ link hdf24to8,  hdf$lib/lib, sys$clib/lib
$ link hdfpack,   hdf$lib/lib, sys$clib/lib
$ link ristosds,  hdf$lib/lib, sys$clib/lib
$ link vshow,     hdf$lib/lib, sys$clib/lib
$ link vmake,     hdf$lib/lib, sys$clib/lib
$ link hdf2jpeg,  hdf$lib/lib, sys$clib/lib
$ link jpeg2hdf,  hdf$lib/lib, sys$clib/lib

$ type sys$input
	Made all utils
****	Please edit setuputils.com and run the script to set up executables
$
