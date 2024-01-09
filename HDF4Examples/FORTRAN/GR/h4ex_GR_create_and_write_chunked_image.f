      program gr_chunking_example
      implicit none
C
C     Parameter declaraction
C
      character*14 FILE_NAME
      character*14 DATASET_NAME
      parameter (FILE_NAME = 'gr_chunked.hdf',
     .           DATASET_NAME = 'gzip_comp_data')
      integer   NCOMP, MFGR_INTERLACE_PIXEL
      parameter(NCOMP = 3, MFGR_INTERLACE_PIXEL = 0)
      integer DFACC_CREATE, DFACC_READ, DFACC_WRITE
      parameter (DFACC_CREATE = 4,
     .           DFACC_READ   = 1,
     .           DFACC_WRITE  = 2)
      integer DFNT_INT32
      parameter (DFNT_INT32   = 24)
      integer X_LENGTH, Y_LENGTH, X_CH_LENGTH, Y_CH_LENGTH
      parameter (X_LENGTH     = 6,
     .           Y_LENGTH     = 10,
     .           X_CH_LENGTH  = 3,
     .           Y_CH_LENGTH  = 2)
C
C     Compression parameters.
C
      integer  COMP_CODE_DEFLATE, DEFLATE_LEVEL
      parameter( COMP_CODE_DEFLATE = 4, DEFLATE_LEVEL = 6)
C
C     Function declaration.
C
      integer mgstart, mgcreat, mgendac, mgend
      integer mgwchnk, mgschnk
      integer hopen, hclose
C
C**** Variable declaration *************************************************
C
      integer ri_id, gr_id, file_id
      integer dims(2), start(2)
      integer status, il
      integer comp_prm(1), comp_type
C
C     Data buffers.
C
      integer*4 chunk11(NCOMP* X_CH_LENGTH*Y_CH_LENGTH)
      integer*4 chunk21(NCOMP* X_CH_LENGTH*Y_CH_LENGTH)
      integer*4 chunk52(NCOMP* X_CH_LENGTH*Y_CH_LENGTH)
C
C     Chunking dimension arrays
C
      integer ch_dims(2)
C
C**** End of variable declaration ******************************************
C
C
C     Data initialization
C
      data  chunk11 / 110, 111, 112, 120, 121, 122,
     .                130, 131, 132, 140, 141, 142,
     .                150, 151, 152, 160, 161, 162
     .              /,
     .      chunk21 /
     .                210, 211, 212, 220, 221, 222,
     .                230, 231, 232, 240, 241, 242,
     .                250, 251, 252, 260, 261, 262
     .              /,
     .      chunk52 /
     .                1010, 1011, 1012, 1020, 1021, 1022,
     .                1030, 1031, 1032, 1040, 1041, 1042,
     .                1050, 1051, 1052, 1060, 1061, 1062
     .              /
C
C     Define chunk dimensions.
C
      ch_dims(1) = Y_CH_LENGTH
      ch_dims(2) = X_CH_LENGTH
C
C     Create and open the file and initiate GR interface..
C
      file_id = hopen(FILE_NAME, DFACC_CREATE, 0)
      gr_id   = mgstart(file_id)
C
C     Define the number of components and dimensions of the image.
C
      il      = MFGR_INTERLACE_PIXEL
      dims(1) = X_LENGTH
      dims(2) = Y_LENGTH
C
C     Create GR dataset.
C
      ri_id = mgcreat(gr_id, DATASET_NAME, NCOMP, DFNT_INT32, il, dims)
C
C     Define chunked GR dataset using GZIP compression.
C
      comp_prm(1) =  DEFLATE_LEVEL
      comp_type = COMP_CODE_DEFLATE
      status = mgschnk (ri_id, ch_dims, comp_type, comp_prm)
C
C     Define the location of the first chunk and write the data.
C
      start(1) = 1
      start(2) = 1
      status = mgwchnk(ri_id, start, chunk11)
C
C     Define the location of the second chunk and write the data.
C
      start(1) = 2
      start(2) = 1
      status = mgwchnk(ri_id, start, chunk21)
C
C     Define the location of the third and write the data.
C
      start(1) = 5
      start(2) = 2
      status = mgwchnk(ri_id, start, chunk52)
C
C     Terminate access to the array.
C
      status = mgendac(ri_id)
C
C     Terminate access to the GR interface.
C
      status = mgend(gr_id)
C
C     Close the file.
C
      status = hclose(file_id)
      end
