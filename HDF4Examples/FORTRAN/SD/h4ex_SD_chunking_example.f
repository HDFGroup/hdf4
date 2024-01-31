      program  chunk_examples
      implicit none
C
C     Parameter declaration.
C
      character*14 FILE_NAME
      character*11 SDS_NAME
      integer      RANK
      parameter   (FILE_NAME = 'SDSchunked.hdf',
     +             SDS_NAME  = 'ChunkedData',
     +             RANK      = 2)
      integer      DFACC_CREATE, DFACC_READ, DFNT_INT16
      parameter   (DFACC_CREATE = 4,
     +             DFACC_READ   = 1,
     +             DFNT_INT16   = 22)
      integer      COMP_CODE_NONE
      parameter   (COMP_CODE_NONE = 0)
C
C     This example does not use compression.
C
C     To use RLE compression, declare:
C
C     integer      COMP_CODE_RLE
C     parameter   (COMP_CODE_RLE = 1)
C
C     To use NBIT compression, declare:
C
C     integer      COMP_CODE_NBIT
C     parameter   (COMP_CODE_NBIT = 2)
C
C     To use Skipping Huffman compression, declare:
C
C     integer      COMP_CODE_SKPHUFF
C     parameter   (COMP_CODE_SKPHUFF = 3)
C
C     To use GZIP compression, declare:
C
C     integer      COMP_CODE_DEFLATE
C     parameter   (COMP_CODE_DEFLATE = 4)
C
C
C     Function declaration.
C
      integer sfstart, sfcreate, sfendacc, sfend,
     +        sfselect, sfsfill, sfschnk, sfwchnk,
     +        sfrchnk, sfgichnk, sfwdata, sfrdata,
     +        sfscchnk
C
C**** Variable declaration *******************************************
C
      integer   sd_id, sds_id, sds_index, status          
      integer   dim_sizes(2), origin(2)
      integer   fill_value, maxcache, new_maxcache, flag
      integer   start(2), edges(2), stride(2)
      integer*2 all_data(4,9)
      integer*2 row(3), column(2)
      integer*2 chunk_out(2,3)
      integer*2 chunk1(2,3),
     +          chunk2(2,3),
     +          chunk3(2,3),
     +          chunk6(2,3)
      integer   i, j
C
C     Compression flag and parameters.
C
      integer comp_type, comp_flag, comp_prm(4)
C
C     Chunk's dimensions.
C
      integer dim_length(2), dim_length_out(2)
C
C     Initialize four chunks
C
      data chunk1 /6*1/
      data chunk2 /6*2/
      data chunk3 /6*3/
      data chunk6 /6*6/
C
C     Initialize row and column arrays.
C
      data row /3*4/
      data column /2*5/
C
C**** End of variable declaration ************************************
C
C
C     Define chunk's dimensions.
C
      dim_length(1) = 2 
      dim_length(2) = 3 
C 
C     Create the file and initialize SD interface.
C
      sd_id = sfstart(FILE_NAME, DFACC_CREATE)

C
C     Create 4x9 SDS
C
      dim_sizes(1) = 4 
      dim_sizes(2) = 9 
      sds_id = sfcreate(sd_id, SDS_NAME, DFNT_INT16,
     +                  RANK, dim_sizes)
C
C     Fill SDS array with the fill value.
C
      fill_value = 0
      status = sfsfill( sds_id, fill_value)
C
C     Create chunked SDS.
C
C     In this example we do not use compression.
C
C     To use RLE compression, initialize comp_type parameter
C     before the call to sfschnk function.
C              comp_type = COMP_CODE_RLE
C
C     To use NBIT, Skipping Huffman, or GZIP compression,
C     initialize comp_prm array and comp type parameter
C     before call to sfschnk function
C
C     NBIT:
C              comp_prm(1) = value_of(sign_ext)
C              comp_prm(2) = value_of(fill_one)
C              comp_prm(3) = value_of(start_bit)
C              comp_prm(4) = value_of(bit_len)
C              comp_type   = COMP_CODE_NBIT
C
C     Skipping Huffman:
C              comp_prm(1) = value_of(skp_size)
C              comp_type   = COMP_CODE_SKPHUFF
C
C     GZIP:
C              comp_prm(1) = value_of(deflate_level) 
C              comp_type   = COMP_CODE_DEFLATE
C       
C
      comp_type = COMP_CODE_NONE
      status = sfschnk(sds_id, dim_length, comp_type, comp_prm)
