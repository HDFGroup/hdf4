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
      subroutine tpf (number_failed)
      implicit none
      include "fortest.inc"
C
C
C Test program: Writes palettes in a file.
C               Reads palettes from the file.
C               Writes palette with specified reference number.
C               Reads palette with specified reference number.
C
C Input file: none
C
C Output file: tpalf.hdf
C
C

      integer number_failed
      character*(*) myname
      parameter (myname = "p")

      integer dpppal, dpapal, dprest, dpgpal, dpnpals
      integer dprref, dpwref
      integer dplref

      character*64 TESTFILE
      character*1 CR
      character pal1(768), pal2(768), ipal(768)
      integer ret 
      integer*2  ref1, ref2
      integer i


      call ptestban("Testing", myname)
      TESTFILE = 'tpalf.hdf'
      CR = char(10)
      number_failed = 0

      do 100 i = 0, 255
          pal1(3*i + 1) = char(i)
          pal1(3*i + 2) = char(i) 
          pal1(3*i + 3) = char(i)
          pal2(i + 1) = char(i) 
          pal2(i + 256 + 1) = char(i) 
          pal2(i + 512 + 1) = char(i) 
100   continue

      call MESSAGE(VERBO_HI, 'Putting pal1 in new file.')
      ret = dpppal(TESTFILE, pal1, 0, 'w')
      call VERIFY(ret, 'dpppal', number_failed)

      call MESSAGE(VERBO_HI, 'Getting ref1')
      ref1 = dplref()
C     call VERIFY(ref1, 'dplref', number_failed)
      print *, 'ref1 is ', ref1

      call MESSAGE(VERBO_HI, 'Putting pal2 in file')
      ret = dpapal(TESTFILE, pal2)
      call VERIFY(ret, 'dpapal', number_failed)

      call MESSAGE(VERBO_HI, 'Getting ref2')
      ref2 = dplref()
C      call VERIFY(ref2, 'dplref', number_failed)
      print *, 'ref2 is ', ref2
     
      call MESSAGE(VERBO_HI, 'Restarting palette interface')
      ret = dprest()
      call VERIFY(ret, 'dprest', number_failed)

      call MESSAGE(VERBO_HI, 'Reading pal1')
      ret = dpgpal(TESTFILE, ipal)
      call VERIFY(ret, 'dpgpal', number_failed)
      do 200 i=1, 768
          if (ipal(i) .ne. pal1(i))  then
              print *, 'Error at ', i, ', ipal:', ipal(i), 
     *                 '      pal1(i):', pal1(i)
          endif
200   continue
      
      call MESSAGE(VERBO_HI, 'Getting ref1')
      ref1 =  dplref()
C      call VERIFY(ref1, 'dplref', number_failed)
      print *, 'Last ref is ', ref1

      call MESSAGE(VERBO_HI, 'Reading pal2.')
      ret = dpgpal(TESTFILE, ipal)
      call VERIFY(ret, 'dpgpal', number_failed)
      do 300 i=1, 768
          if (ipal(i) .ne. pal2(i)) then
              print *, 'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal2:', pal2(i)
          endif
300   continue

      call MESSAGE(VERBO_HI, 'Getting ref2')
      ref2 = dplref()
C      call VERIFY(ref2, 'dplref', number_failed)
      print *, 'Last ref is ', ref2

      call MESSAGE(VERBO_HI, 'Getting number of palettes')
      ret = dpnpals(TESTFILE)
      call VERIFY(ret, 'dpnpals', number_failed)
      print *, 'Number of palettes is:', ret

      call MESSAGE(VERBO_HI, 'Setting read ref to ref2.')
      ret = dprref(TESTFILE, ref2)
      call VERIFY(ret, 'dprref', number_failed)
      
      call MESSAGE(VERBO_HI, 'Reading pal2')
      ret = dpgpal(TESTFILE, ipal)
      call VERIFY(ret, 'dpgpal', number_failed)
      do 400 i=1, 768
          if (ipal(i) .ne. pal2(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal2:', pal2(i)
          endif
400   continue

      call MESSAGE(VERBO_HI, 'Setting read ref to ref1.')
      ret = dprref(TESTFILE, ref1)

      call VERIFY(ret, 'dprref', number_failed)
      
      call MESSAGE(VERBO_HI, 'Reading pal1')
      ret = dpgpal(TESTFILE, ipal)
      call VERIFY(ret, 'dpgpal', number_failed)

      do 500 i=1, 768
          if (ipal(i) .ne. pal1(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal1:', pal1(i)
          endif
500   continue

      call MESSAGE(VERBO_HI, 'Modifying pal1')
      do 600 i=1,256
          pal1(i+256) = char(256-i)
600   continue

      call MESSAGE(VERBO_HI, 'Setting write ref to ref1')
      ret = dpwref(TESTFILE, ref1)
      call VERIFY(ret, 'dpwref', number_failed)
      call MESSAGE(VERBO_HI, 'Writing pal1')
      ret = dpppal(TESTFILE, pal1, 1, 'a')
      call VERIFY(ret, 'dpppal', number_failed)
      ret=dplref()
      print *,'last ref is: ', ret
      call MESSAGE(VERBO_HI, 'setting read ref to ref1')
      ret = dprref(TESTFILE, ref1)
      call VERIFY(ret, 'dprref', number_failed)
      call MESSAGE(VERBO_HI, 'Reading pal1')
      ret = dpgpal(TESTFILE, ipal)
      call VERIFY(ret, 'dpgpal', number_failed)
      do 700 i=1, 768
          if (ipal(i) .ne. pal1(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal1:', pal1(i)
          endif
700   continue

      if (number_failed .eq. 0) then 
          call MESSAGE(VERBO_DEF + 1,
     +		'****** ALL TESTS SUCCESSFUL ******')
      else
          print *, '****** ', number_failed, ' TESTS FAILES  ******'
      endif

      return
      end


