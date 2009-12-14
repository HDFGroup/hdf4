      program modify_image
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*13 IMAGE1_NAME
      integer      X1_LENGTH
      integer      Y1_LENGTH
      integer      N1_COMPS
      character*13 IMAGE2_NAME
      integer      X2_LENGTH
      integer      Y2_LENGTH
      integer      N2_COMPS
C
      parameter (FILE_NAME   = 'General_RImages.hdf',
     +           IMAGE1_NAME = 'Image Array 1',
     +           IMAGE2_NAME = 'Image Array 2',
     +           X1_LENGTH   = 5,
     +           Y1_LENGTH   = 2,
     +           N1_COMPS    = 2,
     +           X2_LENGTH   = 6,
     +           Y2_LENGTH   = 4,
     +           N2_COMPS    = 3)
      integer DFACC_WRITE, DFNT_INT16, DFNT_CHAR8,
     +        MFGR_INTERLACE_PIXEL
      parameter (DFACC_WRITE  = 2,
     +           DFNT_CHAR8   = 4,
     +           DFNT_INT16   = 22,
     +           MFGR_INTERLACE_PIXEL = 0)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgselct, mgcreat, mgwrimg, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer gr_id, ri1_id, ri2_id, data_type, interlace_mode
      integer start1(2), stride1(2), edges1(2)
      integer start2(2), stride2(2), edges2(2), dim_sizes(2)
      integer i, j, k
      integer*2  image1_buf(N1_COMPS, X1_LENGTH, Y1_LENGTH) 
      character  image2_buf(N2_COMPS, X2_LENGTH, Y2_LENGTH)
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for writing.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Select the first raster image in the file.
C
      ri1_id = mgselct(gr_id, 0)
C
C     Fill the buffer with values.
C
      do 20 i = 1, Y1_LENGTH
         do 10 j = 1, X1_LENGTH
               image1_buf(1,j,i) = 0 
               image1_buf(2,j,i) = 0 
10       continue
20    continue
C
C     Define the part of the data in the first image that will be overwritten
C     with the new values from image1_buf.
C
      start1(1) = 0
      start1(2) = 0
      edges1(1) = X1_LENGTH
      edges1(2) = Y1_LENGTH
      stride1(1) = 1
      stride1(2) = 1
C
C     Write the data in the buffer into the image array.
C
      status = mgwrimg(ri1_id, start1, stride1, edges1, image1_buf)

C
C     Set the number type, interlace mode, and dimensions of the second image.  
C
      data_type = DFNT_CHAR8
      interlace_mode = MFGR_INTERLACE_PIXEL
      dim_sizes(1) = X2_LENGTH
      dim_sizes(2) = Y2_LENGTH
C
C     Create the second image in the file.
C
      ri2_id = mgcreat(gr_id, IMAGE2_NAME, N2_COMPS, data_type,
     +                interlace_mode, dim_sizes)
C
C     Fill the image data buffer with values. 
C
      do 60 i = 1, Y2_LENGTH 
         do 50 j = 1, X2_LENGTH
            do 40 k = 1, N2_COMPS 
               image2_buf(k,j,i) = char(65 + k - 1) 
40          continue
50       continue
60    continue

C     
C     Define the size of the data to be written, i.e., start from the origin
C     and go as long as the length of each dimension.
C
      start2(1) = 0
      start2(2) = 0
      edges2(1) =  dim_sizes(1)
      edges2(2) =  dim_sizes(2) 
      stride2(1) = 1
      stride2(2) = 1
C
C     Write the data in the buffer into the image array.
C
      status = mgwrimg(ri2_id, start2, stride2, edges2, image2_buf)

C
C     Terminate access to the raster images and to the GR interface,
C     and close the HDF file.
C
      status = mgendac(ri1_id)
      status = mgendac(ri2_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end
