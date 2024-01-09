      program append_sds
      implicit none
C
C     Parameter declaration.
C
      character*16  FILE_NAME
      character*14  SDS_NAME
      integer       X_LENGTH, Y_LENGTH, RANK
      parameter    (FILE_NAME = 'SDSUNLIMITED.hdf',
     +              SDS_NAME = 'AppendableData',
     +              X_LENGTH = 10, 
     +              Y_LENGTH = 10,
     +              RANK     = 2)
      integer       DFACC_CREATE, DFACC_WRITE, SD_UNLIMITED,
     +              DFNT_INT32
      parameter    (DFACC_CREATE = 4,
     +              DFACC_WRITE  = 2,
     +              SD_UNLIMITED = 0,
     +              DFNT_INT32 =   24)
C
C     Function declaration.
C
      integer sfstart, sfcreate, sfwdata, sfselect 
      integer sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer dim_sizes(2)
      integer start(2), edges(2), stride(2)
      integer i, j
      integer data (X_LENGTH, Y_LENGTH), append_data(X_LENGTH)
C
C**** End of variable declaration ************************************
C
C
C     Data initialization.
C 
      do 20 j = 1, Y_LENGTH
         do 10 i = 1, X_LENGTH
            data(i, j) = i + j
10      continue
20    continue
C
C     Create the file and initialize the SD interface. 
C
      sd_id = sfstart(FILE_NAME, DFACC_CREATE)
C
C     Define dimensions of the array. Make the
C     last dimension appendable by defining its length as unlimited.
C
      dim_sizes(1) = X_LENGTH
      dim_sizes(2) = SD_UNLIMITED

C     Create the array data set. 
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT32, RANK, 
     .                  dim_sizes)
C
C     Define the location and the size of the data to be written
C     to the data set. Note that the elements of array stride are
C     set to 1 for contiguous writing.    
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Write the data. 
C
      status = sfwdata(sds_id, start, stride, edges, data)
C
C     Terminate access to the data set, terminate access
C     to the SD interface, and close the file. 
C
      status = sfendacc(sds_id)
      status = sfend(sd_id)
C
C     Store the array values to be appended to the data set. 
C
      do 30 i = 1, X_LENGTH
         append_data(i) = 1000 + i - 1 
30    continue
C
C     Reopen the file and initialize the SD. 
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)
C
C     Select the first data set. 
C
      sds_index = 0 
      sds_id = sfselect(sd_id, sds_index)
C
C     Define the location of the append to start at the 11th 
C     column of the 1st row and to stop at the end of the 10th row.
C
      start(1) = 0
      start(2) = Y_LENGTH
      edges(1) = X_LENGTH
      edges(2) = 1
C
C     Append the data to the data set. 
C
      status = sfwdata(sds_id, start, stride, edges, append_data)
C
C     Terminate access to the data set. 
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file. 
C
      status = sfend(sd_id)

      end
