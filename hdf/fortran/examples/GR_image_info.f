      program image_info
      implicit none
C
C     Parameter declaration
C
      character*19 FILE_NAME
C
      parameter (FILE_NAME = 'General_RImages.hdf')
      integer DFACC_READ
      parameter (DFACC_READ = 1)
C
C     Function declaration
C
      integer hopen, hclose
      integer mgstart, mgselct, mgfinfo, mggiinf, mgendac, mgend 

C
C**** Variable declaration *******************************************
C
      integer status
      integer file_id, gr_id, ri_id
      integer n_rimages, n_file_attrs, ri_index
      integer n_comps, interlace_mode, n_attrs, data_type
      integer dim_sizes(2)
      character*10 type_string
      character*24 interlace_string
      character*64 name
C
C**** End of variable declaration ************************************
C
C
C     Open the HDF file for reading.
C
      file_id = hopen(FILE_NAME, DFACC_READ, 0)
C
C     Initialize the GR interface.
C
      gr_id = mgstart(file_id)
C
C     Determine the contents of the file.
C
      status = mgfinfo(gr_id, n_rimages, n_file_attrs)
C
C     For each image in the file, get and display image information.
C
      do 100 ri_index = 0, n_rimages-1
         ri_id = mgselct(gr_id, ri_index)
         name = ' '
         status = mggiinf(ri_id, name, n_comps, data_type,
     +                    interlace_mode, dim_sizes, n_attrs)
C
C     Map the number type and interlace mode into text strings for
C     output readability.
C
      if(data_type .eq. 4) then
         type_string = 'DFNT_CHAR8'
      else if(data_type .eq. 22) then
         type_string = 'DFNT_INT16'
      else
         type_string = 'Unknown'
      endif
      if (interlace_mode .eq. 0) then
          interlace_string = 'MFGR_INTERLACE_PIXEL'
      else if(interlace_mode .eq. 1) then
          interlace_string = 'MFGR_INTERLACE_LINE'
      else if(interlace_mode .eq. 2) then
          interlace_string = 'MFGR_INTERLACE_COMPONENT'
      else
         interlace_string = 'Unknown'
      endif
C
C     Display the image information for the current image.
C
      write(*,*) 'Image index: ', ri_index
      write(*,*) 'Image name: ', name 
      write(*,*) 'Number of components: ', n_comps
      write(*,*) 'Number type: ', type_string 
      write(*,*) 'Interlace mode: ', interlace_string
      write(*,*) 'Dimnesions: ', dim_sizes(1), dim_sizes(2)
      write(*,*) 'Number of image attributes: ', n_attrs
      write(*,*) 
C
C     Terminate access to the current raster image.
C
      status = mgendac(ri_id)
100   continue
C
C     Terminate access to the GR interface and close the HDF file.
      status = mgend(gr_id)
      status = hclose(file_id)
      end
