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
C File:     hfileFf.f
C Purpose:  Fortran stubs for Palette Fortran routines
C Invokes:  hfileF.c 
C Contents: 
C   hopen:          Call hiopen to open file
C hnumber:          Call hnumber
C Remarks: none
C----------------------------------------------------------------------------*/


C------------------------------------------------------------------------------
C Name: hopen
C Purpose:  call hiopen, open file
C Inputs:   path: Name of file to be opened
C           access: DFACC_READ, DFACC_WRITE, DFACC_CREATE,
C                      or any bitwise-or of the above.
C           ndds: Number of dds in header block if file needs to be created.
C Returns: 0 on success, FAIL on failure with error set
C Users:    Fortran stub routine
C Invokes: hiopen
C----------------------------------------------------------------------------*/

      integer function hopen(filename, access, defdds)

      character*(*) filename
      integer       access, defdds, hiopen

      hopen = hiopen(filename, access, defdds, len(filename))
      return
      end

C------------------------------------------------------------------------------
C Name: hxsdir
C Purpose:  call hxisdir to set directory variable for locating an external file
C Inputs:   dir: names of directory separated by colons.
C Returns:  SUCCEED if no error, else FAIL
C Users:    Fortran stub routine
C Invokes: hxisdir
C----------------------------------------------------------------------------*/

      integer function hxsdir(dir)

      character*(*) dir

      hxsdir = hxisdir(dir, len(dir))
      return
      end

C------------------------------------------------------------------------------
C Name: hxscdir
C Purpose:  call hxiscdir to set directory variable for creating an external file
C Inputs:   dir: name of the directory
C Returns:  SUCCEED if no error, else FAIL
C Users:    Fortran stub routine
C Invokes: hxiscdir
C----------------------------------------------------------------------------*/

      integer function hxscdir(dir)

      character*(*) dir

      hxscdir = hxiscdir(dir, len(dir))
      return
      end

