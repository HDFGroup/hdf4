      program read_raster_image
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      integer      X_LENGTH
      integer      Y_LENGTH
      integer      N_COMPS
C
      parameter (FILE_NAME  = 'General_RImages.hdf',
     +           X_LENGTH   = 10,
     +           Y_LENGTH   = 5,
     +           N_COMPS    = 2)
      integer PART_COLS, PART_ROWS, SKIP_COLS, SKIP_ROWS
      integer COLS_PART_START, ROWS_PART_START
      integer COLS_SKIP_START, ROWS_SKIP_START
      integer N_STRIDES
      parameter (PART_COLS = 3, PART_ROWS = 2,
     +           SKIP_COLS = 3, SKIP_ROWS = 5,
     +           COLS_PART_START = 1, ROWS_PART_START = 3,
     +           COLS_SKIP_START = 0, ROWS_SKIP_START = 1,
     +           N_STRIDES = 2)
      integer DFACC_READ
      parameter (DFACC_READ = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgselct, mgrdimg, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id
      integer gr_id, ri_id
      integer start(2), stride(2), edges(2)
      integer i, j
      integer*2  entire_image(N_COMPS, X_LENGTH, Y_LENGTH) 
      integer*2  partial_image(N_COMPS, PART_ROWS, PART_COLS) 
      integer*2  skipped_image(N_COMPS, SKIP_ROWS, SKIP_COLS) 
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Select the first raster image in the file.
C
      ri_id = mgselct(gr_id, 0)
C     
C     Define the size of the data to be read, i.e., start from the origin
C     and go as long as the length of each dimension.
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Read the data from the raster image array. 
C
      status = mgrdimg(ri_id, start, stride, edges, entire_image)
C
C     Display only the first component of the image since the two components
C     have the same data in this example.
C
      write(*,*) 'First component of the entire image'
      write(*,*)
      do 10 i = 1, X_LENGTH
         write(*,1000) (entire_image(1,i,j), j = 1, Y_LENGTH)
10    continue
      write(*,*)
C
C     Define the size of the data to be read.
C
      start(1) = ROWS_PART_START
      start(2) = COLS_PART_START 
      edges(1) = PART_ROWS 
      edges(2) = PART_COLS 
      stride(1) = 1
      stride(2) = 1
C
C     Read a subset of the raster image array. 
C
      status = mgrdimg(ri_id, start, stride, edges, partial_image)
C
C     Display only the first component of the read sample. 
C
      write(*,*)
     +  'Two rows and three columns at 4th row and 2nd column',
     +  ' of the first component'
      write(*,*)
      do 20 i = 1, PART_ROWS
         write(*,1000) (partial_image(1,i,j), j = 1, PART_COLS)
20    continue
      write(*,*)
C
C     Define the size and the pattern to read the data.
C
      start(1) = ROWS_SKIP_START
      start(2) = COLS_SKIP_START 
      edges(1) = SKIP_ROWS 
      edges(2) = SKIP_COLS 
      stride(1) = N_STRIDES 
      stride(2) = N_STRIDES 
C
C     Read all the odd rows and even columns of the image.
C
      status = mgrdimg(ri_id, start, stride, edges, skipped_image)
C
C     Display only the first component of the read sample. 
C
      write(*,*) 'All even rows and odd columns of the first component'
      write(*,*)
      do 30 i = 1, SKIP_ROWS
         write(*,1000) (skipped_image(1,i,j), j = 1, SKIP_COLS)
30    continue
      write(*,*)
C
C     Terminate access to the raster image and to the GR interface, 
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
1000  format(1x, 5(I4))
      end
