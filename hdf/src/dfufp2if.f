C
C $Header$
C
C $Log$
C Revision 1.1  1992/08/25 21:40:44  koziol
C Initial revision
C
C
C------------------------------------------------------------------------------
C File:     DFUfptoimFf.f
C Purpose:  Fortran stub for DFUfptoimage()
C Invokes:  DFUfptoimage.c
C Contents: 
C   duf2im:       call duif2i_ to invoke DFUfptoimage()
C   dfufptoimage: call duif2i_ to invoke DFUfptoimage()
C -----------------------------------------------------------------------------

C------------------------------------------------------------------------------
C Name:     duf2im
C Purpose:   call duif2i_ to invoke DFUfptoimage()
C Inputs:   
C      hdim, vdim: horizontal and vertical dimensions of input data
C      max, min:   maximum and minimum values in the data
C      hscale,vscale: optional horizontal and vertical scales
C      data:       input data
C      palette:    optional palette to be stored with the image
C      outfile:n   name of hdf file to store image in
C      ct_method:  color transform method: 1=EXPAND; 2=INTERP
C      hres, vres: resolutions desired for output image
C      compress:   compression flag: 0=don't; 1=do
C  Returns: 0 on success, -1 on failure with DFerror set
C  Users:       HDF HLL (high-level library) users, utilities, other routines
C  Invokes: process
C  Remarks: none
C----------------------------------------------------------------------------


      integer function duf2im(hdim,vdim,max,min,hscale,vscale,data,
     *                palette,outfile,ct_method,hres,vres,compress)

      integer       hdim, vdim
      real          max, min, hscale, vscale, data
      character*(*) palette
      character*(*) outfile
      integer       ct_method, hres, vres, compress

      duf2im = duif2i(hdim,vdim,max,min,hscale,vscale,data,palette,
     *              outfile,ct_method,hres,vres,compress, len(outfile))        
      return
      end

CEND7MAX

C------------------------------------------------------------------------------
C Name:     dfufptoimage
C Purpose:   call duif2i_ to invoke DFUfptoimage()
C Inputs:   
C      hdim, vdim: horizontal and vertical dimensions of input data
C      max, min:   maximum and minimum values in the data
C      hscale,vscale: optional horizontal and vertical scales
C      data:       input data
C      palette:    optional palette to be stored with the image
C      outfile:n   name of hdf file to store image in
C      ct_method:  color transform method: 1=EXPAND; 2=INTERP
C      hres, vres: resolutions desired for output image
C      compress:   compression flag: 0=don't; 1=do
C  Returns: 0 on success, -1 on failure with DFerror set
C  Users:       HDF HLL (high-level library) users, utilities, other routines
C  Invokes: process
C  Remarks: none
C----------------------------------------------------------------------------


      integer function dfufptoimage(hdim,vdim,max,min,hscale,vscale,
     *           data, palette,outfile,ct_method,hres,vres,compress)

      integer       hdim, vdim
      real          max, min, hscale, vscale, data
      character*(*) palette
      character*(*) outfile
      integer       ct_method, hres, vres, compress

      dfufptoimage = 
     *             duif2i(hdim,vdim,max,min,hscale,vscale,data,palette,
     *              outfile,ct_method,hres,vres,compress, len(outfile))        
      return
      end

