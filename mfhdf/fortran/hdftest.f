C
C     Testing the Fortran interface for the multiple SD routines
C
      program hdftest

      implicit none

      integer fid1, fid2

      integer sds1, sds2, sds3, sds4

      integer dim1, dim2

      integer access, nt, rank, stat, ival, ivals(1000), i, err
      integer dims(10), start(10), end(10), stride(10), count, nattr
      integer max, min, num, ref, j
      integer natt(2), inatt(2)
      real fval

      real*8  cal, cale, ioff, ioffe
      real*8  eps
      character*50  name, l, u, f, c
      character cdata(6,4), icdata(6,4), cfill, icfill
      character catt(2), icatt(2)
C comment out the next line for VMS
      integer   i32(2), ii32(2)

      integer sfstart,  sfcreate,  sfendacc, sfend,    sfsfill
      integer sfrdata,  sfwdata,   sfdimid,  sfsdmname
      integer sffinfo,  sfn2index, sfsdmstr, sfsdtstr, sfsdscale
      integer sfscal,   sfselect,  sfginfo,  sfgdinfo, sfgainfo
      integer sffattr,  sfsrange,  sfgrange, sfgfill
      integer sfgcal,   sfgdscale, sfgdtstr, sfgdmstr
      integer sfid2ref, sfref2index, sfsdmvc, sfisdmvc
      integer sfsextf,  hxsdir,    hxscdir
      integer sfwcdata, sfrcdata,  sfscfill, sfgcfill
      integer sfscatt,  sfrcatt,   sfsnatt,  sfrnatt
C commnet out next line for VMS 
      integer sfsattr,  sfrattr
      integer SD_UNLIMITED, SD_DIMVAL_BW_INCOMP, DFNT_INT32
      integer SD_DIMVAL_BW_COMP
      parameter (SD_UNLIMITED = 0,
     +            SD_DIMVAL_BW_INCOMP = 0,
     +            SD_DIMVAL_BW_COMP = 1,
     +            DFNT_INT32 = 24)
      DATA cfill/'@'/, icfill/' '/
      DATA catt/'U','S'/, icatt/' ',' '/
      DATA natt/10,20/, inatt/0,0/
C Comment out next line for VMS 
      DATA i32/15,25/, ii32/0,0/

