C *************************************************************
C                                                                        
C   sd_gi.f
C     This program reads in example, sd.hdf, and uses
C     sffinfo and sfginfo to obtain the number of datasets and
C     then prints out the name, rank and number of attributes  
C     in each dataset. 
C
C *************************************************************

      PROGRAM sd_gi 

      integer sd_id, sds_id
      integer n_datasets, n_file_attrs, index, rank
      integer istat, attributes, num_type
      integer sfstart, sffinfo, sfselect, sfginfo
      integer sfendacc, sfend

      integer*4 DFACC_READ, MAX_VAR_DIMS, FAIL
      parameter (DFACC_READ = 1, MAX_VAR_DIMS = 32, FAIL = -1)

      integer*4 dim_sizes(MAX_VAR_DIMS)
      character name *(64)

C     Open the file and initiate the SD interface.
      sd_id = sfstart('sd.hdf', DFACC_READ)
      if (sd_id .ne. FAIL) then
         print*
         print*,'... sd.hdf opened for READ access.' 
      end if

C     Determine the contents of the file.
      istat = sffinfo(sd_id, n_datasets, n_file_attrs)

C     Access and print the names of every data set in the file.
      do 10 index = 0, n_datasets - 1
       sds_id = sfselect(sd_id, index)

       istat = sfginfo(sds_id, name, rank, dim_sizes, num_type, 
     +                 attributes)

       write(*,*) 'name = ', name
       write(*,*) 'rank = ', rank
       write(*,*) 'number of attributes = ', attributes

       istat = sfendacc(sds_id)
10    continue

C     Terminate access to the SD interface and close the file.
      istat = sfend(sd_id)
      if (istat .ne. FAIL) then
         print*,'... file closed'
         print*
      end if

      end
