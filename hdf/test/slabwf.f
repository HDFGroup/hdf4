C****************************************************************************
C* NCSA HDF                                                                 *
C* Software Development Group                                               *
C* National Center for Supercomputing Applications                          *
C* University of Illinois at Urbana-Champaign                               *
C* 605 E. Springfield, Champaign IL 61820                                   *
C*                                                                          *
C* For conditions of distribution and use, see the accompanying             *
C* hdf/COPYING file.                                                        *
C*                                                                          *
C****************************************************************************
C
C $Id$
C
      subroutine slabwf (num_err)
C
C Program to write data set using the DFSDxxx interface
C 
C Output files: slabwf.hdf, slab1wf, slab4wf

      implicit none
      include "fortest.inc"


      integer dssdims, dssdisc, dssdist, dssfill
      integer dssslab, dswslab, dseslab, dsrslab
      integer dsigdim, dswref, dsclear, dfsdrestart
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    slab1(3,1,1), slab2(3,1,2), slab3(3,2,1)
      real    slab4(3,1,1), slab5(1,3,2)
      real    sa(4,3,2)
      real    fillvalue
      integer i, j, k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 fn, sn, sn1, sn2, sn4
      integer fnlen
      integer num_err
      integer nref
      integer number_failed

      number_failed = 0

      DFTAG_SDT = 709 
      DFO_FORTRAN = 1
      di(1) = 4
      di(2) = 3
      di(3) = 2
      np = 2
      nr = 3
      nc = 4
      rank = 3
      scpln(1) =   0.0
      scpln(2) = 100.0
      scrow(1) =   0.0
      scrow(2) =  10.0
      scrow(3) =  20.0
      sccol(1) =   0.0
      sccol(2) =   1.0
      sccol(3) =   2.0
      sccol(4) =   3.0
      lcol = 'Column'
      ucol = 'Cm'
      fcol = 'Int32'
      lrow = 'Line'
      urow = 'Inch'
      frow = 'Int16'
      lpln = 'Time'
      upln = 'Second'
      fpln = 'Int32'
      fn = 'slabf.hdf'
      sn = 'slabwf.hdf'
      fillvalue = 1.0
      fnlen = 30
      slab1(1,1,1) = 110.0
      slab1(2,1,1) = 111.0
      slab1(3,1,1) = 112.0

      slab2(1,1,1) = 20.0
      slab2(2,1,1) = 21.0
      slab2(3,1,1) = 22.0
      slab2(1,1,2) = 120.0
      slab2(2,1,2) = 121.0
      slab2(3,1,2) = 122.0

      slab3(1,1,1) = 0.0
      slab3(2,1,1) = 1.0
      slab3(3,1,1) = 2.0
      slab3(1,2,1) = 10.0
      slab3(2,2,1) = 11.0
      slab3(3,2,1) = 12.0

      slab4(1,1,1) = 100.0
      slab4(2,1,1) = 101.0
      slab4(3,1,1) = 102.0

      slab5(1,1,1) = 3.0
      slab5(1,2,1) = 13.0
      slab5(1,3,1) = 23.0
      slab5(1,1,2) = 103.0
      slab5(1,2,2) = 113.0
      slab5(1,3,2) = 123.0
      
      do 200 i=1, np
         do 180 j=1, nr
            do 150 k=1, nc
            da(k,j,i) = scpln(i) + scrow(j) + sccol(k)
C           print *, da(k,j,i)
150   continue
180   continue
200   continue

      call MESSAGE(5,'Writing data as 5 slabs to slabwf.hdf',Verbosity)
C      print *,'\n   Writing data as 5 slabs to slabwf.hdf'

      num_err = 0

C Set dimension stuff etc
      ret = dssdims(rank, di)
      call VERIFY(ret,'dssdim',number_failed,Verbosity)

      ret = dssdist(1, lcol, ucol, fcol)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(2, lrow, urow, frow)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(3, lpln, upln, fpln)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdisc(1, di(1),  sccol)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(2, di(2), scrow)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(3, di(3), scpln)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)
C 
C Write it slab by slab   
C 

