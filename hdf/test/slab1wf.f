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
      subroutine slab1wf (num_err)
C
C Program writes data set as the first 3 of 5 slabs to slab1wf.hdf
C The program slab2wf writes the last 2 of 5 slabs to slab1wf.hdf 
C 
C Output file: slab1wf.hdf


      integer dssdims, dssdisc, dssdist
      integer dssfill, dssslab, dswslab, dseslab
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    slab1(3,1,1), slab2(3,1,2), slab3(3,2,1)
      real    slab4(3,1,1), slab5(1,3,2)
      real    fill_value
      integer i, j, k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 fn, sn
      integer fnlen
      integer num_err
     
      print *,'\n  Writing the first 3 of 5 slabs to slab1wf.hdf'

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
      sn = 'slab1wf.hdf'
      fill_value = 1.0
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
C    		print *, da(k,j,i)
150	    continue
180      continue
200   continue

      num_err = 0
      ret = dssdims(rank, di)
      num_err = num_err + ret
      ret = dssdist(1, lcol, ucol, fcol)
      num_err = num_err + ret
      ret = dssdist(2, lrow, urow, frow)
      num_err = num_err + ret
      ret = dssdist(3, lpln, upln, fpln)
      num_err = num_err + ret
      ret = dssdisc(1, di(1),  sccol)
      num_err = num_err + ret
      ret = dssdisc(2, di(2), scrow)
      num_err = num_err + ret
      ret = dssdisc(3, di(3), scpln)
      num_err = num_err + ret
C 
C Write it slab by slab   
C 
      ret = dssfill(fill_value)
      num_err = num_err + ret
      sr(1) = 0
      sr(2) = 0
      sr(3) = 0
      ret = dssdims(rank, di)
      num_err = num_err + ret

      ret = dssslab(sn)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 2
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dswslab(st, sr, sz,slab1)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 3
      sz(2) = 2
      sz(3) = 1
      ret = dswslab(st, sr, sz, slab3)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 3
      sz(3) = 2
      ret = dswslab(st, sr, sz, slab5)
      num_err = num_err + ret

      ret = dseslab()
      num_err = num_err + ret

 
      if (num_err .ne. 0) then
	       print *,'number of failures =',num_err
      else
           print *,' >>> All tests passed for slab1wf, run slab2wf <<< '
      endif
      print *,' '

C
      return 
      end

