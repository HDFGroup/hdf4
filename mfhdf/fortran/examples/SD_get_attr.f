      program  attr_info
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      character*13 FILE_ATTR_NAME
      character*11 SDS_ATTR_NAME
      character*10 DIM_ATTR_NAME
      parameter   (FILE_NAME = 'SDS.hdf',
     +             FILE_ATTR_NAME = 'File_contents',
     +             SDS_ATTR_NAME  = 'Valid_range',
     +             DIM_ATTR_NAME  = 'Dim_metric')
      integer      DFACC_READ, DFNT_FLOAT32
      parameter   (DFACC_READ   = 1,
     +             DFNT_FLOAT32 = 5)

C
C     Function declaration.
C
      integer sfstart, sffattr, sfgainfo, sfrattr, sfselect
      integer sfdimid, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer      sd_id, sds_id, dim_id
      integer      attr_index, data_type, n_values, status
      real         sds_data(2)
      character*20 attr_name 
      character*16 file_data
      character*7  dim_data
      integer      i
C
C**** End of variable declaration ************************************
C
C
C     Open the file and initialize SD interface. 
C
      sd_id = sfstart(FILE_NAME, DFACC_READ)
C
C     Find the file attribute defined by FILE_ATTR_NAME.
C     Note that the first parameter is an SD interface identifier.
C
      attr_index = sffattr(sd_id, FILE_ATTR_NAME)
C
C     Get information about the file attribute. 
C
      status = sfgainfo(sd_id, attr_index, attr_name, data_type, 
     +                  n_values)
C
C     Read the file attribute data. 
C
      status = sfrattr(sd_id, attr_index, file_data)
C
C     Print file attribute value.
C
      write(*,*) "File attribute value is : ", file_data 
C
C     Select the first data set. 
C
      sds_id = sfselect(sd_id, 0)
C
C     Find the data set attribute defined by SDS_ATTR_NAME.
C     Note that the first parameter is a data set identifier. 
C
      attr_index = sffattr(sds_id, SDS_ATTR_NAME)
C
C     Get information about the data set attribute. 
C
      status = sfgainfo(sds_id, attr_index, attr_name, data_type, 
     +                  n_values)
C
C     Read the SDS attribute data. 
C
      status = sfrattr(sds_id, attr_index, sds_data)

C
C     Print SDS attribute data type and values. 
C
      if (data_type .eq. DFNT_FLOAT32)  then
         write(*,*) "SDS attribute data type is : float32 "  
      endif
      write(*,*) "SDS attribute values are  : " 
      write(*,*)  (sds_data(i), i=1, n_values) 
C
C     Get the identifier for the first dimension of the SDS. 
C
      dim_id = sfdimid(sds_id, 0)
C
C     Find the dimensional attribute defined by DIM_ATTR_NAME.
C     Note that the first parameter is a dimension identifier.
C
      attr_index = sffattr(dim_id, DIM_ATTR_NAME)
C
C     Get information about dimension attribute. 
C
      status = sfgainfo(dim_id, attr_index, attr_name, data_type, 
     +                  n_values)
C
C     Read the dimension attribute data. 
C
      status = sfrattr(dim_id, attr_index, dim_data)
C
C     Print dimension attribute value.
C
      write(*,*) "Dimensional attribute value is : ", dim_data 
C
C     Terminate access to the data set. 
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file. 
C
      status = sfend(sd_id)
C
C     Output of this program is :
C
C
C     File attribute value is : Storm_track_data
C     SDS attribute data type is : float32 
C     SDS attribute values are  : 
C         2.00000   10.00000
C      Dimensional attribute value is : Seconds
C
      end
