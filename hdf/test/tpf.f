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
      program tdfpF
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

      integer dpppal, dpapal, dprest, dpgpal, dpnpals
      integer dprref, dpwref
      integer dplref

      character*64 TESTFILE
      character*1 CR
      character pal1(768), pal2(768), ipal(768)
      integer ret 
      integer*2  ref1, ref2
      integer i, number_failed


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

      Print *, 'Putting pal1 in new file.'
      ret = dpppal(TESTFILE, pal1, 0, 'w')
      call RESULT(ret, 'dpppal')

      print *, 'Getting ref1'
      ref1 = dplref()
C     call RESULT(ref1, 'dplref')
      print *, 'ref1 is ', ref1

      print *, 'Putting pal2 in file'
      ret = dpapal(TESTFILE, pal2)
      call RESULT(ret, 'dpapal')

      print *, 'Getting ref2'
      ref2 = dplref()
C      call RESULT(ref2, 'dplref')
      print *, 'ref2 is ', ref2
     
      print *, 'Restarting palette interface'
      ret = dprest()
      call RESULT(ret, 'dprest')

      print *, 'Reading pal1'
      ret = dpgpal(TESTFILE, ipal)
      call RESULT(ret, 'dpgpal')
      do 200 i=1, 768
          if (ipal(i) .ne. pal1(i))  then
              print *, 'Error at ', i, ', ipal:', ipal(i), 
     *                 '      pal1(i):', pal1(i)
          endif
200   continue
      
      print *, 'Getting ref1'
      ref1 =  dplref()
C      call RESULT(ref1, 'dplref')
      print *, 'Last ref is ', ref1

      print *, 'Reading pal2.'
      ret = dpgpal(TESTFILE, ipal)
      call RESULT(ret, 'dpgpal')
      do 300 i=1, 768
          if (ipal(i) .ne. pal2(i)) then
              print *, 'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal2:', pal2(i)
          endif
300   continue

      print *, 'Getting ref2'
      ref2 = dplref()
C      call RESULT(ref2, 'dplref')
      print *, 'Last ref is ', ref2

      print *, 'Getting number of palettes'
      ret = dpnpals(TESTFILE)
      call RESULT(ret, 'dpnpals')
      print *, 'Number of palettes is:', ret

      print *, 'Setting read ref to ref2.'
      ret = dprref(TESTFILE, ref2)
      call RESULT(ret, 'dprref')
      
      print *, 'Reading pal2'
      ret = dpgpal(TESTFILE, ipal)
      call RESULT(ret, 'dpgpal')
      do 400 i=1, 768
          if (ipal(i) .ne. pal2(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal2:', pal2(i)
          endif
400   continue

      print *, 'Setting read ref to ref1.'
         print *, 'ref1 is: ', ref1, ' ref2 is: ',ref2
      ret = dprref(TESTFILE, ref1)

      call RESULT(ret, 'dprref')
      
      print *, 'Reading pal1'
      ret = dpgpal(TESTFILE, ipal)
      call RESULT(ret, 'dpgpal')

      do 500 i=1, 768
          if (ipal(i) .ne. pal1(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal1:', pal1(i)
          endif
500   continue

      print *, 'Modifying pal1'
      do 600 i=1,256
          pal1(i+256) = char(256-i)
600   continue

      print *, 'Setting write ref to ref1'
      ret = dpwref(TESTFILE, ref1)
      call RESULT(ret, 'dpwref')
      print *, 'Writing pal1'
      ret = dpppal(TESTFILE, pal1, 1, 'a')
      call RESULT(ret, 'dpppal')
      ret=dplref()
      print *,'last ref is: ', ret
      print *, 'setting read ref to ref1'
      ret = dprref(TESTFILE, ref1)
      call RESULT(ret, 'dprref')
      print *, 'Reading pal1'
      ret = dpgpal(TESTFILE, ipal)
      call RESULT(ret, 'dpgpal')
      do 700 i=1, 768
          if (ipal(i) .ne. pal1(i)) then
              print *,  'Error at ', i, ', ipal:', ipal(i),
     *                 '      pal1:', pal1(i)
          endif
700   continue

      print *, CR, CR
      if (number_failed .ne. 0) then
          print *, '***** ', number_failed, ' TEST FAILED '
      else
          print *, '***** ALL TESTS SUCCESSFUL *****'
      endif

      stop
      end




C*******************************************
C
C   RESULT
C
C*******************************************

      subroutine RESULT(errval, routine)
      integer errval
      character*(*)  routine

      integer FAIL

      FAIL = -1
      if (errval .eq. FAIL) then
         number_failed = number_failed + 1
         print *, '    >>>', routine, ' FAILED: ret = ',
     *               errval, '   <<<'
      else
         print *, routine, '  SUCCESSFUL'
      endif
      return
      end

