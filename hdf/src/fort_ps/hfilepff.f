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
C File:     hfilepFf.f, based on hfileff.f,v 1.7
C Purpose:  Fortran stubs for H Fortran PowerStation routines
C Invokes:  hfilepF.c 
C Contents: 
C   hopen:          Call hiopen to open file
C hnumber:          Call hnumber
C Remarks: none
C--------------------------------------------------------------


C------------------------------------------------------------------
C Name: hopen
C Purpose:  call hiopen, open file
C Inputs:   path: Name of file to be opened
C           access: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
C                      or any bitwise-or of the above.
C           ndds: Number of dds in header block if file needs to be created.
C Returns: 0 on success, FAIL on failure with error set
C Users:    Fortran stub routine
C Invokes: hiopen
C-------------------------------------------------------------------

      integer function hopen(filename, access, defdds)

      character*(*) filename
      integer       access, defdds
C      integer       hiopen
      INTERFACE 
        INTEGER FUNCTION hiopen(filename,access, defdds, nmlen)
          !MS$ATTRIBUTES C, reference, alias: '_HIOPEN' :: hiopen
          integer access, defdds, nmlen
          character*(*) filename
        END FUNCTION hiopen
      END INTERFACE

      hopen = hiopen(filename, access, defdds, len(filename))
      return
      end

C------------------------------------------------------------------
C Name: hclose
C Purpose:  call hiclose, close file
C           fid:  handle to HDF file to close
C Returns: 0 on success, FAIL on failure with error set
C Users:    Fortran stub routine
C Invokes: hiclose
C-------------------------------------------------------------------

      integer function hclose(fid)

      integer       fid
C      integer       hiclose
      INTERFACE 
        INTEGER FUNCTION hiclose(fid)
          !MS$ATTRIBUTES C, reference, alias: '_HICLOSE' :: hiclose
          integer fid
        END FUNCTION hiclose
      END INTERFACE

      hclose = hiclose(fid)
      return
      end

C------------------------------------------------------------------
C Name: hnumber
C Purpose:  call hinumbr, get number of elements with tag 
C           fid:  handle to HDF file to close
C           tag: the tag which the elements have
C Returns: number of element on success, FAIL on failure with error set
C Users:    Fortran stub routine
C Invokes: hinumbr
C-------------------------------------------------------------------

      integer function hnumber(fid, tag)

      integer       fid, tag
C      integer       hinumbr
      INTERFACE 
        INTEGER FUNCTION hinumbr(fid, tag)
          !MS$ATTRIBUTES C, reference, alias: '_HINUMBR' :: hinumbr
          integer fid, tag
        END FUNCTION hinumbr
      END INTERFACE

      hnumber = hinumbr(fid, tag)
      return
      end

C--------------------------------------------------------------------
C Name: hxsdir
C Purpose:  call hxisdir to set directory variable for locating an external file
C Inputs:   dir: names of directory separated by colons.
C Returns:  SUCCEED if no error, else FAIL
C Users:    Fortran stub routine
C Invokes: hxisdir
C------------------------------------------------------------------

      integer function hxsdir(dir)

      character*(*) dir
C      integer       hxisdir
      INTERFACE
        INTEGER FUNCTION hxisdir(dir, dirlen)
          !MS$ATTRIBUTES C, reference, alias: '_HXISDIR' :: hxisdir
          character*(*) dir
          integer dirlen
        END FUNCTION hxisdir
      END INTERFACE

      hxsdir = hxisdir(dir, len(dir))
      return
      end

C---------------------------------------------------------------------
C Name: hxscdir
C Purpose:  call hxiscdir to set directory variable for creating an external file
C Inputs:   dir: name of the directory
C Returns:  SUCCEED if no error, else FAIL
C Users:    Fortran stub routine
C Invokes: hxiscdir
C--------------------------------------------------------------*/

      integer function hxscdir(dir)

      character*(*) dir
C      integer       hxiscdir
      INTERFACE
        INTEGER FUNCTION hxiscdir(dir, dirlen)
          !MS$ATTRIBUTES C, reference, alias: '_HXISCDIR' :: hxiscdir
          character*(*) dir
          integer dirlen
        END FUNCTION hxiscdir
      END INTERFACE

      hxscdir = hxiscdir(dir, len(dir))
      return
      end

