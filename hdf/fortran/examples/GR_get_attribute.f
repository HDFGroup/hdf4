      program  get_attribute
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
      character*17 RI_ATTR_NAME
C
      parameter (FILE_NAME    = 'General_RImages.hdf',
     +           RI_ATTR_NAME  = 'Image Attribute 2')
      integer DFACC_READ, DFNT_INT16, DFNT_CHAR8
      parameter (DFACC_READ   = 1,
     +           DFNT_CHAR8   = 4,
     +           DFNT_INT16   = 22)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgfinfo, mgatinf, mggcatt, mggnatt , mgfndat,
     +        mgselct, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer      status
      integer      file_id, gr_id, ri_id
      integer      f_att_index, ri_att_index, data_type, n_values 
      integer      n_rimages, n_file_attrs 
      integer*2    int_buf(10)
      character*17 attr_name
      character*80 char_buf
      integer      i
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Determine the number of attributes in the file. 
C
      status = mgfinfo(gr_id, n_rimages, n_file_attrs)
      if ((status .NE. -1) .AND. (n_file_attrs .GT. 0)) then

         do 10 f_att_index = 0, n_file_attrs-1
C
C        Get information about the current file attribute.
C 
         attr_name = ' '
         status = mgatinf(gr_id, f_att_index, attr_name, data_type,
     +                    n_values)
C
C        Check whether data type is DFNT_CHAR8 in order to use allocated buffer.
C
         if(data_type .NE. DFNT_CHAR8) then
            write(*,*) 
     +      'Unable to determine data type to use allocated buffer'
         else
C
C           Read and display the attribute values.
C
            status = mggcatt(gr_id, f_att_index, char_buf)
            write(*,*) 'Attribute ', attr_name, ' : ', 
     +                 char_buf(1:n_values)
         endif
10       continue

      endif

C
C     Select the second image in the file.
C 
      ri_id = mgselct(gr_id, 1) 
C
C     Find the image attribute named RI_ATTR_NAME. 
C
      ri_att_index = mgfndat(ri_id, RI_ATTR_NAME)
C
C     Get information about the attribute.
C
      status = mgatinf(ri_id, ri_att_index, attr_name, data_type,
     +                 n_values)
C      
C     Read and display attribute values.
C
      status = mggnatt(ri_id, ri_att_index, int_buf)
      write(*,*) 'Attributes :', (int_buf(i), i = 1, n_values)
C
C     Terminate access to the image and to the GR interface,
C     and close the HDF file.
C
      status = mgendac(ri_id)
      status = mgend(gr_id)
      status = hclose(file_id)
      end
