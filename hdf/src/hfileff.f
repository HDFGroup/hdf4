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

