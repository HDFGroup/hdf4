      program  write_slab
      implicit none
C
C     Parameter declaration.
C
      character*9  FILE_NAME
      character*13 SDS_NAME
      integer      X_LENGTH, Y_LENGTH, Z_LENGTH, RANK
      parameter   (FILE_NAME = 'SLABS.hdf',
     +             SDS_NAME = 'FilledBySlabs',
     +             X_LENGTH = 4,
     +             Y_LENGTH = 5,
     +             Z_LENGTH = 6,
     +             RANK     = 3)
      integer      DFACC_CREATE, DFNT_INT32
      parameter   (DFACC_CREATE = 4,
     +             DFNT_INT32 = 24)
C
C     Function declaration.
C
      integer sfstart, sfcreate, sfwdata, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id
      integer dim_sizes(3), start(3), edges(3), stride(3)
      integer i, j, k, status
      integer data(X_LENGTH, Y_LENGTH, Z_LENGTH)
      integer xz_data(X_LENGTH, Z_LENGTH)
C
C**** End of variable declaration ************************************
C
C
C     Data initialization.
C
      do 30 k = 1, Z_LENGTH
         do 20 j = 1, Y_LENGTH
            do 10 i = 1, X_LENGTH
               data(i, j, k) = i + j + k 
10            continue
20         continue
30    continue
C
C     Create the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_CREATE)
C
C     Define dimensions of the array to be created. 
C
      dim_sizes(1) = X_LENGTH   
      dim_sizes(2) = Y_LENGTH   
      dim_sizes(3) = Z_LENGTH   
C
C     Create the data set with the name defined in SDS_NAME. 
C
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT32, RANK, 
     .                  dim_sizes)
C
C     Set the parameters start and edges to write 
C     a 4x6 element slab of data to the data set;
C     note that edges(2) is set to 1 to define a 2 dimensional slab
C     parallel to the XZ plane;
C     start(2) (slab position in the array) is initialized inside the
C     for loop.
C 
      edges(1) = X_LENGTH
      edges(2) = 1
      edges(3) = Z_LENGTH
      start(1) = 0 
      start(3) = 0
      stride(1) = 1
      stride(2) = 1
      stride(3) = 1

      do 60 j = 1, Y_LENGTH
       start(2) = j - 1 
C
C     Initialize the buffer xz_data (data slab).
C
       do 50 k = 1, Z_LENGTH
        do 40 i = 1, X_LENGTH
         xz_data(i, k) = data(i, j, k)
40      continue
50     continue
C
C     Write the data slab into SDS array defined in SDS_NAME. 
C     Note that the elements of array stride are set to 1 to
C     specify that the consecutive slabs in the Y direction are written.
C
         status = sfwdata(sds_id, start, stride, edges, xz_data)
60    continue
C
C     Terminate access to the data set. 
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file. 
C
      status = sfend(sd_id)

      end
