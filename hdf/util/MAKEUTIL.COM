$ !
$ ! Make HDF utilities
$ !
$ ! This assumes that you have run top level MAKE.COM which came
$ !   with the HDF distribution.  Otherwise, you will need to 
$ !   manually define hdf$lib and sys$clib
$ !
$ type sys$input
	Making utilities
$ utils="hdfls,hdfrseq,r8tohdf,hdfcomp,hdftor8,hdftopal,"+-
"paltohdf,hdf24to8,hdfpack,ristosds,vshow,vmake"
$ cc/define=VMS 'utils
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
$ type sys$input
	Made all utils
****	Please edit setuputils.com and run the script to set up executables
$
