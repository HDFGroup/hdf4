C * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C  Copyright by The HDF Group.                                               *
C  Copyright by the Board of Trustees of the University of Illinois.         *
C  All rights reserved.                                                      *
C                                                                            *
C  This file is part of HDF.  The full HDF copyright notice, including       *
C  terms governing use, modification, and redistribution, is contained in    *
C  the COPYING file, which can be found at the root of the source code       *
C  distribution tree, or in https://support.hdfgroup.org/ftp/HDF/releases/.  *
C  If you do not have access to either file, you may request a copy from     *
C  help@hdfgroup.org.                                                        *
C * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
C
C
C------------------------------------------------------------------------------
C File:     forsupff.f
C Purpose:  Fortran support routines for Fortran test routines
C Invokes:  Nothing currently...
C Contents:
C Remarks: none
C------------------------------------------------------------------------------


C------------------------------------------------------------------------------
C Name: RESULT
C Purpose:  Report on success of HDF routines
C Inputs:
C       errval: value to check for error
C       routine: name of routine tested
C       num_failed: running sum of the number of failures
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine RESULT(errval, routine, num_failed)
      implicit none
      include 'fortest.inc'

      integer errval
      character*(*)  routine
      integer num_failed

      if (errval .eq. FAIL) then
          num_failed = num_failed + 1
          print *, '    >>> ', routine, ' FAILED: ret = ',
     *           errval, '    <<<'
      else
          print *, routine, ' SUCCESSFUL'
      endif

      return
      end

C------------------------------------------------------------------------------
C Name: VRFY
C Purpose:  Report on success of HDF routines, using verbosity
C Inputs:
C       errval: value to check for error
C       routine: name of routine tested
C       num_failed: running sum of the number of failures
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine VRFY(errval, routine, num_failed)
      implicit none
      include 'fortest.inc'

      integer errval
      character*(*)  routine
      integer num_failed

      if (errval .eq. FAIL) then
          num_failed = num_failed + 1
          print *, '    >>> ', routine, ' FAILED: ret = ',
     *           errval, '    <<<'
      else
          if (verbosity .ge. VERBO_HI) then
              print *, routine, ' SUCCESSFUL'
          endif
      endif

      return
      end

C------------------------------------------------------------------------------
C Name: MESSAGE
C Purpose:  Print something, depending on the verbosity level
C Inputs:
C       priority: priority of message (lower values have higher priority)
C       out_str: string to output
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine MESSAGE(priority, out_str)
      implicit none
      include 'fortest.inc'

      integer priority
      character*(*)  out_str

      if (priority .le. Verbosity) then
          print *, out_str
      endif

      return
      end


C***************************************************************
C
C  gen2Dfloat:  generate 2-D data array
C
C***************************************************************
      subroutine gen2Dfloat(height, width, data)
      implicit none
      include 'fortest.inc'

      integer   height, width
      real data(height,width)

      integer i, j

C     store one value per row, increasing by one for each row
      do 110 i=1, height
          do 100 j=1, width
             data(i, j) = float(i)
  100     continue
  110 continue
      return
      end


C***************************************************************
C
C  genimage:  generate image from 2-D float array
C
C***************************************************************
      subroutine genimage(height, width, data, image)
      implicit none
      include 'fortest.inc'

      integer   height, width
      real      data(height, width)
      character image(height, width)

      integer i, j
      real   max, min, multiplier

      max = data(1,1)
      min = data(1,1)
      do 110 i=1, height
          do 100 j=1, width
             if (max .gt. data(i,j)) max = data(i,j)
             if (min .lt. data(i,j)) min = data(i,j)
  100     continue
  110 continue

C     store one value per row, increasing by one for each row
      multiplier = 255.0 /(max-min)
      do 210 i=1, height
          do 200 j=1, width
             image(i,j) = char( int((data(i,j)-min) * multiplier) )
  200     continue
  210 continue
      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE errchkio
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine errchkio(err1, err2, err3, num_err, msg)
      implicit none
      include 'fortest.inc'

      integer err1, err2, err3, num_err
      character*(*)  msg

      if (err1.eq.FAIL .or. err2.eq.FAIL .or. err3.eq.FAIL) then
          num_err = num_err + 1
          print *
          print *,'>>> Test failed for ',msg, ' <<<'
          print *, '  err1=',err1, '   err2=',err2, '   err3=',err3
      else
          if (verbosity .ge. VERBO_HI) then
            print *,'Test passed for ', msg
          endif
      endif

      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE errchkarr
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine errchkarr(err1, err2, err3, num_err, type)
      implicit none
      include 'fortest.inc'

      integer err1, err2, err3, num_err
      character*(*)  type

      if (err1 .eq. 1) then
        print *, '>>> Test failed for ', type, ' array'
        num_err = num_err + 1
      else
          if (verbosity .ge. VERBO_HI) then
            print *, 'Test passed for ', type, ' array'
          endif
      endif

      if (err2 .eq. 1) then
        print *, '>>> Test failed for ',type, ' scales.'
        num_err = num_err + 1
      else
          if (verbosity .ge. VERBO_HI) then
            print *, 'Test passed for ', type, ' scales.'
          endif
      endif

      if (err3 .eq. 1) then
        print *, '>>> Test failed for ', type, ' max/min.'
        num_err = num_err + 1
      else
          if (verbosity .ge. VERBO_HI) then
            print *, 'Test passed for ', type, ' max/min.'
          endif
      endif

      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE err_check
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine err_check(err, num_err, type)
      implicit none
      include 'fortest.inc'

      integer err, num_err
      character*(*) type

      if (err .eq. 1) then
        print *,'>>> Test failed for ',type, ' array.'
        num_err = num_err+1
      else
          if (verbosity .ge. VERBO_HI) then
            print *,'Test passed for ', type, ' array.'
          endif
      endif

      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE ptestban
C     Print the Test banner
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine ptestban(action, name)
      implicit none
      include 'fortest.inc'

      character*(*) action, name

      if (Verbosity .ge. VERBO_DEF) then
      call MESSAGE(VERBO_LO,
     +     '=====================================')
      print *, action, ' -- ', name
      call MESSAGE(VERBO_LO,
     +     '=====================================')
      endif

      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE hsystem(cmd)
C     Invoke the unix system() function
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine hsystem(cmd)
      implicit none

      character*(*) cmd
      integer retcode, hisystem

      retcode = hisystem(cmd, len(cmd))
      return
      end


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE fixname(name, name_out, name_out_len)
C     Takes care of srcdir build
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine fixname(name, name_out, name_out_len)
      implicit none

      character*(*)  name
      character*(*)  name_out
      integer retcode, fixnamec, name_out_len

      retcode = fixnamec(name, len(name), name_out, name_out_len)
      return
      end
