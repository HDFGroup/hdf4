!$ ! Makefile for VAX/VMX systems.
!$ !
!$ ! Make a no-fortran-stub HDF library
!$ !
$ type sys$input
	Creating base HDF library

$ cobj="df24, dfan, dfcomp, dfgr, dfgroup, dfimcomp, dfp, dfr8, dfsd, "+-
"dfstubs, dfutil, hblocks, herr, hextelt, hfile, hkit, vg, vgp, vhi, "+-
"vio, vparse, vrw, vsfld, dfconv, vconv, dfrle, dfjpeg, dfunjpeg, "+-
"dfufp2i, dfknat, dfkswap, dfkcray, dfkvms, dfkconv, dfkfuji, hbitio, "+-
"tbbt, hdfalloc"
$ jcobj="jutils, jerror, jmemmgr, jmemsys, jcmaster, jcdeflts, jcarith, "+-
      "jccolor, jcexpand, jchuff, jcmcu, jcpipe, jcsample, jfwddct, "+-
      "jdmaster, jddeflts, jbsmooth, jdarith, jdcolor, jdhuff, jdmcu, "+-
      "jdpipe, jdsample, jquant1, jquant2, jrevdct"

$ cc/define=VMS 'cobj 
$ cc/define=VMS 'jcobj
$ library/create df 'cobj 
$ library/insert df 'jcobj
$ type sys$input
	Created base HDF library
$ !
