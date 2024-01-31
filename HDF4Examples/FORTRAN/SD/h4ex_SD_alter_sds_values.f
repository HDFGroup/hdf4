      program  alter_data
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      integer      DFACC_WRITE
      parameter   (FILE_NAME = 'SDS.hdf',
     +             DFACC_WRITE = 2)
C
C     Function declaration.
C
      integer sfstart, sfselect, sfwdata, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index
      integer start(2), edges(2), stride(2)
      integer status
      integer new_data(2) 
C
C**** End of variable declaration ************************************
C

C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)
C
C     Select the first data set.
C     
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)

C
C     Initialize the start, edge, and stride parameters to write 
C     two elements into 2nd row, 10th column and 11th column places.
C     
C     Specify 2nd row.
C
      start(1) = 1 
C
C     Specify 10th column.
C
      start(2) = 9 
      edges(1) = 1 
C
C     Two elements are written along 2nd row.
C
      edges(2) = 2 
      stride(1) = 1
      stride(2) = 1
C
C     Initialize the new values to be written.
C
      new_data(1) = 1000
      new_data(2) = 1000
C
C     Write the new values. 
C
      status = sfwdata(sds_id, start, stride, edges, new_data)
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