C     set the Fill value
      ret = dssfill(fillvalue)
      call VERIFY(ret,'dssfile',number_failed,Verbosity)

      sr(1) = 0
      sr(2) = 0
      sr(3) = 0
      ret = dssdims(rank, di)
      call VERIFY(ret,'dssdim',number_failed,Verbosity)

      ret = dssslab(sn)
      call VERIFY(ret,'dssslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 2
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dswslab(st, sr, sz,slab1)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 3
      st(3) = 1
      sz(1) = 3
      sz(2) = 1
      sz(3) = 2
      ret = dswslab(st, sr, sz, slab2)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 3
      sz(2) = 2
      sz(3) = 1
      ret = dswslab(st, sr, sz, slab3)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 1
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dswslab(st, sr, sz, slab4)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 4
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 3
      sz(3) = 2
      ret = dswslab(st, sr, sz, slab5)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)

      ret = dseslab()
      call VERIFY(ret,'dseslab',number_failed,Verbosity)
C
C Retrieve slab for verification
C
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 4
      sz(2) = 3
      sz(3) = 2
      sr(1) = 4
      sr(2) = 3
      sr(3) = 2
      ret = dsrslab(sn, st, sz, st, sa, sr)
      call VERIFY(ret,'dsrslab',number_failed,Verbosity)

      num_err = number_failed

      if ( num_err .ne. 0) then
         print *,'number of failures =',num_err
      endif
     
      call MESSAGE(5,'   Verifying data ', Verbosity)

      num_err = 0
      do 500 i=1, np
         do 400 j=1, nr
            do 300 k=1, nc
             if ( da(k,j,i) .ne. sa(k,j,i)) then
                 num_err = num_err + 1
             endif
C            print *, 'da() ',da(k,j,i)
C            print *, 'sa() ',sa(k,j,i)
300   continue
400   continue
500   continue

      if (num_err .ne. 0) then
           print *,'slabwf: number of wrong values in slab =',num_err
      else
           print *,'  *** slabwf passed *** '
      endif

C
C   slab1wf test
C
C      call MESSAGE(5,'Writing the first 3 of 5 slabs to slab1wf.hdf',Verbosity)
C      print *,'\n  Writing the first 3 of 5 slabs to slab1wf.hdf'
      sn1 = 'slab1wf.hdf'

C Clear old info fist
      ret = dsclear()
      call VERIFY(ret,'dsclear',number_failed,Verbosity)

C Set dimension stuff etc
      call MESSAGE(5,'Calling dssdim to set info on SDS',Verbosity)
      ret = dssdims(rank, di)
      call VERIFY(ret,'dssgdim',number_failed,Verbosity)

      ret = dssdist(1, lcol, ucol, fcol)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(2, lrow, urow, frow)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(3, lpln, upln, fpln)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdisc(1, di(1),  sccol)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(2, di(2), scrow)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(3, di(3), scpln)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)
C 
C Write it slab by slab   
C 
      sr(1) = 0
      sr(2) = 0
      sr(3) = 0
      ret = dssdims(rank, di)
      call VERIFY(ret,'dssdims',number_failed,Verbosity)

      ret = dssslab(sn1)
      call VERIFY(ret,'dssslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 2
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dswslab(st, sr, sz,slab1)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 3
      sz(2) = 2
      sz(3) = 1
      ret = dswslab(st, sr, sz, slab3)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 4
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 3
      sz(3) = 2
      ret = dswslab(st, sr, sz, slab5)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)

      ret = dseslab()
      call VERIFY(ret,'dseslab',number_failed,Verbosity)


      num_err = number_failed 
      if (num_err .ne. 0) then
           print *,'number of failures =',num_err
      else
           print *,'  *** slab1wf passsed *** '
      endif
      print *,' '

C
C  slab2wf test
C
C      call MESSAGE(5,'Writing the last 2 of 5 slabs to slab1wf.hdf',Verbosity)
C      print *,'\n   Writing the last 2 of 5 slabs to slab1wf.hdf'
      sn2 = 'slab1wf.hdf'
C set ref of SDS to write to
      nref = 2

C 
C Fisrt clear all previous info and restarot to beginning of file
C 
      ret = dsclear()
      call VERIFY(ret,'dsclear',number_failed,Verbosity)

      ret = dfsdrestart()
      call VERIFY(ret,'dfsdrestart',number_failed,Verbosity)

C Get relavant info on SDS

      call MESSAGE(5,'Calling dsigdim to get info on SDS',Verbosity)
      ret = dsigdim(sn2, rank, sz, rank, fnlen)
      call VERIFY(ret,'dsigdim',number_failed,Verbosity)

