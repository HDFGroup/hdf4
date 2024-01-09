      program  write_compressed_data
      implicit none
C
C     Parameter declaration.
C
      character*17  FILE_NAME
      character*7   SDS_NAME
      integer       X_LENGTH, Y_LENGTH, RANK
      parameter    (FILE_NAME = 'SDScompressed.hdf',
     +              SDS_NAME = 'SDSgzip',
     +              X_LENGTH = 5,
     +              Y_LENGTH = 16,
     +              RANK     = 2)
      integer       DFACC_CREATE, DFNT_INT32
      parameter    (DFACC_CREATE = 4,
     +              DFNT_INT32 = 24)
      integer       COMP_CODE_DEFLATE
      parameter    (COMP_CODE_DEFLATE = 4)
      integer       DEFLATE_LEVEL
      parameter    (DEFLATE_LEVEL = 6)
C     To use Skipping Huffman compression method, declare 
C            integer   COMP_CODE_SKPHUFF
C            parameter(COMP_CODE_SKPHUFF = 3) 
C     To use RLE compression method, declare
C            integer   COMP_CODE_RLE
C            parameter(COMP_CODE_RLE = 1)
C
C
C     Function declaration.
C
      integer sfstart, sfcreate, sfwdata, sfendacc, sfend,
     +        sfscompress
C
C**** Variable declaration *******************************************
C
      integer  sd_id, sds_id, status
      integer  start(2), edges(2), stride(2), dim_sizes(2)
      integer  comp_type
      integer  comp_prm(1)
      integer  data(X_LENGTH, Y_LENGTH)
      integer  i, j
C
C**** End of variable declaration ************************************
C
C
C     Buffer array data and define array dimensions. 
C
      do 20 j = 1, Y_LENGTH
         do 10 i = 1, X_LENGTH
            data(i, j) = i + j - 1
10       continue
20    continue
      dim_sizes(1) = X_LENGTH
      dim_sizes(2) = Y_LENGTH
C
C     Open the file and initialize the SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_CREATE)
C
C     Create the data set with the name SDS_NAME.  
C
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT32, RANK, dim_sizes) 
C
C     Initialize compression parameter (deflate level)
C     and call sfscompress function
C     For Skipping Huffman compression, comp_prm(1) should be set
C     to skipping sizes value (skp_size).
C 
      comp_type   = COMP_CODE_DEFLATE
      comp_prm(1) = deflate_level
      status      = sfscompress(sds_id, comp_type, comp_prm(1))
C
C     Define the location and size of the data that will be written to
C     the data set.
C
      start(1) = 0
      start(2) = 0
      edges(1) = X_LENGTH
      edges(2) = Y_LENGTH
      stride(1) = 1
      stride(2) = 1
C
C     Write the stored data to the data set.
C
      status = sfwdata(sds_id, start, stride, edges, data)
C
C     Terminate access to the  data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.
C
      status = sfend(sd_id)

      end
