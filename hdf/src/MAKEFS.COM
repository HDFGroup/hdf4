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
$! Id: MAKEFS.COM,v 1.5 1996/01/18 05:02:09 sxu Exp $
$! Add the Fortran stubs to the HDF library
$! 
$ ccc := cc/noopt/debug/define=VMS
$ type sys$input
	Compiling Fortran stubs
$ ffiles="df24ff.f, dfff.f, dfpff.f, dfr8ff.f, dfanff.f, dfsdff.f, "+-
"hfileff.f, vgff.f, dfufp2if.f, mfgrff.f"
$ fobj="dfff, dfsdff, dfr8ff, df24ff, dfanff, dfpff, vgff, hfileff, "+-
       "dfufp2if, mfgrff "
$ cobj="dff,  dfsdf,  dfr8f,  df24f,  dfanf,  dfpf,  vgf,  hfilef, "+-
       "dfutilf, herrf, mfgrf, mfanf"
$ fortran 'ffiles
$ ccc 'cobj
$ library/replace df 'cobj
$ library/replace df 'fobj
$ type sys$input
	Added Fortran stubs to HDF library
$!