C     create a new file
      err = 0
      eps = 0.0001
      access = 4
      fid1 = sfstart('test1.hdf', access)
      if(fid1.ne.393216) then
         print *, 'Hopen returned bad ID'
         err = err + 1
      endif

      dims(1) = 4
      dims(2) = 9
      nt = 24
      rank = 2
      sds1 = sfcreate(fid1, 'Alpha', nt, rank, dims)
      if(sds1.eq.-1) then
         print *, 'SDcreate #1 returned bad ID', sds1
         err = err + 1
      endif

      dims(1) = 2
      dims(2) = 5
      dims(3) = 15
      nt = 5
      rank = 3
      sds2 = sfcreate(fid1, 'Beta[float32]', nt, rank, dims)
      if(sds2.eq.-1) then
         print *, 'SDcreate #2 returned bad ID', sds2
         err = err + 1
      endif

      ival = 14
      stat = sfsfill(sds1, ival)
      if(stat.ne.0) then
         print *, 'Set fill returned', stat
         err = err + 1
      endif

      max = 10
      min = 1
      stat = sfsrange(sds1, max, min)
      if(stat.ne.0) then
         print *, 'Set range returned', stat
         err = err + 1
      endif
      max = 0
      min = 0

      do 100 i = 1, 10
         ivals(i) = i
 100  continue

      start(1)  = 0
      start(2)  = 0
      stride(1) = 1
      stride(2) = 1
      end(1)    = 3
      end(2)    = 3
      stat = sfwdata(sds1, start, stride, end, ivals)
      if(stat.ne.0) then
         print *, 'Write data returned', stat
         err = err + 1
      endif
      
      fval = -14.25
      stat = sfsfill(sds2, fval)
      if(stat.ne.0) then
         print *, 'Set fill returned', stat
         err = err + 1
      endif

      start(1)  = 1
      start(2)  = 1
      stride(1) = 1
      stride(2) = 1
      end(1)    = 3
      end(2)    = 3
      stat = sfrdata(sds1, start, stride, end, ivals)
      if(stat.ne.0) then
         print *, 'Read data returned', stat
         err = err + 1
      endif

      if (ivals(1).ne.5)  then
         err = err + 1
         print *, 'was expecting 5 got', ivals(1)
      endif
      
      if(err.ne.0) print *, 'Before ReadVerify err = ', err
      if (ivals(2).ne.6)  then
        err = err + 1
        print *, 'was expecting 6 got', ivals(2)
      endif
      if (ivals(3).ne.14) then
        err = err + 1
        print *, 'was expecting 14 got', ivals(3)
        endif
      if (ivals(4).ne.8)  then
        err = err + 1
        print *, 'was expecting 8 got', ivals(4)
        endif


      if (ivals(5).ne.9)  then
        err = err + 1
        print *, 'was expecting 9 got', ivals(5)
        endif

      if (ivals(6).ne.14) then
        err = err + 1
        print *, 'was expecting 14 got', ivals(6)
        endif

      if (ivals(7).ne.14) then
        err = err + 1
        print *, 'was expecting 14 got', ivals(7)
        endif

      if (ivals(8).ne.14) then
        err = err + 1
        print *, 'was expecting 14 got', ivals(8)
        endif

      if (ivals(9).ne.14) then
        err = err + 1
        print *, 'was expecting 14 got', ivals(9)
        endif

      if(err.ne.0) print *, 'After ReadVerify err = ', err

      nt = 24
      stat = sfsnatt(sds2, 'TestAttr', nt, 3, ivals)
      if(stat.ne.0) then
         print *, 'Set numeric attr returned', stat
         err = err + 1
      endif

      dim1 = sfdimid(sds2, 1)
      if(dim1.ne.327683) then
         print *, 'Dim id returned', dim1
         err = err + 1
      endif

      stat = sfsdmname(dim1, 'TestDim')
      if(stat.ne.0) then
         print *, 'Set dim name returned', stat
         err = err + 1
      endif

      stat = sfsdmstr(dim1, 'dA', 'dBB', 'dCCC')
      if(stat.ne.0) then
         print *, 'Set dim strs returned', stat
         err = err + 1
      endif

      do 110 i = 1, 10
         ivals(i) = 10 * i + i
 110  continue

      nt = 24
      count = 5
      stat = sfsdscale(dim1, count, nt, ivals)
      if(stat.ne.0) then
         print *, 'Set dims scales returned', stat
         err = err + 1
      endif

      stat = sfsdtstr(sds1, 'lxxx', 'uyyy', 'fzzz', 'caaa')
      if(stat.ne.0) then
         print *, 'Set data strings returned', stat
         err = err + 1
      endif

      stat = sffinfo(fid1, num, nattr)
      if(stat.ne.0.or.num.ne.3) then
         print *, 'File info returned wrong values', stat, num
         err = err + 1
      endif

      cal   = 10.1
      cale  = 20.1
      ioff  = 40.1
      ioffe = 50.1
      nt    = 16
      stat = sfscal(sds2, cal, cale, ioff, ioffe, nt)
      if(stat.ne.0) then
         print *, 'Set calibration returned', stat
         err = err + 1
      endif

      stat = sfn2index(fid1, 'Alpha')
      if(stat.ne.0) then
         print *, 'Index of Alpha data set is wrong', stat
         err = err + 1
      endif

      ref = sfid2ref(sds1)
      if(ref.eq.0) then
         print *, 'sfidtoref failed'
         err = err + 1
      endif

      stat = sfref2index(fid1, ref)
      if(stat.ne.0) then
         print *, 'mapping from ref to index failed', stat
         err = err + 1
      endif

      stat = sfn2index(fid1, 'Bogus')
      if(stat.ne.(-1)) then
         print *, 'Found a bogus data set with index', stat
         err = err + 1
      endif

      nt = 4
      stat = sfscatt(fid1, 'Globulator', nt, 12, 'Howdy Sailor')
      if(stat.ne.0) then
         print *, 'Set attr returned', stat
         err = err + 1
      endif
