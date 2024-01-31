      program get_data_set_info 
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      parameter   (FILE_NAME = 'SDS.hdf')
      integer      DFACC_READ, DFNT_INT32
      parameter   (DFACC_READ = 1,
     +             DFNT_INT32 = 24)
      integer      MAX_NC_NAME, MAX_VAR_DIMS
      parameter   (MAX_NC_NAME  = 256,
     +             MAX_VAR_DIMS = 32)
C
C     Function declaration.
C
      integer sfstart, sffinfo, sfselect, sfginfo
      integer sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id
      integer n_datasets, n_file_attrs, index
      integer status, n_attrs
      integer rank, data_type
      integer dim_sizes(MAX_VAR_DIMS)
      character name *(MAX_NC_NAME)
      integer i
C
C**** End of variable declaration ************************************
C
C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_READ)
C
C     Determine the number of data sets in the file and the number of 
C     file attributes. 
C
      status = sffinfo(sd_id, n_datasets, n_file_attrs)
C
C     Access every data set in the file and print its name, rank,
C     dimension sizes, data type, and number of attributes.
C     The following information should be displayed:
C
C                name = SDStemplate    
C                rank =   2
C                dimension sizes are :   5  16
C                data type is   24
C                number of attributes is   0
C
      do 10 index = 0, n_datasets - 1
         sds_id = sfselect(sd_id, index)
         status = sfginfo(sds_id, name, rank, dim_sizes, data_type, 
     .                    n_attrs)
         write(*,*)  "name = ", name(1:15)
         write(*,*)  "rank = ", rank
         write(*,*)  "dimension sizes are : ", (dim_sizes(i), i=1, rank)
         write(*,*)  "data type is ", data_type
         write(*,*)  "number of attributes is ", n_attrs   
C
C     Terminate access to the current data set.
C
         status = sfendacc(sds_id)
10    continue
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
