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
C File:     mfgrff.f
C Purpose:  Fortran stubs for Fortran GR routines
C Invokes:  mfgrf.c
C Contents: 
C   dsgdims:        get dimensions of next SDG
C Remarks: none
C------------------------------------------------------------------------------



C------------------------------------------------------------------------------
C Name: dsgdims
C Purpose:  get dimensions of next SDG
C Inputs:   filename: name of HDF file
C           rank: integer to return rank in
C           dimsizes: array to return dimensions in
C           maxrank: size of array dimsizes
C Returns: 0 on success, -1 on failure with DFerror set
C Users:    HDF Fortran programmers
C Invokes: dsigdim
C------------------------------------------------------------------------------

C      integer function dsgdims(filename, rank, dimsizes, maxrank)
C      character*(*) filename
C      integer rank, dimsizes, maxrank, dsigdim
C
C      dsgdims = dsigdim(filename, rank, dimsizes, maxrank,
C     +                                              len(filename))
C
C      return
C      end