C Comment out the sfsattr part for VMS
C sfsattr declairs input data buffer as character*(*)
C  which doesn't work on VMS. Use sfscatt or sfsnatt
C  instead of sfsattr. 
      nt = 24
      stat = sfsattr(fid1, 'Numeric', nt, 2, i32)
      if(stat.ne.0) then
         print *, 'Set attr returned', stat
         err = err + 1
      endif

      stat = sfendacc(sds1)
      if(stat.ne.0) then
         print *, 'SDendaccess returned', stat
         err = err + 1
      endif
      stat = sfend(fid1)
      if(stat.ne.0) then
         print *, 'SDend returned', stat
         err = err + 1
      endif

C
C     OK, let's open it back up and take a look at what we've done
C
      fid2 = sfstart('test1.hdf', 3)
      if(fid2.ne.393216) then
          print *, 'Reopen returned', fid2
          err = err + 1
      endif
 
      sds3 = sfselect(fid2, 0)
      if(sds3.ne.262144) then
         print *, 'Select returned', sds3
         err = err + 1
      endif

      stat = sfginfo(sds3, name, rank, ivals, nt, nattr)
      if(stat.ne.0) then
         print *, 'Get info returned ', stat
         err = err + 1
      endif

      if(nt.ne.24) then
         print *, 'Incorrect number type ', nt
         err = err + 1
      endif

      if(rank.ne.2) then
         print *, 'Incorrect rank ', rank
         err = err + 1
      endif

      if(ivals(1).ne.4) then
         print *, 'Incorrect Dim(1) = ', ivals(1)
         err = err + 1
      endif

      if(ivals(2).ne.9) then
         print *, 'Incorrect Dim(2) = ', ivals(2)
         err = err + 1
      endif

      if(nattr.ne.6) then
         print *, 'Wrong number of attributes returned', nattr
         err = err + 1
      endif

      print *, 'name = ',   name

      stat = sfgrange(sds3, max, min)
      if(stat.ne.0) then
         print *, 'Get range returned', stat
         err = err + 1
      endif

      if(max.ne.10) then
         print *, 'Max from GetRange ', max
         err = err + 1
      endif

      if(min.ne.1) then
         print *, 'Min from GetRange ', min
         err = err + 1
      endif

      if(err.ne.0) print *, 'Current error count ', err

      stat = sfgfill(sds3, max)
      if(stat.ne.0) then
         print *, 'Get fillvalue returned', stat
         err = err + 1
      endif

      if(max.ne.14) then
         print *, 'Incorrect FillValue ', max
         err = err + 1
      endif

      sds4 = sfselect(fid2, 1)
      if(sds4.ne.262145) then
         print *, 'Select #4  returned', sds4
         err = err + 1
      endif

      dim2 = sfdimid(sds4, 1)
      if(dim2.ne.327683) then
         print *, 'Get dim id #2 returned', dim2
         err = err + 1
      endif

      stat = sfgdinfo(dim2, name, rank, nt, nattr)
      if(stat.ne.0) then
         print *, 'Get dim info returned', stat
         err = err + 1
      endif

      if(nt.ne.24) err = err + 1
      if(rank.ne.5) err = err + 1
      print *, 'name = ',   name

      stat = sfgainfo(fid2, 0, name, nt, rank)
      if(stat.ne.0) then
         print *, 'Attr info returned', stat
         err = err + 1
      endif
      
      if(nt.ne.4) err = err + 1
      if(rank.ne.12) err = err + 1
      print *, 'name = ',   name

      cal   = 0
      cale  = 0
      ioff  = 0 
      ioffe = 0
      nt    = 0
      stat = sfgcal(sds4, cal, cale, ioff, ioffe, nt)
      if(stat.ne.0) then
         print *, 'Get cal returned', stat
         err = err + 1
      endif

      if(abs(cal - 10.1) .gt. eps) err = err + 1
      if(abs(cale - 20.1) .gt. eps) err = err + 1
      if(abs(ioff - 40.1) .gt. eps) err = err + 1
      if(abs(ioffe - 50.1) .gt. eps) err = err + 1
      if(nt.ne.16) err = err + 1


      do 120 i = 1, 10
         ivals(i) = 0
 120  continue

      stat = sfgdscale(dim2, ivals)
      if(stat.ne.0) then
         print *, 'Get scales returned', stat
         err = err + 1
      endif

      if (ivals(1).ne.11) err = err + 1
      if (ivals(2).ne.22) err = err + 1
      if (ivals(3).ne.33) err = err + 1
      if (ivals(4).ne.44) err = err + 1
      if (ivals(5).ne.55) err = err + 1

      stat = sfgdtstr(sds3, l, u, f, c, 50)
      if(stat.ne.0) then
         print *, 'Get data strs returned', stat
         err = err + 1
      endif
      
      print *, 'label    = ', l
      print *, 'unit     = ', u
      print *, 'format   = ', f
      print *, 'coordsys = ', c

      stat = sfgdmstr(dim2, l, u, f, 50)
      if(stat.ne.0) then
         print *, 'Get dim strs returned', stat
         err = err + 1
      endif
      
      print *, 'label    = ', l
      print *, 'unit     = ', u
      print *, 'format   = ', f

      stat = sfrcatt(fid2, 0, name)
      if(stat.ne.0) then
         print *, 'Attr read returned', stat
         err = err + 1
      endif
      print *, 'values = ', name
