C     sd_unl.f
C       This program creates an HDF file called "sdunl.hdf"
C       and makes use of the unlimited dimension feature in
C       HDF.
C
C     
      PROGRAM UNLIMITED_DIM 

      implicit none

      integer*4 sd_id, sds_id, rank
      integer dims(2), start(2), edges(2), stride(2)
      integer i, j, istat
      integer sfstart, sfcreate, sfwdata, sfendacc, sfend
      integer sfselect

C     DFACC_CREATE and DFNT_INT16 are defined in hdf.h.
      integer*4 DFACC_CREATE, DFNT_INT16, DFACC_WRITE
      integer*4 DIM1, DIM0, SD_UNLIMITED, FAIL
      parameter (DFACC_CREATE = 4, DFNT_INT16 = 22, DIM1 = 5,
     +			 DIM0 = 10, DFACC_WRITE = 2, SD_UNLIMITED = 0,
     +            FAIL = -1)
      integer*2 array_data(DIM1, DIM0), append_data(DIM1)

C     Create and open the file and initiate the SD interface.
      sd_id = sfstart('sdunl.hdf', DFACC_CREATE)
      if (sd_id .ne. FAIL) then
         print*
         print*,'... sdunl.hdf created'
      end if

C     Define the rank and dimensions of the data set to be created.
      rank = 2
      dims(1) = DIM1
      dims(2) = SD_UNLIMITED

C     Create the data set.
      sds_id = sfcreate(sd_id, 'data1', DFNT_INT16, rank, dims)
      if (sds_id .ne. FAIL) then
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
      if (istat .ne. FAIL) then
         print*,'... data written out'
      end if

C     Terminate access to the array.
      istat = sfendacc(sds_id)

      istat = sfend (sd_id)
      if (istat .ne. FAIL) then
         print*,'... file closed'
      end if

C     Re-open the file and append data to the end of the file
      sd_id = sfstart('sdunl.hdf', DFACC_WRITE)
      if (sd_id .ne. FAIL) then
         print*,'... sdunl.hdf opened in Read/Write mode'
      end if

      sds_id = sfselect (sd_id, 0)
      if (sds_id .ne. FAIL) then
         print*,'... data1 selected'
      end if

      do 30 j = 1, DIM1
        append_data(j) = j 
30    continue

C     Define the location of the append
      start(1) = 0
      start(2) = DIM0
      edges(1) = DIM1
      edges(2) = 1

C     Append data to file
      istat = sfwdata(sds_id, start, stride, edges, append_data)
      if (istat .ne. FAIL) then
         print*,'... data appended'
      end if

C     Terminate access to the array.
      istat = sfendacc(sds_id)

C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)
      if (istat .ne. FAIL) then
         print*,'... file closed'
         print*
      end if

      end


