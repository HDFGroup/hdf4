$!
$! Add the Fortran stubs to the HDF library
$!
$!
$ type sys$input
	Compiling Fortran stubs
$ ffiles="df24Ff.f, dfFf.f, dfpFf.f, dfr8Ff.f, dfanFf.f, dfsdFf.f, "+-
"hfileFf.f, vgFf.f"
$ fobj="dfFf, dfsdFf, dfr8Ff, df24Ff, dfanFf, dfpFf, vgFf, hfileFf"
$ cobj="dfF,  dfsdF,  dfr8F,  df24F,  dfanF,  dfpF,  vgF,  hfileF, dfutilF"
$ fortran 'ffiles
$ cc/define=VMS 'cobj
$ library/insert df 'cobj
$ library/insert df 'fobj
$ type sys$input
	Added Fortran stubs to HDF library
$!
