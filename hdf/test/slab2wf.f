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
      program slab2wf
C
C Program writes data set as the last 2 of 5 slabs to slab1wf.hdf
C The program slab1wf writes the first 3 of 5 slabs to slab1wf.hdf
C 
C Output file: slab1wf.hdf


      integer dssslab, dswslab, dseslab, dsrslab, dsigdim, dswref
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    slab1(3,1,1), slab2(3,1,2), slab3(3,2,1)
      real    slab4(3,1,1), slab5(1,3,2)
      real    sa(4,3,2)
      integer i,j,k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 sn
      integer fnlen
      integer num_err, nref

      print *,'\n   Writing the last 2 of 5 slabs to slab1wf.hdf'
     
      DFTAG_SDT = 709 
      DFO_FORTRAN = 1
      di(1) = 4
      di(2) = 3
      di(3) = 2
      np = 2
      nr = 3
      nc = 4
      rank = 3
      nref = 2
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
      sn = 'slab1wf.hdf'
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
150     continue
180      continue
200   continue

      
C 
C Write it slab by slab   
C 
      ret = 0
      num_err = 0
      ret = dsigdim(sn, rank, sz, rank, fnlen)
      num_err = num_err + ret

      ret = dswref(sn, nref)
      num_err = num_err + ret

      ret = dssslab(sn)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 3
      st(3) = 1
      sz(1) = 3
      sz(2) = 1
      sz(3) = 2
      ret = dswslab(st, sr, sz, slab2)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dswslab(st, sr, sz, slab4)
      num_err = num_err + ret
      
      ret = dseslab()
      num_err = num_err + ret
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
      ret = dsrslab(sn, st, sz, st, sa, sr)
      num_err = num_err + ret

      if ( num_err .ne. 0) then
         print *,'number of failures =',num_err
      endif

      print *,'   Verifying data '
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
           print *,'  >>> All tests passed <<< \n '
      endif

C
C
      stop 
      end

