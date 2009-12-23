      program  read_data
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      integer      X_LENGTH, Y_LENGTH
      parameter   (FILE_NAME  = 'SDS.hdf',
     +             X_LENGTH = 5, 
     +             Y_LENGTH = 16)
      integer      DFACC_READ, DFNT_INT32
      parameter   (DFACC_READ = 1,
     +             DFNT_INT32 = 24)

C
C     Function declaration.
C
      integer sfstart, sfselect, sfrdata, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer start(2), edges(2), stride(2)
      integer data(X_LENGTH, Y_LENGTH)
      integer j
C
C**** End of variable declaration ************************************
C
C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_READ)

C
C     Select the first data set.
C
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)

C
C     Set elements of the array start to 0, elements of the array edges to 
C     SDS dimensions, and elements of the array stride to 1 to read the
C     entire data. 
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Read entire data into data array. Note that sfrdata is used
C     to read the numeric data. 
C
      status = sfrdata(sds_id, start, stride, edges, data)
      
C
C     Print 10th column; the following numbers are displayed:
C
C           10 1000 12 13 14
C
      write(*,*) (data(j,10), j = 1, X_LENGTH)
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
