      program create_raster_image
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*13 IMAGE_NAME
      integer      X_LENGTH
      integer      Y_LENGTH
      integer      N_COMPS
C
      parameter (FILE_NAME  = 'General_RImages.hdf',
     +           IMAGE_NAME = 'Image Array 1',
     +           X_LENGTH   = 10,
     +           Y_LENGTH   = 5,
     +           N_COMPS    = 2)
      integer DFACC_CREATE, DFNT_INT16, MFGR_INTERLACE_PIXEL
      parameter (DFACC_CREATE = 4,
     +           DFNT_INT16   = 22,
     +           MFGR_INTERLACE_PIXEL = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgcreat, mgwrimg, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer gr_id, ri_id, num_type, interlace_mode
      integer start(2), stride(2), edges(2), dimsizes(2)
      integer i, j, k
      integer*2  image_buf(N_COMPS, X_LENGTH, Y_LENGTH) 
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
C     Set the number type, interlace mode, and dimensions of the image.  
C
      num_type = DFNT_INT16
      interlace_mode = MFGR_INTERLACE_PIXEL
      dimsizes(1) = X_LENGTH
      dimsizes(2) = Y_lENGTH
C
C     Create the raster image array. 
C
      ri_id = mgcreat(gr_id, IMAGE_NAME, N_COMPS, num_type,
     +                interlace_mode, dimsizes)
C
C     Fill the image data buffer with values. 
C
      do 30 i = 1, Y_LENGTH
         do 20 j = 1, X_LENGTH
            do 10 k = 1, N_COMPS
               image_buf(k,j,i) = (i+j) - 1
10          continue
20       continue
30    continue

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
      status = mgwrimg(ri_id, start, stride, edges, image_buf)

C
C     Terminate access to the raster image and to the GR interface, 
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end
