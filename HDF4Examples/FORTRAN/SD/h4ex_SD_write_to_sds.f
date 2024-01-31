      program  write_data
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      character*11 SDS_NAME
      integer      X_LENGTH, Y_LENGTH, RANK
      parameter   (FILE_NAME = 'SDS.hdf',
     +             SDS_NAME = 'SDStemplate',
     +             X_LENGTH = 5,
     +             Y_LENGTH = 16,
     +             RANK     = 2)
      integer      DFACC_WRITE, DFNT_INT32
      parameter   (DFACC_WRITE = 2,
     +             DFNT_INT32 = 24)
C
C     Function declaration.
C

      integer sfstart, sfselect, sfwdata, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer start(2), edges(2), stride(2)
      integer i, j
      integer data(X_LENGTH, Y_LENGTH)
C
C**** End of variable declaration ************************************
C

C
C     Data set data initialization. 
C
      do 20 j = 1, Y_LENGTH
         do 10 i = 1, X_LENGTH
            data(i, j) = i + j - 1
10         continue
20    continue

C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)

C
C     Attach to the first data set.
C
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)

C
C     Define the location and size of the data to be written
C     to the data set. Note that setting values of the array stride to 1
C     specifies the contiguous writing of data.
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Write the stored data to the data set named in SDS_NAME.
C     Note that the routine sfwdata is used instead of sfwcdata 
C     to write the numeric data.
C
      status = sfwdata(sds_id, start, stride, edges, data)
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
