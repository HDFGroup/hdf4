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
      integer errval
      character*(*)  routine
      integer num_failed

      integer FAIL

      FAIL = -1
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
C Name: VERIFY
C Purpose:  Report on success of HDF routines, using verbosity
C Inputs:   
C       errval: value to check for error
C       routine: name of routine tested
C       num_failed: running sum of the number of failures
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine VERIFY(errval, routine, num_failed)
      implicit none
      include "fortest.inc"

      integer errval
      character*(*)  routine
      integer num_failed

      integer FAIL

      FAIL = -1
      if (errval .eq. FAIL) then
          num_failed = num_failed + 1
          print *, '    >>> ', routine, ' FAILED: ret = ',
     *           errval, '    <<<'
      else 
          if (verbosity .ge. 9) then
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
      include "fortest.inc"

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
      integer   height, width
      real      data(height, width)
      character image(height, width)

      integer i
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
      integer err1, err2, err3, num_err
      character*(*)  msg

      integer FAIL

      FAIL = -1

      if (err1.eq.FAIL .or. err2.eq.FAIL .or. err3.eq.FAIL) then
          num_err = num_err + 1
          print *
          print *,'>>> Test failed for ',msg, ' <<<'
          print *, '  err1=',err1, '   err2=',err2, '   err3=',err3
      else
          print *,'Test passed for ', msg
      endif
      print *

      return
      end

      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE errchkarr
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine errchkarr(err1, err2, err3, num_err, type)
      integer err1, err2, err3, num_err
      character*(*)  type
      
      print *
      if (err1 .eq. 1) then
        print *, '>>> Test failed for ', type, ' array' 
        num_err = num_err + 1
      else
        print *, 'Test passed for ', type, ' array'
      endif

      if (err2 .eq. 1) then
        print *, '>>> Test failed for ',type, ' scales.'
        num_err = num_err + 1
      else
        print *, 'Test passed for ', type, ' scales.'
      endif

      if (err3 .eq. 1) then
        print *, '>>> Test failed for ', type, ' max/min.'
        num_err = num_err + 1
      else
        print *, 'Test passed for ', type, ' max/min.'
      endif

      print *

      return
      end

       
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     SUBROUTINE err_check
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      subroutine err_check(err, num_err, type)
      integer err, num_err
      character*(*) type

      if (err .eq. 1) then 
  	print *,'>>> Test failed for ',type, ' array.'
        num_err = num_err+1
      else
  	print *,'Test passed for ', type, ' array.'
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
      include "fortest.inc"

      character*(*) action, name

      if (Verbosity .ge. VERBO_DEF) then
	print *, "====================================="
	print *, action, ' ', name
	print *, "====================================="
      endif

      return
      end
