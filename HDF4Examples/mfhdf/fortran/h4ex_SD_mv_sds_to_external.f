      program  write_extfile
      implicit none 
C
C     Parameter declaration.
C
      character*7  FILE_NAME
      character*11 EXT_FILE_NAME
      integer      OFFSET
      integer      DFACC_WRITE
      parameter   (FILE_NAME      = 'SDS.hdf',
     +             EXT_FILE_NAME  = 'ExternalSDS',
     +             OFFSET         = 24,
     +             DFACC_WRITE    = 2)

C
C     Function declaration.
C
      integer sfstart, sfselect, sfsextf, sfendacc, sfend
C
C**** Variable declaration *******************************************
C
      integer sd_id, sds_id, sds_index
      integer status
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_WRITE)
C
C     Select the first data set.
C
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)
C
C     Create a file with the name EXT_FILE_NAME and move the data set
C     into it, starting at byte location OFFSET.
C
      status = sfsextf(sds_id, EXT_FILE_NAME, OFFSET)
C
C     Terminate access to the data set. 
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file. 
C
      status = sfend(sd_id)

      end