C Comment out the sfrattr part for VMS
      stat = sfrattr(fid2, 1, ii32)
      if(stat.ne.0) then
         print *, 'Attr read returned', stat
         err = err + 1
      endif
      if ((ii32(1) .ne. 15) .or. (ii32(2) .ne. 25)) then
         print *, 'Numeirc attr read erro: '
         print *, ' should be 15 25, get ',ii32(1), ii32(2)
         err = err + 1
      endif

C
C     Testing External Element functions: sfsextf, hxsdir, hxscdir.
C     First set the external create directory to "testdir".
C     Set dataset sds3 to store in external file.
C     Try read it back (should fail the first time).
C     Set locating directory to "nosuch:testdir".
C     Read again.  Should succeed this time.
C
      stat = hxscdir('testdir')
      if(stat.ne.0) then
	 print *, 'HX set create dir (hxscdir) returned', stat
	 err = err + 1
      endif

      stat = sfsextf(sds3, 'testext.hdf', 0)
      if(stat.ne.0) then
	 print *, 'set external file (sfsextf) returned', stat
	 err = err + 1
      endif

C
C     Close and reopen sds3 so that data is flushed to the ext. file
C
      stat = sfendacc(sds3)
      if(stat.ne.0) then
         print *, 'sfendacc returned', stat
         err = err + 1
      endif

      sds3 = sfselect(fid2, 0)
      if(sds3.eq.-1) then
         print *, 'Select returned', sds3
         err = err + 1
      endif

      start(1)  = 1
      start(2)  = 1
      stride(1) = 1
      stride(2) = 1
      end(1)    = 3
      end(2)    = 3
      stat = sfrdata(sds3, start, stride, end, ivals)
C
C     Should fail first time.
C
      if(stat.ne.-1) then
         print *, 'Read data (sfrdata) returned', stat
         err = err + 1
      endif

      stat = hxsdir('nosuch|testdir')
      if(stat.ne.0) then
	 print *, 'HX set dir (hxscdir) returned', stat
	 err = err + 1
      endif

      stat = sfrdata(sds3, start, stride, end, ivals)
