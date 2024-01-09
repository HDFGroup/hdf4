      program  write_palette
      implicit none
C
C     Parameter declaration
C
      character*22 FILE_NAME
      character*18 NEW_IMAGE_NAME
      integer      X_LENGTH
      integer      Y_LENGTH
      integer      N_ENTRIES
      integer      N_COMPS_IMG
      integer      N_COMPS_PAL
C
      parameter (FILE_NAME       = 'Image_with_Palette.hdf',
     +           NEW_IMAGE_NAME  = 'Image with Palette',
     +           X_LENGTH        = 5,
     +           Y_LENGTH        = 5,
     +           N_ENTRIES       = 256,
     +           N_COMPS_IMG     = 2,
     +           N_COMPS_PAL     = 3)
      integer DFACC_CREATE, DFNT_CHAR8, DFNT_UINT8, MFGR_INTERLACE_PIXEL
      parameter (DFACC_CREATE = 4,
     +           DFNT_CHAR8   = 4,
     +           DFNT_UINT8   = 21,
     +           MFGR_INTERLACE_PIXEL = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgcreat, mgwcimg, mggltid, mgwclut, 
     +        mgendac, mgend 
C
C**** Variable declaration *******************************************
C
      integer    file_id, gr_id, ri_id, pal_id
      integer    interlace_mode
      integer    start(2), stride(2), edges(2), dim_sizes(2)
      integer    status
      integer    i, j
      character  image_buf(N_COMPS_IMG, X_LENGTH, Y_LENGTH) 
      character  palette_buf(N_COMPS_PAL, N_ENTRIES)
C
C**** End of variable declaration ************************************
C
C
C     Create and open the file.
C
      file_id = hopen(FILE_NAME, DFACC_CREATE, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Define interlace mode and dimensions of the image.  
C
      interlace_mode = MFGR_INTERLACE_PIXEL
      dim_sizes(1) = X_LENGTH
      dim_sizes(2) = Y_lENGTH
C
C     Create the raster image array. 
C
      ri_id = mgcreat(gr_id, NEW_IMAGE_NAME, N_COMPS_IMG, DFNT_CHAR8,
     +                interlace_mode, dim_sizes)
C
C     Fill the image data buffer with values. 
C
      do 20 i = 1, Y_LENGTH
         do 10 j = 1, X_LENGTH
               image_buf(1,j,i) = char(i + j - 1 )
               image_buf(2,j,i) = char(i + j) 
10       continue
20    continue

C     
C     Define the size of the data to be written, i.e., start from the origin
C     and go as long as the length of each dimension.
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Write the data in the buffer into the image array.
C
      status = mgwcimg(ri_id, start, stride, edges, image_buf)
C
C     Initilaize the palette buffer to grayscale.
C
      do 40 i = 1, N_ENTRIES
          do 30 j = 1, N_COMPS_PAL
             palette_buf(j,i) = char(i)
30        continue
40    continue 
C
C     Get the identifier of the palette attached to the image NEW_IMAGE_NAME.
C
      pal_id = mggltid(ri_id, 0)
C
C     Set palette interlace mode.
C
      interlace_mode = MFGR_INTERLACE_PIXEL
C
C     Write data to the palette.
C
      status = mgwclut(pal_id, N_COMPS_PAL, DFNT_UINT8, interlace_mode,
     +                 N_ENTRIES, palette_buf)
C
C     Terminate access to the raster image and to the GR interface,
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end