C
C     Set chunk cache to hold maximum 2 chunks.
C
      flag = 0
      maxcache = 2
      new_maxcache = sfscchnk(sds_id, maxcache, flag) 
C
C     Write chunks using SDwritechunk function.
C     Chunks can be written in any order.
C
C     Write chunk with the coordinates (1,1).
C
      origin(1) = 1
      origin(2) = 1
      status = sfwchnk(sds_id, origin, chunk1) 
C
C     Write chunk with the coordinates (1,2).
C
      origin(1) = 1 
      origin(2) = 2 
      status = sfwchnk(sds_id, origin, chunk3) 
C
C     Write chunk with the coordinates (2,1).
C
      origin(1) = 2 
      origin(2) = 1 
      status = sfwchnk(sds_id, origin, chunk2) 
C
C     Write chunk with the coordinates (2,3).
C
      origin(1) = 2 
      origin(2) = 3 
      status = sfwchnk(sds_id, origin, chunk6) 
C
C     Fill second row in the chunk with the coordinates (2,2).
C
      start(1) = 3
      start(2) = 3
      edges(1) = 1 
      edges(2) = 3 
      stride(1) = 1
      stride(2) = 1
      status = sfwdata(sds_id, start, stride, edges, row)
C
C     Fill second column in the chunk with the coordinates (1,3).
C
      start(1) = 0 
      start(2) = 7 
      edges(1) = 2 
      edges(2) = 1 
      stride(1) = 1
      stride(2) = 1
      status = sfwdata(sds_id, start, stride, edges, column)
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.  
C
      status = sfend(sd_id)
C
C     Reopen the file and access the first data set.
C
      sd_id = sfstart(FILE_NAME, DFACC_READ)
      sds_index = 0
      sds_id = sfselect(sd_id, sds_index)
C
C     Get information about the SDS.
C
      status = sfgichnk(sds_id, dim_length_out, comp_flag)
      if (comp_flag .eq. 0) then
         write(*,*) 'SDS is chunked'
      endif
      if (comp_flag .eq. 1) then
         write(*,*) 'SDS is chunked and compressed'
      endif
      if (comp_flag .eq. 2) then
         write(*,*) 'SDS is chunked and NBIT compressed'
      endif
      write(*,*) 'Chunks dimensions are ', dim_length_out(1),
     + '  x' ,dim_length_out(2)
C
C     Read the whole SDS using sfrdata function and display
C     what we have read. The following information will be displayed:
C
C
C             SDS is chunked
C             Chunks dimensions are   2  x  3
C 
C             1  1  1  3  3  3  0  5  0
C             1  1  1  3  3  3  0  5  0
C             2  2  2  0  0  0  6  6  6
C             2  2  2  4  4  4  6  6  6
C
      start(1) = 0
      start(2) = 0
      edges(1) = 4
      edges(2) = 9
      stride(1) = 1
      stride(2) = 1
      status = sfrdata(sds_id, start, stride, edges, all_data)
C
C     Display the SDS.
C
      write(*,*)
      do 10 i = 1,4
         write(*,*) (all_data(i,j), j=1,9)
10    continue     
C
C     Read chunks with the coordinates (2,2) and (1,3) and display.
C     The following information will be shown:
C
C             Chunk (2,2)
C
C               0  0  0
C               4  4  4
C 
C             Chunk (1,3)
C
C               0  5  0
C               0  5  0
C
      origin(1) = 2
      origin(2) = 2
      status = sfrchnk(sds_id, origin, chunk_out)
      write(*,*) 
      write(*,*) 'Chunk (2,2)'
      write(*,*) 
      do 20 i = 1,2
         write(*,*) (chunk_out(i,j), j=1,3)
20    continue
C
      origin(1) = 1 
      origin(2) = 3 
      status = sfrchnk(sds_id, origin, chunk_out)
      write(*,*) 
      write(*,*) 'Chunk (1,3)'
      write(*,*) 
      do 30 i = 1,2
         write(*,*) (chunk_out(i,j), j=1,3)
30    continue
C
C     Terminate access to the data set.
C
      status = sfendacc(sds_id)
C
C     Terminate access to the SD interface and close the file.   
C
      status = sfend(sd_id)
      end 