C
C     Should succeed this time.
C
      if(stat.ne.0) then
         print *, 'Read data (sfrdata) returned', stat
         err = err + 1
      endif

      if (ivals(1).ne.5)  then
         err = err + 1
         print *, 'was expecting 5 got', ivals(1)
      endif

      stat = sfendacc(sds3)
      if(stat.ne.0) then
         print *, 'sfendacc returned', stat
         err = err + 1
      endif

      stat = sfendacc(sds4)
      if(stat.ne.0) then
         print *, 'sfendacc returned', stat
         err = err + 1
      endif

      stat = sfend(fid2)
      if(stat.ne.0) then
         print *, 'SDend returned', stat
         err = err + 1
      endif

C     test sfsdmvc and sfisdmvc -- dimval backward compatible 
      fid1 = sfstart('test2.hdf', 4)
      if(fid1 .lt. 1) then
         print *, 'sfstart returned', fid1
         err = err + 1
      endif

      dims(1) = 6
      dims(2) = 0
      nt = 24
      rank = 2
      sds1 = sfcreate(fid1, 'ncomp', nt, rank, dims)
      if (sds1 .eq. -1) then
         print *, 'sfcreate returned', sds1
         err = err + 1
      endif

      dim1 = sfdimid(sds1, 0)
      if (dim1 .eq. -1) then
         print *, 'sfdimid returned', dim1
         err = err + 1
      endif

      stat = sfsdmvc(dim1, 0)
      if(stat .ne. 0) then
         print *, 'sfsdmvc returned', stat
         err = err + 1
      endif
      dim2 = sfdimid(sds1, 1)
      stat = sfsdmvc(dim2, 0)
      if(stat .ne. 0) then
         print *, 'sfsdmvc returned', stat
         err = err + 1
      endif
      do 140 i=1, 6
         ivals(i) = i*5
140   continue
      stat = sfsdscale(dim1, 6, DFNT_INT32, ivals)
      if(stat .ne. 0) then
          print *, 'sfsdscale returned', stat
          err = err + 1
      endif
      start(1)=0
      start(2)=0
      stride(1) = 1
      stride(2) = 1
      end(1)=6
      end(2)=4
      do 160 i=1, 24
        ivals(i) = i
160   continue
      stat = sfwdata(sds1, start, stride, end, ivals)
      if (stat .ne. 0) then
          print *, 'sfwdata returned', stat
          err = err + 1
      endif
      stat = sfendacc(sds1)
      if(stat .ne. 0) then
           print *, 'sfendacc returned', stat
           err = err + 1
      endif

      stat = sfend(fid1)
      if(stat .ne. 0) then
         print *, 'SDend returned', stat
         err = err + 1
      endif

C     let's open it back up and take a look at what we've done
C

      fid2 = sfstart('test2.hdf', 3)
      if(fid2 .lt.  0) then
         print *, 'Reopen returned', fid2
         err = err + 1
      endif

      stat = sfn2index(fid2, 'ncomp')
      if (stat .lt. 0) then
         print *, 'sfn2index returned', stat
         err = err + 1
      endif

      sds3 = sfselect(fid2, stat)
      if (sds3 .eq. -1) then
         print *, 'sfselect returned', sds3
         err = err + 1
      endif
      stat = sfginfo(sds3, name, rank, ivals, nt, nattr)
      if (stat .ne. 0) then
          print *, 'sfginfo returned', stat
          err = err + 1
      endif
      if ((rank .ne. 2) .or. (ivals(1) .ne. 6) .or.
     +    (ivals(2) .ne. 4) .or. (nt .ne. 24)) then
          print *, 'error in sfginfo'
          err = err + 1
      endif
      dim2=sfdimid(sds3,1)
      stat = sfgdinfo(dim2, name, dims(2), nt, nattr)
      if ((dims(2) .ne. SD_UNLIMITED) .or. (nt .ne.  0 ))  then
          print *, '1st sfgdinfo error', stat, dims(2), nt
          err = err + 1
      endif
      dim1=sfdimid(sds3,0)
      stat = sfgdinfo(dim1, name, dims(1), nt, nattr)
      if ((dims(1) .ne. 6) .or. (nt .ne. DFNT_INT32 ))  then
         print *, '2nd sfgdinfo error', stat, dims(1), nt
         err = err + 1
      endif
      stat = sfrdata(sds3, start, stride, end, ivals)
      if (stat .ne. 0) then
           print *, 'sfrdata returned', stat
           err = err + 1
      endif
      do 180 i=1, 24
          if (ivals(i) .ne. i)  then
              print *,  'wrong value: should be ',i,'  got ',ivals(i)
              err = err + 1
          endif
