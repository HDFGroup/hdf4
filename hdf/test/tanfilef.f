C
C $Header$
C
C $Log$
C Revision 1.3  1992/05/06 23:03:22  sxu
C changed hiopen to hopen and hiclose to hclose
C hopen returns fid (not ret)
C
c Revision 1.2  1992/04/27  20:49:14  koziol
c Changed hopen and hclose calls to hiopen and hiclose stub routine calls
c
c Revision 1.1  1992/04/27  17:07:49  sxu
c Initial revision
c
C
      program tdfanfileF
C
C
C  Test program: 
C                Writes file labels and descriptions in a file.
C                Reads the  labels and descriptions from the file
C
C  Input file:  none
C  Output files: tdfanflF.hdf
C
C  Possible bug:  When reading in a label, we have to give it a 
C                 length that is one greater than MAXLEN_LAB. This
C                 may be due to a bug in dfan.c in DFANIgetann().
C

      integer daafid, daafds,dagfidl,dagfid
      integer dagfdsl, dagfds, hopen, hclose
      integer ret, number_failed
      integer ISFIRST, NOFIRST, MAXLEN_LAB, MAXLEN_DESC
      integer fid, DFACC_CREATE, DFACC_READ

      character*30 lab1
      character*30 lab2, templab
      character*100 desc1, desc2, tempstr
      character*64 TESTFILE
      character*1 CR
      character*1 NULL

      ISFIRST = 1
      NOFIRST = 0
      number_failed = 0
      TESTFILE = 'tdfanflF.hdf'
      CR = char(10)
      MAXLEN_LAB = 50
      MAXLEN_DESC = 1000
      DFACC_CREATE = 4
      DFACC_READ = 1
      NULL = char(0)

      lab1 = 'File label #1: aaa'
      lab2 = 'File label #2: bbbbbb'
      desc1 = 'File descr #1: This is a test on file annotation'
      desc2 = 'File descr #2: One more test ...'

      print *, '****** Write file labels *******'
      fid = hopen(TESTFILE, DFACC_CREATE, 0)
      call RESULT(fid, 'hopen')
      ret = daafid(fid, lab1)
      call RESULT(ret, 'daafid')

      ret = daafid(fid, lab2)
      call RESULT(ret, 'daafid')

      print *, '****** Write file descriptions *******'
      ret = daafds(fid, desc1, len(desc1))
      call RESULT(ret, 'daafds')

      ret = daafds(fid, desc2, len(desc2))
      call RESULT(ret, 'daafds')

      ret = hclose(fid)
      call RESULT(ret, 'hclose')

      print *, '****** Read length of the first file label ****'
      fid = hopen(TESTFILE, DFACC_READ, 0)
      call RESULT(fid, 'hopen-read')
      ret = dagfidl(fid, ISFIRST)
      call RESULT(ret, 'dagfidl')
      call checklen(ret, lab1,  'label'  )

      print *, '******...followed by the label *****'
      ret = dagfid(fid, templab, MAXLEN_LAB, ISFIRST)
      call RESULT(ret, 'dagfid')
      call checkann(lab1, templab, ret, 'label')


      print *, '****** Read length of the second file label ****'
      ret = dagfidl(fid, NOFIRST)
      call RESULT(ret, 'dagfidl')
      call checklen(ret, lab2, 'label')

      print *, '******...followed by the label *****'
      ret = dagfid(fid, templab, MAXLEN_LAB, NOFIRST)
      call RESULT(ret, 'dagfid')
      call checkann(lab2, templab, ret, 'label')

      print *, '****** Read length of the first file description ****'
      ret = dagfdsl(fid, ISFIRST)
      call RESULT(ret, 'dagfdsl')
      call checklen(ret, desc1, 'description' )

      print *, '******...followed by the description *****'
      ret = dagfds(fid, tempstr, MAXLEN_DESC, ISFIRST)
      call RESULT(ret, 'dagfds')
      call checkann(desc1, tempstr, ret, 'description')


      print *, '****** Read length of the second file description ****'
      ret = dagfdsl(fid, NOFIRST)
      call RESULT(ret, 'dagfdsl')
      call checklen(ret, desc2, 'description' )

      print *, '******...followed by the description *****'
      ret = dagfds(fid, tempstr, MAXLEN_DESC, NOFIRST)
      call RESULT(ret, 'dagfds')
      call checkann(desc2, tempstr, ret, 'description')
     
      ret = hclose(fid)
      call RESULT(ret, 'hclose')
 
      print *, CR, CR

      if (number_failed .eq. 0) then
         print *, '***** ALL TESTS SUCCESSFUL ******'
      else
         print *, '********', number_failed, ' TESTS FAILED'
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

C*********************************************
C
C  checklen
C
C*********************************************

      subroutine checklen(ret, oldstr, type)
      character*(*) type, oldstr 
      integer ret

      integer oldlen

      oldlen = len(oldstr) 
      if (ret .ge. 0 .and.  ret .ne. oldlen) then
          print *, 'Length of ', type, ' is ', len(oldstr),
     *             ' instead of ', ret
      endif
      return
      end

C***********************************************
C
C  checkann
C
C***********************************************

      subroutine checkann(oldstr, newstr, ret, type)
      character*(*) oldstr, newstr, type
      integer ret


      if (ret .ge. 0 .and. oldstr .ne. newstr) then
          print *, type, ' is incorrect.'
          print *, ' It should be ', oldstr
          print *, ' instead of ', newstr
      endif
      return 
      end
