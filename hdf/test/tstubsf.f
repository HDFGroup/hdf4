C
C $Header$
C
C $Log$
C Revision 1.1  1992/03/26 21:51:22  dilg
C Initial revision
C
C
      program tdfstubsF

      character ar0*10
      character ar1*9
      character ar2*30
      character ar3*8

      data ar0 /'Testing...'/
      data ar1 /'...one...'/
      data ar2 /'...two...'/
      data ar3 /'...three'/

      character in*20

      integer nerrors /0/
      integer dfile
      integer dfinfo(16)
      integer dlist(128)
      integer a0size, a1size, a2size, a3size
      integer ret, err, i, nd, dfenum, tag, ref, length

      integer t255 /255/
      integer t127 /127/
      integer r1   /1/
      integer r3   /3/
      integer r7   /7/

      integer dfaccess, dfopen, dfclose, dfdesc, dfdup, dfdel, dfread,
     +		dfwrite, dfupdate, dfget, dfput, dfsfind, dffind, dferrno,
     +		dfishdf, dfnewref, dfnumber, dfstat

      a0size = len(ar0)
      a1size = len(ar1)
      a2size = len(ar2)
      a3size = len(ar3)

      print *, 'This program will test the DF emulation layer'
      print *, 'of HDF 3.2 and beyond (FORTRAN version).  Many'
      print *, 'routines will be tested individually.  Each test'
      print *, 'will report its name and results.  If all goes'
      print *, 'well, all of the results will begin with "Success".'
      print *, 'If a test fails, the results line will begin with'
      print *, '">>>Failure".  An error count is kept and printed'
      print *, 'out at the end.'
      print *, ' '
      print *, 'Hit <return> to continue.'
      read *, in
C      do 1 i=1, 20
C        in(i) = char(20)
C 1    continue
      in(1:20) = '                    '
C    getchar();

C#if defined PC || defined VMS
C    system('del o1');
C#else
C    system('rm o1');
C#endif /* PC .or. VMS */

      print *, ' '
      print *, 'Testing dferrno...'
      ret = dferrno()
      if (ret .ne. 0) then
	print *, '>>>Failure:  Returned ', ret, ' rather than 0.'
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfishdf... (should fail)'
      ret = dfishdf('o1')
      dfenum = dferrno()
      if (ret .eq. -1) then
        print *, 'Success:  dfishdf failed with DFerror = ', dfenum
      else
	print *, '>>>Failure:  Non-existent file looks like HDF file.'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      endif

      print *, ' '
      print *, 'Testing dfopen... (new file)'
      dfile = dfopen('o1', 6, 0)
      dfenum = dferrno()
      if (dfile .eq. 0) then
	print *, '>>>Failure:  Error ', dfenum, ' opening file.'
	print *, '   Quiting.'
	stop
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfclose...'
      ret = dfclose(dfile)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:  could not close file.'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfclose... (invalid file; should fail)'
      ret = dfclose(dfile)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, 'Success: dfclose failed with DFerror = ', dfenum
      else
	print *, '>>>Failure:  Close allowed on unopened file.'
	nerrors = nerrors + 1
      endif

      print *, ' '
      print *, 'Testing dfopen... (existing file)'
      dfile = dfopen('o1', 2, 0)
      dfenum = dferrno()
      if (dfile .eq. 0) then
	print *, '>>>Failure:  Error ', dfenum, ' opening file.'
	print *, '   Quiting.'
	stop
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfput...'
      ret = dfput(dfile, t255, r1, ar0, a0size)
      dfenum = dferrno()
      if (ret .ne. 0) then
	print *, '>>>Failure:  DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfget...'
      ret = dfget(dfile, t255, r1, in)
      dfenum = dferrno()
      if (ret .ne. a0size) then
	print *, '>>>Failure:  read ', ret, ' of ', a0size, ' bytes.'
	print *, '   String read:  ', in
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	err = 0
	do 10 i=1, a0size
	    if (in(i:i) .ne. ar0(i:i)) err = 1
 10	continue
	if (err .eq. 1) then
	  print *, '>>>Failure:  strings differ.'
	  print *, '   String written:  ', ar0
	  print *, '   String read:     ', in
	  nerrors = nerrors + 1
	else
	  print *, 'Success:  string read is the same as string written.'
        endif
      endif

C      do 20 i=1, 20
C        in(i) = char(20)
C 20   continue
      in(1:20) = '                    '

      print *, ' '
      print *, 'Testing dfaccess (write)...'
      ret = dfaccess(dfile, t255, r3, 'w')
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfread... (should fail)'
      ret = dfread(dfile, in, 5)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, 'Success:  dfread failed with DFerror = ', dfenum
      else
	print *, '>>>Failure:  Read allowed on element set up for write.'
	nerrors = nerrors + 1
      endif
