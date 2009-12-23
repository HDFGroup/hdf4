      program  create_SDS 
      implicit none
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      character*11 SDS_NAME
      integer      X_LENGTH, Y_LENGTH, RANK
      parameter   (FILE_NAME = 'SDS.hdf',
     +             SDS_NAME = 'SDStemplate',
     +             X_LENGTH = 5,
     +             Y_LENGTH = 16,
     +             RANK     = 2)
      integer      DFACC_CREATE, DFNT_INT32
      parameter   (DFACC_CREATE = 4,
     +             DFNT_INT32 = 24)
C
C     Function declaration.
C
      integer sfstart, sfcreate, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, dim_sizes(2)
      integer status
C
C**** End of variable declaration ************************************
C
C
C     Create the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_CREATE)
C
C     Define dimensions of the array to be created.
C
      dim_sizes(1) = X_LENGTH
      dim_sizes(2) = Y_LENGTH
C
C     Create the array with the name defined in SDS_NAME.
C     Note that DFNT_INT32 indicates that the SDS data is of type
C     integer. Refer to Tables 2E and 2I for the definition of other types.  
C
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT32, RANK, 
     .                  dim_sizes)
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
