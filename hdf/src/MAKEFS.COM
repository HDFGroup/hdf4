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
! $! Add the Fortran stubs to the HDF library
! $!
$ type sys$input
	Compiling Fortran stubs
$ ffiles="df24ff.f, dfff.f, dfpff.f, dfr8ff.f, dfanff.f, dfsdff.f, "+-
"hfileff.f, vgff.f, dfufp2if.f"
$ fobj="dfff, dfsdff, dfr8ff, df24ff, dfanff, dfpff, vgff, hfileff, "+-
       "dfufp2if"
$ cobj="dff,  dfsdf,  dfr8f,  df24f,  dfanf,  dfpf,  vgf,  hfilef, "+-
       "dfutilf, herrf"
$ fortran 'ffiles
$ cc/define=VMS 'cobj
$ library/insert df 'cobj
$ library/insert df 'fobj
$ type sys$input
	Added Fortran stubs to HDF library
$!
