      program  set_attribute
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*13 IMAGE_NAME
      character*16 F_ATT1_NAME
      character*16 F_ATT2_NAME
      character*17 RI_ATT1_NAME
      character*17 RI_ATT2_NAME
      character*32 F_ATT1_VAL
      character*33 F_ATT2_VAL
      integer      F_ATT1_N_VALUES
      integer      F_ATT2_N_VALUES
      character*35 RI_ATT1_VAL
      integer      RI_ATT1_N_VALUES
      integer      RI_ATT2_N_VALUES
C
      parameter (FILE_NAME    = 'General_RImages.hdf',
     +           IMAGE_NAME   = 'Image Array 2',
     +           F_ATT1_NAME  = 'File Attribute 1',
     +           F_ATT2_NAME  = 'File Attribute 2',
     +           RI_ATT1_NAME = 'Image Attribute 1',
     +           RI_ATT2_NAME = 'Image Attribute 2',
     +           F_ATT1_VAL   = 'Contents of First FILE Attribute',
     +           F_ATT2_VAL   = 'Contents of Second FILE Attribute',
     +           F_ATT1_N_VALUES = 32,
     +           F_ATT2_N_VALUES = 33,
     +           RI_ATT1_VAL = 'Contents of IMAGE''s First Attribute',
     +           RI_ATT1_N_VALUES = 35,
     +           RI_ATT2_N_VALUES = 6)
      integer DFACC_WRITE, DFNT_INT16, DFNT_CHAR8
      parameter (DFACC_WRITE  = 2,
     +           DFNT_CHAR8   = 4,
     +           DFNT_INT16   = 22)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgscatt, mgsnatt , mgn2ndx,
     +        mgselct, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer   status
      integer   file_id, gr_id, ri_id, ri_index
      integer*2 ri_attr_2(RI_ATT2_N_VALUES)
      integer   i

      do 10 i = 1, RI_ATT2_N_VALUES
         ri_attr_2(i) = i
10    continue 
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file.
C
      file_id = hopen(FILE_NAME, DFACC_WRITE, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Set two file attributes to the file with names, data type, numbers of
C     values, and values of attributes specified.
C
      status = mgscatt(gr_id, F_ATT1_NAME, DFNT_CHAR8, 
     +                 F_ATT1_N_VALUES, F_ATT1_VAL)
      status = mgscatt(gr_id, F_ATT2_NAME, DFNT_CHAR8, 
     +                 F_ATT2_N_VALUES, F_ATT2_VAL)
C
C     Obtain the index of the image named IMAGE_NAMR.
C
      ri_index = mgn2ndx(gr_id, IMAGE_NAME)
C
C     Obtain the identifier of this image. 
C
      ri_id = mgselct(gr_id, ri_index)
C
C     Set two attributes of the image with names, data types, number of
C     values, and values of the attributes specified. 
C
      status = mgscatt(ri_id, RI_ATT1_NAME, DFNT_CHAR8, 
     +                 RI_ATT1_N_VALUES, RI_ATT1_VAL) 
      status = mgsnatt(ri_id, RI_ATT2_NAME, DFNT_INT16, 
     +                 RI_ATT2_N_VALUES, ri_attr_2)
C
C     Terminate access to the image and to the GR interface,
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end