C      do 30 i=1, 20
C        in(i) = char(20)
C 30   continue
      in(1:20) = '                    '

      print *, ' '
      print *, 'Testing dfwrite...'
      ret = dfwrite(dfile, ar1, a1size)
      dfenum = dferrno()
      if (ret .ne. a1size) then
	print *, '>>>Failure:  wrote ', ret, ' of ', a1size, ' bytes.'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfaccess (read)...'
      ret = dfaccess(dfile, t255, r3, 'r')
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfwrite... (should fail)'
      ret = dfwrite(dfile, in, 5)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, 'Success:  dfwrite failed with DFerror = ', dfenum
      else
	print *, '>>>Failure:  write allowed on element set up for read.'
	nerrors = nerrors + 1
      endif

      print *, ' '
      print *, 'Testing dfread...'
      ret = dfread(dfile, in, a1size)
      dfenum = dferrno()
      if (ret .ne. a1size) then
	print *, '>>>Failure:  read ', ret, ' of ', a1size, ' bytes.'
	print *, '   String read:  ', in
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	err = 0
	do 40 i=1, a1size
	  if (in(i:i) .ne. ar1(i:i)) err = 1
 40	continue
	if (err .eq. 1) then
	  print *, '>>>Failure:  strings differ.'
	  print *, '   String written:  ', ar1
	  print *, '   String read:     ', in
	  nerrors = nerrors + 1
	else
	  print *, 'Success:  string read is the same as string written.'
        endif
      endif
C      do 50 i=1, 20
C        in(i) = char(20)
C 50   continue
      in(1:20) = '                    '

      print *, ' '
      print *, 'Testing dfnumber...'
      nd = dfnumber(dfile, t255)
      dfenum = dferrno()
      if (nd .ne. 2) then
	print *, '>>>Failure:'
	print *, '   Reported ', nd, ' occurrances of tag 255 rather than 2.'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif
    
      print *, ' '
      print *, 'Testing dfdesc...'
      ret = dfdesc(dfile, dlist, 0, 5)
      dfenum = dferrno()
C
C  add one for version tag
C
      if (ret .ne. (nd + 1)) then
	print *, '>>>Failure:  Returned ', ret, ' rather than ', nd+1
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfupdate'
      ret = dfupdate(dfile)
      dfenum = dferrno()
      if (ret .ne. 0) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfstat'
      ret = dfstat(dfile, dfinfo)
      dfenum = dferrno()
      if (ret .ne. 0) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif
    
      print *, ' '
      print *, 'Testing dfnewref...'
      ret = dfnewref(dfile)
      dfenum = dferrno()
      if (ret .ne. 4) then
	print *, '>>>Failure:  Returned ref. no. ', ret, ' instead of 4'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dfdup...'
      ret = dfdup(dfile, 127, r7, t255, r3)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	ret = dfnumber(dfile, t127)
      dfenum = dferrno()
	if (ret .ne. 1) then
	  print *, '>>>Failure:  duplicated tag not found.'
	  print *, '   DFerror = ', dfenum
	  nerrors = nerrors + 1
	else
	  print *, 'Success!'
        endif
      endif

      print *, ' '
      print *, 'Testing dfdel...'
      ret = dfdel(dfile, t127, r7)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	ret = dfnumber(dfile, t127)
      dfenum = dferrno()
	if (ret .ne. 0) then
	  print *, '>>>Failure:  found ', ret, ' instances of deleted tag.'
	  print *, '   DFerror = ', dfenum
	  nerrors = nerrors + 1
	else
	  print *, 'Success!'
        endif
      endif

      print *, ' '
      print *, 'Testing dfsfind...'
      ret = dfsfind(dfile, t255, r3)
      dfenum = dferrno()
      if (ret .eq. -1) then
	print *, '>>>Failure:'
	print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	print *, 'Success!'
      endif

      print *, ' '
      print *, 'Testing dffind...'
      ret = dffind(dfile, tag, ref, length)
      dfenum = dferrno()
      if (ret .eq. -1) then
        print *, '>>>Failure:'
        print *, '   DFerror = ', dfenum
	nerrors = nerrors + 1
      else
	if ((tag .ne. t255) .or. (ref .ne. r3) .or. (length .ne. a1size)) then
	  print *, '>>>Failure:  tag/ref found is not correct.'
	  print *, '   Looking for:'
	  print *, '      tag:    ', t255
	  print *, '      ref:    ', r3
	  print *, '      length: ', a1size
	  print *, '   Found:'
	  print *, '      tag:    ', tag
	  print *, '      ref:    ', ref
	  print *, '      length: ', length
	  nerrors = nerrors + 1
	else
          print *, 'Success!'
	endif
      endif


      print *, ' '
      print *, ' '
      print *, 'Test Summary:'
      print *, '   ', nerrors, ' errors were encountered.'

      stop
      end
