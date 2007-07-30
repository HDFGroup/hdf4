C *************************************************************     
C
C   sd_rattr.f
C     This program reads the three attributes added to the HDF   
C     file, sd.hdf, in the previous tutorial.                   
C
C *************************************************************     

      PROGRAM sd_rattr
 
      implicit none

      integer*4 sd_id, sds_id, dim_id
      integer attr_index, istat, range_buffer(2)
      character*20 char_buffer 
      character*9  metric_name
      integer sfstart, sffattr, sfselect, sfdimid
      integer sfrcatt, sfrnatt, sfendacc, sfend

      integer DFACC_READ
      parameter (DFACC_READ = 1)

C     Open the file. 
      sd_id = sfstart('sd.hdf', DFACC_READ)
      print*
      print*,'... sd.hdf opened for READ ONLY access'
      print*

C     Find the file attribute named 'file_contents'. 
      attr_index = sffattr(sd_id, 'file_contents')

C     Get information about the file attribute. 
C      istat = sfgainfo(sd_id, attr_index, attr_name, num_type, count)

C     Read the attribute data. 
      istat = sfrcatt(sd_id, attr_index, char_buffer)
      write(*,*)'file_contents: ', char_buffer

C     Get the identifier for the first data set. 
      sds_id = sfselect(sd_id, 0)

C     Find the data set attribute named 'valid_range'. 
      attr_index = sffattr(sds_id, 'valid_range')

C     Read the attribute data. 
      istat = sfrnatt(sds_id, attr_index, range_buffer)
      write(*,*)'valid range:',range_buffer(1),range_buffer(2)

      dim_id = sfdimid (sds_id, 1)
      attr_index = sffattr(dim_id, 'dim_metric')
      istat = sfrcatt (dim_id, attr_index, metric_name)
      write(*,*)'dim_metric: ', metric_name

C     Terminate access to the array 
      istat = sfendacc(sds_id)

C     Terminate access to the SD interface and close the file 
      istat = sfend(sd_id)
      print*
      print*,'... closing file'

      end


