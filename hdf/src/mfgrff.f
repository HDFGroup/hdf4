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
C Name: mgcreat
C Purpose:  Create a new raster image
C Inputs:   
C       grid: GR ID of interface to create image in
C       name: name of raster image
C       ncomp: number of components per pixel
C       nt: number-type of each component
C       il: interlace scheme to use
C       dimsizes[2]: dimensions of the image to create
C Returns: RI ID on success, -1 on failure
C Users:    HDF Fortran programmers
C Invokes: mgicreat
C------------------------------------------------------------------------------

      integer function mgcreat(grid, name, ncomp, nt, il, dimsizes)
      character*(*) name
      integer grid, ncomp, nt, il, dimsizes, mgicreat

      mgcreat = mgicreat(grid, name, ncomp, nt, il, dimsizes,
     +                                              len(name))
      return
      end

C------------------------------------------------------------------------------
C Name: mgn2ndx
C Purpose:  Map the name of a raster image to an index in the file
C Inputs:   
C       grid: GR ID of interface to create image in
C       name: name of raster image
C Returns: index of image on success, -1 on failure
C Users:    HDF Fortran programmers
C Invokes: mgin2ndx
C------------------------------------------------------------------------------

      integer function mgn2ndx(grid, name)
      character*(*) name
      integer grid, mgin2ndx

      mgn2ndx = mgin2ndx(grid, name, len(name))
      return
      end

C------------------------------------------------------------------------------
C Name: mgsxfil
C Purpose:  Convert a standard image into an external image
C Inputs:   
C       riid: RI ID of image to move
C       filename: filename of file to move image into
C       offset: offset in file to move image to
C Returns: SUCCEED/FAIL
C Users:    HDF Fortran programmers
C Invokes: mgisxfil
C-------------------------------------------------------------

      integer function mgsxfil(riid, filename, offset)
      character*(*) filename
      integer riid, mgisxfil, offset

      mgsxfil = mgisxfil(riid, filename, offset, len(filename))
      return
      end

C-------------------------------------------------------------
C Name: mgscatt
C Purpose:  Add a char type attribute to a raster image
C Inputs:   
C       riid: RI ID of image
C       name: the name of the attribute
C       nt: the number-type of the attribute
C       count: the number of values in the attribute
C       data: the data for the attribute
C Returns: SUCCEED/FAIL
C Users:    HDF Fortran programmers
C Invokes: mgiscatt
C-------------------------------------------------------------

      integer function mgscatt(riid, name, nt, count, data)
      character*(*) name
      character*(*) data
      integer riid, mgisattr, nt, count

      mgscatt = mgiscatt(riid, name, nt, count, data, len(name))
      return
      end

C-------------------------------------------------------------
C Name: mgsnatt
C Purpose:  Add a numeric attribute to a raster image
C Inputs:   
C       riid: RI ID of image
C       name: the name of the attribute
C       nt: the number-type of the attribute
C       count: the number of values in the attribute
C       data: the data for the attribute
C Returns: SUCCEED/FAIL
C Users:    HDF Fortran programmers
C Invokes: mgisattr
C-------------------------------------------------------------

      integer function mgsnatt(riid, name, nt, count, data)
      character*(*) name
      integer data
      integer riid, mgisattr, nt, count

      mgsnatt = mgisattr(riid, name, nt, count, data, len(name))
      return
      end

C-------------------------------------------------------------
C Name: mgsattr
C Purpose:  Add an attribute to a raster image
C Inputs:   
C       riid: RI ID of image
C       name: the name of the attribute
C       nt: the number-type of the attribute
C       count: the number of values in the attribute
C       data: the data for the attribute
C Returns: SUCCEED/FAIL
C Users:    HDF Fortran programmers
C Invokes: mgisattr
C-------------------------------------------------------------

      integer function mgsattr(riid, name, nt, count, data)
      character*(*) name
      character*(*) data
      integer riid, mgisattr, nt, count

      mgsattr = mgisattr(riid, name, nt, count, data, len(name))
      return
      end
C---------------------------------------------------------------
C Name: mgfndat
C Purpose:  Locate an attribute for a raster image
C Inputs:   
C       riid: RI ID of image
C       name: the name of the attribute
C Returns: SUCCEED/FAIL
C Users:    HDF Fortran programmers
C Invokes: mgifndat
C------------------------------------------------------------------------------

      integer function mgfndat(riid, name)
      character*(*) name
      integer riid, mgifndat

      mgfndat = mgifndat(riid, name, len(name))
      return
      end