C      call MESSAGE(5,'Calling dswref to ref o SDS to write to',Verbosity)
C     set SDS to write to
      ret = dswref(sn2, nref)
      call VERIFY(ret,'dswref',number_failed,Verbosity)

      call MESSAGE(5,'Calling dssslab to start access on SDS',Verbosity)
      ret = dssslab(sn2)
      call VERIFY(ret,'dssslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 3
      st(3) = 1
      sz(1) = 3
      sz(2) = 1
      sz(3) = 2
C      call MESSAGE(5,'Calling dswslab to write 4of5 slabs to SDS',Verbosity)
      ret = dswslab(st, sr, sz, slab2)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 1
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
C      call MESSAGE(5,'Calling dswslab to write 5of5 slabs to SDS',Verbosity)
      ret = dswslab(st, sr, sz, slab4)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)

      call MESSAGE(5,'Calling dseslab to end access to SDS',Verbosity)
      ret = dseslab()
      call VERIFY(ret,'dseslab',number_failed,Verbosity)

      if ( num_err .ne. 0) then
         print *,'number of failures =',num_err
      endif

C
C Retrieve slab for verification
C
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 4
      sz(2) = 3
      sz(3) = 2
      sr(1) = 4
      sr(2) = 3
      sr(3) = 2
      num_err = 0
C      call MESSAGE(5,'Calling dsrslab to start read access to SDS',Verbosity)
      ret = dsrslab(sn2, st, sz, st, sa, sr)
      call VERIFY(ret,'dsrslab',number_failed,Verbosity)

      num_err = number_failed

      if ( num_err .ne. 0) then
         print *,'number of failures =',num_err
      endif

      call MESSAGE(5,'   Verifying data ', Verbosity)

      num_err = 0
      do 800 i=1, np
         do 700 j=1, nr
            do 600 k=1, nc
             if ( da(k,j,i) .ne. sa(k,j,i)) then
                 num_err = num_err + 1
             endif
C            print *, 'da() ',da(k,j,i)
C            print *, 'sa() ',sa(k,j,i)
600   continue
700   continue
800   continue

      if (num_err .ne. 0) then
           print *,'slab2wf: number of wrong values in slab =',num_err
      else
           print *,'  *** slab2wf passed ***  '
      endif

C
C  slab4wf test
C
      call MESSAGE(5,'Writing data as 1 slab to slab4wf.hdf',Verbosity)
C      print *,'Writing data as 1 slab to slab4wf.hdf'
      sn4 = 'slab4wf.hdf'
C
C Clear previous info

      ret = dsclear()
      call VERIFY(ret,'dsclear',number_failed,Verbosity)

C Set relevant info

      ret = dssdims(rank, di)
      call VERIFY(ret,'dssgdim',number_failed,Verbosity)

      ret = dssdist(1, lcol, ucol, fcol)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(2, lrow, urow, frow)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdist(3, lpln, upln, fpln)
      call VERIFY(ret,'dssdist',number_failed,Verbosity)

      ret = dssdisc(1, di(1),  sccol)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(2, di(2), scrow)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)

      ret = dssdisc(3, di(3), scpln)
      call VERIFY(ret,'dssdisc',number_failed,Verbosity)
C 
C Write it as one slab    
C 

      ret = dssslab(sn4)
      call VERIFY(ret,'dssslab',number_failed,Verbosity)
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 4
      sz(2) = 3
      sz(3) = 2
      ret = dswslab(st, sr, sz, da)
      call VERIFY(ret,'dswslab',number_failed,Verbosity)

      ret = dseslab()
      call VERIFY(ret,'dseslab',number_failed,Verbosity)
C
C Retrieve slab for verification
C
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 4
      sz(2) = 3
      sz(3) = 2
      sr(1) = 4
      sr(2) = 3
      sr(3) = 2
      ret = dsrslab(sn4, st, sz, st, sa, sr)
      call VERIFY(ret,'dsrslab',number_failed,Verbosity)

      num_err = number_failed

      if ( num_err .ne. 0) then
         print *,'number of failures =',num_err
      endif

      call MESSAGE(5,'   Verifying data ', Verbosity)

      num_err = 0
      do 902 i=1, np
         do 901 j=1, nr
            do 900 k=1, nc
             if ( da(k,j,i) .ne. sa(k,j,i)) then
                 num_err = num_err + 1
             endif
C            print *, 'da() ',da(k,j,i)
C            print *, 'sa() ',sa(k,j,i)
900   continue
901   continue
902   continue

      if (num_err .ne. 0) then
           print *,'slab4wf: number of wrong values in slab =',num_err
      else
           print *,'  *** slab4wf passed *** '
      endif

C
C
      return 
      end

