C *****************************************************************
C                                                                 
C  sd_rd.f                                                       
C        This program creates an HDF file, sd.hdf, which        
C        contains an SDS of size 10 x 5.  It then re-opens       
C        the HDF file in Read Only mode, reads the data back     
C        in, and prints it to the screen.                        
C                                                                
C ******************************************************************

      PROGRAM SD_RD
 
      implicit none

      integer*4 sd_id, sds_id, rank, sd_index
      integer dims(2), start(2), edges(2), stride(2)
      integer i, j, istat
      integer sfstart, sfcreate, sfwdata, sfendacc, sfend
   	  integer sfrdata, sfselect
      integer*4 DFACC_CREATE, DFNT_INT16, DFACC_READ
      integer*4 DIM1, DIM0, FAIL
      parameter (DFACC_CREATE = 4, DFNT_INT16 = 22, DIM1 = 5,
     +			 DIM0 = 10, DFACC_READ = 1, FAIL = -1)
      integer*2 array_data(DIM1, DIM0)
      integer*2 new_array(DIM1, DIM0)
      integer heprnt

C     Create and open the file and initiate the SD interface.
      sd_id = sfstart('sd.hdf', DFACC_CREATE)
      if (sd_id .eq. FAIL) then
C         istat=heprnt(0)
      else
         print*
         print*,'... sd.hdf created'
      end if

C     Define the rank and dimensions of the data set to be created.
      rank = 2
      dims(1) = DIM1
      dims(2) = DIM0

C     Create the data set.
      sds_id = sfcreate(sd_id, 'data1', DFNT_INT16, rank, dims)
      if (sds_id .eq. FAIL) then
C         istat=heprnt(0)
      else
        print*,'... data1 created'
      end if

C     Fill the stored-data array with values.
      do 20 j = 1, DIM0
         do 10 i = 1, DIM1
             array_data(i, j) = i + j - 1
10    continue
20    continue

C     Define the location, pattern, and size of the data set
C     that will be written to.
      start(1) = 0
      start(2) = 0
      edges(1) = DIM1
      edges(2) = DIM0
      stride(1) = 1
      stride(2) = 1

C     Write the stored data to the "data1" data set.
      istat = sfwdata(sds_id, start, stride, edges, array_data)
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      else
         print*,'... data written out'
      end if

C     Terminate access to the array.
      istat = sfendacc(sds_id)
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      end if


C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      else
         print*,'... file closed'
      end if


C     Read data back and write to a new array 
      sd_id = sfstart('sd.hdf', DFACC_READ)
      if (sd_id .eq. FAIL) then
C         istat=heprnt(0)
      else
         print*,'... sd.hdf opened with READ access'
      end if

      sd_index = 0
      sds_id = sfselect (sd_id, sd_index) 
      if (sds_id .eq. FAIL) then
C         istat=heprnt(0)
      end if

      istat = sfrdata (sds_id, start, stride, edges, new_array) 
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      end if

      write(*,*)
      write(*,*)'Data:'
      do 40 j=1,DIM0
         write(*,*)(new_array(i,j),i=1,DIM1)
         i=i+5 
40    continue       
      write(*,*)
      
C     Terminate access to the array.
      istat = sfendacc(sds_id)
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      end if

C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)
      if (istat .eq. FAIL) then
C         istat=heprnt(0)
      else
         print*,'... file closed'
         print*
      end if

      end


