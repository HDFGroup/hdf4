C---------------------------------------------------------------------------
C $Header$
C
C $Log$
C Revision 1.4  1993/04/27 21:02:11  georgev
C Changed fortran stubs interface for hyperslabs, made them different
C than the C names.
C
c Revision 1.3  1993/02/01  23:23:31  georgev
c Changed hyperslab test files to reflect new interface
c
c Revision 1.2  1993/01/15  16:55:00  georgev
c Cleaned out unused variables.
c
c Revision 1.1  1993/01/07  20:56:25  georgev
c Added Hyperslab C and Fortran test programs.
c
C
C---------------------------------------------------------------------------
C
      program slabwf
C
C Program to write data set as 5 slabs to slabwf.hdf 
C 
C Output file: slabwf.hdf


      integer dssdims, dssdisc, dssdist
      integer dfsdsslab, dfsdwslab, dfsdeslab, dsigslc
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    slab1(3,1,1), slab2(3,1,2), slab3(3,2,1)
      real    slab4(3,1,1), slab5(1,3,2)
      real    sa(4,3,2)
      integer i, j, k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 fn, sn
      integer fnlen
      integer num_err
     
	  print *,'\n   Writing data as 5 slabs to slabwf.hdf'

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
C     		print *, da(k,j,i)
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
C      ret = dsadata(fn, rank, di, da)
C 
C Write it slab by slab   
C 
      sr(1) = 0
      sr(2) = 0
      sr(3) = 0
      ret = dssdims(rank, di)
      num_err = num_err + ret

      ret = dfsdsslab(sn, fnlen)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 2
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab1)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 3
      st(3) = 1
      sz(1) = 3
      sz(2) = 1
      sz(3) = 2
      ret = dfsdwslab(st, sr, sz, slab2)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 3
      sz(2) = 2
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab3)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 2
      sz(1) = 3
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab4)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 3
      sz(3) = 2
      ret = dfsdwslab(st, sr, sz, slab5)
      num_err = num_err + ret

      ret = dfsdeslab()
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
      ret = dsigslc(sn, st, sz, sa, sr, fnlen)
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
C     		 print *, 'da() ',da(k,j,i)
C     		 print *, 'sa() ',sa(k,j,i)
300	  continue
400   continue
500   continue

      if (num_err .ne. 0) then
	       print *,'slabwf: number of wrong values in slab =',num_err
      else
	       print *,'  >>> All tests passed <<< \n'
      endif
C
C
      stop 
      end

