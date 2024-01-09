      program  sds_vrs_coordvar 
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      parameter   (FILE_NAME = 'SDS.hdf')
      integer      DFACC_READ, DFNT_INT32
      parameter   (DFACC_READ = 1,
     +             DFNT_INT32 = 24)
      integer      MAX_VAR_DIMS
      parameter   (MAX_VAR_DIMS = 32)
C
C     Function declaration.
C 
      integer sfstart, sfselect, sfiscvar, sffinfo, sfginfo
      integer sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer       sd_id, sds_id, sds_index, status
      integer       rank, data_type
      integer       n_datasets, n_file_attrs, n_attrs
      integer       dim_sizes(MAX_VAR_DIMS)
      character*256 sds_name
C
C**** End of variable declaration ************************************
C
C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_READ)
C
C     Obtain information about the file.
C 
      status = sffinfo(sd_id, n_datasets, n_file_attrs) 
C
C     Get information about each SDS in the file.
C     Check whether it is a coordinate variable, then display retrieved 
C     information. 
C     Output displayed:
C 
C           SDS array with the name SDStemplate
C           Coordinate variable with the name X_Axis
C           Coordinate variable with the name Y_Axis
C
      do 10 sds_index = 0, n_datasets-1
         sds_id = sfselect(sd_id, sds_index)
         status = sfginfo(sds_id, sds_name, rank, dim_sizes,
     +                    data_type, n_attrs)
         status = sfiscvar(sds_id)
         if (status .eq. 1) then
             write(*,*) "Coordinate variable with the name ",
     +       sds_name(1:6) 
         else
             write(*,*) "SDS array with the name ", 
     +       sds_name(1:11) 
         endif
C
C        Terminate access to the data set.
C
         status = sfendacc(sds_id)
10    continue
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)
      end
