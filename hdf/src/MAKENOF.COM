$ ! Makefile for VAX/VMX systems.
$ !
$ ! Make a no-fortran-stub HDF library
$ !
$ type sys$input
	Creating base HDF library

$ obj="df24, dfan, dfcomp, dfgr, dfgroup, dfimcomp, dfp, dfr8, dfsd, "+-
"dfstubs, dfutil, hblocks, herr, hextelt, hfile, hkit, vg, vgp, vhi, "+-
"vio, vparse, vrw, vsfld, dfconv, vconv"
$ cc/define=VMS 'obj
$ library/create df 'obj
$ type sys$input
	Created base HDF library
$ !