180    continue
      stat = sfisdmvc(dim1)
      if (stat .ne. 0)  then
          print *, 'sfisdmvc returned', stat
          err = err + 1
      endif
      stat = sfsdmvc(dim1, 1)
      stat = sfendacc(sds3)
      if (stat .ne. 0) then
          print *, 'sfendacc returned', stat
          err = err + 1
      endif
      stat = sfend(fid2)
      if (stat .ne. 0) then
           print *, 'sfend returned', stat
           err = err + 1
      endif

C     open one last time to check that NDG ref has been constant
C     check SDsetdimval_compat
      fid1 = sfstart('test2.hdf', 3)
      if (fid1 .eq. -1) then
           print *, 'sfstart returned', stat
           err = err + 1
      endif
C     read back dimval_non_compat
      stat = sfn2index(fid1, 'ncomp')
      if (stat .lt. 0) then
         print *, 'sfn2index returned', stat
         err = err + 1
      endif

      sds2 = sfselect(fid1, stat)
      if (sds2 .eq. -1) then
         print *, 'sfselect returned', sds2
         err = err + 1
      endif
      stat = sfginfo(sds2, name, rank, ivals, nt, nattr)

      if (stat .ne. 0) then
          print *, 'sfginfo returned', stat
          err = err + 1
      endif
      if ((rank .ne. 2) .or. (ivals(2) .ne. 4) .or.
     +    (ivals(1) .ne. 6) .or. (nt .ne. 24)) then
          print *, 'error in sfginfo'
          err = err + 1
      endif
      dim1=sfdimid(sds2,0)
      stat = sfgdinfo(dim1, name, dims(1), nt, nattr)
      if ((dims(1) .ne. 6) .or. (nt .ne. DFNT_INT32 ))  then
         print *, '3rd sfgdinfo error', stat, dims(1), nt 
         err = err + 1
      endif
      stat = sfisdmvc(dim1)
      if (stat .ne. 1)  then
          print *, 'sfisdmvc returned', stat
          err = err + 1
      endif
      stat = sfendacc(sds2)
      if (stat .lt. 0) then
          print *, 'sfendacc returned', stat
          err = err + 1
      endif
      stat = sfend(fid1)
      if (stat .lt. 0) then
           print *, 'sfend returned', stat
           err = err + 1
      endif

C Test char attr, char fill value and char data routines
C sfscatt,sfrnatt,sfsnatt, sfrnatt,sfwcdata,sfrcdata
C sfscfill, sfgcfill
      fid1 = sfstart('test2.hdf', 4)
      if(fid1 .lt. 1) then
         print *, 'sfstart returned', fid1
         err = err + 1
      endif

      dims(1) = 6
      dims(2) = 0
      nt = 4
      rank = 2
      sds1 = sfcreate(fid1, 'char_type', nt, rank, dims)
      if (sds1 .eq. -1) then
         print *, 'sfcreate returned', sds1
         err = err + 1
      endif
C Set char fill value
      stat = sfscfill(sds1, cfill)
      if (stat .ne. 0) then
         print *, 'sfscfill returned', stat
         err = err + 1
      endif
      start(1) = 0
      start(2) = 1
      stride(1) = 1
      stride(2) = 1
      end(1) = 6
      end(2) = 2
