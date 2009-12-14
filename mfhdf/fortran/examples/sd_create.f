C *************************************************************
C                                                                        
C   sd_create.f
C     This program creates an HDF file, sd.hdf, which contains
C     an SDS of size 10 x 5.
C
C *************************************************************

      PROGRAM CREATE_SD

      implicit none

      integer*4 sd_id, sds_id, rank
      integer dims(2), start(2), edges(2), stride(2)
      integer i, j, istat
      integer sfstart, sfcreate, sfwdata, sfendacc, sfend
      integer heprnt

C     DFACC_CREATE and DFNT_INT16 are defined in hdf.h.
      integer*4 DFACC_CREATE, DFNT_INT16
      integer*4 DIM1, DIM0, FAIL
      parameter (DFACC_CREATE = 4, DFNT_INT16 = 22, DIM1 = 5,
     +					       DIM0 = 10, FAIL = -1)
      integer*2 array_data(DIM1, DIM0)

C     Create and open the file and initiate the SD interface.
      sd_id = sfstart('sd.hdf', DFACC_CREATE)
      if (sd_id .eq. FAIL) then
         print*,'sfstart failed.'
         istat=heprnt(0)
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
         print*,'sfcreate failed.'
         istat=heprnt(0)
      else
        print*,'... data1 created'
      end if

C     Fill the stored-data array with values.
      do 20 j = 1, DIM0 
       do 10 i = 1, DIM1 
        array_data(i, j) = i + j - 1
10     continue
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
         print*,'sfwdata failed.'
         istat=heprnt(0)
      else
         print*,'... data written out'
      end if

C     Terminate access to the array.
      istat = sfendacc(sds_id)
      if (istat .eq. FAIL) then
         print*,'sfendacc failed.'
         istat=heprnt(0)
      end if

C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)
      if (istat .eq. FAIL) then
         print*,'sfend failed.'
         istat=heprnt(0)
      else
         print*,'... file closed'
         print*
      end if

      end


