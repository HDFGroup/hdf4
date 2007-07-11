C *************************************************************
C                                                                        
C   sd_sattr.f
C     This program adds three attributes to the HDF file, 
C     sd.hdf, which was created using sd_create:
C          file_contents   - A file attribute, with a value of    
C                            "storm_track_data"                   
C          valid_range     - A data set attribute with range      
C                            values of 2 and 10                   
C          dim_metric      - A dimension attribute for dimension 
C                            zero, with a value of "millibars"   
C
C *************************************************************
 

      PROGRAM sd_sattr 

      integer*4 sd_id, sds_id, dim_id, istat
      integer num_values(2)
      integer sfstart, sfscatt, sfsnatt, sfselect, sfdimid
      integer sfendacc, sfend
      integer*4 DFACC_WRITE, DFNT_CHAR8, DFNT_INT32
      parameter (DFACC_WRITE = 2, DFNT_CHAR8 = 4, DFNT_INT32 = 24)

C     Open the file and get the identifier for the file. 
      sd_id = sfstart('sd.hdf', DFACC_WRITE)
      print*
      print*, '... sd.hdf opened for WRITE access'

C     Set an attribute that describes the file contents. 
      istat = sfscatt(sd_id, 'file_contents', DFNT_CHAR8, 16, 
     +                'storm_track_data')
      print*, '... file attribute written'

C     Get the identifier for the first data set. 
      sds_id = sfselect(sd_id, 0)

C     Set an attribute the specifies a valid range of values. 
      num_values(1) = 2
      num_values(2) = 10
      istat = sfsnatt(sds_id, 'valid_range', DFNT_INT32, 2, num_values)
      print*,'... data attribute written'

C     Get the identifier for the dimension 0. 
      dim_id = sfdimid(sds_id, 1)

      istat = sfscatt(dim_id, 'dim_metric', DFNT_CHAR8, 9, 'millibars') 
      print*, '... dimension attribute written'

C     Terminate access to the array 
      istat = sfendacc(sds_id)

C     Terminate access to the SD interface and close the file 
      istat = sfend(sd_id)
      print*, '... file closed ' 
      print*

      end


