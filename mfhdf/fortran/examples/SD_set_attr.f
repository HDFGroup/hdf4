      program  set_attribs
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
      integer      DFACC_WRITE, DFNT_CHAR8, DFNT_FLOAT32
      parameter   (DFACC_WRITE = 2,
     +             DFNT_CHAR8  = 4,
     +             DFNT_FLOAT32 = 5)
C
C     Function declaration.
C
      integer sfstart, sfscatt, sfsnatt, sfselect, sfdimid
      integer sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index, status
      integer dim_id, dim_index 
      integer n_values
      character*16 file_values
      real         sds_values(2)
      character*7  dim_values
      file_values   = 'Storm_track_data'
      sds_values(1) = 2.
      sds_values(2) = 10.
      dim_values    = 'Seconds'
C
C**** End of variable declaration ************************************
C
C
C     Open the file and initialize the SD interface. 
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)
C
C     Set an attribute that describes the file contents. 
C
      n_values = 16
      status = sfscatt(sd_id, FILE_ATTR_NAME, DFNT_CHAR8, n_values, 
     +                 file_values)
C
C     Select the first data set. 
C
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)
C
C     Assign attribute to the first SDS. Note that attribute values
C     may have different data type than SDS data.
C
      n_values = 2
      status = sfsnatt(sds_id, SDS_ATTR_NAME, DFNT_FLOAT32, n_values, 
     +                 sds_values)
C
C     Get the identifier for the first dimension. 
C
      dim_index = 0 
      dim_id = sfdimid(sds_id, dim_index)
C
C     Set an attribute to the dimension that specifies the
C     dimension metric. 
C
      n_values = 7
      status = sfscatt(dim_id, DIM_ATTR_NAME, DFNT_CHAR8, n_values, 
     +                 dim_values)
C
C     Terminate access to the data set. 
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
