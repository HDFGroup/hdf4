C---------------------------------------------------------------------------
C $Header$
C
C $Log$
C Revision 1.4  1993/08/03 16:36:24  georgev
C Fixed problem with slab interface.
C
c Revision 1.3  1993/04/27  21:02:09  georgev
c Changed fortran stubs interface for hyperslabs, made them different
c than the C names.
c
c Revision 1.2  1993/02/01  23:23:27  georgev
c Changed hyperslab test files to reflect new interface
c
c Revision 1.1  1993/01/07  20:56:22  georgev
c Added Hyperslab C and Fortran test programs.
c
C
C---------------------------------------------------------------------------
C
      program slab4wf
C
C Program to write a data set as 1 slab to slab4wf.hdf.
C 
C Output file: slab4wf.hdf
C


      integer dssdims,  dssdisc, dssdist
      integer dfsdsslab, dfsdwslab, dfsdeslab, dsgslc
      integer ret, np, nr,nc, di(3), st(3), sz(3), sr(3)
      integer rank, DFTAG_SDT, DFO_FORTRAN
      real    scpln(2), scrow(3), sccol(4), da(4,3,2)
      real    sa(4,3,2)
      integer i, j, k
      character*10 lcol,ucol,fcol,lrow,urow,frow,lpln,upln,fpln
      character*30 fn, sn
      integer num_err
     
      print *,' '
      print *,'Writing data as 1 slab to slab4wf.hdf'

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
      sn = 'slab4wf.hdf'
      do 200 i=1, np
         do 180 j=1, nr
            do 150 k=1, nc
		    da(k,j,i) = scpln(i) + scrow(j) + sccol(k)
C    		print *, da(k,j,i)
150	  continue
180   continue
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
C Write it as one slab    
C 

      ret = dfsdsslab(sn)
      num_err = num_err + ret
      st(1) = 1
      st(2) = 1
      st(3) = 1
      sz(1) = 4
      sz(2) = 3
      sz(3) = 2
      ret = dfsdwslab(st, sr, sz, da)
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