C create the char data
      do 195 i=1,4
         do 190 j=1,6
             cdata(j,i) = 'C'
             icdata(j,i) = ' '
190      continue
195   continue
C Write a slab of char data
      stat = sfwcdata(sds1, start, stride, end, cdata)
      if (stat .ne. 0) then
          print *, 'sfwdata returned', stat
          err = err + 1
      endif
C Set char attr
      stat = sfscatt(sds1, 'CharAttr',nt, 2, catt)
      if(stat.ne.0) then
         print *, 'sfscatt returned', stat
         err = err + 1
      endif
C Set numeric attr
      nt = 24
      stat = sfsnatt(sds1, 'NumericAttr',nt, 2, natt)
      if(stat.ne.0) then
         print *, 'sfsnatt returned', stat
         err = err + 1
      endif
      stat = sfendacc(sds1)
      if(stat .ne. 0) then
           print *, 'sfendacc returned', stat
           err = err + 1
      endif
C Close file
      stat = sfend(fid1)
      if(stat .ne. 0) then
         print *, 'SDend returned', stat
         err = err + 1
      endif

C read back
      fid1 = sfstart('test2.hdf', 3)
      if(fid1 .lt. 1) then
         print *, 'sfstart returned', fid1
         err = err + 1
      endif
      stat = sfn2index(fid1, 'char_type')
      if (stat .lt. 0) then
         print *, 'sfn2index returned', stat
         err = err + 1
      endif
      sds2 = sfselect(fid1, stat)
      if (sds2 .eq. -1) then
         print *, 'sfselect returned', sds2
         err = err + 1
      endif
      stat = sfginfo(sds2, name, rank, ivals, nt, nattr)
      if (stat .ne. 0) then
          print *, 'sfginfo returned', stat
          err = err + 1
      endif
      start(1) = 0
      start(2) = 0
      stride(1) = 1
      stride(2) = 1
      end(1) = 6
      end(2) = 3
C read char data and char fill
      stat = sfrcdata(sds2, start, stride, end, icdata)
      do 200 i=1,6
         if (icdata(i,1) .ne. cfill) then 
          print *, 'error in read c_fill'
          err = err + 1
         endif
200   continue
      do 250 i=2,3
          do 230 j=1,6
             if (icdata(j,i) .ne. 'C') then
                 print *, 'error in sfrcdata'
                 err = err + 1
             endif
230       continue
250   continue
C read char fillvalue
      stat = sfgcfill(sds2, icfill)
      if ((stat .eq. -1) .or. (icfill .ne. cfill)) then
         print *, 'sfgcfill returned', sds2
         err = err + 1
      endif
 
C read char attr
      stat = sffattr(sds2, 'CharAttr')
      if (stat .eq. -1) then
         print *, 'sffattr returned', sds2
         err = err + 1
      endif
      stat = sfrcatt(sds2, stat, icatt)
      if ((icatt(1) .ne. catt(1)) .or. (icatt(2) .ne. catt(2))) then
         print *, 'sfrcatt returned', sds2
         err = err + 1
      endif
C read numeric attr
      stat = sffattr(sds2, 'NumericAttr')
      if (stat .eq. -1) then
         print *, 'sffattr returned', sds2
         err = err + 1
      endif
      stat = sfrnatt(sds2, stat, inatt)
      if ((inatt(1) .ne. natt(1)) .or. (inatt(2) .ne. natt(2))) then
         print *, 'sfrnatt returned', inatt(1), inatt(2)
         err = err + 1
      endif

      stat = sfendacc(sds2)
      if(stat .ne. 0) then
           print *, 'sfendacc returned', stat
           err = err + 1
      endif
C Close file
      stat = sfend(fid1)
      if(stat .ne. 0) then
         print *, 'SDend returned', stat
         err = err + 1
      endif

      print *, 'Total errors : ', err

      end

