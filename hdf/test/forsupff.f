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
C       verbosity: current verbosity of program
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine VERIFY(errval, routine, num_failed, verbosity)
      integer errval
      character*(*)  routine
      integer num_failed, verbosity

      integer FAIL

      FAIL = -1
      if (errval .eq. FAIL) then
          num_failed = num_failed + 1
          print *, '    >>> ', routine, ' FAILED: ret = ',
     *           errval, '    <<<'
      else 
          if (verbosity .gt. 9) then
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
C       verb_lvl: global verbosity level
C Returns: none
C Users:    HDF Fortran programmers
C Invokes: none
C------------------------------------------------------------------------------
      subroutine MESSAGE(priority, out_str, verb_lvl)
      integer priority
      character*(*)  out_str
      integer verb_lvl

      if (priority .le. verb_lvl) then
          print *, out_str
      endif

      return
      end

