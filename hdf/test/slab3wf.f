C---------------------------------------------------------------------------
C $Header$
C
C $Log$
C Revision 1.5  1993/08/03 16:36:23  georgev
C Fixed problem with slab interface.
C
c Revision 1.4  1993/04/27  21:02:04  georgev
c Changed fortran stubs interface for hyperslabs, made them different
c than the C names.
c
c Revision 1.3  1993/02/01  23:23:23  georgev
c Changed hyperslab test files to reflect new interface
c
c Revision 1.2  1993/01/15  16:54:59  georgev
c Cleaned out unused variables.
c
c Revision 1.1  1993/01/07  20:56:19  georgev
c Added Hyperslab C and Fortran test programs.
c
C
C---------------------------------------------------------------------------
C
      program slab3wf
C
C The program writes the 24 elements of the data set individually
C as slabs to slab3wf.hdf 
C 
C Output file: slab3wf.hdf


      integer dssdims, dssdisc, dssdist
      integer dfsdsslab, dfsdwslab, dfsdeslab, dsgslc
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    slab1(1,1,1), slab2(1,1,1), slab3(1,1,1)
      real    slab4(1,1,1), slab5(1,1,1), slab6(1,1,1) 
      real    slab7(1,1,1), slab8(1,1,1), slab9(1,1,1) 
      real    slab10(1,1,1), slab11(1,1,1), slab12(1,1,1) 
      real    slab13(1,1,1), slab14(1,1,1), slab15(1,1,1) 
      real    slab16(1,1,1), slab17(1,1,1), slab18(1,1,1) 
      real    slab19(1,1,1), slab20(1,1,1), slab21(1,1,1) 
      real    slab22(1,1,1), slab23(1,1,1), slab24(1,1,1)
      real    sa(4,3,2)
      integer i, j, k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 fn, sn
      integer num_err
     
      print *,' '
      print *,'Writing all 24 elements of data as slabs to slab3wf.hdf'

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
      sn = 'slab3wf.hdf'
      slab1(1,1,1) = 0.0
      slab2(1,1,1) = 1.0
      slab3(1,1,1) = 2.0
      slab4(1,1,1) = 3.0
      slab5(1,1,1) = 10.0
      slab6(1,1,1) = 11.0
      slab7(1,1,1) = 12.0
      slab8(1,1,1) = 13.0
      slab9(1,1,1) = 20.0
      slab10(1,1,1) = 21.0
      slab11(1,1,1) = 22.0
      slab12(1,1,1) = 23.0
      slab13(1,1,1) = 100.0
      slab14(1,1,1) = 101.0
      slab15(1,1,1) = 102.0
      slab16(1,1,1) = 103.0
      slab17(1,1,1) = 110.0
      slab18(1,1,1) = 111.0
      slab19(1,1,1) = 112.0
      slab20(1,1,1) = 113.0
      slab21(1,1,1) = 120.0
      slab22(1,1,1) = 121.0
      slab23(1,1,1) = 122.0
      slab24(1,1,1) = 123.0
      
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
C     ret = dsadata(fn, rank, di, da)
C 
C Write it slab by slab   
C 
      sr(1) = 0
      sr(2) = 0
      sr(3) = 0
      ret = dssdims(rank, di)
      num_err = num_err + ret

      ret = dfsdsslab(sn)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 2
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab20)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 3
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab21)
      num_err = num_err + ret
      st(1) = 2
      st(2) = 3
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab22)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 3
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab23)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 3
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab24)
      num_err = num_err + ret

      st(1) = 2
      st(2) = 2
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab6)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 2
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab7)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 2
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab8)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 3
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab9)
      num_err = num_err + ret
      st(1) = 2
      st(2) = 3
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab10)
      num_err = num_err + ret

      st(1) = 4
      st(2) = 1
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab16)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 2
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab17)
      num_err = num_err + ret
      st(1) = 2
      st(2) = 2
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab18)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 2
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab19)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 3
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab11)
      num_err = num_err + ret

      st(1) = 4
      st(2) = 3
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab12)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab13)
      num_err = num_err + ret
      st(1) = 2
      st(2) = 1
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab14)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 1
      st(3) = 2
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab15)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab1)
      num_err = num_err + ret

      st(1) = 2
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab2)
      num_err = num_err + ret
      st(1) = 3
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab3)
      num_err = num_err + ret
      st(1) = 4
      st(2) = 1
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz, slab4)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 2
      st(3) = 1
      sz(1) = 1
      sz(2) = 1
      sz(3) = 1
      ret = dfsdwslab(st, sr, sz,slab5)
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
      ret = dsgslc(sn, st, sz, sa, sr)
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
           print *,'slab3wf: number of wrong values in slab =',num_err
      else
           print *,'  >>> All tests passed <<< \n'
      endif

C
C
      stop 
      end

