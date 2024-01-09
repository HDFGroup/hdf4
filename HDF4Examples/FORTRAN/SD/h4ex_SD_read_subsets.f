      program  read_subsets
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      parameter   (FILE_NAME  = 'SDS.hdf')
      integer      DFACC_READ, DFNT_INT32
      parameter   (DFACC_READ = 1,
     +             DFNT_INT32 = 24)
      integer      SUB1_LENGTH, SUB2_LENGTH, SUB3_LENGTH1,
     +             SUB3_LENGTH2 
      parameter   (SUB1_LENGTH  = 5,
     +             SUB2_LENGTH  = 4, 
     +             SUB3_LENGTH1 = 2,
     +             SUB3_LENGTH2 = 3)

C
C     Function declaration.
C
      integer sfstart, sfselect, sfrdata, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer start(2), edges(2), stride(2)
      integer sub1_data(SUB1_LENGTH)
      integer sub2_data(SUB2_LENGTH)
      integer sub3_data(SUB3_LENGTH1,SUB3_LENGTH2)
      integer i, j
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
      sds_id =sfselect(sd_id, sds_index)
C
C            Reading the first subset.
C
C     Set elements of start, stride, and edges arrays to read 
C     every 3d element in in the 2nd row starting in the 4th column. 
C
      start(1) = 1 
      start(2) = 3 
      edges(1) = 1 
      edges(2) = SUB1_LENGTH
      stride(1) = 1
      stride(2) = 3 
C
C     Read the data from sub1_data array. 
C
      status = sfrdata(sds_id, start, stride, edges, sub1_data)
      
C
C     Print what we have just read, the following numbers should be displayed:
C
C           5 8 1000 14 17 
C
      write(*,*) (sub1_data(j), j = 1, SUB1_LENGTH)
C
C            Reading the second subset.
C
C     Set elements of start, stride, and edges arrays to read 
C     first 4 elements of 10th column. 
C
      start(1) = 0 
      start(2) = 9 
      edges(1) = SUB2_LENGTH 
      edges(2) = 1 
      stride(1) = 1
      stride(2) = 1 
C
C     Read the data into sub2_data array. 
C
      status = sfrdata(sds_id, start, stride, edges, sub2_data)
      
C
C     Print what we have just read; the following numbers should be displayed:
C
C          10 1000 12 13 
C
      write(*,*) (sub2_data(j), j = 1, SUB2_LENGTH)
C
C            Reading the third subset.
C
C     Set elements of start, stride and edges arrays to read 
C     every 6th element in the row and every 4th element in the column
C     starting at 1st row, 3rd column.  
C
      start(1) = 0 
      start(2) = 2 
      edges(1) = SUB3_LENGTH1 
      edges(2) = SUB3_LENGTH2 
      stride(1) = 4 
      stride(2) = 6 
C
C     Read the data from the file into sub3_data array. 
C
      status = sfrdata(sds_id, start, stride, edges, sub3_data)
      
C
C     Print what we have just read; the following numbers should be displayed:
C
C         3 9 15
C         7 13 19 
C
      do 50 i = 1, SUB3_LENGTH1
         write(*,*) (sub3_data(i,j), j = 1, SUB3_LENGTH2)
50    continue    
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
